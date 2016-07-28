<span id="title-text"> Storage & Access : Definition of done for the Storage Stories </span>
============================================================================================

This page last changed on Dec 04, 2013 by prabaa1.

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p><strong>Story Type</strong></p></th>
<th align="left"><p><strong>DOD Checklist</strong></p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left">Feature Development</td>
<td align="left"><ul>
<li>Code review complete</li>
<li><span style="color: rgb(255,0,0);">Cyclomatic code complexity on new files &lt;= 10</span></li>
<li><span style="color: rgb(255,0,0);">No new correctness issues flagged by <a href="http://confluence.greenplum.com/display/PROGMGMT/ODASA+Code+Inspection+Tool" class="external-link"><span style="color: rgb(255,0,0);">ODASA</span></a> (where applicable)</span></li>
<li><span style="color: rgb(255,0,0);">No issues flagged by ODASA for new files</span></li>
<li>Unit testing / installcheck-good testing.</li>
<li>Features with a SQL interface must have SQL level testing</li>
<li>No known regressions introduced</li>
<li>All functional testing complete</li>
<li>Performance testing complete, if feature is in a performance critical path</li>
<li>Build and installcheck on all platforms</li>
<li><span style="font-size: 14.0px;line-height: 1.4285715;">New functionality in new files all files</span></li>
<li><span style="font-size: 14.0px;line-height: 1.4285715;">Code coverage for new files &gt; 90% for developer testing (core engine)</span></li>
<li><span style="font-size: 14.0px;line-height: 1.4285715;">Fault injection </span></li>
<li><span style="font-size: 14.0px;line-height: 1.4285715;">Minimal documentation provided in the JIRA </span></li>
</ul></td>
</tr>
<tr class="even">
<td align="left">Bug Fix Verification</td>
<td align="left"><ol>
<li>Bug Fix without Repro
<ol>
<li>Unit tests created</li>
<li>Storage Regression tests excuted on the fix</li>
<li>More critical &amp; Complex fix requires Stress &amp; Longevity</li>
<li>If this is a customer-found issue, fill out the Escape Analysis </li>
</ol></li>
<li>Bug Fix lacking Framework for Automation
<ol>
<li>Unit tests created</li>
<li>Manual Verification done &amp; added to Test Rail</li>
<li>Storage Regression tests excuted on the fix</li>
<li>More critical &amp; Complex fix requires Stress &amp; Longevity</li>
<li>If this is a customer-found issue, fill out the Escape Analysis </li>
</ol></li>
<li>Bug Fixes which can be automated<br />

