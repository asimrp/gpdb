CREATE DATABASE db_with_drop_failed;

-- Inject fault on content0 primary to error during prepare phase of 2PC.
SELECT gp_inject_fault2('end_prepare_two_phase', 'error', dbid, hostname, port)
FROM gp_segment_configuration WHERE content=0 and role='p';

-- Should abort
DROP DATABASE db_with_drop_failed;

-- The database should remain in catalog.
SELECT count(1) = 1 as result FROM pg_database WHERE datname='db_with_drop_failed';

-- Try creating a table in this database.  If it succeeds, we are sure
-- that the database directory and the catalog tables are still
-- present.
\! psql -d db_with_drop_failed -c "create table tbl_in_db_with_drop_failed()"

-- Cleanup
SELECT gp_inject_fault2('end_prepare_two_phase', 'reset', dbid, hostname, port)
FROM gp_segment_configuration WHERE content=0 AND role='p';
DROP DATABASE db_with_dropfailed;
