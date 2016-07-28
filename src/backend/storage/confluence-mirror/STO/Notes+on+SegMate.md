<span id="title-text"> Storage & Access : Notes on SegMate </span>
==================================================================

This page last changed on Mar 18, 2012 by paramr3.

###### Author: Matt McCline

Version: 0.1 Created: November 1<sup>st</sup>, 2011

Introduction
============

Today we have a shared-memory data structure SharedSnapshotSlot (tqual.h) that shares session and transaction information among session's gang processes on a particular database instance. The processes are called a SegMate process group. *SegMate is a term coined by Tim Kordas.*

A **SegMate process group** is a QE (Query Executor) Writer process and 0, 1 or more QE Reader processes. Greenplum needed to invent a SegMate sharing mechanism because in Postgress there is only 1 backend and most needed information is simply available in private memory. With Greenplum session parallelism on database instances, we need to have a way to share not-yet-committed session information among the SegMates. This information includes transaction snapshots, sub-transaction status, so-called combo-cid mapping, etc.

**An example:** the QE readers need to use the same snapshot and command number information as the QE writer so they see the current data written by the QE writer. During a transaction, the QE Writer writes new data into the shared-memory buffered cache. Later in that same transaction, QE Readers will need to recognize which tuples in the shared-memory buffered cache are for its session's transaction to perform correctly.

**Another example:** the QE readers need to know which sub-transactions are active or committed for a session's transaction so they can properly read sub-transaction data written by the QE writer for the transaction.

So, the theme is to share private, not-yet-committed session transaction information with the QE Readers so the SegMate process group can all work on the transaction correctly.
*\[We mostly think of QE Writers/Readers being on the segments. However, masters have special purpose QE Reader called the Entry DB Singleton. So, the SegMate module also works on the master.\]*

Issues
======

Today's SharedSnapshotSlot shared-memory data structure has had issues for many years (5 years). It altogether lacks module boundaries and crisp protocol and semantic definitions.

SharedSnapshotAdd Collision on Backend Create
---------------------------------------------

There are many, many JIRA on this issue.

**For example**:<a href="http://jira.eng.pivotal.io/browse/MPP-1716" class="external-link">MPP-1716</a> (2007): **Asurion POC 20-user concurrency fails with ERROR: Out of Memory and FATAL: SharedSnapshotAdd: writer gang collision**

The SharedSnapshotSlot is allocated when a backend QE Writer is created. When more than one QE Writer on a segment tries to allocate a SharedSnapshotSlot with the same session number, this error occurs.

In the past, this error could occur when the Master received a retry-able error creating a Segment QE Writer as part of creating gangs. When a QE Writer is created, it allocates the SharedSnapshotSlot and then goes to the top of PostgresMain procedure to wait for a command. It may take time for a disconnected QE Writer to detect its connection back to the Master has failed and exit. Exiting will cause the SharedSnapshotSlot to be deallocated. This opens a window for the retry QE Writer create to come in and fail because the SharedSnapshotSlot is still in-use.

A partial solution was implemented that has a QE Writer, which discovers a SharedSnapshotSlot collision to delay and retry for a while. This mostly works, but is not a complete solution for heavily loaded systems. See the SharedSnapshotAdd procedure in tqual.c source file. It delays 0.1 seconds for a maximum of 10 times (1 second total).

A better solution might have been to add a retry sequence number to the backend QE Writer create message. This would allow a new retry create to attach to an existing SharedSnapshotSlot with out error and without delay. We would enforce the "taking over" the SharedSnapshotSlot by adding a new SegMate module and checking the sequence number at the beginning of each module interface call. In theory an abandoned backend shouldn't call the interface again because it will not have received any commands.

SharedSnapshotAdd Collision with Zombies on Backend <span style="text-decoration: underline;">Re-Create</span>
--------------------------------------------------------------------------------------------------------------

The current solution for QE Writer collision issue, which discovers a SharedSnapshotSlot collision to delay and retry for a while, the SharedSnapshot Timeout turned out to be partial for another very important reason. When a gang connection from the Master to a Segment is lost due to network issues or Segment backend process failure while executing a command, the Master would disconnect the gang and re-create the gang. However, the disconnect is one-sided. When the Master disconnects it does not wait for acknowledgement from the Segment that the backend has exited. This has advantages because the Master doesn't get hung waiting for Segment acknowledgment. However, we encounter the problem that the zombie Segment backend might still be busy doing work on behalf of the Master command that failed and has not noticed yet. It is a zombie because it is dead and doesn't know it yet. That is, it is still actively using the SharedSnapshotSlot.

