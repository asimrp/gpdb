<span id="title-text"> Storage & Access : Index Access Methods </span>
======================================================================

This page last changed on May 09, 2012 by paramr3.

***\*****\* Work in progress \*\*****  Author: Rajesh Alavarthi***

Index in Postgres
=================

An index is a mapping from some data key values to “tuple identifiers (TIDs)” in the parent table of the index. A TID consists of a block number and the offset within that block. This is sufficient information to fetch any row from the table. All indexes in PostgreSQL are a technically secondary index; that is, the index is physically separate from the parent table. Each index is stored as its own physical relation and has an entry in the pg\_class catalog. Indexes are not directly aware that under MVCC, there may be multiple versions of the same logical row; to an index, each tuple is an independent object, which needs its own entry. Thus an update of a row always creates new index entry for the same row, even if the key values did not change. Index entries for the dead tuples are reclaimed when the dead tuples from the base table are reclaimed during vacuum.

Index access methods
====================

The contents of an index are entirely under the control of its index access method. All the operations on indexes are done through these index access methods. In practice all index access methods divide indexes into standard-size pages so that they can use the regular storage manager and buffer manager to access the index contents. Although all the existing index access methods use the standard page layout, it is not mandatory. 

Catalog entries for index access methods
========================================

Each index access method is described by a row in the pg\_am system catalog. The main contents of a pg\_am row are the references to pg\_proc entries that identify the index access functions supplied by the access method. In addition, the pg\_am row also specifies a few fixed properties of the access method, such as whether it can support multicolumn indexes, whether it can support unique indices etc
GPDB currently supports 5 types of index access methods

1.  B-tree
2.  Hash
3.  Gist
4.  Gin
5.  Bitmap

The cached lookup information for the index access methods is defined as

``` theme:
typedef struct RelationAmInfo
{
  FmgrInfo        aminsert;
  FmgrInfo        ambeginscan;
  FmgrInfo        amgettuple;
  FmgrInfo        amgetmulti;
  FmgrInfo        amrescan;
  FmgrInfo        amendscan;
  FmgrInfo        ammarkpos;
  FmgrInfo        amrestrpos;
  FmgrInfo        ambuild;
  FmgrInfo        ambulkdelete;
  FmgrInfo        amvacuumcleanup;
  FmgrInfo        amcostestimate;
  FmgrInfo        amoptions;
} RelationAmInfo; 
```

Example

``` theme:
template1=# select * from pg_am;
 amname | amstrategies | amsupport | amorderstrategy | amcanunique | amcanmulticol | amoptionalkey  | amindexnulls  | amstorage  | amclusterable  | amcanshrink  |  aminsert   |  ambeginscan   |  amgettuple   |  amgetmulti   |  amrescan   |  amend
scan   |  ammarkpos   |  amrestrpos   |  ambuild   |  ambulkdelete   |  amvacuumcleanup   |  amcostestimate   |  amoptions
 ---- -- ---+ -+-+ ------- -+-+ -+- ----- -- ---+ -+-+ ---------- -+-+ -+- ------- -- ---+ -+-+ -------- -+-+ -+- --------- -- ---+ -+-+ ------- -+-+ -+- ----- -- ---+ -+-+ -------- -+-+ -+- ------- -- ---+ -+-+ ----- -+-+ -+- --------- -- ---+ -+-+ ------- -+-+ -+- -------- -- ---+ -+-+ ----- -+-+ -+- ---- -
 -- -- ---+ -+-+ ------ -+-+ -+- -------- -- ---+ -+-+ ---- -+-+ -+- ---------- -- ---+ -+-+ ------------ -+-+ -+- ------------ -- --+- -- --------- -
btree   |            5  |         1  |               1  | t            | t              | t              | t             | f          | t              | t            | btinsert    | btbeginscan    | btgettuple    | btgetmulti    | btrescan    | btends
can    | btmarkpos    | btrestrpos    | btbuild    | btbulkdelete    | btvacuumcleanup    | btcostestimate    | btoptions
hash    |            1  |         1  |               0  | f            | f              | f              | f             | f          | f              | t            | hashinsert  | hashbeginscan  | hashgettuple  | hashgetmulti  | hashrescan  | hashen
dscan  | hashmarkpos  | hashrestrpos  | hashbuild  | hashbulkdelete  | hashvacuumcleanup  | hashcostestimate  | hashoptions
gist    |          100  |         7  |               0  | f            | t              | t              | t             | t          | t              | t            | gistinsert  | gistbeginscan  | gistgettuple  | gistgetmulti  | gistrescan  | gisten
dscan  | gistmarkpos  | gistrestrpos  | gistbuild  | gistbulkdelete  | gistvacuumcleanup  | gistcostestimate  | gistoptions
gin     |          100  |         4  |               0  | f            | f              | f              | f             | t          | f              | t            | gininsert   | ginbeginscan   | gingettuple   | gingetmulti   | ginrescan   | ginend
scan   | ginmarkpos   | ginrestrpos   | ginbuild   | ginbulkdelete   | ginvacuumcleanup   | gincostestimate   | ginoptions
bitmap  |            5  |         1  |               0  | f            | t              | t              | t             | f          | f              | f            | bminsert    | bmbeginscan    | bmgettuple    | bmgetmulti    | bmrescan    | bmends
can   | bmmarkpos   | bmrestrpos   | bmbuild    | bmbulkdelete    | bmvacuumcleanup    | bmcostestimate    | bmoptions
(5 rows) 
```

