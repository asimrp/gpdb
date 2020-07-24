/*
 * brin_ao_revmap.h
 *		Prototypes for BRIN_AO reverse range maps
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *		src/include/access/brin_ao_revmap.h
 */

#ifndef BRIN_AO_REVMAP_H
#define BRIN_AO_REVMAP_H

#include "access/brin_ao_tuple.h"
#include "storage/block.h"
#include "storage/buf.h"
#include "storage/itemptr.h"
#include "storage/off.h"
#include "utils/relcache.h"
#include "utils/snapshot.h"


/*
 * In revmap pages, each item stores an ItemPointerData.  These defines let one
 * find the logical revmap page number and index number of the revmap item for
 * the given heap block number.
 */
#define HEAPBLK_TO_REVMAP_AO_BLK(pagesPerRange, heapBlk) \
	((heapBlk / pagesPerRange) / REVMAP_AO_PAGE_MAXITEMS)
#define HEAPBLK_TO_REVMAP_AO_INDEX(pagesPerRange, heapBlk) \
	((heapBlk / pagesPerRange) % REVMAP_AO_PAGE_MAXITEMS)
#define HEAPBLK_TO_REVMAP_AO_UPPER_BLK(pagesPerRange, heapBlk) \
	(HEAPBLK_TO_REVMAP_AO_BLK(pagesPerRange, heapBlk) / REVMAP_AO_UPPER_PAGE_MAXITEMS)
#define HEAPBLK_TO_REVMAP_AO_UPPER_IDX(pagesPerRange, heapBlk) \
	(HEAPBLK_TO_REVMAP_AO_BLK(pagesPerRange, heapBlk) % REVMAP_AO_UPPER_PAGE_MAXITEMS)
#define REVMAP_AO_UPPER_PAGE_TOTAL_NUM(pagesPerRange) \
	(HEAPBLK_TO_REVMAP_AO_UPPER_BLK(pagesPerRange, MaxBlockNumber) + 1)

/* struct definition lives in brin_ao_revmap.c */
typedef struct Brin_AoRevmap Brin_AoRevmap;

extern Brin_AoRevmap *brin_aoRevmapInitialize(Relation idxrel,
										BlockNumber *pagesPerRange, Snapshot snapshot);
extern void brin_aoRevmapTerminate(Brin_AoRevmap *revmap);

extern void brin_aoRevmapExtend(Brin_AoRevmap *revmap,
							 BlockNumber heapBlk);
extern Buffer brin_aoLockRevmapPageForUpdate(Brin_AoRevmap *revmap,
										  BlockNumber heapBlk);
extern void brin_aoSetHeapBlockItemptr(Buffer rmbuf, BlockNumber pagesPerRange,
									BlockNumber heapBlk, ItemPointerData tid);
extern Brin_AoTuple *brin_aoGetTupleForHeapBlock(Brin_AoRevmap *revmap,
										   BlockNumber heapBlk, Buffer *buf, OffsetNumber *off,
										   Size *size, int mode, Snapshot snapshot);
extern bool brin_aoRevmapDesummarizeRange(Relation idxrel, BlockNumber heapBlk);

extern void brin_ao_init_upper_pages(Relation index, BlockNumber pagesPerRange);
extern BlockNumber heapBlockGetCurrentAosegStart(BlockNumber heapBlk);
extern BlockNumber segnoGetCurrentAosegStart(int segno);

#endif							/* BRIN_AO_REVMAP_H */
