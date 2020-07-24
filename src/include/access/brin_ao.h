/*
 * AM-callable functions for BRIN_AO indexes
 *
 * Portions Copyright (c) 1996-2019, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *		src/include/access/brin_ao.h
 */
#ifndef BRIN_AO_H
#define BRIN_AO_H

#include "fmgr.h"
#include "nodes/execnodes.h"
#include "utils/relcache.h"


/*
 * Storage type for BRIN_AO's reloptions
 */
typedef struct Brin_AoOptions
{
	int32		vl_len_;		/* varlena header (do not touch directly!) */
	BlockNumber pagesPerRange;
	bool		autosummarize;
} Brin_AoOptions;


/*
 * Brin_AoStatsData represents stats data for planner use
 */
typedef struct Brin_AoStatsData
{
	BlockNumber pagesPerRange;
	BlockNumber revmapNumPages;
} Brin_AoStatsData;


#define BRIN_AO_DEFAULT_PAGES_PER_RANGE	128
#define Brin_AoGetPagesPerRange(relation) \
	((relation)->rd_options ? \
	 ((Brin_AoOptions *) (relation)->rd_options)->pagesPerRange : \
	  BRIN_AO_DEFAULT_PAGES_PER_RANGE)
#define Brin_AoGetAutoSummarize(relation) \
	((relation)->rd_options ? \
	 ((Brin_AoOptions *) (relation)->rd_options)->autosummarize : \
	  false)


extern void brin_aoGetStats(Relation index, Brin_AoStatsData *stats);

#endif							/* BRIN_AO_H */
