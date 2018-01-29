-- Test that readers obtain in-progress subtransactions from writer's
-- snapshot through shared local snapshot slot.

create table sharedsnapshot_t1(a int, b int) distributed by (a);
create table sharedsnapshot_t2(a int, b int) distributed by (a);

-- Session 1: start a subtransaction and insert a few tuples.  The
-- subtransaction will be included in subxip array of the serializable
-- snapshot taken by session 2.
1: begin;
1: savepoint s1;
1: insert into sharedsnapshot_t1 select 1, i from generate_series(1,5)i;

-- Advance latestCompletedXid, so that xmax of subsquent snapshots
-- will be newer than session 1's subxid.
insert into sharedsnapshot_t2 select 1, i from generate_series(1,10)i;

-- Session 2: acquire a serializable snapshot.  Subxip array of this
-- snapshot should include session 1's subtransaction.
2: begin isolation level serializable;
2: select * from gp_dist_random('gp_id');

-- Commit subtransaction and its top transaction.
1: release s1;
1: commit;

-- Run a multi-slice query involving a reader gang.  Readers should
-- consider tuples inserted by session 1's subtransaction as invisible
-- because their xmin should be in-progress according to writer's
-- subxip array.  Note that distributed snapshot is ignored for
-- subtransaction IDs because local to distributed XID mapping is
-- maintained only for top transaction ID.
2: select * from sharedsnapshot_t1 t1, sharedsnapshot_t2 t2 where t1.a = t2.b;
2: end;

-- Obtain a new snapshot so that session0U's xmin's are visible.
2: select * from sharedsnapshot_t1 t1, sharedsnapshot_t2 t2 where t1.a = t2.b;
