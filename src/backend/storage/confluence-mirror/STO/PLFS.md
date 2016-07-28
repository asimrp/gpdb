<span id="title-text"> Storage & Access : PLFS </span>
======================================================

This page last changed on May 08, 2013 by aagrawal.

**Scratchpad for PLFS effort (page has attachments with all the PLFS documents)**
=================================================================================

------------------------------------------------------------------------

<span style="color: rgb(0,102,0);">**Install Pointers**</span>
--------------------------------------------------------------

PLFS source may be downloaded from Sourceforge: <a href="http://superb-dca3.dl.sourceforge.net/project/plfs/plfs-2.2.2.tar.gz" class="uri" class="external-link">http://superb-dca3.dl.sourceforge.net/project/plfs/plfs-2.2.2.tar.gz</a>(FUSE configuration didn't work on EMC Mac's. So had to install PLFS without FUSE.This can be done by:

``` theme:
   tar xzvf plfs-2.2.2.tar.gz
   cd plfs-2.2.2
   ./configure --disable-fuse
   make
   sudo make install
```

Can check PLFS after installation:

``` theme:
   /usr/local/include/plfs.h
   /usr/local/lib/libplfs.*
   /usr/local/bin/plfs*
```

To start using PLFS, need to create .plfsrc file under your home directory.  
Example contents of .plfsrc:

``` theme:
   mount_point /gpdata/plfs
   workload n-1
   backends /gpdata/.plfs_store
   mlog_defmask DEBUG
   mlog_file /gpdata/plfs_%t.log
```

Run "plfs\_check\_config" to verify propriety of your .plfsrc.

------------------------------------------------------------------------

<span style="color: rgb(0,102,0);">**Useful Debugging Tips**</span>
-------------------------------------------------------------------

<span style="color: rgb(0,0,0);">man pages are very good. Try 'man plfsrc' to get started.</span>
<span style="color: rgb(0,0,0);">Binaries that are built in the tools directory (which get installed with 'make install').  </span>
<span style="color: rgb(0,0,0);">plfs\_check\_config is very handy to make sure that your plfsrc file is correct.  </span>
<span style="color: rgb(0,0,0);">plfs\_query is a great tool to look at all the plfs index and log files stored on the local file system (which we call the plfs backend).</span>
<span style="color: rgb(0,0,0);">plfs\_map is geat one to looking into logical and physical mappings</span>
<span style="color: rgb(0,0,0);">And when you do io to the FUSE mount point, cat /mnt/plfs/.plfsdebug and 'cat /mnt/plfs/.plfslog' are very useful for debugging as well.</span>

------------------------------------------------------------------------

<span style="color: rgb(0,102,0);">**Useful PLFS Links**</span>

<a href="http://sourceforge.net/apps/mediawiki/plfs/index.php?title=Data_Structures" class="uri" class="external-link">http://sourceforge.net/apps/mediawiki/plfs/index.php?title=Data_Structures</a>

<a href="http://sourceforge.net/apps/mediawiki/plfs/index.php?title=Container_Format" class="uri" class="external-link">http://sourceforge.net/apps/mediawiki/plfs/index.php?title=Container_Format</a>

<a href="http://sourceforge.net/projects/plfs/" class="uri" class="external-link">http://sourceforge.net/projects/plfs/</a>

<a href="https://github.com/PLFS" class="uri" class="external-link">https://github.com/PLFS</a>

------------------------------------------------------------------------

<span style="color: rgb(0,128,0);">Open Questions</span>
--------------------------------------------------------

