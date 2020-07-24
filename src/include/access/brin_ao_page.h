/*
 * brin_ao_page.h
 *		Prototypes and definitions for BRIN_AO page layouts
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *		src/include/access/brin_ao_page.h
 *
 * NOTES
 *
 * These structs should really be private to specific BRIN_AO files, but it's
 * useful to have them here so that they can be used by pageinspect and similar
 * tools.
 */
#ifndef BRIN_AO_PAGE_H
#define BRIN_AO_PAGE_H

#include "storage/block.h"
#include "storage/itemptr.h"

/*
 * Special area of BRIN_AO pages.
 *
 * We define it in this odd way so that it always occupies the last
 * MAXALIGN-sized element of each page.
 */
typedef struct Brin_AoSpecialSpace
{
	uint16		vector[MAXALIGN(1) / sizeof(uint16)];
} Brin_AoSpecialSpace;

/*
 * Make the page type be the last half-word in the page, for consumption by
 * pg_filedump and similar utilities.  We don't really care much about the
 * position of the "flags" half-word, but it's simpler to apply a consistent
 * rule to both.
 *
 * See comments above GinPageOpaqueData.
 */
#define Brin_AoPageType(page)		\
	(((Brin_AoSpecialSpace *)		\
	  PageGetSpecialPointer(page))->vector[MAXALIGN(1) / sizeof(uint16) - 1])

#define Brin_AoPageFlags(page)		\
	(((Brin_AoSpecialSpace *)		\
	  PageGetSpecialPointer(page))->vector[MAXALIGN(1) / sizeof(uint16) - 2])

/* special space on all BRIN_AO pages stores a "type" identifier */
#define		BRIN_AO_PAGETYPE_META			0xF091
#define		BRIN_AO_PAGETYPE_REVMAP		0xF092
#define		BRIN_AO_PAGETYPE_REGULAR		0xF093
#define		BRIN_AO_PAGETYPE_UPPER			0xF094

#define BRIN_AO_IS_META_PAGE(page) (Brin_AoPageType(page) == BRIN_AO_PAGETYPE_META)
#define BRIN_AO_IS_REVMAP_PAGE(page) (Brin_AoPageType(page) == BRIN_AO_PAGETYPE_REVMAP)
#define BRIN_AO_IS_REGULAR_PAGE(page) (Brin_AoPageType(page) == BRIN_AO_PAGETYPE_REGULAR)
#define BRIN_AO_IS_UPPER_PAGE(page) (Brin_AoPageType(page) == BRIN_AO_PAGETYPE_UPPER)

/* flags for Brin_AoSpecialSpace */
#define		BRIN_AO_EVACUATE_PAGE			(1 << 0)


/* Metapage definitions */
typedef struct Brin_AoMetaPageData
{
	uint32		brin_aoMagic;
	uint32		brin_aoVersion;
	BlockNumber pagesPerRange;
	BlockNumber lastRevmapPage;
	bool		isAo;
} Brin_AoMetaPageData;

#define BRIN_AO_CURRENT_VERSION		1
#define BRIN_AO_META_MAGIC			0xA8109CFA

#define BRIN_AO_METAPAGE_BLKNO		0

/* Definitions for revmap pages */
typedef struct Revmap_AOContents
{
	/*
	 * This array will fill all available space on the page.  It should be
	 * declared [FLEXIBLE_ARRAY_MEMBER], but for some reason you can't do that
	 * in an otherwise-empty struct.
	 */
	ItemPointerData rm_tids[1];
} Revmap_AOContents;

typedef struct Revmap_AOUpperBlockContents
{
	BlockNumber rm_blocks[1];
} Revmap_AOUpperBlockContents;

#define REVMAP_AO_CONTENT_SIZE \
	(BLCKSZ - MAXALIGN(SizeOfPageHeaderData) - \
	 offsetof(Revmap_AOContents, rm_tids) - \
	 MAXALIGN(sizeof(Brin_AoSpecialSpace)))
/* max num of items in the array */
#define REVMAP_AO_PAGE_MAXITEMS \
	(REVMAP_AO_CONTENT_SIZE / sizeof(ItemPointerData))

#define REVMAP_AO_UPPER_PAGE_MAXITEMS \
	(REVMAP_AO_CONTENT_SIZE/ sizeof(BlockNumber))

#define REVMAP_AO_INDEX_PAGE_NUM \
	(65536 / REVMAP_AO_PAGE_MAXITEMS) * 65536

#endif							/* BRIN_AO_PAGE_H */
