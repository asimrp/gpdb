<span id="title-text"> Storage & Access : Storage Troubleshooting Notes </span>
===============================================================================

This page last changed on Apr 14, 2015 by petang.

[ProposedDesign-PTRebuildutility.docx](attachments/42502284/73990158.docx)

***Author: Ken Sell***

***Created: January 21, 2012 Version 1.10***

-   <span class="TOCOutline">1</span> [Introduction](#StorageTroubleshootingNotes-Introduction)
-   <span class="TOCOutline">2</span> [Useful Stuff](#StorageTroubleshootingNotes-UsefulStuff)
    -   <span class="TOCOutline">2.1</span> [Useful Tools](#StorageTroubleshootingNotes-UsefulTools)
    -   <span class="TOCOutline">2.2</span> [Useful Functions](#StorageTroubleshootingNotes-UsefulFunctions)
    -   <span class="TOCOutline">2.3</span> [Useful Tables](#StorageTroubleshootingNotes-UsefulTables)
    -   <span class="TOCOutline">2.4</span> [Useful Tools - VERY DESTRUCTIVE](#StorageTroubleshootingNotes-UsefulTools-VERYDESTRUCTIVE)
    -   <span class="TOCOutline">2.5</span> [Useful Functions - VERY DESTRUCTIVE](#StorageTroubleshootingNotes-UsefulFunctions-VERYDESTRUCTIVE)
-   <span class="TOCOutline">3</span> [Tips and Tricks](#StorageTroubleshootingNotes-TipsandTricks)
    -   <span class="TOCOutline">3.1</span> [How to connect to the master or a segment in utility mode?](#StorageTroubleshootingNotes-Howtoconnecttothemasterorasegmentinutilitymode?)
    -   <span class="TOCOutline">3.2</span> [How to check if activity is running on the system ?](#StorageTroubleshootingNotes-Howtocheckifactivityisrunningonthesystem?)
    -   <span class="TOCOutline">3.3</span> [How to get configuration information?](#StorageTroubleshootingNotes-Howtogetconfigurationinformation?)
    -   <span class="TOCOutline">3.4</span> [How to check for duplicate entries in pg\_class?](#StorageTroubleshootingNotes-Howtocheckforduplicateentriesinpg_class?)
    -   <span class="TOCOutline">3.5</span> [How to get the total on disk size of a table?](#StorageTroubleshootingNotes-Howtogetthetotalondisksizeofatable?)
    -   <span class="TOCOutline">3.6</span> [How to get the per-segment on disk size of a table?](#StorageTroubleshootingNotes-Howtogettheper-segmentondisksizeofatable?)
    -   <span class="TOCOutline">3.7</span> [How to estimate how long will it take to run gprecoverseg --F (i.e. full)?](#StorageTroubleshootingNotes-Howtoestimatehowlongwillittaketorungprecoverseg--F(i.e.full)?)
    -   <span class="TOCOutline">3.8</span> [How to get system configuration when system will not start?](#StorageTroubleshootingNotes-Howtogetsystemconfigurationwhensystemwillnotstart?)
    -   <span class="TOCOutline">3.9</span> [How to get the compression ratio for AO and CO tables?](#StorageTroubleshootingNotes-HowtogetthecompressionratioforAOandCOtables?)
    -   <span class="TOCOutline">3.10</span> [How to select from a single segment ?](#StorageTroubleshootingNotes-Howtoselectfromasinglesegment?)
    -   <span class="TOCOutline">3.11</span> [How to get state information directly from a segment?](#StorageTroubleshootingNotes-Howtogetstateinformationdirectlyfromasegment?)
    -   <span class="TOCOutline">3.12</span> [How to force a primary to fail over to its mirror?](#StorageTroubleshootingNotes-Howtoforceaprimarytofailovertoitsmirror?)
    -   <span class="TOCOutline">3.13</span> [How to debug a backend process?](#StorageTroubleshootingNotes-Howtodebugabackendprocess?)
    -   <span class="TOCOutline">3.14</span> [How to investigate a specific error message?](#StorageTroubleshootingNotes-Howtoinvestigateaspecificerrormessage?)
    -   <span class="TOCOutline">3.15</span> [How to upgrade a customer's binary and/or how to create a hot patch for a customer?](#StorageTroubleshootingNotes-Howtoupgradeacustomer'sbinaryand/orhowtocreateahotpatchforacustomer?)
-   <span class="TOCOutline">4</span> [Diagnose and Recover](#StorageTroubleshootingNotes-DiagnoseandRecover)
    -   <span class="TOCOutline">4.1</span> [What are some good questions to ask before doing anything (i.e. Look don't touch phase of recovery)?](#StorageTroubleshootingNotes-Whataresomegoodquestionstoaskbeforedoinganything(i.e.Lookdon'ttouchphaseofrecovery)?)
    -   <span class="TOCOutline">4.2</span> [Is the system down and will not start?](#StorageTroubleshootingNotes-Isthesystemdownandwillnotstart?)
    -   <span class="TOCOutline">4.3</span> [Try to figure out what kind of issue it is](#StorageTroubleshootingNotes-Trytofigureoutwhatkindofissueitis)
    -   <span class="TOCOutline">4.4</span> [Is it a backup or restore issue?](#StorageTroubleshootingNotes-Isitabackuporrestoreissue?)
    -   <span class="TOCOutline">4.5</span> [Is it a port already in use issue?](#StorageTroubleshootingNotes-Isitaportalreadyinuseissue?)
    -   <span class="TOCOutline">4.6</span> [Is it a filerep issue?](#StorageTroubleshootingNotes-Isitafilerepissue?)
    -   <span class="TOCOutline">4.7</span> [Is it a hang issue?](#StorageTroubleshootingNotes-Isitahangissue?)
    -   <span class="TOCOutline">4.8</span> [Is it an xlog issue?](#StorageTroubleshootingNotes-Isitanxlogissue?)
    -   <span class="TOCOutline">4.9</span> [Is it an FTS issue?](#StorageTroubleshootingNotes-IsitanFTSissue?)
    -   <span class="TOCOutline">4.10</span> [Are there issues with database consistency?](#StorageTroubleshootingNotes-Arethereissueswithdatabaseconsistency?)
    -   <span class="TOCOutline">4.11</span> [Is the current primary bad, but the mirror looks good?](#StorageTroubleshootingNotes-Isthecurrentprimarybad,butthemirrorlooksgood?)
    -   <span class="TOCOutline">4.12</span> [Is the system up but in change tracking?](#StorageTroubleshootingNotes-Isthesystemupbutinchangetracking?)
    -   <span class="TOCOutline">4.13</span> [Does a primary need to be restored from its mirror?](#StorageTroubleshootingNotes-Doesaprimaryneedtoberestoredfromitsmirror?)
-   <span class="TOCOutline">5</span> [How to Rebuild the Persistent Tables](#StorageTroubleshootingNotes-HowtoRebuildthePersistentTables)
    -   <span class="TOCOutline">5.1</span> [What if part of the problem is that mirror will not come up?](#StorageTroubleshootingNotes-Whatifpartoftheproblemisthatmirrorwillnotcomeup?)

Introduction
============

This document contains information about how to recover a user system that is not in a healthy state, and cannot be recovered using normal methods. The procedures described in this document can be very DESTRUCTIVE to a user system. Only internal Greenplum personal that are very confident with making changes to internal database structures should use these procedures. Customers should be clearly informed of the possible risks and whenever possible, a backup of the database (e.g. gp\_dump) should be made before beginning recovery procedures.

Useful Stuff
============

Useful Tools
------------

The following table contains a list of tools that will assist you in analyzing an issue.
These tools are non-destructive, and therefore can be safely run at any time.

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Tool name</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>gpcheckcat</p></td>
<td align="left"><p>Check the system catalog tables for inconsistencies</p></td>
</tr>
<tr class="even">
<td align="left"><p>gpstate</p></td>
<td align="left"><p>Check the system status</p></td>
</tr>
<tr class="odd">
<td align="left"><p>pg_controldata</p></td>
<td align="left"><p>Dump the contents of the pg_control file in a human readable form</p></td>
</tr>
<tr class="even">
<td align="left"><p>xlogdump</p></td>
<td align="left"><p>Dump the contents of the xlog in a human readable form</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_primarymirror</p></td>
<td align="left"><p>Get information on the state of a segment</p></td>
</tr>
<tr class="even">
<td align="left"><p>changetrackingdump</p></td>
<td align="left"><p>Dumps the contents of a changetracking log file or meta file</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_filedump</p></td>
<td align="left"><p>Displays formatted contents of a PostgreSQL heap/index/control/append only file</p></td>
</tr>
<tr class="even">
<td align="left"><p>gprecoverseg</p></td>
<td align="left"><p>Utility for recovering a failed segment.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gpverify</p></td>
<td align="left"><p>Utility for verifying primary and mirror while GPDB is online.</p></td>
</tr>
<tr class="even">
<td align="left"><p>gpcheckmirrorseg.pl</p></td>
<td align="left"><p>Utility for verifying primary and mirror while no IO activity is going on to GPDB.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gprepairmirrorseg</p></td>
<td align="left"><p>Utility for repairing mirror segment while GPDB is offline.</p></td>
</tr>
</tbody>
</table>

Useful Functions
----------------

The following table contains a list of functions that will assist you in analyzing an issue. Accessing these functions is non-destructive, and therefore they can be safely invoked at any time. These are internal functions, and cannot be called directly. They are located in a library (gp\_ao\_co\_diagnostics), which is available internally in a support package. See the README file that accompanies the support package for more details (contrib/gp\_internal\_tools/README in Perforce).

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Function name</p></th>
<th align="left"><p>Parameter definition</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>gp_aocsseg_history(oid)</p></td>
<td align="left"><p><span style="text-decoration: underline;">oid</span> - The <span style="text-decoration: underline;">oid</span> of a column-oriented (CO) table</p></td>
<td align="left"><p>Returns metadata information contained in the CO table's pg_aoseg.pg_aocsseg_<span style="text-decoration: underline;">&lt;oid&gt;</span>, including non-<span style="text-decoration: underline;">visible</span> rows. Non-visible rows can be interpreted as historic entries in the table, and therefore aid in debugging.</p></td>
</tr>
<tr class="even">
<td align="left"><p>gp_aocsseg(oid)</p></td>
<td align="left"><p><span style="text-decoration: underline;">oid</span> - The <span style="text-decoration: underline;">oid</span> of a column-oriented (CO) table</p></td>
<td align="left"><p>Returns <span style="text-decoration: underline;">metadata</span> information contained in the CO table's pg_aoseg.pg_aocsseg_<span style="text-decoration: underline;">&lt;oid&gt;</span>, not including non-<span style="text-decoration: underline;">visible</span> rows.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_aoseg_history(oid)</p></td>
<td align="left"><p><span style="text-decoration: underline;">oid</span> - The <span style="text-decoration: underline;">oid</span> of append-only (AO) row-oriented table</p></td>
<td align="left"><p>Returns <span style="text-decoration: underline;">metadata</span> information contained in the AO row-oriented table's pg_aoseg.pg_aoseg_<span style="text-decoration: underline;">&lt;oid&gt;</span>, including non-<span style="text-decoration: underline;">visible</span> rows. Non-visible rows can be interpreted as historic entries in the table, and therefore aid in debugging.</p></td>
</tr>
</tbody>
</table>

Useful Tables
-------------

The following table contains a list of database tables that will assist you in analyzing an issue. Accessing these tables is non-destructive, and therefore can be safely read at any time. Table access is via a psql 'select \* &lt;table&gt;' statement.

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Table name</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>gp_configuration_history</p></td>
<td align="left"><p>Contains information about system changes related to fault detection and recovery operations</p></td>
</tr>
<tr class="even">
<td align="left"><p>gp_master_mirroring</p></td>
<td align="left"><p>Contains information about the standby master</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_segment_configuration</p></td>
<td align="left"><p>Contains information about the system's configuration</p></td>
</tr>
<tr class="even">
<td align="left"><p>pg_stat_activity</p></td>
<td align="left"><p>Contains information about current activity on the system</p></td>
</tr>
<tr class="odd">
<td align="left"><p>pg_locks</p></td>
<td align="left"><p>Contains information about currently acquired locks</p></td>
</tr>
</tbody>
</table>

Useful Tools - VERY DESTRUCTIVE
-------------------------------

The following table contains a list of tools that will assist you in repairing a system. The tools **ARE DESTRUCTIVE**, and should be used with the utmost care.

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Tool name</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>pg_resetxlog</p></td>
<td align="left"><p>Resets the xlog. It will cause the system to ignore in flight transactions and data in the xlog that has not been written to disk. It potentially leaves the database in an inconsistent state</p></td>
</tr>
</tbody>
</table>

Useful Functions - VERY DESTRUCTIVE
-----------------------------------

The following table contains a list of GP functions that will assist you in repairing a system. The functions **ARE DESTRUCTIVE**, and should be used with the utmost care.
Each function can be invoked by using a 'select \* from &lt;function&gt;' via psql.

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Function name</p></th>
<th align="left"><p>Parameters</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>gp_persistent_build_db(boolean)</p></td>
<td align="left"><p>boolean - false for segment<br />
              - true for master</p></td>
<td align="left"><p>Backdoor function for re-building persistent tables.<br />
It only applies in to 4.0.x releases after 4.0.5.4<br />
A guc, &quot;<strong>gp_persistent_build_db_overload_string</strong>&quot; is used to control the procedures behavior.<br />
Valid values are:<br />
<em>RESET_ALL</em> – Removes all entries from the persistent tables.<br />
<em>BUILD_ALL</em> – Re-build all entries in the persistent tables.</p></td>
</tr>
<tr class="even">
<td align="left"><p>gp_persistent_reset_all()</p></td>
<td align="left"><p> </p></td>
<td align="left"><p>Function to truncate all persistent tables. It only apples to 4.1.x releases and later.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>gp_persistent_build_all(boolean)</p></td>
<td align="left"><p>boolean - false for segment<br />
              - true for master</p></td>
<td align="left"><p>Function to rebuild all persistent tables. It only apples to 4.1.x releases and later.</p></td>
</tr>
</tbody>
</table>

Tips and Tricks
===============

The following are some useful tips and tricks:

###### **How to connect to the master or a segment in utility mode?**

For a segment, from a shell prompt, enter:

``` theme:
PGOPTIONS='-c gp_session_role=utility' psql <psql options>
```

For the master running in master only mode (i.e. gpstart --m),

``` theme:
PGOPTIONS='-c gp_session_role=utility -c gp_role=utility' psql <psql options>
```

If the system is 4.2 or later, and you wish to modify system tables, then the GUC "**allow\_system\_table\_mods**" must be set in the connection.

From a shell prompt, enter:

``` theme:
PGOPTIONS='-c gp_session_role=utility --c allow_system_table_mods=<mod type>'
psql <psql options>
```

where &lt;mod type&gt; is dml, ddl, or all.

For example, to connect to a segment running on host sdw1 and its postmaster is
running on port 20400 on a 4.2.0.0 system, you would enter:

``` theme:
PGOPTIONS='-c gp_session_role=utility --c allow_system_table_mods=dml' psql --p 20400 --h sdw1
```

###### **How to check if activity is running on the system ?**

``` theme:
select * from pg_stat_activity;
```

###### **How to get configuration information?**

It is important to identify where the problem is occurring. It could be on the master and/or on one or more segments. An important step is to identify all the machines/process in the configuration.

``` theme:
select * from gp_master_mirroring;
select * from gp_segment_configuration;
select * from gp_configuration_history;
```

If the master is down , then look on the master machine in gpAdminLogs or in pg\_log in the $MASTER\_DATA\_DIRECTORY for configuration information.

###### **How to check for duplicate entries in pg\_class?**

``` theme:
SELECT gp_segment_id, cmin, cmax, xmin, xmax, oid, *
FROM GP_DIST_RANDOM('pg_class')
WHERE (gp_segment_id, oid)
IN (SELECT gp_segment_id, oid
    FROM GP_DIST_RANDOM('pg_class')
    GROUP BY gp_segment_id, oid
    HAVING count (*) > 1)
ORDER BY gp_segment_id, oid;
```

###### **How to get the total on disk size of a table?**

``` theme:
select pg_size_pretty(pg_relation_size('<table name>'));
```

###### **How to get the per-segment on disk size of a table?**

``` theme:
SELECT gp_segment_id, pg_size_pretty(pg_relation_size('<table name>'))
FROM gp_dist_random('gp_id');
```

###### **How to estimate how long will it take to run gprecoverseg --F (i.e. full)?**

A full recovery will copy all the data from the primary to the mirror. All primaries are copied in parallel at about 100 megabytes per second (400 GB/hour) on a system with nothing else going on. Assuming all the segments are about the same size, it will take (size of a segment in megabytes / 100) seconds. For example, if a segment contains 1 terabyte, then it will take about 3 hours to complete a full recovery.

###### **How to get system configuration when system will not start?**

Often the master will start in master only mode. gpstart --m. Connect to the master in utility role and utility session role.

``` theme:
PGOPTIONS='-c gp_session_role=utility \-c gp_role=utility'  psql \-p <master port>
```

###### **How to get the compression ratio for AO and CO tables?**

``` theme:
select * from get_ao_compression_ratio('<table name>');
```

###### **How to select from a single segment ?**

``` theme:
select * from <table name> where gp_segment_id = <segment content id>;
```

###### **How to get state information directly from a segment?**

``` theme:
echo getStatus \| gp_primarymirror --h <segment host> \-p <segment postmaster port>
```

###### **How to force a primary to fail over to its mirror?**

There are two methods:

1.

``` theme:
Change gp_segemnt_configuration. This is the preferred method, and is less likely to cause other issues.
# Stop the system (gpstop).
# Start the master (gpstart --m).
# Connect to the master.
# Do "set allow_system_table_mods = dml"
# Do "update gp_segment_configuration set role = 'm', mode = 's',  status = 'd' where dbid = <seg primary dbid>".
# Do "update gp_segment_configuration set role = 'p' , mode = 'c', status = 'u' where dbid = <seg mirror dbid>".
# Stop the master (gpstop).
# Start the system (gpstart).
```

2.

``` theme:
Stop the appropriate primary's postmaster. This will force a failover, which has a higher potential to cause other issues.
# Login to the system with the segment.
# Find the segment's PID (e.g. ps \-ef \| grep "p <segment's port>"
# Kill the postmaster. (e.g. kill --9 <segment's postmaster PID>)
# Clean up any dead process associated with the segment (e.g. ps --ef \| grep "<segment's port>", then kill \-9 <segment pids>).
```

###### **How to debug a backend process?**

``` theme:
Start gdb

gdb> attach <pid of process>
Some useful variables to look at:

gdb> p dataState
gdb> p segmentState
gdb> p fileRepRole
gdb> p held_lwlocks_exclusive
gdb> p held_lwlocks
gdb> p LWLockArray
```

###### **How to investigate a specific error message?**

If you find a specific error message in the log (on the master or on a segment), find that error message in the code (of course may need to use wild cards to factor out specific variable values printed in the message). When you identify the error message in the code, look to see if there are gucs used in the same area of the code that you might be able to turn on for more information. If so set the guc in postgressql.conf (just on the machine – master or segment - that is having the issue) and then run gpstart again.and do a gpstart --u to force a reload of the postgres.conf file.
You can also search in JIRAs for record of the same error message to see if this same issue has already been found and fixed. It may be that the customer needs to upgrade their binary to a new version with the fix in place. See the next question [How to create a hot patch for a customer?](Storage%2BTroubleshooting%2BNotes.md) for more information.

###### **How to upgrade a customer's binary and/or how to create a hot patch for a customer?**

It may be that the customer needs to upgrade their binary to a new version with the fix in place. Be careful to compare the current version number of the customer with the version with the fix and be sure to understand any upgrade issues before attempting an upgrade (e.g. catalog changes may occur between major releases such as from 4.1.X to 4.2.X but not within a 4.Y family of binaries.) Failure to follow the correct upgrade procedures can mess up a customer's system so badly that it must be deleted. Support has details on upgrade procedures, but must be sure to follow them precisely.
If you cannot do a full upgrade, you may be able to make a custom build for the customer. Start with the source code branch for the version they are using and try to cherry pick just the fix needed for the current issue into that branch. If there are no conflicts on the merge, that is a good sign that the hot patch will be relatively straight forward. Once you have a hot patch for them, ftp the binary and reattempt gpstart.

It is worth noting that there are some important changes in persistent tables between 4.0.X and 4.1.X. The code to handle rebuilding of persistency tables was checked into 4.0.5.0 initially. This was accomplished by overloading other functions. Full functionality was in 4.0.5.4.

Diagnose and Recover
====================

The following is a procedure to analyze and correct common issues. Every situation is unique, so some of these steps may not be applicable.

Keep in mind that many issues have been fixed, and it may be that the user simply has to upgrade to the release with the fix, so don't do anything destructive if you don't have too. Look through JIRA for similar issues, and see if a simple binary swap with a newer release would fix the problem.

-   Always take a database system backup if possible before making any changes. If the user doesn't have a recent backup, have them take a backup if they have the space and the system is in a state that will allow a backup. If the system is not recoverable, then it might be best to simply re-initialize the system and load the backup. 
-   Talk to the user about the risks and time involved in fixing the system. Taking a backup and reinitializing the system may be less risky, but it may make the system unavailable for a longer period of time. Talk to the user about which approach they prefer. 
-   For most of these procedures, there should be **\*no I/O activity\*** running in the system. The customer must understand the importance of preventing any activity from creeping in while you are trying to restore the system.

###### What are some good questions to ask before doing anything (i.e. Look don't touch phase of recovery)?

It is wise to have a good sense for the customer's situation.

-   How much data is in the system?
-   What kind of data?
-   How old is the system?
-   Does the customer have a backup?
-   What are the implications for the customer of system downtime?
-   What would be the impact of data loss?

**At each point in the repair process, attempt to identify the area of the code responsible for the problem and pull in the right people to help.** It is not uncommon for one error to cause further errors. Also not uncommon for repair to be a multi-team effort as each part of the system is cleaned up.

**Understand the layout/configuration of the system.**

-   Where is the master?
-   Where are all the segments?
-   Where are errors being reported?
-   See [How to get configuration information?](Storage%2BTroubleshooting%2BNotes.md) and [How to investigate a specific error message?](Storage%2BTroubleshooting%2BNotes.md) for more information.

If you are able to identify a specific error message, consult the database of JIRAs and the developers of that area of the code to determine if a fix for the issue has already been found. See [How to investigate a specific error message?](Storage%2BTroubleshooting%2BNotes.md) and [How to upgrade a customer's binary and/or how to create a hot patch for a customer?](Storage%2BTroubleshooting%2BNotes.md) for more information.

Before you move from "look , don't touch" to active repair, make sure there is no activity in the system. Make sure the customer understands that no I/O activity can be allowed to creep in during repair (even if you are able to bring the DB up during the recovery it may **not** be safe for activity).

If you do change anything at all to get the DB started again, it is very wise to check the consistency of the DB before returning it to production use. See instructions in section below under [Are there issues with database consistency?](Storage%2BTroubleshooting%2BNotes.md) ."Once you are able to start the DB again, you should always take time to check the consistency of the database".

###### Is the system down and will not start?

1.  If the system is up and starts then the answer to the question is NO and so proceed directly to [Try to figure out what kind of issue it is](Storage%2BTroubleshooting%2BNotes.md).
2.  Try starting the master in master only mode (i.e. gpstart --m). If it starts, you can get configuration and status information (see [How to get system configuration when system will not start?](Storage%2BTroubleshooting%2BNotes.md)).
3.  Startup may not have gotten far enough to do much. Take a look at the gpAdminLogs. There may have been problems with the startup scripts.
4.  Proceed to [Try to figure out what kind of issue it is](Storage%2BTroubleshooting%2BNotes.md).

###### Try to figure out what kind of issue it is

1.  Is it a backup or restore issue? If so, go to step [Is it a backup or restore issue?](Storage%2BTroubleshooting%2BNotes.md)
2.  Is the system hanging? If so, go to step [Is it a hang issue?](Storage%2BTroubleshooting%2BNotes.md)
3.  Look at the logs. Start with the master log. Search for strings like "PANIC" and "Traceback". These logs may show that there is an issue with a specific segment or with the master itself. If so, take a look at that segment's logs. Is there an way to search through logs on all segments at once? If not that seems like a very useful troubleshooting tool.
4.  Look at the internal code location referenced in the failure. There may be GUCs that can be used to provide more logging information. Set the appropriate GUCs in the postgres.conf file on the appropriate segment. If the system is already up, run "gpstart --u" to reload the postgres.conf file, else start the system normally (e.g. gpstart)., and try to start the system.
5.  Does it look like a port issue.issue? If so, go to [Is it a port already in use issue?](Storage%2BTroubleshooting%2BNotes.md)
6.  Does it look like a filerep or mirror startup issue? If so, go to step [Is it a filerep issue?](Storage%2BTroubleshooting%2BNotes.md)
7.  Look at the applicable logs. Does the problem look like an xlog issueI removed Jeanna's text here because you don't want to remove the xlog. You may want to reset it, and must use the procedure in the appropriate step.? (Note: if the database is not able to start, then removing the xlog on one or more segments should allow it to start (it basically makes the database start process very simple because no recovery is required) BUT removing the xlog is very dangerous because it can leave the DB in an inconsistent state. If you have tried everything else and still can't get the DB to start then you may need to try removing the xlog.) If so, go to [Is it an xlog issue?](Storage%2BTroubleshooting%2BNotes.md)
8.  Is the system going into changetracking because a primary or mirror is not responding to FTS? If so, go to [Is it an FTS issue?](Storage%2BTroubleshooting%2BNotes.md).
9.  Inconsistency in catalog tables can cause all sorts of issues. Go to [Are there issues with database inconsistency?](Storage%2BTroubleshooting%2BNotes.md)
10. Is the primary corrupt, but the mirror is good. If so, go to [Is the current primary bad, but the mirror looks good?](Storage%2BTroubleshooting%2BNotes.md).
11. Is there enough disk space on all the systems? If not customer must make additional space available.
12. If you got this far, it may have nothing to do with storage. Get the appropriate people involved.

###### Is it a backup or restore issue?

1.  Known storage issues with backup and restore have to do with duplicate entries in some catalog tables (see MPP-15504 and related issues).
2.  If yes, get the catalog people involved (Gavin and Caleb).

###### Is it a port already in use issue?

1.  Sometimes different system processes can attempt to open the same port. Search the logs for "Already in use".
2.  Do a "netstat -anp | grep &lt;port number&gt;" to get information on who is using the port.
3.  If a filerep process is using the port, try killing it.

The port range used by kernel can be defined by setting **net.ipv4.ip\_local\_port\_range** parameter. 'Port already in use' issue can be addressed by configuring **net.ipv4.ip\_local\_port\_range** range that is not overlapped with ‘port’ and ‘replication\_port’ in 'gp\_segment\_configuration'.

**Steps for changing "net.ipv4.ip\_local\_port\_range" parameter**

-   Edit the /etc/sysctl.conf file by changing the following line.

net.ipv4.ip\_local\_port\_range = 1025 65535

-   Restart network for change to take effect. Command is

/etc/rc.d/init.d/network restart

**Recommendation**

It is recommended that ‘port’ and ‘replication\_port’ in 'gp\_segment\_configuration' are configured either close to lower or upper range for **net.ipv4.ip\_local\_port\_range**. The **net.ipv4.ip\_local\_port\_range** range should be as large as possible.

###### Is it a filerep issue?

1.  Make sure you system is in a consistent state (i.e. run gpcheckcat) before running gprecoverseg.
2.  Sometimes there is a hang issue when filerep will not go into sync [Is it a hang issue?](Storage%2BTroubleshooting%2BNotes.md).
3.  Sometimes the user did an incremental gprecoverseg, but the system needed to have a gprecoverseg --F (i.e. full). Search the log files for "WARNING" and for "run gprecoverseg -F (full copy) to re-establish mirror connectivity".

###### Is it a hang issue?

Figure out what process is hanging. Sometimes it is a filerep transition issue. Do a "ps --ef | grep "transition" on the affected host. Do a pstack on the associated processes. Sometimes it is a lock issue. Try to find the processes on the systems that are hanging and do a pstack. Look for an entry for "LWLockAcquire" is the most likely place. If it is in the stack, then it is likely a hang do to a LW lock issue. ". Need some intuition about where each type of process is most likely to be waiting in order to spot the "wrong" places for it be hung up. For example, query dispatcher should spend most of its time just waiting. Killing the hanging process will often fix the issue. The system may be able to continue or recover on its own once the hung process is killed.

1.  One reason for a hang can be a bad transition to change tracking. Use "ps-efl | grep transition" to find a transition process. See the port number of this process. See which segment has the problem and look at the processes on that segment. If you still see a sender/receiver process then transition to change tracking is not working.
2.  Another reason for a hang can be that change tracking recovery is taking a long time.
    1.  Refer to [How to get state information directly from a segment?](Storage%2BTroubleshooting%2BNotes.md). This will show things like the dataState, the segment State and the fault Type. If it is in a fault then it is waiting for the transition to change tracking to happen.
    2.  Go on to master to see what fdiskprober is doing.

3.  Another reason for hang is waiting on the mirror lock. Check on a segment. It is the resync manager process that drives change tracking. Check the stack for that process and see what it is waiting on. Try killing whatever it is waiting on ( backend process, filerep process,..). Don't just kill everything.
4.  Another reason for a hang in change tracking is that the ports needed by the servers are hardcoded high number ports. If that port is already in use by a client, then change tracking will hang. Use netstat and try to find the filerep process that is using that port and kill just that process. Then run gprecoverseg again. (Master could know all the hardcoded ports needed for change tracking and could send to each segment the ports that it should not use. If OS hands one of those out as an ephemeral port then just close that socket and open another.)

###### Is it an xlog issue?

1.  Sometimes the xlog can become corrupt or out of sync. Take a look at the where the error is in the code. Sometimes there is a GUC, which will allow startup to skip the error and continue.
2.  Sometimes the xlog must be reset. This is a VERY dangerous thing to do and should never be done unless you have convinced yourself that turning on a guc or upgrading the database binary will not help resolve the issue. An xlog resultreset will remove all records since the previous checkpoint so that no recovery is attempted. The DB will just come online but it will likely be in an inconsistent state. and the database catalog tables will have to be manually cleaned up to make the system consistent again. Sometimes only one segment is having trouble replaying its xlog and it can vary how much activity is recorded in the log. Basically the more activity in the log and the more segments with problems, the more likely there will be big inconsistencies to resolve. Definitely worth "looking before you leap" to estimate the extent of the problem. If you must reset the xlog on multiple segments, you really should not attempt to script or automate the process. It is important to use gpcheckcat after each log reset and examine any errors.
3.  To reset the xlog, login to the host that has the segment. Run "pg\_resetxlog -f &lt;segment's data directory&gt;".
4.  After resetting the xlog, you will need to make sure the pg\_control file has the correct information. To view the pg\_control file, do a "pg\_controldata &lt;segment's data directory&gt;. The "Latest checkpoint's UNDO location" must be "0/0". The "Latest checkpoint's REDO location" must be the same as the "Latest checkpoint location". If the values need to be changed, talk to L3 support (Lubo and Deepesh).
5.  Use gpcheckcat to ensure that primary segments and catalog are in a consistent state.
6.  After running pg\_xlogreset, the database must be made consistent again. Go to [Are there issues with database consistency?](Storage%2BTroubleshooting%2BNotes.md).

###### Is it an FTS issue?

1.  Search the log files for "FTS", "postmaster reset". Common problems are mirror constantly lagging behind the primary, the system keeps transitioning to change tracking or there is a postmaster reset. Consult the segment logs on both primary and mirror for any segment that is having trouble.
    1.  Look at the panic that occurs right before the FTS or postmaster reset message. The message just before the transition will tell what caused the transition.

2.  On a large busy system, it could simply be the timeout value for the FTS prober areprober is too short. The log may contain a message such as,
3.  ""threshold '75' percent of 'gp\_segment\_connect\_timeout=600 is reached, mirror may not be able to keep up with primary, primary may transition to change tracking", increase guc 'gp\_segment\_connect\_timeout' by 'gpconfig' and 'gpstop -u'"
4.  The timeout value is controlled by a GUC "gp\_fts\_probe\_timeout".
    1.  By default if primary doesn't get response in 10 minutes then it will go into change tracking. This can occur if the mirror is very busy, if there asymmetric usage of system resources or if the file system on the mirror is much slower than on the primary.
    2.  Consider increasing the timeout value from 10 to 20 minutes.

###### Are there issues with database consistency?

1.  Once you are able to start the database again, it is important to check it for consistency before allowing I/O activity to restart.
2.  Run gpcheckcat (remember no I/O activity).
3.  Repair the system catalog. This is a job for the catalog folks (Gavin and Caleb).
4.  Run gpcheckcat.
5.  Repair the persistent tables if needed (see [How to Rebuild the Persistent Tables](Storage%2BTroubleshooting%2BNotes.md)). Note that if the system catalogs are repaired correctly then the persistent table can always be restored correctly if the proper procedure is followed.
6.  Run gpcheckcat.
7.  Remove any orphaned files. Lubo and Deepesh can help with scripts to remove orphaned files. If you don't remove orphaned files, it shouldn't impact the consistency of the DB. It will just take up disk space unnecessarily and could confuse later manual attempts to investigate/restore DB consistency.
8.  Run gpcheckcat.
9.  If there are issues, go to \#1 and repeat.
10. Run gprecoverseg.
    1.  Read the hint messages carefully. It will often say whether a full or incremental gprecoverseg is needed. Incremental will do a resync to validate that all files on the mirror should be there, while full will not.

###### Is the current primary bad, but the mirror looks good?

If you have determined that the primary is bad, but the mirror looks good, you can force a failover to the mirror. For example, say the user deleted one of their file space directories. The primary would stay up, but the filespace would not be available. Assuming the mirror is intact, and you are sure it is good, do the following:

1.  Fail the primary over to the mirror (see [How to force a primary to fail over to its mirror?](Storage%2BTroubleshooting%2BNotes.md))
2.  Verify the segment has failed over to its mirror (gpstate --s).
3.  Run gprecoverseg. *Wait until the segment's primary and mirror are in sync.*
4.  Rebalance the system (run gprecoverseg --r).

###### Is the system up but in change tracking?

Run gprecoverseg.

###### Does a primary need to be restored from its mirror?

If you suspect that a primary has been corrupted, you can attempt to recover from its mirror. If the same issue does not appear on the mirror, then consider using the mirror.
Manually change gp\_segment\_configuration.

How to Rebuild the Persistent Tables
====================================

The persistent tables are re-built using the information in other catalog tables. This is a VERY DESTRUCTIVE process. Be absolutely sure your catalog tables are correct before attempting to rebuild the persistent tables (i.e. gpcheckcat says the catalog is ok).

The system must be version 4.0.5.4 or later. If the system is older, the system binaries must be updated to the appropriate newer release.

There are four persistent tables on all segments and master,

``` theme:
gp_persistent_tablespace_node
gp_persistent_database_node
gp_persistent_relation_node
gp_persistent_filespace_node
```

and a system catalog table that contains mapping between system catalog and persistent tables. It is located on all segments and master.

``` theme:
gp_relation_node
```

The "gp\_persistent\_filespace\_node" table is rarely broken since not that many filespaces are created/dropped. If it is broken, the table must be modified manually via psql. It is not rebuilt using the below procedure because system catalog table "pg\_filespace" exists on master only.

First, make sure nothing is happening on the system (this is very important and the customer must understand this). Connect to the master, and do a:

``` theme:
SELECT * FROM pg_stat_activity;
```

Make sure the segment is up and in sync with its mirror.

###### ***What if part of the problem is that mirror will not come up?***

Connect to the appropriate segment in utility mode, and enable catalog table modification (see [How to connect to the master or a segment in utility mode?](Storage%2BTroubleshooting%2BNotes.md)). It is only necessary to connect to a single database (e.g. template1). There is a single set of persistent tables for all databases on a segment.

The procedure at this point differs between a 4.0.x system and 4.1.x or later systems.

**<span style="text-decoration: underline;">Procedure for</span>** **<span style="text-decoration: underline;">4.0.x systems.</span>**
The ability to rebuild the persistent tables was added in 4.0.5.4. Adding a built-in function would normally require a catalog change. To get around this, a new GUC was added.

**gp\_persistent\_build\_db\_overload\_string**

``` theme:
Valid values:
DEFAULT   -- No action
RESET_ALL -- Remove all entries in the persistent tables.
BUILD_ALL -- Re-build the persistent tables.
```

The function "gp\_persistent\_build\_db()", was enhanced to recognize the new GUC and take appropriate action.
If the segment is the **master**, do the following.

1.  Setting GUC 'gp\_persistent\_build\_db\_overload\_string=RESET\_ALL'
2.  Removing all entries from persistent tables by doing a 'select \* from gp\_persistent\_build\_db(true);'
3.  Setting GUC 'gp\_persistent\_build\_db\_overload\_string=BUILD\_ALL'.
4.  Re-building persistent tables by doing a 'select \* from gp\_persistent\_build\_db(false);'
5.  Run gpcheckcat

The gpcheckcat utility should report no persistent tables inconsistencies, but it will most likely report some file system inconsistencies.
If the segment is **not the master**, make sure the segment is in sync with its mirror, and do the following:

1.  Setting GUC 'gp\_persistent\_build\_db\_overload\_string=RESET\_ALL
2.  Removing all entries from persistent tables by 'select \* from gp\_persistent\_build\_db(true);'
3.  Setting GUC 'gp\_persistent\_build\_db\_overload\_string=BUILD\_ALL'
4.  Re-building persistent tables by 'select \* from gp\_persistent\_build\_db(true);
5.  Setting GUC 'gp\_persistent\_build\_db\_overload\_string=DEFAULT'
6.  Run gpcheckcat

The gpcheckcat utility should report no persistent tables inconsistencies, but it will most likely report some file system inconsistencies.

**<span style="text-decoration: underline;">Procedure for 4.1.x or later systems.</span>**
Two new functions were added in the 4.1.x release to allow rebuilding of the persistent tables.

``` theme:
gp_persistent_reset_all() - Remove all entries from the persistent tables.
gp_persistent_build_all() - Rebuild the persistent tables.
```

Functions can be run via "select \* from function();".

If the segment is the **master**, do the following.

1.  Removing all entries from persistent tables by doing a 'select \* from gp\_persistent\_reset\_all();'
2.  Re-building persistent tables by doing a 'select \* from gp\_persistent\_build\_all(false);'
3.  Run gpcheckcat

The gpcheckcat utility should report no persistent tables inconsistencies, but it will most likely report some file system inconsistencies.
If the segment is **not the master**, make sure the segment is in sync with its mirror, and do the following:

1.  Removing all entries from persistent tables by doing a 'select \* from gp\_persistent\_reset\_all();'
2.  Re-building persistent tables by doing a 'select \* from gp\_persistent\_build\_all(true);'
3.  Run gpcheckcat

The gpcheckcat utility should report no persistent tables inconsistencies, but it will most likely report some file system inconsistencies. The reported files needs to be removed from the file system manually.
If this needs to be done on multiple segments, do several manually before scripting the rest.

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [ProposedDesign-PTRebuildutility.docx](attachments/42502284/73990158.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)

Comments:
---------

<table>
<colgroup>
<col width="100%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><a href=""></a>
<p>Migrated: <a href="https://support.pivotal.io/hc/en-us/articles/206536557" class="uri" class="external-link">https://support.pivotal.io/hc/en-us/articles/206536557</a></p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by bstephens at Jun 17, 2015 10:55
</div></td>
</tr>
</tbody>
</table>

Document generated by Confluence on May 17, 2016 19:14


