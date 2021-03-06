/*-------------------------------------------------------------------------
 *
 * nodeBitmapAnd.h
 *
 *
 *
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/executor/nodeBitmapAnd.h,v 1.4 2007/01/05 22:19:54 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEBITMAPAND_H
#define NODEBITMAPAND_H

#include "nodes/execnodes.h"

extern int	ExecCountSlotsBitmapAnd(BitmapAnd *node);
extern BitmapAndState *ExecInitBitmapAnd(BitmapAnd *node, EState *estate, int eflags);
extern Node *MultiExecBitmapAnd(BitmapAndState *node);
extern void ExecEndBitmapAnd(BitmapAndState *node);
extern void ExecReScanBitmapAnd(BitmapAndState *node, ExprContext *exprCtxt);

enum 
{
	GPMON_BITMAPAND_TOTAL = GPMON_QEXEC_M_NODE_START,
};

static inline gpmon_packet_t *GpmonPktFromBitmapAndState(BitmapAndState *node)
{
	return &node->ps.gpmon_pkt;
}

#endif   /* NODEBITMAPAND_H */
