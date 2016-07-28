<span id="title-text"> Storage & Access : Distributed Transaction Management and Snapshots </span>
==================================================================================================

This page last changed on Mar 18, 2012 by paramr3.

###### \*\*\* Work in progress \*\*\*

###### Authors: Vaijayanti Bharadwaj and Matt McCline

Introduction
============

The Greenplum Database (GPDB) uses distributed transactions and two phase commit protocol to ensure that ACID properties are preserved while doing updates to the database. This is done on top of Postgres transaction support. The <a href="http://confluence.greenplum.com/display/DEEP/Transaction+Research" class="external-link">Transaction Research</a> page has a collection of some of the initial research on support for distributed transactions in GPDB, which you might find useful to understand the big picture.

This document discusses the following briefly:

1.  Transaction Snapshots and other distributed structures.
2.  Interaction between <a href="http://confluence.greenplum.com/display/STO/Notes+on+SegMate" class="external-link">Segmates</a> such as <a href="http://confluence.greenplum.com/display/DEEP/Shared+Local+Snapshot" class="external-link">Shared SnapshotSlot</a>, subtransaction status, combo-cid mapping and cursor snapshots.
3.  Distributed Transaction protocol and state changes during the protocol.

Snapshots and Distributed State
===============================

Snapshots
---------

Multiversion Concurrency Control (MVCC) defines a snapshot as the following:

**MVCC Snapshot Structure**

``` theme:
typedef struct SnapshotData
{
  TransactionId xmin; /* XID < xmin are visible to me */
  TransactionId xmax; /* XID >= xmax are invisible to me */
  uint32 xcnt; /* # of xact ids in xip[] */
  TransactionId xip; / array of xact IDs in progress. Note: all ids in xip[] satisfy xmin <= xip[i]<xmax */
  int32 subxcnt; /* # of xact ids in subxip[]*/
  TransactionId subxip; / array of subxact IDs in progress */
  CommandId curcid; /* in my xact,  CID < curcid are visible */
  bool haveDistribSnapshot;/* True if snapshot is distributed. */
  DistributedSnapshotWithLocalMapping distribSnapshotWithLocalMapping;
} SnapshotData; 
```

Last two fields are MPP additions to the snapshot structure. When a transaction starts executing, concurrent transactions can be in progress and to allow this to execute concurrently with other transactions that are not yet committed, while maintaining ACID semantics, MVCC uses snapshots. A transaction can see effects of all transactions &lt; xmin (which are guaranteed to be committed). It cannot see effect of any transaction &gt;= xmax, which started after it did. Out of transactions between xmin and xmax, it can see what is not in progress. For this an array is maintained. For MPP, an array with local-distributed xact mapping is kept.

The problem being solved for distributed transactions is this. L represents local xid, D represents distributed xid.

<img src="attachments/42502230/42696877.png" class="confluence-embedded-image confluence-content-image-border" />

