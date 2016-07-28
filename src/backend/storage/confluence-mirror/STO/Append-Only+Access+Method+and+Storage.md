<span id="title-text"> Storage & Access : Append-Only Access Method and Storage </span>
=======================================================================================

This page last changed on Aug 19, 2014 by balasr3.

###### Author: Rajesh Alavarthi

Overview
========

The Greenplum Database can store tables in two different formats – heap and append-only. As the name suggests, the append-only storage supports only append operations. It does not support updates and deletes. The main motivation to use append-only storage instead of heap is that it supports compression. Also append-only supports files of large sizes. (Heap restricts the size of a file to 2 GB). An Append Only (AO) table is hash distributed across all of the segments in the same way as heap tables. If we use a column oriented append only table, each column data is stored in a different segment file. On a given segment, append only table is made up of a collection of segfiles. Each segfile is made up of 1 or more storage blocks. . An append-only storage block contains 1 varBlock with multiple tuples, single tuple or a partial tuple.

###### Pictorial representation of an Append-only block

<img src="attachments/42501922/42696847.jpg" class="confluence-embedded-image confluence-content-image-border" width="531" height="412" />

On-disk Layout of Tuples
========================

Tuples are stored on disk in the same format as MemTuples[\_msocom\_1](Append-Only%2BAccess%2BMethod%2Band%2BStorage.md) . A memtuple is a serialized tuple format. This allows for efficient de-serialization of them as we just need to read from disk into memory and can then start forming MemTuples directly from there. The memtuples are stored without any MVCC information attached to them. The tuple itself is stored in variable length blocks called VarBlocks. There is a limit on the maximum number of tuples that are allowed to fit in such a block. When this max limit is reached, the block is closed and a new one is created. A varBlock can have fewer tuples than this maximum number.

Each AO Block contains 1 var block containing tuples. Before the var block itself, at the beginning of the AO block, there is a series of headers. For every AO block, we first have the basic header length information (which is calculated in AppendOnlyStorageFormat\_RegularHeaderLenNeeded depending on whether checksum is enabled). Then the function AppendOnlyStorageFormat\_GetHeaderInfo will extract/parse this basic header information which includes AoHeaderKind(SmallContent, LargeContent, NonBulkDenseContent, BulkDenseContent), and ActualHeaderLen, etc. Then it proceeds to read/parse additional header information depending on HeaderKind.

### Types of Append-only Headers

**1. Small content header** - Small content block is chosen when we need to store data that fits within the blocksize. It is also used to store “fragments” of large content that could not fit within the blocksize. *Please refer to AppendOnlyStorageFormat\_GetSmallContentHeaderInfo and the structure definition of AOSmallContentHeader in cdbappendonlystorage\_int.h*

###### Structure of an AO small content header

<img src="attachments/42501922/42696849.jpg" class="confluence-embedded-image confluence-content-image-border" width="513" height="398" />

**2. Large content header -** If a tuple could not fit within a single SmallContent block, it will be split into multiple “fragments” which are stored in SmallContent block. We write a AOLargeContentHeader (with no data) to indicate how long the large content is and then use as many AOSmallContentHeader blocks as necessary to store all the large content. *Check AppendOnlyStorageFormat\_GetLargeContentHeaderInfo.*

###### Structure of an AO large content header

<img src="attachments/42501922/42696850.jpg" class="confluence-embedded-image confluence-content-image-border" width="513" height="398" />

**3. NonBulk DenseContent Header -** NonBulkDenseContent Header is intended for Column oriented tables that do their own run-length encoding compression and need more row numbers per block, it can store up to 1G of logical rows (30 bits was reserved for largeRowCount, while only 14 bits is reserved for RowCount in SmallContent Header and 25 bits in the LargeContentHeader), the largeRowCount was maintained in the header so blocks can be skipped without decompression. The maximum compression for run length encoding is limited by the maximum number of logical rows that can be stored in each block. For example, in SmallContentHeaders the maximum number of logical rows is 2^14-1. If a single value is repeated more than that number of times in a row, then multiple blocks would be required.  The NonBulkDense Content Header allows a single value to be repeated 2^30-1 times before a new block would be required due to this limit of the maximum number of logical rows.

For inserting into DenseContent block, we will compare the value received from executor with the previous inserted value, if it’s equal and the repetition count is less than 1G, then we will increase the repetition count (stream compression), this is described in detail in design document “Stream Compression and Run Length Encoding Compression Algorithm”.

###### Structure of an AO Non-Bulk Dense content header

<img src="attachments/42501922/42696851.jpg" class="confluence-embedded-image confluence-content-image-border" width="513" height="398" />

**4.** **BulkDenseContent Header** - BulkDenseContent Header is designed for Column oriented tables that want to store both a very large number of repeated rows count and that want a second compression method applied on top of run length encoding. It was primarily designed for content that gets compressed by the Append-Only Storage format layer.  Many compression algorithms can actually lead to expansion for some data patterns. This block header handles this case, but setting the compressionLength field to 0 to indicate the block was stored uncompressed because the compression algorithm failed to reduce its size.  