An individual index is defined by a pg\_class entry that describes its physical relation, plus a pg\_index entry that shows the logical content of the index – that is the set of columns that is part of the index and their semantics. Both the pg\_class and the pg\_index entries for a given index can be accessed as part of the "Relation" data structure that is passed to all the operations on the index.

To be useful, an index access method must also have one or more operator families and operator classes defined in pg\_opfamily, pg\_opclass, pg\_amop and pg\_amproc. These entries allow the planner to determine what kinds of query qualifications can be used with indexes of this access method.

``` theme:
typedef struct RelationData
{
  RelFileNode rd_node;            /* relation physical identifier  */
/* use "struct" here to avoid needing to include smgr.h */
  struct SMgrRelationData *rd_smgr;       /* cached file handle, or NULL  */
  BlockNumber rd_targblock;
/* current insertion target block or InvalidBlockNumber */
  int                     rd_refcnt;    /* reference count */
  bool     rd_istemp;              /* rel uses the local buffer mgr  */
  bool            rd_isnailed;    /* rel is nailed in cache  */
  bool            rd_isvalid;             /* relcache entry is valid  */
  char            rd_indexvalid;  /* state of rd_indexlist */
/* 0 = not valid, 1 = valid, 2 = temporarily forced  */
SubTransactionId rd_createSubid;        /* rel was created in current xact  */
SubTransactionId rd_newRelfilenodeSubid;        /* new relfilenode assigned in
* current xact  */

/*
* rd_createSubid is the ID of the highest subtransaction the rel has
* survived into; or zero if the rel was not created in the current top
* transaction.  This should be relied on only for optimization purposes;
* it is possible for new-ness to be "forgotten" (eg, after CLUSTER).
* Likewise, rd_newRelfilenodeSubid is the ID of the highest
* subtransaction the relfilenode change has survived into, or zero if not
* changed in the current transaction (or we have forgotten changing it).
 */
Form_pg_class rd_rel;           /* RELATION tuple  */
TupleDesc       rd_att;                 /* tuple descriptor  */
Oid                     rd_id;                  /* relation's object id  */
List *rd_indexlist;       /* list of OIDs of indexes on relation  */
Bitmapset *rd_indexattr;       /* identifies columns used in indexes  */
Oid                     rd_oidindex;    /* OID of unique index on OID, if any  */
LockInfoData rd_lockInfo;       /* lock mgr's info for locking relation  */
RuleLock *rd_rules;           /* rewrite rules  */
MemoryContext rd_rulescxt;      /* private memory cxt for rd_rules, if any  */
TriggerDesc *trigdesc;          /* Trigger info, or NULL if rel has none  */

/*
* rd_options is set whenever rd_rel is loaded into the relcache entry.
* Note that you can NOT look into rd_rel for this data.  NULL means "use
* defaults".
 */
bytea *rd_options;         /* parsed pg_class.reloptions  */

/* These are non-NULL only for an index relation:  */
Form_pg_index rd_index;         /* pg_index tuple describing this index  */
struct HeapTupleData *rd_indextuple;            /* all of pg_index tuple  */
/* "struct HeapTupleData  *" avoids need to include htup.h here   */
Form_pg_am      rd_am;                  /* pg_am tuple for index's AM  */

/*
* index access support info (used only for an index relation)
 *
* Note: only default operators and support procs for each opclass are
* cached, namely those with lefttype and righttype equal to the opclass's
* opcintype.  The arrays are indexed by strategy or support number, which
* is a sufficient identifier given that restriction.
 *
* Note: rd_amcache is available for index AMs to cache private data about
* an index.  This must be just a cache since it may get reset at any time
* (in particular, it will get reset by a relcache inval message for the
* index).      If used, it must point to a single memory chunk palloc'd in
* rd_indexcxt.  A relcache reset will include freeing that chunk and
* setting rd_amcache = NULL.
 */
MemoryContext rd_indexcxt;      /* private memory cxt for this stuff  */
RelationAmInfo *rd_aminfo;      /* lookup info for funcs found in pg_am  */
Oid *rd_opfamily;        /* OIDs of op families for each index col  */
Oid *rd_opcintype;       /* OIDs of opclass declared input data types  */
Oid *rd_operator;        /* OIDs of index operators  */
RegProcedure *rd_support;       /* OIDs of support procedures  */
FmgrInfo *rd_supportinfo; /* lookup info for support procedures  */
int16 *rd_indoption;       /* per-column AM-specific flags  */
List *rd_indexprs;        /* index expression trees, if any  */
List *rd_indpred;         /* index predicate tree, if any  */
void *rd_amcache;         /* available for use by index AM  */

/* use "struct" here to avoid needing to include pgstat.h:  */
struct PgStat_TableStatus *pgstat_info;         /* statistics collection area  */
 } RelationData;
  
```

