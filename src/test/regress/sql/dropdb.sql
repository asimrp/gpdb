CREATE DATABASE db_with_drop_failed;

-- Inject fault on content0 primary to error during prepare phase of 2PC.
SELECT gp_inject_fault2(
       'transaction_abort_after_distributed_prepared', 'error', dbid, hostname, port)
FROM gp_segment_configuration WHERE content=-1 and role='p';

-- Should abort
DROP DATABASE db_with_drop_failed;

-- The database should remain in catalog.
SELECT count(1) = 1 as result FROM pg_database WHERE datname='db_with_drop_failed';

-- Try creating a table in this database.  If it succeeds, we are sure
-- that the database directory and the catalog tables are still
-- present.
\c db_with_drop_failed
create table tbl_in_db_with_drop_failed(a int) distributed by (a);

-- Cleanup
\c regression
SELECT gp_inject_fault2(
       'transaction_abort_after_distributed_prepared', 'reset', dbid, hostname, port)
FROM gp_segment_configuration WHERE content=-1 AND role='p';
DROP DATABASE db_with_drop_failed;