This block has 2 uses:

1.  Store data that all fits within the blocksize.
2.  To store the "fragments" of large content that will not all fit within the blocksize. In this case, we write an AOLargeContentHeader (with no data) to indicate how long the large content is and then as many AOBulkDenseContentHeader blocks as necessary to store all the large content.

###### Structure of an AO Bulk Dense content header

<img src="attachments/42501922/42696854.jpg" class="confluence-embedded-image confluence-content-image-border" width="513" height="398" />

BulkDenseContent Header Extension part (The 2nd 64 bit of the header go after the checksums as a separate part.

Functions and source files for reference for On-disk layout\*:\*

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Source Files</p></th>
<th align="left"><p>Functions</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>cdb-pg/src/backend/cdb/cdbappendonlystorageformat.c</p></td>
<td align="left"><ul>
<li>AppendOnlyStorageFormat_GetHeaderInfo</li>
<li>AppendOnlyStorageFormat_GetSmallContentHeaderInfo</li>
<li>AppendOnlyStorageFormat_GetLargeContentHeaderInfo</li>
<li>AppendOnlyStorageFormat_GetNonBulkDenseContentHeaderInfo</li>
</ul></td>
</tr>
<tr class="even">
<td align="left"><p>cdb-pg/src/backend/cdb/cdbappendonlystorageread.c</p></td>
<td align="left"><ul>
<li>AppendOnlyStorageRead_InternalGetBlockInfo</li>
<li>AppendOnlyStorageRead_PositionToNextBlock</li>
</ul></td>
</tr>
<tr class="odd">
<td align="left"><p>cdb-pg/src/include/cdb/cdbappendonlystorage_int.h</p></td>
<td align="left"><p> </p></td>
</tr>
<tr class="even">
<td align="left"><p>cdb-pg/src/backend/utils/datumstream/datumstreamblock.c</p></td>
<td align="left"><ul>
<li>DatumStreamBlockWrite_PutDense</li>
</ul></td>
</tr>
</tbody>
</table>

Catalog Representation
======================

When an append only table is created, an entry for it is added to the pg\_appendonly catalog on each segment. In addition, every row oriented AO table has a corresponding table named pg\_aoseg\_XXXX where XXXX is the OID of that table and every  column oriented AO table has a corresponding table named pg\_aocsseg\_XXXX where again XXXX is the OID of the table.These catalog tables and their corresponding indexes are stored in the pg\_aoseg namespace. This is a reserved namespace that is essentially used to hide these relations from the user.

Each segment database has its own set of entries within the pg\_aoseg\_XXXX tables for row oriented tables or pg\_aocsseg\_XXXX for column oriented tables.

**Example**

An append only table can be created by specifying appendonly=true in the with clause. If we need the table to be column oriented, then we need to specify orientation=column in with clause. By default the orientation=row. We can also specify the compression type and compression level in the create table with clause.

Currently zlib and quicklz compressions are supported.

We see that an entry is added to pg\_appendonly when the table is created.

``` theme:
create table table1(a int, b varchar) with (appendonly=true, orientation=column, compresstype=zlib, compresslevel=1);
```

Select all the appendonly tables (**query1**). Notice that the relid is 32780 and the segrelid is 32785 and that the block directory relid (blkdirrelid)is 40975. We will need these values as we continue this example.

``` theme:
db1=# select * from pg_appendonly;
 relid | blocksize | safefswritesize | compresslevel | majorversion | minorversion | checksum | compresstype | columnstore | segrelid | segidxid | blkdirrelid | blkdiridxid | version
-------+-----------+-----------------+---------------+--------------+--------------+----------+--------------+-------------+----------+----------+-------------+-------------+---------
 32780 |     32768 |               0 |             1 |            1 |            0 | f        | zlib         | t           |    32785 |    32786 |       40975 |       40976 |       2
(1 row)
```

 

Check this appendonly table’s details in pg\_class using the clause “relfilnode = &lt;RELID&gt;”.

``` theme:
db1=# select * from pg_class where relfilenode=32780;
 relname | relnamespace | reltype | relowner | relam | relfilenode | reltablespace | relpages | reltuples | reltoastrelid | reltoastidxid | relaosegrelid | relaosegidxid | relhasindex | relisshared | relkind | relstorage | relnatts | relchecks | reltriggers | relukeys | relfkeys | relrefs | relhasoids | relhaspkey | relhasrules | relhassubclass | relfrozenxid | relacl |                               reloptions
---------+--------------+---------+----------+-------+-------------+---------------+----------+-----------+---------------+---------------+---------------+---------------+-------------+-------------+---------+------------+----------+-----------+-------------+----------+----------+---------+------------+------------+-------------+----------------+--------------+--------+------------------------------------------------------------------------
 table1  |         2200 |   32781 |       10 |     0 |       32780 |             0 |        0 |         0 |         32782 |             0 |             0 |             0 | t           | f           | r       | c          |        2 |         0 |           0 |        0 |        0 |       0 | f          | f          | f           | f              |         1012 |        | {appendonly=true,orientation=column,compresstype=zlib,compresslevel=1}
(1 row)
```

Associated with each append only table is a segment file relation to keep track of the different segment files associated with this relation. It is named pg\_aocssseg\_XXXX for column oriented AO tables and it is named pg\_aoseg\_XXXX for row oriented AO tables.

As seen from the first query, the segment file relid is 32785. Now get the details of the segfile relation for this table

``` theme:
db1=# select * from pg_class where relfilenode=32785;
     relname      | relnamespace | reltype | relowner | relam | relfilenode | reltablespace | relpages | reltuples | reltoastrelid | reltoastidxid | relaosegrelid | relaosegidxid | relhasindex | relisshared | relkind | relstorage | relnatts | relchecks | reltriggers | relukeys | relfkeys | relrefs | relhasoids | relhaspkey | relhasrules | relhassubclass | relfrozenxid | relacl | reloptions
------------------+--------------+---------+----------+-------+-------------+---------------+----------+-----------+---------------+---------------+---------------+---------------+-------------+-------------+---------+------------+----------+-----------+-------------+----------+----------+---------+------------+------------+-------------+----------------+--------------+--------+------------
 pg_aocsseg_32780 |         6104 |   32789 |       10 |     0 |       32785 |             0 |        0 |         0 |             0 |             0 |             0 |             0 | t           | f           | o       | h          |        4 |         0 |           0 |        0 |        0 |       0 | f          | t          | f           | f              |         1012 |        |
(1 row)
```

To check details of all the segment files associated with this append only relation, you must first create a special function gp\_aocsseg and then log into utility mode into the segment server on which the data of this table is present as shown below.

``` theme:
Create a function as follows ( only applies to column oriented AO table)

db1=# CREATE FUNCTION gp_aocsseg(oid) RETURNS TABLE(gp_tid tid, segno integer, column_num smallint, physical_segno integer, tupcount bigint, eof bigint, eof_uncompressed bigint, modcount bigint, state smallint) AS '/data/ravi/o2/greenplum-db-devel/bin/postgres', 'gp_aocsseg' LANGUAGE C STRICT;

office-4-24:~ rbalasubramanian$ psql -d gptest -c "select * from gp_aocsseg(309367)"
 gp_tid | segno | column_num | physical_segno | tupcount | eof | eof_uncompressed | modcount | state 
--------+-------+------------+----------------+----------+-----+------------------+----------+-------
 (0,2)  |     1 |          0 |              1 |      100 |   0 |                0 |        1 |     1
 (0,2)  |     1 |          1 |            129 |      100 |   0 |                0 |        1 |     1
 (0,2)  |     1 |          2 |            257 |      100 |   0 |                0 |        1 |     1
(3 rows)
```

( Note : Accessing from utility mode works only on mac. On other platforms )

we can directly access from using the function )

