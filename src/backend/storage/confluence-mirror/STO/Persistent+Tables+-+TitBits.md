<span id="title-text"> Storage & Access : Persistent Tables - TitBits </span>
=============================================================================

This page last changed on Apr 14, 2015 by petang.

[PT-Presentation.pptx](attachments/49147264/56656448.pptx)

### **<span style="text-decoration: underline;">Introduction</span>**

Persistent tables (PT) has been an area of concern in the field & in-house and hence there is an opportunity that needs to be explored to stabilize it. So, this is an effort to stabilize it by studying the internals, looking different areas of impact, implementing re-modeling of code modules, enhancements, new tools to diagnose and recover from PT issues etc.

------------------------------------------------------------------------

### **<span style="text-decoration: underline;">Open Questions</span>**

1. Rationale behind PT serial \# ? There is a note under the PT document under storage wiki that talks about changes in PARIS ?

2. Like other PT, gp\_relation\_node works in non-MVCC mode (frozen). If observed this table maintains "create\_mirror\_data\_loss\_tracking\_session\_num, persistent\_tid, persistent\_serial\_num" (this info is available in gp\_persistent\_relation\_node)  with relfilenode\_oid,segnum. It has an unique index based on relfilenode\_oid,segnum as the key. I think this must have been done to make access to most frequently accessed fields like PTID,PSNUM. If I'm not wrong in my understanding on this, should we have an index on top of 'gp\_persistent\_relation\_node' and get the same functionality ?

\[Answer\] 'gp\_relation\_node' is a regular heap table without any frozen modifications. On the other hand 'gp\_persistent\_relation\_node' is a frozen table which is frequently accessed to either retrieve or modify info about a given relation/seg file combination. Tuples in this table can be efficiently retrieved by using explicitly stored PTTID (ctid of 'gp\_persistent\_relation\_node' tuples). To find a PTTID given a uniqe combinition of relfilenode/segnum we need an efficient mechanism.  This need is satisfied by 'gp\_relation\_node' along with a unique index on top of it which is based on keys relfilenode and seg number. This extra table is needed as frozen tables can't have indexes and thus we can't index 'gp\_persistent\_relation\_node' table.

3. What is use of parentXid in 'gp\_persistent\_relation\_node' ?

\[Answer\] As per my (AshwinA) understanding, its used to check during recovery the Status (committed/aborted) for transaction creating the entry. Simple example to explain, while adding entry in "Create Pending" state the parentXID would store XID of transaction creating the relation, then if crash happens during recovery it will check using this parentXID if Xact is committed / aborted and based on that take action to move the State to "Created" or cleanup the physical file. Note: while marking entry in state "Created" parentXID is set to 0.

4. Persistent Write takes LOCKS in order (MIRROR \[shared\], CHECKPOINT\_START \[shared\], PERSISTENT \[exclusive\], START\_CRITICAL\_SECTION). And Release the locks in same order (MIRROR \[shared\], CHECKPOINT\_START \[shared\], PERSISTENT \[exclusive\], END\_CRITICAL\_SECTION) why the locks are not released in reverse order (END\_CRITICAL\_SECTION, PERSISTENT \[exclusive\], CHECKPOINT\_START \[shared\], MIRROR \[shared\]) which generally is the practice when dealing with multiple locks ?

5. Why do we check if lock is acquired by me (process) and only if not go ahead to acquire the lock? Is it for performance else locking module should be bale to handle the case fine, why we need to explicitly check from module. Plus, even if we do check shouldn't we use the function LWLockHeldExclusiveByMe and not LWLockHeldByMe (which doesn't differentiate between shared and exclusive lock being held).

\[Answer:HH\] If we try to acquire the same LWLock we held before, it is a PANIC, so we need to check if we held it already.  The reason why LWLock is designed so is because LWLock is not expected to be held so long.  If you go to README file in backend/storage/lmgr, it explains why.  Basically LWLock doesn't provide rich functionality such like dead lock detection and it holds interrupt, so you shouldn't hold it too long.  So the need for the check "if it's held by me" indicates the design is wrong.