<ol>
<li>Unit tests created</li>
<li>Tests automated</li>
<li>Storage Regression tests excuted on the fix</li>
<li>More critical &amp; Complex fix requires Stress &amp; Longevity</li>
<li>If this is a customer-found issue, fill out the Escape Analysis </li>
</ol></li>
</ol></td>
</tr>
<tr class="odd">
<td align="left">Feature Testing</td>
<td align="left"><ul>
<li>Created Software Test Plan</li>
<li>Team review on test plan</li>
<li>All Funaction testings automated</li>
<li>Code coverage 90% for new files</li>
<li>Related Automated Tests Green on Pulse before checkin</li>
<li>Related Automated Tests Green on Pulse after final check in</li>
<li>No tests relying on private branches</li>
<li>Run full Regression test on Feature Branch including Stress, Longevity &amp; Performance before integrating<br />
<br />
</li>
<li><span style="color: rgb(0,128,0);">ToDo : Create a JIRA for tracking why the tests are manual and resolve as wont fix </span></li>
</ul></td>
</tr>
<tr class="even">
<td align="left">Hotfix</td>
<td align="left"><ul>
<li><span>Unit test</span></li>
<li>Execute Targetted Regression tests</li>
<li>Verify the bug Fix</li>
<li>Validate the RC</li>
<li>Validate the Server Package/ Appliance Build</li>
</ul></td>
</tr>
<tr class="odd">
<td align="left">Escalation</td>
<td align="left"><ol>
<li>Identify the problem the customer is facing</li>
<li>Remove the problem if possible (e.g. bring back system, provide workaround, not necessary RCA)</li>
<li>Collect information necessary for further investigation</li>
</ol></td>
</tr>
<tr class="even">
<td align="left">Triage</td>
<td align="left"><ol>
<li>Understand the issue reported in the JIRA</li>
<li>If RCA gets known, set Target Version appropriately</li>
<li>If more information is necessary, assign back to the reporter to ask for it.</li>
<li>If RCA is not known, remove Triage Pending and set RCA_pending label.</li>
</ol></td>
</tr>
<tr class="odd">
<td align="left"><p>TOI/Training</p></td>
<td align="left"><ol>
<li>Story description has clear details of the scope of the training activity</li>
<li>Story description has defined – a Mentor and a reviewer for the task</li>
<li>The assignee has completed the work on the story as per the defined scope</li>
<li>The assignee has added relevant information about the tasks in this ticket   (session log, status of subtasks etc)</li>
<li>The assignee has added actual effort details in the ticket (logged hours)</li>
<li> If there is not enough documentation available for the task on Confluence,   assignee will create a page with the information which will help future   members</li>
<li>The reviewer/mentor has reviewed the progress and verified completion ( with the help of an assessment process)</li>
</ol></td>
</tr>
<tr class="even">
<td align="left"><p>Regression Monitoring</p></td>
<td align="left"><ol>
<li><p>Story description clearly mentions the regression cycle detail:</p>
<ol>
<li>Which release</li>
<li>Which build</li>
<li>Which test area/schedule</li>
<li>Which test hardware – cloud/DCA(with rack   details)/physical machine</li>
<li>Which test OS – RHEL/CentOS/Solaris/Suse etc</li>
<li>The test scenarios 
<ol>
<li><span style="font-size: 14.0px;line-height: 1.4285715;">For automated run - Document  the Pulse project or cdbfast or tinc test   schedule</span></li>
<li><span style="font-size: 14.0px;line-height: 1.4285715;">For Manual run – Document the TestSuite you are choosing, are you selecting all tests or a selected subset</span></li>
</ol></li>
</ol></li>
<li>Test Run summary:
<ol>
<li>  If automated run – Pulse project run link, and the link to the report of automatically triggered run (sent by releng)</li>
<li>  If manual run – A TestRail Test run with   results updated as appropriate</li>
</ol></li>
<li>Tests are re-run/ build re-triggered if required</li>
<li>Links and Observations about each re-run are documented</li>
<li>The assignee has checked Pulse results and artifacts</li>
<li>The assignee has logged in to the test environments and checked the log files</li>
<li>If the run indicates product failures, defects logged in Jira following the GPDB   defect template</li>
<li>If the run indicated test code issues, a task ticket is logged to track code   fixes</li>
<li>The Confluence page created for the regression cycle is updated by the Owner with all relevant details</li>
<li>The assignee has added actual effort details in the ticket (logged hours)</li>
<li>The assignee has updated the QA status page</li>
<li>The assignee has published the summary of test run (usually done in emails)</li>
</ol></td>
</tr>
<tr class="odd">
<td align="left"><p>Test Automation</p>
<p>(new or fix)</p></td>
<td align="left"><ol>
<li>The <span>Story</span> description clearly indicates the objective of the coding task</li>
<li>The Story description clearly indicates the reviewers</li>
<li>The assignee confirm the branch from where code is to be synched from, and later merged </li>
<li>The assignee does a sync with latest code base before starting any coding</li>
<li>The assignee follows coding standards (add appropriate comments, follow the same structure as other tests, re-use library functions etc.)</li>
<li>The assignee unit tests the code</li>
<li>The assignee does another sync with latest code and tests again</li>
<li><span style="color: rgb(0,128,0);">To do: before starting the actual implementation review it with appropriate team (FIT3)</span></li>
<li>A code review is initiated with appropriate reviewers</li>
<li>Review comments incorporated</li>
<li>Confirmation from the reviewers on #9, all review comments closed</li>
<li>The assignee does another sync with latest code and tests again (after all review comments are closed)</li>
<li>Check in the code</li>
<li>Publish the changes to the relevant test-automation owners/email alias/group</li>
<li>If this code base is used in any Pulse projects, a build is triggered to ensure it passes ( on the agents which are normally used for the tests)</li>
</ol></td>
</tr>
<tr class="even">
<td align="left"><p>Spike*</p></td>
<td align="left"><ol>
<li>The ticket clearly defines what is the spike story for</li>
<li>Define a minimum completion criteria</li>
<li>Define additional coverage that can be completed in a spike</li>
<li>Based on the exact “spike” story, Scrum master will define the DOD for it after discussion with the team and the PO.</li>
<li>The assignee will keep the story updated with progress summary/comments</li>
</ol></td>
</tr>
</tbody>
</table>

 

**\*What is a spike?**

A spike is a story which is created for the team usually to figure out stuff that they don't know and need to know in order to understand the complexity so that it can be properly estimated in future. The spike is over when the time is up, not necessarily when the objective has been delivered.

**Spike in Storage:**

In case of Storage we have create spike stories for:

1.  Catch up on training activity
2.  Refresh the learning and brush up skills learnt in past few sprints
3.  recap previous stories and redo those which needed more hands on practice
4.  Fast-study of tasks (that someone else owns) to act as a backup in case needed.

Since the spike is time bound and will not be pushed to backlog or re-estimated, it will be necessary for PO and SM to come up with what they want to achieve at the end of the spike. Need to define a minimum / maximum progress expected in a spike.

This will be done on case-by-case basis depending on what the “spike” story is.

Document generated by Confluence on May 17, 2016 19:14