“select \* from gp\_dist\_random(‘pg\_aoseg.pg\_aoseg\_XXXX’) where gp\_segment\_id=&lt;segment\_id&gt;”

For a row oriented table use

$ PGOPTIONS='-c gp\_session\_role=utility' psql db1 -p51001 -c "select \* from pg\_aoseg.pg\_aoseg\_XXXX";

A sample output for a row oriented AO table whose segment relid is 65548 would be as follows

``` theme:
$ PGOPTIONS='-c gp_session_role=utility' psql db1 -p51001 -c "select * from pg_aoseg.pg_aoseg_65548";
 segno | eof | tupcount | varblockcount | eofuncompressed
-------+-----+----------+---------------+-----------------
     1 |  48 |        2 |             2 |              48
     2 |  24 |        1 |             1 |              24
```

(**Note** : The master only has information about the overall tuple count.  Each segment  maintains its own segment file information. We always need to log in utility mode on each segment to check the segment file information).

Block Directory
---------------

For each append only table, there’s a corresponding entry in pg\_appendonly catalog table:

``` theme:
db1=# select * from pg_appendonly;
 relid | blocksize | safefswritesize | compresslevel | majorversion | minorversion | checksum | compresstype | columnstore | segrelid | segidxid | blkdirrelid | blkdiridxid | version
-------+-----------+-----------------+---------------+--------------+--------------+----------+--------------+-------------+----------+----------+-------------+-------------+---------
 32780 |     32768 |               0 |             1 |            1 |            0 | f        | zlib         | t           |    32785 |    32786 |       40975 |       40976 |       2
(1 row)
```

 

A block directory (blkdirrelid) for an append only table is used to track the physical location of an AO tuple inside an AO relation segment file (i.e. inside which AO block the tuple is stored). The block directory is created only when an index is created on the AO table, the block directory will be used by index scan to find out which AO block contains the tuple it’s trying to fetch. The block directory relation is stored as a heap relation. There’s a B-tree index (blkdirididx) built on top of it to improve block directory look up performance. 

To check the block directory relation for an append only table, log into utility mode:

