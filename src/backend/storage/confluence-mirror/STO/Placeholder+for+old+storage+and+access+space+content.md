<span id="title-text"> Storage & Access : Placeholder for old storage and access space content </span>
======================================================================================================

This page last changed on Mar 15, 2012 by paramr3.

#### Coordinated File-System Snapshots

[Brief Functional Spec](attachments/42501899/42696834.pdf) (Matt M)

Generational Storage
--------------------

#### What is it?

Kurt noted that since data warehouse databases are mostly read-only a better organization is to keep most data in a large read-only store (we are calling it the Generation Store) and keeps recent changes in a separate delta.

In this scheme Version control storage overhead almost completely eliminated, both for space and speed considerations.

#### Thoughts and ideas

[Concepts, basic operation, design goals](attachments/42501899/42696831.pdf) (John E)
[Initial design ideas](attachments/42501899/42696832.pdf) (John E)
[Background analysis, incremental release plan](attachments/42501899/42696833.pdf) (Matt M)

#### AO tables Functional Specifications

-   [4th version](attachments/42501899/42696830.pdf)

#### AO tables Design

-   [design](attachments/42501899/42696835.doc)
-   pg\_aoseg tables and AO concurrency <a href="http://confluence.greenplum.com/display/STO/pg_aoseg+tables" class="external-link">pg_aoseg tables</a>

#### Other references

-   <a href="http://www.postgresql.org/docs/8.2/static/storage.md" class="external-link">Postgresql 8.2.4 Physical Storage</a>
-   [Append-Only Storage in Greenplum](attachments/42501899/42696836.doc)
-   [GPDB Append-Only Storage Layer for 3.4 Release](attachments/42501899/42696837.pdf)

#### Database Compression

#### Related documents

-   [Implementation and performance of compressed databases](attachments/42501899/42696838.pdf)
-   [Compression techniques used in Oracle 8](attachments/42501899/42696839.pdf)
-   <a href="http://www.teradata.com/t/pdf.aspx?a=83673&amp;b=86846" class="external-link">Teradata's Multi-Value Compression</a>

High-Availability
-----------------

Replace current segment replication with physical replication to allow update and delete DML statements to be concurrent.

Centralize the FTS (Fault-Tolerance System) logic as another step towards GPCC (Greenplum Command and Control system).

#### Related documents

-   [New GPDB Mirroring Requirements](attachments/42501899/42696840.docx)
-   [New GPDB Mirroring Notes](attachments/42501899/42696841.docx)
-   [GPDB Append-Only Compression](attachments/42501899/42696842.docx)
-   [QuickLZ compression in AO func spec](attachments/42501899/42696843.pdf)

GPDB General
------------

-   [GPDB Architecture slides](attachments/42501899/42696844.pptx)

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [append\_only\_func\_spec\_3\_2\_V4(1).pdf](attachments/42501899/42696830.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [genOrgPlusOne.pdf](attachments/42501899/42696831.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [genOrgTables.pdf](attachments/42501899/42696832.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [The Next Greenplum Revolution -- Massively Scalable Data Warehouse Storage.pdf](attachments/42501899/42696833.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Coordinated File-System Snapshots (Brief Functional Spec).pdf](attachments/42501899/42696834.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [AO-design.doc](attachments/42501899/42696835.doc) (application/msword)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [Append-Only Storage in Greenplum.doc](attachments/42501899/42696836.doc) (application/msword)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [GPDB Append-Only Storage Layer for 3.4 Release.pdf](attachments/42501899/42696837.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [compressed\_databases.pdf](attachments/42501899/42696838.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [oracle\_compression.pdf](attachments/42501899/42696839.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [New GPDB Mirroring Requirements.docx](attachments/42501899/42696840.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [New GPDB Mirroring Notes.docx](attachments/42501899/42696841.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [GPDB Append-Only Compression.docx](attachments/42501899/42696842.docx) (application/vnd.openxmlformats-officedocument.wordprocessingml.document)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [AO\_quicklz\_func\_spec.pdf](attachments/42501899/42696843.pdf) (application/download)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [gpdb\_architecture.pptx](attachments/42501899/42696844.pptx) (application/vnd.openxmlformats-officedocument.presentationml.presentation)

Document generated by Confluence on May 17, 2016 19:14


