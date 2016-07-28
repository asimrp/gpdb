<span id="title-text"> Storage & Access : DDBoost Backup and Restore Enhancement </span>
========================================================================================

This page last changed on Jun 10, 2013 by apraveen.

An enhancement to current (as of 4.2.5) DDBoost backup/restore mechanism in GPDB is proposed here. From the point of view of GPDB, DDBoost is a mechanism for transferring backup files to/from Data Domain appliance. In a typical deployment, a Data Domain appliance is shared with multiple applications, including GPDB for backup / disaster recovery purposes.

Problem with current DDBoost backup/restore in GPDB
===================================================

A GPDB backup using `gpcrondump` generates one backup file on each GPDB segment (postgres). In case of DDBoost backup (`gpcrondump --ddboost`), the backup files on all segments are transferred to Data Domain appliance concurrently. The sequence of steps is:

1.  `gpcrondump --ddboost` on master node invokes `gp_dump` on master node. `gp_dump` spins off one thread for each segment.
2.  Each thread connects to a segment in utility mode and executes the following query:
    ``` theme:
    SELECT * FROM gp_backup_launch(...);
    ```

3.  The function `gp_backup_launch()` executes `gp_backup_lanch()` and `gpddboost` in a single command line. Output (stdout) of `gp_backup_launch()` is piped to stdin of `gpddboost`. Output of `gp_backup_launc()` is contents of the backup file on a segment.
4.  `gpddboost` connects to a pre-configured Data Domain appliance, creates a new file on Data Domain and writes the contents on its stdin to the file using DDBoost.

One file transfer using DDBoost (`ddp_open_file() ... ddp_close_file()`) consumes on I/O stream on the Data Domain appliance. A Data Domain appliance has limited number of I/O streams. The **soft limit** on read / write I/O streams is small enough to be crossed if all segments in a full rack DCA start transferring backup files to Data Domain using DDBoost. According to Data Domain, exceeding the I/O stream soft limit is "risky". Therefore, the soft limit must be adhered to in GPDB backup/restore. The following sections propose an enhancement to current GPDB backup/restore mechanism to this effect.

**Note**: Soft limit differs on different Data Domain hardware and software. Moreover, different kinds of I/O streams (read/write/replication) have different soft limits on a Data Domain.

Stream Counter Service
======================

We propose a new option `--max-streams <limit>` to `gpcrondump` and `gpdbrestore` scripts. E.g. Consider a GPDB instance with 96 primary segments.

``` theme:
gpcrondump -x <dbname> -z --ddboost --max-streams 50
```

The backup operation will use at the most 50 write streams on the configured Data Domain appliance. 46 segments will wait for a write stream to become available before writing their backups to Data Domain.

Note that soft limits on a Data Domain can be obtained using DDBoost API. Also, the number of in-use I/O streams on a Data Domain at any instant can be obtained using DDBoost API. But DDBoost API does not offer a mechanism to reserve an I/O stream. Therefore, we have to leave the choice of `--max-streams` value to the user.

The `--max-streams` limit is enforced with the help of a stream counter service, running on master node. The service takes the user-specified `--max-streams` value as input and keeps track of I/O streams used by GPDB backup or restore operation. `gpddboost` connects with stream counter service and checks if it can use an I/O stream before invoking `ddp_open_file()` API. If response from the service is positive, `gpddboost` utilizes one I/O stream by invoking `ddp_open_file()` API and subsequently reading from / writing to the file on Data Domain. Interaction between stream counter service and each `gppddboost` instance can be outlined as follows:

1.  Stream counter service is initialized with user specified `--max-streams` value.
2.  Each `gpddboost` instance gets hostname and port of the stream counter service as command line arguments.
3.  Before opening a file on Data Domain for reading or writing using `ddp_open_file()`, `gpddboost` connects to stream counter service and checks if it can use one I/O stream.
4.  Stream counter service simply examines its stream count to see if it has reached the `--max-streams` value. If no, it increments the stream counter and sends a positive response to `gpddboost`. Otherwise, a negative response is sent.
5.  Upon receiving a positive response, `gpddboost` invokes `ddp_open_file()` API and starts using one Data Domain I/O stream.
6.  If the response is negative, `gpddboost` waits for an interval of random length and tries again.
7.  After relinquishing an I/O stream using `ddp_close_file()` API, `gpddboost` connects with stream counter service and indicates that it has relinquished one I/O stream. The stream counter service then increments stream counter by one.

### Stream counter messages

``` theme:
typedef enum stream_counter_msg_type
{
        SC_REQ_RESERVE_READ,
        SC_REQ_RESERVE_WRITE,
        SC_REQ_RELEASE,
        SC_RESP_AVAILABLE,
        SC_RESP_ALL_BUSY,
        SC_RESP_RELEASED,
        SC_RESP_ERROR
} StreamCounterMsgType;

typedef struct stream_counter_msg
{
        StreamCounterMsgType type;
        int dbid;
} StreamCounterMsg;
```

All `SC_REQ_*` messages are requests made by `gpddboost` to stream counter service and `SC_RESP_*` are responses sent by stream counter service. `SC_REQ_RESERVE_READ` is sent by `gpddboost` before opening a file on Data Domain in read mode. This happens during restore operation. `SC_REQ_RESERVE_WRITE` is sent before opening a file in write mode, which is applicable to backup operation.

`dbid` is the content id of primary segment. It may be used to enforce basic validations such as a request to release stream `SC_REQ_RELEASE` must be preceded by a read or write request with the same `dbid`, i.e. a segment must reserve a stream before relinquishing one.

Performance
===========

Stream counter service runs on master node. The overhead incurred by the service is creating one thread and one TCP connection for each incoming request. Time scale of a backup/restore operation is assumed to be in hours. The performance gained by pre-creating a pool of threads to handle requests from `gpddboost` should not result in significant gain in such a timescale. If during initial testing, it's observed that a burst of connection requests at the start of a backup/restore operation is causing overflow in TCP queue of listen socket, a pool of threads may be created.

On segments, overhead incurred by the proposed enhancement is that of a new TCP connection for each `ddp_open_file()` and `ddb_close_file()` APIs. In the case of `SC_RESP_ALL_BUSY` response, the `gpddboost` process on the segment sleeps for a random amount of time. Recall that `gp_backup_launch()` function on each segment executes

``` theme:
gp_backup_agent ... | gpddboost --write-from-stdin ...
```

where output of `gp_backup_agent` is piped into `stdin` of `gppddboost`. When reader at one end of the pipe is sleeping (not ready), writer on the other end of the pipe should be blocked. This should result in no additional memory overhead / buffering. <a href="#" class="unresolved">this needs to be verified</a>.

Depending on the value of `--max-streams`, data transfers to/from Data Domain will be staggered by this enhancement. This invariably introduces performance degradation as compared to the current DDBoost backup/restore implementation. This should be ok if the risk of exceeding soft limit is overcome. The value of `--max-streams` should be chosen based on capacity of Data Domain appliance and utilization of Data Domain appliance by backup apps in addition to GPDB at the time of DDBoost backup / restore operation.

Document generated by Confluence on May 17, 2016 19:14