``` theme:
alavar-mbp:~ alavar$ PGOPTIONS='-c gp_session_role=utility' psql db1 -p51001 -c "select * from pg_aoseg.pg_aoblkdir_32780";
 segno | columngroup_no | first_row_no | minipage
-------+----------------+--------------+----------
     1 |              0 |            1 |
     1 |              1 |            1 |
     2 |              0 |            1 |
     2 |              1 |            1 |
```

The “segno” column is the segment file number for append only table, “columngroup\_no” column is used for Column Oriented append only table to track segment files for different columns. “first\_row\_no” column is the first AO row number covered by the row in block directory relation.

One minipage might contain multiple block directory entries, it’s in binary format (data type “bit varying” (Type ID 1562)), so we could not check its content by running SQL query. In future we will have an internal tool to check the contents of block directory entries inside minipage (GP\_FILEDUMP tool).

Each block directory entry contains information such as first row number, offset in the segment file and the row count of the AO block(s) it covered.

Block directory related code is shared by AO and CO access method, so we will use AO as an example in this document.

**Block directory creation and insert/update**

Block directory will be created when an index is created on the AO table.

&lt;1&gt; if the index was created before any data were inserted into AO table, then it’s possible that there’s invisible rows from aborted insert transaction that were covered by block directory entry.

&lt;2&gt; if the index was created after data were inserted into AO table, then there’s no invisible rows covered by block directory entry.

AppendOnlyBlockDirectory\_InsertEntry() is called to insert a block directory entry when we finished inserting into every AO/CO block. The newly inserted block directory entry will be appended to in-memory minipage. The maximum number of block directory entries inside a minipage is gp\_blockdirectory\_minipage\_size/NUM\_MINIPAGE\_ENTRIES (default value is 161). Whenever a minipage is full, it will be written back to on-disk block directory relation by calling write\_minipage().

There’s a GUC “gp\_blockdirectory\_entry\_min\_range” that control the behavior of block directory insert. Its default value is 0, which means there will be a block directory entry for every AO/CO block. If the GUC is set to a non-zero value, it means there will be a new block directory entry, only if the file offset range covered by block directory entry is bigger than the GUC value. GUC “gp\_blockdirectory\_entry\_min\_range” was removed from MAIN after Orca was introduced into MAIN, which means there will be a block directory for every AO/CO block from Orca onwards.

**Block directory look up during index scan on AO table**

function call flow:

  <img src="attachments/42501922/49382239.png" class="confluence-embedded-image confluence-content-image-border" />

During AO index scan, appendonly\_fetch() will use the AO TupleID received from executor (from AO table index) to look up block directory, in order to find out inside which AO block the tuple is stored.

appendonly\_fetch() will call AppendOnlyBlockDirectory\_GetEntry() to find the block directory entry that covers the AO TupleID. AppendOnlyBlockDirectory\_GetEntry() will first do an index scan(using blkdiridxid)on block directory relation(blkdirrelid), then call extract\_minipage() to exact the block directory entries inside the found minipage, and finally it will call find\_minipage\_entry() to perform a binary search over the minipage to find the block directory entry for the AO block that contains the tuple. Then appendonly\_fetch() will call scanToFetchValue() to find the AO block and read in it’s contents if needed, and call fetchFromCurrentBlock() to fetch the tuple. 

In order to improve performance of index scan, a caching mechanism is used in appendonly\_fetch() and AppendOnlyBlockDirectory\_GetEntry(). Take AppendOnlyBlockDirectory\_GetEntry() as an example, if the segment file number in AO TupleID is the same as cached blockDirectory-&gt;currentSegmentFileNum, the in-memory minipage may contain the block directory entry we’re looking for, so it will call find\_minipage\_entry to search the in-memory minipage, and only if this search failed, it will call the index scan on block directory relation to find the appropriate minipage. Similar approach was used in appendonly\_fetch() to cache the block directory entry and AO block content. This caching mechanism will boost performance if there’s a lot of locality in the access pattern, this is true for today’s implementation since executor will presort all the AO TupleID before sending the fetch request to appendonly\_fetch(), although this might change in Orca according to Joy’s explanation.

This caching mechanism will only work properly if the cache key (blockDirectory-&gt;currentSegmentFileNum in AppendOnlyBlockDirectory\_GetEntry) is consistent/in-sync with cached content (minipage in AppendOnlyBlockDirectory\_GetEntry), it will return incorrect look up result if cache key is inconsistent with cached content. One possible example is in datumstreamread\_find\_block():

``` theme:
booldatumstreamread_find_block(DatumStreamRead *datumStream,
                               DatumStreamFetchDesc datumStreamFetchDesc,
                               int64 rowNum)
{… …
        /*
         * Update the current block typeInfo.
         */
        const bool isOldBlockFormat = (datumStream->getBlockInfo.firstRow == -1);
        datumStreamFetchDesc->currentBlock.have = true;
… …
        if (rowNum < datumStreamFetchDesc->currentBlock.firstRowNum)
        {            /*
             * Since we have read a new block, the temporary
             * range for the read needs to be adjusted
             * accordingly. Otherwise, the underlying bufferedRead
             * may stop reading more data because of the
             * previously-set smaller temporary range.
             */
            int64 beginFileOffset = datumStreamFetchDesc->currentBlock.fileOffset;
            int64 afterFileOffset = datumStreamFetchDesc->currentBlock.fileOffset +
                datumStreamFetchDesc->currentBlock.overallBlockLen;

            AppendOnlyStorageRead_SetTemporaryRange(
                &datumStream->ao_read,
                beginFileOffset,
                afterFileOffset);

            return false;

/* if we return false here, we would risk updating current block’s block header
   metadata without reading in it’s block contents */
        }

… …
}
```

