/*-------------------------------------------------------------------------
 *
 * define.c
 *	  Support routines for various kinds of object creation.
 *
 *
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $PostgreSQL: pgsql/src/backend/commands/define.c,v 1.100 2007/01/05 22:19:26 momjian Exp $
 *
 * DESCRIPTION
 *	  The "DefineFoo" routines take the parse tree and pick out the
 *	  appropriate arguments/flags, passing the results to the
 *	  corresponding "FooDefine" routines (in src/catalog) that do
 *	  the actual catalog-munging.  These routines also verify permission
 *	  of the user to execute the command.
 *
 * NOTES
 *	  These things must be defined and committed in the following order:
 *		"create function":
 *				input/output, recv/send procedures
 *		"create type":
 *				type
 *		"create operator":
 *				operators
 *
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <ctype.h>
#include <math.h>

#include "catalog/namespace.h"
#include "commands/defrem.h"
#include "nodes/makefuncs.h"
#include "parser/parse_type.h"
#include "parser/scansup.h"
#include "utils/int8.h"


/*
 * Translate the input language name to lower case, and truncate if needed.
 *
 * Returns a palloc'd string
 */
char *
case_translate_language_name(const char *input)
{
	return downcase_truncate_identifier(input, strlen(input), false);
}


/*
 * Extract a string value (otherwise uninterpreted) from a DefElem.
 */
char *
defGetString(DefElem *def, bool *need_free)
{
	if (def->arg == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("%s requires a parameter",
						def->defname)));
	Assert(NULL != need_free);
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			{
				char	   *str = palloc(32);

				snprintf(str, 32, "%ld", (long) intVal(def->arg));
				*need_free = true;
				return str;
			}
		case T_Float:

			/*
			 * T_Float values are kept in string form, so this type cheat
			 * works (and doesn't risk losing precision)
			 */
			*need_free = false;
			return strVal(def->arg);
		case T_String:
			*need_free = false;
			return strVal(def->arg);
		case T_TypeName:
			*need_free = true;
			return TypeNameToString((TypeName *) def->arg);
		case T_List:
			*need_free = true;
			return NameListToString((List *) def->arg);
		default:
			elog(ERROR, "unrecognized node type: %d", (int) nodeTag(def->arg));
	}
	return NULL;				/* keep compiler quiet */
}

/*
 * Extract a numeric value (actually double) from a DefElem.
 */
double
defGetNumeric(DefElem *def)
{
	if (def->arg == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("%s requires a numeric value",
						def->defname)));
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			return (double) intVal(def->arg);
		case T_Float:
			return floatVal(def->arg);
		default:
			ereport(ERROR,
					(errcode(ERRCODE_SYNTAX_ERROR),
					 errmsg("%s requires a numeric value",
							def->defname)));
	}
	return 0;					/* keep compiler quiet */
}

/*
 * Extract a boolean value from a DefElem.
 */
bool
defGetBoolean(DefElem *def)
{
	/*
	 * If no parameter given, assume "true" is meant.
	 */
	if (def->arg == NULL)
		return true;

	/*
	 * Allow 0, 1, "true", "false"
	 */
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			switch (intVal(def->arg))
			{
				case 0:
					return false;
				case 1:
					return true;
				default:
					/* otherwise, error out below */
					break;
			}
			break;
		default:
			{
				bool need_free_value = false;
				char *sval = defGetString(def, &need_free_value);
				bool result = false;
				if (pg_strcasecmp(sval, "true") == 0)
				{
					result = true;
				}
				if (pg_strcasecmp(sval, "false") == 0)
				{
					result = false;
				}
				if (need_free_value)
				{
					pfree(sval);
					sval = NULL;
				}

				AssertImply(need_free_value, NULL == sval);

				return result;
			}
			break;
	}
	ereport(ERROR,
			(errcode(ERRCODE_SYNTAX_ERROR),
			 errmsg("%s requires a Boolean value",
					def->defname)));
	return false;				/* keep compiler quiet */
}

