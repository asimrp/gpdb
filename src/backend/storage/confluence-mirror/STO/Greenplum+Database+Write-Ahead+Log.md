<span id="title-text"> Storage & Access : Greenplum Database Write-Ahead Log </span>
====================================================================================

This page last changed on Feb 10, 2016 by tyao.

**Author: Ken Sell**

<span style="color: rgb(23,54,93);">**Version: 1.0**</span> <span style="color: rgb(23,54,93);">**Created: 3/26/2012**</span>

-   <span class="TOCOutline">1</span> [Introduction](#GreenplumDatabaseWrite-AheadLog-Introduction)
-   <span class="TOCOutline">2</span> [Terms](#GreenplumDatabaseWrite-AheadLog-Terms)
-   <span class="TOCOutline">3</span> [Background](#GreenplumDatabaseWrite-AheadLog-Background)
-   <span class="TOCOutline">4</span> [The Write Ahead Log (WAL)](#GreenplumDatabaseWrite-AheadLog-TheWriteAheadLog(WAL))
    -   <span class="TOCOutline">4.1</span> [Commit Log (pg\_clog)](#GreenplumDatabaseWrite-AheadLog-CommitLog(pg_clog))
    -   <span class="TOCOutline">4.2</span> [Savepoints or Subtransactions (pg\_subtrans)](#GreenplumDatabaseWrite-AheadLog-SavepointsorSubtransactions(pg_subtrans))
    -   <span class="TOCOutline">4.3</span> [Physical Implementation of WAL](#GreenplumDatabaseWrite-AheadLog-PhysicalImplementationofWAL)
    -   <span class="TOCOutline">4.4</span> [Checkpoint](#GreenplumDatabaseWrite-AheadLog-Checkpoint)
        -   <span class="TOCOutline">4.4.1</span> [Illustration of checkpoint redo location on an active system.](#GreenplumDatabaseWrite-AheadLog-Illustrationofcheckpointredolocationonanactivesystem.)
-   <span class="TOCOutline">5</span> [Normal Operation WAL Logic](#GreenplumDatabaseWrite-AheadLog-NormalOperationWALLogic)
    -   <span class="TOCOutline">5.1</span> [Normal course of events for a simple transaction on MASTER (QD).](#GreenplumDatabaseWrite-AheadLog-NormalcourseofeventsforasimpletransactiononMASTER(QD).)
    -   <span class="TOCOutline">5.2</span> [Normal course of events for a simple transaction on a Segment (QE)](#GreenplumDatabaseWrite-AheadLog-NormalcourseofeventsforasimpletransactiononaSegment(QE))
    -   <span class="TOCOutline">5.3</span> [Timeline for a simple statement on QD and QEs.](#GreenplumDatabaseWrite-AheadLog-TimelineforasimplestatementonQDandQEs.)
-   <span class="TOCOutline">6</span> [Recovery WAL Logic](#GreenplumDatabaseWrite-AheadLog-RecoveryWALLogic)
    -   <span class="TOCOutline">6.1</span> [QE Recovery Steps](#GreenplumDatabaseWrite-AheadLog-QERecoverySteps)
        -   <span class="TOCOutline">6.1.1</span> [An example](#GreenplumDatabaseWrite-AheadLog-Anexample)
    -   <span class="TOCOutline">6.2</span> [QD Recovery Steps](#GreenplumDatabaseWrite-AheadLog-QDRecoverySteps)
    -   <span class="TOCOutline">6.3</span> [WAL Passes](#GreenplumDatabaseWrite-AheadLog-WALPasses)
        -   <span class="TOCOutline">6.3.1</span> [Pass 1](#GreenplumDatabaseWrite-AheadLog-Pass1)
        -   <span class="TOCOutline">6.3.2</span> [Pass 2](#GreenplumDatabaseWrite-AheadLog-Pass2)
        -   <span class="TOCOutline">6.3.3</span> [Pass 3](#GreenplumDatabaseWrite-AheadLog-Pass3)
        -   <span class="TOCOutline">6.3.4</span> [Pass 4](#GreenplumDatabaseWrite-AheadLog-Pass4)
        -   <span class="TOCOutline">6.3.5</span> [DTM recovery](#GreenplumDatabaseWrite-AheadLog-DTMrecovery)
-   <span class="TOCOutline">7</span> [References](#GreenplumDatabaseWrite-AheadLog-References)

Introduction
============

The Greenplum Database system is designed with Atomic Consistent Isolated Durable (ACID) properties. This means that the system will maintain a certain set of properties, which guarantee that all operations in a transaction will execute as one logical operation (Atomic), that the different elements within the system will be in agreement with each other on the valid state of the system (Consistent), that a given transaction is un-affected by other transactions (Isolated), and that once a transaction has completed, its affects are permanent (Durable).
This document describes the internal logic for transaction logging and crash recovery, which is used to support the ACID properties of the Greenplum database (GPDB) system.

Terms
=====

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>ACID</p></td>
<td align="left"><p>A set of properties (Atomic, Consistency, Isolation, and Durability), which guarantee that database transactions are processed reliably.</p></td>
</tr>
<tr class="even">
<td align="left"><p>Control File</p></td>
<td align="left"><p>A file called &quot;global/pg_control &quot;, which is located in the system default filespace directory. The file contains information needed for proper startup of the system. It can be read by using the &quot;pg_controldata&quot; tool.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>LSN</p></td>
<td align="left"><p>Log Sequence Number</p></td>
</tr>
<tr class="even">
<td align="left"><p>TLI</p></td>
<td align="left"><p>Time Line ID</p></td>
</tr>
<tr class="odd">
<td align="left"><p>WAL</p></td>
<td align="left"><p>Write-Ahead Logging (WAL).</p></td>
</tr>
<tr class="even">
<td align="left"><p>XID</p></td>
<td align="left"><p>Transaction ID</p></td>
</tr>
<tr class="odd">
<td align="left"><p>Xlog</p></td>
<td align="left"><p>A synonym for the Write Ahead Log (WAL).</p></td>
</tr>
</tbody>
</table>

Background
==========

The Greenplum database system (GPDB) uses Postgres two-phase commit support.  Two-phase commit support consists of the PREPARE TRANSACTION, COMMIT PREPARED, and ABORT PREPARED SQL commands.  It allows a Postgres instance to participate in a distributed transaction as a child.  GPDB segments use two-phase commit support so they can be children in GPDB distributed transactions.  The GPDB master has distributed transaction coordinator logic that broadcasts the Postgres two-phase commit SQL commands to the segments and records new distributed transaction commit and forget transaction log records.  The distributed commit record is the one (transaction log) record in the GPDB cluster that indicates the distributed transaction committed.  The distributed forget record is written later after all segments (i.e. children) have executed the COMMIT or ABORT PREPARED command and have finished with the distributed transaction.  Note: GPDB does not expose two-phase commit support to customers.  GPDB cannot participate in a distributed transactions coordinated outside of GPDB.  This is by design.  We don't want the availability of a GPDB cluster data to be dependent on external transaction coordinators.

The Greenplum database system uses a Write Ahead Log (WAL) facility to ensure data integrity and improve system performance. System performance is improved by only writing data from memory (i.e. buffer pool) to disk when necessary, but data is always written to a log file (i.e. the WAL) to maintain data integrity.

When appropriate, the system will physically write the data to disk (i.e. fsync). This happens during a checkpoint. In addition to writing all data to disk, a checkpoint record is added to the WAL. During recovery situations, this checkpoint record tells the system what data still needs to be written to disk. After a checkpoint, the portion of the wall that lies before the checkpoint can be recycled. The files containing this portion of the WAL can be reused.

The Write Ahead Log (WAL)
=========================

Write-Ahead Logging (WAL) is a standard approach to transaction logging. WAL's central concept is that changes to data files (where tables and indexes reside) are logged before they are written to permanent storage. Data pages do not need to be flushed to disk on every transaction commit. In the event of a crash, data changes not yet applied to the database can be recovered from the WAL. A major benefit of using WAL is a significantly reduced number of disk writes.

A basic assumption of a write ahead log is that log entries must reach stable storage before the data-page changes they describe. This ensures that replaying the log to its end will bring us to a consistent state where there are no partially-performed transactions. To guarantee this, each data page (either heap or index) is marked with the LSN (log sequence number — in practice, a WAL file location) of the latest XLOG record affecting the page. Before the buffer manager can write out a dirty page, it must ensure that xlog has been flushed to disk at least up to the page's LSN.

Not all data is logged to the WAL. AO and CO table data bypasses the WAL, and goes directly to their associated disk files. "CREATE AS SELECT FROM …" tables bypass the WAL to improve performance.

Each segment has it's own write ahead log located in the system default filespace under the "pg\_xlog" directory". Additional supporting files are located in the following directories:

**<span style="text-decoration: underline;">Directory</span>** **<span style="text-decoration: underline;">Description</span>**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>pg_clog</p></td>
<td align="left"><p>Records the commit status for each transaction that has been assigned a transaction ID (XID).</p></td>
</tr>
<tr class="even">
<td align="left"><p>pg_distributedlog</p></td>
<td align="left"><p>Records the full distributed XID information for each local transaction ID.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>pg_distributedxidmap</p></td>
<td align="left"><p>Stores the local transaction ID for each global transaction.</p></td>
</tr>
<tr class="even">
<td align="left"><p>pg_multixact</p></td>
<td align="left"><p>Stores an array of Transaction IDs for each MultiXactId.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>pg_subtrans</p></td>
<td align="left"><p>Records the transaction ID (XID) of each sub-transaction's parent.</p></td>
</tr>
</tbody>
</table>

Commit Log (pg\_clog)
---------------------

The pg\_clog records the commit status for each transaction that has been assigned an XID. A transaction can be in progress, committed, aborted, or "sub-committed". This last state means that it's a subtransaction that's no longer running, but its parent has not updated its state yet. It is not necessary to update a subtransaction's transaction status to subcommit, so we can just defer it until main transaction commit. The main role of marking transactions as sub-committed is to provide an atomic commit protocol when transaction status is spread across multiple clog pages. As a result, whenever transaction status spreads across multiple pages we must use a two-phase commit protocol: the first phase is to mark the subtransactions as sub-committed, then we mark the top level transaction and all its subtransactions committed (in that order). Thus, subtransactions that have not aborted appear as in-progress even when they have already finished, and the subcommit status appears as a very short transitory state during main transaction commit. Subtransaction abort is always marked in clog as soon as it occurs. When the transaction status all fit in a single CLOG page, we atomically mark them all as committed without bothering with the intermediate sub-commit state.

Every GPDB segment instance stores these transaction commit status under pg\_clog directory in a number of clog segment files. These files are named from 0000 to 03FF (naming convention explained later). Each transaction commit status is(currently) stored as a set of two bits i.e. in progress (00), committed (01), aborted (10), sub-committed (11).  Thus, a single byte can store commit status for 4 transactions at a time.  Postgres (and thus GPDB currently) uses blocks of size 32KB to store these transaction commit status.  With calculation, a single page can store 32 \* 1024 \* 4 = 131072 transaction commit status. Another limit used is that each segment file consists of maximum 32 such pages (131072 \* 32 = 4194304 transaction commit status).  Maximum possible transaction Id (currently) is MAX\_UINT32 = 2 ^ 32 - 1  = 4294967295. Postgres (and thus GPDB) doesn’t store all the commit status in one clog segment file. This is because the system (for e.g. vacuum) can then reclaim old unrequired clog files, which have commit status of dead transactions (transaction changes are either persistent or aborted). Hence all the transaction Id (0 to 4294967295) commit status are stored in (2^32 / (32\*32\*1024\*4) = 1024) 0x400 seg files equally distributed and thus have names from 0000 to 03FF (in hex).  GPDB clog subsystem uses a page caching system based on simple LRU theory. More technical details can be found in slru.c and clog.c. Some more relevant information about clog can obtained from Postgres wiki article on hint bits.

Savepoints or Subtransactions (pg\_subtrans)
--------------------------------------------

Savepoints are implemented using subtransactions. A subtransaction is a transaction inside a transaction; its commit or abort status is not only dependent on whether it committed itself, but also whether its parent transaction committed. To implement multiple savepoints in a transaction we allow unlimited transaction nesting depth, so any particular subtransaction's commit state is dependent on the commit status of each and every ancestor transaction.

The "subtransaction parent" (pg\_subtrans) mechanism records, for each transaction with an XID, the Transaction ID of its parent transaction. This information is stored as soon as the subtransaction is assigned an XID. Top-level transactions do not have a parent, so they leave their pg\_subtrans entries set to the default value of zero (InvalidTransactionId).

Physical Implementation of WAL
------------------------------

The WAL is made up of files located in the pg\_xlog directory of each segment. Each file is referred to as a "WAL segment", and each "WAL segment" contains 64 megabytes of WAL data.

Each file is named using the following conventions:

**&lt;Timeline ID&gt;&lt;Log ID&gt;&lt;WAL Segment Number&gt;**

``` theme:
For example, 00000001000000000000000 is formed by using

Timeline ID   = 00000001
Log ID   = 00000000
WAL Seg #  = 00000000

Each file contains XLOG records with the following format.

typedef struct XLogRecord
```

**XLog record format**

``` theme:
typedef struct XLogRecord
{
  pg_crc32      xl_crc;     /* CRC for this record */
  XLogRecPtr    xl_prev;    /* ptr to previous record in log */
  TransactionId xl_xid;     /* xact id */
  uint32        xl_tot_len; /* total len of entire record */
  uint32        xl_len;     /* total len of rmgr data */
  uint8         xl_info;    /* flag bits */
  RmgrId        xl_rmid;    /* resource manager for this record */
} XLogRecord;
```

<span style="color: rgb(128,0,0);">**/**</span> <span style="color: rgb(128,0,0);">ACTUAL LOG DATA FOLLOWS AT END OF STRUCT</span> <span style="color: rgb(128,0,0);">**/**</span>

For example, given the following insert into a heap table "t1",

``` theme:
INSERT INTO t1 VALUES (1);

This would generate an xlog record similar to:
A CRC value                     = A 32-bit checksum
Previous xlog record location   = 0/A18A5C
Transaction ID                  = 926
Total length of entire record   = 72
Length of data at end of record = 44
Info Flag bits                  = 80
Resource Manager                = 10 (HEAP)
```

``` theme:
< DATA for the insert >

The "Data for the insert" would contain... <KAS need to add>
```

Checkpoint
----------

*Checkpoints* are points in the sequence of transactions at which it is guaranteed that the data files have been updated with all information written before the checkpoint. At checkpoint time, all dirty data pages are flushed to disk and a special checkpoint record is written to the WAL. A checkpoint allows the system to truncate (i.e. discard) WAL data that precedes the checkpoint and has been committed.

**Checkpoint record format**

``` theme:
typedef struct CheckPoint
{
  XLogRecPtr  redo;  /* Next RecPtr available when we began to
                             create CheckPoint (i.e. REDO start point) */
  XLogRecPtr  undo;  /* First record of oldest in-progress
                             transaction when we started (i.e. UNDO end
                             point) */
  TimeLineID      ThisTimeLineID;  /* current TLI */
  uint32          nextXidEpoch;     /* higher-order bits of nextXid */
  TransactionId   nextXid;         /* next free XID */
  Oid             nextOid;          /* next free OID */
  MultiXactId     nextMulti;       /* next free MultiXactId */
  MultiXactOffset nextMultiOffset; /* next free MultiXact offset */
  time_t              time;            /* time stamp of checkpoint */
  /* Checkpoint data follows the CheckPoint record */

} CheckPoint;
```

<span style="color: rgb(255,0,0);">*Note, undo is not used.*</span>
The checkpoint data following the CheckPoint record has 5 sections.

1.  Global transaction information (GXACT)
2.  Filespace information
3.  Tablespace information
4.  Database information
5.  Prepared transaction information

Sections 2 through 4 are only used by the QD and not by QEs.

Section 5 (prepared transaction information) is used in the event of a crash and recovery. It contains information about "in flight" transactions at the time of the checkpoint, and will be used to re-create the state of the in flight transactions.

If a checkpoint is taken on a system with no other activity, then the redo location will be the same location as the checkpoint itself (i.e. everything up to the checkpoint has already been written to disk. If there is other activity on the system, then additional records may be added to the WAL while taking a checkpoint. In that case, the redo location would actually point to a point in the WAL where the checkpoint started and all physical writes to disk where completed.

###### Illustration of checkpoint redo location on an active system.

<img src="attachments/43091908/43221118.jpg" class="confluence-embedded-image confluence-content-image-border" width="758" height="281" />

In the event of a crash, the crash recovery procedure looks at the latest checkpoint record to determine where to start re-applying data (i.e. the Redo location).

Normal Operation WAL Logic
==========================

The behavior of the WAL differs somewhat on the master (QD) and the segments (QEs). The QD WAL generally keeps track of distributed actions, and the QE WALs keep track of a given segment's actions.

Normal course of events for a simple transaction on MASTER (QD).
----------------------------------------------------------------

``` theme:
Execute query (e.g. INSERT INTO t1 VALUES (1) .
At commit time,
    Send a prepare message to the QEs.
    Insert a distributed commit record into the xlog.
    Send a commit message to the QEs.
    Insert a distributed forget record into the xlog.
```

Normal course of events for a simple transaction on a Segment (QE)
------------------------------------------------------------------

``` theme:
Execute data writes to xlog (e.g. for INSERT INTO t1 VALUES (1), write a heap record into the xlog containing the data for the insert).
At commit time, insert a prepare record into the xlog.
Insert a commit prepared record.
```

Timeline for a simple statement on QD and QEs.
----------------------------------------------

The following chart illustrates the WAL entries for a QD (master) and QE (segment) for a simple SQL statement, "INSERT INTO t1 VALUES (1)".

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Time</p></th>
<th align="left"><p>QD WAL Records</p></th>
<th align="left"><p>QE WAL Records</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p>Data record for insert</p></td>
</tr>
<tr class="even">
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p>Prepare record</p></td>
</tr>
<tr class="odd">
<td align="left"><p> </p></td>
<td align="left"><p>Distributed Commit record</p></td>
<td align="left"><p> </p></td>
</tr>
<tr class="even">
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p>Commit prepared record</p></td>
</tr>
<tr class="odd">
<td align="left"><p> </p></td>
<td align="left"><p>Distributed Forget record</p></td>
<td align="left"><p> </p></td>
</tr>
</tbody>
</table>

Recovery WAL Logic
==================

During WAL replay, we can check the LSN of a page to detect whether the change recorded by the current log entry is already applied (it has been, if the page LSN is &gt;= the log entry's WAL location).

QE Recovery Steps
-----------------

In general, recovery will undo work for transactions that must be aborted because they were not in a state to be completed, and will redo work for transactions that were in a completed state at the time of the crash.

### An example

Using the timeline example described in 5.3, a crash at each step is described below.
**<span style="text-decoration: underline;">After Time 1, but before Time</span>** **<span style="text-decoration: underline;">2.</span>**

The data has been written to the xlog and possibly to disk (i.e. the physical storage of the table), but the transaction has not been prepared. A recovery at this point would "fix" any actions taken by the transaction. In the case of writes to a heap table, the clog entry for the transaction would be marked as "aborted". No changes to the physical storage of the records are necessary because the records are not visible. Their hint bits have not been updated to show they are committed.

**<span style="text-decoration: underline;">After Time 2, but before Time 3.</span>**

At this point, the QD has dispatched a prepare to commit command to the QE, and the QE has inserted a "prepare" record into the xlog. The QD recovery logic doesn't know if the QE actually received the prepare and/or completed all of its work, so during recovery the QD will send an "abort prepared" command for the transaction.

The QE will abort the transaction and insert an abort record in the xlog.

**<span style="text-decoration: underline;">After Time 3, but before Time 4.</span>**

The QD will query each QE and determine which not executed the commit of the prepare (i.e. have not completed Time 4 yet). The QD does a "select gid from pg\_prepared\_xacts" to get each segment's prepared, but not committed transactions. The QD then aborts all QE transactions that have not yet committed.

**<span style="text-decoration: underline;">After Time 4, but before Time 5.</span>**

The QD will re-send a "commit prepared" record to the QE to commit the transaction.

QD Recovery Steps
-----------------

&lt;KAS need to do.&gt;

WAL Passes
----------

There are 4 major recovery passes during WAL recovery. One final step is taken when distributed transactions must be either committed or aborted by the QD. This step is taken at DTM startup and after WAL recovery. The following sections describe each pass and include pseudo code for each pass.

### Pass 1

**Pass 1**

``` theme:
StartupXLOG()
        SetupCheckpointPreparedTransactionList(Checkpoint *)
            “Add any prepared transactions on the checkpoint to the
 crashRecoverPostCheckpointPreparedTransaction_map_ht”
      “If records follow the checkpoint, apply redo to them.”
        while records past checkpoint
            if (PersistentRecovery_ShouldHandlePass1XLogRec())
                “I think this is only for persistent tables, etc.”
                ApplyStartupRedo()
                “This function will call all the resource managers redo
                 functions including”
                 xlog_redo()
 Xact_redo()
     “if prepare record then
            RecreateTwoPhaseFile()
                crashRecoverPostCheckpointPreparedTransaction_map
     else if commit prepared record then
RemoveTwoPhaseFile()
crashRecoverPostCheckpointPreparedTransaction_map
                        else if abort prepared
RemoveTwoPhaseFile()
crashRecoverPostCheckpointPreparedTransaction_map”
        if NOT clean shutdown or NOT checkpoint is last record then
            Checkpoint_RecoveryPass()
            CheckPointGuts()
        if NOT multiple recovery passes needed then
        StartupXLOG_InProduction()
            RecoverPreparedTransactions(CheckPoint record pointer)
                    “Find all the prepared pointers left on the hash list”.
                    MarkAsPreparing()
                        “Adds it to the TwoPhaseState list.”
                CreateCheckPoint()
                    “Build a checkpoint record and insert it into the xlog.”
                    “This includes adding any prepared transactions to the
                    checkpoint record.”
                    getTwoPhasePreparedTransactionData()
                        “Get all entries on the TwoPhaseState list”
                CheckPointGuts()
                        CheckPointTwoPhase()
                            “No need to do anything.”
```

### Pass 2

**Pass 2**

``` theme:
StartupProcessMain()
    StartupXLOG_Pass2()
        “Get Time line information.”
        “Get any prepared transaction information from the checkpoint.”
        PersistentRecovery_HandlePass2XlogRec()
            “Looks like this recreates any Persistent file info.”
        PersistentRecovery_CrashAbort()
            “Generate abort xlog records for Persistent Table entries”
        PersistentRecovery_Update()
            “Update state of objects in persistent objects”
        PersistentRecovery_Drop()
            “Drop objects from persistent tables”
        PersistentRecovery_UpdateAppendOnlyMirrorResyncEofs()
            “Update EOFs for Appendonly tables in persistent tables”
        PersistentRecovery_SerializeRedoRelationFile()
        “Create a checkpoint”
        “Create a file (pg_xlog/RedoRelationFile), which contains
         information for redo in Pass 3”
```

### Pass 3

**Pass 3**

``` theme:
StartupProcessMain()
StartupXLOG_Pass3()
    “Redo file from pass 2”
SetupCheckpointPreparedTransactionList(Checkpoint *)
            “Add any prepared transactions on the checkpoint to the
 crashRecoverPostCheckpointPreparedTransaction_map_ht”
        “If we are the master, create a file called
         pg_utilitymodetmredo/savetmredo.file that will hold DTM redo
 information.”
        “LOOP starting at pass 1 start (i.e. start of next record past the
         original checkpoint. “
        ApplyStartupRedo()
                “This function will call all the resource managers redo
                 functions including”
                 xlog_redo()
             xact_redo()
       StartupXLOG_InProduction()
            PrescanPreparedTransactions()
        RecoverPreparedTransactions(CheckPoint record pointer)
                “By the time we get here, we have a hash list containing
                 all prepared transactions that were both before and after
                 the last checkpoint. All aborted transactions have been
                 removed.”
                    “Find all remaining prepared pointers left on the hash
                     list”.
                    MarkAsPreparing()
                        “Adds it to the TwoPhaseState list.”
                CreateCheckPoint()
                    “Build a checkpoint record and insert it into the xlog.”
                    “This includes adding any prepared transactions to the
                    checkpoint record.”
                    getTwoPhasePreparedTransactionData()
                        “Get all entries on the TwoPhaseState list”
                CheckPointGuts()
                        CheckPointTwoPhase()
                            “No need to do anything.”
```

### Pass 4

**Pass 4**

``` theme:
StartupXLOG_Pass4()
    “Performs checks to validate the relationship between objects in persistent tables
 and their associated database.”
```

### DTM recovery

If we are the master, and there are entries in the, pg\_utilitymodetmredo/savetmredo.file created in pass 3, then we want to resend "commit prepared" or abort distributed commands.

References
==========

Explanation of pg\_clog, <a href="http://wiki.postgresql.org/wiki/Hint_Bits" class="uri" class="external-link">http://wiki.postgresql.org/wiki/Hint_Bits</a>
General WAL info, cdb-pg/src/backend/access/transam/README

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Checkpoint\_redo\_location.jpg](attachments/43091908/43221118.jpg) (image/jpeg)

Document generated by Confluence on May 17, 2016 19:14


