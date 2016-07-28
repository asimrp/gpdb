<span id="title-text"> Storage & Access : Stuff to collect for AO Table issues </span>
======================================================================================

This page last changed on Dec 02, 2014 by aagrawal.

-   Table schema and indexes for the table

-   Operations performed on the table and index using pg\_stat\_last\_operation table

-   <span style="font-size: 14.0px;line-height: 1.4285715;">In utility mode from affected segment</span>

    -   SELECT * FROM pg_aoseg.pg_aoseg_<relfilenode>

    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Output forcing sequential scan with ctid</span>

        -   SELECT ctid,* FROM <table>

    -   For 4.3 Visimap information using gptoolkit functions
        -   SELECT * FROM gp_toolkit.__gp_aovisimap_hidden_info('<table_name>'::regclass)

        -   SELECT * FROM gp_toolkit.__gp_aovisimap('<table_name>'::regclass)

    -   Actual index tuples using the AO index\_reader tool

        -   <span>Copy the "indexscan.so" file attached to jira (MPP-24913) to master and all the segments</span>
        -   <span><span>Create function similar to one provided in "indexscan.sql" file like...</span></span>
            -   CREATE OR REPLACE FUNCTION readaoindex(oid) RETURNS SETOF record AS'<directory_path_where_(.so)_file_is_copied>/indexscan', 'readaoindex' LANGUAGE C STRICT;

        -   <span style="font-size: 14.0px;line-height: 1.4285715;">*<span style="font-size: 14.0px;line-height: 1.4285715;"> </span>*Once this is done, for the AO table having broken index, find the index name and type of column its built on. Connect in utility mode to affected segment and run the following query</span>
            -   SELECT * FROM readaoindex('<index_name>'::regclass) AS (ictid tid, hctid tid, aotid text, istatus text, value <index_column_type>)
    -   <span style="font-size: 14.0px;line-height: 1.4285715;">Relation file, index file, block directory file and visimap files from affected segment (if customer okay to share), else atleast get the gp\_filedump outputs from primary and mirror relation files</span>

<span style="font-size: 14.0px;line-height: 1.4285715;">
</span>

<span style="font-size: 14.0px;line-height: 1.4285715;">In cases of wrong results or errors / failures in AO reads, will be helpful to e</span><span style="font-size: 14.0px;line-height: 1.4285715;">nable the following debug gucs and run the query and upload the collected pg\_log</span><span style="font-size: 14.0px;line-height: 1.4285715;"> </span>

-   SET debug_appendonly_print_scan=1

-   SET debug_appendonly_print_scan_tuple=1

-   SET debug_appendonly_print_blockdirectory=1

-   SET Debug_appendonly_print_read_block=1

<!-- -->

    Query to find aoseg file for pg_aoseg.pg_aoseg_<refilenode>;

From the affected segment: <span>"select </span><span>relid,</span><span>segrelid,</span><span>segidxid from pg\_appendonly where relid= &lt;oid for the affected table&gt;;</span><span>" </span>

<span style="font-size: 14.0px;line-height: 1.4285715;"> </span>

Comments:
---------

<table>
<colgroup>
<col width="100%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><a href=""></a>
<p>oid to be used in the below queries</p>
<ul>
<li><pre><code>SELECT * FROM gp_toolkit.__gp_aovisimap_hidden_info(&#39;&lt;oid_of_table_name&gt;&#39;)</code></pre></li>
<li><pre><code>SELECT * FROM gp_toolkit.__gp_aovisimap(&#39;&lt;oid_of_table_name&gt;&#39;)</code></pre></li>
</ul>
<p>Question - In case of index built on 2 or more columns ( Ex: &quot;idx_passenger_itinerary_pnrtatooflt&quot; btree (pnr_loc, pnr_crtn_dt, tattoo_nbr, flt_nbr) what should be used for value &lt;index_column_type&gt;.</p>
<ul>
<li><pre><code>SELECT * FROM readaoindex(&#39;&lt;index_name&gt;&#39;::regclass) AS (ictid tid, hctid tid, aotid text, istatus text, value &lt;index_column_type&gt;)</code></pre></li>
</ul>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by bchaudhary at Dec 01, 2014 21:33
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<p>In case of index <span>built on 2 or more columns specify that many value columns. </span><span style="font-size: 14.0px;line-height: 1.4285715;">So, for mentioned example it would be like</span></p>
<ul>
<li><pre><code>SELECT * FROM readaoindex(&#39;&lt;index_name&gt;&#39;::regclass) AS (ictid tid, hctid tid, aotid text, istatus text, pnr_loc &lt;pnr_loc_type&gt;, pnr_crtn_dt &lt;pnr_crtn_dt_type&gt;, tattoo_nbr &lt;tattoo_crtn_dt_type&gt;, flt_nbr &lt;flt_nbr_type&gt;)</code></pre></li>
</ul>
<p><span style="font-size: 14.0px;line-height: 1.4285715;"><br />
</span></p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by aagrawal at Dec 02, 2014 10:16
</div></td>
</tr>
<tr class="odd">
<td align="left"><a href=""></a>
<p>How about changetracking logs?</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by hharada at Dec 02, 2014 12:23
</div></td>
</tr>
</tbody>
</table>

Document generated by Confluence on May 17, 2016 19:14


