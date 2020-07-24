/*
 * brin_ao_tuple.h
 *		Declarations for dealing with BRIN_AO-specific tuples.
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/include/access/brin_ao_tuple.h
 */
#ifndef BRIN_AO_TUPLE_H
#define BRIN_AO_TUPLE_H

#include "access/brin_ao_internal.h"
#include "access/tupdesc.h"


/*
 * A BRIN_AO index stores one index tuple per page range.  Each index tuple
 * has one Brin_AoValues struct for each indexed column; in turn, each Brin_AoValues
 * has (besides the null flags) an array of Datum whose size is determined by
 * the opclass.
 */
typedef struct Brin_AoValues
{
	AttrNumber	bv_attno;		/* index attribute number */
	bool		bv_hasnulls;	/* are there any nulls in the page range? */
	bool		bv_allnulls;	/* are all values nulls in the page range? */
	Datum	   *bv_values;		/* current accumulated values */
} Brin_AoValues;

/*
 * This struct is used to represent an in-memory index tuple.  The values can
 * only be meaningfully decoded with an appropriate Brin_AoDesc.
 */
typedef struct Brin_AoMemTuple
{
	bool		bt_placeholder; /* this is a placeholder tuple */
	BlockNumber bt_blkno;		/* heap blkno that the tuple is for */
	MemoryContext bt_context;	/* memcxt holding the bt_columns values */
	/* output arrays for brin_ao_deform_tuple: */
	Datum	   *bt_values;		/* values array */
	bool	   *bt_allnulls;	/* allnulls array */
	bool	   *bt_hasnulls;	/* hasnulls array */
	/* not an output array, but must be last */
	Brin_AoValues	bt_columns[FLEXIBLE_ARRAY_MEMBER];
} Brin_AoMemTuple;

/*
 * An on-disk BRIN_AO tuple.  This is possibly followed by a nulls bitmask, with
 * room for 2 null bits (two bits for each indexed column); an opclass-defined
 * number of Datum values for each column follow.
 */
typedef struct Brin_AoTuple
{
	/* heap block number that the tuple is for */
	BlockNumber bt_blkno;

	/* ---------------
	 * bt_info is laid out in the following fashion:
	 *
	 * 7th (high) bit: has nulls
	 * 6th bit: is placeholder tuple
	 * 5th bit: unused
	 * 4-0 bit: offset of data
	 * ---------------
	 */
	uint8		bt_info;
} Brin_AoTuple;

#define SizeOfBrin_AoTuple (offsetof(Brin_AoTuple, bt_info) + sizeof(uint8))

/*
 * bt_info manipulation macros
 */
#define BRIN_AO_OFFSET_MASK		0x1F
/* bit 0x20 is not used at present */
#define BRIN_AO_PLACEHOLDER_MASK	0x40
#define BRIN_AO_NULLS_MASK			0x80

#define Brin_AoTupleDataOffset(tup)	((Size) (((Brin_AoTuple *) (tup))->bt_info & BRIN_AO_OFFSET_MASK))
#define Brin_AoTupleHasNulls(tup)	(((((Brin_AoTuple *) (tup))->bt_info & BRIN_AO_NULLS_MASK)) != 0)
#define Brin_AoTupleIsPlaceholder(tup) (((((Brin_AoTuple *) (tup))->bt_info & BRIN_AO_PLACEHOLDER_MASK)) != 0)


extern Brin_AoTuple *brin_ao_form_tuple(Brin_AoDesc *brdesc, BlockNumber blkno,
								  Brin_AoMemTuple *tuple, Size *size);
extern Brin_AoTuple *brin_ao_form_placeholder_tuple(Brin_AoDesc *brdesc,
											  BlockNumber blkno, Size *size);
extern void brin_ao_free_tuple(Brin_AoTuple *tuple);
extern Brin_AoTuple *brin_ao_copy_tuple(Brin_AoTuple *tuple, Size len,
								  Brin_AoTuple *dest, Size *destsz);
extern bool brin_ao_tuples_equal(const Brin_AoTuple *a, Size alen,
							  const Brin_AoTuple *b, Size blen);

extern Brin_AoMemTuple *brin_ao_new_memtuple(Brin_AoDesc *brdesc);
extern Brin_AoMemTuple *brin_ao_memtuple_initialize(Brin_AoMemTuple *dtuple,
											  Brin_AoDesc *brdesc);
extern Brin_AoMemTuple *brin_ao_deform_tuple(Brin_AoDesc *brdesc,
									   Brin_AoTuple *tuple, Brin_AoMemTuple *dMemtuple);

#endif							/* BRIN_AO_TUPLE_H */
