<span id="title-text"> Storage & Access : Heap Storage </span>
==============================================================

This page last changed on Apr 10, 2012 by matthj2.

***\*****\* Work in progress \*\*****  Author: Jeanna Matthews***

-   <span class="TOCOutline">1</span> [Introduction](#HeapStorage-Introduction)
    -   <span class="TOCOutline">1.1</span> [Storage Blocks/Pages](#HeapStorage-StorageBlocks/Pages)
    -   <span class="TOCOutline">1.2</span> [Page Header](#HeapStorage-PageHeader)
    -   <span class="TOCOutline">1.3</span> [Heap tuples](#HeapStorage-Heaptuples)
    -   <span class="TOCOutline">1.4</span> [Heap Tuples Visibility Information](#HeapStorage-HeapTuplesVisibilityInformation)
    -   <span class="TOCOutline">1.5</span> [Vacuuming](#HeapStorage-Vacuuming)
    -   <span class="TOCOutline">1.6</span> [TOAST](#HeapStorage-TOAST)
-   <span class="TOCOutline">2</span> [Operations on Heap tables](#HeapStorage-OperationsonHeaptables)
    -   <span class="TOCOutline">2.1</span> [Quick Overview of Query Processing](#HeapStorage-QuickOverviewofQueryProcessing)
    -   <span class="TOCOutline">2.2</span> [Inserting Heap Data](#HeapStorage-InsertingHeapData)
        -   <span class="TOCOutline">2.2.1</span> [Insert into Base Relation](#HeapStorage-InsertintoBaseRelation)
        -   <span class="TOCOutline">2.2.2</span> [Insert Into Indices](#HeapStorage-InsertIntoIndices)
        -   <span class="TOCOutline">2.2.3</span> [Write-Ahead Logging](#HeapStorage-Write-AheadLogging)
        -   <span class="TOCOutline">2.2.4</span> [Special Insert Functions](#HeapStorage-SpecialInsertFunctions)
    -   <span class="TOCOutline">2.3</span> [Updating Heap Data](#HeapStorage-UpdatingHeapData)
        -   <span class="TOCOutline">2.3.1</span> [Updating Data in Base Relation](#HeapStorage-UpdatingDatainBaseRelation)
        -   <span class="TOCOutline">2.3.2</span> [Updating Indices](#HeapStorage-UpdatingIndices)
        -   <span class="TOCOutline">2.3.3</span> [Write-Ahead Logging](#HeapStorage-Write-AheadLogging.1)
        -   <span class="TOCOutline">2.3.4</span> [Special Updates](#HeapStorage-SpecialUpdates)
    -   <span class="TOCOutline">2.4</span> [Delete](#HeapStorage-Delete)
    -   <span class="TOCOutline">2.5</span> [Scanning Heap Relations](#HeapStorage-ScanningHeapRelations)
        -   <span class="TOCOutline">2.5.1</span> [Scan Without Index](#HeapStorage-ScanWithoutIndex)
        -   <span class="TOCOutline">2.5.2</span> [Scan With Index](#HeapStorage-ScanWithIndex)
        -   <span class="TOCOutline">2.5.3</span> [Scans and Changes to Heap Tuples](#HeapStorage-ScansandChangestoHeapTuples)
    -   <span class="TOCOutline">2.6</span> [Creating a Heap Table](#HeapStorage-CreatingaHeapTable)
        -   <span class="TOCOutline">2.6.1</span> [When Heap Tables Are Created](#HeapStorage-WhenHeapTablesAreCreated)
            -   <span class="TOCOutline">2.6.1.1</span> [CREATE TABLE (User Defined Table)](#HeapStorage-CREATETABLE(UserDefinedTable))
            -   <span class="TOCOutline">2.6.1.2</span> [SELECT INTO (User Defined Table)](#HeapStorage-SELECTINTO(UserDefinedTable))
            -   <span class="TOCOutline">2.6.1.3</span> [Metadata Tables](#HeapStorage-MetadataTables)
                -   <span class="TOCOutline">2.6.1.3.1</span> [Toast Tables](#HeapStorage-ToastTables)
                -   <span class="TOCOutline">2.6.1.3.2</span> [pg\_aoseg and pg\_aocsseg relations](#HeapStorage-pg_aosegandpg_aocssegrelations)
                -   <span class="TOCOutline">2.6.1.3.3</span> [pg\_aoblkdir relation](#HeapStorage-pg_aoblkdirrelation)
                -   <span class="TOCOutline">2.6.1.3.4</span> [Bitmap Index Tables](#HeapStorage-BitmapIndexTables)
            -   <span class="TOCOutline">2.6.1.4</span> [Indices](#HeapStorage-Indices)
                -   <span class="TOCOutline">2.6.1.4.1</span> [User Defined Indices](#HeapStorage-UserDefinedIndices)
                -   <span class="TOCOutline">2.6.1.4.2</span> [Indices on Metadata tables](#HeapStorage-IndicesonMetadatatables)
            -   <span class="TOCOutline">2.6.1.5</span> [CLUSTER](#HeapStorage-CLUSTER)
            -   <span class="TOCOutline">2.6.1.6</span> [Bootstrap catalog tables and indices](#HeapStorage-Bootstrapcatalogtablesandindices)
        -   <span class="TOCOutline">2.6.2</span> [How Heap Tables Are Created](#HeapStorage-HowHeapTablesAreCreated)
            -   <span class="TOCOutline">2.6.2.1</span> [heap\_create](#HeapStorage-heap_create)
            -   <span class="TOCOutline">2.6.2.2</span> [heap\_create\_with\_catalog](#HeapStorage-heap_create_with_catalog)
-   <span class="TOCOutline">3</span> [Important Source Code Files](#HeapStorage-ImportantSourceCodeFiles)
-   <span class="TOCOutline">4</span> [Some Useful References](#HeapStorage-SomeUsefulReferences)
-   <span class="TOCOutline">5</span> [Appendix A: HeapTupleHeaderData bit masks](#HeapStorage-AppendixA:HeapTupleHeaderDatabitmasks)

Introduction
============

GPDB can store tables in two different formats – heap and append-only. Append-only tables are unique to GPDB while heap tables are part of standard Postgres. Unlike append-only tables, heap tables support updates and deletes as well as inserts. Unlike append-only tables, heap tables do not support compression.
A heap table is hash distributed across all of the segments. On a given segment, a heap table is made up of a collection of segfiles. For heap tables, each segfile is at most RELSEG\_SIZE (usually 2 GB) in length and is made up of storage blocks or pages. Storage blocks or pages are fixed size (BLCKSZ which is at most 32 KB). Storage blocks or pages contain data tuples.

Storage Blocks/Pages
--------------------

Heap files are made up of a fixed size storage blocks or pages. The format of these blocks or pages is described in include/storage/bufpage.h.

``` theme:
+--------------------------------------------------+
| PageHeaderData | linp1 linp2 linp3 ...           |
+-----------+----+---------------------------------+
| ... linpN |                                      |
+-----------+--------------------------------------+
|          ^ pd_lower              |
|                          |
|            v pd_upper            |
+-------------+------------------------------------+
|            | tupleN ...              |
+-------------+------------------+-----------------+
|   ... tuple3 tuple2 tuple1 | "special space" |
+--------------------------------+-----------------+
```

The line pointers, linp1..N, form an ItemId array. ItemPointers point into this array rather than pointing directly to a tuple. Data tuples, tuple1..N, are added "backwards" on the page. Because a tuple's ItemPointer points to its ItemId entry rather than its actual byte-offset position, tuples can be physically shuffled on a page whenever the need arises. The space between the line pointers and the tuples is free space.

-   There is a maximum number of tuples that can fit in a page, but they are only filled up to a "fill factor" with new inserts. Remaining space is reserved for updates in an attempt to keep the update tuple on the same page.
-   The maximum number of tuples that can fit in a page is determined by the BLCKSIZE. Minimum possible page size is perhaps 64B to fit page header, opaque space and a minimal tuple; of course, in reality you want it much bigger. On the high end, we can only support pages up to 32KB because lp\_off/lp\_len are 15 bits.
    The special space is used by indices. Different index access methods use this special area differently and it is empty in ordinary heap tables. For btree indexes, the special area includes links to the page's left and right siblings.

Page Header
-----------

Page Header Data contains pointers to the major sections of the page. Specifically, it contains pd\_linp, pd\_lower, pd\_upper, pd\_special that identify the start and end of the line pointers, the start of the tuples and the special area respectively. pd\_lower and pd\_upper mark the start and the end of the free space between the line pointers which are growing down and the tuples which are growing up (as illustrated above).

The Page Header Data also identifies the xlog record for the last change to this page (pd\_lsn and pd\_tli). The LSN is used by the buffer manager to enforce the basic rule of WAL: "thou shalt write xlog before data". A dirty buffer cannot be dumped to disk until xlog has been flushed at least as far as the page's LSN.

**The PageHeaderData stucture defined in include/storage/bufpage.h**

``` theme:
typedef struct PageHeaderData
{
  XLogRecPtrpd_lsn;
  uint16 pd_tli;
  uint16 pd_flags;
  LocationIndex pd_lower;
  LocationIndex pd_upper;
  LocationIndex pd_special;
  uint16 pd_pagesize_version;
  ItemIdDatapd_linp[1];
} PageHeaderData; 
```

Heap tuples
-----------

**Overall structure of a heap tuple**

``` theme:
Fixed fields (HeapTupleHeaderData struct)
Nulls bitmap (if HEAP_HASNULL is set in t_infomask)
Alignment padding (as needed to make user data MAXALIGN'd)
Object ID (if HEAP_HASOID is set in t_infomask)
User data fields 
```

The HeapTupleHeaderData is a densely packed structure with a number of fields used for multiple purposes depending on the conditions. It is a well-known source of complexity and confusion in Postgres!

**The HeapTupleHeaderData structure defined in src/include/access/htup.h**

``` theme:
typedef struct HeapTupleHeaderData
{
  union
  {
    HeapTupleFields t_heap;
    DatumTupleFields t_datum;
  }   t_choice;
  ItemPointerData t_ctid;
  uint16  t_infomask2;
  uint16  t_infomask;
  uint8  t_hoff;
  bits8  t_bits[1];
} HeapTupleHeaderData; 
```

*(Note: In Appendix A, there are two reference tables listing the bit maps applied to the  two infomasks in this structure.  We discuss the purpose of many of these bit maps in the main body of this document, but the tables may be helpful for quick reference.*

*(Note: As far as I can tell, DatumTupleFields are not used for on-disk storage. Thus they don't need the xmin, xmax and other visibility information in HeapTupleFields. Instead they are used for in-memory tuples that represents a value of a composite type.*)

Many fields in this HeapTupleHeader relate to the visibility of the tuple. We will cover this separately in the next section. Besides the visibility information, a number of fields contain information about the size and contents of the tuple itself. For example, t\_hoff is the size of the header itself including the nulls bitmap and the padding which are variable length ( in other words t\_hoff is a pointer to be beginning of the user data).. The two infomasks (t\_infomask and t\_info\_mask2) contain a variety of information including the total number of attributes in the tuple, whether the tuple has an OID, and whether the tuple contains null attributes, variable-width attributes, externally stored attributes (see section on TOAST below), compressed attributes, etc. If the tuple does contain null attributes, t\_bits points to the beginning of a map showing which attributes are null.

Reading and interpreting the actual data in a tuple requires additional information stored in metadata tables such as pg\_attribute. For example, for each attribute, we need to know its length (attlen) and its alignment ( attalign). Depending on the flags, an attribute can be null, fixed width, variable width, stored externally in a TOAST table and/or compressed. The functions heap\_getattr, fastgetattr and heap\_getsysattr handle many of these details.

Heap Tuples Visibility Information
----------------------------------

Much of the information in the HeapTupleHeader relates to the visibility of this tuple both for the purposes of transactions and for the background vacuum process that removes unneeded tuples.
We can begin with the HeapTupleFields structure in src/include/access/htup.h.

**The HeapTupleFields structure defined in src/include/access/htup.h (t\_heap in the HeapTupleHeaderData)**

``` theme:
typedef struct HeapTupleFields
{
  TransactionId t_xmin;
  TransactionId t_xmax;

  union
  {
    CommandIdt_cid;
    TransactionId t_xvac;
  }   t_field3;
} HeapTupleFields; 
```

The t\_xmin field (often called simply Xmin) records the ID of the transaction that inserted this tuple. If the tuple has been deleted or updated, the t\_xmax field ( or Xmax) records the ID of the transaction that deleted the tuple. For example, if a tuple was inserted by transaction 40 and not deleted or updated, Xmin would contain 40 and Xmax would be empty (bit mask of HEAP\_XMAX\_INVALID in t\_infomask). If that same tuple is later deleted by transaction 47, then Xmax or Exp would be set to 47.

**INSERT**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Xmin</p></td>
<td align="left"><p>40</p></td>
</tr>
<tr class="even">
<td align="left"><p>Xmax<br />
</p></td>
<td align="left"><p>-<br />
</p></td>
</tr>
</tbody>
</table>

**DELETE**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Xmin</p></td>
<td align="left"><p>40</p></td>
</tr>
<tr class="even">
<td align="left"><p>Xmax<br />
</p></td>
<td align="left"><p>47</p></td>
</tr>
</tbody>
</table>

If instead of being deleted the tuple was updated by transaction 78, Xmax of the old version of the tuple would be set to 78 and a new version of the tuple would be written with Xmin set to 78.

**INSERT**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Xmin</p></td>
<td align="left"><p>40</p></td>
</tr>
<tr class="even">
<td align="left"><p>Xmax<br />
</p></td>
<td align="left"><p>-<br />
</p></td>
</tr>
</tbody>
</table>

**UPDATE**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Xmin</p></td>
<td align="left"><p>40</p></td>
</tr>
<tr class="even">
<td align="left"><p>Xmax<br />
</p></td>
<td align="left"><p>78</p></td>
</tr>
</tbody>
</table>

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Xmin</p></td>
<td align="left"><p>78</p></td>
</tr>
<tr class="even">
<td align="left"><p>Xmax<br />
</p></td>
<td align="left"><p>-<br />
</p></td>
</tr>
</tbody>
</table>

Adding some complexity, it is also possible for Xmax to refer not to a transaction that deleted or updated a tuple, but instead to a transaction that is locking a tuple. The t\_infomask field can be used to indicate that Xmax refers to a locking rather than deleting transaction by setting the masks HEAP\_XMAX\_EXCL\_LOCK or HEAP\_XMAX\_SHARED\_LOCK.

The t\_ctid field in HeapTupleHeaderData is also related to tuple visibility. Whenever a new tuple is stored on disk, its t\_ctid field is initialized with its own TID (tuple id or location). If the tuple is ever updated, its t\_ctid is changed to point to the replacement version of the tuple. One can follow the chain of t\_ctid links to find the newest version of the row. Thus, a tuple is the latest version of its row iff XMAX is invalid or t\_ctid points to itself (in which case, if XMAX is valid, the tuple is either locked or deleted).

The t\_infomask field can also be used to indicate when the transaction pointed to by Xmin has committed (HEAP\_XMIN\_COMMITTED), when the transaction pointed to by Xmax has committed (HEAP\_XMAX\_COMMITTED), when the heap tuple has been updated (HEAP\_UPDATED).

In order to handle transactions with multiple commands or statements, the situation gets a bit trickier for in-progress transactions. During the lifetime of the transaction, the t\_cid field in HeapTupleFields (note: this is different than t\_ctid in the HeapTupleHeaderData) is used to identify the command that inserted (Cmin) or deleted (Cmax) the tuple. (From <a href="http://momjian.us/main/writings/pgsql/mvcc.pdf" class="uri" class="external-link">http://momjian.us/main/writings/pgsql/mvcc.pdf</a> : Multi-statement transactions require extra tracking because each statement has its own visibility rules. For example, a cursor's contents must remain unchanged even if later statements in the same transaction modify rows. Such tracking is implemented using system command id columns Cmin/Cmax, which internally actually is a single column.) If a tuple is inserted and deleted in the same transaction, there is a special trick to map t\_cid to both Cmin and Cmax by using a special hash table stored in memory in the backend database. Specifically, the value if t\_cid is used to index into the in-memory hash table to obtain both Cmin and Cmax. The field t\_infomask is set to indicate that HEAP\_COMBOCID is true in this case.

If that wasn't already complicated enough, the same storage space within the heap tuple (t\_cid) can also used by vacuum for a completely different purpose (t\_xvac). This is possible because vacuum only works on tuples that are not being used by an in-progress transaction and therefore would not be storing any Cmin or Cmax information. There is more information about how vacuum uses t\_cid in the next section.

We have now reviewed a variety of visibility related data stored in the heap tuple. We have also discussed some alternate, non-visibility related uses of these same fields. We are now ready to discuss how the visibility information is used in relation to a transaction's snapshot data. In particular, in order to determine if a given tuple is visible to a given transaction, the tuple's visibility information is compared against a transaction's snapshot information:

**The The SnapshotData structure defined in include/utils/tqual.h**

``` theme:
typedef struct SnapshotData
{
  TransactionId xmin;
  TransactionId xmax;
  uint32        xcnt;
  TransactionId *xip;
  int32         subxcnt;
  TransactionId *subxip;
  CommandId     curcid;
  bool          haveDistribSnapshot;
  DistributedSnapshotWithLocalMapping distribSnapshotWithLocalMapping;
} SnapshotData; 
```

In the SnapshotData for a transaction, any transaction below xmin is visible to this transaction and any transaction &gt;= xmax is invisible. xip contains a list of inprogress transactions and xcnt is the number of transaction ids in the xip list. subxip contains a list of sub transaction ids that are in progress and subxcnt is the number of sub-transaction ids in the subxip list. Note all ids in the xip list satisfy xmin &lt;= xip\[i\] &lt; xmax. All ids is the subxip list are xmin &lt;= subxip\[i\], but may not be &lt; xmax.
The following picture from <a href="http://www.postgresql.org/files/developer/internalpics.pdf" class="uri" class="external-link">http://www.postgresql.org/files/developer/internalpics.pdf</a> illustrates how this transaction information is used to determine the visibility of specific tuples during a sequential scan (Cre = Xmin and Exp= Xmax for each tuple):
<img src="attachments/43090511/43221006.png" class="confluence-embedded-image" width="576" height="371" />
Tuples that a visible to a given query or transaction must have a creation transaction id (Cre = t\_xmin) that is a committed transaction and the tuple's t\_xmin must be less than the transaction counter stored at the query's start and the tuple's t\_xmin may not refer to a transaction that was in-progress at query start. Visible tuples must also have an expire transaction id (Exp = t\_xmax) that is blank or aborted or the tuple's t\_xmax must be greater than the transaction counter stored at the query's start or the tuple's t\_xmax must refer to a transaction that was in-progress at the query's start.

The picture shows 6 tuples – three visible and three invisible. Starting at the top of the picture, the first tuple (with Cre=30 and Exp= invalid) is visible because Cre= 30 &lt; 100 and Exp is invalid. The second tuple (with Cre=30 and Exp= 80) is invisible because Cre and Exp &lt; 100. The third tuple ( with Cre=30 and Exp=110) is visible because Cre &lt; 100 but Exp &gt; 100. The fourth tuple ( with Cre=30 and Exp= 75) is visible because Cre &lt; 100 and even though Exp &lt; 100 , Exp = 75 is also in the list of in-progress transactions. The fifth tuple ( with Cre = 50 and Exp = invalid) us invisible because even though Cre &lt; 100 and Exp is invalid, Cre=50 is also in the list of in-progress transactions. Finally, the sixth tuple ( with Cre=110 and Exp invalid) is invisible because Cre &gt; 100.

The file src/backend/utils/time/tqual.c contains a number of routines to determine the visibility of tuples update the heap tuple hint bits such as HeapTupleSatisfiesMVCC, HeapTupleSatisfiesNow, HeapTupleSatisfiesUpdate, HeapTupleSatisfiesSelf, HeapTupleSatisfiesDirty, HeapTupleSatisfiesVacuum, HeapTupleSatisfiesToast and HeapTupleSatisfiesAny.
For GPDB's distributed queries, there is additional snapshot data kept about each distributed transaction. Notice the haveDistribSnapshot and distribSnapshotWithLocalMapping fields in the SnapshotData structure. Also, the field, t\_infomask2, in the HeapTupleHeaderData contains some information related to GPDB's distributed queries. Specifically, it can be used to indicate when t\_xmin or t\_xmax can be ignored when processing distributed snapshots. This would happen for example when the transaction pointed to by Xmin or Xmax is local only transaction or when it is from an old committed distributed transaction.

Vacuuming
---------

The background vacuum process removes tuples (also item pointers and index entries) that will no longer visible to any transactions ( i.e. they've been deleted or a newer version exists after an update). The details of vacuum are beyond the scope of this document, but a few notes about vacuum as it relates to the heap tuple format are in order.
The t\_infomask structure can be use to indicate when vacuum has moved the tuple to another place (HEAP\_MOVED\_OFF) and when vacuum has moved the tuple from another place (HEAP\_MOVED\_IN).

It is also worth noting that vacuum might erase the pointed-to (newer) tuple before erasing the pointing (older) tuple. Hence, when following a t\_ctid link, it is necessary to check to see if the referenced slot is empty or contains an unrelated tuple. Specifically, one can check that the referenced tuple has XMIN equal to the referencing tuple's XMAX to verify that it is actually the descendant version and not an unrelated tuple stored into a slot recently freed by VACUUM. If either check fails, one may assume that there is no live descendant version.

TOAST
-----

***\* Much of this section copied from:*** ***<a href="http://www.postgresql.org/docs/8.0/static/storage-toast.md" class="uri" class="external-link">http://www.postgresql.org/docs/8.0/static/storage-toast.md</a>***

Heap pages are a fixed size and do not allow tuples to span multiple pages. The Oversized-Attribute Storage Technique or TOAST is used to enable rows that are larger than could fit in a single page. TOAST is only available for data types that have a variable-length (varlena) representation. This means that the first 32-bit work of the stored value contains the total length of the stored value in bytes.

TOAST usurps the high-order two bits of this valena format. When bits are zero, the value is an ordinary un-TOASTed value of the data type. One of these bits, if set, indicates that the value has been compressed and must be decompressed before use. The other bit, if set, indicates that the value has been stored out-of-line. In this case the remainder of the value is actually just a pointer, and the correct data has to be found elsewhere. When both bits are set, the out-of-line data has been compressed too. In each case the length in the low-order bits of the varlena word indicates the actual size of the datum, not the size of the logical value that would be extracted by decompression or fetching of the out-of-line data.
If any of the columns of a table are TOAST-able, the table will have an associated TOAST table, whose OID is stored in the table's pg\_class.reltoastrelid entry. Out-of-line TOASTed values are kept in the TOAST table.

The compression technique used is a fairly simple and very fast member of the LZ family of compression techniques. See src/backend/utils/adt/pg\_lzcompress.c for the details.
Out-of-line values are divided (after compression if used) into chunks of at most TOAST\_MAX\_CHUNK\_SIZE bytes (this value is a little less than BLCKSZ/4, or about 2000 bytes by default). Each chunk is stored as a separate row in the TOAST table for the owning table. Every TOAST table has the columns chunk\_id (an OID identifying the particular TOASTed value), chunk\_seq (a sequence number for the chunk within its value), and chunk\_data (the actual data of the chunk). A unique index on chunk\_id and chunk\_seq provides fast retrieval of the values. A pointer datum representing an out-of-line TOASTed value therefore needs to store the OID of the TOAST table in which to look and the OID of the specific value (its chunk\_id). For convenience, pointer datums also store the logical datum size (original uncompressed data length) and actual stored size (different if compression was applied). Allowing for the varlena header word, the total size of a TOAST pointer datum is therefore 20 bytes regardless of the actual size of the represented value.

The TOAST code is triggered only when a row value to be stored in a table is wider than BLCKSZ/4 bytes (normally 2Kb). The TOAST code will compress and/or move field values out-of-line until the row value is shorter than BLCKSZ/4 bytes or no more gains can be had. During an UPDATE operation, values of unchanged fields are normally preserved as-is; so an UPDATE of a row with out-of-line values incurs no TOAST costs if none of the out-of-line values change.

Operations on Heap tables
=========================

Quick Overview of Query Processing
----------------------------------

To give some context for the sections to follow, this section gives a brief overview of query processing starting in the main function.
The main function calls PostgresMain, which has the infinite loop accepting all new query requests in turn. Depending on the nature of the request, PostgresMain calls a variety of exec functions such as exec\_simple\_query or exec\_mpp\_query.

``` theme:
main (src/backend/main/main.c)
   PostgresMain(src/backend/tcop/postgres.c )
      exec_mpp_query  (src/backend/tcop/postgres.c)
      exec_simple_query (src/backend/tcop/postgres.c)
      exec_execute_message (src/backend/tcop/postgres.c)
      exec_parse_message (src/backend/tcop/postgres.c)
```

These exec queries typically take a char \* of the query string, parse the query string and plan the execution of the query. Once a plan has been created, there will be a call to PortalStart and then a call to PortalRun.  PortalRun will actually run the query based on the plan that was created.

``` theme:
PortalRun(src/backend/tcop/pquery.c )
   PortalRunMulti(src/backend/tcop/pquery.c )
      ProcessQuery(src/backend/tcop/pquery.c )
```

The actual processing of the query starts with ProcessQuery. ProcessQuery calls CreateQueryDesc (also in src/backend/tcop/pquery.c ) to build a QueryDesc object. This query object is passed in turn to ExecutorStart, ExecutorRun and ExecutorEnd from src/backend/executor/execMain.c.

``` theme:
ProcessQuery(src/backend/tcop/pquery.c )
   queryDesc = CreateQueryDesc (src/backend/executor/execMain.c)
   ExecutorStart (queryDesc) (src/backend/executor/execMain.c)
   ExecutorRun(querDesc) (src/backend/executor/execMain.c)
   ExecutorEnd(queryDesc) (src/backend/executor/execMain.c) 
```

In the rest of this document, we will begin our discussion with one of these high level executor functions, typically ExecutorRun.

Inserting Heap Data
-------------------

The primary function for inserting data into heap tables is ExecInsert in src/backend/executor/execMain.c. This is called from ExecutorRun in the following way:

``` theme:
ExecutorRun (src/backend/executor/execMain.c)
  ExecutePlan (src/backend/executor/execMain.c)
    ExecInsert (src/backend/executor/execMain.c) 
```

ExecInsert handles both inserting the tuple into the base relation and the appropriate tuples into the index relations as described in the following two sections.

### Insert into Base Relation

In order to insert tuple data into heap table, ExecInsert calls heap\_insert in src/backend/access/heap/heapam.c.

heap\_insert first calls RelationGetBufferForTuple which looks for a page with sufficient free space for the new tuple. Pages are not completely filled with newly inserted data. Some of the space is left open, how much is determined by a fill factor, so that updates to tuples may be able to stay in the same page.

RelationGetBufferForTuple first tries to put the new tuple on same page where the last tuple was inserted for this relation. If that doesn't work, the Free Space Map (FSM) is consulted to locate a suitable page (GetPageWithFreeSpace). The FSM's information is not kept perfectly up-to-date and therefore the FSM may return a page that is too full. If that happens, the FSM information for that page will be updated and the FSM will be asked again for a new candidate page. If the FSM has no record of a page with sufficient free space, then the relation is extended.

It is also possible to pass in a parameter that directs RelationGetBufferForTuple to ignore the FSM. In this case, the tuple will either be placed on the last page used or if that is too full, the relation will be automatically extended.

If needed, the relation is extended with a call to ReadBuffer with a parameter P\_NEW. This will eventually call smgrextend and then mdextend and \_mdmir\_getseg which will actually open a new segment file if necessary to keep size of file under RELSEG\_SIZE.
RelationGetBufferForTuple will lock the buffer chosen before returning. If the relation was extended, it will get the lock on the buffer before releasing relation extension lock to prevent a race condition with vacuumlazy.

Most of the interesting work occurred in RelationGetBufferForTuple. When that returns, all that remains is to actually do the insertion into the locked buffer with RelationPutHeapTuple and PageAddItem.

**Overview of Heap\_insert and the functions it calls**

``` theme:
heap_insert (src/backend/access/heap/heapam.c)
  RelationGetBufferForTuple(src/backend/access/heap/hio.c)
    RelationGetTargetPageFreeSpace
    GetPageWithFreeSpace
    RecordAndGetPageWithFreeSpace
    LockRelationForExtension (src/backend/storage/lmgr/lmgr.c)
    ReadBuffer (src/backend/storage/buffer/bufmgr.c)
      smgrextend (src/backend/storage/smgr/smgr.c)
        mdextend(src/backend/storage/smgr/md.c)
          _mdmir_getseg (src/backend/storage/smgr/md.c)
    UnlockRelationForExtension (src/backend/storage/lmgr/lmgr.c)
  RelationPutHeapTuple(src/backend/access/heap/hio.c)
    PageAddItem(src/backend/storage/page/bufpage.c) 
```

### Insert Into Indices

In order to insert index entries for rows newly inserted into a heap table, ExecInsert calls ExecInsertIndexTuples in src/backend/executor/execUtils.c. ExecInsertIndexTuples calls index\_insert for each index on the relation. index\_insert calls GET\_REL\_PROCEDURE(aminsert) to determine the access method's insert procedure for this index and then calls that routine to do the heavy lifting.

``` theme:
ExecInsert (src/backend/executor/execMain.c)
ExecInsertIndexTuples (src/backend/executor/execUtils.c)
   index_insert (src/backend/access/index/indexam.c)
      GET_REL_PROCEDURE
      access method's insert procedure 
```

The call to the access method's insert procedure is done through a function call table as illustrated here.

``` theme:
/* have the am's insert proc do all the work. */
result = DatumGetBool(FunctionCall6(procedure, PointerGetDatum(indexRelation),
                                               PointerGetDatum(values),
                                               PointerGetDatum(isnull),
                                               PointerGetDatum(heap_t_ctid),
                                               PointerGetDatum(heapRelation), BoolGetDatum(check_uniqueness))); 
```

In src/include/utils/rel.h, the RelationAmInfo structure lists the functions that each index access method should define including aminsert:

``` theme:
typedef struct RelationAmInfo
{
  FmgrInfoaminsert;
  FmgrInfoambeginscan;
  FmgrInfoamgettuple;
  FmgrInfoamgetmulti;
  FmgrInfoamrescan;
  FmgrInfoamendscan;
  FmgrInfoammarkpos;
  FmgrInfoamrestrpos;
  FmgrInfoambuild;
  FmgrInfoambulkdelete;
  FmgrInfoamvacuumcleanup;
  FmgrInfoamcostestimate;
  FmgrInfoamoptions;
} RelationAmInfo;
```

From src/include/catalog/pg\_am.h, the current choices for aminsert are B-tree Index, Hash Index, GiST Index, GIN Index and Bitmap Index. The following table identifies the main insert functions for each of these access methods. This document will not elaborate on the details of any of these access method specific insert functions.

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p>Access Method</p></th>
<th align="left"><p>Insert functions</p></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td align="left"><p>B-tree Index</p></td>
<td align="left"><p>btinsert (src/backend/access/nbtree/nbtree.c)<br />
_bt_doinsert (src/backend/access/nbtree/nbtinsert.c)</p></td>
</tr>
<tr class="even">
<td align="left"><p>Hash Index</p></td>
<td align="left"><p>hashinsert (src/backend/access/hash/hash.c)<br />
_hash_doinsert (src/backend/access/hash/hashinsert.c)<br />
</p></td>
</tr>
<tr class="odd">
<td align="left"><p>GiST Index</p></td>
<td align="left"><p>gistinsert (src/backend/access/gist/gist.c)<br />
gistdoinsert(src/backend/access/gist/gist.c)</p></td>
</tr>
<tr class="even">
<td align="left"><p>GIN Index</p></td>
<td align="left"><p>gininsert(src/backend/access/gin/gininsert.c)<br />
ginHeapTupleInsert(src/backend/access/gin/gininsert.c)</p></td>
</tr>
<tr class="odd">
<td align="left"><p>Bitmap Index</p></td>
<td align="left"><p>bminsert (src/backend/access/bitmap/bitmap.c)<br />
_bitmap_doinsert(src/backend/access/bitmap/bitmapinsert.c)</p></td>
</tr>
</tbody>
</table>

### Write-Ahead Logging

When heap\_insert is called, there is a parameter use\_wal, that indicates whether or not the new tuple should be logged in the write-ahead log (WAL). Safe usage of the write-ahead log requires that all new tuples go into new pages not containing any tuples from other transactions, that the relation gets fsync'd before commit, and that the transaction emits at least one WAL record to ensure RecordTransactionCommit will decide to WAL-log the commit.

When use\_wal is true, heap\_insert calls the function heap\_log\_tuple\_insert where an array of XLogRecData is filled out to represent this tuple.

``` theme:
heap_insert (src/backend/access/heap/heapam.c
  heap_log_tuple_insert (src/backend/access/heap/heapam.c)
    XLogInsert (src/backend/access/transam/xlog.c) 
```

The details of the Write-Ahead Logging record format is beyond the scope of this document.

XLogInsert is called to insert the new log record into the log. If the tuples is frozen, then XLogInsert\_OverrideXid is called instead. The LSN and TLI of the page containing the tuple are set to the recptr returned by XLogInsert and ThisTimeLineID respectively. ThisTimeLineID will be the same in all backends, it identifies the current WAL timeline for the database system as a whole.

### Special Insert Functions

There are a few special variants of calls to heap\_insert that are worth mentioning: simple\_heap\_insert, frozen\_heap\_insert and frozen\_heap\_insert\_directed.

**simple\_heap\_insert** (src/backend/access/heap/heapam.c)

Calls heap\_insert but supplies a default command ID. Used rather than calling heap\_insert directly in most places where we are modifying system catalogs.

**frozen\_heap\_insert** and **frozen\_heap\_insert\_directed** (src/backend/access/heap/heapam.c)

Insert a heap tuple and freeze it so it is always visible. Calls heap\_insert but supplies a default command ID and a frozen transaction id. Also, the committed bit of the tuple is set. This function is currently used for data that goes into error tables and need to stay there even if transaction aborts. Both frozen\_heap\_insert and frozen\_heap\_insert\_directed use the write-ahead log. frozen\_heap\_insert uses the Free Space Map (FSM), but frozen\_heap\_insert\_directed does not.

Updating Heap Data
------------------

The primary function for inserting data into heap tables is ExecUpdate in src/backend/executor/execMain.c. This is called from ExecutorRun in the following way:

**ExecUpdate**

``` theme:
ExecutorRun (src/backend/executor/execMain.c)
   ExecutePlan (src/backend/executor/execMain.c)
      ExecUpdate (src/backend/executor/execMain.c) 
```

ExecUpdate handles both updating the tuple into the base relation and updating any indices on the table accordingly.
Updates are basically treated as a delete followed up an insert.

### Updating Data in Base Relation

The updated tuple is treated like a new tuple insert. If there is room in the same page as the original tuple, the updated tuple is put there. If not it is put in another page. As discussed earlier, vacuum takes care of actually removing the old tuple eventually.
Exec\_Update and some important functions it calls are outlined here:

``` theme:
ExecUpdate (src/backend/executor/execMain.c
   heap_update (src/backend/access/heap/heapam.c)
      heap_update_internal (src/backend/access/heap/heapam.c)
         HeapTupleSatisfiesUpdate (src/backend/utils/time/tqual.c)
         toast_insert_or_update (src/backend/access/heap/tuptoaster.c)
         RelationPutHeapTuple(src/backend/access/heap/hio.c)
            PageAddItem(src/backend/storage/page/bufpage.c)
         log_heap_update (src/backend/access/heap/heapam.c)
            XLogInsert (src/backend/access/transam/xlog.c)
         MarkBufferDirty(src/backend/storage/buffer/bufmgr.c)
         CacheInvalidateHeapTuple (src/backend/utils/cache/inval.c) 
```

heap\_update\_internal first reads and locks the buffer containing the old tuple that is being updated and reads in a copy of the old tuple data. It checks whether the old tuple is eligible for an update using the function HeapTupleSatisfiesUpdate. If the old tuple is invisible, it cannot be updated and an error is returned. If the tuple is already being updated, then this update may need to wait until it can safely proceed.

When all the proper locks can be acquired guaranteeing exclusive access, it proceeds with building the new tuple properly including setting HEAP\_XMAX\_INVALID and HEAP\_UPDATED in the new tuple's t\_infomask and setting the Xmin and Cmin of the new tuple to represent the current transaction and command respectively. Xmax of the new tuple is set to invalid both by setting HEAP\_XMAX\_INVALID and my setting t\_xmax to 0 for cleanliness.

If the there are external attributes or if the new tuple will not fit on the same page as the old tuple, then it is necessary to invoke the toaster by calling toast\_insert\_or\_update. toast\_insert\_or\_update deletes no longer needed toast-entries and creates new ones to make a new tuple fit. In order to call, toast\_insert\_or\_update, the lock on the old tuple's buffer must be released and then reacquired. The tuple itself is marked as "being updated" so that no other processes try to update that tuple in the meantime.

When all of this preparation work is complete, the buffer containing the old tuple (buffer) and the buffer that will contain the new tuple (newbuf) are pinned and locked (buffer can be the same as newbuf in some cases). Newbuf must also have enough free space to store the new tuple. RelationPutHeapTuple is called to actually write the new tuple into the new buffer.

Various fields of the old tuple are marked to reflect the change. For example, Xmax and Cmax are set to represent the current transaction and command respectively. Also, t\_ctid of the old tuple is set to the address of the new tuple.

The buffers containing the old tuple and new tuple are marked as dirty with MarkBufferDirty. It marks the buffer contents as dirty, but the actual writes happen later.
An Xlog record is written and the in the process an XLogRecPtr is returned. The LSN of the pages containing the old tuple and new tuple are marked with this XLogRecPtr and the TLI of these pages are set to reflect ThisTimeLineID.

The old and new tuples are marked for cache invalidation by calling the function CacheInvalidateHeapTuple. There are some subtle aspects of this as described in a comment in src/backend/utils/cache/inval.c. (The remainder of this paragraph is taken from that comment. See the comment for even more detail.) When a tuple is updated or deleted, the standard time qualification rules consider that it is **still valid** as long as we are in the same command, ie, until the next CommandCounterIncrement() or transaction commit. At the command boundary, the old tuple stops being valid and the new version, if any, becomes valid. Therefore, we cannot simply flush a tuple from the system caches during heap\_update() or heap\_delete(). The tuple is still good at that point; what's more, even if we did flush it, it might be reloaded into the caches by a later request in the same command. So the correct behavior is to keep a list of outdated (updated/deleted) tuples and then do the required cache flushes at the next command boundary. We must also keep track of inserted tuples so that we can flush "negative" cache entries that match the new tuples; again, that mustn't happen until end of command. Once we have finished the command, we still need to remember inserted tuples (including new versions of updated tuples), so that we can flush them from the caches if we abort the transaction. Similarly, we'd better be able to flush "negative" cache entries that may have been loaded in place of deleted tuples, so we still need the deleted ones too.

The last action in heap\_update\_internal are related to releasing all buffers and locks that were acquired.

### Updating Indices

Indices are updated in exactly the same way as with inserts. ExecUpdate called ExecInsertIndexTuples exactly like ExecInsert.

``` theme:
ExecUpdate (src/backend/executor/execMain.c)
   ExecInsertIndexTuples (src/backend/executor/execUtils.c) 
```

Vacuum handles removing any of the old index entries. In the meantime, an index can physically contain pointers to multiple versions of the same row. However, visibility rules should enforce that no MVCC snapshot could include two rows with equal index keys.

### Write-Ahead Logging

If the relation is not a temporary relation (!relation-&gt;rd\_istemp), then the update must be written to the write-ahead log (WAL). As with inserts, an array of XlogRelData is initialized to represent the tuple, XLogInsert or XLogInsert\_OverrideXid are called and the LSN and TLI of the page containing the tuple are set.

### Special Updates

There are a few special variants of calls to heap\_update\_internal that are worth mentioning: simple\_heap\_update, frozen\_heap\_inplace\_update and heap\_inplace\_update\_internal.

**simple\_heap\_update** (src/backend/access/heap/heapam.c)

This routine may be used to update a tuple when concurrent updates of the target tuple are not expected (for example, because we have a lock on the relation associated with the tuple).

**heap\_inplace\_update\_internal** (src/backend/access/heap/heapam.c)

This routine allows a tuple to be updated in place – i.e. overwritten rather than inserting a brand new version of the tuple in a new spot and marking the old tuple to reflect that it was been superseded. Overwriting violates both MVCC and transactional safety, so the uses of this function must be extremely limited. For this to work, the tuple cannot change size, and therefore it's reasonable to assume that its null bitmap (if any) doesn't change either. So we just overwrite the data portion of the tuple without touching the null bitmap or any of the header fields.

**frozen\_heap\_inplace\_update** (src/backend/access/heap/heapam.c)

Calls heap\_inplace\_update\_internal with the FrozenTransactionId rather than the current transaction id.

Delete
------

Delete is much like update except there is no new tuple to deal with and no index modifications are needed. Vacuum will handle deleting the old tuple and index entries later. We can't delete index tuples immediately anyway because the tuple could still be visible to other transactions.

``` theme:
ExecutorRun (src/backend/executor/execMain.c)
   ExecutePlan (src/backend/executor/execMain.c)
      ExecDelete (src/backend/executor/execMain.c)
         heap_delete(src/backend/access/heap/heapam.c)
            HeapTupleSatisfiesUpdate (src/backend/utils/time/tqual.c)
            MarkBufferDirty(src/backend/storage/buffer/bufmgr.c)
            XLogInsert (src/backend/access/transam/xlog.c)
            toast_delete(src/backend/access/heap/tuptoaster.c)
               heap_deform_tuple(src/backend/access/common/heaptuple.c)
               toast_delete_datum(src/backend/access/heap/tuptoaster.c)
            CacheInvalidateHeapTuple (src/backend/utils/cache/inval.c) 
```

As we saw with heap\_update\_internal, heap\_delete reads and locks the buffer containing the tuple to be deleted. It checks whether the old tuple is eligible for an update using the function HeapTupleSatisfiesUpdate. If the old tuple is invisible, it cannot be deleted and an error is returned. If the tuple is already being updated, then the delete may need to wait until it can safely proceed.

Various fields of the tuple are marked to reflect the fact that it is being deleted. For example, Xmax and Cmax are set to represent the current transaction and command respectively. Also, t\_ctid of the old tuple is set to itself to make sure there is no more forwarding chain link.
The buffer containing the tuple is marked as dirty with MarkBufferDirty. Again, the buffer is simply marked as dirty. The actual writes will happen later.
An Xlog record is written and the in the process an XLogRecPtr is returned. The LSN of the page the deleted tuple is marked with this XLogRecPtr and the TLI of the page is set to reflect ThisTimeLineID.

If the tuple had external attributes, toast\_delete is called to remove those as well.
The tuple is marked for cache invalidation with CacheInvalidateHeapTuple.
The last action in heap\_delete related to releasing all buffers and locks that were acquired.

Scanning Heap Relations
-----------------------

### Scan Without Index

When performing a sequential scan on a heap table, we start in ExecutorRun. The path from ExecutorRun to ExecSeqScan is shown here:

``` theme:
ExecutorRun (src/backend/executor/execMain.c)
   ExecutePlan (src/backend/executor/execMain.c)
      ExecProcNode (src/backend/executor/execProcnode.c)
         ExecSeqScan (src/backend/executor/nodeSeqscan.c) 
```

The state of the scan is tracked using the SeqScanState struct defined in src/include.nodes/execnodes.h:

**ScanState structure**

``` theme:
typedef struct ScanState
{
  PlanState        ps;
  Relation        ss_currentRelation;
  struct HeapScanDescData *ss_currentScanDesc;
  struct TupleTableSlot  *ss_ScanTupleSlot;
  int scan_state;
  struct {
    HeapTupleData item[512];
    int bot, top;
    HeapTuple last;
    int seen_EOS;
  } ss_heapTupleData;
} ScanState;

typedef ScanState SeqScanState; 
```

When ExecSeqScan is first called, it will call OpenScanRelation. OpenScanRelation will set ss\_heapTupleData.seen\_EOS to 0 to indicate that the end of the scan has not been reached and ss\_heapTupleData.last to NULL to indicate that this scan has not yet seen any tuples. It sets the scan\_state to SCAN\_SCAN to show that it is currently scanning. It calls heap\_beginscan to allocation and initialize the ss\_currentScanDesc.

The workhorse of ExecSeqScan is the routine SeqNext. Specifically, ExecSeqScan calls ExecScan and passes it the access method, SeqNext. SeqNext calls heap\_getnextx which calls heapgettup which calls heapgetpage. Heapgetpage actually reads and pins a specific page of a heap relation.

There are actually two versions of heapgettup – one which gets one tuple (heapgettup) and one which gets a whole page of tuples (heapgettup\_pagemode). When heapgetpage is called from the page-at-a-time version (heapgettup\_pagemode), it does additional work to determine which tuples on the page are actually visible using the routine HeapTupleSatisfiesVisibility. The single tuple version (heapgettup) will call HeapTupleSatisfiesVisibility itself rather than relying on heapgetpage to do it.

As it goes along SeqNext and the routines it calls update the scan state so that the SeqScanState records where the last call to SeqNext stopped and where the next call to SeqNext should pick up. It can fetch and store multiple tuples in the scan tuple slots in the SeqScanState structure so that they are already ready when SeqNext is called again.
When ExecSeqScan detects the end of the scan, it will call CloseScanRelation. CloseScanRelation calls heap\_endscan to deallocate the ss\_currentScanDesc. It sets ss\_heapTupleData.seen\_EOS to indicate the end of the scan has been reached and resets scan\_state to SCAN\_INIT.

The main code path for sequential scan of a heap table starting with ExecSeqScan is outlined here:

``` theme:
ExecSeqScan (src/backend/executor/nodeSeqscan.c)
   OpenScanRelation (src/backend/executor/nodeSeqscan.c)
     heap_beginscan (src/backend/access/heap/heapam.c)
   ExecScan (src/backend/executor/execscan.c)
      SeqNext (src/backend/executor/nodeSeqscan.c)
         heap_getnextx (src/backend/access/heap/heapam.c)
            heapgettup  (src/backend/access/heap/heapam.c)
               heapgetpage (src/backend/access/heap/heapam.c)
                  HeapTupleSatisfiesVisibility (src/backend/utils/time/tqual.c)
   CloseScanRelation (src/backend/executor/nodeSeqscan.c)
      heap_endscan (src/backend/access/heap/heapam.c)
```

### Scan With Index

Scans of heap table using an index start out in much the same way as sequential scans. The path from ExecutorRun to ExecIndexScan is shown here:

``` theme:
ExecutorRun (src/backend/executor/execMain.c)
   ExecutePlan (src/backend/executor/execMain.c)
      ExecProcNode (src/backend/executor/execProcnode.c)
         ExecIndexScan (src/backend/executor/nodeIndexscan.c)
```

ExecIndexScan calls ExecScan and passes it the access method, IndexNext. Picking up where we left in ExecIndexScan, the rest of the path is shown here:

``` theme:
ExecIndexScan (src/backend/executor/nodeIndexscan.c)
   ExecScan (src/backend/executor/execscan.c)
      IndexNext (src/backend/executor/nodeIndexscan.c)
         index_getnext (src/backend/access/index/indexam.c)
            heap_release_fetch (src/backend/access/heap/heapam.c)
               HeapTupleSatisfiesVisibility (src/backend/utils/time/tqual.c)
```

The state of the scan is tracked in an IndexScanState structure as defined in src/include.nodes/execnodes.h:

``` theme:
typedef struct IndexScanState
{
  ScanState         ss;
  List             *indexqualorig;
  ScanKey           iss_ScanKeys;
  int               iss_NumScanKeys;
  IndexRuntimeKeyInfo   *iss_RuntimeKeys;
  int                iss_NumRuntimeKeys;
  bool             iss_RuntimeKeysReady;
  ExprContext      *iss_RuntimeContext;
  Relation         iss_RelationDesc;
  struct IndexScanDescData *iss_ScanDesc;
} IndexScanState; 
```

Notice that the IndexScanState contains a basic ScanState structure in it. It adds fields to track the keys used in scanning the index relation.

As SeqNext used the SeqScanState structure to record where to pick up on the next call, so IndexNext uses an IndexScanState structure for the same purpose. It can fetch and store multiple tuples in the scan tuple slots in the IndexScanState structure so that they are already ready when IndexNext is called again.

IndexNext calls index\_getnext which actually gets the next heap tuple that satisfies the visibility of the current snapshot. The buffer containing the next tuple is pinned.

It is worth calling out how the index scan is opened. Here is the high level code path for open:

``` theme:
ExecutorStart (src/backend/executor/execMain.c)
  InitPlan (src/backend/executor/execMain.c)
    ExecInitNode (src/backend/executor/execProcnode.c)
      ExecInitIndexScan (src/backend/executor/nodeIndexscan.c)
        ExecOpenScanRelation (src/backend/executor/execUtils.c)
          heap_open (src/backend/access/heap/heapam.c)
            relation_open (src/backend/access/heap/heapam.c)
          index_open (src/backend/access/index/indexam.c)
            relation_open (src/backend/access/heap/heapam.c)
          ExecIndexBuildScanKeys(src/backend/executor/nodeIndexscan.c) 
```

ExecOpenScanRelation opens the base heap relation, index\_open opens the index relation, and ExecIndexBuildScanKeys establishes a set of compare operators used to determine which tuples will need to be returned based on the matching constraints specified in the query.

### Scans and Changes to Heap Tuples

Earlier in this document, we discussed how MarkBufferDirty is called when changes are made to a buffer containing a heap page. For example, in heap\_insert, after the tuple is actually written into the buffer with RelationPutHeapTuple, there is a call to MarkBufferDirty. The buffer is marked as dirty but the actual writes from memory to disk happen later.
It is important to understand that reads can also cause the heap tuples to change and buffers to be marked dirty. To understand why this happens, we can look at the HeapTupleSatisfies\* routines in src/backend/utils/time/tqual.c (e.g. HeapTupleSatisfiesDirty\_Internal, HeapTupleSatisfiesItself\_Internal, etc.). During a scan/read, these routines are called to determine the visibility of a tuple and possibly to change the visibility hint bits in the tuple and to indicate whether certain transactions such as xmin or xmax have completed. For example, HEAP\_XMAX\_INVALID may be set if the scan detects that Xmax must have crashed or aborted (not in progress and didn't commit, etc.).

When these changes in heap tuples are made during a scan, the buffer containing these modified tuples is marked dirty with the routine markDirty.

``` theme:

markDirty (src/backend/utils/time/tqual.c)

SetBufferCommitInfoNeedsSave (src/backend/storage/buffer/bufmgr.c)
```

SetBufferCommitInfoNeedsSave is essentially the same as MarkBufferDirty except that the caller can have only a share-lock instead of an exclusive-lock on the buffer's content lock. This acceptable because like fundamental changes to the heap tuple's data, these status bit changes are simply hints and could be redone by another reader later. Similarly, no xlog record needs to be generated where calls to MarkBufferDirty should be associated with an action that will be generating an xlog record.

Similarly, there is a guc, gp\_disable\_tuple\_hints, that controls whether the buffer is even set to dirty when these hint bit changes are made. With the guc on, SetBufferCommitInfoNeedSave will not be called in many cases. With the guc off, it will be called and therefore reads/scans of heap relations can cause buffers to be dirtied and written out as described.

With the guc off, a common scenario is that the first read/scan after an update will once again dirty the tuples as hint bits are updated. Thus the first read/scan after an update will take much longer than subsequent reads/scans.

Creating a Heap Table
---------------------

This section discusses the creation of heap tables – first when and why heap tables are created and then second, how they are created with the functions heap\_create and heap\_create\_with\_catalog.

### When Heap Tables Are Created

There are a number of times where heap tables are created. CREATE TABLE or SELECT INTO are used to create regular heap relations. CLUSTER creates a new reorganized version of an existing table. Many important "metadata tables" such as pg\_aoseg\*, pg\_aocssseg\*, pg\_aoblkdir\*, toast tables and certain bootstrap catalog tables are heap tables. Finally, indices are actually stored as additional heap tables.

#### CREATE TABLE (User Defined Table)

In the case of CREATE TABLE commands for creating new heap tables, PortalRunMulti which we saw calling ProcessQuery for other queries instead calls PortalRunUtility and then ProcessUtility.

``` theme:
PortalRun(src/backend/tcop/pquery.c )
  PortalRunMulti(src/backend/tcop/pquery.c )
    PortalRunUtility (src/backend/tcop/pquery.c )
      ProcessUtility(src/backend/tcop/utility.c )
        DefineRelation (src/backend/commands/tablecmds.c)
          DefineRelation_int (src/backend/commands/tablecmds.c)
            heap_create_with_catalog (src/backend/catalog/heap.c) 
```

#### SELECT INTO (User Defined Table)

A SELECT INTO statement can be used to create a new heap table as well. This path calls heap\_create\_with\_catalog from InitPlan and OpenIntoRel.

``` theme:
ExecutorStart (src/backend/executor/execMain.c)
  InitPlan (src/backend/executor/execMain.c)
    OpenIntoRel(src/backend/executor/execMain.c)
      heap_create_with_catalog (src/backend/catalog/heap.c)
```

#### Metadata Tables

##### Toast Tables

The main routine is AlterTableCreateToastTableWithOid which eventually calls heap\_create\_with\_catalog.
Like we saw in the previous two sections, AlterTableCreateToastTableWithOid can be called by way of either the CREATE TABLE path through PortalRun and Process Utility or the SELECT INTO path through ExecutorStart and OpenIntoRel.

``` theme:
PortalRun(src/backend/tcop/pquery.c )
  PortalRunMulti(src/backend/tcop/pquery.c )
    PortalRunUtility (src/backend/tcop/pquery.c )
      ProcessUtility(src/backend/tcop/utility.c )
        AlterTableCreateToastTableWithOid (src/backend/catalog/toasting.c)
          create_toast_table (src/backend/catalog/toasting.c)
            heap_create_with_catalog (src/backend/catalog/heap.c)
```

``` theme:
InitPlan (src/backend/executor/execMain.c)
  OpenIntoRel(src/backend/executor/execMain.c)
    AlterTableCreateToastTableWithOid (src/backend/catalog/toasting.c)
      create_toast_table (src/backend/catalog/toasting.c)
        heap_create_with_catalog (src/backend/catalog/heap.c) 
```

Toast table creation can also be stimulated through ALTER TABLE if the table is altered in such a way that a Toast table becomes necessary.

``` theme:
AlterTable ( src/backend/commands/tablecmds.c)
ATController ( src/backend/commands/tablecmds.c)
ATAddToastIfNeeded( src/backend/commands/tablecmds.c)
  AlterTableCreateToastTableWithOid (src/backend/catalog/toasting.c)
    create_toast_table (src/backend/catalog/toasting.c)
      heap_create_with_catalog (src/backend/catalog/heap.c) 
```

##### pg\_aoseg and pg\_aocsseg relations

For each, row oriented AO table a pg\_aoseg relation is created and for each column oriented AO table a pg\_aocsseg relation is created. These relations are metadata relations that track the different segment files associated with the base table. These pg\_aoseg and pg\_aocsseg relations are heap tables. These heap tables are automatically created with a call to heap\_create\_with\_catalog.
The basic function is AlterTableCreateAoSegTableWithOid.
Like we saw in the previous two sections, AlterTableCreateAoSegTableWithOid can be called by way of either the CREATE TABLE path through PortalRun and Process Utility or the SELECT INTO path through ExecutorStart and OpenIntoRel.

``` theme:
PortalRun(src/backend/tcop/pquery.c )
  PortalRunMulti(src/backend/tcop/pquery.c )
    PortalRunUtility (src/backend/tcop/pquery.c )
      ProcessUtility(src/backend/tcop/utility.c )
        AlterTableCreateAoSegTableWithOid (src/backend/catalog/aoseg.c)
        create_aoseg_table (src/backend/catalog/aoseg.c)
          heap_create_with_catalog (src/backend/catalog/heap.c)
```

``` theme:
ExecutorStart (src/backend/executor/execMain.c)  InitPlan (src/backend/executor/execMain.c)
    OpenIntoRel(src/backend/executor/execMain.c)
      AlterTableCreateAoSegTableWithOid (src/backend/catalog/aoseg.c)
        create_aoseg_table (src/backend/catalog/aoseg.c)
          heap_create_with_catalog (src/backend/catalog/heap.c) 
```

##### pg\_aoblkdir relation

When an index is created on an AO table, a pg\_aoblkdir relation is created to track the location of tuples for each transaction. Each entry in a block directory contains information such as first row number assigned to this transaction, offset in the segment file and the row count that was part of this transaction. The block directory is created when an index is created on the AO table and it is deleted when we drop the index.

The main routine is AlterTableCreateAoBlkdirTableWithOid which eventually calls heap\_create\_with\_catalog. This is called from IndexBuildAppendOnlyColScan in the case of column oriented AO tables and from IndexBuildAppendOnlyRowScan in the case of row oriented AO tables.

``` theme:
IndexBuildAppendOnlyColScan(src/backend/catalog/index.c)
  AlterTableCreateAoBlkdirTableWithOid (src/backend/catalog/aoblkdir.c)
    create_aoblkdir_table (src/backend/catalog/aoblkdir.c)
      heap_create_with_catalog (src/backend/catalog/heap.c)
```

``` theme:
IndexBuildAppendOnlyRowScan(src/backend/catalog/index.c)
  AlterTableCreateAoBlkdirTableWithOid (src/backend/catalog/aoblkdir.c)
    create_aoblkdir_table (src/backend/catalog/aoblkdir.c)
      heap_create_with_catalog (src/backend/catalog/heap.c) 
```

##### Bitmap Index Tables

The List-of-values (LOV) metadata in a bitmap index is stored in a heap table.

``` theme:
bmbuild(src/backend/access/bitmap/bitmap.c)
  _bitmap_init(src/backend/access/bitmap/bitmappages.c)
    _bitmap_create_lov_heapandindex (src/backend/access/bitmap/bitmapattutil.c)
      heap_create_with_catalog (src/backend/catalog/heap.c)
```

#### Indices

Indices on a base relation are often heap tables of their own\*. They are referred to as secondary indices because they stored in their own relation, separate from the base relation.

The basic routines creating an index are as follows:

``` theme:
DefineIndex (src/backend/commands/indexcmds.c)
  index_create (src/backend/catalog/index.c)
    heap_create (src/backend/catalog/heap.c) 
```

***\* The contents of an index are entirely under the control of its index access method so they wouldn't have to be implemented as heap tables, but the existing index access methods use the standard page layout for heap tables as described earlier.***

In the subsections that follow, we discuss in more detail what can trigger DefineIndex to be called.
An index is effectively a mapping from some data key values to *tuple identifiers*, or TIDs, of row versions (tuples) in the index's parent table. A TID consists of a block number and an item number within that block. This is sufficient information to fetch a particular row version from the table\*.

***\* From*** ***<a href="http://www.postgresql.org/docs/8.3/static/indexam.md" class="uri" class="external-link">http://www.postgresql.org/docs/8.3/static/indexam.md</a>***

##### User Defined Indices

User defined indices can be created on tables during CREATE TABLE or ALTER TABLE.

``` theme:
PortalRun(src/backend/tcop/pquery.c )
  PortalRunMulti(src/backend/tcop/pquery.c )
    PortalRunUtility (src/backend/tcop/pquery.c )
      ProcessUtility(src/backend/tcop/utility.c )
        DefineIndex (src/backend/commands/indexcmds.c)
          index_create (src/backend/catalog/index.c)
            heap_create (src/backend/catalog/heap.c) 
```

For ALTER TABLE, the parser converts UNIQUE and PRIMARY KEY constraints into AT\_AddIndex subcommands which are actualy executed by ATExecAddIndex.

``` theme:
ATExecCmd ( src/backend/commands/tablecmds.c)
  ATExecAddIndex ( src/backend/commands/tablecmds.c)
    DefineIndex (src/backend/commands/indexcmds.c)
      index_create (src/backend/catalog/index.c)
        heap_create (src/backend/catalog/heap.c) 
```

##### Indices on Metadata tables

Many of the metadata tables we discussed in the previous section have indices automatically created on them including indices on the pg\_aoseg and pg\_aocsseg tables, indices on the pg\_aoblkdir tables, indices on toast tables and bitmap indices.

The source code to create these indices can easily be found by searching for the index\_create function in src/backend/access/bitmap/bitmapattutil.c, src/ /backend/catalog/aoblkdir.c, src/backend/catalog/aoseg.c and src/ backend/catalog/toasting.c.

#### CLUSTER

Clustering also creates heap tables, however it creates a new heap table based on a restructured version of an existing heap table. Specifically, a table is physically reordered based on index information. When the table is subsequently updated, the changes are not clustered i.e. no attempt is made to store new or updated rows according to their index order.

Clustering may also stimulate the creation of new metadata tables such as AO seg, toast and AOblkdir tables. For example, if the original heap table being clustered had toast tables, then the new heap table will have toast tables as well.

``` theme:
PortalRun(src/backend/tcop/pquery.c )
  PortalRunMulti(src/backend/tcop/pquery.c )
    PortalRunUtility (src/backend/tcop/pquery.c )
      ProcessUtility(src/backend/tcop/utility.c )
        cluster (src/backend/commands/cluster.c)
          cluster_rel (src/backend/commands/cluster.c)
            rebuild_relation (src/backend/commands/cluster.c)
              make_new_heap (src/backend/commands/cluster.c)
                heap_create_with_catalog (src/backend/catalog/heap.c)
                   AlterTableCreateToastTableWithOid
                      create_toast_table (src/backend/catalog/toasting.c)
                      heap_create_with_catalog (src/backend/catalog/heap.c)
                    AlterTableCreateAoSegTableWithOid
                      create_aoseg_table (src/backend/catalog/aoseg.c)
                      heap_create_with_catalog (src/backend/catalog/heap.c)
                    AlterTableCreateAoBlkdirTableWithOic
                      create_aoblkdir_table (src/backend/catalog/aoblkdir.c)
                      heap_create_with_catalog (src/backend/catalog/heap.c) 
```

#### Bootstrap catalog tables and indices

A number of heap tables and indices are created during the bootstrap process. Tables such as pg\_class, pg\_proc, pg\_type and pg\_attribute must exist before the rest of the database can be constructed/initialized.

To accomplish this, there are calls to both heap\_create, heap\_create\_with\_catalog and DefineIndex in src/backend/bootstrap/bootparse.c.

### How Heap Tables Are Created

In the previous section, we discussed why and where heap tables are created. We looked as far as where the routines heap\_create and heap\_create\_with\_catalog are called. In this section, we drill into heap\_create and heap\_create\_with\_catalog themselves. heap\_create\_with\_catalog actually calls heap\_create but it also enters a bunch of things in the system catalog tables and does more extensive sanity checking on the relation being created.

#### heap\_create

The heart of heap\_create is a call to either MirroredFileSysObj\_TransactionCreateBufferPoolFile or MirroredFileSysObj\_TransactionCreateAppendOnlyFile to actually create the new relation on disk. Both of these routines are found in src/backend/cdb/cdbmirroredfilesysobj.c.

Both of these routines call PersistentFileSysObjName\_SetRelationFile, MirroredFileSysObj\_BeginMirroredCreate and smgrcreatepending to prepare for the actual creation of the on disk relation, but they use different routines to do the actual creation.

MirroredFileSysObj\_TransactionCreateBufferPoolFile calls smgrcreate which creates the files on both primary and mirror.

``` theme:
smgrcreate (src/backend/storage/smgr/smgr.c)
  mdcreate (src/backend/storage/smgr/md.c)
    MirroredBufferPool_Create (src/backend/cdb/cdbmirroredbufferpool.c)
      MirroredBufferPool_DoOpen (src/backend/cdb/cdbmirroredbufferpool.c)
        FormDatabasePath (src/backend/catalog/catalog.c)
        PathNameOpenFile (src/backend/storage/file/fd.c)
        FileRepPrimary_MirrorOpen (src/backend/cdb/cdbfilerepprimary.c) 
```

MirroredFileSysObj\_TransactionCreateAppendOnlyFile calls MirroredAppendOnly\_Create.

``` theme:
MirroredAppendOnly_Create (src/backend/cdb/cdbmirroredappendonly.c)
 MirroredAppendOnly_DoOpen (src/backend/cdb/cdbmirroredappendonly.c)
   FormDatabasePath (src/backend/catalog/catalog.c)
   PathNameOpenFile (src/backend/storage/file/fd.c)
   FileRepPrimary_MirrorOpen (src/backend/cdb/cdbfilerepprimary.c) 
```

In both cases, FormDatabasePath determines the filename that will contain the relation, PathNameOpenFile opens the file on the primary and FileRepPrimary\_MirrorOpen opens the file on the mirror.

#### heap\_create\_with\_catalog

In addition to calling heap\_create to actually create the new relation on disk, heap\_create\_with\_catalog, it also enters a bunch of things in the system catalog tables and does more extensive sanity checking on the relation being created.
Specifically, it calls CheckAttributeNamesTypes() is used to make certain the tuple descriptor contains a valid set of attribute names and types. It uses get\_relname\_relid() to scan pg\_class to make sure that no relation with the same name already exists. It calls TypeCreate() to define a new type corresponding to the new relation and AddNewRelationTuple() to register the new relation in pg\_class. AddNewAttributeTuples() is called to register the new relation's schema in pg\_attribute.

Important Source Code Files
===========================

**src/include/access/**

``` theme:
heapam.h
hio.h
htup.h
```

**src/include/storage/**

``` theme:
bufpage.h
```

**src/backend/access/heap/**

``` theme:
heapam.c
hio.c
tuptoaster.c
```

**src/backend/access/index/**

``` theme:
heaptuple.c
indexam.c
```

**src/backend/catalog/**

``` theme:
heap.c
```

**src/backend/executor/**

``` theme:
execMain.c
execProcnode.c
execscan.c
nodeIndexscan.c
nodeSeqscan.c
```

**src/backend/storage/buffer/**

``` theme:
bufmgr.c
```

**src/utils/cache**

``` theme:
inval.c
```

**src/utils/time**

``` theme:
tqual.c 
```

Some Useful References
======================

Mvcc Unmasked, Bruce Momjian,
<a href="http://momjian.us/main/writings/pgsql/mvcc.pdf" class="uri" class="external-link">http://momjian.us/main/writings/pgsql/mvcc.pdf</a>

PostgreSQL Internals Through Pictures, Bruce Momjian,
<a href="http://www.postgresql.org/files/developer/internalpics.pdf" class="uri" class="external-link">http://www.postgresql.org/files/developer/internalpics.pdf</a>

PostgreSQL Manual<a href="http://www.postgresql.org/docs/9.1/interactive/storage-toast.md" class="uri" class="external-link">http://www.postgresql.org/docs/9.1/interactive/storage-toast.md</a>

<a href="http://www.postgresql.org/docs/9.1/interactive/storage-toast.md" class="uri" class="external-link">http://www.postgresql.org/docs/9.1/interactive/storage-toast.md</a>
<a href="http://www.postgresql.org/docs/9.1/static/pageinspect.md" class="uri" class="external-link">http://www.postgresql.org/docs/9.1/static/pageinspect.md</a>
<a href="http://www.postgresql.org/docs/9.1/interactive/storage-page-layout.md" class="uri" class="external-link">http://www.postgresql.org/docs/9.1/interactive/storage-page-layout.md</a><a href="http://www.postgresql.org/docs/9.1/interactive/index-unique-checks.md" class="uri" class="external-link">http://www.postgresql.org/docs/9.1/interactive/index-unique-checks.md</a>

Appendix A: HeapTupleHeaderData bit masks
=========================================

The following tables summarize the hint bit masks and their meanings for reference. The first table lists bit masks applied to the t\_infomask field and the second lists bit masks applies to the t\_infomask2 field.
**Table A.1: Bit Masks Used in Interpreting the t\_infomask Field in the HeapTupleHeaderData**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p><strong>Bit Mask Name</strong></p></td>
<td align="left"><p><strong>Purpose</strong></p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_HASNULL</p></td>
<td align="left"><p>Indicates whether the tuple has any null attributes<br />
If true the HeapTupleHeaderData will be followed by a null bitmap that will indicate which fields in the tuple are null</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_HASVARWIDTH</p></td>
<td align="left"><p>Indicates whether the tuple has any variable width attributes</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_HAS_EXTERNAL</p></td>
<td align="left"><p>Indicates whether the tuple has externally stored attributes</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_HAS_COMPRESSED</p></td>
<td align="left"><p>Indicates whether the tuple has any compressed attributes</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_HASEXTENDED</p></td>
<td align="left"><p>This is a combination of HEAP_HAS_EXTERNAL &amp;&amp; HEAP_HAS_COMPRESSED</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_HASOID</p></td>
<td align="left"><p>Indicates whether the tuple has an object-id field<br />
If true, tuple will have an Object-ID. Object-ID is stored after the HeapTupleHeaderData, the nulls bitmap if present ( see HEAP_HASNULL) and the alignment padding area as needed <strong>and</strong> before the user data fields.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_COMBOCID</p></td>
<td align="left"><p>Indicates whether t_cid contains a combo cid field.<br />
If true, t_cid can be mapped to both Cmin and Cmax using a hash table stored in memory in the backend database.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_XMAX_EXCL_LOCK</p></td>
<td align="left"><p>If true, Xmax refers to a locking rather than deleting transaction and the transaction indicated by Xmax has an exclusive lock.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_XMAX_SHARED_LOCK</p></td>
<td align="left"><p>If true, Xmax refers to a locking rather than deleting transaction and the transaction indicated by Xmax has a shared lock.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_XMIN_COMMITTED</p></td>
<td align="left"><p>Indicates whether the transaction identified by Xmin has committed.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_XMIN_INVALID</p></td>
<td align="left"><p>Indicates whether Xmin is invalid ( and should be ignored).</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_XMAX_COMMITTED</p></td>
<td align="left"><p>Indicates whether the transaction identified by Xmax has committed.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_XMAX_INVALID</p></td>
<td align="left"><p>Indicates whether Xmax is invalid ( and should be ignored).</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_XMAX_IS_MULTI</p></td>
<td align="left"><p>Indicates whether the transaction identified by Xmax is a multi-statement transaction. Multi-statement transactions require extra tracking because each statement has its own visibility rules.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_UDPATED</p></td>
<td align="left"><p>Indicates whether this tuple is an updated version of another tuple.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_MOVED_OFF</p></td>
<td align="left"><p>Indicates whether this tuple has been moved to another place by vacuum.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_MOVED_IN</p></td>
<td align="left"><p>Indicates whether this tuple has been moved from another place by vacuum.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_MOVED</p></td>
<td align="left"><p>This is a combination of HEAP_MOVED_OFF || HEAP_MOVED_IN</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_XACT_MASK</p></td>
<td align="left"><p>This bit mask (0XFFE0) refers to all the visibility related bits at once.</p></td>
</tr>
</tbody>
</table>

**Table A.2: Bit Masks Used in Interpreting the t\_infomask2 Field in the HeapTupleHeaderData**

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p><strong>Bit Mask Name</strong></p></td>
<td align="left"><p><strong>Purpose</strong></p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_NATTS_MASK</p></td>
<td align="left"><p>This bit-mask carves off 11 bits that be used to indicate the number of attributes in this tuple.</p></td>
</tr>
<tr class="odd">
<td align="left"><p>HEAP_XMIN_DISTRIBUTED_<br />
SNAPSHOT_IGNORE</p></td>
<td align="left"><p>Indicates whether tuple can be ignored for distributed snapshots.<br />
<br />
Xmin is either an old committed distributed transaction or a local transaction.</p></td>
</tr>
<tr class="even">
<td align="left"><p>HEAP_XMAX_DISTRIBUTED_<br />
SNAPSHOT_IGNORE</p></td>
<td align="left"><p>Indicates whether tuple can be ignored for distributed snapshots.<br />
<br />
Xmax is either an old committed distributed transaction or a local transaction.</p></td>
</tr>
</tbody>
</table>

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [worddavfd453da23ab34dc529b4b75bb8370961.png](attachments/43090511/43221006.png) (image/png)

Comments:
---------

<table>
<colgroup>
<col width="100%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><a href=""></a>
<p>Migrated: <a href="https://support.pivotal.io/hc/en-us/articles/206536327" class="uri" class="external-link">https://support.pivotal.io/hc/en-us/articles/206536327</a></p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by bstephens at Jun 17, 2015 10:19
</div></td>
</tr>
</tbody>
</table>

Document generated by Confluence on May 17, 2016 19:14


