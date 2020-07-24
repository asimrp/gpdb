/*
 * brin_ao_pageops.h
 *		Prototypes for operating on BRIN_AO pages.
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/include/access/brin_ao_pageops.h
 */
#ifndef BRIN_AO_PAGEOPS_H
#define BRIN_AO_PAGEOPS_H

#include "access/brin_ao_revmap.h"

extern bool brin_ao_doupdate(Relation idxrel, BlockNumber pagesPerRange,
						  Brin_AoRevmap *revmap, BlockNumber heapBlk,
						  Buffer oldbuf, OffsetNumber oldoff,
						  const Brin_AoTuple *origtup, Size origsz,
						  const Brin_AoTuple *newtup, Size newsz,
						  bool samepage, bool skipextend);
extern bool brin_ao_can_do_samepage_update(Buffer buffer, Size origsz,
										Size newsz);
extern OffsetNumber brin_ao_doinsert(Relation idxrel, BlockNumber pagesPerRange,
								  Brin_AoRevmap *revmap, Buffer *buffer, BlockNumber heapBlk,
								  Brin_AoTuple *tup, Size itemsz);

extern void brin_ao_page_init(Page page, uint16 type);
extern void brin_ao_metapage_init(Page page, BlockNumber pagesPerRange,
							   uint16 version, bool isAo);

extern bool brin_ao_start_evacuating_page(Relation idxRel, Buffer buf);
extern void brin_ao_evacuate_page(Relation idxRel, BlockNumber pagesPerRange,
							   Brin_AoRevmap *revmap, Buffer buf);

extern void brin_ao_page_cleanup(Relation idxrel, Buffer buf);

#endif							/* BRIN_AO_PAGEOPS_H */
