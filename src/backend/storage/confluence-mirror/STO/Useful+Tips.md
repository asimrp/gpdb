<span id="title-text"> Storage & Access : Useful Tips </span>
=============================================================

This page last changed on Jan 28, 2015 by apraveen.

**A Collection of tips and pointers relevant to storage development.**

-   [gpverify - Jeanna's note on adding files or directories to the default "ignored lists" for verification.](#UsefulTips-gpverify-Jeanna'snoteonaddingfilesordirectoriestothedefault%22ignoredlists%22forverification.)
-   [Valgrind Set up - Setting up Valgrind on your system](#UsefulTips-ValgrindSetup-SettingupValgrindonyoursystem)
-   [Using gdb remotely](#UsefulTips-Usinggdbremotely)

#### gpverify - Jeanna's note on adding files or directories to the default "ignored lists" for verification.

When adding new metadata files or directories, it is common that these files are not expected to the same on the primary and mirror. As such
they should be ignored by our verification utilities, gpcheckmirrorseg and gpverify. Both utilities have lists of files and
directories that should be ignored when doing a comparison of all files on a primary and mirror.

``` theme:
To add a file or directory to these lists,  modify gpMgmt/bin/gpcheckmirrorseg.pl and gpMgmt/bin/gppylib/commands/gp.py.

In gp.py in SendFilerepVerifyMessage, there are two lists DEFAULT_IGNORE_FILES and DEFAULT_IGNORE_DIRS. Simply
add the filename or directory name to these lists like this:


DEFAULT_IGNORE_FILES =
[
        'pg_internal.init', 'pgstat.stat', 'pga_hba.conf',
        'pg_ident.conf', 'pg_fsm.cache', 'gp_dbid', 'gp_pmtransitions_args',
        'gp_dump', 'postgresql.conf', 'postmaster.log', 'postmaster.opts',
        'postmaser.pids', 'postgresql.conf.bak', 'core',  'wet_execute.tbl',
        'recovery.done', 'NEW_FILE_TO_IGNORE']

    DEFAULT_IGNORE_DIRS = [
        'pgsql_tmp', 'pg_xlog', 'pg_log', 'pg_stat_tmp', 'pg_changetracking', 'pg_verify', 'NEW_DIR_TO_IGNORE','db_dumps', 'pg_utilitymodedtmredo', 'gpperfmon'
    ]

In gpcheckmirrorseg.pl in the subroutine ignore_list, add the file or directory name to my @m1 like this:

my @m1 = qw(
                pg_stat
                pg_verify
                NEW_DIR_TO_IGNORE
                pg_log
                pg_internal
                pg_fsm.cache
                pgstat
                pgsql_tmp
                pg_changetracking
                pg_utilitymodedtmredo
                gp_dbid
                core
                db_dumps
                NEW_FILE_TO_IGNORE
                );

For directories, it is also necessary to add an additional line soon after BEGIN like this:

    push @{$glob_dirignore}, "pgsql_tmp"; # this directory only on primaries
    push @{$glob_dirignore}, "pg_verify"; # this directory may be just on primary
    push @{$glob_dirignore}, "NEW_DIR_TO_IGNORE"; 
```

#### Valgrind Set up - Setting up Valgrind on your system

``` theme:
1. Download valgrind and install it in your system:
For example:  /data/kentj/pkgs/valgrind-3.7.0. 
2. In cdb-pg/src/bin/pg_ctl/pg_ctl.c, inside function start_postmaster(), find the line:     
               cmd[0] = '\0';    
  After this line, add    :
               sprintf(cmd, "/data/kentj/pkgs/valgrind-3.7.0/bin/valgrind --leak-check=yes ");   
3. Compile and start gpdb. 
4. Run the repro attached in MPP-16598. 
5. The valgrind output should be in <gpdb_log_dir>/startup.log.
```

#### Using gdb remotely

In some cases we may need to step through recovery process on a customer system. This is difficult because the segment to be recovered is usually down. The Python module `gpsegstart.py` will start the segment using `pg_ctl`. By the time we determine the process ID of postmaster using `"ps -ef"`, the recovery process might have passed the interesting point we wanted it to stop at. A solution to this may be as follows. Note that we have never tried this in practice. But we want to document it here so that we may try it in future. Perform all the following steps on the affected segment host:

1.  Create a wrapper shell file `$GPHOME/bin/gdb_wrapper.sh` with the content {{gdb -x /tmp/gdb\_cmd.txt pg\_ctl $@}}. Then modify the file {{$GPHOME/<span style="line-height: 1.4285715;">lib/python/gppylib/commands/gp.py}}:</span>

    **gp.py class SegmentStart(Command):**

    ``` theme:
            # build pg_ctl command                                                                                    

            c = PgCtlStartArgs(datadir, b, era, "$GPHOME/bin/gdb_wrapper.sh", wrapper_args, not noWait, timeout)

            self.cmdStr = str(c) + ' 2>&1'
    ```

2.  Create a file `/tmp/gdb_cmd.txt`:
3.  **/tmp/gdb\_cmd.txt**

    ``` theme:
    target remote localhost:40000 
    ```

4.  Start `gdbserver` as: `"gdbserver localhost:40000 pg_ctl"`

Now execute `gpstart` from master to trigger recovery. On the affected segment, you should see a connection on the gdb server started. Use the `gdbserver` to drive the pg\_ctl process which spawns postmaster and the recover process. Use `"follow fork mode"` command of gdb to follow child process after a fork.

Document generated by Confluence on May 17, 2016 19:14


