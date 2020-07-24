/*
 * brin_ao_xlog.c
 *		XLog replay routines for BRIN_AO indexes
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/access/brin_ao/brin_ao_xlog.c
 */
#include "postgres.h"

#include "access/brin_ao_page.h"
#include "access/brin_ao_pageops.h"
#include "access/brin_ao_revmap.h"
#include "access/brin_ao_xlog.h"
#include "access/bufmask.h"
#include "access/xlogutils.h"


/*
 * xlog replay routines
 */
static void
brin_ao_xlog_createidx(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_createidx *xlrec = (xl_brin_ao_createidx *) XLogRecGetData(record);
	Buffer		buf;
	Page		page;

	/* create the index' metapage */
	buf = XLogInitBufferForRedo(record, 0);
	Assert(BufferIsValid(buf));
	page = (Page) BufferGetPage(buf);
	brin_ao_metapage_init(page, xlrec->pagesPerRange, xlrec->version, xlrec->isAo);
	PageSetLSN(page, lsn);
	MarkBufferDirty(buf);
	UnlockReleaseBuffer(buf);
}

static void
brin_ao_xlog_revmap_init_upper_blk(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_createupperblk *xlrec = (xl_brin_ao_createupperblk *) XLogRecGetData(record);
	Buffer		buf;
	Page		page;
	XLogRedoAction action;
	Buffer		metabuf;

	/* Update the metapage */
	action = XLogReadBufferForRedo(record, 0, &metabuf);
	if (action == BLK_NEEDS_REDO)
	{
		Page		metapg;
		Brin_AoMetaPageData *metadata;

		metapg = BufferGetPage(metabuf);
		metadata = (Brin_AoMetaPageData *) PageGetContents(metapg);

		Assert(metadata->lastRevmapPage == xlrec->targetBlk - 1);
		metadata->lastRevmapPage = xlrec->targetBlk;

		PageSetLSN(metapg, lsn);
		MarkBufferDirty(metabuf);
	}

	/* create upper blk */
	buf = XLogInitBufferForRedo(record, 1);
	page = (Page) BufferGetPage(buf);
	brin_ao_page_init(page, BRIN_AO_PAGETYPE_UPPER);

	PageSetLSN(page, lsn);
	MarkBufferDirty(buf);

	UnlockReleaseBuffer(buf);
	if (BufferIsValid(metabuf))
		UnlockReleaseBuffer(metabuf);
}

/*
 * Common part of an insert or update. Inserts the new tuple and updates the
 * revmap.
 */
static void
brin_ao_xlog_insert_update(XLogReaderState *record,
						xl_brin_ao_insert *xlrec)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	Buffer		buffer;
	BlockNumber regpgno;
	Page		page;
	XLogRedoAction action;

	/*
	 * If we inserted the first and only tuple on the page, re-initialize the
	 * page from scratch.
	 */
	if (XLogRecGetInfo(record) & XLOG_BRIN_AO_INIT_PAGE)
	{
		buffer = XLogInitBufferForRedo(record, 0);
		page = BufferGetPage(buffer);
		brin_ao_page_init(page, BRIN_AO_PAGETYPE_REGULAR);
		action = BLK_NEEDS_REDO;
	}
	else
	{
		action = XLogReadBufferForRedo(record, 0, &buffer);
	}

	/* need this page's blkno to store in revmap */
	regpgno = BufferGetBlockNumber(buffer);

	/* insert the index item into the page */
	if (action == BLK_NEEDS_REDO)
	{
		OffsetNumber offnum;
		Brin_AoTuple  *tuple;
		Size		tuplen;

		tuple = (Brin_AoTuple *) XLogRecGetBlockData(record, 0, &tuplen);

		Assert(tuple->bt_blkno == xlrec->heapBlk);

		page = (Page) BufferGetPage(buffer);
		offnum = xlrec->offnum;
		if (PageGetMaxOffsetNumber(page) + 1 < offnum)
			elog(PANIC, "brin_ao_xlog_insert_update: invalid max offset number");

		offnum = PageAddItem(page, (Item) tuple, tuplen, offnum, true, false);
		if (offnum == InvalidOffsetNumber)
			elog(PANIC, "brin_ao_xlog_insert_update: failed to add tuple");

		PageSetLSN(page, lsn);
		MarkBufferDirty(buffer);
	}
	if (BufferIsValid(buffer))
		UnlockReleaseBuffer(buffer);

	/* update the revmap */
	action = XLogReadBufferForRedo(record, 1, &buffer);
	if (action == BLK_NEEDS_REDO)
	{
		ItemPointerData tid;

		ItemPointerSet(&tid, regpgno, xlrec->offnum);
		page = (Page) BufferGetPage(buffer);

		brin_aoSetHeapBlockItemptr(buffer, xlrec->pagesPerRange, xlrec->heapBlk,
								tid);
		PageSetLSN(page, lsn);
		MarkBufferDirty(buffer);
	}
	if (BufferIsValid(buffer))
		UnlockReleaseBuffer(buffer);

	/* XXX no FSM updates here ... */
}