Index access method functions
=============================

Every index access method must provide the following construction and maintenance functions, which are described below.

Index create
------------

``` theme:
void ambuild (Relation heapRelation, Relation indexRelation, IndexInfo *indexInfo); 
```

Builds a new index. The index relation will be physically created, but is empty. It must be filled in with entries for all the tuples along with any fixed data structures, which the access method requires. We have different ambuild functions for different access methods in GPDB. For example btbuild for btree, bmbuild for bitmap and so on.

The ambuild() function calls IndexBuildScan() which will call either IndexBuildHeapScan() ( for heap tables ) ,

IndexBuildAppendOnlyRowScan() ( for append-only row oriented table ) or

IndexBuildAppendOnlyColScan() ( for append-only column oriented table) to scan the base table for existing tuples and compute the keys that need to be inserted into the index.

Builds a new index. The index relation will be physically created, but is empty. It must be filled in with entries for all the tuples along with any fixed data structures, which the access method requires. We have different ambuild functions for different access methods in GPDB. For example btbuild for btree, bmbuild for bitmap and so on.
The ambuild() function calls IndexBuildScan() which will call

-   IndexBuildHeapScan() ( for heap tables ) ,
-   IndexBuildAppendOnlyRowScan() ( for append-only row oriented table ) or
-   IndexBuildAppendOnlyColScan() ( for append-only column oriented table)

 to scan the base table for existing tuples and compute the keys that need to be inserted into the index.

``` theme:
bool aminsert (Relation indexRelation, Datum *values, bool *isnull, ItemPointer heap_tid, Relation heapRelation, bool check_uniqueness); 
```

