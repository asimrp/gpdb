<span id="title-text"> Storage & Access : DataManagement - Storage Planning Meeting Archive </span>
===================================================================================================

This page last changed on Dec 13, 2013 by prabaa1.

Archive of Meeting Agenda for DataManagement - Storage Planning Meeting
=======================================================================

Nov 22nd
========

**Retrospective**

**Triage Pending **

-    3 (Raghav), 6 (Hitoshi), 2(Ashwin), 2(George)

**Escalation**

-   DT, Bloomberg, MM, CCB, Symantic, Click Forensic, SDB

**Demo**

-   none

**This Week Update**

-   Hotfix : 4.2.6.3DT3 - Nikhil & Jason
-   4.2.7 & PARISTX
-   MPP Program Highlights
    -   Wrong Results Strategy: Dev working on Postgres SQL test suite
    -   Rel Eng Vacation Update:
        -   Ishan out staring 11/22, Working out of Pune: 11/26-11/28, Working out of Beijing:12/27-12/28, 1/2-1/3
    -   Plan for orca\_bench releases
    -   HAWQ Open Issues: <a href="https://jira.eng.pivotal.io/browse/GPSQL-1197" class="external-link">GPSQL-1197</a> - Rel cache memory leak for PL/R when ORCA falls back to Planner (Fix will not make Dec 1, will need to document)
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Green, Storage - ?
    -   Issues/Risks
        -   None
    -   Dependencies
        -   None

Nov 11th
========

**Retrospective**

**Triage Pending **

-    3 (Raghav) 1 (Hitoshi)

**Resolved bugs for verification**

-   <a href="https://confluence.greenplum.com/display/QA/GPDB+PARISTX+Dashboard" class="external-link">GPDB PARISTX Dashboard</a>
    -   1 open doc bug on UAO

**Escalation**

-   Bloomberg** **escalation - MPP-20572

**Completed Task from Nov 4th**

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

**Demo ( Wed 1.00 p.m)**

-   FTS demo - Ashwin

**This Week Update**

-   Friday afternoon reoccuring meeting - for accepting the stories in the current sprint
-   CCB hotfix - 4.2.6.3B ready to be tested - Nikhil to validate the Hotfix
-   MAIN Pre RC regression test validation
    -   Secure Hadoop testing
    -   gppkg name not rec for plr & pljava yet
    -   CC testing in progress
    -   KIT regression tests in progress
    -   Upgrade test 
        -   <span>Failed on Solaris - </span><a href="https://jira.eng.pivotal.io/browse/MPP-21784" class="external-link">MPP-21784</a><span> - PANIC due to SIGSEGV on Optimizer. This with Orca ON - Hence this is not a blocker for PARISTX.  This will not be fixed in EFT release since upgrade is not supported and this issue is isolated to Solaris. The bug is expected to be fixed in the next EFT release.</span>
-   MPP Program Highlights
    -   GIT Migration: 
        -   Stash user limit increased to 500 users
        -   Mirror repository created for all release branches
        -   Next week plan: Build from GIT
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Green, Storage - ?
    -   Issues/Risks
        -   None
    -   Dependencies
        -   None

Nov 4th
=======

**Retrospective**

**Triage Pending **

-    2 (Raghav)

**Resolved bugs for verification**

-   <a href="https://confluence.greenplum.com/display/QA/GPDB+PARISTX+Dashboard" class="external-link">GPDB PARISTX Dashboard</a>
    -   1 open doc bug on UAO

**Escalation**

**Completed Task from Oct 28th**

-   Walrep: Change pulse projects to point to main branch after the tests and code are merged to main/PARISTX
-   <span>Walrep: Merge tinc tests to //tincrepo/main</span>
-   MAIN Runs: TINC GPDB-PARISTX-UAO-Regression-instrument schedule
-   PARISTX Pre-RC: CrashRecovery & Filerep\_End\_to\_End Regression tests
-   PARISTX Pre-RC: WALREP Regression tests
-   PARISTX Pre-RC: Upgrade tests
-   Upgrade with Incremental backup and part/subpart tables
-   UAO:Trigger Longevity Validation and Install Command Center
-   Automate (MPP-21545) DOTOMI : pg\_terminate\_backend causes postmaster reset
-   Automate Online Help: MPP-21655 - Updates for gpinitstandby, gpactivatestandby
-   Fix and integrate the AO filedump tool to MAIN
-   Hotfix: Validate IVECO hotfix 4.2.6.3IV2
-   IVECO hotfix 4.2.6.3IV2
-   FTS: Detect IO issue in FTS check in segments
-   <span style="color: rgb(0,0,0);">PARISTX Pre-RC: Stress tests</span>
-   <span style="color: rgb(255,0,0);"><span style="color: rgb(0,0,0);">CR review comments for Gpstop and Gpstart -</span> <span style="color: rgb(0,0,255);">to add to Pivotal Tracker</span></span>
-   <span style="color: rgb(255,0,0);"><span class="s1"><span><a href="https://jira.eng.pivotal.io/browse/STO-862" class="external-link">STO-862</a> </span></span><span style="color: rgb(0,0,0);">gppkg Automation -</span> add a label</span>
-   <span style="color: rgb(255,0,0);"><span class="s1"><span><a href="https://jira.eng.pivotal.io/browse/STO-857" class="external-link">STO-857</a><span style="color: rgb(0,0,0);"> </span></span></span><span style="color: rgb(0,0,0);">PARISTX Pre-RC: UAO Regression tests -</span> 3 backup bugs opened need to be closed by ACE, need to create new story <span style="color: rgb(0,0,0);">-</span><span> </span><span style="color: rgb(0,0,255);">to add to Pivotal Tracker</span></span>
-   <span style="color: rgb(0,51,102);">GPPC gppkg update for 4.3</span>
-   <span style="color: rgb(0,51,102);">DR: Something similar to pg\_basebackup in Go</span>
-   <span style="color: rgb(0,51,102);">Checkin AO filedump in MAIN</span>
-   <span style="color: rgb(0,51,102);">MPP-21741 \[Triage\] - Unexpected internal error: Master 12657 received signal 11 in worker thread</span>
-   <span style="color: rgb(0,51,102);">Investigate Stress test issue with UAO</span>
-   <span style="color: rgb(255,0,0);">change file rep runs to point to MAIN - to move to this sprint</span>

**Demo ( Wed 1.00 p.m)**

-   FTS demo - Ashwin

**This Week Update**

-   MAIN Pre RC regression test validation
    -   Stress Test
    -   retrigger UAO tests after ACE fix
    -   Merge Upgrade tests
-   MPP Program Highlights
    -   Shanghai SP2 is GA (shipping from manufacturing now)
    -   Wrong results bug: <a href="https://jira.eng.pivotal.io/browse/MPP-20952" class="external-link">MPP-20952</a> (ALL, NOT IN queries with null values) - continue to work on this but if not done by PARISTX RC, will push out to next release
    -   GIT Migration Update
        -   GPDB blocked by Stash license limit hit
    -   China Construction Bank Escalation - Lei, Hitoshi
        -   600K Partitioned tables
    -   Goals for HAWQ in Q1:
        -   Full UDF Support
        -   hawq toolkit (similar to gptoolkit)
        -   Expansion
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Green, Storage - ?
    -   Issues/Risks
        -   None
    -   Dependencies
        -   None

Oct 28th
========

**Retrospective**

**Triage Pending **

-    3 (Hitoshi), 3 (Raghav)

**Resolved bugs for verification**

-   <a href="https://confluence.greenplum.com/display/QA/GPDB+PARISTX+Dashboard" class="external-link">GPDB PARISTX Dashboard</a>
    -   4 bugs to be Verified for UAO - Savita
    -   1 open doc bug on UAO

**Escalation**

**Completed Task from Oct 21st**

-   UAO
    -   UAO: Upgrade with 100K mixture of (AO, CO)
    -   UAO: Manual Upgrade 4.0.x -&gt; 4.1.x -&gt; 4.2.x -&gt; 4.3.x
    -   UAO:Verify MPP-21616 after merge to main
    -   UAO:Verify the fix for MPP-21661
    -   MPP-21616: Correct pseudo compaction drop
    -   Integrate UAO stress test to main
    -   UAO: Verify MPP-21616
    -   Triage MPP-21676 
-   CDBFAST To TINC Migration
    -   Crash-Recovery: cdbfast to TINC : Incorporate CR comments
    -   Crash-Recovery: cdbfast to TINC : rerun after incorporating CR comments
    -   Crash-Recovery: cdbfast to TINC : schema topology with all tests in pulse & analyze
    -   Adding new sql for Schema topology
-   WALREP
    -   WALREP: Manual Upgrade 4.0.x -&gt; 4.1.x -&gt; 4.2.x -&gt; 4.3.x
    -   Walrep: Failback mechanism on walrepl tinc tests need to be changed: ref: MPP-21535
    -   MPP-21655 - Updates for gpinitstandby, gpactivatestandby - Manual Verification
    -   WalRep: Change Tinc test case after fixing the fail back to old master method
    -   <span class="s2"><a href="https://jira.eng.pivotal.io/browse/STO-764" class="external-link">STO-764</a> </span><span style="color: rgb(255,0,0);">Walrep: Merge tinc tests to //tincrepo/main</span>
-   Hotfix
    -   Hotfix: MPP-21545 DOTOMI : pg\_terminate\_backend causes postmaster reset
-   MAIN Regression Run Validation
    -   MAIN Runs: List\_weekend\_customer schedule
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
-   Escalation
    -   CCB escalation: hang issue with TRUNCATE
    -   <span style="color: rgb(0,51,102);">MPP-21609 :- MediaMind gprecoverseg issue w/ catalog corruption</span>
-   SPIKE: Building up GPDB TODOs
-   Look at possible next steps for FTS to detect the server hung issues
-   <span style="color: rgb(0,51,102);">Finalize GPDB Roadmap (Storage Perpsective)</span>
-   <span class="s2"><a href="https://jira.eng.pivotal.io/browse/STO-855" class="external-link">STO-855</a> </span><span style="color: rgb(255,0,0);">Fix and integrate the AO filedump tool to MAIN</span>