The above case is handled because distributed transactions are evaluated against their distributed snapshot only, and local transactions are handled because the distributed transactions are included in local snapshots through their local xids. In above case, even if 101 starts later, it may reach a QE and get committed earlier than 100. So when 100 reaches the QE, 101 is committed, but 100 will not see effects of 101 because of the distributed snapshot. If L=6 on QE (a local only transaction starts after 101 commits locally, it will see effects of 101 locally, but not that of 100.

How is a Snapshot Created?
--------------------------

**GetSnapshotData**
This function does multiple things:

-   Populates the Distributed transaction snapshot. For a QD, the distributed snapshot (DistributedSnapshotWithLocalMapping – dslm) is filled up looking at TMGXACT array(**createDtxSnapshot**()), which stores local, as well as distributed transaction xids. For QE readers and writers, the distributed snapshot is simply filled up from DtxContextInfo, there is no coordination required.
-   The local snapshot for QE **writer** and QD is filled up looing at all PGPROCs and their xids. The xmin and xmax together with xid array is computed looking at PGPROC array. This is to cover local-only transactions. Then the QE writer copies this information into SharedLocalSnapshotSlot-&gt;snapshot to be used by QE readers.
-   QE writer also updates other fields in Shared snapshot slot (covered later, also copies Xidcache from PGPROC of writer to SharedSnapshotSlot. See later for Segmate interaction.)
-   For QE readers wait for the snapshot slot to get ready and then make the shared snapshot its own. Set the local TopTransaction xid to be the QE writer's xid.

Interaction between Segmates
----------------------------

QE readers need to know current xact id information from QE writers. This is because QE Readers do not start their own transaction. We just set a few variables in the xact.c module to fake making it look like there is a current transaction, including which sub-transactions are active or committed. If we do not share some information with QE reader, it will not be able to see tuples created by the QE writer.

Shared Snapshots
----------------

A QE reader does not compute a local snapshot by itself – it sees the writer's local snapshot, which the writer writes in shared memory.

**MPP Shared Snapshot Structure**

``` theme:
typedef struct SharedSnapshotSlot
{
  int4 slotindex; /* where in the array this one is. */
  int4 slotid;
  pid_t pid; /* pid of writer seg */
  TransactionId xid;
  CommandId cid;
  TimestampTz startTimestamp;  /* Distributed xact information, from QD: DtxContextInfo */
  volatile TransactionId QDxid;
  volatile CommandId QDcid;
  volatile bool ready;
  volatile uint32 segmateSync;  /* Subtransaction information copied from QE writer. */
  uint32 total_subcnt; /* Total # of subxids */
  uint32 inmemory_subcnt; /* subxids in memory */
  TransactionId subxids[MaxGpSavePoints];
  uint32 combocidcnt;
  ComboCidKeyData combocids[MaxComboCids]; /* Copied from QE writer's local snap. */
  SnapshotData snapshot;
} SharedSnapshotSlot; 
```

Subtransaction state
--------------------

QE readers need to have same view of subtransactions as QE writer. A QE writer has subtransaction tree stored in its memory and that is dumped into SharedSnapshotSlot and then a file if required for the QE reader. A lot of detail on subtransaction state is described in the Rio SP2 design doc. This is skipped here for brevity.

Combocids
---------

Heap tuples are never updated or deleted in place, but every time an insert or an update happens, a new tuple is created and marked with xmin, the creating transaction id and old one (for update) is marked with mxax, the deleting transaction id. Similarly, there is a cmin, cmax for creating and deleting command ids. To save space, inserting and deleting cids are combined into one value, and a mapping is maintained, since cids are relevant just during the duration of the top-level transaction.

**Combocids**

``` theme:
typedef struct HeapTupleFields
{
  TransactionId t_xmin; /* inserting xact ID */
  TransactionId t_xmax; /* deleting or locking xact ID */
  union
  {
    CommandId t_cid;/* inserting/deleting cid, or both */
    TransactionId t_xvac;/* VACUUM FULL xact ID */
  } t_field3;
} HeapTupleFields; 
```

Combocids are deleting and inserting cid combined (a number representing cmin-cmax). Writer knows the mapping combocid: &lt;cmin, cmax&gt; and shares it with reader via SharedSnapshotSlot and a file.

Cursors
-------

A cursor all the time sees the same state as at the time it was created. A cursor does not live across transactions.

**Example**

``` theme:
begin;
savepoint s1;
savepoint s2;
savepoint s3;
insert into t1 values(10);
insert into t2 values(10);
declare c0 cursor for select * from t1 inner join t2 on (c1=c2);
savepoint s3;
insert into t1 values(20);
insert into t2 values(20);
fetch c0;
commit; 
```

Should just give 10, 10 and not give 20,20. The tuples inserted after the cursor was declared are stamped with subtransaction xid of s3 and will not be seen by the cursor, even though the QE writer's SharedLocalSnapshotSlot lists xid of s3. When a cursor is created, it dumps its own snapshot and this snapshot is used by all the readers, not the SharedSnapshotSlot.

Distributed State
-----------------

GPDB maintains distributed state across the master and all QEs. Global distributed state is tracked by these structures:

1.  Array of TMGXACT in shared memory (shmGxactArray). This is only on the QD.
    1.  Contains distributed xid
    2.  Char gid (timestamp + distributed xid)
    3.  Distributed state (DtxState)
    4.  Local transaction id
    5.  xminDistributedSnapshot: At the time this is running, what is the minimum distributed xact id of all in progress xacts?
    6.  LocalDistribXactRef: a reference to a local-distributed xact mapping.
    7.  LocalDistribXact which maps a local to a distributed xact id. TMGXACT keeps a reference on this and so also PGPROC.

2.  PGPROPGPROC stores the xid of current transaction (local xid). It also stores a reference to LocalDistribXactRef (which gives easy access to the distributed xid of the current proc.) It also keeps a XidCache of subxids. See later for more details on this.
3.  **XidCache**: Each PGPROC of the QE writer's backend also stores a list of its subcommitted and active subtransaction xids. This is kept updated during creation of subtransaction xids and rollback. For knowing subtransactions of current xact, this is the cheapest way. But if this cache overflow, pg\_subtrans file needs to be referred, which is not cheap, since this requires a disk access.
4.  Lock Table: Xact ids of in progress transactions are entered in the lock table. Waiters can wait on an in progress xact on this table. Lock table keeps local xids and not distributed xids.
5.  DtxContextInfo: A structure used for transferring distributed context to the QEs. The DtxContext info sent over the protocol command together with the state maintained by the receiving process completely defines the behavior of the process. It contains distributed-only xid information such as:
    1.  DistributedTransactionTimeStamp
    2.  DistributedTransactionId
    3.  Curcid
    4.  Havedistributedsnapshot
    5.  DistributedSnapshot: Purely distributed version of snapshot, with distributed xact ids only, since this goes over the wire to QEs, which do not understand local xids.
    6.  segmateSync
    7.  distributedTxnOptions DtxContextInfo is serialized only on QDs and deserialized only on QEs. It has a distributed snapshot that is constructed from the QD's Snapshot's DSLM (distributed snapshot with local mapping)

6.  A distributed log (distributedlog.c/h) noting the committed distributed xids. DistributedLog\_CommittedCheck() sees if a given local xid got committed in the distributed log. This is front-ended by a cache.
7.  LocalDistribXact: for each QE writer and QD it keeps a mapping between distributed and local xact id, with a reference to it from PGPROC of QE writer/QD. 
8.  Two phase file and two phase shared memory array. When a master sends DTX\_PROTOCOL\_CMD\_PREPARE to the segments, a transaction is prepared and all details of it are added to shared memory array and also to a file to persist it. The reason is that if this transaction needs to be committed later, even if the segment crashes, it should be able to honor the commit. &lt;More details TBD&gt;. Refer to the section on Distributed Protocol for more context.
9.  Pg\_subtrans: The pg\_subtrans manager is a pg\_clog-like manager that stores the parent transaction Id for each transaction. The tree can easily be walked from child to parent, but not in the opposite direction. This code is based on clog.c, but the robustness requirements are completely different from pg\_clog, because we only need to remember pg\_subtrans information for currently-open transactions. Thus, there is no need to preserve data over a crash and restart. During database startup, we simply force the currently-active page of SUBTRANS to zeroes.
10. Clog: TBD.

Visibility and Important Functions
==================================

Visibility determines if this tuple is seen by this transaction?
HeapTupleSatisfiesVisibility() says: is this tuple visible as per this snapshot. Special snapshot values are:

1.  SnapshotNow: Now means as of the current instant. HeapTupleSatisfiesNow() will address Halloween kind of problems where a command update what it already updated. It considers effect of:
    1.  all committed transactions (as of the current instant), even if these were running when this transaction started.
    2.  previous commands of this transaction

2.  SnapshotAny: anything is visible as per this criterion.
3.  SnapshotToast: &lt;TBD&gt;
4.  SnapshotDirty: HeapTupleSatisfiesDirty() will consider:
    1.  all committed transactions (as of the current instant) and in-progress transactions, even if these were running when this transaction got started.
    2.  previous commands of this transaction
    3.  changes made by the current command

5.  SnapshotSelf: HeapTupleSatisfiesItself() will consider:
    1.  all committed transactions (as of the current instant), even if these were running when this transaction got started.
    2.  previous commands of this transaction
    3.  changes made by the current command

TBD: More general information on what situations require these different visibility checks.
If instead of special snapshot values, a real snapshot is supplied to visibility check, it will evaluate visibility with respect to the given snapshot (HeapTupleSatisfiesSnapshot).

The transaction ids stamped on heap tuples are always local transaction ids and will be transaction ids of either a top-level or a subtransaction. The above visibility checks require that we need to know following information given a local (non-distributed, belonging to a segment only) transaction id:

-   Is this transaction in progress at this instant?
-   Is this transaction id committed or not?
-   Is this transaction id part of the current transaction (to know if this change is made by me.)
-   Is this transaction id in progress as per this snapshot?

There are some functions that are called from visibility checks that tell the status of a transaction. These are:

**TransactionIdIsInProgress (Is this transaction in progress at this instant?)**

All top level in progress transactions can be found by going over list of PGPROCs. In addition, in the XidCache of each PGPROC, there is a cache of sutransactions of the top level transaction, which is kept current by updating it every time a subtransaction xid is created or rolled back. So, the algorithm of this function is:

1.  the given Xid is a main transaction Id. We will find this out cheaply by looking at the PGPROC struct for each backend and store top level running xids if required for step 5.
2.  the given Xid is one of the cached subxact Xids in the PGPROC array.
3.  If not found in 1 or 2 and if the cache of subxact Xids in PGPROC is not overflowed, then this xid is not in progress.
4.  If cache is overflowed, then check to see if transaction did abort and if so, its not in progress.
5.  If not, we need to check pg\_subtrans and this is not cheap. We keep getting recursively the parent of this subxact id till we find topmost parent from pg\_subtrans (SubTransGetTopmostTransaction()). Compare this topmost xid to the list of xids we got from PGPROC in 1.

**TransactionIdIsCurrentTransactionId**

**For QE Writers:** We will return true for the Xid of the current subtransaction, any of its subcommitted children, any of its parents, or any of their subcomitted children. We go over the TransactionState tree, since this function is also called for local only transactions, which may not have SharedSnapshotSlot.

**For QE Readers:** We will return true if this xid is found in the list of subtransactions in the SharedSnapshotSlot subxid array or file or if this xid is the current top level transaction itself.

**XidInSnapshot**

Very interesting function, documenting later, TBD for lack of time.

Distributed Transaction Protocol
--------------------------------

Following illustration gives an idea of how the 2 phase commit protocol works. This is not an exhaustive state diagram, but for the simple case of everything succeeding and getting committed for a simple command such as insert, it illustrates the flow between master and segments.

<img src="attachments/42502230/42696881.jpg" class="confluence-embedded-image confluence-content-image-border" width="938" height="1205" /> <img src="attachments/42502230/42696882.jpg" class="confluence-embedded-image confluence-content-image-border" width="938" height="1208" />

Consider a simple statement such as inserting a value into a table. The following flow describes flow of commands and state changes on master and segment till the transaction gets committed.

Files and Important Functions
=============================

Files and functions:

**procarray.c**

``` theme:
GetSnapshotData(): function for taking the snapshot
FillInDistributedSnapshot() function for distributed snapshot
updateSharedLocalSnapshot: Function to update shared snapshot
```

**tqual.c**

``` theme:
All interesting visibility functions
Shared snapshot related helper functions
```

**xact.c**

``` theme:
Transaction functions
Functions to update subtransaction information in shared snapshot.
```

**cdbtm.c**

``` theme:
Routines for distributed transactions
```

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [SegMate Group Notes.docx](attachments/42502230/42696876.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Distributed Transaction ID.png](attachments/42502230/42696877.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Distributed Transaction Protocol Flow.jpg](attachments/42502230/42696881.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Distributed Transaction Protocol Flow 2.jpg](attachments/42502230/42696883.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Distributed Transaction Protocol Flow 2.jpg](attachments/42502230/42696882.jpg) (image/jpeg)

Document generated by Confluence on May 17, 2016 19:14