If we are doing an index only scan on AO/CO tables (which will be supported in Orca) then the block directory is only used to find if a given row is visible or not in that case.

To check the history of the Append only column oriented table, first create a function as shown and then access the history using utility mode.

``` theme:
gptest=# create or replace function gp_aocsseg_history_dup(oid) RETURNS TABLE(gp_tid tid, gp_xmin integer, gp_xmin_status text, gp_xmin_distrib_id text, gp_xmax integer, gp_xmax_status text, gp_xmax_distrib_id text, gp_command_id integer, gp_infomask text, gp_update_tid tid, gp_visibility text, segno integer, column_num smallint, physical_segno integer, tupcount bigint, eof bigint, eof_uncompressed bigint, modcount bigint, state smallint) AS '/data/ravi/o2/greenplum-db-devel/bin/postgres' , 'gp_aocsseg_history' LANGUAGE C STRICT;

gptest=# SELECT * from gp_aocsseg_history_dup(309367);
 gp_tid | gp_xmin | gp_xmin_status |  gp_xmin_distrib_id   | gp_xmax | gp_xmax_status |  gp_xmax_distrib_id   | gp_command_id |                                    gp_infomask                                    | gp_update_tid | gp_visibility | segno | column_num | physical_segno | tupcount | eof | eof_uncompressed | modcount | state 
--------+---------+----------------+-----------------------+---------+----------------+-----------------------+---------------+-----------------------------------------------------------------------------------+---------------+---------------+-------+------------+----------------+----------+-----+------------------+----------+-------
 (0,1)  |       2 | Frozen         |                       |    7737 | Hint-Committed | 1408386921-0000004168 |             0 | {HEAP_XMIN_COMMITTED, HEAP_XMAX_COMMITTED, HEAP_XMIN_DISTRIBUTED_SNAPSHOT_IGNORE} | (0,2)         | Past          |     1 |          0 |              1 |        0 |   0 |                0 |        0 |     1
 (0,1)  |       2 | Frozen         |                       |    7737 | Hint-Committed | 1408386921-0000004168 |             0 | {HEAP_XMIN_COMMITTED, HEAP_XMAX_COMMITTED, HEAP_XMIN_DISTRIBUTED_SNAPSHOT_IGNORE} | (0,2)         | Past          |     1 |          1 |            129 |        0 |   0 |                0 |        0 |     1
 (0,1)  |       2 | Frozen         |                       |    7737 | Hint-Committed | 1408386921-0000004168 |             0 | {HEAP_XMIN_COMMITTED, HEAP_XMAX_COMMITTED, HEAP_XMIN_DISTRIBUTED_SNAPSHOT_IGNORE} | (0,2)         | Past          |     1 |          2 |            257 |        0 |   0 |                0 |        0 |     1
 (0,2)  |    7737 | Hint-Committed | 1408386921-0000004168 |       0 | None           |                       |             0 | {HEAP_XMIN_COMMITTED, HEAP_XMAX_INVALID, HEAP_UPDATED}                            | (0,2)         | Now           |     1 |          0 |              1 |       50 | 232 |              232 |        1 |     1
 (0,2)  |    7737 | Hint-Committed | 1408386921-0000004168 |       0 | None           |                       |             0 | {HEAP_XMIN_COMMITTED, HEAP_XMAX_INVALID, HEAP_UPDATED}                            | (0,2)         | Now           |     1 |          1 |            129 |       50 | 232 |              232 |        1 |     1
 (0,2)  |    7737 | Hint-Committed | 1408386921-0000004168 |       0 | None           |                       |             0 | {HEAP_XMIN_COMMITTED, HEAP_XMAX_INVALID, HEAP_UPDATED}                            | (0,2)         | Now           |     1 |          2 |            257 |       50 | 232 |              232 |        1 |     1
(6 rows)
```

Concurrency
-----------

AO supports a maximum of 128 concurrent insert transactions on each table. Initially when the AO table is created, then a single empty file with the name as its relid is created. If there are multiple concurrent transactions, then a new segment file is created, for Row-Oriented AO table, the filename of segment file is from OID.1 to OID.128, one for each simultaneous transaction. For Column-Oriented AO table, the first column’s segment file is named from OID.1 to OID.128, the next column is named OID.129 to OID.256 and so on.

Logical EOF
-----------