Insert a new tuple into an existing index. The values and isnull arrays give the key values to be indexed, and heap\_tid is the TID to be indexed. If the access method supports unique indexes ( its pg\_am.amcanunique flag is true) then check\_uniqueness may be true. In this case the access method must verify that there is no conflicting row. This is the only situation in which the access method needs the heapRelation parameter. The result is true if an index entry was inserted.

Index deletion
--------------

``` theme:
IndexBulkDeleteResult *ambulkdelete (Relation indexRelation,IndexBulkDeleteCallback callback, void *callback_state); 
```

Delete tuple(s) from an index. This is a "bulk delete" operation, which is intended to be implemented by scanning the whole index and checking each entry to see if it can be deleted. The passed-in callback function may be called as callback (TID, callback\_state), which returns a bool and determines whether any particular index entry, as identified by its referenced TID, is to be deleted.

``` theme:
IndexBulkDeleteResult *amvacuumcleanup (IndexVacuumInfo *info, IndexBulkDeleteResult *stats); 
```

Cleanup after a VACUUM operation. At the minimum it only has to return the statistics. It can also perform bulk cleanup such as reclaiming empty index pages. The stats argument is the result of the previous call for this index. The statistics it contains is used to update pg\_class.

``` theme:
void amcostestimate (PlannerInfo *root, IndexOptInfo *index, List *indexQuals, RelOptInfo \*outer_rel, Cost *indexStartupCost, Cost *indexTotalCost, Selectivity  *indexSelectivity, double *indexCorrelation); 
```

Estimate the costs of an index scan. This function is given a list of WHERE clauses that have been determined to be usable with the index. It must return estimates of the cost of accessing the index and the selectivity of the WHERE clause ( by selectivity we mean the fraction of the parent-table rows that will be retrieved during the index scan ). For simple cases, calling standard routines in the optimizer can do nearly all the work of the cost estimator. The point of having an amcostestimate function is to allow index access methods to provide index-type-specific knowledge, in case it is possible to improve on the standard estimates.
A typical cost estimator works as follows (genericcostestimate() in GPDB)

1.  Estimate and return the fraction of parent-table rows that will be visited based on the given qual conditions. In the absence of any index-type-specific knowledge, use the standard optimizer function clauselist\_selectivity().
2.  Estimate the number of index rows that will be visited during the scan. Usually it is the same as indexSelectivity times the number of rows in the index, but it might be more.
3.  Estimate the number of index pages that will be retrieved during the scan.
4.  Compute the index access cost. A generic estimator might do this.
    ``` theme:
    Our generic assumption is that the index pages will be read * sequentially, so they cost seq_page_cost each, not random_page_cost. Also, we charge for evaluation of the indexquals at each index row. All the costs are assumed to be paid incrementally during the scan.

    cost_qual_eval(&index_qual_cost, indexQuals); *indexStartupCost = index_qual_cost.startup;

    *indexTotalCost = seq_page_cost * numIndexPages + (cpu_index_tuple_cost + index_qual_cost.per_tuple) * numIndexTuples;
    ```

5.  Estimate the index correlation. For a simple ordered index on a single field, this can be retrieved from pg\_statistic.

-   Examples of cost estimator functions can be found in src/backend/utils/adt/selfuncs.c

    Index scan
    ----------

    The purpose of an index, of course, is to support scans for tuples matching an indexable WHERE condition, often called a qualifier or a scan key. In an index scan, the index access method is responsible for collecting all the TIDs that match the scan keys, which is of the form *index\_key operator constant*. The access method is not involved in fetching the actual tuples from the parent table, or in determining whether they pass the scan's time qualification test or other conditions. An index scan has zero or more scan keys, which are ANDed that is the returned tuples are expected to satisfy all the indicated conditions.
    The operator class may indicate that the index is lossy for a particular operator. This implies that the index scan will return all the entries that pass the scan key, plus additional entries that do not. The actual index-scan will then apply that operator again to the tuple to verify if it should be selected. Lossy index searches are implemented by having the index method return a recheck flag when a row might or might not really satisfy the query condition.
    The amgettuple function has a direction argument, which can be either ForwardScanDirection or BackwardScanDirection. If the first call after amrescan specifies BackwardScanDirection, then set of matching index entries is to be scanned back-to-front, so amgettuple must return the last matching tuple in the index.
    The access method must support "marking" a position in a scan and later returning to the marked position. Only one position need be remembered per scan.
    Both the scan position and the mark position must be maintained consistently in the face of concurrent insertions and deletions in the index.
    Instead of using amgettuple, an index scan can be done with amgetmulti to fetch multiple tuples per call. Note that amgetmulti does not support backward scan. The access method need not support marking or restoring scan positions during an amgetmulti scan. Also amgetmulti does not guarantee any locking of the returned tuples.
    The index scan access method functions are