/*
 * replay a BRIN_AO index insertion
 */
static void
brin_ao_xlog_insert(XLogReaderState *record)
{
	xl_brin_ao_insert *xlrec = (xl_brin_ao_insert *) XLogRecGetData(record);

	brin_ao_xlog_insert_update(record, xlrec);
}

/*
 * replay a BRIN_AO index update
 */
static void
brin_ao_xlog_update(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_update *xlrec = (xl_brin_ao_update *) XLogRecGetData(record);
	Buffer		buffer;
	XLogRedoAction action;

	/* First remove the old tuple */
	action = XLogReadBufferForRedo(record, 2, &buffer);
	if (action == BLK_NEEDS_REDO)
	{
		Page		page;
		OffsetNumber offnum;

		page = (Page) BufferGetPage(buffer);

		offnum = xlrec->oldOffnum;

		PageIndexTupleDeleteNoCompact(page, offnum);

		PageSetLSN(page, lsn);
		MarkBufferDirty(buffer);
	}

	/* Then insert the new tuple and update revmap, like in an insertion. */
	brin_ao_xlog_insert_update(record, &xlrec->insert);

	if (BufferIsValid(buffer))
		UnlockReleaseBuffer(buffer);
}

/*
 * Update a tuple on a single page.
 */
static void
brin_ao_xlog_samepage_update(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_samepage_update *xlrec;
	Buffer		buffer;
	XLogRedoAction action;

	xlrec = (xl_brin_ao_samepage_update *) XLogRecGetData(record);
	action = XLogReadBufferForRedo(record, 0, &buffer);
	if (action == BLK_NEEDS_REDO)
	{
		Size		tuplen;
		Brin_AoTuple  *brin_aotuple;
		Page		page;
		OffsetNumber offnum;

		brin_aotuple = (Brin_AoTuple *) XLogRecGetBlockData(record, 0, &tuplen);

		page = (Page) BufferGetPage(buffer);

		offnum = xlrec->offnum;

		if (!PageIndexTupleOverwrite(page, offnum, (Item) brin_aotuple, tuplen))
			elog(PANIC, "brin_ao_xlog_samepage_update: failed to replace tuple");

		PageSetLSN(page, lsn);
		MarkBufferDirty(buffer);
	}
	if (BufferIsValid(buffer))
		UnlockReleaseBuffer(buffer);

	/* XXX no FSM updates here ... */
}

/*
 * Replay a revmap page extension
 */
static void
brin_ao_xlog_revmap_extend(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_revmap_extend *xlrec;
	Buffer		metabuf;
	Buffer		buf;
	Page		page;
	BlockNumber targetBlk;
	XLogRedoAction action;

	xlrec = (xl_brin_ao_revmap_extend *) XLogRecGetData(record);

	XLogRecGetBlockTag(record, 1, NULL, NULL, &targetBlk);
	Assert(xlrec->targetBlk == targetBlk);

	/* Update the metapage */
	action = XLogReadBufferForRedo(record, 0, &metabuf);
	if (action == BLK_NEEDS_REDO)
	{
		Page		metapg;
		Brin_AoMetaPageData *metadata;

		metapg = BufferGetPage(metabuf);
		metadata = (Brin_AoMetaPageData *) PageGetContents(metapg);

		Assert(metadata->lastRevmapPage == xlrec->targetBlk - 1);
		metadata->lastRevmapPage = xlrec->targetBlk;

		PageSetLSN(metapg, lsn);

		/*
		 * Set pd_lower just past the end of the metadata.  This is essential,
		 * because without doing so, metadata will be lost if xlog.c
		 * compresses the page.  (We must do this here because pre-v11
		 * versions of PG did not set the metapage's pd_lower correctly, so a
		 * pg_upgraded index might contain the wrong value.)
		 */
		((PageHeader) metapg)->pd_lower =
			((char *) metadata + sizeof(Brin_AoMetaPageData)) - (char *) metapg;

		MarkBufferDirty(metabuf);
	}

	/*
	 * Re-init the target block as a revmap page.  There's never a full- page
	 * image here.
	 */

	buf = XLogInitBufferForRedo(record, 1);
	page = (Page) BufferGetPage(buf);
	brin_ao_page_init(page, BRIN_AO_PAGETYPE_REVMAP);

	PageSetLSN(page, lsn);
	MarkBufferDirty(buf);

	UnlockReleaseBuffer(buf);
	if (BufferIsValid(metabuf))
		UnlockReleaseBuffer(metabuf);
}

