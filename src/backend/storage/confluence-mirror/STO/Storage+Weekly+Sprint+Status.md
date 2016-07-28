<span id="title-text"> Storage & Access : Storage Weekly Sprint Status </span>
==============================================================================

This page last changed on Jan 06, 2014 by prabaa1.

-   [Jan 6th](#StorageWeeklySprintStatus-Jan6th)
-   [Dec 23rd](#StorageWeeklySprintStatus-Dec23rd)
-   [Dec 16th](#StorageWeeklySprintStatus-Dec16th)
-   [Dec 9th](#StorageWeeklySprintStatus-Dec9th)
-   [Dec 2nd](#StorageWeeklySprintStatus-Dec2nd)
-   [Nov 25th](#StorageWeeklySprintStatus-Nov25th)
-   [Nov 18th](#StorageWeeklySprintStatus-Nov18th)
-   [Nov 11th](#StorageWeeklySprintStatus-Nov11th)
-   [Nov 4th](#StorageWeeklySprintStatus-Nov4th)
-   [Oct 21st](#StorageWeeklySprintStatus-Oct21st)
-   [Oct 14th](#StorageWeeklySprintStatus-Oct14th)
-   [Oct 7th](#StorageWeeklySprintStatus-Oct7th)
-   [Sep 30th](#StorageWeeklySprintStatus-Sep30th)
-   [Sep 23rd](#StorageWeeklySprintStatus-Sep23rd)
-   [Sep 16th](#StorageWeeklySprintStatus-Sep16th)
-   [Sep 9th](#StorageWeeklySprintStatus-Sep9th)
-   [Sep 2nd](#StorageWeeklySprintStatus-Sep2nd)
-   [Aug 19th](#StorageWeeklySprintStatus-Aug19th)
-   [Aug 12th](#StorageWeeklySprintStatus-Aug12th)
-   [Aug 5th](#StorageWeeklySprintStatus-Aug5th)
-   [July 22nd](#StorageWeeklySprintStatus-July22nd)
-   [July 15th](#StorageWeeklySprintStatus-July15th)
-   [July 8th](#StorageWeeklySprintStatus-July8th)
-   [July 1st](#StorageWeeklySprintStatus-July1st)

Jan 6th
=======

**<span style="color: rgb(34,34,34);">Velocity : 38</span>**

**<span style="color: rgb(34,34,34);">Done</span>**<span> </span>

-   <span style="font-size: 14.0px;line-height: 1.4285715;">4.2.x</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Hotfix: 4.2.6.3DT4</span>
        -   Signed off on 4.2.6.3.DT4 Hotfix
    -   crash\_recovery\_filerep\_end\_to\_end: Recoverseg failure due to mirror Adreess port already in use
    -   42x-Storage Regression runs
-   CDBFAST to TINC Migration/ Test Optimization:
    -   Make Crash-Recovery Schedules Green in Pulse
    -   Analyze the runtime difference between CrashRecovery schedules with and without the  ScenarioTestCase and SQLConcurrencyTestCase framework
    -   fix schema topology test issues 
    -   Pg\_twophase pulse projects
    -   Make crash recovery optimized code compatible with 4.2x 
-   SPIKE:HAWQ Ramp up

<span>**To Do**<span> </span></span>

-   <span><span>Spike on HAWK</span></span>

Dec 23rd
========

**<span style="color: rgb(34,34,34);">Velocity : 71</span>**

**<span style="color: rgb(34,34,34);">Done</span>**<span style="font-size: 14.0px;line-height: 1.4285715;"> </span>

CDBFAST to TINC Migration/ Test Optimization:
-   optimize CrashRecovery Code to match pilot project
-   incorporate code review comments of   filerep\_end\_to\_end optimization
-   fix schema topology tpch test
-   Data corruption:  Initial proposal

Wings: 
-   Remote backup
-   Bring up the cluster in a standby mode
-   Design AO table replication mechanism for DR

Triage:
-   MPP-21753: SYMENTAC:master reset frequently
-   MPP-22240 "PANIC","XX000","PTCheck: Failed object entry already exist

Spike:
-   Run stress test on DCA
-   spike for data corruption tests
-   spike for gpcheckmirrorseg

Escalation:
-   MPP-22106 : Fidelity DU due to PT Serial \# inconsistency

Hotfix: 4.2.6.3DT4
-   MPP-21545: Dotomi: New Tests for pg\_terminate\_backend and immediate shutdown
-   Automation for pg\_terminate\_backend tests MPP21545

verification of CCB for 4.2.7<span style="font-size: 14.0px;line-height: 1.4285715;"> </span>
**To Do**<span style="font-size: 14.0px;line-height: 1.4285715;"> </span>

Cleanup pulse-storage to have one set of 4.3 runs
Producing inconsistent oid for pg\_constraint for MPP-16659
Spike:
-   Play with GemFire to evaluate GemWAN
-   Cluster consistent point
-   HAWQ Ramp up
-   Spike: 4x-basic with standby

Escalation<span>:</span>
-   MPP-22205 :- gprecoverseg -r lead to persistent issue

CDBFAST to TINC Migration/ Test Optimization:
-   Make sure the Filerep\_end\_to\_end TINC test can run against 42x
-   Pg\_twophase pulse projects

Wings:
-   Block diagram for DR primary site
-   Review Sender side GO Code

Dec 16th
========

**<span style="color: rgb(34,34,34);">Velocity : 60</span>**

**<span style="color: rgb(34,34,34);">Done</span>**

-   4.2.7
    -   Change and Run Tinc Upgrade test for 4.2.7 RC2
-   Escalation:
    -   (MPP-22061) JPMC: SEGV when calculating hasttext for tuple
    -   MPP - 22044 Mediamind Select/Insert fails on partition table
    -   Dataxu - gprecoverseg failing
-   Spike:
    -   spike: Run Longevity on dca
    -   spike for upgrade test
    -   spike for tinc storage tests
    -   spike for multinode schedules
-   Analyze Storage-Dev regression runs
-   remove gppkg tests from 4x basic
-   4.3.x:  Setting up the pulse project and create a branch to reproduce MPP-21917
-   UAO: Upgrade 100K AO/CO tables on 16 node DCA
-   Wings:
    -   Restore a cluster snapshot
    -   SPIKE: Systemtap for GPDB
    -   Continue to investigate AO WAL overhead and design choice
-   CDBFAST to TINC Migration/ Test Optimization:
    -   Fix faultinjector enhancement
    -   MPP-16659: Concurrent tests with partition tables fails on Alter with ERROR: duplicate key violates unique constraint "pg\_constraint\_oid\_index" 
    -   Review: filerep end to end-TINC optimization
    -   fix TINC schema topology test
    -   code review for filerep-end-to end optimization

<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);">****To Do****</span>**</span>

<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);"> </span>**</span>

-   Hotfix: 4.2.6.3DT4
    -   MPP-21545: Dotomi: New Tests for pg\_terminate\_backend and immediate shutdown
    -   Automation for pg\_terminate\_backend tests MPP21545
-   Escalation:
    -   MPP-22106 : Fidelity DU due to PT Serial \# inconsistency
    -   Producing inconsistent oid for pg\_constraint for MPP-16659
    -   Cleanup pulse-storage to have one set of 4.3 runs
-   Spike:
    -   spike for data corruption tests
    -   spike:Run stress test on DCA
-   Wings:
    -   Remote backup
    -   Bring up the cluster in a standby mode
    -   Design AO table replication mechanism for DR
    -   Block diagram for DR primary site
    -   Review Sender side GO Code
    -   Cluster consistent point
    -   Play with GemFire to evaluate GemWAN
-   CDBFAST to TINC Migration/ Test Optimization:
    -   optimize CrashRecovery Code to match pilot proj
    -   Pg\_twophase pulse projects
    -   incorporate code review comments of   filerep\_end\_to\_end optimization
    -   CDBFast to TINC Migration: Data corruption
    -   Make sure the Filerep\_end\_to\_end TINC test can run against 42x

Dec 9th
=======

**<span style="color: rgb(34,34,34);">Velocity : 77</span>**

**<span style="color: rgb(34,34,34);">Done</span>**

-   CDBFAST to TINC Migration/Test optimization:
    -   Crash\_Recovery suite : Collapse pre and Trigger sqls in tinc 
    -   Filerep\_end\_to\_end : Remove workload from fault\_ct
    -   Filerep\_end\_to\_end : Remove the workload from checkpoint\_changetracking.
    -   Filerep\_end\_to\_end : Running sql files in parallel by using Scenario Test
    -   pgtwophase\_removal: switch\_checkpoint tests in serial mode
    -   Change the ScenarioTestcase of pg\_twophase execute\_split\_sqls to pass in parameters
    -   Address Code Review Comments for Pg\_twophase execute\_split\_sqls
-   gppkg
    -   Make gpppkgs backward compatible
    -   gppkg: plr test fix
    -   Raise code review for gppkgs schedule
    -   gppkgs wiki and session with central qa team
-   PARISTX
    -   gppkg errors in 4xbasic runs for 43 rc3 
    -   4.3.0.0 RC3: Verifying the Longevity run
    -   TRIAGE MPP-22053 "ERROR: Append-Only Storage Read could not open segment file"
-   Wings
    -   SPIKE: AO WAL performance analysis
    -   Define cluster snapshot format
    -   Data collector
    -   SPIKE: go through golang tutorial
-   4.2.7
    -   MPP-22074 4.2.7 Pre RC Longevity Checkmirrordiff
    -   Madlib tests on 4.2.7 Pre RC:  analyzing pulse result
    -   427PreRC Subtranasaction\_limit\_removal failure
    -   Run gppkgs schedule for 427 RC1
    -   Verify Manual Upgrade on DCA for 4.2.7
-   Escalation
    -   MPP-21545: Dotomi: pg\_terminate\_backend causes postmaster reset
    -   MPP-21861:  59035208:Bloomberg: orphan lock causing function to wait on lock 
-   4.2.x
    -   CR-9224 - Revisit proc\_exit vs thread stale pointer issue.

**<span style="color: rgb(34,34,34);">**<span>****To Do****</span>**</span>**

-   4.2.x
    -   (MPP-22061) JPMC: SEGV when calculating hasttext for tuple
    -   Analyze Storage-Dev regression runs
    -   MPP-21545: Dotomi: New Tests for pg\_terminate\_backend and immediate shutdown
-   <span style="font-size: 14.0px;line-height: 1.4285715;">Spike</span>
    -   Spike: Run Longevity on dca
    -   Spike for upgrade test
    -   Spike for tinc storage tests
    -   Spike: 4x-basic with standby
-   <span style="font-size: 14.0px;line-height: 1.4285715;">4.3.x</span>
    -   Setting up the pulse project and create a branch to reproduce MPP-21917
    -   Cleanup pulse-storage to have one set of 4.3 runs
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Escalation</span>
    -   MPP - 22044 Mediamind Select/Insert fails on partition table
-   <span style="font-size: 14.0px;line-height: 1.4285715;">wings</span>
    -   Restore a cluster snapshot
    -   SPIKE: Systemtap for GPDB
    -   Block diagram for DR primary site
    -   Continue to investigate AO WAL overhead and design choice
    -   Review Sender side GO Code
-   <span style="font-size: 14.0px;line-height: 1.4285715;">gppkg</span>
    -   Remove gppkg tests from 4x basic
    -   CDBFAST to TINC Migration/Test optimization:
    -   Pg\_twophase pulse projects
    -   Fix faultinjector enhancement

Dec 2nd
=======

**<span style="color: rgb(34,34,34);">Velocity : 59</span>**

**<span style="color: rgb(34,34,34);">Done</span>**

-   4.2.x
    -   Fix for MPP-20466: Orbitz: duplicate CONSTRAINT created. No way to remove second constraint without dropping all constraints 
    -   TRIAGE: Symantec: Segment has not reached expected state MPP-21871
    -   Escalation
        -   MPP-21974: DOTOMI - Request RCA for DB crash 
    -   Hotfix 4.2.6.3DT3 test schedule verification
    -   4.2.7
        -   Test FTS disk-io fix on 4.2.7
        -   Monitor 4.2.7 Regression Schedules
        -   42x-StorageDev build and 4x-basic for orbitz change
        -   Run gppkgs tests for 4.2.7
        -   4.2.7: Verifying Longevity and validate Stress
-   WINGS
    -   pg\_basebackup on segment
    -   Introduce SystemTap
        -   Build with --enable-dtrace
        -   Run a basic script with stap command on linux
        -   Write a basic doc for build and usage
-   CDBFAST to TINC Migration
    -   TINC migration of pg\_twophase module run\_with\_switch\_checkpoint()
-   Create and run separate schedule for gppkgs
-   Spike: ECHO schedules
-   4.3.x
    -   Monitor 4.3 RC3 regression
    -   4.3.0.0 RC3 Regression: Crash Recovery schedules
    -   4.3 RC3, Monitor Stress testing and perform Validation
    -   4.3.0.0 RC3 : Trigger the Longevity Test Run

**<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);">**<span>To Do</span>**</span>**</span>**

**<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);"> </span>**</span>**

-   CDBFAST to TINC Migration
    -   Collapse pre and Trigger sqls in tinc Crash\_Recovery suite
    -   Filerep\_end\_to\_end: Remove workload from fault\_ct
    -   Filerep\_end\_to\_end: Remove the workload from checkpoint\_changetracking.
    -   Filerep\_end\_to\_end: Running sql files in parallel by using Scenario Test
    -   pg\_twophase\_removal: switch\_checkpoint tests in serial mode
-   4.3.x
    -   TRIAGE: MPP-22053 "ERROR: Append-Only Storage Read could not open segment file"
    -   Cleanup pulse-storage to have one set of 4.3 runs
-   WINGS
    -   Define cluster snapshot format
    -   Data collector
    -   gpdr create
    -   SPIKE: AO WAL performance analysis
    -   Block diagram for DR primary site
-   gppkg
    -   gppkg: plr test fix
    -   DUPLICATE:::list and run gppkg dependent 4 basic tests
    -   Raise code review for gppkgs schedule
    -   gppkgs migration to TINC -- identify requirements
    -   gppkgs migration
    -   Make gpppkgs backward compatible
-   4.2.x
    -   Escalation:
        -   MPP-21861:  59035208:Bloomberg: orphan lock causing function to wait on lock 

**<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);">
</span>**</span>**

Nov 25th
========

**<span style="color: rgb(34,34,34);">Velocity : 73</span>**

**<span style="color: rgb(34,34,34);">Done</span>**

-   Test optimization:
    -   TINC migration of pg\_twophase module execute\_split\_sqls
    -   Review pg\_two\_phase\_exe\_split\_sq
-   PARISTX:
    -   MPP-21910 Persistent\_Tables Checkmirror integrity differences
    -   TRIAGE and RCA: MPP-21917 Filerep\_end\_to\_end\_incr\_mirror: gpcheckmirrorseg shows difference
    -   Monitor 4.3 schedules after RC 2
    -   Longevity 4.3.0.0 RC2
    -   4.3.0.0 RC 2, Monitoring the regression schedules, Crash Recovery&Filerep\_e2e
    -   Stress Test, Triggering and Monitoring
    -   Integrate OpenBAR tests to main after adding version numbers to uao files.
    -   MPP-21921 ParisTx RC2 build list\_storage failure
    -   MPP-21967 Investigate ParisTx regression
    -   Test MADLIB installation
    -   Run gppkg on post rc (SUSE10, RHEL 6, RHEL 5)
    -   Run Additional 4x basic tests failing because of gppkgs
    -   Add gppkg fixes to 4-3 branch and run pulse project
-   4.2.x:
    -   Delete the FTS disk check file if magic gets corrupted
    -   Test Ashwin's FTS fix - Detect IO issue in FTS check in segments
    -   4.2.7 storage:  Trigger and Monitor regression schedule for Storage Schedules
    -   Escalation:
        -   MPP-19974: Mirror segment failed with "semop(id=1429799523,num=1) failed: Numerical result out of range (pg\_sema.c:499)" 
        -   ClickForensics : Gprecoverseg fails! - gprecoverseg was failing due to missing database directory for some relations.
        -   TRIAGE: CCB mirror and gprecoverseg failures MPP-22024

**<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);">To Do</span>**</span>**

**<span style="color: rgb(34,34,34);">**<span style="color: rgb(34,34,34);"> </span>**</span>**

-   Spike: ECHO schedules
-   SPIKE: study catalog
-   gppkg: plr test fix
-   Create and run separate schedule for gppkgs<span> </span>
-   Test optimization:
    -   Collapse pre and Trigger sqls in tinc Crash\_Recovery suite
    -   TINC migration of pg\_twophase module run\_with\_switch\_checkpoint()
    -   Filerep\_end\_to\_end:
        -   Remove the workload from checkpoint\_changetracking.
        -   Running sql files in parallel by using Scenario Test
        -   Remove workload from fault\_ct
-   <span style="font-size: 14.0px;line-height: 1.4285715;">4.2.x:</span>
    -   Analyse 4.2.7 Pre RC Regression test Results
    -   Montior Stress & Longevity for 4.2.7 Pre RC runs
    -   MPP-13825: Plpgsql exception block does not handle exception properly ("The distributed transaction 'Prepare' broadcast failed to one or more segments") 
    -   Escalation:
        -   MPP-20466: Orbitz: duplicate CONSTRAINT created. No way to remove second constraint without dropping all constraints 
        -   TRIAGE: Symantec: Segment has not reached expected state MPP-21871
        -   MPP-21974: DOTOMI - Request RCA for DB crash <span style="font-size: 14.0px;line-height: 1.4285715;"> </span>
-   DR:
    -   Introduce SystemTap
    -   SPIKE: AO WAL performance analysis<span style="font-size: 14.0px;line-height: 1.4285715;"> </span>
-   ParisTx 4.3 RC3:
    -   Analyse 4.3. RC3 Regression test Results
    -   Monitor Stress tests

Nov 18th
========

**<span style="color: rgb(34,34,34);">Velocity : 63</span>**

**<span style="color: rgb(34,34,34);">Done</span>**

-   Test optimization
    -   SPIKE:  pg\_twophase\_removal - execute\_split\_sqls()
    -   SPIKE: Filerep end to end
    -   SPIKE: crash\_recovery
    -   SPIKE: pg\_twophase\_removal- execute\_split\_sqls()
    -   'Abort' fault - Support multiple invokations
-   CDBFAST to TINC Migration
    -   Incorporate CR comments to gpstart
    -   Incorporate CR comments to gpstop
-   PARISTX
    -   Trigger and Monitor failing Backup schedule for pre-RC
    -   Review for Walrep Documentaion
    -   change filerep runs to point to main
    -   Gppkgs
        -   Gppkgs test on suse
        -   gppkg Testing: plr & pljava
        -   Incorporate CR comments for gppkgs and update 4-3 branch
-   42x
    -   Creating pulse project for tinc access\_methods
    -   Detect IO issue in FTS check in segments
    -   MPP-20572: Bloomberg - Master reset causing outage 
    -   Triage MPP-21760 Virgin Australia - System hang issue
    -   Hotfix: 4.2.6.3B
        -   Validate Hotfix 4.2.6.3B  - CCB Truncate Hang 
-   SPIKE: BTree index layout and answering questions from QP team for the same

**<span style="color: rgb(34,34,34);">To Do</span>**

-   Test Optimization
    -   Collapse pre and Trigger sqls in tinc Crash\_Recovery suite
    -   TINC migration of pg\_twophase module execute\_split\_sqls
    -   Running sql files in parallel by using Scenario Test
    -   Filerep\_end\_to\_end: Remove the workload from checkpoint\_changetracking
    -   CDBFAST to TINC Migration
        -   TINC migration of pg\_twophase module run\_with\_switch\_checkpoint() with 5 tests
-   42x
    -   MPP-20466: Orbitz: duplicate CONSTRAINT created. No way to remove second constraint without dropping all constraints 
    -   MPP-13825: Plpgsql exception block does not handle exception properly ("The distributed transaction 'Prepare' broadcast failed to one or more segments") 
    -   To test Detect IO issue in FTS check in segments
-   PARISTX
    -   Integrate OpenBAR tests to main after adding version numbers to uao files.
    -   Cleanup pulse-storage to have one set of 4.3 runs
    -   MPP-21910 Persistent\_Tables Checkmirror integrity differences
    -   TRIAGE and RCA: MPP-21917 Filerep\_end\_to\_end\_incr\_mirror: gpcheckmirrorseg shows difference
    -   gppkg
        -   gppkg: plr test fix
        -   Run gppkg on post rc (SUSE10)
        -   Run gppkg tests on post RC (RHEL 6)
        -   Run gppkg tests on post RC (RHEL 5)
-   Disaster Recovery
    -   DR: Introduce SysteTap
    -   DR: SPIKE: AO WAL performance analysis
    -   Block diagram for DR primary site
-   SPIKE: study catalog

Nov 11th
========

**<span style="color: rgb(34,34,34);">Velocity : 32</span>**

**<span style="color: rgb(34,34,34);">Done</span>**

-   Persistent\_tables test has intermittent failures. this story is to correct them
-   gppkgs 
    -   Run postgis test
    -   Change plperl pgcrypto and gppc tests
-   PARISTX PreRC:  
    -   Walrepl Tinc on Main, retrigger schedule after fix of QAINF-574
    -   MPP-21784:gpmigrator script fails when upgrading from 4.2.6 to 4.3 during a upstart 
    -   Fix OpenBAR tests - Few tests changes needed due to  MPP-21720 and MPP-21798
-   Hotfix : 4.2.6.3B : MPP-21625: CCB - RCA for hung truncate session - Fix
-   Test Optimization:
    -   SPIKE:  pg\_twophase\_removal - execute\_split\_sqls()
    -   SPIKE: Crash Recovery
    -   SPIKE: Filrep End to End

**<span style="color: rgb(34,34,34);">To Do</span>**

-   CDBFAST to TINC Migration:
    -   Incorporate CR comments to gpstart & gpstop
-   PARISTX
    -   change filerep runs to point to main
    -   Review for Walrep Documentaion
    -   Trigger and Monitor failing Backup schedule  for pre-RC
-   Creating pulse project for tinc access\_methods
-   Test Optimization: Continued
    -   pg\_twophase\_removal - execute\_split\_sqls()
    -   Crash Recovery
    -   Filrep End to End
-   Detect IO issue in FTS check in segments
-   SPIKE: BTree index layout and answering questions from QP team for the same
-   MPP-20466: Orbitz: duplicate CONSTRAINT created. No way to remove second constraint without dropping all constraints 
-   Hotfix : 4.2.6.3B : MPP-21625: CCB - RCA for hung truncate session - Validation

Nov 4th
=======

**<span style="color: rgb(34,34,34);">Done</span>**

-   PARISTX
    -   PARISTX Pre-RC: CrashRecovery & Filerep\_End\_to\_End Regression tests
    -   PARISTX Pre-RC: WALREP Regression tests
    -   PARISTX Pre-RC: Upgrade tests
    -   Upgrade with Incremental backup and part/subpart tables
    -   <span style="color: rgb(0,0,0);">PARISTX Pre-RC: Stress tests</span>
    -   <span style="color: rgb(255,0,0);"><span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-862" class="external-link">STO-862</a> </span><span style="color: rgb(0,0,0);">gppkg Automation In progress</span></span>
    -   <span style="color: rgb(255,0,0);"><span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-857" class="external-link">STO-857</a><span style="color: rgb(0,0,0);"> </span></span><span style="color: rgb(0,0,0);">PARISTX Pre-RC: UAO Regression tests</span></span>
    -   <span style="color: rgb(0,51,102);">GPPC gppkg update for 4.3</span><span style="color: rgb(255,0,0);"><span style="color: rgb(0,0,0);">
        </span></span>
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-Regression-instrument schedule
    -   Walrep: Change pulse projects to point to main branch after the tests and code are merged to main/PARISTX
    -   Walrep: Merge tinc tests to //tincrepo/main<span style="color: rgb(0,51,102);">
        </span>
    -   UAO:Trigger Longevity Validation and Install Command Center
    -   <span style="color: rgb(0,51,102);">Investigate Stress test issue with UAO</span>
-   Automate (MPP-21545) DOTOMI : pg\_terminate\_backend causes postmaster reset
-   Automate Online Help: MPP-21655 - Updates for gpinitstandby, gpactivatestandby
-   Fix and integrate the AO filedump tool to MAIN
-   Hotfix
    -   Validate IVECO hotfix 4.2.6.3IV2
    -   IVECO hotfix 4.2.6.3IV2
-   FTS: Detect IO issue in FTS check in segments
-   <span style="color: rgb(0,51,102);">DR: Something similar to pg\_basebackup in Go</span>
-   <span style="color: rgb(0,51,102);">Checkin AO filedump in MAIN</span>
-   <span style="color: rgb(0,51,102);">MPP-21741 \[Triage\] - Unexpected internal error: Master 12657 received signal 11 in worker thread</span>
-   <span style="color: rgb(0,51,102);">Escalation</span>
    -   <span style="color: rgb(0,51,102);"><span style="color: rgb(34,34,34);">Symantec - gpexpand failed</span>
        </span>

**To Do**

-   Figure-out solution to resolve the SSH to cloud instances
-   PARISTX PreRC
    -   Walrepl Tinc on Main, retrigger schedule after fix of QAINF-574
    -   Stress Test
-   <span style="color: rgb(255,0,0);"><span style="color: rgb(0,0,0);">CDBFAST to TINC Migration</span></span>
    -   <span style="color: rgb(255,0,0);"><span style="color: rgb(0,0,0);">CR review comments for Gpstop and Gpstart</span></span>
-   <span style="font-size: 14.0px;line-height: 1.4285715;">Revisit Test schedules & do optimization</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Filrep\_end\_to\_end</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Crash Recovery</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">pgtwophase</span>

<span style="color: rgb(255,0,0);"><span style="color: rgb(0,0,0);">
</span></span>

<span style="color: rgb(0,102,0);font-size: 24.0px;line-height: 1.25;">Oct 28th</span>

<span style="color: rgb(34,34,34);">Velocity : 96</span>

**<span style="color: rgb(34,34,34);">Done</span>**

UAO

-   -   UAO: Upgrade with 100K mixture of (AO, CO)
    -   UAO: Manual Upgrade 4.0.x -&gt; 4.1.x -&gt; 4.2.x -&gt; 4.3.x
    -   UAO:Verify MPP-21616 after merge to main
    -   UAO:Verify the fix for MPP-21661
    -   MPP-21616: Correct pseudo compaction drop
    -   Integrate UAO stress test to main
    -   UAO: Verify MPP-21616
    -   Triage MPP-21676 

CDBFAST To TINC Migration

-   -   Crash-Recovery: cdbfast to TINC : Incorporate CR comments
    -   Crash-Recovery: cdbfast to TINC : rerun after incorporating CR comments
    -   Crash-Recovery: cdbfast to TINC : schema topology with all tests in pulse & analyze
    -   Adding new sql for Schema topology

WALREP

-   -   WALREP: Manual Upgrade 4.0.x -&gt; 4.1.x -&gt; 4.2.x -&gt; 4.3.x
    -   Walrep: Failback mechanism on walrepl tinc tests need to be changed: ref: MPP-21535
    -   MPP-21655 - Updates for gpinitstandby, gpactivatestandby - Manual Verification
    -   WalRep: Change Tinc test case after fixing the fail back to old master method

Hotfix

-   -   Hotfix: MPP-21545 DOTOMI : pg\_terminate\_backend causes postmaster reset

MAIN Regression Run Validation

-   -   MAIN Runs: List\_weekend\_customer schedule
    -   Main run: triggered longevity testing against main after merge of UAO
    -   Make sure upgrade tests in main are pointing to the latest 4.2. version in all the tests
    -   MAIN Runs: 4x basic schedule with Stand by
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-OpenBAR schedule
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-DDAuto schedule
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-BehaveBackupRestore schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Persistent-Tables schedule
    -   MAIN Runs: TINC GPDB-PARISTX\_UAO-BehaveBackupScale schedule
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-Regression/home/ schedule
    -   MAIN Runs: TINC GPDB-WalRepl-TINC-upgrade schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Transaction schedule
    -   MAIN Runs: TINC GPDB-WalRepl-LongRunning schedule
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Full\_Primary\_Failover schedule
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Full\_Mirror\_Failover schedule
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Incr\_Primary\_Failover schedule
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Incr\_Mirror\_Failover schedule
    -   MAIN Runs: TINC Crash\_Recovery\_filerep\_end\_to\_end\_with\_transistion schedule
    -   MAIN Runs: TINC Crash\_Recovery\_04\_to\_10 schedule
    -   MAIN Runs: TINC Crash\_Recovery\_11\_to\_20 schedule
    -   MAIN Runs: TINC Crash\_Recovery\_21\_to\_30 schedule
    -   MAIN Runs: TINC Crash\_Recovery\_31\_to\_40 schedule

Escalation

-   -   CCB escalation: hang issue with TRUNCATE
    -   <span style="color: rgb(0,51,102);">MPP-21609 :- MediaMind gprecoverseg issue w/ catalog corruption</span>

<span style="color: rgb(0,51,102);">SPIKE: Building up GPDB TODOs</span>

<span style="color: rgb(0,51,102);">Look at possible next steps for FTS to detect the server hung issues</span>

<span style="color: rgb(0,51,102);">Finalize GPDB Roadmap (Storage Perpsective)</span>

<span style="color: rgb(255,0,0);"><span style="color: rgb(0,51,102);">**To Do**</span></span>

Walrep: Change pulse projects to point to main branch after the tests and code are merged to main/PARISTX

<span style="color: rgb(0,51,102);">Walrep: Merge tinc tests to //tincrepo/main</span>

<span style="color: rgb(0,51,102);">Fix and integrate the AO filedump tool to MAIN</span>

Automate (MPP-21545) DOTOMI : pg\_terminate\_backend causes postmaster reset

CR review comments for Gpstop and Gpstart

gppkg Automation

Automate Online Help: MPP-21655 - Updates for gpinitstandby, gpactivatestandby

Correct the flakyness of Persistent test\_suites created by India team

CR review comments for gpdbverify

Block diagram for DR primary site

Hotfix: 

-   -   IVECO hotfix 4.2.6.3IV2

PARISTX Pre RC

-   -   PARISTX Pre-RC: UAO Regression tests
    -   PARISTX Pre-RC: CrashRecovery & Filerep\_End\_to\_End Regression tests
    -   PARISTX Pre-RC: WALREP Regression tests
    -   PARISTX Pre-RC: Stress tests
    -   PARISTX Pre-RC: Upgrade tests

Oct 21st
========

<span style="color: rgb(34,34,34);">Velocity : 81</span>

**<span style="color: rgb(34,34,34);">Done</span>**

WALREP

-   -   WALREP: Trigger 4x basic schedules with standby after fix from MPP-21413
    -   TRIAGE and fix MPP-21413
    -   Walrep: Verify Upgrade issue
    -   MAIN Runs: TINC GPDB-WalRepl-Miscellaneous schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Persistent schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Filerep schedule

CDBFAST To TINC Migration

-   -   Crash-Recovery: cdbfast to TINC : Schema topology with single test
    -   Crash-Recovery: cdbfast to TINC : Raise code review
    -   Crash-Recovery: cdbfast to TINC : schema topology with all tests on local machine
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Incorporating all the CR comments
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Rerun after incorporating comments

UAO

-   -   Merging ans.uao files in 4 3 branch
    -   UAO: Make the crash-recovery TINC tests green
    -   UAO: Trigger WAL REP TINC tests on UAO Branch
    -   UAO: Trigger QP TINC tests on UAO Branch
    -   UAO: Trigger stress testing
    -   UAO: Trigger & Monitor Upgrade tests after integration of gpmigrator fix MPP-21542
    -   UAO: UAO Regression schedule - Trigger & Monitor Regression schedule
    -   UAO: List\_weekend Schedule- Trigger & Monitor Regression schedule
    -   UAO: Crash Recovery (7 schedules) to be triggered & Monitored
    -   UAO: TINC schedule - Trigger & Monitor the Regression test
    -   UAO: Analyse Filerep\_end\_to\_end TINC test in Pulse
    -   UAO: 4x basic schedules: Triggering and Monitoring Regression
    -   UAO: MultiNode (Incr & Full Recoverseg) Trigger & Monitor Regression schedule
    -   UAO: AO\_CO\_compression Trigger & Monitor Regression schedule
    -   UAO: Pg\_twophase Trigger & Monitor Regression schedule
    -   UAO: Long Running TINC Trigger & Monitor Regression schedule
    -   UAO: Persistent- Trigger & Monitor Regression schedule
    -   UAO: Backup & Restore schedule - Trigger & Monitor Regression schedule
    -   UAO: DDAuto schedule - Trigger & Monitor Regression schedule
    -   UAO: Data Corruption - Trigger & Monitor Regression schedule
    -   UAO: TINC Transaction Schedule- Trigger & Monitor Regression schedule
    -   UAO: Madlib - Trigger & Monitor Regression schedule
    -   UAO: Longevity testing (Cont.), Verifying
    -   UAO: Integrate UAO to MAIN
    -   UAO Upgrade - With large number of tables
    -   UAO: TINC Instrument - Trigger & Monitor Regression schedule
    -   UAO: tinc regression-Persistent-Tables: trigger and monitor
    -   UAO: Bug Fix Verification
    -   UAO:Run Upgrade tinc test
    -   UAO:change the fault\_injection test suite so that it is skipped for Instrumented(gcov) build and trigger in pulse
    -   Fix the Relase\_4\_3 main checkins after uao merge
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-Regression-instrument schedule

Persistent Tables

-   -   PT: Integrate PARISTX-PT catalog changes to MAIN
    -   MPP-19161 Post recovery PT catalog changes Upgrade test
    -   PT: Trigger & Monitor Upgrade tests after integration of PARISTX-PT catalog changes to MAIN

**<span style="color: rgb(34,34,34);"> </span>**<span style="color: rgb(34,34,34);">**To Do**</span>

WALREP

-   -   <span style="color: rgb(34,34,34);"> </span>WALREP: Manual Upgrade 4.0.x -&gt; 4.1.x -&gt; 4.2.x -&gt; 4.3.x
    -   <span style="color: rgb(34,34,34);"> </span>Walrep: Failback mechanism on walrepl tinc tests need to be changed: ref: MPP-21535
    -   <span style="color: rgb(34,34,34);"> </span>Walrep: Merge tinc tests to //tincrepo/main
    -   <span style="color: rgb(34,34,34);"> </span><span style="font-size: 14.0px;line-height: 1.4285715;">Walrep: Change pulse projects to point to main branch after the tests and code are merged to main/PARISTX</span>

<span style="color: rgb(34,34,34);font-size: 14.0px;line-height: 1.4285715;">UAO</span>

-   -   UAO: Upgrade with 100K mixture of (Heap, AO, CO)<span style="color: rgb(34,34,34);font-size: 14.0px;line-height: 1.4285715;"> </span>
    -   UAO: Manual Upgrade 4.0.x -&gt; 4.1.x -&gt; 4.2.x -&gt; 4.3.x
    -   Make sure upgrade tests in main are pointing to the latest 4.2. version in all the tests<span style="color: rgb(34,34,34);font-size: 14.0px;line-height: 1.4285715;"> </span>
    -   MAIN Runs: List\_weekend\_customer schedule
    -   MAIN Runs: 4x basic schedule with Stand by
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-OpenBAR schedule
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-DDAuto schedule<span style="color: rgb(34,34,34);font-size: 14.0px;line-height: 1.4285715;"> </span>
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-BehaveBackupRestore schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Persistent-Tables schedule
    -   MAIN Runs: TINC GPDB-PARISTX\_UAO-BehaveBackupScale schedule
    -   MAIN Runs: TINC GPDB-PARISTX-UAO-Regression/home/ schedule
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC GPDB-WalRepl-TINC-upgrade schedule</span>
    -   MAIN Runs: TINC GPDB-WalRepl-Transaction schedule
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC GPDB-WalRepl-LongRunning schedule</span>
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Full\_Primary\_Failover schedule
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Full\_Mirror\_Failover schedule
    -   MAIN Runs: TINC Filrep\_end\_to\_end\_Incr\_Primary\_Failover schedule
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC Filrep\_end\_to\_end\_Incr\_Mirror\_Failover schedule</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC Crash\_Recovery\_filerep\_end\_to\_end\_with\_transistion schedule</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC Crash\_Recovery\_04\_to\_10 schedule</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC Crash\_Recovery\_11\_to\_20 schedule</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC Crash\_Recovery\_21\_to\_30 schedule</span>
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">MAIN Runs: TINC Crash\_Recovery\_31\_to\_40 schedule</span>

CDBFAST To TINC Migration<span style="color: rgb(34,34,34);font-size: 14.0px;line-height: 1.4285715;"> </span>

-   -   Crash-Recovery: cdbfast to TINC : Incorporate CR comments
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Crash-Recovery: cdbfast to TINC : rerun after incorporating CR comment</span><span style="font-size: 14.0px;line-height: 1.4285715;color: rgb(34,34,34);"> </span>
    -   Crash-Recovery: cdbfast to TINC : schema topology with all tests in pulse & analyze
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Adding new sql for Schema topology</span>

Oct 14th
========

<span style="color: rgb(34,34,34);">Velocity : 92</span>

**<span style="color: rgb(34,34,34);">Done</span>**

WALREP

-   -   WAL REP: Sync from MAIN
    -   Walrep: Merge cdbfast private branch changes to Release\_4\_3
    -   WALREP: Trigger tests after Sync from MAIN
    -   Triage MPP-21535
    -   Walrep: Upgrade 4\_3 baseline need to be changed to avoid checking attrelid&gt;10000
    -   Commiting the fix for MPP-21413
    -   Fix oid&gt;10000 issue using an init\_file ignore
    -   Analyse main regression runs
    -   <span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-781" class="external-link">STO-781</a></span>WALREP: Trigger 4x basic schedules with standby after fix from MPP-21413

CDBFAST To TINC Migration

-   -   cdbfast to TINC Mig: Pulse monitor & retrigger if req for Filerep (Cont.)
    -   Crash Recovery in TINC : Spike - Understand Crash recovery for Migration
    -   Crash-Recovery: cdbfast to TINC : Filerep e2e
    -   cdbfast to TINC Mig: Trigger the test in Pulse Crash Recovery\_Filerep e2e
    -   Crash-Recovery cdbfast to TINC : Add sqls and run the test\_cases

UAO

-   -   UAO: Sync from MAIN
    -   UAO: Check on progress of internal Performance Test
    -   UAO: minor fixes to neg test files
    -   UAO: Regression Schedule : pg\_twophase\_removal (Cont.) after the interconnect fix
    -   UAO: Jason to run the UAO & all regression in UAO branch
    -   UAO: Trigger stress test
    -   UAO: Trigger Filerep & Crash Recovery on Pulse after the innterconnect fix
    -   UAO Test: Run Longevity on UAO Build after the fix for Vacuum
    -   UAO: Updating the ans files for Isolation test case
    -   UAO: Verify 11 resolved bugs
    -   UAO lazyvisimap demo
    -   Optimize visimap partition crawl
    -   UAO: Check design document
    -   UAO: Check if query to get segment file state from segment nodes is necessary
    -   Integrating the cdbfast 4.3 Release branch into private uao.
    -   Updating Ans files for 4xbasic schedules in cdbfast private branch to run against UAO Build
    -   Fix UAO aoseg permission regression
    -   UAO: Trigger Longevity test
    -   UAO: Trigger Filerep\_end\_to\_end TINC test in Pulse
    -   <span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-732" class="external-link">STO-732</a></span>UAO: Check on Upgrade performance with 100 K tables (Cont.) To Trigger after the fix
    -   <span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-742" class="external-link">STO-742</a></span>Fix upgrade test case

Persistent Tables

-   -   <span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-747" class="external-link">STO-747</a></span>MPP-19161 Post recovery PT catalog changes Upgrade test

Escalation

-   -   Triage MPP-21458
    -   MPP-21415 \[RCA\] :- Broadridge Checkpoint Failure
    -   Triage MPP-21465: Query unable to acquire lock on relation
    -   Triage MPP-21458: 58186536:Mediamind:Master restarted with error: "PANIC: Unexpected internal error: Master process received signal SIGSEGV"

**<span style="color: rgb(34,34,34);"> </span>**<span style="color: rgb(34,34,34);font-size: 14.0px;line-height: 1.4285715;">**To Do**</span>

WALREP

-   -   Walrep: Failback mechanism on walrepl tinc tests need to be changed: ref: MPP-21535
    -   Walrep: Merge tinc tests to //tincrepo/main
    -   Walrep: Change pulse projects to point to main branch after the tests and code are merged to main/PARISTX

CDBFAST To TINC Migration

-   -   Crash-Recovery: cdbfast to TINC : Schema topology with single test
    -   Crash-Recovery: cdbfast to TINC : Raise code review
    -   Crash-Recovery: cdbfast to TINC : schema topology with all tests in pulse
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Incorporating all the CR comments
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Rerun after incorporating comments
    -   Crash-Recovery: cdbfast to TINC : Incorporate CR comments
    -   Crash-Recovery: cdbfast to TINC : rerun after incorporating CR comments

UAO

-   -   UAO: Make the crash-recovery TINC tests green
    -   UAO: Trigger WAL REP TINC tests on UAO Branch
    -   UAO: Trigger QP TINC tests on UAO Branch
    -   UAO: Analyse Filerep\_end\_to\_end TINC test in Pulse
    -   UAO: Trigger & Monitor Upgrade tests after integration of gpmigrator fix MPP-21542
    -   UAO: 4x basic schedules: Triggering and Monitoring Regression
    -   UAO: MultiNode (Incr & Full Recoverseg) Trigger & Monitor Regression schedule
    -   UAO: AO\_CO\_compression Trigger & Monitor Regression schedule
    -   UAO: Pg\_twophase Trigger & Monitor Regression schedule
    -   UAO: Long Running TINC Trigger & Monitor Regression schedule
    -   UAO: Persistent Instrument - Trigger & Monitor Regression schedule
    -   UAO: UAO Regression schedule - Trigger & Monitor Regression schedule
    -   UAO: Backup & Restore schedule - Trigger & Monitor Regression schedule
    -   UAO: DDAuto schedule - Trigger & Monitor Regression schedule
    -   UAO: Data Corruption - Trigger & Monitor Regression schedule
    -   UAO: TINC Transistion Schedule- Trigger & Monitor Regression schedule
    -   UAO: Flush Cache Schedule- Trigger & Monitor Regression schedule
    -   UAO: Crash Recovery (7 schedules) to be triggered & Monitored
    -   UAO: stress testing(Cont.), verifying
    -   UAO: Longevity testing (Cont.), Verifying
    -   UAO: Integrate UAO to MAIN

Persistent Tables

-   -   PT: Integrate PARISTX-PT catalog changes to MAIN
    -   PT: Trigger & Monitor Upgrade tests after integration of PARISTX-PT catalog changes to MAIN

Oct 7th
=======

<span style="color: rgb(34,34,34);">Velocity : 81</span>

**<span style="color: rgb(34,34,34);">Done</span>**

WALREP

-   -   Walrep: Run weekend, resource mgm will be run at the point of integration
    -   WalRep: Review upgrade tests
    -   Triage asnd FIX MPP-21413
    -   WalRep: Create a script to do automatic failover of stress and re-trigger after n days
    -   WalRep:Run Madlib tests on Walrep Build
    -   WALRep longevity with Guc on
    -   WalRep: Run QP-TINC one more time before integration
    -   Walrep: Persistent tables test\_suite on pulse
    -   WAL REP: Sync from Main after QP checks in catalog changes
    -   WAL REP: Run Upgrade tests after sync from MAIN

CDBFAST to TINC Mig:

-   -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - send code review
    -   CDBFAST to TINC Mig: clean run for all sqls in filerep
    -   CDBFAST to TINC Mig: Incorporating review comments
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Trigger in Pulse

UAO

-   -   UAO: Regression Schedule : pg\_twophase\_removal
    -   UAO: Jason to run the UAO & all regression in UAO branch
    -   UAO Test: Run Longevity on UAO Build
    -   UAO:Spike estimate the impact of Lazy Visi-map creation on existing tests
    -   UAO: Failover tests for gpmigrator (upgrade) tests
    -   Crash Recovery cdbfast to TINC: Overall Framework
    -   UAO: RCA interconnect bug MPP-21013
    -   UAO: Add Vacuum tests to Stress test
    -   UAO:Reduce the number of new UAO sqls in Crash-Recovery Test
    -   UAO - modify negative tests
    -   <span style="color: rgb(0,51,102);">UAO: Spike on TINC gpmigrator (upgrade) tests</span>
    -   <span style="color: rgb(0,51,102);">UAO: Check on Upgrade performance with 100 K tables</span>
    -   <span style="color: rgb(0,51,102);">UAO: Spike on gpmigrator (upgrade) from 4.2.x to PARISTX\_UAO branch</span>
    -   <span style="color: rgb(0,51,102);">UAO: Lazy visimap creation</span>
    -   <span style="color: rgb(0,51,102);">MPP-21013: UAO hangs intermittently</span>

<span style="color: rgb(0,51,102);">Add CR comments MPP-20077</span>

Triage/RCA - MPP 21415 Checkpoint Failure

<span style="color: rgb(34,34,34);">**To Do**</span>

WAL REP:

-   -   WAL REP: Sync from MAIN
    -   Walrep: Merge all private branch tests to main
    -   Walrep: Change pulse projects to point to main branch after the tests and code are merged to main/PARISTX
    -   WALREP: Trigger tests after Sync from MAIN

UAO: 

-   -   UAO: Sync from MAIN
    -   UAO: Jason to run the UAO & all regression in UAO branch
    -   UAO: Trigger stress test
    -   UAO: Regression Schedule : pg\_twophase\_removal (Cont.) after the interconnect fix
    -   UAO: Updating the ans files for Isolation test case
    -   UAO: Trigger Filerep & Crash Recovery on Pulse after the innterconnect fix
    -   UAO: Check on Upgrade performance with 100 K tables (Cont.) To Trigger after the fix
    -   UAO Test: Run Longevity on UAO Build after the fix for Vacuum

CDBFAST to TINC Mig:

-   -   CDBFAST to TINC Mig: Pulse monitor & retrigger if req for Filerep (Cont.)
    -   Crash Recovery in TINC : Spike - Understand Crash recovery for Migration
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Incorporating all the CR comments
    -   Crash-Recovery: cdbfast to TINC : First 3 tests
    -   Crash-Recovery cdbfast to TINC : Add sqls and run the test\_cases
    -   cdbfast to TINC Mig: Trigger the test in Pulse Crash Recovery<span style="color: rgb(34,34,34);line-height: 1.4285715;"> </span>

Sep 30th
========

<span style="color: rgb(34,34,34);">Velocity : 104</span>

<span style="color: rgb(34,34,34);">**Done**</span>

<span>WAL REP</span>

-   -   WALREP: xlogdump output changes
    -   WalR: Testing: OOM on standby
    -   Design doc update to reflect switch changes for gpactivatestandby and gpinitstandby
    -   Data verifiicationa after failover to standby
    -   WAL REP: Stress test with failover
    -   OOM tips for WAL REP
    -   WAL REP: Trigger All tests (TINC & cdbfast)
    -   WALRep - Design Doc
    -   WALRep - Increasing memory consumption by Standby startup
    -   Enable the PARISTX-WALREP Regression schedules with Standby
    -   WalRep: Change the gpactivate library to use -f only if it failed on the first attempt

UAO

-   -   UAO: Remove unused guc from uao code and update the design doc
    -   UAO Test: Run Longevity on UAO Build
    -   UAO: Spike for Multinode setup & Run one cdbfast schedule by Savita
    -   UAO: Run filerep\_end\_to\_end on Stand alone machines
    -   UAO: Make Crash Recovery tests green in non pulse ENV
    -   UAO Test: Run Stress Test for WalRepl Build
    -   UAO Test: Test all GUCs in Test Plan
    -   UAO: Deal with backup behave tests
    -   UAO Test: Cursors read only/binary//hold/no-scroll
    -   UAO: Regression Schedule - Sub transaction Limit removal
    -   UAO: Analyzing/Reproducing incremental backup failures
    -   UAO update should use new delete approach
    -   UAO Test after upgrade if gptoolkit/visimap UDFs are created
    -   UAO Upgrade Demo
    -   UAO Behave BackupScale: Performance test- Shows difference in the time taken to do gpcrondump

CDBFAST To TINC Migration

-   -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - write unit tests and code cleanup
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - add gpstate and checkintegrity functions
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Run with all sqls from UAO private branch
    -   CDBFAST to TINC Migration: Handle external table and hybrid partition table cases

MPP-21274 Triage

\[Triage\] MPP-21209 - West -- Double fault of one segment when replacing a segment server

Rhel6.4 : IPV6 Testing - Failure analysis and rerun

Fix MPP-20077

**To Do **

WALREP

-   -   WalRep: Sync from Main after QP checks in catalog changes
    -   WalRep: Run Upgrade tests after sync from MAIN
    -   WalRep: Run weekend, resource mgm, madlib will be run at the point of integration
    -   WalRep: Create a script to do automatic failover of stress and re-trigger after n days
    -   WalRep: Review upgrade tests
    -   WalRep: Run QP-TINC one more time before integration
    -   WalRep: Persistent tables test\_suite on pulse

UAO

-   -   UAO: Divya to run the UAO & all regression in UAO branch
    -   UAO: Add Vacuum tests to Stress test
    -   UAO: Regression Schedule : pg\_twophase\_removal
    -   UAO: Jason to run the UAO & all regression in UAO branch
    -   UAO: Trigger stress test
    -   UAO: RCA interconnect bug MPP-21013
    -   UAO Test: Run Longevity on UAO Build
    -   UAO - modify negative tests
    -   UAO: Analyse internal Performance Test
    -   UAO: Lazy visimap creation
    -   UAO: Spike on TINC gpmigrator (upgrade) tests
    -   UAO: Spike on gpmigrator (upgrade) from 4.2.x to PARISTX\_UAO branch
    -   UAO: Check on Upgrade performance with 100 K tables
    -   UAO: Failover tests for gpmigrator (upgrade) tests

CDBFAST To TINC Migration

-   -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - send code review
    -   CDBFAST to TINC Mig: clean run for all sqls in filerep
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Trigger in Pulse
    -   Create Crash Recovery Scenario Framework in TINC & add UAO testcases
    -   Crash Recovery in TINC : Spike - Understand Crash recovery for Migration
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Incorporating all the CR comments

Sep 23rd
========

<span style="color: rgb(34,34,34);">Velocity : 101</span>

<span style="color: rgb(34,34,34);">**Done**</span>

WAL REP

-   -   WALRep - Design Doc
    -   MPP-21229 WALRep Master & Standby differences on Alter/Create DB
    -   WalR Testing: gpexpand : Standby failed to start with "FATAL","XX000","Unable to find entry for tablespace OID = 16994 when getting filespace directory paths (cdbpersistenttablespace.c:614)
    -   Testing Wal-R: Positive Automation -standby in failover mode - filerep fail overs
    -   Wal-R: Verification : Imporvement: gpactivatestandby should exit out faster than current implemetantion if PGPORT is not set/5432
    -   WalR Verification: STO-469: WalR Testing: gpexpand : Standby failed to start with "FATAL","XX000","Unable to find entry for tablespace OID = 16994 when getting filespace directory paths )
    -   Wal-R: Test code cleanup and re-factor
    -   Wal-R: Change to tincrepo tests as needed to support multinode run
    -   Testing Wal-R: Load Testing-
    -   Make 4xbasic Green: mkcheck.mkcheck.testCHECK1194 & other failing tests
    -   Testing Wal-R: Stress Testing
    -   WalRep: Analyse pulse-storage failures
    -   Fix MPP-21229
    -   FIX MPP-21228
    -   Add unit test for verifying XLOG is not cleaned on segments with prepared xans
    -   Fix MPP-21267:WalRep:Stress Test: Create database failed with "ERROR","58P01","could not stat file ""global/pg\_internal.init.31771"": No such file or directory Fix contrib build
    -   WalRep: Re-trigger stress test with fix for 21282
    -   Trigger the Longevity test for 4.3 Paristx Walrep

UAO

-   -   Savita : Making UAO Pulse/Pulse-Storage Green for Filerep\_end\_to\_end & Crash Recovery
    -   UAO: Create tests to verify fix for MPP-21144 and MPP-21146 for row and column oriented tables
    -   UAO: Recreate .ans files for filerep and crash-recovery for vacuum changes to segment related messages
        UAO: Review Upgarde test with Dirk
    -   Add OOM fault injector into UAO delete code
    -   UAO trigger & Analyze Regression tests
    -   UAO: Test OOD – Compaction
    -   UAO: Compaction and Out of memory
    -   UAO: Backup Behave tests - Failing with error 'report file not present'
    -   UAO : Check backup upgrade process - Make Pulse green
    -   Incremental backup after delete will fail with optimizer=on
    -   Error in postgis creation because of error in vacuum.h
    -   Fix the upgrade script for schema changes
    -   Wed Demo : New Deletes in UAO
    -   Fix MPP-21230: UOA Out-of-Order Deletes
    -   UAO: Look into gp\_select\_invisible with aborted Update
    -   UAO: Add some corner cases to DML
    -   UAO: Trigger Internal Performance Tests

\[Triage\] MPP-21228 Stress test failure

Fix 42x OSx build failure

Filerep\_end\_to\_end in TINC : Check with sample test case, migrate from scenario test to GPDB test, migrate to library

**<span style="color: rgb(34,34,34);">To Do </span>**

UAO

-   -   UAO - modify negative tests
    -   UAO - 4xbasic in storage and analysis
    -   UAO: Analyse internal Performance Test
    -   UAO: Patent
    -   UAO: Spike for Multinode setup & Run one cdbfast schedule by Savita
    -   UAO: Run filerep\_end\_to\_end on Stand alone machines
    -   UAO: Make Crash Recovery tests green in non pulse ENV
    -   UAO Test: Run Stress Test for UAO Build
    -   UAO Test: Run Longevity on UAO Build
    -   UAO Test: Test all GUCs in Test Plan
    -   UAO Test: Cursors read only/binary//hold/no-scroll
    -   UAO Filerep end to end with libraries
    -   Filerep\_end\_to\_end in Tinc - Triggering & Running with all the test cases
        -   UAO: Regression Schedule - Sub transaction Limit removal

        CDBFAST to TINC Mig
        -   Filrep\_end\_to\_end - write unit tests
        -   Filrep\_end\_to\_end - add gpstate and checkintegrity functions
        -   Filrep\_end\_to\_end - Run with all sqls from UAO private branch
        -   Filrep\_end\_to\_end - send code review
        -   Filrep\_end\_to\_end - Trigger in Pulse

WAL REP

-   -   WalR: Testing: OOM on standby
    -   Data verifiicationa after failover to standby
    -   WAL REP: Stress test
    -   WAL REP: Trigger All tests (TINC & cdbfast)
    -   Testing Wal-R: Longevtiy Testing
    -   WalRep: Change the gpactivate library to use -f only if it failed on the first attempt

\[Triage\] MPP-21209 - West -- Double fault of one segment when replacing a segment server

Sep 16th
========

<span style="color: rgb(34,34,34);">Velocity : 107</span>

<span style="color: rgb(34,34,34);">**Done**</span>

WAL Rep

-   -   Add --exclude option to pg\_basebackup
    -   Wal-R: Verification: gpactivatestandby on the same host: Error activating standby master: sequence item 1: expected string, NoneType found
    -   Send alerts in walreceiver \[Merge into p4\]
    -   gpinitstandby : Do not give default path option when the default path is not actually possible
    -   Wal-R gpinitstandby: Creation of standby directory is not consistent with different options
    -   Imporvement: gpactivatestandby should exit out faster than current implemetantion if PGPORT is not set/5432
    -   gpactivatestandby: 'list index out of range' stdout message
    -   Testing Wal-R: Negative Manual (management utilities) Part 2
    -   WAL REP: Making Multinode schedules Green
    -   Wal-R: Verification : gpinitstandby : Do not give default path option when the default path is not actually possible
    -   Wal-R: Verification: gpactivatestandby - Unable to create another host as stanby
    -   Wal-R: Verification: gpinitstandby -n is not working
    -   Tracking the fix in Regression schedules: multinode resync schedules
    -   Fixing new issues from Walrepl Instrument schedule
    -   WAL based replication - Official Design Doc
    -   Divya: Create WAL REP Pulse Projects in Pulse-Storage
    -   Fix Installcheck fail after rebase

UAO

-   -   UAO: Incremental backup - A CTAS table after a full backup and vacuum shows up in the incremental table list
    -   UAO: recreate the issue of concurrent transaction (MPP-21190) in a non gpcloud environment and in ParixTx
    -   UAO:Run cdbfast filerep related UAO changes thru Pulse-Storage
    -   UAO: Exchange and Split partition with update/delete
    -   UAO: Review Tests with Dirk
    -   Run with gp\_test\_system\_cache\_flush\_force = plain
    -   UAO : Check backup upgrade process
    -   UAO Test: Upgrade tests for UAO Build
    -   UAO: Update to Appendonly table (one to many relationship), inserts extra row to the base table
    -   Consider gp\_toolkit as place to provide diagnostics functions
    -   UAO:Run cdbfast crashRecovery related UAO changes thru Pulse-Storage
    -   UAO Test: Concurrency (select) with Compaction
    -   UAO: Test Fault injector while compaction
    -   UAO Test: Modify Stress test to include test cases for UAO
    -   UAO: CO tables with encoding - updates/ deletes
    -   Nikhil: Create UAO Pulse Projects in Pulse-Storage

CDBFAST TO TINC Migration - Test suite

-   -   Filerep\_end\_to\_end in TINC : Migrate all the Modules into TINC
    -   Filerep\_end\_to\_end in TINC : Run all the new UAO tests & existing tests

Analyze Rhel6.4 runs for storage

Rhel6.4 certification : Data\_corruption fts\_parallel\_1 - Rerun manually

**ToDo**

WAL Rep

-   -   Testing Wal-R: Positive Automation -standby in failover mode - filerep fail overs
    -   Wal-R: Verification : Imporvement: gpactivatestandby should exit out faster than current implemetantion if PGPORT is not set/5432
    -   WalR Verification: STO-469
    -   Testing Wal-R: Load Testing-
    -   WalR: Testing: OOM on standby
    -   Make 4xbasic Green: mkcheck.mkcheck.testCHECK1194 & other failing tests
    -   Wal-R: Test code cleanup and re-factor
    -   Wal-R: Change to tincrepo tests as needed to support multinode run
    -   Testing Wal-R: Longevtiy Testing
    -   Testing Wal-R: Stress Testing

UAO

-   -   UAO: Review Upgarde test with Dirk
    -   UAO Test: Run Stress Test for UAO Build
    -   UAO trigger & Analyze Regression tests
    -   UAO: Create tests to verify fix for MPP-21144 and MPP-21146 for row and column oriented tables
    -   Do CR for Divya
    -   UAO: Test OOD – Compaction
    -   UAO: Compaction and Out of memory
    -   UAO: Recreate .ans files for filerep and crash-recovery for vacuum changes to segment related messages
    -   UAO: Backup Behave tests - Failing with error 'report file not present'
    -   UAO : Check backup upgrade process - Make Pulse green
    -   Incremental backup after delete will fail with optimizer=on
    -   Error in postgis creation because of error in vacuum.h
    -   Fix the upgrade script for schema changes

CDBFAST TO TINC Migration - Test suite

-   -   Filerep\_end\_to\_end in TINC : Migrate Write the unit test was the Libs migrated
    -   Crash Recovery in TINC : Spike - Understand Crash recovery for Migration
    -   Create Crash Recovery Scenario Framework in TINC & add UAO testcases

Sep 9th
=======

<span style="color: rgb(34,34,34);">Velocity : 114</span>

<span style="color: rgb(34,34,34);">**Done**</span>

WAL Rep

-   -   Continue with STO-493: Testing Wal-R: Negative Manual + Automation - (management utilities) Part 1
    -   Fix the tests - Make it Green - Spill from Sprint 11 - instrument Build
    -   WalRepl Test: analyze & Fix Regression schedules: multinode resync schedules
    -   Assertion failure "!(!caughtup || caughtup\_within\_range)"
    -   Modify tests to stop/kill the master first before activating the standby
    -   Merge code from ashwin's private to walrep main
    -   Test gpmodcontrol
    -   Testing Wal-R: Upgrade Testing
    -   Walrepl regression schedule: GPDB-WalRepl-crash-recov-31-42, segment was not able to come up during fault injection
    -   Complete all the TODO mentioned in the code
    -   Review/add logging for walreplication
    -   WAL Based SR (Master Mirroring) - Code Reviews
    -   After promote, OLD master should not be allowed to act as MASTER
    -   Send Alert in walreceiver
    -   Merge WALREP Private Branch into PARISTX-WALREP

UAO

-   -   Merge UAO Private Branch into PARISTX-UAO
    -   UAO: Performance test for UAO
    -   UAO: Merge the cdb2 test cases for UAO into TINC REPO
    -   UAO Test: Code Coverage
    -   Change compaction threshold message to LOG instead of NOTICE
    -   gprecoverseg failure
    -   UAO: Test Persistent Table changes for UAO
    -   UAO Test: Create modified sql files for QP and MVCC and run against AOCS build
    -   UAO: Test fix for Vacuum from utility mode crashes the connection
    -   UAO: Test the fix for Vacuum and Vacuum full does not vacuum the
    -   pg\_aoseg.pg\_aoseg catalog table
    -   UAO Test: Create Huge Data set
        -   UAO: Test Huge Data Query Plan
        -   UAO: Test Huge Data Compaction
        -   UAO: Test Huge data Delete/Update for its impact on visimap
        -   UAO: Test Huge Data Sort
    -   UAO: Test: Run All Backup & Restore Schedules (DDAuto, Behave, other TINC etc)
    -   UAO: Analysis of Regression schedules
    -   UAO Incremental Backup: Fix OpenBAR solaris issues
    -   UAO Test: UDFs for Visimap

CDBFAST TO TINC Migration - Test suite

-   -   Create Filerep\_end\_to\_end Scenario Framework in TINC & add UAO testcases

**
**

**ToDo**

WAL REP

-   -   Add --exclude option to pg\_basebackup
    -   gpinitstandby : Do not give default path option when the default path is not actually possible
    -   Wal-R gpinitstandby: Creation of standby directory is not consistent with different options
    -   Imporvement: gpactivatestandby should exit out faster than current implemetantion if PGPORT is not set/5432
    -   gpactivatestandby: 'list index out of range' stdout message
    -   Testing Wal-R: Negative  Manual + Automation - (management utilities) Part 2
    -   Create WAL REP Pulse Projects in Pulse-Storage
    -   Wal-R: Verification : Imporvement: gpactivatestandby should exit out faster than current implemetantion if PGPORT is not set/5432
    -   Wal-R: Verification: gpinitstandby -n is not working
    -   WalR Verification: STO-469
    -   WAL REP: Making Multinode schedules Green
    -   Make 4xbasic Green: mkcheck.mkcheck.testCHECK1194 & other failing tests
    -   Testing Wal-R: Positive  Automation  -standby in failover mode - filerep fail overs
    -   Wal-R: Verification: gpactivatestandby on the same host: Error activating standby master: sequence item 1: expected string, NoneType found
    -   Wal-R: Verification : gpinitstandby : Do not give default path option when the default path is not actually possible
    -   Wal-R: Verification: gpactivatestandby - Unable to create another host as stanby

UAO

-   -   UAO: Incremental backup - A CTAS table after a full backup and vacuum shows up in the incremental table list
    -   Distribute data after CTAS/AT evenly
    -   UAO: Backup Behave tests - Failing with error 'report file not present'
    -   Run with gp\_test\_system\_cache\_flush\_force = plain
    -   UAO : Check backup upgrade process
    -   UAO Test: Upgrade tests for UAO Build
    -   UAO:Run cdbfast filerep related UAO changes & Crash Recovery thru Pulse-Storage 
    -   UAO Test: Concurrency (select) with Compaction
    -   UAO: Test Fault injector while compaction
    -   UAO Test: Run Stress Test for UAO Build
    -   UAO Test: Modify Stress test to include test cases for UAO
    -   UAO: CO tables with encoding - updates/ deletes
    -   UAO: Exchange and Split partition with update/delete
    -    Create UAO Pulse Projects in Pulse-Storage

CDBFAST TO TINC Migration - Test suite

-   -   Continue - Create Filerep\_end\_to\_end Scenario Framework in TINC & add UAO testcases

Sep 2nd
=======

<span style="color: rgb(34,34,34);">Velocity : 109</span>

<span style="color: rgb(34,34,34);">**Done**</span>

<span style="color: rgb(34,34,34);">\[TRIAGE\] - MPP-21049 :- FINRA primary got stuck while moving into changetracking</span>

<span style="color: rgb(34,34,34);">WAL Rep</span>

-   -   <span style="color: rgb(34,34,34);"> </span>Support movetransfilespace
    -   <span style="color: rgb(34,34,34);"> </span>Process MM FS/TS and DB info from Checkpoint Review & Merge
    -   <span style="color: rgb(34,34,34);"> </span>Fixing the testing issue of Walrepl cdbfast regression schedule: crash\_recov 04\_10 and 20\_31
    -   <span style="color: rgb(34,34,34);"> </span>wal\_sender\_timeout/wal\_receiver\_timeout
    -   <span style="color: rgb(34,34,34);"> </span>Add logic not to hault DB operations while Standby is catching-up
    -   <span style="color: rgb(34,34,34);"> </span>gpdbrestore from standby after activating as master wont work on a backup taken from old master
    -   <span style="color: rgb(34,34,34);"> </span>gpinitstandby -n is not working
    -   <span style="color: rgb(34,34,34);"> </span>GPDB WalRepl, gpstart DESCRIPTION has typoes.
    -   <span style="color: rgb(34,34,34);"> </span>Fixing the testing issue of WALREPL tincrepo
    -   <span style="color: rgb(34,34,34);"> </span>WAL REP: Need one lib to initstandby and handle filespace mismatch between master and new standby
    -   <span style="color: rgb(34,34,34);"> </span>gpactivatestandby - deprecate -c option (creation of another host as standby)
    -   <span style="color: rgb(34,34,34);"> </span>Modify tests to stop/kill the master first before activating the standby

<span style="color: rgb(34,34,34);"> </span>UAO

-   -   Rebase UAO on main
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: Full Backup & Restore
    -   <span style="color: rgb(34,34,34);"> </span>UAO : Vacuum does not work when its transaction isolation level is SERIALIZABLE
    -   <span style="color: rgb(34,34,34);"> </span>Ensure that vacuum eof truncation is executed when a compaction is dropped
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: Install AOCS build and run AOCS tests
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: Incremental Backup & Restore
    -   <span style="color: rgb(34,34,34);"> </span>UAO: Analyze table after alter table + update table generates error "array size exceeds the maximum allowed "
    -   <span style="color: rgb(34,34,34);"> </span>UAO: Segment crashes after alter table Add Col + update 1 row + Analyze
    -   <span style="color: rgb(34,34,34);"> </span>UAO: Incremental backup- Table from which rows are deleted not part of the incremental set
    -   <span style="color: rgb(34,34,34);"> </span>UAO: Incremental backup with option -j is not picking up the tables for backup correctly
    -   <span style="color: rgb(34,34,34);"> </span>Investigate UAO with optimizer=on
    -   <span style="color: rgb(34,34,34);"> </span>UAO: vacuum analyze on AO table fails in "dispatchVacuum" with ERROR "could not open relation with OID"
    -   <span style="color: rgb(34,34,34);"> </span>Prepare for Thursday demo
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: CO Compression - Test Plan Coverage
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: Spike UAO Design Doc & Test Plan
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: add test case to Filerep\_end\_to\_end in CDBFAST
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: add test case to crashrecovery test suite in CDBFAST
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: Run All cdbfast & TINC tests on AOCS build
    -   <span style="color: rgb(34,34,34);"> </span>Performance Test for AO
    -   <span style="color: rgb(34,34,34);"> </span>AO Test : Negative tests from Test Plan
    -   <span style="color: rgb(34,34,34);"> </span>UAO Test: gpexpand tests for UAO 
    -   <span style="color: rgb(34,34,34);"> </span>After vaccuum one row is missing

**ToDo**

WAL Rep

-   -   Continue with STO-493: Testing Wal-R: Negative Manual + Automation - (management utilities) Part 1
    -   Fix the tests - Make it Green - Spill from Sprint 11 - instrument Build
    -   Wal-R: Change to tincrepo tests as needed to support multinode run
    -   WalRepl Test: analyze & Fix Regression schedules: multinode resync schedules
    -   Make 4xbasic Green: mkcheck.mkcheck.testCHECK1194
    -   Wal-R: Verification : Imporvement: gpactivatestandby should exit out faster than current implemetantion if PGPORT is not set/5432
    -   WalR Verification: STO-469
    -   Wal-R: Verification: gpinitstandby -n is not working
    -   Test gpmodcontrol
    -   Merge WALREP Private Branch into PARISTX-WALREP
    -   Testing Wal-R: Upgrade Testing
    -   Send Alert in walreceiver
    -   Walrepl regression schedule: GPDB-WalRepl-crash-recov-31-42, segment was not able to come up during fault injection
    -   Complete all the TODO mentioned in the code
    -   Review/add logging for walreplication
    -   WAL Based SR (Master Mirroring) - Code Reviews
    -   After promote, OLD master should not be allowed to act as MASTER

UAO

-   -   Merge UAO Private Branch into PARISTX-UAO
    -   UAO: Performance test for UAO
    -   UAO: Merge the cdb2 test cases for UAO into TINC REPO
    -   UAO Test: Code Coverage
    -   UAO: Test Persistent Table changes for UAO
    -   UAO Test: Create modified sql files for QP and MVCC and run against AOCS build
    -   UAO: Test fix for Vacuum from utility mode crashes the connection
    -   UAO: Test the fix for Vacuum and Vacuum full does not vacuum the pg\_aoseg.pg\_aoseg catalog table
    -   UAO Test: Create Huge Data set
    -   UAO: Test Huge Data Query Plan
    -   UAO: Huge Data Compaction
    -   UAO: Test Huge data Delete/Update for its impact on visimap
    -   UAO: Test Huge Data Sort
    -   UAO: Test: Run All Backup & Restore Schedules (DDAuto, Behave, other TINC etc)
    -   UAO: Analysis of Regression schedules

<span style="line-height: 1.4285715;">CDBFAST TO TINC Migration - Test suite</span>

-   -   <span style="line-height: 1.4285715;">Create Filerep\_end\_to\_end Scenario Framework in TINC & add UAO testcases</span>

<span style="color: rgb(0,102,0);font-size: 24.0px;line-height: 1.25;">Aug 26th</span>

<span style="color: rgb(34,34,34);">Velocity : 107</span>

**Done**

<span style="color: rgb(51,51,51);">WAL Rep</span>

-   -   Basebackup Cleanup
    -   Promote missing record - Test failure in Pulse
    -   gpactivatestandby MUST confirm primary master is down before promoting the standby
    -   SPIKE: how FTS handles today's MM
    -   Stop WAL Send Server process
    -   Kill MM code
    -   Replace wait\_replication with more meaningful column
    -   walsender should handle ERROR
    -   walsender bugfix backport
    -   Testing Wal-R: Positive  Manual + Automation -management utilities(gpstop)
    -   Testing Wal-R: Negative  Manual + Automation -  (OOD, OOM) 
    -   Testing Wal-R: Positive  Automation  -backup/restore
    -   Testing Wal-R: Positive  Manual + Automation -management utilities(gpstate)
    -   WalRepl regression: list\_queries: pg\_terminate\_backend does not terminate sessions properly
    -   Prepare for Thursday demo - WAL REP
    -   Make 4xbasic Green: list\_mgmt\_expand, gpload2.gpload
    -   WalR Testing: gpexpand : Standby failed to start with  "FATAL","XX000","Unable to find entry for tablespace OID = 16994 when getting filespace directory paths (cdbpersistenttablespace.c:614)
    -   Testing Wal-R: Negative Manual + Automation - (management utilities) Part 1

UAO

-   -   Check cursor support in combination with update/deletes and vacuum.
    -   Fix index vacuum on append-only relation
    -   AO Column Compaction
    -   UAO: Regression Schedule - Multinode schedules
    -   UAO: Regression Schedule - Data Corruption(schedule\_data\_corruption)
    -   UAO Test: Run all regression tests (cdbfast & TINC)
    -   UAO: Vacuum generates warning  WARNING:  index "mytab\_int\_idx1" contains 0 row versions, but table contains 2 row versions
    -   Gpmigrator fails on UAO build
    -   UAO Test: Analyze/QP - GUC Statistics - Testing & Automation
    -   UAO Test: Transaction Management MVCC READ COMMITTED,SERIALIZABLE
    -   UAO: Create Tests for CO from Savita's AO tests (DDL & DML)
    -   Check UAO isolation semantics against heap's
    -   Allow compaction of segment 0
    -   Fix gp\_size\_of\_database
    -   UAO Test: Vacuum Use Vacuum concurrently with other tranx
    -   UAO test: Install AOCS build and run AO tests

IVECO - HOTFIX - 4.2.6.2IV1 - Verify MPP-20964

Mentoring Nikhil with Tinc Test(Manual run, Pulse configuration and failure analyzing).

Analyzed MAIN regression results for ORCA-POC merge

**
**

**ToDo**

WAL Rep

-   -   Support movetransfilespace
    -   gpdbrestore from standby after activating as master wont work on a backup taken from old master
    -   wal\_sender\_timeout/wal\_receiver\_timeout
    -   Add logic not to hault DB operations while Standby is catching-up
    -   gpactivatestandby - Unable to create another host as stanby
    -   Modify tests to stop/kill the master first before activating the standby
    -   gpinitstandby -n is not working
    -   GPDB WalRepl, gpstart DESCRIPTION has typoes.
    -   Testing Wal-R: Negative Manual + Automation - (management utilities) Part 2
    -   Testing Wal-R: Positive Automation -standby in failover mode - filerep fail overs
    -   WalRepl Test: analyze & Fix Regression schedules: multinode resync schedules

UAO

-   -   Revisit bitmap compression performance
    -   Check backup upgrade process
    -   Performance Test for AO
    -   Prepare for Thursday demo
    -   UAO Test : Negative tests from Test Plan
    -   Rebase UAO on main
    -   UAO Test: Incremental Backup & Restore
    -   UAO Test: Full Backup & Restore
    -   UAO Test: CO Compression - Test Plan Coverage
    -   UAO Test: Spike UAO Design Doc & Test Plan
    -   UAO Test: add test case to Filerep\_end\_to\_end in CDBFAST
    -   UAO Test: add test case to crashrecovery test suite in CDBFAST
    -   UAO Test: Concurrency (select) with Compaction
    -   UAO Test: Run All cdbfast & TINC tests on AOCS build
    -   UAO Test: Install AOCS build and run AOCS tests
    -   UAO: Test: Run All Backup & Restore Schedules (DDAuto, Behave, other TINC etc)
    -   UAO Test: gpexpand tests for UAO

Aug 19th
========

<span>Velocity : 110</span>

**Done**

WAL Rep

-   -   <span style="color: rgb(0,0,0);">Promote may miss some records to replay</span>
    -   <span style="color: rgb(0,0,0);">Upgrade: pg\_control file</span>
    -   <span style="color: rgb(0,0,0);">Upgrade: Remove gp\_master\_mirroring</span>
    -   <span style="color: rgb(0,0,0);">Upgrade: bootstrap XLOG</span>
    -   <span style="color: rgb(0,0,0);">Reflect new parameters to Standby Management Utilities in Usage</span>
    -   <span style="color: rgb(0,0,0);">Testing Wal-R: Negative  Manual + Automation - (killing processes) </span>
    -   <span style="color: rgb(0,0,0);">Testing Wal-R: Negative testing standby in failover mode others</span>
    -   <span style="color: rgb(0,0,0);">Merge XLOG cleanup changes</span>
    -   <span style="color: rgb(0,0,0);">Walrepl regression schedule, Work to make them green</span>
    -   <span style="color: rgb(0,0,0);">Mirror transition sometimes fail</span>
    -   <span style="color: rgb(0,0,0);">Investigate pulse failures by devs</span>
    -   <span style="color: rgb(0,0,0);">cdbfast\[RCA\] :list\_mgmt with standby : Query hanging on replication </span>

UAO

-   -   <span style="color: rgb(0,51,102);">Remove the toasted values of deleted tuples in UAO</span>
    -   <span style="color: rgb(0,51,102);">UAO Design Document and Code Review</span>
    -   <span style="color: rgb(0,0,0);">UAO: Create DML queries with multiple table joins</span>
    -   <span style="color: rgb(0,0,0);">Fix file seg information for AOCS in appendonlywriter.</span>
    -   <span style="color: rgb(0,0,0);">UAO Regression Schedule - AO CO compression(schedule\_aoco\_compression)  </span>
    -   <span style="color: rgb(0,0,0);">UAO\_REGRESSION 4x Basic Rerun after changing tests</span>
    -   <span style="color: rgb(0,0,0);">UAO:  Regression Schedule - Crash Recovery</span>
    -   <span style="color: rgb(0,0,0);">UAO: Create tables with very large number of objects</span>
    -   <span style="color: rgb(0,0,0);">UAO : Test Plan Review with Stake Holders</span>
    -   <span style="color: rgb(0,0,0);">UAO Review - Code review by devs</span>
    -   <span style="color: rgb(0,0,0);">UAO: Look into the compaction design and EOFproblem</span>
    -   <span style="color: rgb(0,0,0);">Ensure a crash during compaction is ok</span>
    -   <span style="color: rgb(0,0,0);">UAO Test: add sql test to Pulse</span>
    -   <span style="color: rgb(0,0,0);">UAO: Vacuum from utility mode crashes the connection</span>
    -   <span style="color: rgb(0,0,0);">Fix installcheck-good appendonly failure</span>
    -   <span style="color: rgb(0,0,0);">Deal with SERIALIZABLE transactions and Compaction</span>

<span style="color: rgb(0,0,0);">MPP-20856 \[TRIAGE\] :- cdbfast-crash\_recovery31\_42: storage-dev. Unable to start the cluster after a recovery failure</span>

<span style="color: rgb(0,0,0);">4.2.6.2 - work on bug fiuxes & monitoring Regression schedules for RC </span>

**
**

**ToDo**

WAL Rep

-   -   Basebackup Cleanup
    -   Standby shouldn't be promoted if it hasn't caught up
    -   SPIKE: how FTS handles today's MM
    -   Stop WAL Send Server process
    -   Kill MM code
    -   Replace wait\_replication with more meaningful column
    -   walsender should handle ERROR
    -   walsender bugfix backport
    -   Testing Wal-R: Positive  Manual + Automation -management utilities(gpstop)
    -   Testing Wal-R: Negative Manual + Automation - (management utilities) Part 1
    -   Testing Wal-R: Negative  Manual + Automation -  (OOD, OOM) 
    -   Testing Wal-R: Positive  Automation  -backup/restore
    -   Testing Wal-R: Positive  Manual + Automation -management utilities(gpstate)
    -   WalRepl regression: list\_queries: pg\_terminate\_backend does not terminate sessions properly
    -   WalR Testing: gpexpand : Standby failed to start with  "FATAL","XX000","Unable to find entry for tablespace OID = 16994 when getting filespace directory paths (cdbpersistenttablespace.c:614)
    -   Prepare for Thursday demo

UAO

-   -   Check cursor support in combination with update/deletes and vacuum.
    -   Fix index vacuum on append-only relation
    -   Revisit bitmap compression performance
    -   AO Column Compaction
    -   Check backup upgrade process
    -   Performance Test for AO
    -   UAO: Regression Schedule - Multinode schedules, Data Corruption
    -   UAO Test: Run all regression tests (cdbfast & TINC)
    -   UAO: Vacuum generates warning  WARNING:  index "mytab\_int\_idx1" contains 0 row versions, but table contains 2 row versions
    -   UAO Test: Analyze/QP - GUC Statistics - Testing & Automation
    -   UAO Test: Transaction Management MVCC READ COMMITTED,SERIALIZABLE

Aug 12th
========

<span style="color: rgb(0,0,0);">Velocity : 122</span>

**Done**

<span>WAL Rep</span>

-   -   <span>Test Plan Review with StakeHolders</span>
    -   <span>Recovery during standby promote needs to be idempotent</span>
    -   Manual Testing while standby in recovery mode - utilities
    -   Testing Wal-R: Positive  Manual + Automation -standby in recovery\_mode
    -   Cleanup archive recovery code (Startup Passes)
    -   Cleanup Xlog.c Code
    -   cdbfast \[TRIAGE\] :list\_mgmt with standby : Query hanging on replication
    -   Standby falling behind management (Xlog files reuse at standby)
    -   MPP-20725: cdbfast: checkmastermirrorintegrity: Redo encountered unexpected error 
    -   Testing Wal-R: Regression schedule
    -   Upgrade: pg\_stat\_replication view with pg\_stat\_get\_walsenders function
    -   Upgrade: pg\_database, pg\_authid and pg\_auth\_members
    -   WAL Based SR (Master Mirroring) - Code Coverage (Build project)
    -   Testing Wal-R: Positive  Manual + Automation-standby in failover mode: 
    -   Wal-R: Change to tincrepo  tests as needed to support multinode run Phase 1
    -   cdbfast Filerep\_full\_mirror\_failover: WalRepl: "PANIC","XX000","xlog write request 0/0 is past end of log 0/0 (xlog.c:2479)

<span>UAO</span>

-   -   <span>Support incremental backup</span>
    -   UAO: Regression schedule
    -   Add descriptionof pg\_class statistics to UAO design document
    -   Change policy of select insertion segment file
    -   Increase append-only version number
    -   Fix visimap relkind after Upgrade
    -   Updates & Deletion in AO Column Tables
    -   support compaction with parallel inserts
    -   UAO: All Possible Alter+Update+Delete & Update+Alter+Delete

<span>Worked on 4.2.6.2 Bug</span>

<span>Weekly Regression schedules</span>

-   -   <span>4.2x StorageDev Regression </span>
    -   <span>Weekly Regression schedule - 42x-Storage - Sprint 8</span>
    -   <span>Weekly Regression schedule - WAL Replication Schedule</span>
    -   <span>SPIKE: learn pulse like create, clone, configure projects</span>

<span>MPP-20488: PHP program using libpq for database connections results in database panics and restarts.</span>

<span>\[TRIAGE\] MPP-20859 FINRA : Persistent Table Free TID issue</span>

<span>cdbfast to TINC migration</span>

-   -   <span>GpVerify.py - Write Unit tests</span>
    -   <span>Incorporate the code-review comments for Gpstart.py</span>

<span>Training/mentoring Sessions</span>

-   -   Mentor Nikhil for regression schedules

**TODO**<span style="line-height: 1.4285715;"> </span>

WAL Rep

-   -   gpinitstandby -n is not working 
    -   Promote may miss some records to replay?
    -   gpactivatestandby - Unable to create another host as stanby
    -   Standby shouldn't be promoted if it hasn't caught up
    -   Upgrade: pg\_control file
    -   Upgrade: Remove gp\_master\_mirroring
    -   Upgrade: bootstrap XLOG
    -   Reflect new parameters to Standby Management Utilities in Usage
    -   Testing Wal-R: Negative  Manual + Automation - (killing processes) 
    -   Testing Wal-R: Negative testing standby in failover mode others
    -   Merge XLOG cleanup changes
    -   Basebackup Cleanup
    -   Walrepl regression schedule, Work to make them green
    -   Mirror transition sometimes fail
    -   Investigate pulse failures by devs

UAO

-   -   Revisit bitmap compression performance
    -   AO Column Compaction
    -   Performance Test for AO
    -   UAO: Create DML queries with multiple table joins
    -   Check UAO usage of persistent tables
    -   Fix file seg information for AOCS in appendonlywriter.
    -   Check backup upgrade process
    -   UAO Regression Schedule - AO CO compression(schedule\_aoco\_compression)  
    -   UAO\_REGRESSION 4x Basic Rerun after changing tests
    -   UAO:  Regression Schedule - Crash Recovery
    -   UAO: Create tables with very large number of objects
    -   UAO : Test Plan Review with Stake Holders
    -   UAO Review - Code review by devs

MPP-20856 \[TRIAGE\] :- cdbfast-crash\_recovery31\_42: storage-dev. Unable to start the cluster after a recovery failure

Aug 5th
=======

<span style="color: rgb(0,0,0);">Velocity : 121</span>

**Done**

WAL Rep

-   -   Disallow different filespace for pg\_xlog with master standby
    -   Migrate Gpinitstandby.py cdbfast library to tinc
    -   Testing Wal-R : Creating DDL test\_suite
    -   Testing Wal-R : Creating DML test\_suite
    -   Testing Wal-R: Positive Manual + Automation -management utilities(gpstart)
    -   Testing Wal-R: Positive Manual Testing -management utilities(gpactivatestandby)
    -   gpstate
    -   Resync Manager Fatals out during commit Transactions
    -   consumer process title is incorrect
    -   Dead lock between basebackup, checkpoint and backend
    -   Wal Sender Death Mgmt
    -   Fixed regression schedule failures for WalRepl, regression schedule crash-recov-filerep\_e2e\_w\_trans & FileRep\_Full\_mirror\_failover
    -   Testing Wal-R: Automation -management utilities(gpactivatestandby)
    -   Wal-R: Create local temp table hanging on WalR build
    -   WAL Replication TINC Test schedule failure (Timeline ID bump up Issue)

UAO

-   -   Write UAO design document in official template
    -   Provide an upgrade path for UAO
    -   Verify UAO behavior with concurrent operations
    -   Verify UAO behavior with different isolation levels
    -   Verify CLUSTER command with UAO
    -   Create a way to test blocking concurrent operations
    -   UAO: Install check failing on UAO Pulse Project
    -   Check that gp\_appendonly\_compaction\_threshold is ignored when VACUUM FULL is used.
    -   Check that an updated tuple is added to the index
    -   UAO:Enhance the Test Plan
    -   UAO:Automate Test cases (visimap)
    -   Add tuple id to visimap udf.
    -   Check drop column after compaction.
    -   Change the bitstream error handling concept
    -   Move bitmap compression to utils
    -   Start UAO tinc tests in pulse
    -   Work on review comments (round two)
    -   Check the visimap UDFs
    -   Create a visimap entry UDF
    -   Select during compaction fails
    -   Better negative case handling in the visimap UDFs
    -   Modify gp\_size\_of\_table\_disk to include the visimap
    -   UAO: Vacuum and Vacuum full does not vacuum the pg\_aoseg.pg\_aoseg catalog table

Weekly Regression schedules

-   -   4.2x StorageDev Regression - Week 4
    -   Weekly Regression schedule - 42x-Storage - Sprint 7
    -   Weekly Regression schedule - WAL Replication Schedule

Training/mentoring Sessions

-   -   Mentor Nikhil for gpdb installation on std configurations & Transition States
    -   Mentor Nikhil in QA-2560 Install P4 in VCloud VM & Run cdbfast tests

**In Progress**

CDBFAST to TINC Lib Migration

-   -   Gpinitstandby.py - Done with migrating the lib, need to create Unit tests

WAL Rep

-   -   Cleanup archive recovery code (Startup Passes)

UAO

-   -   UAO: Regression schedule - list\_basic\_metadata

Weekly

-   -   4.2x StorageDev Regression - Week 4

**ToDo**<span style="line-height: 1.4285715;"> </span>

WAL Rep

-   -   WAL Based SR (Master Mirroring) - Code Coverage (Build project)
    -   Testing Wal-R: Positive Manual + Automation -standby in recovery\_mode
    -   Testing Wal-R: Positive Manual + Automation-standby in failover mode
    -   Standby falling behind management (Xlog files reuse at standby)
    -   Testing WAL-R: Manual Testing while standby in recovery mode - utilities
    -   MPP-20725: cdbfast: checkmastermirrorintegrity: Redo encountered unexpected error
    -   MPP-20488: PHP program using libpq for database connections results in database panics and restarts.
    -   Testing Wa;-R: Regression schedule - list\_basic with standby
    -   Testing Wa;-R: Regression schedule - list\_queries with standby
    -   Testing Wa;-R: Regression schedule - list\_storage with standby
    -   Testing Wa;-R: Regression schedule - list\_mgmt\_expand with standby
    -   Wal-R: Change to tincrepo tests as needed to support multinode run Phase 1
    -   Recovery during standby promote needs to be idempotent
    -   Cleanup Xlog.c Code
    -   WAL Rep: Test Plan Review with StakeHolders

UAO

-   -   Support incremental backup
    -   UAO : Test Plan Review with Stake Holders
    -   UAO: Regression schedule - list\_storage
    -   UAO: Regression schedule - list\_mgmt\_expand & list\_queries
    -   UAO: Regression Schedule (Filerep\_end\_to\_end)
    -   Upgrade: pg\_database, pg\_authid and pg\_auth\_members
    -   Upgrade: pg\_stat\_replication view with pg\_stat\_get\_walsenders function

Weekly

-   -   Sprint 8: Weekly 42x-Storage-dev
    -   Sprint 8: Weekly 42x-Storage
    -   Sprint 8: Weekly WalRepl regression

CDBFAST to TINC Lib Migration

-   -   GpVerify.py - Write Unit tests

Training/Mentoring

-   -   SPIKE: learn pulse like create, clone, configure projects
    -   Mentor Nikhil for regression schedules

July 22nd
=========

**Done**

WAL Rep

-   -   Support gpinitsystem -s option
    -   <span>Make WAL based replication True synchronous</span>
    -   <span>Add WAL Recv details - Design Doc - initial </span>
    -   <span>Timeline ID, Pass 3 issues</span>
    -   <span>Testing Wal-R: Rearrange existing git/tincrepo to test and lib </span>
    -   <span>gpactivatestandby</span>
    -   <span>Update design document for gpinitstandby, gpactivatestandby</span>
    -   <span>Testing Wal-R: Define the DDL and DML statements in Testrail</span>
    -   <span>Implement gpinitsystem -s</span>
    -   <span>SPIKE - Standby Promotion Before Catchup
        </span>
    -   <span>Wal-R: cdbfast regression list\_storage: Gpcheckcat failed with Unexpected Internal Error</span>
    -   <span style="color: rgb(51,51,51);">Pulse Projects for regression schedules with standby</span>
    -   <span>List up detailed test cases for management scripts</span>

Investigating into the GetSnapshotData( ) Function

Reproduce MPP-20276

Update-able AO

-   -   <span>Create UAO visimap UDF to allow fine-grained testing and support</span>
    -   UAO compaction  and UDF testing
    -   <span>UAO testcase automation - Initial</span>
    -   <span>Allow index vacuum without a compaction</span>
    -   <span>Visibility map bitmap compression</span>
    -   <span>UAO: Not able to create UDF</span>
    -   <span>UAO should recognize gp\_select\_invisible</span>
    -   <span>UAO Bug Fx: Vacuum Full fails with error 'could not temporarily connect to one or more segments'</span>
    -   <span>UAO Bug Fix: aovisimap appends wrong  tuple after a table is updated</span>
    -   <span>create UAO preliminary tests in testrail </span>

Weekly

-   -   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev
    -   Investigating into the regression failures of Wal-Rel-build

4.2.6.1

-   -   Trigger Stress & Longevity on 4.2.6.1
    -   <span>Monitoring and investigating into failures of Regression schedule for 4.2.6.1</span>

cdbfast to TINC Lib migration

-   -   Gpstop.py
    -   Gpstart.py
    -   Config.py
    -   Gpfilespace.py

**In Progress**

UAO testcase automation 

<span>Monitor the Weekly regression schedules</span>

<span>Original Master (now dead) should not act as the new standby</span>

<span>Work in comments from UAO code review</span>

**ToDo**

WAL Rep

-   -   <span>SPIKE: MinRecoveryPoint</span>
    -   <span>SPIKE: wal\_level</span>
    -   <span>Work on Segment crash recovery failed in pass 3 after transaction\_abort\_after\_distributed\_prepared suspend</span>
    -   <span>Support different filespace for pg\_xlog</span>
    -   <span>Unpack checkpoint should be able to handle multiple checkpoints</span>
    -   <span>Testing Wal-R : Creating DDL & DML test\_suite</span>
    -   Testing Wal-R: Positive  Manual + Automation -standby in recovery\_mode
    -   Testing Wal-R: Positive  Manual + Automation -management utilities(gpinitstandby) 
    -   Promoted standby should not accept queries until role is changed
    -   Verify utility mode of primary master can replicated
    -   samehost, samenet  from postgres into pg\_hba
    -   WAL sender authorization
    -   gpstate

Investigating into the GetSnapshotData( ) Function

Rebase latest Main

Update-able AO

-   -   UAO: Mem leak in Update Tuple in  Utility mode. Verify the fix
    -   <span>Introduce GUC to avoid UAO compaction if segment file more than x% visible</span>
    -   <span>Update statistics for UAO tables</span>
    -   <span>Update all error messages for AO column orientation.</span>
    -   <span>Delaying allocation of visimap relation until the first delete/update</span>
    -   <span>Create elogif macro</span>
    -   <span>Revisit all Asserts to check which ones should be elog(ERROR) checks.</span>
    -   UAO: CreateTest Plan Doc - STP
    -   UAO: Create Pulse Project with all the existing schedules
    -   Continue automating UAO

Weekly

-   -   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev

cdbfast to TINC Lib migration

-   -   Gpinitstandby.py 
    -   <span>GpVerify.py</span>

July 15th
=========

**Done**

Investigate Architecture of MoreVRP Agent Project

Triage of MPP-20610, MPP-20601

<span>Worked with Escalation for Franklin - System is slow</span>

RCA for MPP-20276

<span>Longevity on 42x-Storage - Triggering</span>

<span>Stress testing on DCA against 42x Metro Branch</span>

UAO

-   -   Separate UAO visimap from block directory
    -   Fixed "Unable to select from table after add column on table"
    -   Make index-less AO tables up-dateable
    -   Compaction of AO row-orientation
    -   UAO - Installation & Test cases of the UAO prototype

WAL Rep

-   -   WAL Rep: createdb failed with Masterstandby running.
    -   keep\_wal\_segments
    -   Change the way walreceiver dies
    -   pg\_controldata should report updated fields
    -   Testrail initial test plan
        -   <span>Positive & Negative Integration Tests</span>
        -   <span>Positive & Negative Management Utilities Tests</span>
        -   <span>Stress/Longevity/Load/Performance</span>
    -   gpinitstandby

BFV

-   -   BFV(42x-Storage) - MPP-19890, MPP-19038

Weekly

-   -   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev

Training

-   -   Investigating in Google Unit Test Framework
    -   <span>WAL Rep: Going through Postgres streaming Rep</span>

**In Progress**

<span style="color: rgb(51,51,51);">Reproduce MPP-20276</span>

Create UAO visimap UDF to allow fine-grained testing and support

<span style="color: rgb(51,51,51);">cdbfast to TINC lib migration</span>

-   -   Gprecoverseg.py
    -   Gpstart.py
    -   Gpstop.py

**ToDo**

WAL Rep

-   -   Support gpinitsystem -s option
    -   <span>Make WAL based replication True synchronous</span>
    -   <span>Migrate to official Design Doc Template & Add WAL Recv details</span>
    -   <span>SPIKE: MinRecoveryPoint</span>
    -   <span>Work on Segment crash recovery failed in pass 3 after transaction\_abort\_after\_distributed\_prepared suspend</span>
    -   <span>Support different filespace for pg\_xlog</span>
    -   <span>Pulse Projects for regression schedules with standby</span>
    -   <span>Testing Wal-R: Rearrange existing git/tincrepo to test and lib </span>
    -   <span>gpactivatestandby</span>
    -   <span>List up detailed test cases for management scripts</span>
    -   <span>Update design document for gpinitstandby, gpactivatestandby</span>
    -   <span>Testing Wal-R: Define the DDL and DML statements in Testrail</span>

Investigating into the GetSnapshotData( ) Function

Update-able AO

-   -   UAO compaction  and UDF testing
    -   <span>UAO testcase automation</span>
    -   <span>Allow index vacuum without a compaction</span>
    -   <span>Visibility map bitmap compression</span>

Weekly

-   -   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev

cdbfast to TINC Lib migration

-   -   Gpinitstandby.py 
    -   Gpfilespace.py
    -   Config.py

July 8th
========

**Done**

Investigate Building MoreVRP Services in Visual Studio 2010

Triage of MPP-20507

Create Pulse projects to run against 42x-storage

Create 42x-Storage Build Project

Longevity on 42x-Storage - Triggering 

WAL Rep

-   -   pg\_basebackup -R
    -   Integrate latest //tinc/main and tincrepo/main to walrepl/tincrepo
    -   Tryout the new standby start & promote
    -   Handle smart shutdown during recovery
    -   Modify gpstart to adopt new replication
    -   SPIKE: does walsender need to connect database
    -   <span style="color: rgb(0,0,0);">Prepared transaction should be replicated - phase-2</span>
    -   Software Test Plan WAL Rep - Stress & load testing
        -   Component Testing
        -   Integration Testing
        -   Sessions not mentioned in other tasks

BFV

-   -   BFV(42x-Storage) - MPP-20264, MPP-20203, MPP-20426

Weekly

-   -   Monitoring Weekly Regression Test 4.2.x Storage

Training

-   -   Going Through Stress test
    -   Understanding GIT version system
    -   Understanding GPDB unit testing
    -   Setup TINC & gain familiarity on required Components/Classes to write unit test

**In Progress**

Separate UAO visimap from block directory

Investigate Architecture on MoreVRP Agent Project

MPP-20276 : Crash recovery after Signal -11 failed - RCA

WAL Rep : Create test Rail Project

<span style="color: rgb(51,51,51);">cdbfast to TINC lib migration</span>

-   -   Gprecoverseg.py
    -   Gpstart.py

**ToDo**

Unit test for MPP-19259

Trigger Stress Test on 42x Metro Branch

WAL Rep

-   -   Manual Testing on feature Branch
    -   gpinitstandby
    -   createdb failed with Masterstandby running
    -   Keep\_wal\_Segments
    -   Test Rail
        -   positive & negative Integration Tests for components (management utilities)
        -   positive & negative Integration Tests for components
        -   Stress/load/Performance
    -   Pulse Projects (Tincrepo tests in git, Regression schedules for Standby)
    -   Change the way WAL Rep dies
    -   pg\_controldata should report updated fields

Update-able AO

-   -   Installation of UAO prototype for testing
    -   UAO Test Cases for the prototype
    -   Make index-less AO tables update-able
    -   AO Row file compaction

cdbfast to TINC Lib migration

-   -   Config.py
    -   Gpstop.py 
    -   Gpfilespace.py

July 1st
========

**Done**

Triage MPP-20460/ADP core

Triage MPP-20408/FINRA insert

Additional issues encountered while verifying CR fixes(MPP-20507)

Main regression test analysis

Automated MPP-19038

Update-able AO Design Document

MPP-20447 \[RCA & Fix Design\]

Bug Fix Verification of MPP-19973

Sync from 4.2.x main to 42x-Storage & 42x-Storage-Dev

Persistent Table Presentation

MPP-19960 - Fixed & unit Test written

Bug Fix Verification in 42x-Storage-Dev (MPP-20426 & MPP-20264)

Understanding GIT Version system (Team)

Getting familiar with regression schedules (New Hires)

Bugs to be merged from 42x-Storage-Dev to 42x-Storage

Verify Duplicate bugs on ParisTX

WAL

-   -   Walsender needs to die after bgwriter
    -   Support Multiple filespaces with pg\_basebackup
    -   Prepared transaction should be replicated - phase-1

4.2.6 RC:

-   -   Stress & Longevity Monitored

**In Progress**

MPP-19259 Create unit test

MPP-20464 VarBlock is not valid: Triage, RCA, Fix

Try the new Standby start & promote

MPP-20276 Fix

**ToDo**

Investigate Building MoreVRP Services in Visual Studio 2010

Enable Pulse-projects to run against 42x-Storage

Create 42x-Storage Build Project

Create Cron Triggers on existing projects to run against 42x-Storage

WAL

-   -   Start Feature Test Plan for WAL
    -   Modify gpstart to adopt new replication
    -   SPIKE: does walsender need to connect database?
    -   Handle smart shutdown during recovery
    -   Prepared Transaction should be replicated - Phase-2
    -   WAL Rep-postgres Streaming Rep

cdbfast to TINC Migration

-   -   gprecoverseg.py
    -   config.py
    -   gpstart.py

Document generated by Confluence on May 17, 2016 19:14


