/*-------------------------------------------------------------------------
 *
 * brin_ao_xlog.h
 *	  POSTGRES BRIN_AO access XLOG definitions.
 *
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/access/brin_ao_xlog.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef BRIN_AO_XLOG_H
#define BRIN_AO_XLOG_H

#include "access/xlogreader.h"
#include "lib/stringinfo.h"
#include "storage/bufpage.h"
#include "storage/itemptr.h"
#include "storage/relfilenode.h"
#include "utils/relcache.h"


/*
 * WAL record definitions for BRIN_AO's WAL operations
 *
 * XLOG allows to store some information in high 4 bits of log
 * record xl_info field.
 */
#define XLOG_BRIN_AO_CREATE_INDEX		0x00
#define XLOG_BRIN_AO_INSERT			0x10
#define XLOG_BRIN_AO_UPDATE			0x20
#define XLOG_BRIN_AO_SAMEPAGE_UPDATE	0x30
#define XLOG_BRIN_AO_REVMAP_EXTEND		0x40
#define XLOG_BRIN_AO_DESUMMARIZE		0x50
#define XLOG_BRIN_AO_REVMAP_INIT_UPPER_BLK	0x60
#define XLOG_BRIN_AO_REVMAP_EXTEND_UPPER	0x70

#define XLOG_BRIN_AO_OPMASK			0x70

/*
 * When we insert the first item on a new page, we restore the entire page in
 * redo.
 */
#define XLOG_BRIN_AO_INIT_PAGE		0x80

/*
 * This is what we need to know about a BRIN_AO index create.
 *
 * Backup block 0: metapage
 */
typedef struct xl_brin_ao_createidx
{
	BlockNumber pagesPerRange;
	uint16		version;
	bool 		isAo;
} xl_brin_ao_createidx;
#define SizeOfBrin_AoCreateIdx (offsetof(xl_brin_ao_createidx, isAo) + sizeof(bool))


typedef struct xl_brin_ao_createupperblk
{
	BlockNumber targetBlk;
} xl_brin_ao_createupperblk;
#define SizeOfBrin_AoCreateUpperBlk (offsetof(xl_brin_ao_createupperblk, targetBlk) \
								  + sizeof(BlockNumber))

/*
 * This is what we need to know about a BRIN_AO tuple insert
 *
 * Backup block 0: main page, block data is the new Brin_AoTuple.
 * Backup block 1: revmap page
 */
typedef struct xl_brin_ao_insert
{
	BlockNumber heapBlk;

	/* extra information needed to update the revmap */
	BlockNumber pagesPerRange;

	/* offset number in the main page to insert the tuple to. */
	OffsetNumber offnum;
} xl_brin_ao_insert;

#define SizeOfBrin_AoInsert	(offsetof(xl_brin_ao_insert, offnum) + sizeof(OffsetNumber))

/*
 * A cross-page update is the same as an insert, but also stores information
 * about the old tuple.
 *
 * Like in xlog_brin_ao_update:
 * Backup block 0: new page, block data includes the new Brin_AoTuple.
 * Backup block 1: revmap page
 *
 * And in addition:
 * Backup block 2: old page
 */
typedef struct xl_brin_ao_update
{
	/* offset number of old tuple on old page */
	OffsetNumber oldOffnum;

	xl_brin_ao_insert insert;
} xl_brin_ao_update;

#define SizeOfBrin_AoUpdate	(offsetof(xl_brin_ao_update, insert) + SizeOfBrin_AoInsert)

/*
 * This is what we need to know about a BRIN_AO tuple samepage update
 *
 * Backup block 0: updated page, with new Brin_AoTuple as block data
 */
typedef struct xl_brin_ao_samepage_update
{
	OffsetNumber offnum;
} xl_brin_ao_samepage_update;

#define SizeOfBrin_AoSamepageUpdate		(sizeof(OffsetNumber))

/*
 * This is what we need to know about a revmap extension
 *
 * Backup block 0: metapage
 * Backup block 1: new revmap page
 */
typedef struct xl_brin_ao_revmap_extend
{
	/*
	 * XXX: This is actually redundant - the block number is stored as part of
	 * backup block 1.
	 */
	BlockNumber targetBlk;
} xl_brin_ao_revmap_extend;
#define SizeOfBrin_AoRevmapExtend	(offsetof(xl_brin_ao_revmap_extend, targetBlk) + \
								 sizeof(BlockNumber))

/*
 * This is what we need to know about a range de-summarization
 *
 * Backup block 0: revmap page
 * Backup block 1: regular page
 */
typedef struct xl_brin_ao_desummarize
{
	BlockNumber pagesPerRange;
	/* page number location to set to invalid */
	BlockNumber heapBlk;
	/* offset of item to delete in regular index page */
	OffsetNumber regOffset;
} xl_brin_ao_desummarize;

#define SizeOfBrin_AoDesummarize	(offsetof(xl_brin_ao_desummarize, regOffset) + \
								 sizeof(OffsetNumber))


typedef struct xl_brin_ao_revmap_extend_upper
{
	BlockNumber heapBlk;

	/* extra information needed to update the revmap */
	BlockNumber pagesPerRange;
	BlockNumber revmapBlk;
} xl_brin_ao_revmap_extend_upper;
#define SizeOfBrin_AoRevmapExtendUpper	(offsetof(xl_brin_ao_revmap_extend_upper, pagesPerRange) + \
									 sizeof(BlockNumber))



extern void brin_ao_redo(XLogReaderState *record);
extern void brin_ao_desc(StringInfo buf, XLogReaderState *record);
extern const char *brin_ao_identify(uint8 info);
extern void brin_ao_mask(char *pagedata, BlockNumber blkno);

#endif							/* BRIN_AO_XLOG_H */