So, the solution here was to get a new session number before re-creating a gang. This dodges the collision altogether for re-create.

SharedSnapshot Cursor Problems
------------------------------

Cursors are funny case because they read through a snapshot taken when the create cursor command was executed, not through the current snapshot. Originally, the SharedSnapshotSlot was designed for just the current command. The default transaction isolation mode is READ COMMITTED, which cause a new snapshot to be created each command. Each command in an explicit transaction started with BEGIN and completed with COMMIT, etc. So, cursors would read through the current snapshot instead of the create cursor snapshot and see data they shouldn't see.
The problem turns out to be a little more subtle because of the existence of QE Readers and the fact that QE Readers can be created later – long after the create cursor command.

So, the solution was to serialized the current snapshot to a temporary file during create cursor so that subsequently created QE Readers could get the right snapshot to use from the temporary table and ignore the SharedSnapshotSlot.

SharedSnapshot Timeout during Cancel
------------------------------------

A whole new area of discussion is how transaction snapshot information in the SharedSnapshotSlot advances during commands for proper update data execution.
The formal transaction snapshot is acquired at the beginning of each command when isolation level READ COMMITTED (the default) is used, or when the first command is executed in an explicit transaction when the isolation level is SERIALIZABLE.

However, there is another very important part of transaction snapshot during execution called the command number. The command number allows the executor to read data it has written itself in earlier parts of a command but not to read data it is currently writing. This is used to prevent variations on the so-called Halloween problem Discovered by IBM on Halloween, 1976.

See <a href="http://en.wikipedia.org/wiki/Halloween_Problem" class="uri" class="external-link">http://en.wikipedia.org/wiki/Halloween_Problem</a> where a command will update tuple during a scan and then see the new updated tuples later in the same scan and try to update them. An infinite loop occurs and multiply updating isn't the sematic desired anyway. The challenge for Greenplum is advancing the command number across all the QE Writers and QE Readers in a consistent way.
So, at the beginning of each command, QE Readers wait for the QE Writer to advance the command number. If a command cancel is issued during this time, the QE Writer might get canceled before it updates the command number and then a QE Reader might not see the cancel it is at the top of PostgreMain reading the next command and they end up timing out waiting for the command number update. The timeout shows up as an error in the system log and gets reported by nervous customers.

Solution to be determined…

Sub-Transaction Limit
---------------------

QE Readers need to know which sub-transactions the QE Writer has committed and which are active so QE Readers can see the right data. While a sub-transaction may be committed in an active parent transaction, that data is not formally committed until the parent commits. And, active sub-transactions are not even sub-transaction committed yet. So, other transactions cannot see active or committed sub-transaction work yet.

Without adding special logic to a QE Reader, it would be considered another transaction and not see the committed or active sub-transaction work. This is because QE Readers do not start their own transaction. We just set a few variables in the xact.c module to fake making it look like there is a current transaction, including which sub-transactions are active or committed. This is a kludge.
In order for the QE Reader to fake being part of the QE Writer transaction, we put the current transaction id and the values of all active and committed sub-transaction ids into the SharedSnapshotSlot shared-memory structure. Since shared-memory is not dynamic, we have an arbitrary limit on the number of sub-transaction ids we keep in the SharedSnapshotSlot. This limits the number of sub-transactions that can be executed – which is a terrible limit. There should be not practical limit to the number of sub-transactions.

See how the TransactionIdIsCurrentTransactionId procedure in xact.c checks to see if the backend executing is a QE Reader (or Entry DB Singleton), and if it is, walk through the sub-transaction ids in SharedSnapshotSlot. One thing to observe is that sub-transactions change in state in finite numbers during sub-transaction creates, sub-transaction abort commands, or abort of regular commands that abort the current sub-transaction. Couldn't we have the last QE Writer command put the sub-transaction state change into the SharedSnapshotSlot at the end of the command and have the QE Reader just read the change during the next command and apply it to its private memory? That is, keep the potentially large sub-transaction information cached in the QE Reader and only keep the finite changes in the SharedSnapshotSlot?

The answer is YES and NO. Normally, YES we could keep just the last change in the SharedSnapshotSlot. However, we need to keep the whole state saved somewhere for two very important reasons:

1.  not every QE Reader is involved in the next command and may miss out on changes, and
2.  QE Readers can be created on demand by complex commands at any time.

So, a complete solution might be to put the recent changes into the SharedSnapshotSlot so QE Readers can efficiently stay current and also write behind old changes to a temporary file so behind or new QE Readers can pull the whole sub-transaction status into their private memory.

Document generated by Confluence on May 17, 2016 19:14


