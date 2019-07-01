CREATE DATABASE db_with_dropfailed;
DROP TABLE IF EXISTS before_drop;
CREATE TABLE before_drop(oid Oid);

INSERT INTO before_drop SELECT oid FROM pg_database WHERE datname='db_with_dropfailed';

-- inject fault on content0 primary to error out before removing object
SELECT gp_inject_fault2('drop_db_before_remove_object', 'panic', dbid, hostname, port)
FROM gp_segment_configuration 
WHERE content=0 and role='p';

-- should crash on segment 0
DROP DATABASE db_with_dropfailed;

-- cleanup
SELECT gp_inject_fault2('drop_db_before_remove_object', 'reset', dbid, hostname, port)
FROM gp_segment_configuration 
WHERE content=0 AND role='p';

SELECT * FROM pg_database WHERE datname='db_with_dropfailed';

DROP TABLE before_drop;
DROP DATABASE db_with_dropfailed;
