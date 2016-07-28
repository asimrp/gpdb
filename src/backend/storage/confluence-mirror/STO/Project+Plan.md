<span id="title-text"> Storage & Access : Project Plan </span>
==============================================================

This page last changed on Jan 15, 2010 by bergam1.

### Project Plan

Feature will be delivered in milestones

**Milestone 1**

-   Delivery time is **Nov 18**.
-   The functionality
    1.  Mirroring
    2.  DML operations (insert, update, delete)
    3.  DDL operations (create and drop database, create and drop table, create and drop index)
    4.  Management Utilities (gpinitsystem, gpstart, gpstop)
    5.  gp\_configuration

**Milestone 2**

-   The functionality
    1.  Failover
    2.  Transaction Recovery
    3.  DDL operations
    4.  Management Utilities (see below)
-   Milestone 2 is delivered in three phases to QA
    1.  No Mirroring - Estimate Time Jan 13.
    2.  Mirroring - Estimate Time Jan 22. Per status on 01/15/10 the following tasks (described also in table below) has to be completed before testing can proceed
        -   gpcheckmirrorseg (verification utility while no IO (reads and writes) are running against database)

    3.  Failover and Fault Detection - Estimate Time Feb 12.

**Milestone 3**

-   Estimate time of delivery is Mar 5.
-   The functionality
    1.  Change Logging
    2.  Resynchronization (incremental and full copy)
    3.  DDL operations during Resynchronization
    4.  Management Utilities (gpaddmirrors, gprecoverseg)
    5.  Management Utilities (new utility gpcheckmirrorseg ONLINE)
    6.  Support for <a href="/pages/createpage.action?spaceKey=STO&amp;title=FileSpaces&amp;linkCreation=true&amp;fromPageId=23790224" class="createlink">FileSpaces</a> with mirroring

**Milestone 4**

-   Estimate time of delivery is Mar 5.
-   The functionality
    1.  upgrade (mirror + system upgrade)
    2.  gpexpand
    3.  Management Utilities (new utility gpmirrorrepairseg)
    4.  Performance enhancements
    5.  Master Mirroring
    6.  Fault Injection (Jan 20th)

The table captures detail description of tasks.

<img src="plugins/servlet/confluence/placeholder/unknown-macro" class="wysiwyg-unknown-macro" />

Comments:
---------

