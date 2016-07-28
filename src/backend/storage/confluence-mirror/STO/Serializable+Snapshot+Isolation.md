<span id="title-text"> Storage & Access : Serializable Snapshot Isolation </span>
=================================================================================

This page last changed on Jun 12, 2012 by meistd.

Serializable Snapshot Isolation
===============================

**Snapshot Isolation is not “real” serializable consistency.**

Example: “Write Skew”
---------------------

Two interleaved transactions. There is the basic table t:

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>ID</p></th>
<th align="left"><p>COLOR</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>1</p></td>
<td align="left"><p>white</p></td>
</tr>
<tr class="even">
<td align="left"><p>2</p></td>
<td align="left"><p>black</p></td>
</tr>
<tr class="odd">
<td align="left"><p>3</p></td>
<td align="left"><p>white</p></td>
</tr>
<tr class="even">
<td align="left"><p>4</p></td>
<td align="left"><p>black</p></td>
</tr>
</tbody>
</table>

Operations
T1: BEGIN
T2: BEGIN
T1: UPDATE t SET color = 'black' WHERE color = 'white';
T2: UPDATE t SET color = 'white' WHERE color = 'black';
T1: COMMIT
T2: COMMIT

With current SI, the output would be

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>ID</p></th>
<th align="left"><p>COLOR</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>1</p></td>
<td align="left"><p>black</p></td>
</tr>
<tr class="even">
<td align="left"><p>2</p></td>
<td align="left"><p>white</p></td>
</tr>
<tr class="odd">
<td align="left"><p>3</p></td>
<td align="left"><p>black</p></td>
</tr>
<tr class="even">
<td align="left"><p>4</p></td>
<td align="left"><p>white</p></td>
</tr>
</tbody>
</table>

-   This is no valid output under any serializable schedule.
-   Classical “locking” based ways to realize serializable consistency are slow and highly limit concurrency.
-   Current serializable transaction mode is using snapshot isolation.

Why serializable?
-----------------

-   If your application is doing the right thing without other concurrent transaction, it is also guaranteed to do the right thing with concurrent transactions iff serializable consistency is used.
-   It is possible to check the application if it could cause violations, but this is hard in complex, agile applications.
-   Serializable snapshot isolation is a novel approach to achieve serializable consistency with snapshot MVCC based systems.
-   Based on 2008 PhD thesis of Cahill
-   Feature in Postgres 9.1

Basic idea:
-----------

-   The SSI works by tracking so called RW-dependencies together with WW and WR-dependencies.
-   “T1 produces a version of x, and T2 produces a later version of x (this is a ww-dependency)
-   T1 produces a version of x, and T2 reads this (or a later) version of x (this is a wr-dependency)
-   T1 reads a version of x, and T2 produces a later version of x (this is a rw-dependency, also known as an anti-dependency, and is the only case where T1 and T2 can run concurrently)” .
-   Thesis: When a schedule is not serializable then, there is a cycle in the dependency graph with two read-write consecutive dependencies.
-   The SSI algorithm tracks the RW dependencies and aborts a transaction when it finds two of these dependencies.

Implementation in Postgres 9.1:
-------------------------------

-   It does this by using a new lock mode called SIREAD, which is used to record the fact that an SI transaction has read a version of an item.
-   No additional blocking is introduced. SIREAD locks are more “flags” then locks.
-   At commit time, it is checked if the current transaction is part of an incoming rw- dependency and an outgoing rw-dependency. This transaction is then called a “pivot” transaction. This transaction is then aborted.

Important notes:
----------------

-   The algorithm produces false positives. It aborts transactions even when there is no serialization error.
-   It is not possible to maintain the complete read information for each tuple that has been read. The Postgres implementation falls back to courser locks if necessary to avoid memory exhaustion. Courser locks generally result in more false positives.
-   Multiple optimizations are done to avoid these false positives, but they are in the end an inherent fact in this approach.
-   Overhead against snapshot isolation is hard to estimate and there seems to be no clear picture
-   2% has been reported, 20% has been reported.

Can it work in GPDB?
--------------------

-   Currently GPDB allows only a single tuple transaction per relation (exclusive table locking). SSI will not be useful in this setting. However, it is a long term plan to drop that restriction. The further discussion assumes that multiple tuples are allowed per table.
-   There are multiple issues that can influence if the approach will work in GPDB and/or how much effort has to be invested to get with working.
    1.  WAL replay: It looks like that the WAL replay of Postgres might introduce violate serializable execution. It is not clear if this would also be true in GPDB. Our current guess is that this comes from the concurrent WAL replay of Postgres. Thus, this will not apply to GPDB. Currently, we don't see an issue here
    2.  Detecting of rw-conflicts. In certain distributed architectures, it is not possible to detect rw-conflicts only using local knowledge. This is e.g. the case when a hot standby is also used to answer (read-only) queries. In GPDB there is (currently) only a single segment that serves a tuple. Thus, rw-conflicts can be detected locally.
    3.  Detecting of consecutive rw-conflicts. This cannot be done only with local knowledge. Here is a counterexample: We have distributed transaction T1. T2, and T3. T1 has a rw-conflict with T2 on a tuple or page on segment 1 and T2 has a rw-confict with T3 on another tuple on segment 2. When the graph information is only checked locally this structure is not detected.

### What can we do?

It could be investigated if the "cycle detection" can be done on the master. The idea is that the segments report all relevant rw-conflicts to the master and the master then does the cycle detection and aborts a transaction if necessary. This could e.g. be done by sending a special query to all segments calling a new special function gp\_ssi\_rw\_conflicts.

#### Open Questions:

-   Is it enough to do the cycle detection during commit? The current code and the thesis also abort a transaction during read and write. Is that strictly necessary or would it be enough to mark the conflict. The thesis text doesn't answer the question. It only discusses that a certain check is needed during read and write
-   There is a race condition between the call of the special function on the segments and the check on the master. In the normal local check, the lock SerializableXactHashLock is held. This makes the check atomic. In a sense, that lock needs to be held on all segments while the master searches for cycles. So the process could be: Acquire SerializableXactHashLock on all segments, get rw conflict data from all segments, searches for the dangerous structure on master, unlock on all segments. However, this means three broadcasted, serialized roundtrips. This would significantly increase the length that the lock has to be held. 
-   A complete alternative would be to avoid the predicate locking on the segments at all. Any command could sent its predicate locks to the master (attention another race condition here) which maintains it in the in-memory structure (+ the SLRU list). Then the check would be a local atomic check. This would increase the memory consumption of the master.
-   Can we limit witch rw-conflicts are needed to be transferred to the master?

Links
-----

-   <a href="http://wiki.postgresql.org/wiki/Serializable" class="external-link">Postgres Wiki: Serializable</a>
-   <a href="http://doi.acm.org/10.1145/1376616.1376690" class="external-link">Michael J. Cahill, Uwe Röhm, and Alan D. Fekete. Serializable isolation for snapshot databases. SIGMOD’08</a>
-   <a href="http://hdl.handle.net/2123/5353" class="external-link">Michael J. Cahill. Serializable Isolation for Snapshot Databases. 2009, PhD Thesis</a>

Document generated by Confluence on May 17, 2016 19:14


