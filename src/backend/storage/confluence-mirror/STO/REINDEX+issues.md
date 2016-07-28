<span id="title-text"> Storage & Access : REINDEX issues </span>
================================================================

This page last changed on Jan 16, 2013 by alavar.

**<span style="text-decoration: underline; ">Deadlock involving gp\_fastsequence ( fixed in private branch )</span>**

**              ** Each segment maintains its own gp\_fastsequence table. When we insert data into an AO/CO table, on every segment we assign new row numbers from gp\_fastsequence and as a result gp\_fastsequence is updated. This is done per segment. After all the inserts are done on the segments, we need to update the segment EOFs on the master. On the very first update on master, we create an entry for a particular segment in pg\_aoseg and take a lock on the gp\_fastsequence on the master. This sequence of taking the lock on gp\_fastsequence on the segments first and then on the master is a potential deadlock causing sequence because when we try to reindex the gp\_fastsequence table, we first lock it on the master and then try to acquire locks on the segments.To avoid any deadlocks, I release the lock on segments as soon as the access is done. So that it doesn't conflict with a concurrent reindex operation.

** **

**<span style="text-decoration: underline; "> </span>**

**<span style="text-decoration: underline; ">2) Deadlock because of a dummy lock.</span>**

This was seen when I was testing on Jeff's private branch. It looks as if one of the processes is holding a lock in 2PC

I had changed the reindex code to take a lock on pg\_class at the beginning of reindex as we would run into deadlocks because a reindex operation might first scan pg\_class and then much later will add an entry about the new index thereby taking a row exclusive lock.

alavar=\# select \* from pg\_locks where relation=2662 and database=16993;
 locktype | database | relation | page | tuple | transactionid | classid | objid | objsubid | transaction | pid  |        mode         | granted | mppsessionid | mppiswriter | gp\_segment\_id
--<span style="text-decoration: line-through; ">-</span>}<span style="text-decoration: line-through; ">--</span>}<span style="text-decoration: line-through; ">}</span>

<span class="error">Unknown macro: {--}</span>

<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">-</span>}<span style="text-decoration: line-through; ">{</span>}{<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">{</span>--<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">}</span>-<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">-</span>-<span style="text-decoration: line-through; ">-</span>-----
 relation |    16993 |     2662 |      |       |               |         |       |          |        8841  | 1259 | AccessExclusiveLock  | f       |          175 | t           |            -1
 relation |    16993 |     2662 |      |       |               |         |       |          |           0     |          |  AccessShareLock      | t       |            0    | f           |            -1

-

**3) Current issues**

Currently I was running into a few deadlocks which involve pg\_class and pg\_class\_oid\_index as they are accessed in various ways . So to fix these I had made the change to acquire the required lock on pg\_class as soon as reindex starts. Saw issue 2 because after that.

A few log messages of the form

2012-12-19 11:21:48.855517 PST,"rajesh2","gptest",p32556,th-1854449056,"192.168.1.3","64630",2012-12-19 11:21:48 PST,9376,con186,,seg-1,,,x9376,sx1,"LOG","00000","lock AccessShareLock on object 16993/2701/0 is already held",,,,,,,0,,"lock.c",900,
2012-12-19 11:21:48.855538 PST,"rajesh2","gptest",p32556,th-1854449056,"192.168.1.3","64630",2012-12-19 11:21:48 PST,9376,con186,,seg-1,,,x9376,sx1,"LOG","00000","writer found lock AccessShareLock on object 16993/2701/0 that it didn't know it held",,,,,,,0,,"lock.c",906,
2012-12-19 11:21:48.855786 PST,"rajesh2","gptest",p32556,th-1854449056,"192.168.1.3","64630",2012-12-19 11:21:48 PST,9376,con186,,seg-1,,,x9376,sx1,"LOG","00000","lock AccessShareLock on object 0/2677/0 is already held",,,,,,,0,,"lock.c",900,
2012-12-19 11:21:48.855802 PST,"rajesh2","gptest",p32556,th-1854449056,"192.168.1.3","64630",2012-12-19 11:21:48 PST,9376,con186,,seg-1,,,x9376,sx1,"LOG","00000","writer found lock AccessShareLock on object 0/2677/0 that it didn't know it held",,,,,,,0,,"lock.c",906,

**4) Reader gang lock issue**

<a href="http://jira.eng.pivotal.io/browse/MPP-18974" class="uri" class="external-link">http://jira.eng.pivotal.io/browse/MPP-18974</a>

REINDEX (and potentially other operations such like VACUUM) acquires locks on catalog table that conflict even with AccessShareLock.  The read locks are taken by writers and readers in normal queries including DML.  In INSERT ... VALUES for example, the VALUES scan is executed on one of the segments as a reader by direct dispatch and the segment also has a writer for INSERT part.  If REINDEX has acquired an AccessExlusiveLock on pg\_class, the writer tries to acquire AccessShareLock and waits, in the mean time the reader also tries to acquire it and finds the writer already exists.  The reader tries to escalate the writer's lock to the higher level somehow, but fails because the writer hasn't actually acquired the lock.  The right behavior I imagine is for the reader to wait along with the writer, but it actually fails with bizarre "session deadlock" error.  It occurs because the catalog read doesn't hold long lock on catalog tables when it's used to just construct Relation for example.

5) **Deadlocks involving pg\_class and its indices**

REINDEX table operation takes a share lock ( concurrent reads are allowed ) on the base table and exclusive lock on the indices. This could be a problem with pg\_class and its indices.

Assume Transaction 1 is trying to read pg\_namespace\_index and then pg\_class\_oid\_index. So it will acquire a share lock on pg\_class first, then sharelock on pg\_namespace index and finally on pg\_class\_oid\_index.

Now in Transaction 2,  when we reindex pg\_class, we first take ShareLock on pg\_class it does not conflict with the read lock in Transaction 1. If we reindex pg\_class\_oid index before it is accessed in Transaction 1,

then we reach a deadlock situation.

I am currently taking an EXCLUSIVE LOCK on pg\_class whenever we are trying to reindex pg\_class

**6) New issue with gp\_fastsequence**

    In PersistentRelation\_MarkAbortingCreate() we try to update gp\_fastsequence and take a Row-ex lock.

    We do an index scan and then release the lock only on the base table. We then release the lock on base table.

    If we do multiple create tables in a transaction and abort the transaction, then we take and relase the lock

    on gp\_fastsequence multiple times, but we dont release the lock on its index.

    If a reindex operation comes in between the two PersistentRelation\_MarkAbortingCreate() calls in a single transaction, then we hit a deadlock.

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [ReindexIssues.docx](attachments/50004186/50167846.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)

Document generated by Confluence on May 17, 2016 19:14