For Append only tables, the end of data is versioned. When a transaction successfully commits it adds  a new version which enables more data to be read by future transactions. The versioned EOFs are kept in a system catalog table. Logical EOF is the marker for the valid data range in a particular segment file. By valid data we mean data that has been committed. For example if we insert 100 rows into an AO table, the logical EOF is 100. Now if we try to insert 200 more rows, but the transaction is cancelled, then the logical EOF is still 100.

Fast Sequence Numbers
---------------------

Every time a new INSERT statement tries to insert data to append only table, a fast sequence number is used to identify the row number. This sequence is allocated in batches of 100 at a time. For example if transaction T1 inserts 10 rows, then sequence numbers 1-10 is allocated. If a new transaction T2 inserts 20 rows, the sequence numbers allocated will be 101-120. If a single transaction inserts more than 100 rows, then each time we finish a set of 100 rows, a new batch of fast sequence numbers are assigned. The fast sequence number is a 64 bit value and it is stored in the heap.

Compression
-----------

AO tables support two different types of compression – quicklz and zlib. By default the tables are not compressed. We need to specify the compression type and the compression level when we create the table. Whenever we insert new data to a compressed table, we ensure that the compressed data is less than or equal to the size of uncompressed data. After the insert is completed, we update the uncompressed and the compressed logical EOFs.

To check the compressed/uncompressed EOF values of AO column oriented table, perform the following steps

``` theme:
CREATE FUNCTION gp_aocsseg(oid) RETURNS
TABLE (gp_tid tid, segno integer, column_num smallint, physical_segno integer, tupcount bigint, eof bigint, eof_uncompressed bigint)
AS '/Users/mbergant/greenplum-db-devel/bin/postgres', 'gp_aocsseg'
LANGUAGE C STRICT;

$PGOPTIONS='-c gp_session_role=utility' psql db1 -p51001 -c "select * from gp_aocsseg(RelfileOID)";

DROP FUNCTION gp_aocsseg(oid);
```

Compression ratio
-----------------

Use the following query to check the compression ratio of an AO table

``` theme:
template1=# select * from get_ao_compression_ratio ('<table_name>');
```

Operations on AO tables
=======================

Inserting data to AO table
--------------------------

-   Get the pg\_appendonly information for the specified table.
-   Obtain the next list of fast sequences for this relation. *See section Fast sequences for more information*
-   Setup the firstRowNumber for the current insert transaction. *We keep track of the firstRowNumber and the number of rows to be inserted for each insert transaction.*
-   Initialize the block directory to prepare for inserts if an index is created.
-   Find the segment file to which the tuple needs to be inserted and its EOF.
-   Find the correct varBlock to insert the tuple. A new varBlock is used for every new insert. Inside an insert statement if we fill up a varBlock, we continue the insert in a new varBlock.
-   Write the content to the varBlock.
-   Update the block directory relation if there is one associated with this table. Note that a block directory is created only if we create an index on the append only table.
-   Finally the catalog tables are updated with the EOF, block directory entries, segment file entries after the insert transaction is committed. If the insert transaction is aborted then the catalog tables are not updated and new inserts will overwrite the entries in the segment file.

Index Scan
----------

-   Get the pg\_appendonly information for the specified table.
-   Get the catalog entries for appendonly relation from the pg\_aoseg\_\* relation that belongs to the currently used Appendonly table.
-   Using this info, get information about all the segment files we need to scan. This includes the number of segment files for this relation, then eof, tupleCount, varBlockCount etc of each segment.
-   If an index is present, scan through the index to retrieve all the TId’s that match and generate a bitmap of these TIds. This bitmap is then passed to the executor. We use  a bitmap to sort the TIds which in turn avoids random IO’s.
-   Initialize the block directory for search.
-   Get the block directory entry that covers the TID.
    1.  Retrieve the segment file number and row number from the AOTupleId.
    2.  Based on the segment file number and the row number, retrieve the minipage which has the requested row number.
    3.  If we do not find a minipage entry for the specified rowNum, that means the tuple might have been part of an aborted transaction. (Visibility check)
-   Scan through the block to fetch the row. 

Scan without an index
---------------------

This is typically a full table scan. Here we display all the rows based on the eof and tupleCount of all the different segment files, which are part of this relation. There is no block directory access. The table is scanned in the order of TIds.

-   Get the pg\_appendonly information for the specified table.
-   Get the catalog entries for appendonly relation from the pg\_aoseg\_\* relation that belongs to the currently used Appendonly table.
-   Using this info, get information about all the segment files we need to scan. This includes the number of segment files for this relation, then eof, tupleCount, varBlockCount etc of each segment.
-   Read all the blocks and scan the tuples in the blocks one by one.

AO Code walkthrough
===================

Append only row insert   
-------------------------

``` theme:
ProcessQuery -> ExecutorRun -> ExecutePlan -> ExecInsert -> \{appendonly_insert_init, appendonly_insert}

---> Followed by

ProcessQuery -> ExecutorEnd -> ExecEndPlan-> appendonly_insert_finish
```

Append only column oriented insert
----------------------------------

``` theme:
ProcessQuery -> ExecutorRun -> ExecutePlan -> ExecInsert -> \{aocs_insert_init, aocs_insert}

---> Followed by

ProcessQuerExecutorEnd -> ExecutorEnd -> ExecEndPlan -> aocs_insert_finish
```

