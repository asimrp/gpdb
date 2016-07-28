<span id="title-text"> Storage & Access : Using Behave </span>
==============================================================

This page last changed on Jan 20, 2016 by slari.

Behave (<a href="http://pythonhosted.org/behave/" class="uri" class="external-link">http://pythonhosted.org/behave/</a>) is a Python-based open source testing
framework.

See <a href="https://jenisys.github.io/behave.example/" class="uri" class="external-link">https://jenisys.github.io/behave.example/</a> for tutorial and examples.

To run an existing Behave test(s), do:

cd $SRC\_DIR/gpMgmt/bin

make behave tags=&lt;test(s) tag&gt;

For example, to run the diskfull test, you would do:

cd $SRC\_DIR/gpMgmt/bin

make behave tags=diskfull

To run a test from Python command line, first make sure that PYTHONPATH is setup
to something like the following to include the Greenplum Python directories:

export PYTHONPATH=$SRC\_DIR/gpMgmt/bin:$SRC\_DIR/gpMgmt/bin/../sbin:$PYTHONPATH:$SRC\_DIR/gpMgmt/bin/pythonSrc/ext/install/lib/python2.6/site-packages

Then cd to $SRC\_DIR/gpMgmt/bin

Invoke Python with the following command line:

python pythonSrc/ext/install/bin/behave gppylib/test/behave/mgmt\_utils/&lt;test feature file&gt;

For example, to run the diskfull test from the command line, you would do:

cd $SRC\_DIR/gpMgmt/bin

python pythonSrc/ext/install/bin/behave gppylib/test/behave/mgmt\_utils/diskfull.feature

The .feature file for the tests reside in the
$SRC\_DIR/gpMgmt/bin/gppylib/test/behave/mgmt\_utils directory.

The Python steps go in the steps sub-directory.

The data sub-directory contains .sql files, and the scripts sub-directory
contains .sh and other .py script files.

To create a TAGS file for emacs, you can do.

cd $SRC\_DIR/gpMgmt

find . -name '\*.py' -exec etags -a -o TAGS {} \\;

Then load the TAGS file in emacs using visit-tags-table command.

Examples
========
The mgmt\_utils.py file in the steps directory has a wealth of utilities for many
common tasks. Here is a brief sample. You can search for the corresponding
pattern and see the detailed test.

Timestamps
----------
@when('the timestamp from gpcrondump is stored')
@then('the timestamp from gpcrondump is stored in the backup timestamp list')
@then('verify that the incremental file has the stored timestamp')
@then('verify that the incremental file in "{location}" has all the stored
timestamps')
@then('the timestamp in the report file should be same as timestamp key')
@then('a timestamp in increments file in "{directory}" is modified to be newer')
@then('the "{table\_type}" state file under "{backup\_dir}" is saved for the "{backup\_type}" timestamp')
@then('verify that plan file has latest timestamp for "{table\_name}"')

Tables
------
@then('verify that there is a "{table\_type}" table "{tablename}" in "{dbname}"')
@then('tables names should be identical to stored table names in "{dbname}"')
@then('verify that there is no table "{tablename}" in "{dbname}"')
@then('tables in "{dbname}" should not contain any data')
@when('the user deletes rows from the table "{table\_name}" of database "{dbname}" where "{column\_name}" is "{info}"')
@then('verify that the query "{query}" in database "{dbname}" returns "{nrows}"')

File System
-----------
@when('"{filetype}" file should be created under "{dir}"')
@then('the saved state file is deleted')
@given('the file "{filename}" is removed from the system')
@given('the file "{filename}" exists under "{directory}" in current working directory')
@given('the directory "{dirname}" exists in current working directory')
@given('the directory "{dirname}" does not exist in current working directory')

File Contents
-------------
@then('the gpfdist should print {msg} to "{filename}" under {parent\_dir}')
@then('verify that the "{file\_type}" file in "{file\_dir}" dir contains "{text\_find}"')
@then('verify that the "{file\_type}" file in "{file\_dir}" dir does not contain
"{text\_find}"')
@when('the data line "{dataline}" is appened to "{fname}" in cwd')
@then('the gpfdist should print {msg} to "{filename}" under {parent\_dir}')
@then('verify that the file "{filename}" contains the string "{output}"')
@then('verify that the file "{filepath}" contains "{line}"')
@then('verify that the file "{filepath}" does not contain "{line}"')

Program Should Print
--------------------
@then('{command} should print {out\_msg} to stdout')
@then('the client program should print {msg} to stdout with value in range {min\_val} to {max\_val}')
@given('{command} should not print {out\_msg} to stdout')
@given('{command} should return a return code of {ret\_code}')
@then('{command} should print {err\_msg} error message')

Exceptions
----------
@then('an exception should be raised with "{txt}"')
@then('an "{ex\_type}" should be raised')

Performance Timers
------------------
@when('the performance timer is started')
@then('the performance timer should be less then "{num\_seconds}" seconds')

Process Management
------------------
@given('the "{process\_name}" process is killed')

OS Path
-------
@then('verify that gptransfer is in order of "{filepath}"')

Segments
--------
@given('the segments are synchronized')

Filespaces
----------
@given('the user "{USER}" creates filespace\_config file for "{fs\_name}" on
host "{HOST}" with gpdb port "{PORT}" and config "{config\_file}" in "{dir}"
directory')
@given('the user "{USER}" creates filespace on host "{HOST}" with gpdb port
"{PORT}" and config "{config\_file}" in "{dir}" directory')

The behave\_utils/utils.py has the basic utilities used by mgmt\_utilities.py.

For example, see run\_gpcommand(context, command).
The output of the command is stored as:

context.ret\_code = result.rc
context.stdout\_message = result.stdout
context.error\_message = result.stderr

The GpArray class in gparray.py provides information for the Grennplum instance
such as the hosts, segments, database, filespaces, data directories. See
diskfull\_steps.py for example usage.

 

Document generated by Confluence on May 17, 2016 19:14