``` theme:
IndexScanDesc ambeginscan (Relation indexRelation, int nkeys, ScanKey key); 
```

Begin a new scan. The "key" array specifies the scan key(s) for the index scan. The result must be a palloc'd struct. The struct is created by calling RelationGetIndexScan().

``` theme:
boolean amgettuple (IndexScanDesc scan, ScanDirection direction); 
```

Fetch the next tuple in the given scan, moving in the given direction. If a tuple was obtained, then the tuple TID is stored in the scan structure. A success only means that the index contains an entry that matches the scan keys, not necessary that the tuple still exists in the heap, or it will pass the caller's snapshot test.

``` theme:
boolean amgetmulti (IndexScanDesc scan, ItemPointer tids, int32 max_tids, int32 *returned_tids);
```

Fetch multiple tuples in the given scan. Here tids point to a caller-supplied array of max-tids ItemPointerData records, which is filled with the TIDs of matching tuples.
\*returned\_tids is set to the number of TIDs actually returned.
Note that amgetmulti and amgettuple cannot be used in the same index scan.

``` theme:
void amrescan (IndexScanDesc scan, ScanKey key); 
```

Restart the scan, possibly with new scan keys.( To continue using the old key, NULL is passed). Note that it is not possible for the number of keys to be changed. In practice, the restart feature is used when a nested-loop join selects a new outer tuple and so a new key comparison value is needed, but the scan structure remains the same.

``` theme:
void amendscan (IndexScanDesc scan); 
```

End a scan and release the resources. The scan struct itself must not be freed.

``` theme:
void ammarkpos (IndexScanDesc scan);
```

Mark current scan position.

``` theme:
void amrestrpos (IndexScanDesc scan);
```

Restore the scan to the most recently marked position.

Index Locking
-------------

The index access methods must handle concurrent updates of the index by multiple processes. The backend obtains AccessShareLock on the index during an index scan, and RowExclusiveLock when updating the index. Since these locks do not conflict, the access method is responsible for handling any fine-grained locking it may need.
Aside from the index's own internal consistency requirements, concurrent updates create issues about consistency between the parent table and the index. Postgres separates accesses and updates of the base table from that of the index. So there are windows in which the index may become inconsistent with the heap. The problem is handled as follows

1.  The base table entry is made before making its index entries.
2.  When a base table entry is to be deleted, all its index entries must be removed first,'
3.  An index scan must maintain a pin on the index page holding the last item returned by amgettuple, and also ambulkdelete cannot delete entries from pages pinned by other backends.

In an amgetmulti index scan, the access method need not guarantee to keep an index pin on any of the returned tuples. Therefore it is safe to use to use such scans with MVCC-compliant snapshots.

Index uniqueness checks
=======================

PostgreSQL enforces SQL uniqueness constraints using unique indexes, which are indexes that disallow multiple entries with identical keys. An access method that supports this feature sets pg\_am.amcanunique to true.
Because of MVCC, it is always necessary to allow duplicate entries to exist physically in an index; the entries might refer to successive versions of a single logical row. The actual behavior that needs to be enforced is that no MVCC snapshot could include two rows with equal index keys. For this the following cases need to be checked:

1.  If a conflicting row has been deleted by the current transaction, its okay.
2.  If a conflicting row has been inserted by an as-yet-uncommitted transaction, the new inserter must wait to see if the transaction commits. If it rolls back, then there is no violation. If it commits without deleting the conflicting row again, there is a uniqueness violation. ( In practice we just wait for the other transaction to end and redo the visibility check ).
3.  If a conflicting valid row has been deleted by an as-yet-uncommitted transaction, the new inserter must wait for that transaction to commit or abort and then repeat the test.

Index access methods and operator classes.
==========================================

To define an index on a column of a new data type, we must define an operator class for the new data type. The pg\_am table contains one row for every index access method. The routines for an index access method do not directly know anything about the data types that the index method will operate on. Instead, an operator class identifies, the set of operations that the index access methods need to use to work with a particular data type.
Operator classes specify the set if WHERE-clause operators that can be used with an index. Note that it is possible to have multiple operator classes for the same data type and index method. For example, a B-tree index requires a sort ordering to be defined for each data type it works on. It might be useful for a complex-number data type to have one B-tree operator class that sorts the data by complex absolute value, another sorts by the real part.

Index access method Strategies
------------------------------

The operators associated with an operator class are identified by "strategy numbers", which serve to identify the semantics of each operator within the context of its operator class. For example, B-tree impose a strict ordering on keys, lesser to greater, and so operators like "less than" and "greater than or equal to" are interesting with respect to a B-tree. Each operation is associated with a strategy number, which will help, in easy identification of the operators.
For example, B-tree index method defines five strategies

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Operation</p></td>
<td align="left"><p>Strategy number</p></td>
</tr>
<tr class="even">
<td align="left"><p>Less than</p></td>
<td align="left"><p>1</p></td>
</tr>
<tr class="odd">
<td align="left"><p>Less than or equal</p></td>
<td align="left"><p>2</p></td>
</tr>
<tr class="even">
<td align="left"><p>Equal</p></td>
<td align="left"><p>3</p></td>
</tr>
<tr class="odd">
<td align="left"><p>Greater than or equal</p></td>
<td align="left"><p>4</p></td>
</tr>
<tr class="even">
<td align="left"><p>Greater than</p></td>
<td align="left"><p>5</p></td>
</tr>
</tbody>
</table>

Index access method support routines
------------------------------------

Strategies aren't enough information for the system to figure out how to use an index. In practice, the index access methods require additional support routines in order to work. For example the B-tree index access method must be able to compare two keys and determine whether one is greater than, equal to or lesser than the other. Again similar to strategies, the support functions are numbered.
For example B-tree requires a single support function

<table>
<colgroup>
<col width="50%" />
<col width="50%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><p>Function</p></td>
<td align="left"><p>Support number</p></td>
</tr>
<tr class="even">
<td align="left"><p>Compare two keys</p></td>
<td align="left"><p>1</p></td>
</tr>
</tbody>
</table>

Interaction with Vacuum
=======================

Vacuum with indexes:
At a high level the steps followed in lazy vacuuming a relation is

1.  Open all the indexes for a given relation and obtain the necessary locks.
2.  Scan the heap to perform the actual cleanup.
3.  Truncate the relation if required.
4.  Update the free space map.

For all the indexes on the relation, we call lazy\_vacuum\_index() which inturn calls the generic access method index\_bulk\_delete(). Here we delete all the index entries, which are belonging to dead tuples. After the vacuum operation is complete, it calls the generic access method index\_vacuum\_cleanup() which inturn calls the specific vacuum\_cleanup() routine which updates all the statistics.

References
==========

The official Postgres documentation has very good information about the index access methods and most of the material in this document is from there.
<a href="http://www.postgresql.org/docs/9.1/static/indexam.md" class="uri" class="external-link">http://www.postgresql.org/docs/9.1/static/indexam.md</a>
The source code for the index access methods are located at
<span style="text-decoration: underline;">cdb-pg/src/backend/access/</span>

Document generated by Confluence on May 17, 2016 19:14