6. Fix / code review link in MPP-18228, suggests there was major refactoring of LOCKING done to PT but due to some issues it was not committed. Please can we know what was the rational behind need to refactor PT locking specifically where related to START and END of CRITICAL\_SECTION? Why it was not finally done and what alternative fix was committed? Can we plan to take in Paris now?

\[Answer\] As per my knowledge, START/END\_CRITICAL SECTION are still a part of macro WRITE\_PERSISTENT\_STATE\_ORDER\_LOCK/UNLOCK due to which there was a larger code under critical section than require and in some cases even lesser code was under this locks/crit section(one such case was MPP-18228) where we have to move a function inside this MACRO.

Initially Matt and I\[AshwinP\] was working on this, the motive was to make the 2 functions atomic, and since the function to be brought under CRITICAL SECTION has a memory allocation(which can fail), it was decided to seperate  the START/END\_CRITICAL SECTION from the macro WRITE\_PERSISTENT\_STATE\_ORDER\_LOCK/UNLOCK so that a error wont cause a postmaster reset(PANIC). Hence the refactoring of code was done, where we had separated the critical section from the locks and had taken them explicitly when needed in all the needed files. But the code change was very big, QA effort was large for the code to go in 4.2.3 and so it was decided to not separate the locks and critical section and just bring in the function inside the critical section even if it has memory allocation, and so all changes were not commited. I still have  the branch and the code changes, we can take a look at the same.

7. Need to give detailed thought but still missing to understand the rational behind two-step Drop sequence of objects with respect to persistent tables. In step-1 changes the PT entry in AbortingCreate/DropPending, flushes the Xlog and then step-2 actually Drops the object and marks the entry Free/Dropped ?

8. Why master performs UTILITY mode and then DISPATCH mode recovery post shutdowns/ interruptions ? Why just DISPATCH mode auto-recover on PANICs ?

9. Why DTM recovery / redo is only performed in UTILITY mode and not in DISPATCH mode ? 

------------------------------------------------------------------------

### **<span style="text-decoration: underline;">Suggestions, Ideas, Enhancements</span>**

