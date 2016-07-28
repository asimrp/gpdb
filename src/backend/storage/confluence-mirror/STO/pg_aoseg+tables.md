<span id="title-text"> Storage & Access : pg\_aoseg tables </span>
==================================================================

This page last changed on Sep 16, 2008 by goldsa1.

### creation

When an AO table is created, another secondary table is created, called pg\_aoseg.pg\_aoseg\_&lt;oid of AO table&gt;. This is much like pg\_toast.pg\_toast\_&lt;oid&gt; table creation.

    gpdb=# create table aotest(a int) with (appendonly=true);
    CREATE TABLE

    gpdb=# select oid,relfilenode,relstorage,relaosegrelid from pg_class where relname='aotest';
      oid  | relfilenode | relstorage | relaosegrelid 
    -------+-------------+------------+---------------
     80137 |       80137 | a          |         80141
    (1 row)

    gpdb=# select relname from pg_class where oid=80141;
        relname     
    ----------------
     pg_aoseg_80137
    (1 row)

    gpdb=# select relname,relkind from pg_class where oid=80141;
        relname     | relkind 
    ----------------+---------
     pg_aoseg_80137 | o
    (1 row)

note that the aoseg table name is pg\_aoseg\_&lt;oid of main table&gt;. however this may not be the case always. the best thing to do is to always check the relaosegrelid of the main table, and find the aoseg table name this way, like in the example above.

note that this table always exists in a special reserved schema called 'pg\_aoseg'.

this table is initially empty:

    gpdb=# select * from pg_aoseg.pg_aoseg_80137;
     segno | eof | tupcount | varblockcount 
    -------+-----+----------+---------------
    (0 rows)

the same is true on the QEs.

now lets load some data into the AO table

    gpdb=# copy aotest from stdin;
    Enter data to be copied followed by a newline.
    End with a backslash and a period on a line by itself.
    >> 1
    >> 2
    >> 3
    >> 4
    >> 5
    >> 5
    >> 5
    >> \.
    gpdb=# select count(*) from aotest;
     count 
    -------
         7
    (1 row)

### usage

#### on QE's

This table on the QE's is in charge of keeping track of AO table file segments - each entry represents an existing file segment, its eof, number of tups, and number of blocks, as stands true for the local QE. When you scan a table the scan node looks at the eof and knows it needs to scan up to this eof for the specific fileseg. When you update a table you need to update the entry.

    [alon-goldshuvs-computer:~] PGOPTIONS='-c gp_session_role=utility' psql gpdb -p11001 -c "select * from pg_aoseg.pg_aoseg_80137"
     segno | eof | tupcount | varblockcount 
    -------+-----+----------+---------------
         1 |  68 |        5 |             1
    (1 row)

    [alon-goldshuvs-computer:~] PGOPTIONS='-c gp_session_role=utility' psql gpdb -p11002 -c "select * from pg_aoseg.pg_aoseg_80137"
     segno | eof | tupcount | varblockcount 
    -------+-----+----------+---------------
         1 |  36 |        2 |             1
    (1 row)

if you look at the first query on the top of this page you'll see that table 'aotest' has a relfilenode value of 80137. this means that the physical files on disk have this name. looking at the QE's, we'll expect to find 1 file per pg\_aoseg entry. for example the QE on port 11001 shows a single segfile with segno "1". therefore, on the file system of this QE we will expect to find a file named "80137.1". (NOTE: relfilenodes don't always stay the same during the life of a table. look at pg\_class first before making assumptions).

#### on QD

This table (the aoseg table) on the QD needs to have the same number of entries, for the same file segments, but on the QD table we only care about the tupcounts, which is the summary of all tup counts on the QE's for that specific segment (we ignore EOF and block count fields). We need this information on the QD for the AO concurrency model.

<span class="error">\[alon-goldshuvs-computer:~\]</span> psql gpdb -c "select \* from pg\_aoseg.pg\_aoseg\_80137"
segno | eof | tupcount | varblockcount
------<span style="text-decoration: line-through; "><span style="text-decoration: underline; "></span>---<span style="text-decoration: line-through; "></span></span>--------<span style="text-decoration: line-through; ">+</span>--------------
1 | 0 | 7 | 0
(1 row)

### with table partitioning too

note that with table partitioning this has to be done for **Each one of the AO partitions**. so for example, if a table has 5 AO partitions, each of these 5 tables should have its own pg\_aoseg table (on the QD and QEs) and after we insert into them (directly or through the master) each partition that inserted data must also update its pg\_aoseg tables! (There's quiet a bit of code written to make this work. it should definitely get tested, see MPP-3268).

### AO concurrency

The AO concurrency model is (shortly): QD looks at its local aoseg table, picks a segfile number (segno) that is not full and is not currently being used by a concurrent transaction. This segno will be used on **all QE's that insert data into the AO table in this transaction on both pri and mir**. The QD passes its decision (i.e "segno \#2") down to the QE's. QE's get it, and insert into segno \#2. Then they update their local aoseg table.

Note that according to this, if no concurrency is ever used, the table will only have 1 segfile, segfile "1". You'll only start creating more segfiles ones you insert data concurrently.

After writing data we need to tell the QD about the changes made to segno \#2 (how many tuples added to it). This is done at the end of a COPY or INSERT command.

### special cases

#### gp\_restore

... gp\_restore ...
... gp\_update\_ao\_master\_stats ...

#### table rewrites

Document generated by Confluence on May 17, 2016 19:14