**appendonly\_insert\_init**:

Initialize AppendOnlyInsertDesc structure, generate first row number from system catalog table gp\_fastsequence by calling GetFastSequences. Fast sequence numbers are monotonically increasing, they are allocated in batches(100) and never reused across insert statement.

**appendonly\_insert**:

AO table is different from heap since it did not use buffer pool, it will only buffer the data of inserted tuple in memory(AO Block buffer), when AO Block buffer gets full, it will be written to disk(finishWriteBlock) and a new AO Block buffer will be setup (setupNextWriteBlock) for more tuples to be inserted

**appendonly\_insert\_finish**:

Visibility check on AO table tuples is different from heap tuples since it does not have MVCC header information(Xmin, Xmax, Cmin, Cmax, hint bits, etc), AO table would rely on Block Directory and Logic EOF of Segment Files to verify the visibility of AO tuples.

**appendonly\_insert\_finish**:

As part of query plan execution process, it would be called even if the insert transaction was aborted, AO will use Logical EOF and Block Directory to filter out aborted rows during SeqScan/IndexScan.

Inside appendonly\_insert\_finish, finishWriteBlock will write last AO block ( which might not be full ) to file system, CloseWritableFileSeg will call AppendOnlyStorageWrite\_TransactionFlushAndCloseFile to flush all the data to disk and close segment file, and call UpdateFileSegInfo to update the Logical EOF of Segment File in pg\_aoseg\_OID system catalog table (simple\_heap\_update). Since system catalog tables are in heap, this update will not be visible unless the whole distributed transaction is committed. If distributed transaction was aborted, the update to pg\_aoseg\_OID catalog table will not be visible by heap’s visibility check, the same rule applies to Block Directory catalog table.

When finishWriteBlock write a AO Block to filesystem, it will also add a minipage entry to Block Directory (by calling AppendOnlyBlockDirectory\_InsertEntry), (firstRowNum, fileOffset, rowCount) of the AO Block will be recorded in the minipage entry, and these minipage entries will be buffered in in-memory minipage buffer, when minipage gets full, it will be inserted into Block Directory catalog table (write\_minipage). Inside appendonly\_insert\_finish, AppendOnlyBlockDirectory\_End\_forInsert will insert the last remaining minipage to Block Directory catalog table.

SeqScan for Row-Oriented Append only Table
------------------------------------------

``` theme:
-> ExecProcNode ->ExecAppendOnlyScan -> OpenAOScanRelation -> appendonly_beginscan

-> ExecAppendOnlyScan -> ExecScan (node, AppendOnlyNext) -> AppendOnlyNext -> Appendonly_getnext

-> ExecAppendOnlyScan -> CloseAOScanRelation -> appendonly_endscan
```

**appendonly\_getnext** will read the segment file up to it’s Logical  EOF before switch to next segment file, any tuples located before Logical EOF was considered to be visible, it’s a full table scan/sequential scan.

**Example 1**: If transaction A tried to insert 20 rows into empty AO table but aborted, then transaction B tried to insert 10 rows into the same AO table but commited. For Transaction A, since it’s aborted, the Logical EOF of segment file remained at 0, although there’s some invalid data on file system beyond offset 0 in the segment file. Transaction B will find the Logical EOF is still 0, and start to write data from offset 0 and overwrite the invalid leftover data from Transaction A. When Transaction B is committed, the Logical EOF was updated to 10. *For Column-Oriented table, please check aocs\_beginscan, AOCSNext, aocs\_getnext, aocs\_endscan.*

Index Scan for AO Row-Oriented table
------------------------------------

``` theme:
BitmapAppendOnlyScanNext -> initFetchDesc -> appendonly_fetch_init

ExecProcNode -> ExecBitmapAppendOnlyScan -> ExecScan (&node>ss, BitmapAppendOnlyScanNext) -> BitmapAppendOnlyScanNext -> appendonly_fetch

BitmapAppendOnlyScanNext -> freeFetchDesc -> appendonly_fetch_finish
```

Executor will use index to generate the TID of tuples that satisfies the predicate in query plan and put the TID in a bitmap, this TID bitmap is sorted to avoid random I/O on base table, AO access layer will use this bitmap to fetch the tuples by calling appendonly\_fetch.

**appendonly\_fetch** will do 2 things:

1.  Visibility check on the TID provided by executor. In GPDB the index was not updated when tuple was deleted/aborted/updated, so for aborted rows, there’re still relevant entries in index. AO access layer will use Block Directory catalog table to filter out these invisible/invalid tuples, since Block Directory is stored in heap, only committed tuple would be visible through Block Directory lookup.
2.  The segment file number and row number could be exacted from TID, appendonly\_fetch will call AppendOnlyBlockDirectory\_GetEntry to get the Block Directory entry that covers the TID(also check fetchFromCurrentBlock and extract\_minipage). *For Column-Oriented table, please check aocs\_fetch\_init, aocs\_fetch, aocs\_fetch\_finish.*