**Demo ( Wed 1.00 p.m)**

-   Go Overview - Hitoshi

**This Week Update**

-   MAIN Pre RC regression test validation
    -   Stress
    -   UAO Regression test analysis
    -   WALREP Regression test analysis
-   MPP Program Highlights
    -   Command Center
    -   GIT Migration
    -   Dotomi Escalation: 9-rack cluster
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Green
    -   Issues/Risks
        -    None
    -   Dependencies
        -   None

Oct 21st
========

**Retrospective**

**Triage Pending **

-    4 (Hitoshi), 3 (Raghav),  1(Ashwin)

**Resolved bugs for verification**

-   <a href="https://confluence.greenplum.com/display/QA/GPDB+PARISTX+Dashboard" class="external-link">GPDB PARISTX Dashboard</a>
    -   3 bugs to be Verified for WALREP - Divya
    -   2 open bugs, 1 doc bug on UAO

**Escalation**

-   CCB - MPP-21625 - truncate Hang issue
-   CCB - MPP-21621 - Master Instance aborted as pg\_resetxlog
-   <span>Hotfix: MPP-21545 DOTOMI : pg\_terminate\_backend causes postmaster reset</span>

**Completed Task from Oct 14th**

-   WALREP
    -   WALREP: Trigger 4x basic schedules with standby after fix from MPP-21413
    -   TRIAGE and fix MPP-21413
    -   Walrep: Verify Upgrade issue
    -   MAIN Runs: TINC GPDB-WalRepl-Miscellaneous schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Persistent schedule
    -   MAIN Runs: TINC GPDB-WalRepl-Filerep schedule
-   CDBFAST To TINC Migration
    -   Crash-Recovery: cdbfast to TINC : Schema topology with single test
    -   Crash-Recovery: cdbfast to TINC : Raise code review
    -   Crash-Recovery: cdbfast to TINC : schema topology with all tests on local machine
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Incorporating all the CR comments
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Rerun after incorporating comments
-   UAO
    -   Merging ans.uao files in 4 3 branch
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
-   Persistent Tables
    -   PT: Integrate PARISTX-PT catalog changes to MAIN
    -   MPP-19161 Post recovery PT catalog changes Upgrade test
    -   PT: Trigger & Monitor Upgrade tests after integration of PARISTX-PT catalog changes to MAIN

**Demo ( Wed 1.00 p.m)**

-   No Demo

**This Week Update**