<table>
<colgroup>
<col width="100%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><a href=""></a>
<h3 id="ProjectPlan-FileRepMeetingNotes:1/4/10">FileRep Meeting Notes: 1/4/10</h3>
<h5 id="ProjectPlan-Wherearewetoday?">Where are we today?</h5>
<ul>
<li>3.4_FileRep branches exist for cdb2 and cdbfast</li>
<li>Bugs are tracked with Fix Version = &quot;3.4 FileRep&quot;</li>
<li>FTS, Upgrade and Expansion tests are moved over to sys_mgmt_test</li>
<li>sys_mgmt_test needs to be branched for differences between 3.3.x and 3.4.x. Johnny is currently working on upgrade test coverage. This should not be a criteria for Entry to MAIN</li>
</ul>
<h5 id="ProjectPlan-MilestoneA:EntryCriteriatoMAIN">Milestone A: Entry Criteria to MAIN</h5>
<ul>
<li>cdbfast will run without system management tests</li>
<li>Minimal: cdbfast runs with both without mirror.
<ul>
<li>Fix cdbfast core issues</li>
<li>gpdetective: need to verify</li>
<li>gpcheckcat: needs to verify</li>
<li>gpload: Ken has minor fix to make</li>
<li>gpexpand: Should work without mirroring, tablespaces or SAN</li>
<li>Upgrade: Will work without mirror without mirroring, tablespaces or SAN</li>
</ul></li>
</ul>
<h5 id="ProjectPlan-MilestoneB:SegmentMirrorsEnabled">Milestone B: Segment Mirrors Enabled</h5>
<ul>
<li>cdbfast will run with mirrors and include recovery
<ul>
<li>Fix cdbfast core locking issues</li>
<li>failover: Tim working on this will deliver gprecoverseg that does a full copy recovery</li>
<li>filerep verification tool (OFFLINE)</li>
</ul></li>
</ul>
<h5 id="ProjectPlan-MilestoneC:Forcedfaults,crashrecoveryandresynchronization">Milestone C: Forced faults, crash recovery and resynchronization</h5>
<ul>
<li>Additional Coverage with mirrors
<ul>
<li>Upgrade: major work</li>
<li>gpexpand: minor work</li>
<li>gpdetective: minor issues ... could wait</li>
<li>gpcheckcat: minor issues ... could wait</li>
</ul></li>
<li>Additional Scripts as well as Master Mirroring
<ul>
<li>gpinitstandby</li>
<li>gpactivatestandby</li>
<li>gpaddmirrors</li>
<li>gprecoverseg</li>
<li>gprebuildsystem: deprecated?</li>
</ul></li>
</ul>
<h5 id="ProjectPlan-ExitCriteriafor3.4:">Exit Criteria for 3.4:</h5>
<ul>
<li>Performance testing</li>
<li>sys_mgmt_test suite</li>
<li>cdbfast passsing with master and segment mirroring</li>
<li>Testing of all phases: Failure Detection, Failover, Change logging and Resyncronization</li>
</ul>
<h5 id="ProjectPlan-AutomationRequirements">Automation Requirements</h5>
<ul>
<li>filerep verification tool (OFFLINE/ONLINE): can be interleaved within all test suites</li>
<li>cdbfast: interleave filerep</li>
<li>installcheck-good</li>
<li>PDW</li>
<li>sys_mgmt_test</li>
</ul>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by cdaw at Jan 04, 2010 16:27
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<h4 id="ProjectPlan-NotesfromPerformanceTestingDiscussion">Notes from Performance Testing Discussion</h4>
<h5 id="ProjectPlan-ThephasesofanFTStestare:">The phases of an FTS test are:</h5>
<ol>
<li>Error Detection: we will NOT be measuring the time to detect the failure</li>
<li>Failover to Continue Mode: we will need to ensure we failover in under 60 seconds with it appearing</li>
<li>Change logging while in Continue Mode: need to measure the impact to the system with this on vs. off</li>
<li>Resynchronization to get the change log sync'ed: need to measure the time required to sync the system, as well as impact of system due to resynchronization actions</li>
<li>Upgrade: need to ensure we can upgrade our largest customers in under 24 hours</li>
</ol>
<h5 id="ProjectPlan-Requiresacomparisonof3.3vs.3.4">Requires a comparison of 3.3 vs. 3.4</h5>
<div class="table-wrap">
<table>
<colgroup>
<col width="20%" />
<col width="20%" />
<col width="20%" />
<col width="20%" />
<col width="20%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Storage Type</p></th>
<th align="left"><p>3.3 no mirrors</p></th>
<th align="left"><p>3.3 with mirrors</p></th>
<th align="left"><p>3.4 no mirrors</p></th>
<th align="left"><p>3.4 with mirrors</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>heap</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
</tr>
<tr class="even">
<td align="left"><p>AO</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
</tr>
<tr class="odd">
<td align="left"><p>AO Compressed</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
</tr>
<tr class="even">
<td align="left"><p>CO</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
</tr>
<tr class="odd">
<td align="left"><p>DDL Ops</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
<td align="left"><p>x</p></td>
</tr>
</tbody>
</table>
</div>
<p><em>x = CRUD + bulk load timings</em></p>
<h5 id="ProjectPlan-Requiresacomparisonof3.4vs.3.4-betweenphases">Requires a comparison of 3.4 vs. 3.4 - between phases</h5>
<div class="table-wrap">
<table>
<colgroup>
<col width="25%" />
<col width="25%" />
<col width="25%" />
<col width="25%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Actions</p></th>
<th align="left"><p>Change logging Off<br />
(Normal State)</p></th>
<th align="left"><p>Change logging On<br />
(Continue Mode)</p></th>
<th align="left"><p>During Resyncronization<br />
(gprecoverseg)</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>Bulk Loads</p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
</tr>
<tr class="even">
<td align="left"><p>Concurrent Inserts</p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
</tr>
<tr class="odd">
<td align="left"><p>Concurrent Updates</p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
</tr>
<tr class="even">
<td align="left"><p>Concurrent Deletes</p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
</tr>
<tr class="odd">
<td align="left"><p>DDL Ops</p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
<td align="left"><p> </p></td>
</tr>
</tbody>
</table>
</div>
<h5 id="ProjectPlan-Scale/StressTests">Scale / Stress Tests</h5>
<ul>
<li>Scale Tests: Need to determine what we will attempt for this release. <a href="http://confluence.greenplum.com/display/Perf/3.3+migrator+scale+tests" class="external-link">Review 3.3 upgrade scale tests as reference</a>. For this test the 3 dimensions of scales are number of objects, data size and cluster size.</li>
<li>Longevity Tests: Important to run a very long stress test which has the mirror verification tool interweaved to ensure no corruption to the mirror. The proposal is to run gperf against this.</li>
</ul>
<h5 id="ProjectPlan-AdditionalItems">Additional Items</h5>
<ul>
<li>POC / EAP: Strong request to get this exercised early in the field to get feedback. (Cathy)</li>
<li>Integrate gperf into PDW (Ivan)</li>
<li>Add DDL test cases into PDW (Bharath / Ivan)</li>
<li>Add instrumentation to the sys_mgmt_test suite to capture performance metrics and store in PDW (Bharath)</li>
</ul>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by cdaw at Jan 04, 2010 17:00
</div></td>
</tr>
</tbody>
</table>

Document generated by Confluence on May 17, 2016 19:14


