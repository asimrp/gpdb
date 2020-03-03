-- Test to validate that replay process on mirror does not affect
-- synchronous replication when the mirror restarts.  If a mirror
-- develops replay lag, transactions should not wait for replication
-- when the mirror restarts.  WAL replay is performed asynchronously
-- by a separate replay process and that shouldn't interfere with
-- synchronous replication.

-- Skip FTS so that it doesn't mark the mirror down when it's
-- restarted by the test.
select gp_inject_fault_infinite('fts_probe', 'skip', dbid)
from gp_segment_configuration where content = -1 and role = 'p';
select gp_request_fts_probe_scan();
select gp_wait_until_triggered_fault('fts_probe', 1, dbid)
from gp_segment_configuration where content = -1 and role = 'p';

-- Deliberately slow down WAL replay on content 1 mirror.  All tuples
-- inserted below are hashed to content 1.
select gp_inject_fault('wal_redo', 'sleep', '', '', '', 1, -1, 1, dbid)
from gp_segment_configuration where role = 'm' and content = 1;

-- Create three tables, these will be loaded concurrently.
create table replay_lag_t1(a int, b int) with (appendonly=true) distributed by (a);
create table replay_lag_t2(a int, b int) distributed by (a);
create table replay_lag_t3(a int, b int) with (appendonly=true) distributed by (a);

-- Concurrent load so as to generate a lot of WAL on primary and let
-- mirror replay lag behind.  Insert tuples only on segment with
-- content 1.
1&:insert into replay_lag_t1 select 1, i from generate_series(1,1000000)i;
2&:insert into replay_lag_t2 select 1, i from generate_series(1,1000000)i;
3&:insert into replay_lag_t3 select 1, i from generate_series(1,1000000)i;

1<:
2<:
3<:

1&:insert into replay_lag_t1 select * from replay_lag_t1;
2&:insert into replay_lag_t2 select * from replay_lag_t2;
3&:insert into replay_lag_t3 select * from replay_lag_t3;

1<:
2<:
3<:

1&:insert into replay_lag_t1 select * from replay_lag_t1;
2&:insert into replay_lag_t2 select * from replay_lag_t2;
3&:insert into replay_lag_t3 select * from replay_lag_t3;

1<:
2<:
3<:

1&:insert into replay_lag_t1 select * from replay_lag_t1;
2&:insert into replay_lag_t2 select * from replay_lag_t2;
3&:insert into replay_lag_t3 select * from replay_lag_t3;

1<:
2<:
3<:
1q:
2q:
3q:

-- Replay lag should be several hundred MBs.
select 2^20 < (flush_location - replay_location) from gp_stat_replication
where gp_segment_id = 1;

-- Disconnect replication connection by killing WAL receiver process.
select gp_inject_fault('walrcv_loop', 'fatal', dbid)
from gp_segment_configuration where content = 1 and role = 'm';

select gp_wait_until_triggered_fault('walrcv_loop', 1, dbid)
from gp_segment_configuration where content = 1 and role = 'm';

-- Commit a transaction, it should wait until mirror comes back up and
-- sets up syncrep.  But it should NOT wait until the replay is
-- finished.  The ">" means isolation2 framwork will start the command
-- in background but will not ensure that the command blocks.
1>:insert into replay_lag_t1 select i, i from generate_series(1, 20)i;

-- TODO: introduce loop to wait until the insert command started in
-- background shows up in pg_stat_activity on QD.  Primary segment
-- logs indicate that the insert command reaches the primary after the
-- following utility mode select.  That's definitely not what the test
-- intends to do.
select pg_sleep(3);

-- Replay lag should continue to exist.
select 0 < (flush_location - replay_location) from gp_stat_replication
where gp_segment_id = 1;

-- The insert statement should not need to wait for replication.
-- TODO: use a loop with retry to make the test more robust?
1U:select query, state, waiting_reason from pg_stat_activity
where query like 'insert into replay_lag_t1%';
1Uq:

select gp_inject_fault('all', 'reset', dbid) from gp_segment_configuration;

1<:
1q:
