<span id="title-text"> Storage & Access : Run HAWQ in mac </span>
=================================================================

This page last changed on Sep 25, 2014 by onosen.

Prerequisite for making HAWQ Build on your mac
----------------------------------------------

-   Ensure your mac's ssh key is registered on <a href="https://stash.greenplum.com" class="uri" class="external-link">https://stash.greenplum.com</a>. To do that go to the the stash website. On upper righthand corner, from your profile choose 'Manage Account' and left hand options choose 'ssh keys'. Copy the contents of your mac's <span>~/.ssh/id\_rsa.pub to the 'key' field.</span>
-   <span>Ensure your have write permission on /opt </span>
-   <span>Install github on your mac (google and download)</span>
-   <span>Ensure you have apache-ant  in /opt/releng directory. If you do not have apache-ant then </span><span style="font-size: 14.0px;line-height: 1.4285715;">download </span><span style="color: rgb(34,34,34);"> </span><a href="http://build-prod.sanmateo.greenplum.com/releng/tools/ant/apache-ant.1.8.1.tar.gz" class="uri" class="external-link">http://build-prod.sanmateo.greenplum.com/releng/tools/ant/apache-ant.1.8.1.tar.gz</a> and untar it in /opt/releng directory
-   Place to get latest PHD <a href="http://hdsh132.lss.emc.com:8080/view/PHD_2_x_DevBuild/job/HudsonHD2_2_0HadoopBuild_Git/lastSuccessfulBuild/artifact/output/yarn/hadoop/hadoop-2.2.0-gphd-3.0.0.0-SNAPSHOT.tar.gz" class="uri" class="external-link">http://hdsh132.lss.emc.com:8080/view/PHD_2_x_DevBuild/job/HudsonHD2_2_0HadoopBuild_Git/lastSuccessfulBuild/artifact/output/yarn/hadoop/hadoop-2.2.0-gphd-3.0.0.0-SNAPSHOT.tar.gz</a>

 

Build HAWQ from source
----------------------

HAWQ is hosted in stash.  Get the source first.

``` theme:
$ git clone ssh://git@stash.greenplum.com:2222/hawq/main.git hawq
$ cd hawq
$ export BLD_ARCH=osx106_x86
$ make sync_tools
$ make
```

Install and run HDFS daemon
---------------------------

-   Get PHD binary from here: gpadmin@rh55-qavm99.sanmateo.greenplum.com:/data/home/gpadmin/hadoop-2.0.5-alpha-gphd-2.1.0.0-OSX.tar.gz (for linux, s/-OSX/-rhel/)
-   Extract it and go to the directory
-   vim etc/hadoop/hdfs-site.xml and paste this

    ``` theme:
    <?xml version="1.0" encoding="UTF-8"?>
    <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
    <!--
      Licensed under the Apache License, Version 2.0 (the "License");
      you may not use this file except in compliance with the License.
      You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

      Unless required by applicable law or agreed to in writing, software
      distributed under the License is distributed on an "AS IS" BASIS,
      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
      See the License for the specific language governing permissions and
      limitations under the License. See accompanying LICENSE file.
    -->

    <!-- Put site-specific property overrides in this file. -->
    <!-- dfs.client.read.shortcircuit setting true doesn't seem to work for HDD2.2 right now, so set it to false -->
    <configuration>
        <property>
            <name>dfs.permissions</name>
            <value>false</value>
        </property>
        <property>
            <name>dfs.support.append</name>
            <value>true</value>
        </property>
        <property>
            <name>dfs.client.read.shortcircuit</name>
            <value>false</value>
        </property>
        <property>
            <name>dfs.block.local-path-access.user</name>
            <value>haradh1</value>
            <description>
                specify the user allowed to do short circuit read
            </description>
        </property>
        <property>
            <name>dfs.namenode.name.dir</name>
            <value>file:/data/hadoop/dfs/name</value>
        </property>
        <property>
            <name>dfs.datanode.data.dir</name>
            <value>file:/data/hadoop/dfs/data</value>
        </property>
        <property>
            <name>dfs.replication</name>
            <value>3</value>
        </property>
        <property>
            <name>dfs.datanode.max.transfer.threads</name>
            <value>40960</value>
        </property>
        <property>
            <name>dfs.client.socket-timeout</name>
            <value>0</value>
        </property>
    </configuration>
    ```

-   Edit dfs.namenode.name.dir and dfs.namenode.data.dir and dfs.block.local-path-access.user
-   Also copy this in core-site.xml

    ``` theme:
    <?xml version="1.0" encoding="UTF-8"?>
    <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
    <!--
      Licensed under the Apache License, Version 2.0 (the "License");
      you may not use this file except in compliance with the License.
      You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

      Unless required by applicable law or agreed to in writing, software
      distributed under the License is distributed on an "AS IS" BASIS,
      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
      See the License for the specific language governing permissions and
      limitations under the License. See accompanying LICENSE file.
    -->

    <!-- Put site-specific property overrides in this file. -->

    <configuration>
        <property>
            <name>fs.default.name</name>
            <value>hdfs://localhost:9000</value>
        </property>
    </configuration>
    ```

-   ./bin/hdfs namenode -format -clusterId gpsql

-   ./sbin/start-dfs.sh

You should be able to connect <a href="http://localhost:9000" class="external-link">localhost:9000</a> via telnet

Initialize system
-----------------

Create a file <span style="line-height: 1.4285715;">gpinitsystem\_config with the following content</span>

     # FILE NAME: gpinitsystem_config
    # Configuration file needed by the gpinitsystem
    ################################################
    #### REQUIRED PARAMETERS
    ################################################
    #### Name of this Greenplum system enclosed in quotes.
    ARRAY_NAME="Hawq Cluster"
    #### Naming convention for utility-generated data directories.
    SEG_PREFIX=gpseg
    #### Base number by which primary segment port numbers
    #### are calculated.
    PORT_BASE=40000
    #### File system location(s) where primary segment data directories
    #### will be created. The number of locations in the list dictate
    #### the number of primary segments that will get created per
    #### physical host (if multiple addresses for a host are listed in
    #### the hostfile, the number of segments will be spread evenly across
    #### the specified interface addresses).
    declare -a DATA_DIRECTORY=(/Users/nonose/data/segments /Users/nonose/data/segments)
    #### OS-configured hostname or IP address of the master host.
    MASTER_HOSTNAME=bors.local
    #### File system location where the master data directory
    #### will be created.w
    MASTER_DIRECTORY=/Users/nonose/data/master
    #### Port number for the master instance.
    MASTER_PORT=5432
    #### Shell utility used to connect to remote hosts.
    TRUSTED_SHELL=ssh
    #### Maximum log file segments between automatic WAL checkpoints.
    CHECK_POINT_SEGMENTS=8
    #### Default server-side character set encoding.
    ENCODING=UNICODE
    ################################################
    #### Distributed File System Informaiton
    ################################################
    DFS_NAME=hdfs
    DFS_URL=localhost:9000/gpsql
    KERBEROS_KEYFILE=/etc/krb5.keytab
    ENABLE_SECURE_FILESYSTEM=off
    ################################################
    #### OTHER OPTIONAL PARAMETERS
    ################################################
    #### Create a database of this name after initialization.
    DATABASE_NAME=gpadmin
    #### Specify the location of the host address file here instead of
    #### with the the -h option of gpinitsystem.
    #MACHINE_LIST_FILE=/home/gpadmin/hostfile_gpinitsystem
    ~

Set MASTER\_HOSTNAME to what hostname reports on your machine. Create segments and master directories and set the DATA\_DIRECTORY and MASTER\_DIRECTORY to the corresponding paths.

Create a hosts file:

    localhost

Remove the XML libraries in the greenplum directory, which will make sure that the system ones, from /usr/lib, are used:

    $ rm ~/greenplum-db-devel/lib/libxml*
    $ postgres -V
    postgres (HAWQ) 8.2.15

<span style="line-height: 1.4285715;">**Attention**, you may need to remove these libraries every time you run make sync\_tools, since they keep being copied back.</span>

<span style="line-height: 1.4285715;">Run gpinitsystem</span>

    $ source ~/greenplum-db-devel/greenplum_path.sh 
    $ DFS_NAME=hdfs DFS_URL=localhost:9000/gpsql gpinitsystem -c ~/gpinitsystem_config -h ~/hosts

It is recommended to set DFS\_NAME and DFS\_URL in your .bash\_profile

Run your first HAWQ query!

    $ psql -d gpadmin
    psql (8.2.15)

    Type "help" for help.

    gpadmin=#
    gpadmin=# select * from gp_segment_configuration;

     dbid | content | role | preferred_role | mode | status | port  |  hostname  |  address   | replication_port | san_mounts 

    ------+---------+------+----------------+------+--------+-------+------------+------------+------------------+------------

        1 |      -1 | p    | p              | s    | u      |  5432 | bors.local | bors.local |                  | 

        2 |       0 | p    | p              | s    | u      | 40000 | bors.local | localhost  |                  | 

        3 |       1 | p    | p              | s    | u      | 40001 | bors.local | localhost  |                  | 

    (3 rows)

<span style="color: rgb(0,102,0);font-size: 20.0px;line-height: 1.5;">Simple commands to verify your hadoop instance</span>

You might get some warnings about native java libraries or *Error: JAVA\_HOME is not set and could not be found.*

Set the value of JAVA\_HOME in etc/hadoop/hadoop-env.sh file

 

 

To get information about your installation

| <span style="color: rgb(0,51,0);">bin/hdfs dfsadmin -report</span> |
|--------------------------------------------------------------------|

 

To list the contents of your hadoop dataNode(s)

| <span style="color: rgb(0,0,0);">bin/hdfs dfs -ls /</span> |
|------------------------------------------------------------|

 

See all the available commands

<table>
<colgroup>
<col width="100%" />
</colgroup>
<thead>
<tr class="header">
<th align="left"><p><span style="color: rgb(0,0,0);">bin/hdfs dfs</span></p></th>
</tr>
</thead>
<tbody>
</tbody>
</table>

 

 

 

Comments:
---------

<table>
<colgroup>
<col width="100%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><a href=""></a>
<p>I also need to set export HADOOP_OPTS=&quot;-Djava.security.krb5.realm=-Djava.security.krb5.kdc=&quot; in my etc/hadoop/hadoop-env.sh for error: &quot;Unable to load realm mapping info from SCDynamicStore&quot;; and dfs.client.read.shortcircuit to be false for error: &quot;<span style="color: rgb(34,34,34);">-ls: The short-circuit local reads feature is enabled but dfs.domain.socket.path is not set.</span>&quot;</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by xfeng at Jan 22, 2014 12:49
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<p>BTW., just curious if anyone know how to fix the warning &quot; WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable&quot; or know that this is not fixable.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by xfeng at Jan 22, 2014 12:50
</div></td>
</tr>
<tr class="odd">
<td align="left"><a href=""></a>
<p>That warning disappeared on my OSX after moving to HDFS 2.2 and JDK 1.7.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by apraveen at Jan 23, 2014 15:15
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<p>Do you need Java 7 for this to work?</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by gcaragea at Jan 30, 2014 13:06
</div></td>
</tr>
<tr class="odd">
<td align="left"><a href=""></a>
<p>Yes with HD2.2 need to have JAVA 1.7</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by aagrawal at Jan 31, 2014 08:21
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<p>Ok, that rules out Macs with OSX 10.6.x, since Apple doesn't provide Java 1.7 for those.. <img src="s/en_GB-1988229788/4224/875a2723d3ca93f267db5c35cdbfcf91f5910d2d.36/_/images/icons/emoticons/sad.png" alt="(sad)" class="emoticon emoticon-sad" /></p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by gcaragea at Jan 31, 2014 09:28
</div></td>
</tr>
<tr class="odd">
<td align="left"><a href=""></a>
<p>Java 7 for OSX 64 bit can be downloaded from Oracle.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by onosen at Aug 07, 2014 12:04
</div></td>
</tr>
</tbody>
</table>

Document generated by Confluence on May 17, 2016 19:14