-   <span style="color: rgb(0,0,0);">Rational behind picking PLFS as middleware layer (GPDB or HAWQ doesn't have N-1 model, only usecase for which PLFS is best) ?</span>
-   Is PLFS right fit for all kinds on work loads, because as middleware layer its expected to perform efficiently for all scenarios ?
-   PLFS by itself is not cluster aware or parallel in notion (100% relies on layer below to provide those capabilities), it by itself just works per node basis. Does that affect any long term plans as middleware layer ?
-   Do we envision to use this middleware layer to implement all the features like HA, disaster recovery, backups, snapshots, etc.. ?
-   <span style="color: rgb(0,0,0);">Do we have plan to enhance PLFS as per our needs and make it fit out usecases ?</span>
-   <span style="color: rgb(0,0,0);">Read performance seems would be low with PLFS given its current index layout ?</span>

------------------------------------------------------------------------

<span style="color: rgb(0,102,0);">Issues during POC</span>
-----------------------------------------------------------

-   Directory permission on PLFS directories seem always to have (x) executable permission but GPDB expects it to have 700 (restricted access. Need to figure out how to resolve this. Currently just converted FATAL message into WARNING "data directory ""/gpdata/plfs/agrawa2/gpdata\_PLFS/p1/gpseg0"" has group or world access","Permissions should be u=rwx (0700).",,,,,,,"checkDataDir","postmaster.c",1779
-   Pre-checks during gpinitsystem use the shell commands to touch files on PLFS segment directories. Several GPDB tools written in Python also use posix file system APIs offered by Python to access files.
-   PLFS uses only absolute path names, while within GPDB relative path names (PGDATA directory) are passed. Covered for right now by converting relative path names to full path names.
-   Some GPDB code uses posix APIs such as `mkfifo`, `select`, which don't have a direct analogue in PLFS. The code in GPDB that uses these APIs deals with temp files / temp filespace only. For now, as far as the first PoC demo is concerned, it may be ok to leave this code unchanged but if we decide to proceed further along PLFS route, we will have to address this.

------------------------------------------------------------------------

<span style="color: rgb(0,102,0);">Compiling GPDB code with PLFS</span>
-----------------------------------------------------------------------

To enable compilation of GPDB code that uses PLFS API (`plfs.h`), the following steps may be performed. Note that this is most certainly not the right way, we will have to involve Release Engineering at some point to find the right way of adding PLFS as third party source in GPDB. The following instructions assume that you have checked out `//cdb2/private/pravea/cbd2-plfs/...` branch in `cdb2-plfs` directory on your Linux system or Mac. The instructions should work for Mac even though the examples below are Linux specific.

1.  Install FUSE if it's not already installed. `README.install` in PLFS source contains some pointers.
2.  Untar PLFS source under `cdb2-plfs/ext/rhel5_x86_64/`, e.g. `cdb2-plfs/ext/rhel5_x86_64/plfs-2.2.2/`
    ``` theme:
    cd cdb2-plfs/ext/rhel5_x86_64
    tar <path-to-plfs-tarball>
    ```

3.  Set PLFS install prefix as `cdb2-plfs/ext/rhel5_x86_64` while running configure script.
    ``` theme:
    cd cdb2-plfs/ext/rhel5_x86_64/plfs-2.2.2
    ./configure --prefix {{cdb2-plfs/ext/rhel5_x86_64}}
    ```

4.  Ensure that there are no errors in `./configure`. Run `make` followed by `make install`.
5.  Verify that `libplfs*` can be found in `cdb2-plfs/ext/rhel5_x86_64/lib` and `plfs.h` can be found in `cdb2-plfs/ext/rhel5_x86_64/include`.
6.  It is useful to install PLFS binaries in default Linux locations, comes handy for debugging. Hence re-run `./configure` without install prefix.
    ``` theme:
    cd cdb2-plfs/ext/rhel5_x86_64/plfs-2.2.2
    ./configure
    ```

7.  Run `make`. Then run `make install` as root. Verify that you can run `plfs_check_config` and other commands.
    ``` theme:
    cd cdb2-plfs/ext/rhel5_x86_64/plfs-2.2.2
    make
    sudo make install
    ```

8.  Edit `cdb2-plfs/cdb-pg/src/Makefile.global`. Append "`-lplfs -lpthread`" to `LIBS` definition on line 251. After edit, the line should look like the following.
    ``` theme:
    LIBS = -lesmtp -lnetsnmp -lbz2 -lpam -lrt -lssl -lcrypto -lkrb5 -lz -ledit -lcrypt -lm -lxml2 -L/data/espine1/dev/tools/curl/7.21.7/dist/rhel5_x86_64/lib -lcurl -L/data/espine1/dev/tools/cache-curl-7.21.7/lib -L/data/espine1/dev/tools/cache-curl-7.21.7/lib -lplfs -lpthread
    ```

9.  Now you are ready to compile GPDB code that uses PLFS API. Run the usual compilation command from workspace root.
    ``` theme:
    cd cdb2-plfs
    make HOME=$PWD devel
    ```

10. In case this compilation fails with errors in `plfs.h`, a workaround is run the following command and retry the compilation.
    ``` theme:
    cd cdb2-plfs/ext/rhel5_x86_64/include
    mv plfs.h .plfs.h
    ```

    Not sure why, but this workaround seems to be working for now...

Attachments:
------------

<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PLFS1.pdf](attachments/53184206/53477475.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PLFS Index compression.pdf](attachments/53184206/53477477.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PLFS-HDFS.pdf](attachments/53184206/53477478.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [PLFS.pdf](attachments/53184206/53477480.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [plfs-hdfs-cmu.pdf](attachments/53184206/53477481.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [LogStructuredFS.pdf](attachments/53184206/53477482.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [DiscoveringStructureInUnstrcuturedIO.pdf](attachments/53184206/53477503.pdf) (application/pdf)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [global\_append.pptx](attachments/53184206/54231041.pptx) (application/vnd.openxmlformats-officedocument.presentationml.presentation)
<img src="images/icons/bullet_blue.gif" width="8" height="8" /> [transformative.pdf](attachments/53184206/55279736.pdf) (application/pdf)

Document generated by Confluence on May 17, 2016 19:14