static void
brin_ao_xlog_desummarize_page(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_desummarize *xlrec;
	Buffer		buffer;
	XLogRedoAction action;

	xlrec = (xl_brin_ao_desummarize *) XLogRecGetData(record);

	/* Update the revmap */
	action = XLogReadBufferForRedo(record, 0, &buffer);
	if (action == BLK_NEEDS_REDO)
	{
		ItemPointerData iptr;

		ItemPointerSetInvalid(&iptr);
		brin_aoSetHeapBlockItemptr(buffer, xlrec->pagesPerRange, xlrec->heapBlk, iptr);

		PageSetLSN(BufferGetPage(buffer), lsn);
		MarkBufferDirty(buffer);
	}
	if (BufferIsValid(buffer))
		UnlockReleaseBuffer(buffer);

	/* remove the leftover entry from the regular page */
	action = XLogReadBufferForRedo(record, 1, &buffer);
	if (action == BLK_NEEDS_REDO)
	{
		Page		regPg = BufferGetPage(buffer);

		PageIndexTupleDeleteNoCompact(regPg, xlrec->regOffset);

		PageSetLSN(regPg, lsn);
		MarkBufferDirty(buffer);
	}
	if (BufferIsValid(buffer))
		UnlockReleaseBuffer(buffer);
}

/*
 * We have an extra upper layer in the brin_ao revmap of the
 * ao / aocs table. Set the block number of revmap page by this
 * function.
 */
static void
brin_aoSetRevmapBlockNumber(Buffer buf, BlockNumber pagesPerRange,
						 BlockNumber heapBlk, BlockNumber revmapBlk)
{
	Revmap_AOUpperBlockContents *contents;
	Page		page;
	BlockNumber targetupperindex;
	BlockNumber *blks;

	page = BufferGetPage(buf);
	contents = (Revmap_AOUpperBlockContents*) PageGetContents(page);
	targetupperindex = HEAPBLK_TO_REVMAP_AO_UPPER_IDX(pagesPerRange, heapBlk);
	blks = (BlockNumber*) contents->rm_blocks;
	blks[targetupperindex] = revmapBlk;
}

static void
brin_ao_xlog_revmap_extend_upper(XLogReaderState *record)
{
	XLogRecPtr	lsn = record->EndRecPtr;
	xl_brin_ao_revmap_extend_upper *xlrec;
	Buffer		buf;
	Page		page;
	XLogRedoAction action;

	xlrec = (xl_brin_ao_revmap_extend_upper *) XLogRecGetData(record);
	action = XLogReadBufferForRedo(record, 0, &buf);
	if (action == BLK_NEEDS_REDO)
	{
		page = (Page) BufferGetPage(buf);
		brin_aoSetRevmapBlockNumber(buf, xlrec->pagesPerRange, xlrec->heapBlk, xlrec->revmapBlk);
		PageSetLSN(page, lsn);
		MarkBufferDirty(buf);
	}

	if (BufferIsValid(buf))
		UnlockReleaseBuffer(buf);
}

void
brin_ao_redo(XLogReaderState *record)
{
	uint8		info = XLogRecGetInfo(record) & ~XLR_INFO_MASK;

	switch (info & XLOG_BRIN_AO_OPMASK)
	{
		case XLOG_BRIN_AO_CREATE_INDEX:
			brin_ao_xlog_createidx(record);
			break;
		case XLOG_BRIN_AO_REVMAP_INIT_UPPER_BLK:
			brin_ao_xlog_revmap_init_upper_blk(record);
			break;
		case XLOG_BRIN_AO_INSERT:
			brin_ao_xlog_insert(record);
			break;
		case XLOG_BRIN_AO_UPDATE:
			brin_ao_xlog_update(record);
			break;
		case XLOG_BRIN_AO_SAMEPAGE_UPDATE:
			brin_ao_xlog_samepage_update(record);
			break;
		case XLOG_BRIN_AO_REVMAP_EXTEND:
			brin_ao_xlog_revmap_extend(record);
			break;
		case XLOG_BRIN_AO_DESUMMARIZE:
			brin_ao_xlog_desummarize_page(record);
			break;
		case XLOG_BRIN_AO_REVMAP_EXTEND_UPPER:
			brin_ao_xlog_revmap_extend_upper(record);
			break;
		default:
			elog(PANIC, "brin_ao_redo: unknown op code %u", info);
	}
}

/*
 * Mask a BRIN_AO page before doing consistency checks.
 */
void
brin_ao_mask(char *pagedata, BlockNumber blkno)
{
	Page		page = (Page) pagedata;
	PageHeader	pagehdr = (PageHeader) page;

	mask_page_lsn_and_checksum(page);

	mask_page_hint_bits(page);

	/*
	 * Regular brin_ao pages contain unused space which needs to be masked.
	 * Similarly for meta pages, but mask it only if pd_lower appears to have
	 * been set correctly.
	 */
	if (BRIN_AO_IS_REGULAR_PAGE(page) ||
		(BRIN_AO_IS_META_PAGE(page) && pagehdr->pd_lower > SizeOfPageHeaderData))
	{
		mask_unused_space(page);
	}
}
