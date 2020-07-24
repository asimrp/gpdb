/*
 * brin_ao_internal.h
 *		internal declarations for BRIN_AO indexes
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *		src/include/access/brin_ao_internal.h
 */
#ifndef BRIN_AO_INTERNAL_H
#define BRIN_AO_INTERNAL_H

#include "access/amapi.h"
#include "storage/bufpage.h"
#include "utils/typcache.h"


/*
 * A Brin_AoDesc is a struct designed to enable decoding a BRIN_AO tuple from the
 * on-disk format to an in-memory tuple and vice-versa.
 */

/* struct returned by "OpcInfo" amproc */
typedef struct Brin_AoOpcInfo
{
	/* Number of columns stored in an index column of this opclass */
	uint16		oi_nstored;

	/* Opaque pointer for the opclass' private use */
	void	   *oi_opaque;

	/* Type cache entries of the stored columns */
	TypeCacheEntry *oi_typcache[FLEXIBLE_ARRAY_MEMBER];
} Brin_AoOpcInfo;

/* the size of a Brin_AoOpcInfo for the given number of columns */
#define SizeofBrin_AoOpcInfo(ncols) \
	(offsetof(Brin_AoOpcInfo, oi_typcache) + sizeof(TypeCacheEntry *) * ncols)

typedef struct Brin_AoDesc
{
	/* Containing memory context */
	MemoryContext bd_context;

	/* the index relation itself */
	Relation	bd_index;

	/* tuple descriptor of the index relation */
	TupleDesc	bd_tupdesc;

	/* cached copy for on-disk tuples; generated at first use */
	TupleDesc	bd_disktdesc;

	/* total number of Datum entries that are stored on-disk for all columns */
	int			bd_totalstored;

	/* per-column info; bd_tupdesc->natts entries long */
	Brin_AoOpcInfo *bd_info[FLEXIBLE_ARRAY_MEMBER];
} Brin_AoDesc;

/*
 * Globally-known function support numbers for BRIN_AO indexes.  Individual
 * opclasses can define more function support numbers, which must fall into
 * BRIN_AO_FIRST_OPTIONAL_PROCNUM .. BRIN_AO_LAST_OPTIONAL_PROCNUM.
 */
#define BRIN_AO_PROCNUM_OPCINFO		1
#define BRIN_AO_PROCNUM_ADDVALUE		2
#define BRIN_AO_PROCNUM_CONSISTENT		3
#define BRIN_AO_PROCNUM_UNION			4
#define BRIN_AO_MANDATORY_NPROCS		4
/* procedure numbers up to 10 are reserved for BRIN_AO future expansion */
#define BRIN_AO_FIRST_OPTIONAL_PROCNUM 11
#define BRIN_AO_LAST_OPTIONAL_PROCNUM	15

#undef BRIN_AO_DEBUG

#ifdef BRIN_AO_DEBUG
#define BRIN_AO_elog(args)			elog args
#else
#define BRIN_AO_elog(args)			((void) 0)
#endif

/* brin_ao.c */
extern Brin_AoDesc *brin_ao_build_desc(Relation rel);
extern void brin_ao_free_desc(Brin_AoDesc *bdesc);
extern IndexBuildResult *brin_aobuild(Relation heap, Relation index,
								   struct IndexInfo *indexInfo);
extern void brin_aobuildempty(Relation index);
extern bool brin_aoinsert(Relation idxRel, Datum *values, bool *nulls,
					   ItemPointer heaptid, Relation heapRel,
					   IndexUniqueCheck checkUnique,
					   struct IndexInfo *indexInfo);
extern IndexScanDesc brin_aobeginscan(Relation r, int nkeys, int norderbys);
extern Node *brin_aogetbitmap(IndexScanDesc scan, Node *tbm);
extern void brin_aorescan(IndexScanDesc scan, ScanKey scankey, int nscankeys,
					   ScanKey orderbys, int norderbys);
extern void brin_aoendscan(IndexScanDesc scan);
extern IndexBulkDeleteResult *brin_aobulkdelete(IndexVacuumInfo *info,
											 IndexBulkDeleteResult *stats,
											 IndexBulkDeleteCallback callback,
											 void *callback_state);
extern IndexBulkDeleteResult *brin_aovacuumcleanup(IndexVacuumInfo *info,
												IndexBulkDeleteResult *stats);
extern bytea *brin_aooptions(Datum reloptions, bool validate);

/* brin_ao_validate.c */
extern bool brin_aovalidate(Oid opclassoid);

#endif							/* BRIN_AO_INTERNAL_H */