DTM Prepare/Commit Prepared 2PC
-------------------------------

 From the master segment, due to explicit or implicit commit command, it will call CommitTransaction. Inside CommitTransaction, as 1<sup>st</sup> phase of 2 Phase Commit Protocol, it will call prepareDtxTransaction to broadcast PREPARE TRANSACTION to segments over libpq connection.

-   On master segment:
    prepareDtxTransaction -&gt; doPrepareTransaction -&gt; doDispatchDtxProtocolCommand(DTX\_PROTOCOL\_COMMAND\_PREPARE) -&gt; cdbdisp\_dispatchDtxProtocolCommand -&gt; cdbdisp\_dispatchToGang -&gt; thread\_DispatchCommand -&gt; thread\_DispatchOut -&gt; dispatchCommandDtxProtocol -&gt; PQsendGpQuery\_shared -&gt; pqFlush

<!-- -->

-   On segment instance, do Prepare Transaction:

``` theme:
PostgresMain -> \{ pq_getmsgint, DtxContextInfo_Deserialize,exec_mpp_dtx_protocol_command}-> performDtxProtocolCommand -> performDtxProtocolPrepare -> \{ PrepareTransactionBlock, CommitTransactionCommand} -> PrepareTransaction
```

Inside PrepareTransaction, EndPrepare will writes state file (the prepare record) to WAL and actually prepare the transaction.(also check MarkAsPrepared and the comments above this funciton). PrepareTransaction was modified from CommitTransaction.

-   Once Prepare Transaction phase succeeded, segment instance will response to master segment:

``` theme:
exec_mpp_dtx_protocol_command -> EndCommand -> \{sendQEDetails, AddQEWriterTransactionInfo}
```

-   On master, inside doPrepareTransaction and doDispatchDtxProtocolCommand, it will check the response from all segment instances, if all segment instances finished prepare transaction successfully, master will continue to 2<sup>nd</sup> phase - Commit Prepared Transaction:

``` theme:
CommitTransaction -> notifyCommittedDtxTransaction -> doNotifyingCommitPrepared -> doDispatchDtxProtocolCommand (DTX_PROTOCOL_COMMAND_COMMIT_PREPARED)
```

-   When segment instance received the DTX\_PROTOCOL\_COMMAND\_COMMIT\_PREPARED command:

``` theme:
-> performDtxProtocolCommand -> performDtxProtocolCommitPrepared -> \{ FinishPreparedTransaction, CommitTransactionCommand} -> CommitTransaction
```

When all segments are done with “Commit Prepared”, master will insert an FORGET COMMITED RECORD into XLog.

``` theme:
doNotifyingCommitPrepared -> doInsertForgetCommitted
```

Useful GUCs for debugging
=========================

<table>
<colgroup>
<col width="33%" />
<col width="33%" />
<col width="33%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>GUC name</p></th>
<th align="left"><p>Acceptable values</p></th>
<th align="left"><p>Description</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>Debug_appendonly_print_scan</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only scan</em></p></td>
</tr>
<tr class="even">
<td align="left"><p>Debug_appendonly_print_insert</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only insert</em></p></td>
</tr>
<tr class="odd">
<td align="left"><p>Debug_appendonly_print_scan_tuple</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only scan tuples</em></p></td>
</tr>
<tr class="even">
<td align="left"><p>Debug_appendonly_print_insert_tuple</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only insert tuples</em></p></td>
</tr>
<tr class="odd">
<td align="left"><p>Debug_appendonly_print_blockdirectory</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only block directory operations</em></p></td>
</tr>
<tr class="even">
<td align="left"><p>Debug_appendonly_print_segfile_choice</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only writers about their choice for AO segment file</em></p></td>
</tr>
<tr class="odd">
<td align="left"><p>Debug_appendonly_print_append_block</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only writes</em></p></td>
</tr>
<tr class="even">
<td align="left"><p>Debug_appendonly_print_verify_write_block</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only verify block during write</em></p></td>
</tr>
<tr class="odd">
<td align="left"><p>Debug_appendonly_print_read_block</p></td>
<td align="left"><p>true<br />
false</p></td>
<td align="left"><p><em>Print log messages for append-only reads</em></p></td>
</tr>
</tbody>
</table>

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Append Only Storage Block.pdf](attachments/42501922/42696846.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Append Only Storage Block.jpg](attachments/42501922/42696847.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Small Content header.pdf](attachments/42501922/42696848.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Small Content header.jpg](attachments/42501922/42696849.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Large Content header.jpg](attachments/42501922/42696850.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [NonBulk Dense Content Header.jpg](attachments/42501922/42696852.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [NonBulk Dense Content Header.jpg](attachments/42501922/42696853.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [NonBulk Dense Content Header.jpg](attachments/42501922/42696851.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Bulk Dense Content Header.jpg](attachments/42501922/42696854.jpg) (image/jpeg)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Function Call Flow during AO Index Scan.png](attachments/42501922/49382239.png) (image/png)

Document generated by Confluence on May 17, 2016 19:14