1.  <span style="color: rgb(255,0,0);">PT GUCs have been an issue consuming large amount of space due to each and every table including catalog accessed/modified as part of a (un)successful transaction. An enhancement which can make such logs configurable should be implemented. A framework something on similar lines as logging for tuple VISIBILITY can be used.</span>
2.  As we know the changes to persistent tables are frozen(committed then and there), so we'll have to take a look at the code path that does the same. As in example, I was working on the JIRA which does Add\_CreatePendingentry() and Add\_PendingDeleteListentry() one after the other,but was not done as a atomic operation due to which failure in between the two functions caused inconsistency, So LOCKS and CRITICAL\_SECTIONS should also be our prime focus for this, as this can prevent us from many of the inconsistencies.
3.  <span style="color: rgb(255,0,0);">WatchDog process to continuously monitor the consistency of PT. Currently most of the PT problems are detected during recovery, so might be better idea to see if can catch the problems earlier if possible to implement a low-priority process checking for PT consistency. Could be just used in-house in DEBUG env only. Also, can evaluate if gpcheckcat can provide such facility.</span> <span style="color: rgb(255,0,0);">**(I like this idea - Raghav)**</span> <span style="color: rgb(255,0,0);">Integrity checker.</span>\#\# <span style="color: rgb(255,0,0);">gp\_relation\_node has an index on top of it. A check should be added</span> <span style="color: rgb(255,0,0);">**after**</span> <span style="color: rgb(255,0,0);">the index lookup fails. This check will  verify that the relation-segnum combination is</span> <span style="color: rgb(255,0,0);">**actually**</span> <span style="color: rgb(255,0,0);">not present in the 'gp\_relation\_node' table. This will help us to avoid catastrophic data loss in case the index screws up.</span> 
    1.  <span style="color: rgb(255,0,0);">Found this in one of JIRA's quoted by Matt hence adding to list, Can add more integrity checks in dbcommands.c:dropdb to detect that there is a missing gp\_relation\_node entry (that has the persistent TID) for a pg\_class entry. Thus we can fail the DROP DATABASE command with a better error message indicating a missing gp\_relation\_node entry before getting to the prepare phase because the ERROR is a lower level consistency check. Then the question becomes how to remove the database when DROP DATABASE fails due to integrity checks.One way would be to use a manual procedure that removes the database from pg\_database and the flat file of database OIDs. Then rebuild the persistent tables.</span>
    2.  <span style="color: rgb(255,0,0);">When updating the PT state, verify/assert for its current state to adhere to PT State Diagram</span>
    3.  <span style="color: rgb(255,0,0);">Can we add verification function in persistentendxactrec.c for AppendOnlyMirrorResyncEofs similar to PersistentEndXactRec\_VerifyFileSysActionInfos.</span>
    4.  <span style="color: rgb(255,0,0);">smgrcreate function assumes bunch of stuff (like persistent 'Create Pending' work has already been done, Just-In-Time database directory in the tablespace has already</span><span style="color: rgb(255,0,0);"> been created) good place to assert for these assumptions.</span>
    5.  <span style="color: rgb(255,0,0);">PT are to track life-cycle of an object, issues seen due to duplicate entries in PT for same object. Need to think through if can have keying based on object (OID) instead of TID/serial numbers would avoid duplicates OR to implement something on top of existing to easily check and verify no duplicate entry is getting added.</span>

