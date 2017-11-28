-- Test that readers obtain in-progress subtransactions from writer's
-- snapshot through shared local snapshot slot.

create table sharedsnapshot_t1(a int, b int) distributed by (a);
create table sharedsnapshot_t2(a int, b int) distributed by (a);

-- Session1: create two subtransactions.
1: begin;
1: savepoint s1;
1: insert into sharedsnapshot_t1 select i, i from generate_series(1,10)i;
1: savepoint s2;
1: delete from sharedsnapshot_t1 where a = 3;

-- Session2: cereate one more subtransaction. 
2: begin;
2: insert into sharedsnapshot_t2 select i, i from generate_series(1,10)i;
2: savepoint s1;
2: insert into sharedsnapshot_t2 select i, i from generate_series(11,20)i;

-- Session3: run multi-slice query involving one reader gang.
-- Serializable isolation level is needed so that the writer's
-- snapshot sees all 4 subtransactions created earlier as in-progress.
-- Readers share this snapshot from their writers.
3: begin isolation level serializable;
3: select 1;

-- Commit all subtransactions and their top transactions so that
-- in-progress array is consulted to determine visibility.  This
-- doesn't happen for a transaction that is not known committed.
1: release s1;
2: release s1;
1: commit;
2: commit;

-- No tuples should be returned because sessions 1 & 2 are still
-- in-progress according to the serializable snapshot.
3: select * from sharedsnapshot_t1 t1, sharedsnapshot_t2 t2 where t1.a = t2.b and t1.a > 7;
3: end;

-- Obtain new snapshot so that effects of sessions 1 & 2 are visible.
3: select * from sharedsnapshot_t1 t1, sharedsnapshot_t2 t2 where t1.a = t2.b and t1.a > 7;
