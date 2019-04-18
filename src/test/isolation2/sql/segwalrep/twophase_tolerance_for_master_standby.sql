-- Standby must be in sync with master before starting the test
select application_name, state, sync_state from pg_stat_replication;

-- obtain standby details into psql variables
select datadir as datadir, port as port, hostname as hostname
from gp_segment_configuration where content=-1 and role = 'm' \gset standby_
-- obtain master details into psql variables
select hostname as hostname, port as port
from gp_segment_configuration where content=-1 and role = 'p' \gset master_

-- Scenario1: Prepared but not committed transaction.  This should be
-- rolled back by standby after promotion.

-- Scenario2: Prepared and committed transaction on master but suspend
-- before commit-prepared broadcast.  Standby should send
-- commit-prepared broadcast after promotion and commit the
-- transation.

-- Promote standby.  We cannot use gpactivatestandby because it cannot
-- activate standby when master is also running.
\setenv PGDATA :standby_datadir
\! pg_ctl promote

-- replication should be stopped by now
select application_name, state, sync_state from pg_stat_replication;

-- TODO: wait for standby to be promoted

-- connect to standby
\c postgres apraveen :standby_hostname :standby_port
select * from gp_segment_configuration where content = -1;

-- connect back to master
\c postgres apraveen :master_hostname :master_port
select * from gp_segment_configuration where content = -1;

-- Restore the cluster back to the state where we started - standby
-- should act as standby.

-- stop standby
\setenv PGDATA :standby_datadir
\! pg_ctl stop -m fast
-- destroy standby
\! gpinitstandby -ra
-- initialize a new standby
\! rm -f /tmp/cmd.sh
\o /tmp/cmd.sh
\qecho gpinitstandby -a -s :standby_hostname -P :standby_port -S :standby_datadir
\! /bin/sh /tmp/cmd.sh
\o

select * from gp_segment_configuration where content = -1;
