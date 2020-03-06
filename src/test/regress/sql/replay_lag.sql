-- Test to validate that replay process on mirror does not affect
-- synchronous replication when the mirror restarts.  If a mirror
-- develops replay lag, transactions should not wait for replication
-- when the mirror restarts.  WAL replay is performed asynchronously
-- by a separate replay process and that shouldn't interfere with
-- synchronous replication.
create extension gp_inject_fault;

select gp_inject_fault_infinite('fts_probe', 'skip', dbid)
from gp_segment_configuration where content = -1 and role = 'p';
select gp_request_fts_probe_scan();
select gp_wait_until_triggered_fault('fts_probe', 1, dbid)
from gp_segment_configuration where content = -1 and role = 'p';

create table replay_lag_test(a int, b text, c timestamp with time zone)
distributed by (a);

-- Switch WAL so that streaming start point (segment number, TLI) is
-- remembered on mirrors.  This is useful only when the test is run
-- against a newly created cluster, where the first WAL segment is not
-- filled up yet.
select pg_switch_xlog() > '0/0' from gp_dist_random('gp_id');

-- Deliberately slow down WAL replay by 10 seconds on content 1
-- mirror.  Enabling this fault also has the effect of setting
-- wal_receiver_start_condition GUC on the mirror to 'consistency'.
select gp_inject_fault('wal_redo', 'sleep', '', '', '', 1, -1, 10, dbid)
from gp_segment_configuration where role = 'm' and content = 1;

-- Induce replay lag of 10 * 3 = 30 seconds on content 1 mirror.  All
-- tuples inserted below are hashed to content 1.
insert into replay_lag_test select 1, 'before disconnect', now()
from generate_series(1,3);

-- Disconnect replication connection by killing WAL receiver process.
select gp_inject_fault('walrcv_loop', 'fatal', dbid)
from gp_segment_configuration where content = 1 and role = 'm';

select gp_wait_until_triggered_fault('walrcv_loop', 1, dbid)
from gp_segment_configuration where content = 1 and role = 'm';

insert into replay_lag_test values (1, 'after disconnect', now());

-- Replay lag should continue to exist after WAL receiver restart, as
-- we have not reset the wal_redo fault yet.
select replay_location < flush_location from gp_stat_replication
where gp_segment_id = 1;

-- The insert should finish before the replay process catches up.
-- Expect WAL receiver to resume streaming within 20 seconds.
select (now() - c) < '20 seconds' from replay_lag_test where b = 'after disconnect';

select gp_inject_fault('all', 'reset', dbid) from gp_segment_configuration;