-   Upcoming Merges from storage
    -   UAO Bug fixes (<a href="https://jira.eng.pivotal.io/browse/MPP-21639" class="external-link">MPP-21639</a> - Stress test, <a href="https://jira.eng.pivotal.io/browse/MPP-21616" class="external-link">MPP-21616</a> -  Concurrent Vacuum
-   **<span style="color: rgb(0,0,255);">Savita</span>** to work with MArbin on pinning the successful test runs for the UAO runs.
    -   Update as of 10/22/2013 10:50 am. 
        -   The confluence dashboard to track the regression in main has been  updated with an extra column to track successful runs.
        -   40 schedules were run. 38 passed successfully. 1 MPP-bug was opened (BehaveBackupRestore) and 1 is being evaluated (list\_weekend\_customer)
        -   All the URLs for successful runs, that have been updated in the  confluence dashboard, have been pinned in pulse.
-   MPP Program Highlights
    -   FITT System Test Framework - <a href="http://confluence.greenplu/" class="external-link">http://confluence.greenplu</a><a href="http://m.com/display/QA/" class="external-link">m.com/display/QA/</a>System+Testing+Framework+%28STF%29+Requirements
    -   AO =&gt; Append Optimized
    -   gppkg name change for 4.3.0.0
    -   orca\_bench Plan
    -   GIT Migration: HAWQ complete, GPDB to follow
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Green
    -   Issues/Risks
        -   DCA team should make sure that the latest DCA software is compatable with the latest Master Mirroring 
    -   Dependencies

Oct 14th
========

**Retrospective**

**Triage Pending **

-    2 (Hitoshi), 2 (Raghav),  1(Dirk)

**Resolved bugs for verification**

-   [GPDB PARISTX Dashboard](https://confluence.eng.pivotal.io/display/QA/GPDB+PARISTX+Dashboard)

**Escalation**

-   ?

**Completed Task from Oct 7th**

-   WALREP
    -   WAL REP: Sync from MAIN
    -   Walrep: Merge cdbfast private branch changes to Release\_4\_3
    -   WALREP: Trigger tests after Sync from MAIN
    -   Triage MPP-21535
    -   Walrep: Upgrade 4\_3 baseline need to be changed to avoid checking attrelid&gt;10000
    -   Commiting the fix for MPP-21413
    -   Fix oid&gt;10000 issue using an init\_file ignore
    -   Analyse main regression runs
    -   <span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-781" class="external-link">STO-781</a></span>WALREP: Trigger 4x basic schedules with standby after fix from MPP-21413
-   CDBFAST To TINC Migration
    -   cdbfast to TINC Mig: Pulse monitor & retrigger if req for Filerep (Cont.)
    -   Crash Recovery in TINC : Spike - Understand Crash recovery for Migration
    -   Crash-Recovery: cdbfast to TINC : Filerep e2e
    -   cdbfast to TINC Mig: Trigger the test in Pulse Crash Recovery\_Filerep e2e
    -   Crash-Recovery cdbfast to TINC : Add sqls and run the test\_cases
-   UAO
    -   UAO: Sync from MAIN
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
    -   UAO: Check on Upgrade performance with 100 K tables (Cont.) To Trigger after the fix
    -   <span class="s1"><a href="https://jira.eng.pivotal.io/browse/STO-742" class="external-link">STO-742</a></span>Fix upgrade test case
-   Escalation
    -   Triage MPP-21458
    -   MPP-21415 \[RCA\] :- Broadridge Checkpoint Failure
    -   Triage MPP-21465: Query unable to acquire lock on relation
    -   Triage MPP-21458: 58186536:Mediamind:Master restarted with error: "PANIC: Unexpected internal error: Master process received signal SIGSEGV"

**Demo ( Wed 1.00 p.m)**

-   Check with Saurin on Performance numbers ?

**This Week Update**

-   Upcoming Merges from storage
    -   WALREP (3 bug fixes) - Monday 10/14/2013 
        -   <span style="color: rgb(255,0,0);">Incude <a href="https://jira.eng.pivotal.io/browse/MPP-21563" class="external-link" title="Upgrade_test: Extra row in pg_proc after upgrade with walrep"><span style="color: rgb(255,0,0);">MPP-21563</span></a> in Merge JIRA - WALREP will not be Merged today, since we reopened the issue.</span>
    -   UAO (Feature Branch Merge) - Thursday 10/17/2013
    -   PT  (catalog change for additional debugging) - Friday 10/18/2013
-   UAO Stress test ran into OOD on the 4th day 
    -   Lets trigger the stress test again in a clean ENV on UAO Branch
-   MPP Program Updates
    -   FITT System Test Framework - <a href="http://confluence.greenplu" class="uri" class="external-link">http://confluence.greenplu</a><a href="http://m.com/display/QA/" class="external-link">m.com/display/QA/</a>System+Testing+Framework+%28STF%29+Requirements
    -   <span>Tech Ops SLA</span>
        -   Operating Hours: 8am - 5pm
        -   H/W Issues: 3 business days
        -   Acknoledgement of Sev 1 within 24 hours
        -   Set Severity appropriately, Priority will be set by TechOps during Triage
    -   gppkg name change for 4.3.0.0
        -   Teams providing estimate (Ace - Dev, Jacqui - QA, Ed - Builds)
-   <span>Status to be included in MPP Program Team meeting</span>
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Yellow
    -   Issues/Risks
        -   UAO Stress test ran into OOD on the 4th day
        -   Upgrade
        -   Schedules which needs to triggered
        -   Crash Recovery TINC Migration
    -   Dependencies

Oct 7th
=======

**Retrospective**

**Triage Pending **

-    4 (Hitoshi), 4 (Raghav), 12(George), 1(Dirk) , 1(Ashwin)

**Resolved bugs for verification**

-   [GPDB PARISTX Dashboard](https://confluence.eng.pivotal.io/display/QA/GPDB+PARISTX+Dashboard)

**Escalation**

-   Threatmatrix -
-   Broadbrige-  21415

**Completed Task from Sep 30th **

-   WALREP
    -   Walrep: Run weekend, resource mgm will be run at the point of integration
    -   WalRep: Review upgrade tests
    -   Triage asnd FIX MPP-21413
    -   WalRep: Create a script to do automatic failover of stress and re-trigger after n days
    -   WalRep:Run Madlib tests on Walrep Build
    -   WALRep longevity with Guc on
    -   WalRep: Run QP-TINC one more time before integration
    -   Walrep: Persistent tables test\_suite on pulse
    -   WAL REP: Sync from Main after QP checks in catalog changes
    -   WAL REP: Run Upgrade tests after sync from MAIN
-   CDBFAST to TINC Mig:
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - send code review
    -   CDBFAST to TINC Mig: clean run for all sqls in filerep
    -   CDBFAST to TINC Mig: Incorporating review comments
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Trigger in Pulse
-   UAO
    -   UAO: Regression Schedule : pg\_twophase\_removal
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
    -   <span style="color: rgb(0,51,102);">UAO: Analyse internal Performance Test</span>
    -   <span style="color: rgb(0,51,102);">UAO: Lazy visimap creation</span>
    -   <span style="color: rgb(0,51,102);">MPP-21013: UAO hangs intermittently</span>
-   <span style="color: rgb(0,51,102);">Add CR comments MPP-20077</span>
-   Triage/RCA - MPP 21415 Checkpoint Failure

**Demo ( Wed 1.00 p.m)**

-   Lazy Visimap demo (Dirk)
-   Check with Saurin on Performance numbers

**This Week Update**

-   WAL REP to Sync from MAIN - Run WALREP Smoke with tests enabled to verify all the 4.2.6.1/4.2.6.2 test failures
-   WALREP to Merge to MAIN
-   UAO to sync from MAIN with WALREP changes
-   Trigger Longevity, Stress & all Regression tests on UAO build (cdbfast & TINC)
-   MPP Program Updates - covering from next week
-   WAL REP design doc 
-   Create a story for Design Doc for UAO
-   4.2.6.3 :
-   RC2 with ACE changes
-   TV Monitor
    -   Login Credentials - Username - "storage" & <span style="color: rgb(0,0,0);"><a href="http://passwordstorage123/" class="external-link">password:storage123</a></span>
    -   [Remote access - ECHO MacMini Access](https://confluence.eng.pivotal.io/display/ECHO/ECHO+MacMini+Access) - <vnc://137.69.6.227>
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Yellow
    -   Issues/Risks
        -   Fixed interconnect & Vacuum bugs this weekend. Validating the test runs. 
        -   Delay in Integration 
    -   Dependencies

Sep 30th
========

**Retrospective**

**Triage Pending - 7**

-    6 (Hitoshi), 2 (Raghav)

**Resolved bugs for verification**

-   8 (Doc bugs & will be verified with 4.2.x regression test runs)

**Escalation**

-   DOTOMI
-   GE

**Completed Task from Sep 23rd **

-   CDBFAST to TINC Mig:
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - write unit tests and code cleanup
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - add gpstate and checkintegrity functions
    -   CDBFAST to TINC Mig: Filrep\_end\_to\_end - Run with all sqls from UAO private branch
    -   CDBFAST to TINC Migration: Handle external table and hybrid partition table cases
-   UAO:
    -   <span class="s1"><a href="http://jira.eng.pivotal.io/browse/STO-699" class="external-link">STO-699</a> </span><span style="color: rgb(0,51,102);">UAO: Remove unused guc from uao code and update the design doc</span>
    -   UAO: Spike for Multinode setup & Run one cdbfast schedule by Savita
    -   UAO: Run filerep\_end\_to\_end on Stand alone machines
    -   UAO: Make Crash Recovery tests green in non pulse ENV
    -   UAO Test: Test all GUCs in Test Plan
    -   UAO: Deal with backup behave tests
    -   UAO Test: Cursors read only/binary//hold/no-scroll
    -   UAO: Regression Schedule - Sub transaction Limit removal
    -   UAO: Analyzing/Reproducing incremental backup failures
    -   UAO update should use new delete approach
    -   UAO Test after upgrade if gptoolkit/visimap UDFs are created
    -   UAO Upgrade Demo
    -   UAO Behave BackupScale: Performance test- Shows difference in the time taken to do gpcrondump
-   WAL REP:
    -   <span class="s1"><a href="http://jira.eng.pivotal.io/browse/STO-217" class="external-link">STO-217</a> </span><span style="color: rgb(0,51,102);">xlogdump output changes</span>
    -   <span class="s1"><a href="http://jira.eng.pivotal.io/browse/STO-242" class="external-link">STO-242</a> </span><span style="color: rgb(0,51,102);">Testing Wal-R: Longevtiy Testing - Ran into a known issue which is fixed in 4.2.6.1</span>
    -   WalR: Testing: OOM on standby
    -   Design doc update to reflect switch changes for gpactivatestandby and gpinitstandby
    -   Data verifiicationa after failover to standby
    -   Run Stress Test for WalRepl Build
    -   WAL REP: Stress test with failover
    -   OOM tips for WAL REP
    -   WAL REP: Trigger All tests (TINC & cdbfast)
    -   WALRep - Design Doc
    -   WALRep - Increasing memory consumption by Standby startup
    -   Enable the PARISTX-WALREP Regression schedules with Standby
    -   WalRep: Change the gpactivate library to use -f only if it failed on the first attempt
-   MPP-21274 Triage
-   \[Triage\] MPP-21209 - West -- Double fault of one segment when replacing a segment server
-   Rhel6.4 : IPV6 Testing - Failure analysis and rerun
-   <span class="s1"><a href="http://jira.eng.pivotal.io/browse/STO-693" class="external-link">STO-693</a> </span><span style="color: rgb(0,51,102);">MPP-20077</span>

**Demo ( Wed 1.00 p.m)**

-   Rel Cache - Hitoshi
-   WalR: Testing: OOM on standby - Divya

**This Week Update**

-   WAL REP: Sync catalog from MAIN and trigger Upgrade tests
-   <span style="color: rgb(255,0,0);"><span>Divya & Jason</span> - Make sure that the cdbfast test are synced from 4.3 branch - All cdbfast & TINC - Need to create a story for this sprint</span>
-   <span style="color: rgb(255,0,0);">Divya & Jason - Run weekend, resource mgm, madlib will be run at the point of integration</span>
-   Issues with Pulse-Storage ENV 
-   Upgrade Ownership - UAO
-   WAL REP design doc 
-   RHEL 6.4 Certifictaion : Sign Off today
-   4.2.6.3 :
-   Signoff today
-   TV Monitor
    -   Login Credentials - Username - "storage" & <span style="color: rgb(0,0,0);"><a href="http://passwordstorage123/" class="external-link">password:storage123</a></span>
    -   [Remote access - ECHO MacMini Access](https://confluence.eng.pivotal.io/display/ECHO/ECHO+MacMini+Access) - <vnc://137.69.6.227>
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Yellow
    -   Issues/Risks
        -   No inputs from the Platform team on the Design Doc Review for WAL REP
        -   New Requirement for Lazy visi map
        -   No resolution on the interconnect bug
    -   Dependencies

Sep 22nd
========

**Retrospective**

-   <span style="color: rgb(255,0,0);">GPCloud is not stable - <span style="color: rgb(0,0,0);">Owner: Anu</span></span>
-   <span style="color: rgb(0,128,0);">Good bugs found during last week - WAL REP ( stress(failover), checkmirror integrity) </span>
-   <span style="color: rgb(255,0,0);">Please highlight all the bugs found which is assigned to a different scrum team - provide in the planning meeting - <span style="color: rgb(0,0,0);">Owner: Team</span></span>
-   <span style="color: rgb(0,0,0);">cdbfast Test Sync from 4.3 branch - for both WAL REP & UAO -  Team</span>
-   <span style="color: rgb(0,0,0);">Run weekend, resource mgm, madlib will be run at the point of integration - Team</span>

**Triage Pending - 7**

-    3 (Hitoshi), 2 (Raghav), 1 (Ashwin), 1 (Anu)

**Storage Scrum Bugs**

-   0

**Resolved bugs for verification**

-   8 (Doc bugs & will be verified with 4.2.x regression test runs)

**Escalation**

-   ?

**Completed Task from Sep 16th **

-   WALREP
    -   <span style="color: rgb(0,0,0);"><span class="s1"><a href="http://jira.eng.pivotal.io/browse/STO-641" class="external-link"><span style="color: rgb(0,0,0);">STO-641</span></a></span>WALRep - Design Doc</span>
    -   <span style="color: rgb(0,0,0);"><span class="s1"><a href="http://jira.eng.pivotal.io/browse/STO-651" class="external-link"><span style="color: rgb(0,0,0);">STO-651</span></a></span>Add unit test for verifying XLOG is not cleaned on segments with prepared xans</span><span style="color: rgb(255,0,0);">
        </span>
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
    -   FIX MPP-21228<span style="color: rgb(255,0,0);">
        </span>
    -   Fix MPP-21267:WalRep:Stress Test: Create database failed with "ERROR","58P01","could not stat file ""global/pg\_internal.init.31771"": No such file or directory Fix contrib build
    -   WalRep: Re-trigger stress test with fix for 21282
    -   Trigger the Longevity test for 4.3 Paristx Walrep
-   UAO:
    -   Savita : Making UAO Pulse/Pulse-Storage Green for Filerep\_end\_to\_end & Crash Recovery
    -   UAO: Create tests to verify fix for MPP-21144 and MPP-21146 for row and column oriented tables
    -   UAO: Recreate .ans files for filerep and crash-recovery for vacuum changes to segment related messages
    -   UAO: Review Upgarde test with Dirk
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
-   Filerep\_end\_to\_end in TINC : Check with sample test case, migrate from scenario test to GPDB test, migrate to library
-   \[Triage\] MPP-21228 Stress test failure
-   Fix 42x OSx build failure

**Demo ( Wed 1.00 p.m)**

-   Upgrade UAO - Dirk
-   Rel Cache - Hitoshi

**This Week Update**

-   WAL REP
    -   Trigger full stress - mid week after failover
    -   Longevity al ready triggered  - validation to be done on Thursday
    -   <span style="color: rgb(0,0,0);">weekend, resource mgm, madlib to be run</span>
    -   <span style="color: rgb(0,0,0);">Run the PT test on WAL REP</span>
-   UAO:
    -   Incr Backup is not green & Performance test fails
    -   Saurin is Running Performance test
    -   Revisit GUCs & Cursor tests
    -   FUll regression run on UAO (pulse & pulse-eng) (include filerep\_end\_to\_end & Crash recovery)
    -   Trigger/Run QP tests runs on UAO
    -   Need to run Stress & Longevity
    -   filerep\_end\_to\_end & crash recover in TINC before integration.
-   Issues with Pulse-Storage ENV 
-   4.2.x Regression test runs triggered
-   RHEL 6.4 Certifictaion in progress
-   4.2.6.3 :
-   One bug committed for 4.2.6.3 - MPP-20964 - <span style="color: rgb(0,0,0);">IVECO c/o IBM Pero - Stuck Transactions in DB cause other queries to wait</span>
-   TV Monitor
    -   Login Credentials - Username - "storage" & <span style="color: rgb(0,0,0);"><a href="http://passwordstorage123/" class="external-link">password:storage123</a></span>
    -   [Remote access - ECHO MacMini Access](https://confluence.eng.pivotal.io/display/ECHO/ECHO+MacMini+Access) - <vnc://137.69.6.227>
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Yellow
    -   Issues/Risks
        -   GPCloud is not reliable. We will be using Pulse & Pulse\_eng
        -   Upgrade test was failing on MAIN - failed with syntax issues - MAIN is not green
        -   Stack Trace - MPP <img src="s/en_GB-1988229788/4224/875a2723d3ca93f267db5c35cdbfcf91f5910d2d.36/_/images/icons/emoticons/help_16.png" alt="(question)" class="emoticon emoticon-question" />
    -   Dependencies

Sep 16th
========

**Retrospective**

**Triage Pending - 7**

-    2 (Hitoshi), 2 (Raghav), 1 (Dirk), 2 (Anu)

**Storage Scrum Bugs**

-   2 (Dirk)

**Resolved bugs for verification**

-   8 (Doc bugs & will be verified with 4.2.x regression test runs)

**Escalation**

-   Dotomi - MPP-21103 floating point interpretation have changed from DCA v1 to v2 due to glibc version change.  No fix or something from Storage, but just consultation.

**Completed Task from Sep 9th **

-   WAL Rep
    -   Add --exclude option to pg\_basebackup
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
-   UAO
    -   UAO: Incremental backup - A CTAS table after a full backup and vacuum shows up in the incremental table list
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
-   CDBFAST TO TINC Migration - Test suite
    -   Filerep\_end\_to\_end in TINC : Migrate all the Modules into TINC
    -   Filerep\_end\_to\_end in TINC : Run all the new UAO tests & existing tests
-   Analyze Rhel6.4 runs for storage
-   Rhel6.4 certification : Data\_corruption fts\_parallel\_1 - Rerun manually

**Demo ( Wed 1.00 p.m)**

-   Dirk - on UAO deletes 
-   Upgrade - WAL REP (Hitoshi)

**This Week Update**

-   Issues with Pulse-Storage ENV 
-   4.2.x Regression test runs triggered
-   Orca team create a pulse project for upgrade
-   RHEL 6.4 Certifictaion in progress
-   In the process of making original regression schedules green - WAL REP
-   4.2.6.3 :
-   One bug committed for 4.2.6.3 - MPP-20964 - <span style="color: rgb(0,0,0);">IVECO c/o IBM Pero - Stuck Transactions in DB cause other queries to wait</span>
-   TV Monitor
    -   Login Credentials - Username - "storage" & <span style="color: rgb(0,0,0);"><a href="http://passwordstorage123" class="external-link">password:storage123</a></span>
    -   [Remote access - ECHO MacMini Access](https://confluence.eng.pivotal.io/display/ECHO/ECHO+MacMini+Access) - <vnc://137.69.6.227>
-   <span style="color: rgb(0,51,102);">What is Storage Scrum?</span> Everything which the storage team is working on.
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Yellow, UAO - Green
    -   Issues/Risks
        -   WAL Rep: filerep\_end\_to\_end & Crash recovery test all the tests are failing on check master mirror integrity - Passes on Pulse & Fails on Pulse-Storage
        -   UAO - Need to investigate why check mirror integrity is hanging on Pulse
        -   Issues with suse build box - delayed dev for UAO - costed a day
    -   Dependencies
        -   DCA scripts might fail with the new master mirror implimentation
        -   Saurin will be triggering the UAO Performance tests

Sep 9th
=======

**Retrospective**

**Triage Pending**

-    3 (Hitoshi), 1 (Raghav)

**Storage Scrum Bugs**

-   5 (Dirk), 1 (Hitoshi)

**Resolved bugs for verification**

-   8 (Divya, Savita & Jason check on Doc bug Verification)

**Escalation**

-   WalMart - <a href="http://jira.eng.pivotal.io/browse/MPP-21134" class="external-link">MPP-21134</a> - XFS filesystem got corrupted

**Completed Task from Aug 12th**

WAL Rep
-   <span style="color: rgb(0,0,0);">Continue with STO-493: Testing Wal-R: Negative Manual + Automation - (management utilities) Part 1</span>
-   <span style="color: rgb(0,0,0);">Fix the tests - Make it Green - Spill from Sprint 11 - instrument Build</span>
-   <span style="color: rgb(0,0,0);">WalRepl Test: analyze & Fix Regression schedules: multinode resync schedules</span>
-   <span style="color: rgb(0,51,102);">Assertion failure "!(!caughtup || caughtup\_within\_range)"</span>
    -   <span style="color: rgb(0,0,0);"><span style="color: rgb(0,51,102);">"ERROR","XX000","could not connect to the primary server: FATAL:  number of requested standby connections exceeds max\_wal\_senders (currently 1) (gp\_libpqwalreceiver.c:79)" - STO-595</span>
        </span>
-   <span style="color: rgb(0,0,0);">Modify tests to stop/kill the master first before activating the standby</span>
-   <span style="color: rgb(0,0,0);">Merge code from ashwin's private to walrep main</span>
-   <span style="color: rgb(0,0,0);">Test gpmodcontrol</span>
-   <span style="color: rgb(0,0,0);">Testing Wal-R: Upgrade Testing</span>
-   <span style="color: rgb(0,0,0);">Walrepl regression schedule: GPDB-WalRepl-crash-recov-31-42, segment was not able to come up during fault injection</span>
-   <span style="color: rgb(0,0,0);">Complete all the TODO mentioned in the code</span>
-   <span style="color: rgb(0,0,0);">Review/add logging for walreplication</span>
-   <span style="color: rgb(0,0,0);">WAL Based SR (Master Mirroring) - Code Reviews</span>
-   <span style="color: rgb(0,51,102);">After promote, OLD master should not be allowed to act as MASTER</span>
-   <span style="color: rgb(0,51,102);">Send Alert in walreceiver</span>
-   <span style="color: rgb(0,51,102);">Merge WALREP Private Branch into PARISTX-WALREP</span><span style="color: rgb(255,0,0);">
    </span>

UAO
<span style="color: rgb(0,0,0);">Merge UAO Private Branch into PARISTX-UAO</span>
<span style="color: rgb(0,0,0);">UAO: Performance test for UAO</span>
<span style="color: rgb(0,0,0);">UAO: Merge the cdb2 test cases for UAO into TINC REPO</span>
<span style="color: rgb(0,0,0);">UAO Test: Code Coverage</span>
<span style="color: rgb(0,0,0);">Change compaction threshold message to LOG instead of NOTICE</span>
<span style="color: rgb(0,0,0);">gprecoverseg failure
</span>
<span style="color: rgb(0,0,0);">UAO: Test Persistent Table changes for UAO</span>
<span style="color: rgb(0,0,0);">UAO Test: Create modified sql files for QP and MVCC and run against AOCS build</span>
<span style="color: rgb(0,0,0);">UAO: Test fix for Vacuum from utility mode crashes the connection</span>
<span style="color: rgb(0,0,0);">UAO: Test the fix for Vacuum and Vacuum full does not vacuum the pg\_aoseg.pg\_aoseg catalog table</span>
<span style="color: rgb(0,0,0);">UAO Test: Create Huge Data set</span>
-   <span style="color: rgb(0,0,0);">UAO: Test Huge Data Query Plan</span>
-   <span style="color: rgb(0,0,0);">UAO: Test Huge Data Compaction</span>
-   <span style="color: rgb(0,0,0);">UAO: Test Huge data Delete/Update for its impact on visimap</span>
-   <span style="color: rgb(0,0,0);">UAO: Test Huge Data Sort</span>

<span style="color: rgb(0,0,0);">UAO: Test: Run All Backup & Restore Schedules (DDAuto, Behave, other TINC etc)</span>
<span style="color: rgb(0,0,0);">UAO: Analysis of Regression schedules</span>
<span style="color: rgb(0,0,0);">UAO Incremental Backup: Fix OpenBAR solaris issues</span>
<span style="color: rgb(0,0,0);">UAO Test: UDFs for Visimap</span>
<span style="color: rgb(0,0,0);">CDBFAST TO TINC Migration - Test suite</span>
-   <span style="color: rgb(0,0,0);">Create Filerep\_end\_to\_end Scenario Framework in TINC & add UAO testcases</span>

**Demo**

-   Divya - Incremental Backup for UAO - 5 - 10 mins
-   Regression & Code Coverage - UAO - Savita
-   Regression & Code Coverage - Wal Rep - Jason
-   Upgrade content for Wal Rep & UAO  - (Next week)

**This Week Update**

-   Pulse-Storage ENV will be available starting from Sep 9th
-   RHEL 6.4 Certifictaion in progress
-   Feature Branches for WAL REP & UAO
-   In the process of making Multinode schedules green - WAL REP
-   4.2.6.3 :
-   Review all JIRAs targeted for 4.2.6.3 and close on content and date
-   TV Monitor
    -   Login Credentials - Username - "storage" & <span style="color: rgb(0,0,0);">password:G0Piv0tal!!</span>
    -   [Remote access - ECHO MacMini Access](https://confluence.eng.pivotal.io/display/ECHO/ECHO+MacMini+Access) - <vnc://137.69.6.195>
-   Central QA - <span>Story for turning PT Regression Green</span>
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Yellow
    -   Issues/Risks
    -   Dependencies

Aug 26th
========

**Retrospective**

-   <span style="color: rgb(0,128,0);">Team is AWESOME !!!!</span>
-   <span style="color: rgb(255,102,0);">Bugs have not been prioritized</span>
-   <span style="color: rgb(255,102,0);">No story for refering to an Hotfix, No story for tracking ORCA -POC regression results monitoring</span>
-   <span style="color: rgb(255,102,0);">Please keep me (Scrum Master) in loop for any unplanned activity which comes in the way of a sprint</span>
-   <span style="color: rgb(255,102,0);">How to estimate progress</span>
-   <span style="color: rgb(255,102,0);">Communication can be improved</span>
-   <span style="color: rgb(255,0,0);">One pending issue not been RCA'd yet.</span>
-   <span style="color: rgb(255,0,0);">Meetings are time consuming</span>
-   <span style="color: rgb(255,0,0);">Better estimation still needed - 1/2 a day</span>
-   <span style="color: rgb(255,0,0);">Improved doc/knowledge base</span>
-   <span style="color: rgb(255,0,0);">speed of pulse - counter productive, not able to find agents, no robust test cases <span>(\*\*\*)</span></span>
-   <span style="color: rgb(0,128,0);">Dev & QA collaboration to RCA the failing regression schedules in WAL REP</span>
-   <span style="color: rgb(0,128,0);">Good estimatation on pointing & completing the stories</span>
-   <span style="color: rgb(0,128,0);">Estimated 4 weeks in advance <img src="s/en_GB-1988229788/4224/875a2723d3ca93f267db5c35cdbfcf91f5910d2d.36/_/images/icons/emoticons/smile.png" alt="(smile)" class="emoticon emoticon-smile" /></span>
-   <span style="color: rgb(0,128,0);">Good Demo's - full participation</span><span style="color: rgb(0,128,0);">
    </span>

**Triage Pending**

-    1 (Hitoshi)

**RCA Pending**

-   10 (None)

**Resolved bugs for verification**

-   8 (Divya, Savita & Jason check on Doc bug Verification)

**Escalation**

-   None

**Completed Task from Aug 12th**

-   WAL Rep
    -   Basebackup Cleanup
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
    -   <span style="color: rgb(0,0,0);">WalR Testing: gpexpand : Standby failed to start with  "FATAL","XX000","Unable to find entry for tablespace OID = 16994 when getting filespace directory paths (cdbpersistenttablespace.c:614)</span>
    -   <span style="color: rgb(0,51,102);">Testing Wal-R: Negative Manual + Automation - (management utilities) Part 1</span>
-   UAO
    -   Check cursor support in combination with update/deletes and vacuum.
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
-   IVECO - HOTFIX - 4.2.6.2IV1 - Verify MPP-20964
-   Mentoring Nikhil with Tinc Test(Manual run, Pulse configuration and failure analyzing).
-   Analyzed MAIN regression results for ORCA-POC merge

**Demo**

-   No Demo**. **Since we will be doing Thursday demo for UAO

**This Week Update**

-   <span class="hP">4 sprints to go - will WAL Rep stories are placed in respective sprints.</span>
-   <span class="hP">UAO stories to be completed today - Anu to meet with Dirk & Hitoshi</span>
-   TV Monitor
    -   Login Credentials - <span>Username - "storage" & </span><span style="color: rgb(0,0,0);">password:G0Piv0tal!!</span>
    -   [Remote access - ECHO MacMini Access](https://confluence.eng.pivotal.io/display/ECHO/ECHO+MacMini+Access) - <vnc://137.69.6.195>
-   4.2.6.2IV1(MPP-20964) are we targeting for 4.2.6.3? - update the target version<span class="hP">
    </span>
-   4.2.6.3 
-   Central QA to own stress & longevity also
-   Story for turning PT Regression Green
-   Branch created for UAO & WAL REP - need to do the private branch merge
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; WAL REP - Green, UAO - Yellow
    -   Issues/Risks
        -   WAL REP & UAO: Schedules not Green yet
        -   Tests will be added to CDBFAST ( filerep\_end\_to\_end & Crash Recovery)
    -   Dependencies
        -   No

<span>Aug 19th</span>
=====================

**Triage Pending**

-    1 (Hitoshi)

**RCA Pending**

-   10 (None)

**Resolved bugs for verification**

-   8 (Divya, Savita & Jason check on Doc bug Verification)

**Escalation**

-   None

**Completed Task from Aug 12th**

WAL Rep
-   <span style="color: rgb(0,0,0);">Promote may miss some records to replay?</span>
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
-   <span style="color: rgb(0,51,102);">Remove the toasted values of deleted tuples in UAO</span>
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
**Demo**

-   No Demo** **

**Issues of UAO:**

-   Row
    -   Upgrade
    -   Backup Coding is done - No testing done
    -   Performance testing is blocked by hardware
    -   Warning on Index during Vacuum
-   Column

**This Week Update**

-   <span class="hP">UAO vs Serializable transaction - Test Case will be added - New stories will be added in backlog</span>

-   Rhel 6.4 certification - due end of Sep
-   UAO (Row & Column) in Paris TX
-   Moving from Green Hopper to Pivotal Tracker
-   Branch created for UAO & WAL REP as soon as code complete
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt;
    -   Issues/Risks
        -   WAL REP & UAO: Schedules not Green yet
        -   <span style="color: rgb(255,0,0);">Performance Test for AO  - Sprint 10 - blocked on Hardware</span>
        -   Serializable tranasaction - disable Updates & Deletes on UAO
    -   Dependencies
        -   UAO: needs collaboration with QP, Backup(ACE)s

Aug 12th
========

**Triage Pending**

-    1

**RCA Pending**

-   10 (None)

**Resolved bugs for verification**

-   3 (Divya & Jason check on Doc bug Verification)

**Escalation**

-   MPP-20859 - Finra - DU - Known workaround

**Completed Task from Aug 5th**

-   <span>WAL Rep</span>
    -   <span>Test Plan Review with StakeHolders</span>
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
-   <span>UAO</span>
    -   <span>Support incremental backup</span>
    -   UAO: Regression schedule

    -   Add descriptionof pg\_class statistics to UAO design document

    -   Change policy of select insertion segment file

    -   Increase append-only version number

    -   Fix visimap relkind after Upgrade

    -   Updates & Deletion in AO Column Tables

    -   support compaction with parallel inserts

    -   UAO: All Possible Alter+Update+Delete & Update+Alter+Delete

-   <span>Weekly Regression schedules</span>
    -   <span>4.2x StorageDev Regression </span>
    -   <span>Weekly Regression schedule - 42x-Storage - Sprint 8</span>
    -   <span>Weekly Regression schedule - WAL Replication Schedule</span>
-   <span>SPIKE: learn pulse like create, clone, configure projects</span>
-   <span>MPP-20488: PHP program using libpq for database connections results in database panics and restarts.</span>
-   <span>\[TRIAGE\] MPP-20859 FINRA : Persistent Table Free TID issue</span>
-   <span>cdbfast to TINC migration</span>
    -   <span>GpVerify.py - Write Unit tests</span>
    -   <span>Incorporate the code-review comments for Gpstart.py</span>
-   <span>Training/mentoring Sessions</span>
    -   Mentor Nikhil for regression schedules

**Demo**

-   UAO - Test - Savita
-   Hitoshi - TBD

**This Week Update**

-   Rhel 6.4 certification - due end of Sep
-   Moving from Green Hopper to Pivotal Tracker
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; Green
    -   Issues/Risks
        -   WAL REP & UAO: Schedules not Green yet
    -   Dependencies
        -   UAO: needs collaboration with QP, Backup(ACE)

Aug 5th
=======

**Triage Pending**

-    0

**RCA Pending**

-   9 (None)

**Resolved bugs for verification**

-   Need to identify the QA owner for 4.2.6.2 bug - Dev to be done by EOD tomorrow**
    **

**Escalation**

-   None

**Completed Task from Jul 29th**

WAL Rep
-   Disallow different filespace for pg\_xlog with master standby
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
-   Write UAO design document in official template
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
-   4.2x StorageDev Regression - Week 4
-   Weekly Regression schedule - 42x-Storage - Sprint 7
-   Weekly Regression schedule - WAL Replication Schedule

Training/mentoring Sessions
-   Mentor Nikhil for gpdb installation on std configurations & Transition States
-   Mentor Nikhil in QA-2560 Install P4 in VCloud VM & Run cdbfast tests

**Demo**

-   <span style="color: rgb(34,34,34);">Fix for local temp tables with new WAL Rep features</span>
-   <span style="color: rgb(34,34,34);">DDL & DML Test Cases</span>

**This Week Update**

-   4.2.6.2 Dates
-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green -&gt; Yellow
    -   Issues/Risks
        -   WAl Rep Regression schedule
    -   Dependencies
        -   UAO: needs collaboration with QP, Backup(ACE)

Jul 29th
========

**Triage Pending**

-    None

**RCA Pending**

-   MPP-19353 needs investigation**
    **

**Resolved bugs for verification**

-   4.2.6.1 items to be verified in 4.2.x.

**Escalation**

-   None

**Completed Task from Jul 22 th**

WAL Rep
-   gpinitsystem with -s to a new host is failing
-   cdbfast <a href="http://regressionschedule" class="external-link">regression:schedule</a> Transaction, aborting startup, "PANIC","XX000","Unexpected internal error: Segment process received signal SIGSEGV"
-   Rebase on latest main
-   Unpack checkpoint should be able to handle multiple checkpoints
-   Original Master (now dead) should not act as the new standby
-   Testing Wal-R: Positive Manual + Automation -management utilities(gpinitstandby)
-   segment crash recovery failed in pass 3 after transaction\_abort\_after\_distributed\_prepared suspend
-   Start regressions for wal replication tincrepo tests in pulse
-   SPIKE: wal\_level
-   Verify utility mode of primary master can replicated
-   SPIKE: MinRecoveryPoint
-   Promoted standby should not accept queries until role is changed
-   WAL sender authorization

Update-able AO
-   Analyzing the failures in 4x basic schedules
-   Test and Verify the Fix for Error creating append-only table with (appendonly=true, compresstype = zlib, compresslevel=3 )
-   Test the fix for Insert into appendonly table fails after Alter table set with ( reorganize='true') distributed randomly
-   Insert into appendonly table fails after Alter table set with ( reorganize='true') distributed randomly
-   Fix installcheck-good issues in UAO
-   Mem leak in Update Tuple in Utility mode. Verify the fix
-   Create elogif macro
-   Update all error messages for AO column orientation
-   erify that UPDATE/DELETE operations on AOCO tables are still rejected
-   Updatable cursor should be rejected
-   Introduce GUC to avoid UAO compaction if segment file more than x% visible
-   Update statistics for UAO tables

Weekly
-   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev
-   Investigating into the regression failures of Wal-Rel-build
-   Triage: cdbfast: checkmastermirrorintegrity: Redo encountered unexpected error
-   Triage: MPP-20707 Inserting into multi-level partitioned tables triggers high I/O wait on segment
-   MPP-20705 \[Triage\] ChinaUnicom :- set max\_prepared\_transactions &gt; 1000 causes it to set 50
-   Verify MPP-18131 in GB Build 4.2.6.1

4.1.3.0A (Hotfix)
-   Hotfix 4.1.3.0A for Samsung Life Insurance(MPP-13559)

cdbfast to TINC Lib migration
-   GpVerify.py - Migrate the modules

**Demo**

-   Not this week

Jul 22nd
========

**Triage Pending**

-    2 (Needs to be done this week)

**RCA Pending**

-   9 to be revisited**
    **

**Resolved bugs for verification**

-   Bugs to be verified on 4.2.6.1 & 4.2.x

**Escalation**

-   Genscape (4.2.3.4) - dup
-   Franklin - Slow

**Completed Task from Jul 22 th**

WAL Rep
-   Support gpinitsystem -s option
-   Make WAL based replication True synchronous
-   Add WAL Recv details - Design Doc - initial 
-   Timeline ID, Pass 3 issues
-   Testing Wal-R: Rearrange existing git/tincrepo to test and lib 
-   gpactivatestandby
-   Update design document for gpinitstandby, gpactivatestandby
-   Testing Wal-R: Define the DDL and DML statements in Testrail
-   Implement gpinitsystem -s
-   SPIKE - Standby Promotion Before Catchup
-   Wal-R: cdbfast regression list\_storage: Gpcheckcat failed with Unexpected Internal Error
-   Pulse Projects for regression schedules with standby
-   List up detailed test cases for management scripts

Investigating into the GetSnapshotData( ) Function
Reproduce MPP-20276
Update-able AO
-   Create UAO visimap UDF to allow fine-grained testing and support
-   UAO compaction  and UDF testing
-   UAO testcase automation - Initial
-   Allow index vacuum without a compaction
-   Visibility map bitmap compression
-   UAO: Not able to create UDF
-   UAO should recognize gp\_select\_invisible
-   UAO Bug Fx: Vacuum Full fails with error 'could not temporarily connect to one or more segments'
-   UAO Bug Fix: aovisimap appends wrong  tuple after a table is updated
-   create UAO preliminary tests in testrail 

Weekly
-   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev
-   Investigating into the regression failures of Wal-Rel-build

4.2.6.1
-   Trigger Stress & Longevity on 4.2.6.1
-   Monitoring and investigating into failures of Regression schedule for 4.2.6.1

cdbfast to TINC Lib migration
-   Gpstop.py
-   Gpstart.py
-   Config.py
-   Gpfilespace.py

**
**

**Demo**

-   Wal Rep demo (Hitoshi/Raghav)

**This Week Update**

-   Status to be included in MPP Program Team meeting
    -   Overall Status - Red/Yellow/Green
    -   Issues/Risks
    -   Dependencies
-   Labels (42x-Stoarge-Dev -&gt; 42x-Storage)

Jul 15th
========

**Triage Pending**

-    0

**RCA Pending**

-   9 (None)

**Resolved bugs for verification**

-   Revisit Savita's Bugs - To be Verified

**Escalation**

-   Franklin 

**Completed Task from Jul 8 st**

Investigate Architecture of MoreVRP Agent Project
Triage of MPP-20610, MPP-20601
Worked with Escalation for Franklin - System is slow
RCA for MPP-20276
Longevity on 42x-Storage - Triggering
Stress testing on DCA against 42x Metro Branch
UAO
-   Separate UAO visimap from block directory
-   Fixed "Unable to select from table after add column on table"
-   Make index-less AO tables up-dateable
-   Compaction of AO row-orientation
-   UAO - Installation & Test cases of the UAO prototype

WAL Rep
WAL Rep: createdb failed with Masterstandby running.
keep\_wal\_segments
Change the way walreceiver dies
pg\_controldata should report updated fields
Testrail initial test plan
-   Positive & Negative Integration Tests
-   Positive & Negative Management Utilities Tests
-   Stress/Longevity/Load/Performance

gpinitstandby
BFV
-   BFV(42x-Storage) - MPP-19890, MPP-19038

Weekly
-   Monitoring Weekly Regression Test 4.2.x Storage & 4.2.x Storage Dev

Training
-   Investigating into Google Unit Test Framework
-   WAL Rep: Going through Postgres streaming Rep

**Demo**

-   <span>UAO - Dirk & Savita</span>

**This Week Update**

-   Central QA
-   cdbfast to TINC Migration - QA Priority
-   QA - Escalation - rotation
-   4.2.6.0 Retrospective
-   Escape Analysis
    -   Check on all the storage Bugs
-   GP Cloud
    -   Tech ops - Blocker for GPCloud

Jul 8th
=======

**Triage Pending**

-    0

**RCA Pending**

-    8 (None for this sprint)

**Resolved bugs for verification**

-    Anu to reassign bugs for Verification
-   To make the resolution to Status

**Escalation**

-    None

**Completed Task from Jul 1 st**

-   Investigate Building MoreVRP Services in Visual Studio 2010
-   Triage of MPP-20507
-   Create Pulse projects to run against 42x-storage
-   Create 42x-Storage Build Project
-   **WAL Rep:**
    -   pg\_basebackup -R
    -   Integrate latest //tinc/main and tincrepo/main to walrepl/tincrepo
    -   Tryout the new standby start & promote
    -   Handle smart shutdown during recovery
    -   Modify gpstart to adopt new replication
    -   SPIKE: does walsender need to connect database?
    -   STP: Software Test Plan
        -   WAL Rep - Stress & load testing
        -   Component Testing
        -   Integration Testing
        -   Sessions not mentioned in other tasks
-   **BFV**
    -   BFV(42x-Storage) - MPP-20264, MPP-20203, MPP-20426
-   **Weekly**:
    -   Monitoring Weekly Regression Test 4.2.x Storage
-   **Training:**
    -   Going Through Stress test
    -   Understanding GIT version system
    -   Understanding GPDB unit testing
    -   Setup TINC & gain familiarity on required Components/Classes to write unit test

**Demo**

-   cdbfast to TINC migration: gpstart & gprecoverseg
-   Show Pulse projects for  42x-Storage & 42x-StorageDev

**This Week Update**

-   Check Back on the 4.2.6.1 patches GA 

**Team:**

-   **Raghav**
    -   <a href="http://jira.eng.pivotal.io/browse/STO-53" class="external-link" title="STO-53">STO-53</a><span> </span><span>Change the way walreceiver dies </span><span class="aui-label" title="WAL Replication - MM">WAL Replication</span>
    -   <a href="http://jira.eng.pivotal.io/browse/STO-61" class="external-link" title="STO-61">STO-61</a> <span>pg\_controldata should report updated fields </span><span class="aui-label" title="WAL Replication - MM">WAL Replication</span>

        <a href="http://jira.eng.pivotal.io/browse/STO-76" class="external-link" title="STO-76">STO-76</a><span> </span><span>MPP-20276 :- Crash recovery after -11 Signal failed \[RCA\]</span>

-   **Hitoshi**
    -   **
        **STO-139: gpinitstandby
    -   STO-145: WAL Rep: createdb failed with Masterstandby running.
-   **Jason**
    -   <span style="color: rgb(34,34,34);">STO-131: <span>WAL Rep: Going through Postgres streaming Rep</span></span>
    -   <span style="color: rgb(34,34,34);"><span><span>STO-167: <span>Monitoring Weekly Regression Test 4.2x Storage</span></span></span></span>
    -   <span style="color: rgb(34,34,34);"><span><span><span><span>STO-166: <span>Wal-repl: Manual Testing on feature branch</span></span></span></span></span></span>
    -   <span style="color: rgb(34,34,34);"><span>STO-106: <span>Create Unit Test Case for Verification of MPP-19259</span></span></span>
    -   <span style="color: rgb(34,34,34);"><span><span>STO-167: <span>Testplan-Testrail: Positive Integration Tests (management utilities)</span></span></span></span>
    -   <span style="color: rgb(34,34,34);"><span><span><span>STO-160: <span>Testplan-Testrail: Negative Integration Tests (management utilities)</span></span></span></span></span>

Jul 1st
=======

**Triage Pending**

-    1

**RCA Pending**

-    8

**Resolved bugs for verification**

-    7

**Escalation**

-    None

**Completed Task from Jun 10th**

-   Triage MPP-20460/ADP core
-   Triage MPP-20408/FINRA insert
-   Verify Duplicate bugs on ParisTX
-   Additional issues encountered while verifying CR fixes(MPP-20507)
-   Main regression test analysis
-   Automated MPP-19038
-   Update-able AO Design Document
-   MPP-20447 \[RCA & Fix Design\] 
-   Bug Fix Verification of MPP-19973
-   Sync from 4.2.x main to 42x-Storage & 42x-Storage-Dev
-   Persistent Table Presentation
-   MPP-19960 - Fixed & unit Test written
-   Bug Fix Verification in 42x-Storage-Dev (MPP-20426 & MPP-20264)
-   Understanding GIT Version system
-   Getting familiar with regression schedules (New Hires)
-   Bugs to be merged from 42x-Storage-Dev to 42x-Storage
-   WAL
    -   Walsender needs to die after bgwriter
    -   Support Multiple filespaces with pg\_basebackup
    -   Manual verification of MM consistency at transaction Commit/Abort (Prepared)
-   4.2.6 RC: 
    -   Stress & Longevity Monitored

**Demo**

-   MPP-19973 (Jason) Bug Automation
-   TBD

**This Week Update**

-    

**Team:**

-   **Raghav**
    -   On Vacation
-   **Hitoshi**
    -    <a href="http://jira.eng.pivotal.io/browse/STO-47" class="external-link">STO-47</a> Handle smart shutdown in crash recovery
    -   <a href="http://jira.eng.pivotal.io/browse/STO-81" class="external-link">STO-81</a> SPIKE: does walsender need to connect database?
    -   <a href="http://jira.eng.pivotal.io/browse/STO-82" class="external-link">STO-82</a> Adopt gpstart to new replication code<span> </span>
-   **Jason**
    -   Stress Testing
    -   Verify as many bugs as possible
    -   <span style="color: rgb(34,34,34);">Software Test Plan Template - </span>WAL Rep
    -   Unit testing
-   **Savita**
    -    <a href="http://jira.eng.pivotal.io/browse/STO-112" class="external-link">STO-112</a> Migrate gpstart.py from cdbfast to Tinc framework
    -   4.2x Storage-Dev branch - Regression schedule
-   **Ashwin**
    -    STO-94 unit test and commit MPP-19960
    -   STO-51 Prepared transactions should be WAL replicated - phase -1
-   **Divya**
    -   STO-133 : Pulse project for 42x-storage
    -   STO-134: <span>Create Pulse projects to run against 42x-storage</span>
    -   STO-90- High level STP

Jun 24th
========

**Triage Pending**

-    MPP-20464 (VarBlock is not valid) (assigned to DM for initial investigation)
-   <a href="http://jira.eng.pivotal.io/browse/MPP-20460" class="external-link">MPP-20460</a>
-   <a href="http://jira.eng.pivotal.io/browse/MPP-20408" class="external-link">MPP-20408</a>
-   <a href="http://jira.eng.pivotal.io/browse/MPP-20276" class="external-link">MPP-20276</a>

**RCA Pending**

-   MPP-20258 (Assigned to AshwinA) 
-   MPP-20276

**Resolved bugs for verification**

-    

**Escalation**

-    <a href="http://jira.eng.pivotal.io/browse/STO-60" class="external-link">STO-60</a> RAfter - pg\_attribute heap page header corrupted 

**Completed Task from Jun 10th**

-   <a href="http://jira.eng.pivotal.io/browse/STO-24" class="external-link">STO-24</a> Design doc for WAL Replication 1
-   <a href="http://jira.eng.pivotal.io/browse/STO-49" class="external-link">STO-49</a> pg\_stat\_replication
-   <a href="http://jira.eng.pivotal.io/browse/STO-77" class="external-link">STO-77</a> replay\_location is not replied
-   <a href="http://jira.eng.pivotal.io/browse/STO-74" class="external-link">STO-74</a> MPP-20222 Prosphere (No root cause found)
-   <a href="http://jira.eng.pivotal.io/browse/STO-32" class="external-link">STO-32</a> MPP-20447 gp\_checkcat inconsistencies (RCA complete)
-   <a href="http://jira.eng.pivotal.io/browse/STO-76" class="external-link">STO-76</a> MPP-20276 Crash Rec : - 11 Sig, System Unavailable (Triage Complete)
-   STO-27 Duplicate BFV on 42x
-   4.2.6 RC regression analysis & command center

**Demo**

-    

**This Week Update**

-   RHEL 6.3,RHEL 6.4 & SUSE SP2 OS Certification
-   Affected Version "Current on going Release" + Scrum Branch + last identified release
-   Central QA update

**Team:**

-   **Raghav**
    -   <a href="http://jira.eng.pivotal.io/browse/STO-48" class="external-link">STO-48</a> Wal Sender needs to die after bgwriter (Changes ready, Need to push)
    -   <a href="http://jira.eng.pivotal.io/browse/STO-70" class="external-link">STO-70</a> Persistent Tables Presentation
    -   <a href="http://jira.eng.pivotal.io/browse/STO-32" class="external-link">STO-32</a>, <a href="http://jira.eng.pivotal.io/browse/STO-76" class="external-link">STO-76</a>
    -   Out of office from 1st till 5th July.
-   **Hitoshi**
    -    <a href="http://jira.eng.pivotal.io/browse/STO-50" class="external-link">STO-50</a> Support multiple filespaces in pg\_basebackup
    -   Triage <a href="http://jira.eng.pivotal.io/browse/MPP-20408" class="external-link">MPP-20408</a>
    -   Triage <a href="http://jira.eng.pivotal.io/browse/MPP-20460" class="external-link">MPP-20460</a>
-   **Divya**
    -   WAL Replication - Spike and STP
    -   BFV (pending from last week) - Duplicates on ParisTx
    -   Reproduce <span style="color: rgb(80,80,80);"> </span><a href="http://jira.eng.pivotal.io/browse/MPP-20371" class="external-link">MPP-20371</a> (crash\_recovery OOD issue)
    -   Complete 4.2.6 stress & longevity
-   **Jason**
    -   Work on Gnet regression(Secure hadoop, Cloudera Hadoop)
    -   Bug verification of MPP-19259
    -   Continue studying feature testing of WAL replication
-   **Savita**
    -   On Vacation 
-   **Ashwin**
    -    Build insight into WAL replication through code, design and other postgres documents
    -    Looking into transfered JIRAs MPP-19469 and MPP-19913
    -   Testing the fix after merging the changes for MPP-19960 to STORAGE-DEV will commit this week
-   **Anu**
    -   Finalize on the Performance Degradation - MPP-20465
    -   Manage & Lead 4.2.6 - Wrapup 4.2.6 & Generate Test Report 
-   **Dirk**
    -   Working on MPP-20464 and UAO
    -   Out of Office beginning Wednesday

Jun 17th
========

**Triage Pending**

-    MPP-20290: CR/gprecoverseg (HH)

**RCA Pending**

-    

**Resolved bugs for verification**

-   MPP-20426: Longevity/ExtendSUBTRANS - with unit test.  No need for QA test. (HH)

**Escalation**

-    NIL

**Completed Task from Jun 10th**

-    MPP-19973 LWLock already held (shmem slot): submitted for Code Review
-   4.2.6 Pre-RC tests runs- manual regressions. stress and longevity
-   4.2.5.2C - Done

<!-- -->

-   MPP-20415/20416/20417: Crash Recovery tests (HH), some to Jason
-   MPP-20231: gpstart couldn't start segment (HH)

<!-- -->

-   Sync WAL replication merged into the Dev private branch (RT)
-   MPP-20258 (Huawei) issue - Analyzed & assigned to EE.
-   No More Triage Pending issues(RT)
-   Dirk Ramp Up, Reviews (RT) 

**Demo**

-    MPP-19973(DM), MPP-20426(HH)

**This Week Update**

-   <span style="color: rgb(255,0,0);">(SC) - Blocked on Hari's reply on TINC Framework (Anu can you please look into this ?)</span>
-   <span style="color: rgb(255,0,0);">(DM) - Blocked on bad performance on Physical machines (Dvya to work with him)</span>
-   <span style="color: rgb(255,0,0);">(HH) - Wants to understand 4.2.6 status (Anu can you please updates ?)</span>

**Team:**

-   **Raghav**
    -    WAL Replication
    -    MPP-20258, MPP-20222
-   **Hitoshi**
    -    WAL Replication: Design document ph1 / walsender termination
    -   Finalize MPP-20426
-   **Divya**
    -   Go thrugh BFV pending bugs and verify as much as possible
    -   Rerun/ modify CR tests based on outcome of few bug analysis
    -   4.2.6 RC stress & Longevity
-   **Jason**
    -   Continue the manual regression test of Core Management Utilities
    -   Bug verification of MPP-19259
    -   Reproduce MPP-20276
    -   Continue monitoring 4.2.6 Pre-RC tests (Gnet)
-   **Savita**
    -   Completed studying the existing cdbfast test-suits ear-marked for migration.
    -   This week, Start working on Cdbfast migration plan
    -   If get the go ahead from Hari and Ravi, start coding some libraries in Tinc-framework
    -     
-   **Ashwin**
    -   Started working on WAL replication project (getting insights into the stuff right now)
    -   MPP-18858 (FINRA) is on hold from Software FTS side, its decided to handle and resolve the stuff first from hardware perspective
-   **Anu**
    -   Work with QP to get the last regression bug fix into 4.2.x
    -   Finalize on the Performance Testing numbers
    -   Manage & Lead 4.2.6 - Wrapup 4.2.6 - Cut RC 
    -   Create State of Product Report for 4.2.6
    -   Create QA Dashboard for 4.2.6
-   **Dirk**
    -   Working on Performance Characteristic of AO Tables
    -   MPP-19973 LWLock already held (shmem slot): creates patch and test automatization.
    -   Started working on Update-able AO tables design and prototype

Jun 10th
========

**Triage Pending**

-   7 bugs to be triaged 

**RCA Pending**

-   10 bugs on RCA\_Pending 

**Resolved bugs for verification**

-   None

**Escalation**

-   Master Mirroring Memory Leak issues -  Hotfix for Samsung - In Progress

**Completed Task from Jun 3rd**

-   Prosphere - 20222 - recovery failed - Analysis is done, RCA is pending 
-   MPP-20268 SWITCHFLY COPY error handling: triage + RCA
-   MPP-20264 DTCC Master Mirroring failure: triage + RCA
-   MPP-19973 LWLock already held (shmem slot): triage + RCA
-   MPP-20231 gpstart fail: RCA pending 
-   MPP-20203 MM memory leak: hot fix check in feature branch
-   WAL Replication: filespace support w/o testing
-   Pulse Project for Crash Recovery created
-   Enhancement to Backup & Restore feature - MPP-20219 - submitted for Code Review
-   MPP-19960  worked on fixing the issues & addressing the same - Submitted Code Review

**Demo**

-   Sync Replication - Demo (Raghav)
-   Filespaces in WAL - Demo (Hitoshi) 

**This Week Update**

-   QA Offsite Thursday & Friday

**Team:**

-   **Raghav**
    -   WAL replication tobe sync into the Dev private branch
    -   Raghav to work on 20258 (Huawei) issue.
    -   Look into Triage Pending bugs
    -   Dirk Ramp Up
    -   Today : 42x sync to 42x-Storage branch
    -   To initiate conv on the Storage Branch (Private) 
-   **Hitoshi**
    -    MPP-20290 CR issue: to triage
    -    MPP-19973 LWLock already held (shmem slot): make fix
    -    MPP-20231 gpstart fail: look into it
    -    WAL Replication: filespace testing / demo on Wednesday
-   **Divya**
    -   Working on 4.2.6 Pre-RC tests runs
    -   Triggered Stress & Longevity
    -   Master Mirroring Memory Leak issues -  Hotfix for Samsung
    -   Manual Automation tests for 4.2.6
-   **Jason**
    -   4.2.6 Pre-RC tests (Gnet)
    -   Manual Automation tests for 4.2.6
-   **Savita**
    -   In India working on Strategy of cdbfast to TINC  Migration
-   **Asim**
    -   20218 - spin lock(Asim)
    -   19469 - Equifax - recoverseg (Asim)
-   **Ashwin**
    -   20190 - FINRA (Ashwin) - working on (FTS) - duplicate of 18858
    -   MPP-19960 will work on Code Review comments
-   **Anu**
    -   4.2.6 - Manage & Lead
    -   4.2.6.1 need to work on Yelena
-   **Dirk**
    -   Ramping Up with Raghav
    -   Working on Performance Characteristic of AO Tables

Jun 3rd
=======

**Triage Pending**

-    8 bugs pending triage

**RCA Pending**

-    7 bugs RCA\_pending

**Resolved bugs for verification**

-    Anu to look into the bugs pending verification and schedule for the following week for verificatio

**Escalation**

-   18993 - NSN - actively working (Raghav)
-   20222 - Prosphere - recovery failed - persistent table EOF file issue (Raghav)
-   20258 - Huawai (Not yet decided) (Raghav/Hitoshi)
-   20264 - Standby master failed with SIGSEGV
-   20190 - FINRA (Ashwin) - done triage pending moving into RCA (FTS)
-   20218 - spin lock(Asim)
-   19469 - Equifax - recoverseg (Asim)

**Completed Task from May 20th**

-   Triggered Stress Test on 4.2.x
-   All the regression tests are done on the latest 4.2.x storage branch
-   sync up from the latest 4.2.x main into storage on Friday
-   Crash Recovery - Master process automated & tested
-   Outlined a plan for Migration from cdbfast to TINC
-   Raghav helped escalation team to extract information from customer
-   Sync nature of WAL replication - Spike

**Demo**

-   TBD

**This Week Update**

-   Sync from 4.2.x into Storage branch on weekly basis
-   Savita will be working from India starting Jun 5th to Jun 26th 
-   New Dev (Dirk) joining the team on Jun 5th - in San mateo for 2-3 weeks

**Team:**

-   **Raghav**
    -   Prosphere - 20222 - recovery failed - persistent table EOF file issue (Raghav)
    -   WAL replication tobe sync into the DEv private branch
    -   Raghav to work with Ashwin 20258 (Huawei) issue.
    -   Look into New escalation
-   **Hitoshi**
    -   Take Triage pending and escalation assigned to George Tuma
    -   Filespace support in WAL Replication
-   **Divya**
    -   Continue with Crash Recovery tests
    -   create Pulse project for Crash Recovery
    -   4.2.6 Pre-RC tests
-   **Jason**
    -   Continue with Crash Recovery tests
    -   work with Divya for Pulse project
    -   4.2.6 Pre-RC tests (Gnet)
-   **Savita**
    -   get familiar with the 6 storage test suites 
    -   Learn to trigger the stress test 
    -   Learn creation of pulse project
-   **Asim**
    -   20218 - spin lock(Asim)
    -   19469 - Equifax - recoverseg (Asim)
    -   Enhancement to Backup & Restore feature - MPP-20219
-   **Ashwin**
    -   Need to push the fix for MPP-19960 into 42x STORAGE branch 
    -   20190 - FINRA (Ashwin) - working on (FTS) - duplicate of 18858
-   **Anu**
    -   Look into the bugs pending verification and schedule for the following week for verification
    -   Assign to QA MPP-19259 pending verification 
    -   4.2.6 Release - Manage & Lead
    -   Schedule Bugs for the next GA release

May 20th
========

**Triage Pending**

-   In progress

**RCA Pending**

-   No Plans for this week

**Escalation**

-   **NSN -** Raghav & Divya

**Completed Task from May 13th**

-   4.2.5.2 BFV done
-   PT Stress & Longevity Done
-   Automation fro CR is done (validation in pending)
-   Fixed the Crash Recovery jiras filed last week

**This Week**

-   Demo - (Raghav/Hitoshi) WAL replication - Content to be discussion by EOD Tuseday

**Team:**

-   **Raghav**
    -   18993 and 19276 - NSN Issue
    -   WAL Replication - InstallCheckGOOD automation and demo
    -   Things to be decided yet
-   **Hitoshi**
    -   20144 - Triage Pending
    -   To work on FTS old bugs
    -   Demo WAL Replication
    -   Bug fixes for CR
-   **Divya**
    -   4.2.6 finishing SIMEX
    -   Continuting with Crash Recovery Testing
    -   Verify 4.2.2.4SY1 Release 
    -   Need to work on <a href="http://jira.eng.pivotal.io/browse/MPP-19714" class="external-link">MPP-19714</a>
-   **Jason**
    -   Validation for CR
    -   Running the Test
    -   Monitor Regression tests
    -   Trigger Stress Test 
-   **Savita**
    -   Clustion installation & transistion states done
    -   Setting up cluster on Vcloud - In progress
    -   Working on other internal tasks - targetting to complete EOW 
-   **Anu**
    -   Work with Hitoshi & Raghav to set the Traget version to "RIO SP6"
    -   Work with Hitoshi & Raghav on content of Demo
    -   4.2.6 - work with all Scrum team on current 4.2.6 content
    -   Signoff 4.2.5.2 & 4.2.2.4SY1 this week
    -   Work with Persistent India Team to wrap up on Persistent Table testing & Run code coverage on PARISTX-PT branch

May 13th
========

**Completed Task from May 6th**

-   4.2.x Merge - with only 2 bugs fixes
-   4.2.5.2 - 4 fixes

**Team:**

**Raghav:**

-   MPP-18993, 20094
-   Pull the PT changes into the 42x-storage-RIOSP5
-   **WAL Receiver:** successful in bringing the receiver, now the sender & receiver can communicate.
-   Decide on the demo

**Hitoshi:**

-   19714,19988 - For 4.2.6
-    WAL replication - Master Mirroring Failover

**Divya**

-   Validating the the 4.2.5.2 RC 1
-   4.2.6 CR (Simex, Manual Test with Workload (in TINC))
-   Fixing Pulse issues (4.2.x)
-   PT stress is done, Longevity targetted  to be done May 14th

**Anu**

-   Reopen merge jira - include 3 fixes

**Jason**

-   Crash recovery 
    -   with Workload pending
    -   OOM testing

May 5th
=======

**Completed Task from April 29th**

-   4.1.3 Signed Off
-   Bugs Compeleted Verification
    -   18998
    -   15435
    -   19044
    -   19643
    -   19535

**Retrospective**

-   Pulse project creation took longer than anticipated, due to not knowing how the scheduling will work, ahead of planning
-   Ashwin Agarwal to join the planning meeting?

**Planning for this week**

**4.2.5.2**

-   Test failure in pulse for master mirror test\_suite
-   Merge JIRA - edit to have only two fixes (19643 & 19535) instead of the current 5 CLs

**4.2.6 Bugs**

-   17759 (Raghav to investigate on the Rajesh's Bug fix to be included as part of 4.2.6)

**Team:**

-   **Raghav**
    -   Look into NSN performance issue (4.2.2.1)-- 18993
    -   WAL Replication – HA
    -   17759 (4.2.6)
    -   Reply to Archana on Rebuilding Persistent Tables
    -   Escalation Support
-   **Hitoshi**
    -   19988 (TP)
    -   19714 (Bug Fix)
-   **Divya**
    -   4.2.5.2 (Merge and validation)
    -   PT branch -Stress & Longevity, Code Review
    -   4.2.6 crash recovery manual tests
-   **Jason**
    -   Internal tasks - gpmigrator, tpch, tincrepo KT from Ravi
    -   4.2.6 crash recovery automation

April 29th
==========

**4.2.6: Bug Fixes:**\* \*Hitoshi to integrate bugs  (18998,15435,19044) from Storage Branch -&gt; Storage RIOSP5 Branch

-   18998 (Santosh/Pramod to do the BFV & Automate)
-   15435 (Santosh/Pramod to do the BFV & Automate)
-   19044 (Santosh already Automated, Divya to turn on the tests in TINC to be a part of the schedule) 
-   17759 (Raghav to investigate on the Rajesh's Bug fix to be included as part of 4.2.6)

**4.1.3 Pending:** ** **

-   Longevity validation running expected to be completed April 29th
-   Upgrade tests in progress
-   Sign Off – May 1st

**4.2.5.2:**

-   19643 – Master Mirroring & 19535 – Finra TM Lock issue
    -   Bug Verification done
    -   Regression Tests validated
    -   Stress tests went down – Raghav to look into the system 
    -   Anu to create a Merge jira once we are done with validating these Bug fixes to be integrated into 4.2.5.2

**Team:**

-   Triage Pending Bugs
-   RCA Pending Bugs
-   Look into NSN performance issue (4.2.2.1)-- 18993
-   WAL Replication – HA

Document generated by Confluence on May 17, 2016 19:14