/*
 * Extract an int64 value from a DefElem.
 */
int64
defGetInt64(DefElem *def)
{
	if (def->arg == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("%s requires a numeric value",
						def->defname)));
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			return (int64) intVal(def->arg);
		case T_Float:

			/*
			 * Values too large for int4 will be represented as Float
			 * constants by the lexer.	Accept these if they are valid int8
			 * strings.
			 */
			return DatumGetInt64(DirectFunctionCall1(int8in,
										 CStringGetDatum(strVal(def->arg))));
		default:
			ereport(ERROR,
					(errcode(ERRCODE_SYNTAX_ERROR),
					 errmsg("%s requires a numeric value",
							def->defname)));
	}
	return 0;					/* keep compiler quiet */
}

/*
 * Extract a possibly-qualified name (as a List of Strings) from a DefElem.
 */
List *
defGetQualifiedName(DefElem *def)
{
	if (def->arg == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("%s requires a parameter",
						def->defname)));
	switch (nodeTag(def->arg))
	{
		case T_TypeName:
			return ((TypeName *) def->arg)->names;
		case T_List:
			return (List *) def->arg;
		case T_String:
			/* Allow quoted name for backwards compatibility */
			return list_make1(def->arg);
		default:
			ereport(ERROR,
					(errcode(ERRCODE_SYNTAX_ERROR),
					 errmsg("argument of %s must be a name",
							def->defname)));
	}
	return NIL;					/* keep compiler quiet */
}

/*
 * Extract a TypeName from a DefElem.
 *
 * Note: we do not accept a List arg here, because the parser will only
 * return a bare List when the name looks like an operator name.
 */
TypeName *
defGetTypeName(DefElem *def)
{
	if (def->arg == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("%s requires a parameter",
						def->defname)));
	switch (nodeTag(def->arg))
	{
		case T_TypeName:
			return (TypeName *) def->arg;
		case T_String:
			/* Allow quoted typename for backwards compatibility */
			return makeTypeNameFromNameList(list_make1(def->arg));
		default:
			ereport(ERROR,
					(errcode(ERRCODE_SYNTAX_ERROR),
					 errmsg("argument of %s must be a type name",
							def->defname)));
	}
	return NULL;				/* keep compiler quiet */
}

/*
 * Extract a type length indicator (either absolute bytes, or
 * -1 for "variable") from a DefElem.
 */
int
defGetTypeLength(DefElem *def)
{
	if (def->arg == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("%s requires a parameter",
						def->defname)));
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			return intVal(def->arg);
		case T_Float:
			ereport(ERROR,
					(errcode(ERRCODE_SYNTAX_ERROR),
					 errmsg("%s requires an integer value",
							def->defname)));
			break;
		case T_String:
			if (pg_strcasecmp(strVal(def->arg), "variable") == 0)
				return -1;		/* variable length */
			break;
		case T_TypeName:
			/* cope if grammar chooses to believe "variable" is a typename */
			if (pg_strcasecmp(TypeNameToString((TypeName *) def->arg),
							  "variable") == 0)
				return -1;		/* variable length */
			break;
		case T_List:
			/* must be an operator name */
			break;
		default:
			elog(ERROR, "unrecognized node type: %d", (int) nodeTag(def->arg));
	}
	bool need_free_value = false;
	ereport(ERROR,
			(errcode(ERRCODE_SYNTAX_ERROR),
			 errmsg("invalid argument for %s: \"%s\"",
					def->defname, defGetString(def, &need_free_value))));
	return 0;					/* keep compiler quiet */
}

/*
 * Create a DefElem setting "oids" to the specified value.
 */
DefElem *
defWithOids(bool value)
{
	DefElem    *f = makeNode(DefElem);

	f->defname = "oids";
	f->arg = (Node *) makeInteger(value);
	return f;
}