4.  Coding
    1.  Minor Suggestion - relfilenode\_oid is a little misnomrer. Oid is something that does NOT change. But relfilenode\_oid changes on truncate, alter tables. Should we think on renaming \*\_oid coulmns to something non-ambiguous ?
    2.  See functions being passed huge set of arguments (like GpPersistentRelationNode\_GetValues, GpPersistentRelationNode\_SetDatumValues..) best would be to define structures for the same and just pass them around instead of independent values, makes code simple and much more maintainable.
    3.  Eyes just ROLLED over-over and MIND boggled **470** lines function PersistentFileSysObj\_ScanStateAction, **390** lines function PersistentFileSysObj\_StateChange, **495** lines PersistentFileSysObj\_PreparedEndXactAction :-((((
    4.  Any faults and erros log as much as can , just dump out all the context

5.  <span style="color: rgb(255,0,0);">FreeTID</span>\#\#  <span style="color: rgb(255,0,0);">Why the job of setting previousFreeTid to zero for allocated entries if left to uses of persistentStore, ideally should be done at centralized place for deterministic behavior during PersistentStore\_AddTuple. Users of the persistentStore should not even be aware of anything regarding previousFreeTid, its store specific attribute. So, must be set and read only by persistentStore. Thought / Guess on one of possibilities for issues where inconsistency is seen in freeOrderNum, mostly the difference between expected and found is huge and not +/- one/two which in some sense gives feeling may not be issue of race-condition. The checking if entry is free or not is based on if previousFreeTid is set or not. Since persistentSerialNum is overloaded filed in a sense for normal entries it stores correct serial numbers but for freeEntries it stores the freeOrderNum. So, need to make sure in code and assert that there is no case where previousFreeTid remains set (non zero) for normal non-free tuples. Else regular persistentSerialNum may be treated as freeOrderNum and cause failure in the verification.</span>\#\# <span style="color: rgb(255,0,0);">Need to check if this was fixed or not, Caleb points this in OLD JIRA: Calling PersistentFileSysObj\_FreeTuple() on a TID that has not yet been allocated in the persistent table will error out (this is fine), but not without consequences. Specifically it increases the maxInUseSerialNum which can make later calls insert bad values for persistent\_serial\_num and result in a corrupted freelist.</span>
6.  <span style="color: rgb(51,153,102);">Need to understand semantics behind gpcheckcat (What's recommended env for it to run, how customers are actually using it). What consistency checks gpcheckcat performs relating to PT, how it relates the things together, what's the severity of inconsistencies (are they all fatal issues) or are they some false positives just because of the very nature of PT (frozen inserts/updates)</span>
7.  <span style="color: rgb(51,153,102);">Add protection to gpcheckcat PT tests to run only in offline or with no activity on system. Never to allow online mode / with activity on system</span>
8.  There is a small note in the PT document on storage wiki that says there is a plan to change the way PT serial numbers work. Can someone talk about this ?
9.  Just a point to note: The routine that actually deletes (unlinks) the physical file makes best attempt to delete the physical file, but doesn't ERROR out on failure just issues WARNING. Irrespective of its success/failure the current transaction commits/aborts. So, its kind of behavior which can get flagged later as PT inconsistency with filesystem. Where physical file exist but PT entry doesn't and seems should be treated as NOPE and cleaned.
10. While solving issue around PT and FileRep, we may need offline dump of PT (lets say on mirror) for debugging. Improvement to gp\_filedump to dump PT formatted data (relation, database, tablespace) will be helpful. Plus, distributed and two-phase DTM logs.
11. Some way for EE/support to simplify removing PT inconsistencies like instead of rebuilding full PTs, simple command to just free the extra entries, etc...

------------------------------------------------------------------------

### **<span style="text-decoration: underline;">Important sharable Information (including documents related to PT and its working)</span>**

1. Introduction and working overview of PT - [http://confluence.greenplum.com/display/STO/Persistent+Tables](Persistent%2BTables.md)

2. Rebuilding PT & Other useful troubleshooting titbits - <a href="http://confluence.greenplum.com/display/STO/Storage+Troubleshooting+Notes#StorageTroubleshootingNotes-HowtoRebuildthePersistentTables" class="uri" class="external-link">http://confluence.greenplum.com/display/STO/Storage+Troubleshooting+Notes#StorageTroubleshootingNotes-HowtoRebuildthePersistentTables</a>

3. <a href="http://confluence.greenplum.com/display/QA/Info+-+Persistent+tables+discussion" class="uri" class="external-link">http://confluence.greenplum.com/display/QA/Info+-+Persistent+tables+discussion</a>

4. <a href="http://jira.eng.pivotal.io/browse/QA-934" class="uri" class="external-link">http://jira.eng.pivotal.io/browse/QA-934</a>

5. <a href="http://jira.eng.pivotal.io/browse/QA-935" class="uri" class="external-link">http://jira.eng.pivotal.io/browse/QA-935</a>

6. <a href="http://jira.eng.pivotal.io/browse/QA-936" class="uri" class="external-link">http://jira.eng.pivotal.io/browse/QA-936</a>

7. Private branch of Matt (Separation of CRITICAL\_SECTION and PERSISTENT\_LOCKS) - //cdb2/private/mmccline/main\_rare\_mem\_create\_pending\_issue/...

8. Information on parentXid, gp\_persistent\_node and freeOrderNum can be found at  <span style="text-decoration: underline;"><a href="http://confluence.greenplum.com/display/~pednea1/Persistent+Table+Info" class="uri" class="external-link">http://confluence.greenplum.com/display/~pednea1/Persistent+Table+Info</a></span>

9. Transaction Flow Diagram (storage wiki) - <a href="http://confluence.greenplum.com/download/attachments/42502230/Distributed+Transaction+Protocol+Flow.jpg?version=1&amp;modificationDate=1332108367885" class="uri" class="external-link">http://confluence.greenplum.com/download/attachments/42502230/Distributed+Transaction+Protocol+Flow.jpg?version=1&amp;modificationDate=1332108367885</a>

                                                                       <a href="http://confluence.greenplum.com/download/attachments/42502230/Distributed+Transaction+Protocol+Flow+2.jpg?version=2&amp;modificationDate=1332109176053" class="uri" class="external-link">http://confluence.greenplum.com/download/attachments/42502230/Distributed+Transaction+Protocol+Flow+2.jpg?version=2&amp;modificationDate=1332109176053</a>

10. Persistent Tables state transitions based on possible actions <a href="http://confluence.greenplum.com/pages/viewpageattachments.action?pageId=49147264" class="uri" class="external-link">http://confluence.greenplum.com/pages/viewpageattachments.action?pageId=49147264</a>

11. Under review document on PT and FileRep Interactions (CT and Resync) - [FilRep-PT-Short.docx](attachments/49147264/50167966.docx) [FileRep-PT-Long.docx](attachments/49147264/50167967.docx)

------------------------------------------------------------------------

### <span style="text-decoration: underline;">Issues & JIRAs</span>

Feature JIRA targeted for ParisTx - <a href="http://jira.eng.pivotal.io/browse/MPP-16155" class="uri" class="external-link">http://jira.eng.pivotal.io/browse/MPP-16155</a>

Recent important / relevant issues fixed for PT

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Number</p></th>
<th align="left"><p>Version Fixed</p></th>
<th align="left"><p>Brief Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p><a href="http://jira.eng.pivotal.io/browse/MPP-11179" class="external-link">MPP-11179</a></p></td>
<td align="left"><p>PARIS</p></td>
<td align="left"><p>Redo and Recovery were completely skipped before fix, if no record post checkpoint, causing dangling/stale entries in PT. The issues fixes to perform recovery irrespective of redo record after checkpoint.</p></td>
</tr>
<tr class="even">
<td align="left"><p><a href="http://jira.eng.pivotal.io/browse/MPP-18228" class="external-link">MPP-18228</a></p></td>
<td align="left"><p>PARIS</p></td>
<td align="left"><p>If PendingDelete_AddCreatePendingEntry() hit ERROR, a persistent relation entry with CP state becomes orphan and won't get deleted in AbortTransaction(). This results in segment file missing situation in crash recovery later.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>MPP-11448</p></td>
<td align="left"><p>4.2.0.0</p></td>
<td align="left"><p>Missing to drop the objects and clean-up PT entries during crash recovery</p></td>
</tr>
<tr class="even">
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p>TID (0,0) one</p></td>
</tr>
<tr class="odd">
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
</tr>
</tbody>
</table>

------------------------------------------------------------------------

### <span style="color: rgb(0,102,0);">**<span style="text-decoration: underline;">Patterns of problems</span>**</span>

Reviewed about 100 issues (JIRAs) from the Escalation (From Jun 2012 - Till Date). Some patterns I found are as below --

1. Frequent inconsistencies found during gpcheckat runs for PT tests.
    -- Need to find out why do we run gpchekcat in first place ? Most Esc Jiras dont have any background.
    -- gp\_persistent\_\*\_node state check failed is a common concern.
    -- gp\_persistent\_relation\_node &lt;=&gt; gp\_relation\_node fails in some case
    -- EE team re-builds PT and gpcheckcat runs fine. Then return the system.

One of the queries that exposes inconsistencies

    SELECT p.tablespace\_oid, p.relfilenode\_oid, p.segment\_file\_num,
       case when p.persistent\_state = 0 then 'free'
            when p.persistent\_state = 1 then 'create pending'
            when p.persistent\_state = 2 then 'created'
            when p.persistent\_state = 3 then 'drop pending'
            when p.persistent\_state = 4 then 'abort create'
            when p.persistent\_state = 5 then 'JIT create pending'
            else 'unknown state: ' || p.persistent\_state
       end as persistent\_state,
       case when p.mirror\_existence\_state = 0 then 'mirror free'
             when p.mirror\_existence\_state = 1 then 'not mirrored'
             when p.mirror\_existence\_state = 2 then 'mirror create pending'
             when p.mirror\_existence\_state = 3 then 'mirror created'
             when p.mirror\_existence\_state = 4 then 'mirror down before create'
             when p.mirror\_existence\_state = 5 then 'mirror down during create'
             when p.mirror\_existence\_state = 6 then 'mirror drop pending'
             when p.mirror\_existence\_state = 7 then 'mirror only drop remains'
             else 'unknown state: ' || p.mirror\_existence\_state
        end as mirror\_existence\_state
    FROM gp\_persistent\_relation\_node p
    WHERE (p.persistent\_state not in (0, 2)
           or p.mirror\_existence\_state not in (0,1,3))
      and p.database\_oid in (
        SELECT oid FROM pg\_database WHERE datname = current\_database()
      )

2. During recovery the current EOF is greater or equal to new to be set EOF
  -- EE team sets some GUCS to avoid this part of recovery. Get the system up, rebuild in some cases and clean up corrupted data if any
   -- Similar issues were resolved in past (MPP jiras) due to incorrect snapshots (but not during recovery)
   -- Power outage has been seen around this issue atleast in one case

3. A number of incidents go for gprecoverseg -F/-A and it fails to complete. These dont complete because of some PT issue. Rebuilding PT was the option chosen to get over such issues.

4. Killing processes on either Master or Segment by the customer causes the DB to crash during recovery with inconsistencies in transaction status.
For e.g recovery thinks the transaction shoudl be aborted but it is already committed. Another side effect of this is inconsistency in expected state
during recovery and recovery fails. The EE guys suppress these errors and once the DB comes up they see tons of PT incosnsitency and other catalog
consistency issues. They either rebuilt it or drop many tables.

5. A critical issue : ERROR: Append-Only Storage Read could not open segment file 'xxx/yyyy.1' (walmart, equifax case)

The one possible scenario for this was the MPP-18228. Though it was not proven that this actually caused the WALMART issue, but could be the one.

6. Extra Persistent Relation entries issue found with the below query:

``` theme:
SELECT  coalesce(p.relfilenode_oid, r.relfilenode_oid) as relfilenode,
        p.ctid, r.persistent_tid
    FROM  (
      SELECT p.ctid, p.* FROM gp_persistent_relation_node p
      WHERE persistent_state = 2 AND p.database_oid in (
        SELECT oid FROM pg_database WHERE datname = current_database()
        UNION ALL
        SELECT 0
        )
      ) p
      FULL OUTER JOIN gp_relation_node r
        ON (p.relfilenode_oid = r.relfilenode_oid and
            p.segment_file_num = r.segment_file_num)
    WHERE  (p.relfilenode_oid is NULL OR
            r.relfilenode_oid is NULL OR
            p.ctid != r.persistent_tid)
```

7. Found this Comment by Milena in one of the JIRAs:

Three main issues(happened with WALMART):

1.  inconsistency in 'gp\_persistent\_relation' node
    -   Fix is available in 4.1
    -   To repair gp\_persistent\_relation\_node persistent tables has to be re-build.
        -   Rebuilding persistent tables can be done only by support people (escalation engineers)

2.  left over files in the file system
    -   Files have to be deleted manually from file system. Should be done by support people (escalation engineers)

3.  Inconsistency between gp\_persistent\_relation\_node and pg\_class
    -   needs more investigation
    -   I believe that re-building persistent tables would fix this issue.

 8. Also there are issues for inconsistency around persistent tables and filesystem

``` theme:
 gp_persistent_relation_node   <=> filesystem

SELECT coalesce(a.tablespace_oid, b.tablespace_oid) as tablespace_oid,
      coalesce(a.database_oid, b.database_oid) as database_oid,
       coalesce(a.relfilenode_oid, b.relfilenode_oid) as relfilenode_oid,
       coalesce(a.segment_file_num, b.segment_file_num) as segment_file_num,
       a.relfilenode_oid is null as filesystem,
       b.relfilenode_oid is null as persistent,
       b.relkind, b.relstorage
    FROM   gp_persistent_relation_node a
    FULL OUTER JOIN (
      SELECT p.*, c.relkind, c.relstorage
      FROM   gp_persistent_relation_node_check() p
        LEFT OUTER JOIN pg_class c
          ON (p.relfilenode_oid = c.relfilenode)
      WHERE (p.segment_file_num = 0 or c.relstorage != 'h')
    ) b ON (a.tablespace_oid   = b.tablespace_oid    and
            a.database_oid     = b.database_oid      and
            a.relfilenode_oid  = b.relfilenode_oid   and
            a.segment_file_num = b.segment_file_num)
    WHERE (a.relfilenode_oid is null OR
           (a.persistent_state = 2 and b.relfilenode_oid is null))  and
      coalesce(a.database_oid, b.database_oid) in
        SELECT oid FROM pg_database WHERE datname = current_database()
        UNION ALL
        SELECT 0
      );
```

------------------------------------------------------------------------

### <span style="text-decoration: underline;">Utilities, functions etc</span>

1. Simple function to make gp\_persistent\_relation\_node more readable -- It will print out primary/mirror states on all the segments (No master) -- You'll notice that the column names/values have been shortened to fit one row on a line -- Changes can done to add any needed columns -- Usage "Select \* from ptrel(&lt;some relfilenodeid&gt;)"

(\*Note that this function needs to created each time you initdb the system)

``` theme:
DROP function ptrel(oid);
CREATE or REPLACE FUNCTION ptrel             (        IN relfilenodeid oid,
                                                      OUT gp_seg_id int,
                                                      OUT tbl_spc_oid oid,
                                                      OUT db_oid oid,
                                                      OUT rel_oid oid,
                                                      OUT seg_f_no int,
                                                      OUT rel_strg_mgr smallint,
                                                      OUT p_state text,
                                                      OUT m_exst_state text,
                                                      OUT m_data_sync_state text)
RETURNS SETOF record as
$$
  SELECT gp_segment_id, tablespace_oid, database_oid, relfilenode_oid, segment_file_num,
         relation_storage_manager,
        case when persistent_state = 0 then 'free'
              when persistent_state = 1 then 'crt pndg'
              when persistent_state = 2 then 'crtd'
              when persistent_state = 3 then 'drp pndg'
              when persistent_state = 4 then 'abrt crt'
              when persistent_state = 5 then 'JIT crt endg'
              when persistent_state = 6 then 'blk lod crt pndg'
              else 'unknown state: ' || persistent_state
        end as p_state,
        case
          when mirror_existence_state = 0 then 'mrr free'
          when mirror_existence_state = 1 then 'not mrrd'
          when mirror_existence_state = 2 then 'mrr crt pndg'
          when mirror_existence_state = 3 then 'mrr crtd'
          when mirror_existence_state = 4 then 'mrr dwn bfr crt'
          when mirror_existence_state = 5 then 'mrr dwn during crt'
          when mirror_existence_state = 6 then 'mrr drp pndg'
          when mirror_existence_state = 7 then 'mrr only drp remans'
          else 'unknown state: ' || mirror_existence_state
        end as m_exst_state,
        case
          when mirror_data_synchronization_state = 0 then 'none'
          when mirror_data_synchronization_state = 1 then 'data sync'
          when mirror_data_synchronization_state = 2 then 'full copy'
          when mirror_data_synchronization_state = 3 then 'bufpl pg incr'
          when mirror_data_synchronization_state = 4 then 'bufpl scn incr'
          when mirror_data_synchronization_state = 5 then 'ao catchup'
          else 'unknown state: ' || mirror_data_synchronization_state
        end as m_data_sync_state
FROM gp_dist_random('gp_persistent_relation_node')
WHERE relfilenode_oid = $1;
$$
LANGUAGE SQL;
```

### **<span style="text-decoration: underline;">Scenarios to be added in QA cycles</span>**

1.  <span style="color: rgb(255,0,0);">Without any load on system, move DB directory from one / more segments to some other location, execute DB delete which essential would fail as expected. Restore the DB directory to its original location, execute DB delete again. The Db delete should succeed. Check catalog consistency, stale entries in PT should not be reported.</span>
2.  <span style="color: rgb(255,0,0);">gprecoverseg functionality is not tested across GPDB restarts. The expected behavior is if GPDB is restarted while recovery (gprecoverseg) is in progress, should resume and complete successfully once GPDB is UP again. Primary and Mirror should be completely in SYNC. So, basically this results in multiple combinations which needs to be tested having dimensions like </span>
    1.  <span style="color: rgb(255,0,0);">gprecoverseg FULL and INCREMENTAL</span>
    2.  <span style="color: rgb(255,0,0);">Graceful (gpstop) and immediate (gpstop -i) shutdown of GPDB</span>
    3.  <span style="color: rgb(255,0,0);">With and without LOAD</span>

<span style="text-decoration: underline;">Integration Team - Imp points and TO DOs</span>
-----------------------------------------------------------------------------------------

1. Every integration to the main will a set of satisfactory set of tests proving the sanity along with what components changed in the branch from last merge.

The components will be used by the QA team to run specific component based schedules after the merges to verify basic sanity of the merge in the Main.

The list of components can be found at <a href="http://jira.eng.pivotal.io/browse/MPP#selectedTab=com.atlassian.jira.plugin.system.project%3Acomponents-panel" class="external-link">http://jira.eng.pivotal.io/browse/MPP#selectedTab=com.atlassian.jira.plugin.system.project%3Acomponents-panel</a>

TO DO :- <span style="color: rgb(255,0,0);">Sync Up into PT from Main (Recent UCS and DELTA changes, Planned for 1/28)</span>

###### **<span style="text-decoration: underline;">Contact</span>**

Ashwin.Agrawal@emc

Ashwin.Pednekar@emc

Raghav.Tulshibagwale@emc

*\*Changes to above headers will occur over time.*

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PT\_State\_Machine.xlsx](attachments/49147264/50167959.xlsx) (application/vnd.openxmlformats-officedocument.spreadsheetml.sheet)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PT\_info\_on\_freeOrderNum.docx](attachments/49147264/50167960.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [XLOG information.docx](attachments/49147264/50167961.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PT\_info\_on\_gp\_persistent\_node.docx](attachments/49147264/50167962.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PT\_info\_on\_parentXid.docx](attachments/49147264/50167963.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [TRANSACTION\_RECOVERY\_ver1.docx](attachments/49147264/50167964.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [FilRep-PT-Short.docx](attachments/49147264/50167966.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [FileRep-PT-Long.docx](attachments/49147264/50167967.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PT-Questions-Kushal-Final.docx](attachments/49147264/50167968.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Body of CheckPoint XLOG records.docx](attachments/49147264/51281950.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [TRANSACTION\_RECOVERY\_ver2.docx](attachments/49147264/51282061.docx) (application/vnd.openxmlformats)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [gpcheckcat-tests.txt](attachments/49147264/52101182.txt) (text/plain)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PT-Presentation.pptx](attachments/49147264/56656448.pptx) (application/vnd.openxmlformats-officedocument.presentationml.presentation)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [ProposedDesign-PTRebuildutility.docx](attachments/49147264/73990160.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [ProposedDesign-PTRebuildutility.docx](attachments/49147264/73990159.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)

Document generated by Confluence on May 17, 2016 19:14


