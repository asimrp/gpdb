<span id="title-text"> Storage & Access : Persistent Tables </span>
===================================================================

This page last changed on Dec 18, 2012 by pednea1.

***Author: Ashwin Pednekar***

-   <span class="TOCOutline">1</span> [Introduction](#PersistentTables-_Toc201147856Introduction)
-   <span class="TOCOutline">2</span> [Motivation](#PersistentTables-_Toc201147857Motivation)
-   <span class="TOCOutline">3</span> [Glossary](#PersistentTables-_Toc201147858Glossary)
-   <span class="TOCOutline">4</span> [Persistent Tables in System](#PersistentTables-_Toc201147859PersistentTablesinSystem)
    -   <span class="TOCOutline">4.1</span> [Row Structure](#PersistentTables-_Toc201147860RowStructure)
-   <span class="TOCOutline">5</span> [Some Points to Note](#PersistentTables-_Toc201147861SomePointstoNote)
-   <span class="TOCOutline">6</span> [State Diagrams](#PersistentTables-_Toc201147862StateDiagrams)
    -   <span class="TOCOutline">6.1</span> [Persistent File-System Object States.](#PersistentTables-_Toc201147863PersistentFile-SystemObjectStates.)
    -   <span class="TOCOutline">6.2</span> [Mirror related states](#PersistentTables-_Toc201147864Mirrorrelatedstates)
-   <span class="TOCOutline">7</span> [Database Actions](#PersistentTables-_Toc201147865DatabaseActions)
    -   <span class="TOCOutline">7.1</span> [CREATE OBJECT](#PersistentTables-_Toc201147866CREATEOBJECT)
    -   <span class="TOCOutline">7.2</span> [DROP OBJECT](#PersistentTables-_Toc201147867DROPOBJECT)
-   <span class="TOCOutline">8</span> [Scenarios](#PersistentTables-_Toc201147868Scenarios)
    -   <span class="TOCOutline">8.1</span> [Create object (Sync)](#PersistentTables-_Toc201147869Createobject(Sync))
    -   <span class="TOCOutline">8.2</span> [Create Object (Change Tracking)](#PersistentTables-_Toc201147870CreateObject(ChangeTracking))
    -   <span class="TOCOutline">8.3</span> [Drop Object (Sync) ](#PersistentTables-_Toc201147871DropObject(Sync))
    -   <span class="TOCOutline">8.4</span> [Drop Object (Change Tracking Mode)](#PersistentTables-_Toc201147872DropObject(ChangeTrackingMode))
    -   <span class="TOCOutline">8.5</span> [AO inserts](#PersistentTables-_Toc201147873AOinserts)
-   <span class="TOCOutline">9</span> [Crash Recovery](#PersistentTables-_Toc201147874CrashRecovery)
    -   <span class="TOCOutline">9.1</span> [Primary Segment Scenarios without Mirroring.](#PersistentTables-_Toc201147875PrimarySegmentScenarioswithoutMirroring.)
    -   <span class="TOCOutline">9.2</span> [Mirroring Scenarios](#PersistentTables-_Toc201147876MirroringScenarios)
-   <span class="TOCOutline">10</span> [Troubleshooting and Repair](#PersistentTables-_Toc201147877TroubleshootingandRepair)
-   <span class="TOCOutline">11</span> [](#PersistentTables-_Toc201147878)
-   <span class="TOCOutline">12</span> [Code Walkthrough](#PersistentTables-CodeWalkthrough)
    -   <span class="TOCOutline">12.1</span> [](#PersistentTables-_Toc201147879)
    -   <span class="TOCOutline">12.2</span> [Drop heap table / append-only table / index](#PersistentTables-_Toc201147880Dropheaptable/append-onlytable/index)
    -   <span class="TOCOutline">12.3</span> [Crash Recovery](#PersistentTables-_Toc201147881CrashRecovery)
-   <span class="TOCOutline">13</span> [Important GUCs](#PersistentTables-_Toc201147882ImportantGUCs)

<span id="PersistentTables-_Toc201147856" class="confluence-anchor-link"></span>Introduction
============================================================================================

Persistent Tables, in a very layman terms we would say something that stays forever. Now assume you are creating an object and suddenly the system crashes at the last moment. The database restarts and you again start the process all over again but you never know the process which was crashed before might have almost finished the work of creating the object in the system except for the last step which makes it visible to the outside world. The object (before crash) would lie dangling without any reference to it and hence would always be unreachable thus resulting in a wastage of space. Many such objects would eventually eat up our all space making systems unusable.

So we need a mechanism that could survive even a crash and store/record the system state (or system actions) before the crash, about what the system was doing when the system went down. This mechanism would keep a track for all such system actions (‘intentions’ wrt to Persistent Tables), which were started but not yet completed/committed and would therefore be helpful during crash recovery to take necessary steps to obtain consistency.

Same goes the discussion while deleting a object, where the system crashes when system had almost deleted the object except for the last few steps which makes the object completely unusable. In such case also system should know after crash recovery that it had almost deleted the object, so just go and finish it rather than starting over.

The scenario complicates in case of primary-mirror scenario. It may happen that the system has created/deleted the object on primary but not on mirror. After crash recovery system should identify this and would just create/delete object on mirror.  This is why concept of Persistent Tables were designed.

Persistent Tables are those database objects, which are primarily designed for accurately and durably manage the create and drop transactional statements on the file-system object data on mirror and primary segments.

The Persistent tables work on the basis of a careful-write technique known as “intentions”. Intentions are nothing but the way of telling the system that I am doing some changes (create/delete/update) to the system, kindly note it and store it. Thus the transaction action is first logged into the persistent table and then flushed to disk before the actual transaction takes place. Now if the database goes down after writing the intention in the persistent table, after coming up the system scans the persistent tables to check and get all the transaction actions it was performing before the database went down to recover the objects so that GPDB will be consistent when it is up and running.

During the mirror resynchronization, system will consult the Persistent Tables so find

1.  if there are any objects that were created or deleted on primaries and not on mirror while mirror was down. On discovery, it would take necessary action on mirror to get back the mirror to consistent state
2.  what tuples were inserted to Append Only tables on primary while mirror was down. The inserted tuples will be copied to mirror in order to get mirror to synchronized state

<span id="PersistentTables-_Toc201147857" class="confluence-anchor-link"></span>Motivation
==========================================================================================

1.  The system did not have the crash recovery undo processing. Thus the half-done work by the aborted transactions was never removed during the crash recovery. This was one of the very reason Persistent table concept was needed.
2.  The database did not support participating in the external local or the distributed transactions and we needed some kind of file system object that would keep records of the database actions. This will help track a transaction across a mirror pair.
3.  When a database crashes at certain points it is sometimes not able to clean up the work it was doing during the crash, thus it might cause the files to eat up the memory and never get deleted as database would never get the reference to such memory. This case worsens in case of mirroring scenario. The Mirror always would need to know the creation and deletion of the objects on its mirrored pair while it was down.
4.  There are also cases when objects are created on primaries and the mirror goes down just in time so as to create an inconsistent state. In such cases we need certain mechanism to tell the system once its up that the objects were created/dropped just on primaries and changes needs to be done on mirror in order to get back the system in consistent state.
5.  During mirror resync, the system needs to know which objects were logically created/deleted on the mirrored pair when the mirror was down so that it could create/delete the same on the mirror to get it back into a consistent/synced state.

<span id="PersistentTables-_Toc201147858" class="confluence-anchor-link"></span>Glossary
========================================================================================

***Persistent States***: (PS)

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p><em>PersistentFileSysState_Free</em></p></td>
<td align="left"><p>Free</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>PersistentFileSysState_CreatePending</em></p></td>
<td align="left"><p>CP</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>PersistentFileSysState_Created</em></p></td>
<td align="left"><p>C</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>PersistentFileSysState_DropPending</em></p></td>
<td align="left"><p>DP</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>PersistentFileSysState_AbortingCreate</em></p></td>
<td align="left"><p>AC</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>PersistentFileSysState_JustInTimeCreatePending</em></p></td>
<td align="left"><p>JTCP</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>PersistentFileSysState_BulkLoadCreatePending</em></p></td>
<td align="left"><p>BLCP</p></td>
</tr>
</tbody>
</table>

***Mirror Existence States*** (MES):

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p><em>MirroredObjectExistenceState_None</em></p></td>
<td align="left"><p>None</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredObjectExistenceState_NotMirrored</em></p></td>
<td align="left"><p>NM</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>MirroredObjectExistenceState_MirrorCreatePending</em></p></td>
<td align="left"><p>CP</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredObjectExistenceState_MirrorCreated</em></p></td>
<td align="left"><p>C</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>MirroredObjectExistenceState_MirrorDownBeforeCreate</em></p></td>
<td align="left"><p>DBC</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredObjectExistenceState_MirrorDownDuringCreate</em></p></td>
<td align="left"><p>DDC</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>MirroredObjectExistenceState_MirrorDropPending</em></p></td>
<td align="left"><p>DP</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredObjectExistenceState_OnlyMirrorDropRemains</em></p></td>
<td align="left"><p>OMDR</p></td>
</tr>
</tbody>
</table>

***Mirror Data Synchronization States*** (MDS):

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p><em>MirroredRelDataSynchronizationState_None</em></p></td>
<td align="left"><p>None</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredRelDataSynchronizationState_DataSynchronized</em></p></td>
<td align="left"><p>DS</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>MirroredRelDataSynchronizationState_FullCopy</em></p></td>
<td align="left"><p>FC</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredRelDataSynchronizationState_BufferPoolPageIncremental</em></p></td>
<td align="left"><p>BPI</p></td>
</tr>
<tr class="odd">
<td align="left"><p><em>MirroredRelDataSynchronizationState_BufferPoolScanIncremental</em></p></td>
<td align="left"><p>BSI</p></td>
</tr>
<tr class="even">
<td align="left"><p><em>MirroredRelDataSynchronizationState_AppendOnlyCatchup</em></p></td>
<td align="left"><p>AOC</p></td>
</tr>
</tbody>
</table>

***<span style="text-decoration: underline;">Before CHANGE</span>*** 
<img src="attachments/47449898/47349915.jpg" class="confluence-embedded-image confluence-content-image-border" width="515" height="279" />

***<span style="text-decoration: underline;">After Change</span>***:
<img src="attachments/47449898/47349918.jpg" class="confluence-embedded-image confluence-content-image-border" width="520" height="348" />

<span id="PersistentTables-_Toc201147859" class="confluence-anchor-link"></span>Persistent Tables in System
===========================================================================================================

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Tables</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>gp_relation_node</p></td>
<td align="left"><p>It maps the pg_class objects and the persistent tablke objects.</p></td>
</tr>
<tr class="even">
<td align="left"><p>gp_persistent_relation_node</p></td>
<td align="left"><p>It consists all the persistent relation entries.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_persistent_database_node</p></td>
<td align="left"><p>It consists all the persistent database entries.</p></td>
</tr>
<tr class="even">
<td align="left"><p>gp_persistent_tablespace_node</p></td>
<td align="left"><p>It consists all the persistent tablespace entries.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_persistent_filespace_node</p></td>
<td align="left"><p>It consists all the persistent filespace entries.</p></td>
</tr>
</tbody>
</table>

In the above shown table, gp\_relation\_node is a regular system catalog table whereas others are special tables designed specifically for tracking and storing persistent information.

<span id="PersistentTables-_Toc201147860" class="confluence-anchor-link"></span>Row Structure
---------------------------------------------------------------------------------------------

The typical row information (identified by a TID) contained in the above tables is as shown below:

<table style="width:100%;">
<colgroup>
<col width="16%" />
<col width="16%" />
<col width="16%" />
<col width="16%" />
<col width="16%" />
<col width="16%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>File System Object Name</p></th>
<th align="left"><p>Persistent Serial<br />
Number</p></th>
<th align="left"><p>Persistent State<br />
(Primary)</p></th>
<th align="left"><p>Persistent State (Mirror)</p></th>
<th align="left"><p>File Replication info columns</p></th>
<th align="left"><p>TID</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>…</p></td>
<td align="left"><p>…</p></td>
<td align="left"><p>…</p></td>
<td align="left"><p>…</p></td>
<td align="left"><p>...</p></td>
<td align="left"><p>…</p></td>
</tr>
</tbody>
</table>

1.  File System Object Name – The name of the file.
2.  Persistent Serial Number – It’s an identifier for uniquely identifying each record in the Persistent FS object table. The Persistent shared memory always maintains the *maxInUseSerialNumber* allocated on the basis of global sequence values (functions defined in cdbglobalsequence.c). Whenever a new entry is to be made into the Persistent objects, the *maxInUseSerialNumber* is incremented and assigned to the tuple being stored. The statement looks like as follows:
    -   *persistentSerialNumber = ++storeSharedData-&gt;maxInUseSerialNumber*
    -   This is very important identifier as in the system catalogue, the transaction subsystem and various other objects references the persistent row entries through the pair &lt;Persistent TID, Persistent serial Number&gt;
    -   Note: This mechanism is going to be changed in Paris

3.  Persistent State – It represents the current state for the file system object (various possible states shown below).
4.  Mirror State – It represents the current state of the file system object on the mirror side from the primary’s POV.
5.  Data Replication Columns – These consists of some metadata information helping in the replication of the object files across mirror thus used in mirror resynchronization.
6.  TID – This is the tuple identifier used to uniquely identify each record of the table.

<span id="PersistentTables-_Toc201147861" class="confluence-anchor-link"></span>Some Points to Note
===================================================================================================

1.  The initdb process loads the persistent tables once we are out of the bootstrap processes.
2.  Changes to Persistent File System table data are made outside the scope of any running transaction, and thus it could be used even during the post-end-transaction processing to clean up.
3.  Heap tables only ever add a single segment\_file\_num=0 entry to gp\_persistent\_relation regardless of how many segment files there really are. **PersistentRelation\_AddCreated()** is the function for that.
4.  There is a 1:1 matching of physical and logical files for append only segment files. But for heap there is only one file per relation even though it has many files on the disk.
5.  AbortingCreateNeededAction:

-   AbortingCreateNeeded action is used in reference to the Collapsed Create-Drops.
-   We do a CREATE-DROP collapsing when both are at or below the requested transaction nest level.
    -   Here we set flag dropForCommit = true, indicating CREATE marked as DROP. (True --&gt; delete at commit(special case) and False --&gt; delete at abort)
    -   Also set the flag sameTransCreateDrop = TRUE, indicates collapsed CREATE-DROP
    -   Now if the transaction goes through the function "*PersistentEndXactFileSysAction PendingDelete\_Action() in smgr.c*" to identify the action that’s being performed by transaction. The code is as follows:

``` theme:
PersistentEndXactFileSysAction PendingDelete_Action()

{

if (pendingDelete->dropForCommit)

        {

                return (pendingDelete->sameTransCreateDrop ?

                                                PersistentEndXactFileSysAction_AbortingCreateNeeded :

                                                PersistentEndXactFileSysAction_Drop);

        }

        else

                return PersistentEndXactFileSysAction_Create;

}
```

-   In above code, if the dropForCommit is not set(FALSE), it means that this transaction is regular CREATE type of transaction and it return the action PersistentEndXactFileSysAction\_Create (meaning that go to CREATED(from Create Pending) on commit and Aborting Create(from Create Pending) on abort).
-   On the other hand if the dropForCommit is set(TRUE), it means that this transaction is DROP type of transaction.
    -   If sameTransCreateDrop = TRUE (Collapsed Create-Drop)
        -   Thus this is a Special DROP, which was originally CREATE and has been marked as DROP. And so we need to counter for this special CREATE (or special DROP) with a PersistentEndXactFileSysAction\_AbortingCreateNeeded type of action since it is originally a CREATE transaction.But on commit it should delete the file(since collapsed). The action PersistentEndXactFileSysAction\_AbortingCreateNeeded means to go Aborting Create on both commit and abort. This is different than the normal CREATE action (PersistentEndXactFileSysAction\_Create) where it goes to Aborting Create only on abort transaction.
        -   If sameTransCreateDrop = FALSE (Non Collapsed)
            -   It means that it a non collapsed, normal drop statement and it returns PersistentEndXactFileSysAction\_Drop(meaning, go to DROP\_PENDING(from CREATED)  on commit and do nothing on abort).

The code for this above 2 points can be seen any *smgrDoDeleteActions*() in *smgr.c*.

Note: The fault *finish\_prepared\_transaction\_abort\_pass1\_aborting\_create\_needed* is injected to catch this state change in the code.

<span id="PersistentTables-_Toc201147862" class="confluence-anchor-link"></span>State Diagrams
==============================================================================================

<span id="PersistentTables-_Toc201147863" class="confluence-anchor-link"></span>Persistent File-System Object States.
---------------------------------------------------------------------------------------------------------------------

The various persistent object states are:

-   PersistentFileSysState\_Free = 0,
    -   This state indicates that the file system object is deleted or the transaction has been aborted(after create statement). This state is generally after the object has been deleted or non-existing. Transfers from AC to Free or DP to Free.

<!-- -->

-   PersistentFileSysState\_CreatePending = 1,
    -   Indicates the transaction has initiated the creation of this object but not yet committed. The object is becomes visible in the System Catalog table only once the transaction is committed i.e. when Master executes the ForgetCommitted() after receiving replies from all the segment databases.but the physical file is still not created.

<!-- -->

-   PersistentFileSysState\_Created = 2,
    -   Indicates that the object file has been actually created on the File system. On transaction commit, the state changes from CP to Created.

<!-- -->

-   PersistentFileSysState\_DropPending = 3,
    -   Indicates a transaction has initiated the dropping of this object but not yet and has been committed. The object file still ismay be present on the file system but the object entry is not visible in the system catalog so no one could refer to it since its invisible on the system catalog. Transfers from Created to Drop Pending.

<!-- -->

-   PersistentFileSysState\_AbortingCreate = 4,
    -   Indicates that the transaction that was trying to create an object has been aborted. Transfer from CP to AC.

<!-- -->

-   PersistentFileSysState\_JustInTimeCreatePending = 5
    -   (Doubt)

<!-- -->

-   PersistentFileSysState\_BulkLoadCreatePending,
    -   This is used for the bulk copy of files.

<span style="color: rgb(79,129,189);">**Figure 1: State Diagram**</span>
<img src="attachments/47449898/47349922.jpg" class="confluence-embedded-image" width="700" height="900" />

<span id="PersistentTables-_Toc201147864" class="confluence-anchor-link"></span>Mirror related states
-----------------------------------------------------------------------------------------------------

-   MirroredObjectExistenceState\_None = 0,

<!-- -->

-   MirroredObjectExistenceState\_NotMirrored = 1,
    -   indicates that no mirrors are configured for the system.

<!-- -->

-   MirroredObjectExistenceState\_MirrorCreatePending = 2,
    -   same as PersistentFileSysState\_CreatePending but for Mirror side.

<!-- -->

-   MirroredObjectExistenceState\_MirrorCreated = 3,
    -   same as PersistentFileSysState\_Created but for Mirror side.

<!-- -->

-   MirroredObjectExistenceState\_MirrorDownBeforeCreate = 4,
    -   indicates that the Mirror was down before the transaction that initiated the create on the primary initiated. This will tell the system to resyn the object on mirror once its up and running.

<!-- -->

-   MirroredObjectExistenceState\_MirrorDownDuringCreate = 5,
    -   indicates that the mirror went down or crashed while the transaction doing create was executing. This state will tell the system to resync the object on the mirror side once its up and running.

<!-- -->

-   MirroredObjectExistenceState\_MirrorDropPending = 6,
    -   same as PersistentFileSysState\_DropPending but on mirror side.

<!-- -->

-   MirroredObjectExistenceState\_OnlyMirrorDropRemains = 7,
    -   indicates that the mirror was either down or went down while the transaction dropping the object was executing. Also it says that the object has been deleted on the primary side but not on the mirror side.

<span id="PersistentTables-_Toc201147865" class="confluence-anchor-link"></span>Database Actions
================================================================================================

#### <span id="PersistentTables-_Toc201147866" class="confluence-anchor-link"></span>CREATE OBJECT

-   When we issue a create command:
    -   An entry is added to the System catalog.
    -   Persistent table entry with state Create Pending.
    -   The Database physical file is created.
    -   On Commit, Persistent State changes to Created.
    -   COMMIT RECORD is flushed
    -   On Abort, the Persistent state changes to Aborting Create
    -   On Crash before commit, the the state Create Pending would tell the system to . And also the system will know that it will have to delete the database object that was may(or may not) have been created.

#### <span id="PersistentTables-_Toc201147867" class="confluence-anchor-link"></span>DROP OBJECT

-   When we issue a drop command:
    -   The entry is deleted from the System Catalog table (logical deletion).
    -   On Commit, COMMIT RECORD is flushed to disk for the object. The state is changed from Created to Drop Pending.
    -   On Post CommitThen, the database file is actually deleted in from the disk and state is changed from Created Drop Pending to Drop Pending to Free. Here the file is actually deleted after commit since the transaction could abort and aborted transactions do not modify any data.
    -   On Abort or crash before commit, all changes to system catalog are reverted and state remains same (Created) as was before the transaction started.

<span id="PersistentTables-_Toc201147868" class="confluence-anchor-link"></span>Scenarios
=========================================================================================

The following are the various scenarios shown for understanding how the persistent tables state changes and how it manages to recover consistently after a database crash or aborted transaction.

<span id="PersistentTables-_Toc201147869" class="confluence-anchor-link"></span>Create object (Sync)
----------------------------------------------------------------------------------------------------

The object can be table (heap or AO), index, database, filespace or tablespace.

<img src="attachments/47449898/47349923.jpg" class="confluence-embedded-image" width="700" height="900" />

<span id="PersistentTables-_Toc201147870" class="confluence-anchor-link"></span>Create Object (Change Tracking)
---------------------------------------------------------------------------------------------------------------

<img src="attachments/47449898/47349924.jpg" class="confluence-embedded-image" width="700" height="900" />

<span id="PersistentTables-_Toc201147871" class="confluence-anchor-link"></span>Drop Object (Sync) 
---------------------------------------------------------------------------------------------------

<img src="attachments/47449898/47349925.jpg" class="confluence-embedded-image" width="700" height="900" />

<span id="PersistentTables-_Toc201147872" class="confluence-anchor-link"></span>Drop Object (Change Tracking Mode)
------------------------------------------------------------------------------------------------------------------

<img src="attachments/47449898/47349926.jpg" class="confluence-embedded-image" width="747" height="1117" />

<span id="PersistentTables-_Toc201147873" class="confluence-anchor-link"></span>AO inserts
------------------------------------------------------------------------------------------

<img src="attachments/47449898/47349927.jpg" class="confluence-embedded-image" width="1002" height="652" />

<span class="aui-icon icon-hint">Icon</span>
***Exception*** - If the transaction inserting the rows is in progress while the resync is being done then those will get resynchronized during the COMMIT and not during the RESYNC action.

<span id="PersistentTables-_Toc201147874" class="confluence-anchor-link"></span>Crash Recovery
==============================================================================================

-   Crash Recovery uses the information stored in the persistent tables to cleanup the objects created/dropped across primary-mirror interface to maintain synchronization between the two.
-   When we flush the WAL to disk and if the database is crashed, then this change is visible, when the system is up and running and thus it could take appropriate actions to get the system back in sync.

<span id="PersistentTables-_Toc201147875" class="confluence-anchor-link"></span>Primary Segment Scenarios without Mirroring.
----------------------------------------------------------------------------------------------------------------------------

-   <span style="text-decoration: underline;">Scenario 1:</span> While creating an object, System crashes before commit and the PS = Create Pending and there is no commit record for this transaction in WAL.
    -   Since the transaction was not committed, System Catalog entry = invisible for the object.
    -   The physical file may or may not have been created. If created, it would not be able to access the same because system catalog doesn’t have any references to it.
    -   Thus, during recovery, system would see that the PS = CreatePending but there is no COMMIT record in the WAL logs and hence concludes to delete the physical file, if existed.

<!-- -->

-   <span style="text-decoration: underline;">Scenario 2</span>: While creating an object, System crashes after commit and the PS = Create Pending and there is no commit record for this transaction in WAL.
    -   Can be there such a case? What if crash was between commit and state is actually changed? Yes, master can commit and sends the commit to segments and before the segments commits and informs master, they crashes leaving the commit inconsistent.
    -   IF its in PS = Created then the system is consistent.

<!-- -->

-   <span style="text-decoration: underline;">Scenario 3:</span> While Dropping an object, System crashes after the commit but before PS was updated in Persistent table thus keeping the PS = Created (as before).
    -   Since the transaction was committed, the drop was successful and hence System Catalog entry – invisible for the object.
    -   But the actual deletion of the database object file is done in post commit processing and so the physical file may or may not have been deleted/dropped till now.
    -   During recovery, if system would see that the PS = Created and there is COMMIT on the same object for drop transaction in the WAL logs, it concludes that the object needs to be physically removed since it has been deleted logically from system catalog table and no one could reference the same.

<!-- -->

-   <span style="text-decoration: underline;">Scenario 4:</span>  While Dropping an object, System crashes before the commit but before PS was updated in Persistent table thus keeping the PS = Created (as before).
    -   Since all the changes like Persistent state change and actual deletion is done in post commit processing, nothing changes in this scenario.

<span id="PersistentTables-_Toc201147876" class="confluence-anchor-link"></span>Mirroring Scenarios
---------------------------------------------------------------------------------------------------

-   <span style="text-decoration: underline;">Scenario 1:</span> While creating the object, the mirror goes down and transaction gets committed.
    -   Since the mirror was down during the create, the Mirror existence state goes to “Mirror Down Before Create” or “Mirror Down During Create” depending upon the phase at which mirror went down.
    -   Since the transaction got committed, the persistent state is “Created”.
    -   So thus during the crash recovery, the system gets to know that the mirror was down and so physical file needs to created on mirror side, thus it creates the same and updates the state to “Created”, thus resynchronizing the system.

<!-- -->

-   <span style="text-decoration: underline;">Scenario 2:</span> While dropping the object, the mirror goes down and transaction gets committed.
    -   Since the mirror was down during the drop, the Mirror existence state goes to “Mirror Down Before Create” or “Mirror Down During Create” depending upon the phase at which mirror went down.
    -   Since the transaction got committed, the persistent state is “Drop Pending” and the Mirror Existence state changes to “Only Mirror Drop Remains”.
    -   So thus during the crash recovery, the system gets to know that the mirror was down and so physical file needs to be deleted on mirror side, thus it deletes the same and updates the state to “DroppedàFree, thus resynchronizing the system.

<!-- -->

-   <span style="text-decoration: underline;">Scenario 3:</span> While creating the object, the mirror goes down and transaction gets aborted.
    -   Since the mirror was down during the create, the Mirror existence state goes to “Mirror Down Before Create” or “Mirror Down During Create” depending upon the phase at which mirror went down.
    -   Since the transaction got aborted the Persistent state would be in “Aborting Create”.
    -   So thus during crash recovery, the system gets to know that there is no need to sync this object to mirror even though its down since the transaction was aborted.

<!-- -->

-   <span style="text-decoration: underline;">Scenario 4:</span> While dropping the object, the mirror goes down and transaction gets aborted.
    -   Since the mirror was down during the drop, the Mirror existence state goes to “Mirror Down Before Create” or “Mirror Down During Create” depending upon the phase at which mirror went down.
    -   Since the transaction got aborted the Persistent state would be in “Created” .
    -   So thus during crash recovery, the system gets to know that there is no need to sync this object to mirror even though its down since the transaction was aborted.

<span id="PersistentTables-_Toc201147877" class="confluence-anchor-link"></span>Troubleshooting and Repair
==========================================================================================================

<span class="aui-icon icon-hint">Icon</span>
**Gpcheckcat** is the tool used for consistency checks among the catalog tables, persistent tables and objects present in the File system. Any inconsistencies are reported along with the repair file to create/drop file object on the respective segment.

<span id="PersistentTables-_Toc201147878" class="confluence-anchor-link"></span>
================================================================================

Code Walkthrough
================

This code walk through is done for “relations”. For database, filespace and tablespace the respective files would be cdbpersistentdatabase.c, cdbpersistenttablespace.c and cdbpersistentfilespace.c. Similar is the case with many of the functions described in those files.For Create heap table / append-only table / index:

<span id="PersistentTables-_Toc201147879" class="confluence-anchor-link"></span>
--------------------------------------------------------------------------------

**<span style="text-decoration: underline;">During Prepare Transaction Phase:</span>**

``` theme:
* Cdbpersistentrelation.c
* Most of the code during the prepare phase of the transaction is in  this file and hence its worth looking at some of the important  functions in it.
* PersistentRelation_CurrentMaxSerialNum()
** Return the current in max to be used serial number
* PersistentRelation_AddCreatePending()
** This is to indicate the intent to create a relation file as a  part of the current transaction. This function creates a XLOG  IntendToCreate record to guard the subsequent file system create in case  of transaction aborts. After 1 or more calls to this routine, we call DoPendingCreates() to do actual file system creates.
** Calls the PersistentFileSysObj_AddTuple() form cdbpersistentstore.c to actually form and add a tuple in the table.
** Eg: add a entry corresponding to relid(17004) in 'Create Pending' state in gp_persistent_relation_node.
* PersistentRelation_Created()
** Indicate the transaction commited and the relation is officially created.
```

**<span style="text-decoration: underline;">During Commit Transaction Phase</span>**

``` theme:
* Persistentendxactrec.c
** Most of the code concerning the persistent tables in commit phase  is executed from this file. This file make and read the persistent  information in commit, abort and prepared records.
** smgrGetPendingFileSysWork() -- Get a list of relations that have  post-commit or post-abort work, This call the  EndXactRecKind_NeedsAction() from Persistentendxactrec.c file to choose a  proper action of create, drop, abortingcreate.
*** For "create", the variable {color:#0000c0}{_}PersistentEndXactFileSysAction_Create{_}{color} is set.
**** Created on commit and Aborting create & drop on abort.
*** For "Aborting Create" the variable {color:#0000c0}{_}PersistentEndXactFileSysAction_AbortingCreateNeeded{_}{color} is set.
**** Clean up required on commit and abort.

** CommitTransactionCommand()/AbortTransactionCommand() from xact.c
*** CommitTransaction() or AbortTransaction() from xact.c
**** CommitTransaction()
***** RecordTransactionCommit()
****** +PersistentEndXactRec_FetchObjectsFromSmgr+ () to get the data for commit record
******* smgrGetPendingFileSysWork (): gets a list of relation thathave post commit/abort work.
******** EndXactRecKind_NeedsAction():       choose a proper action of create, drop, abortingcreate. For "create", the variable {color:#0000c0}{_}PersistentEndXactFileSysAction_Create{_}{color} is set.
******* PersistentEndXactRec_VerifyFileSysActionInfos():
****** *PersistentEndXactRec_Serialize()*
****** PersistentEndXactRec_Print()
** This actually creates a Physical file after the commit happens.

* Cdbpersistentfilesysobj.c
** FinishPreparedTransaction()
*** PersistentEndXactRec_Deserialize()
*** PersistentFileSysObj_PreparedEndXactAction()
**** Here we need to do the transition to Aborting create or drop  pending and then perform the file system drop while under mirrored lock,  else race condition with resynchronize's redrop.
**** End transaction changes:
***** For case "Create"
****** If(isCommit), Change Create Pending to Created by calling PersistentFileSysObj_Created()
****** Else, create pending to aborting create state by calling PersistentFileSysObj_MarkAbortingCreate()
* Flush the above changes: PersistentFileSysObj_FlushXLog()
```

<span id="PersistentTables-_Toc201147880" class="confluence-anchor-link"></span>Drop heap table / append-only table / index
---------------------------------------------------------------------------------------------------------------------------

**<span style="text-decoration: underline;">During Prepare Transaction Phase:</span>**

``` theme:
* Cdbpersistentrelation.c
** PersistentRelation_CurrentMaxSerialNum()
*** Return the current in max to be used serial number
** PersistentRelation_MarkDropPending()
**** Inputs
***** Relfilenode
***** Segment file number
***** persistentTid
***** persistent serial number
**** This is to indicate the intend to drop a relation file as a part  of the current transaction. This relation file to drop will be listed  inside a commit, distributed commit, a distributed  prepared, and  distributed commit prepared XOG records.
**** For any of the commit type records, once that XLOG record is  flushed then the actual file-system delete will occur.  The flush  guarantees the action will be retried after system crash.
*** Calls the PersistentFileSysObj_StateChange() function to check  for the expected state that should be present currently, also the new  state that should be updated (_drop pending_ in this case).
```

**<span style="text-decoration: underline;">During Commit Transaction Phase:</span>**

``` theme:
* Persistentendxactrec.c
** Most of the code concerning the persistent tables in commit phase  is executed from this file. This file make and read the persistent  information in commit, abort and prepared records.
** smgrGetPendingFileSysWork() -- Get a list of relations that have  post-commit or post-abort work, This call the  EndXactRecKind_NeedsAction() from Persistentendxactrec.c file to choose a  proper action of create, drop, abortingcreate.
*** For "drop", the variable {color:#0000c0}{_}PersistentEndXactFileSysAction_Drop{_}{color} is set.
**** Drop on commit.
*** For "Aborting Create" the variable {color:#0000c0}{_}PersistentEndXactFileSysAction_AbortingCreateNeeded{_}{color} is set.
**** Clean up required on commit and abort.

** CommitTransactionCommand()/AbortTransactionCommand() from xact.c
*** CommitTransaction() or AbortTransaction() from xact.c
**** CommitTransaction()
***** PersistentEndXactRec_WillHaveObjectsFromSmgr() from peristentendxactrec.c
****** EndXactRecKind_NeedsAction() smgrIsPendingFileSysWork from smgr.c

** CommitTransactionCommand()/AbortTransactionCommand() from xact.c
*** CommitTransaction() or AbortTransaction() from xact.c

**** CommitTransaction()
***** RecordTransactionCommit()
****** +PersistentEndXactRec_FetchObjectsFromSmgr+ () to get the data for commit record
******* smgrGetPendingFileSysWork (): gets a list of relation that have post commit/abort work.
******** EndXactRecKind_NeedsAction():       choose a proper action of create, drop, abortingcreate. For "drop", the variable {color:#0000c0}{_}PersistentEndXactFileSysAction_Drop{_}{color} is set.
******* PersistentEndXactRec_VerifyFileSysActionInfos():
****** *PersistentEndXactRec_Serialize()*
****** PersistentEndXactRec_Print()

* Cdbpersistentfilesysobj.c
** FinishPreparedTransaction()
*** PersistentEndXactRec_Deserialize()
*** PersistentFileSysObj_PreparedEndXactAction()
**** Here we need to do the transition to Aborting create or drop  pending and then perform the file system drop while under mirrored lock,  else race condition with resynchronize's redrop.
**** End transaction changes:
****# For case "Drop"
****## IF(isCommit): Created to Drop Pending by calling PersistentFileSysObj_MarkDropPending()
* Flush the above changes to xlog: PersistentFileSysObj_FlushXLog()
* &nbsp;
* If(abortCreate \|\| dropPending)
** PersistentFileSysObj_EndXactDrop()
*** PersistentFileSysObj_DropObject()
**** PersistentFileSysObj_ReadTuple()
**** PersistentFileSysObj_ReadTuple()
**** pfree(values)
**** heap_freetuple(tupleCopy)
**** MirroredFileSysObj_DropRelFile() : drop on mirror
**** *PersistentRelation_Dropped():* {color:#ff0000}{*}This indicate we physically removed the file.*{color}
***** PersistentFileSysObj_StateChange(): changes the state in the persistent table
```

<span id="PersistentTables-_Toc201147881" class="confluence-anchor-link"></span>Crash Recovery
----------------------------------------------------------------------------------------------

-   This majorly takes place in the PASS-2 of the xlog recovery process(xlog.c)
-   **StartupXLOG\_Pass2()è**
    -   PersistentRecovery\_HandlePass2XLogRec()
        -   For a transaction module, xlog record, it gets the xaction\_type and its kind info viz..,
            -   XLOG\_XACT\_COMMIT - *XACT\_INFOKIND\_COMMIT*
            -   XLOG\_XACT\_ABORT - *XACT\_INFOKIND\_ABORT*
            -   XLOG\_XACT\_PREPARE - *XACT\_INFOKIND\_PREPARE*
            -   XLOG\_XACT\_COMMIT\_PREPARED - *XACT\_INFOKIND\_COMMIT*
            -   XLOG\_XACT\_ABORT\_PREPARED - *XACT\_INFOKIND\_ABORT*
            -   XLOG\_XACT\_DISTRIBUTED\_COMMIT - *XACT\_INFOKIND\_COMMIT*
        -   Based on this infokind, the new states of persistent file system are recognized. Generally we keep track of committed, aborted and prepared xacts so that CP objects don’t get automatically aborted by crash recovery.
        -   Based on its kind, it creates the Persistent change entry keyed on TID (**PersistentRecovery\_AddEndXactFsObj())**
        -   Only For *XACT\_INFOKIND\_COMMIT*
            -   Calls PersistentRecovery\_AddAppendOnlyMirrorResyncEofs()
            -   Adds the xid and other eof information(<span style="text-decoration: underline;">mirrorLossEof, mirrorNewEof..) to the</span> recoveryAppendOnlyMirrorResyncEofsTable.
        -   PersistentRecovery\_Scan()
        -   Calls PersistentRecovery\_ScanType()
            -   for all types(i.e. Relation, dbs, tablsapces, filespaces) to check if recovery is needed in the those type of objects.
            -   Calls PersistentRecovery\_ScanTuple()
                -   Calls persistentRecovery\_AddScanEntry()
                    -   Checks for the existence of the persistenceTID with the function PersistenceRecovery\_FindOrCreareFsObjectEntry() and if not present creates a Persistence Change Entry. This function also handles the state changes by comparing with what is already present with the new fsObjEntry.
                    -   If it finds an entry then it Scan the states – 3 cases:
                        -   Newer stateZ(fsObj-&gt;state &lt; state)
                            -   **fsObjEntry-&gt;state = state;**
                            -   **fsObjEntry-&gt;mirrorExistenceState = mirrorExistenceState;**
                            -   **fsObjEntry-&gt;xid = parentXid;**
                            -   **older state**(fsObj-&gt;state &lt; state)
                                -   **--------------**
                                -   **same state**
                                    -   **fsObjEntry-&gt;mirrorExistenceState = mirrorExistenceState;**
                                    -   During crash recovery when we drop objects, we should skip the objects whose mirror existence state is MirrorDropRemains. It will be dropped during resync.

<!-- -->

-   PersistentRecovery\_CrashAbort
    -   Checks for transactions that needs to be aborted, like the ones lying in the create pending mode and gets it count(**createPendingCount**) and sets  them to “Aborting Create” state.
    -   PersistentRecovery\_Update()
        -   For each of the object type, calls PersistentRecovery\_UpdateType()
            -   It updates all the persistent entries with any updates that are required. It marks the file sys obj state to fsObjEntry-&gt;state.
        -   PersistentRecovery\_Drop()
            -   It drops(PersistentFileSysObj\_DropObject()) the object whichever are marked as Aborting Create and Drop Pending. Also if the mirror existence state = *MirroredObjectExistenceState\_OnlyMirrorDropRemains* then the corresponding object os not dropped since the object will automatically be dropped in resync operation and so need not be handled in crash recovery.
            -   Calls
        -   PersistentRecovery\_UpdateAppendOnlyMirrorResyncEofs()
            -   It handles the syncing of the cimmit and loss eof.
            -   If(eofsEntry == -1) mirror catch up required, else if(newEof != lossEof) then not required.
            -   Calls PersistentFileSysObj\_UpdateAppendOnlyMirrorResyncEofs()
                -   Takes care of updating the mirrorNewEofs

<span id="PersistentTables-_Toc201147882" class="confluence-anchor-link"></span>Important GUCs
==============================================================================================

-   Debug\_persistent\_store\_print
-   Debug\_persistent\_appendonly\_commit\_count\_print
-   debug\_persistent\_recovery\_print=true'
-   'debug\_persistent\_recovery\_print\_level=log'
-   'debug\_persistent\_print=true'
-   'debug\_persistent\_print\_level=log'
-   'debug\_print\_xlog\_relation\_change\_info=true'

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [worddav4cf944fb6fbbe7a5a55e26cc4067282b.png](attachments/47449898/47349913.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [worddav500824c39ef2c7d1840538c3b5ea4d90.png](attachments/47449898/47349914.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Before Change.jpg](attachments/47449898/47349930.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.png](attachments/47449898/47349917.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.png](attachments/47449898/47349919.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.jpg](attachments/47449898/47349921.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.png](attachments/47449898/47349920.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.png](attachments/47449898/47349916.png) (image/png)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.jpg](attachments/47449898/47349935.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [State Diagram.jpg](attachments/47449898/47349922.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Create Object sync.jpg](attachments/47449898/47349931.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Create Object changetracking.jpg](attachments/47449898/47349934.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Drop Object sync.jpg](attachments/47449898/47349933.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Drop Object changetracking.jpg](attachments/47449898/47349932.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [AO Inserts.jpg](attachments/47449898/47349929.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [AO Inserts.jpg](attachments/47449898/47349927.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Before Change.jpg](attachments/47449898/47349915.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Create Object sync.jpg](attachments/47449898/47349923.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Drop Object changetracking.jpg](attachments/47449898/47349926.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Drop Object sync.jpg](attachments/47449898/47349925.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Create Object changetracking.jpg](attachments/47449898/47349924.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [After Change.jpg](attachments/47449898/47349918.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [primary\_mirror\_transition.docx](attachments/47449898/73990151.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)

Document generated by Confluence on May 17, 2016 19:14


