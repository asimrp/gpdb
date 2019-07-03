/*-------------------------------------------------------------------------
 *
 * storage.h
 *	  prototypes for functions in backend/catalog/storage.c
 *
 *
 * Portions Copyright (c) 1996-2014, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/catalog/storage.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef STORAGE_H
#define STORAGE_H

#include "storage/block.h"
#include "storage/relfilenode.h"
#include "utils/relcache.h"

extern void RelationCreateStorage(RelFileNode rnode, char relpersistence, char relstorage);
extern void DatabaseCreateStorage(Oid db_id, Oid dsttablespace);
extern void RelationDropStorage(Relation rel);
extern void DatabaseDropStorage(Oid db_id, Oid dsttablespace,char pending_mode);
extern void RelationPreserveStorage(RelFileNode rnode, bool atCommit);
extern void RelationTruncate(Relation rel, BlockNumber nblocks);

/*
 * These functions used to be in storage/smgr/smgr.c, which explains the
 * naming
 */
extern void smgrDoPendingDeletes(bool isCommit);
extern int	smgrGetPendingDeletes(bool forCommit, RelFileNodePendingDelete **ptr);
extern void AtSubCommit_smgr(void);
extern void AtSubAbort_smgr(void);
extern void PostPrepare_smgr(void);

#define		  PENDINGDEL_R      'r'		/* create/delete table */
#define		  PENDINGDEL_DB     'd'		/* create/delete database */
#define		  PENDINGDEL_DB_ALT 'a'		/* alter database*/

#endif   /* STORAGE_H */
