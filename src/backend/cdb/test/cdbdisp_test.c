#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmockery.h"

#include "../cdbdisp.c"

/*
 * Mocked object initializations required for dispatchPlan.
 */
void
_init_cdbdisp_dispatchPlan(QueryDesc *queryDesc)
{
	queryDesc->estate = (struct EState *)palloc0(sizeof(struct EState));
	queryDesc->estate->es_sliceTable = (struct SliceTable *)
                                            palloc0(sizeof(struct SliceTable));
	queryDesc->operation = CMD_NOTHING;
	queryDesc->plannedstmt = (PlannedStmt *)palloc0(sizeof(PlannedStmt));

	will_be_called(clear_relsize_cache);

	expect_any(RootSliceIndex, estate);
	will_return(RootSliceIndex,0);
}

/*
 * Tests that cdbdisp_dispatchPlan handles a plan size overflow
 * when splan_len_uncompressed * num_slices.
 */
void
test__cdbdisp_dispatchPlan__Overflow_plan_size_in_kb(void **state)
{
	bool		success = false;

	struct CdbDispatcherState *ds = (struct CdbDispatcherState *)
		palloc0(sizeof(struct CdbDispatcherState));

	struct QueryDesc *queryDesc = (struct QueryDesc *)
		palloc0(sizeof(QueryDesc));

	_init_cdbdisp_dispatchPlan(queryDesc);

	/* Set max plan to a value that will require handling INT32
	 * overflow of the current plan size */
	gp_max_plan_size = INT_MAX;

	queryDesc->plannedstmt->planTree = (struct Plan *)palloc0(sizeof(struct Plan));

	/* Set num_slices and uncompressed_size to be INT_MAX-1 to force overflow */
	queryDesc->plannedstmt->planTree->nMotionNodes = INT_MAX-1;

	expect_any(serializeNode, node);
	expect_any(serializeNode, size);
	expect_any(serializeNode, uncompressed_size_out);
	will_assign_value(serializeNode, uncompressed_size_out, INT_MAX-1);
	will_return(serializeNode, NULL);

	PG_TRY();
	{
		cdbdisp_dispatchPlan(queryDesc, true, true, ds);
	}
	PG_CATCH();
	{
		/* Verify that we get the correct error (limit exceeded) */

		/* CopyErrorData() requires us to get out of ErrorContext */
		CurrentMemoryContext = TopMemoryContext;

		ErrorData *edata = CopyErrorData();

		StringInfo message = makeStringInfo();
		appendStringInfo(message,
						 "Query plan size limit exceeded, current size: "  UINT64_FORMAT "KB, max allowed size: %dKB",
						 ((INT_MAX-1)*(INT_MAX-1)/(uint64)1024), INT_MAX);

		if (edata->elevel == ERROR &&
			strncmp(edata->message, message->data, message->len))
		{
			success = true;
		}

	}
	PG_END_TRY();

	assert_true(success);
}

int
main(int argc, char* argv[])
{
	cmockery_parse_arguments(argc, argv);

	const UnitTest tests[] = {
		unit_test(test__cdbdisp_dispatchPlan__Overflow_plan_size_in_kb)
	};

	/* There are assertions in dispatch code for this */
	Gp_role = GP_ROLE_DISPATCH;
	MemoryContextInit();

	return run_tests(tests);
}
