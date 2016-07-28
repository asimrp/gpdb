<span id="title-text"> Storage & Access : Unofficial GPDB build instructions for Mac OS 10.10 </span>
=====================================================================================================

This page last changed on Sep 21, 2015 by nkak.

Written by Navneet Potti, with many thanks to Asim Praveen and Abhijit Subramanya in text file gpdb-build.txt.

Revision History

7-14-15   Shoaib Lari   Added notes in bracket with SL prefix based on my experience.

7-22-15   Shoaib Lari    Added notes about additional steps for libz1.dylib, JAVA\_HOME etc.

9-21-15   Nikhil Kak      XCode requirements 

 

Steps

-   You will need to use gcc 4.4.2 to compile GPDB.
-   \[nkak\] As of 09/21/2015 , GPDB does not compile with Xcode 7 and up . I was able to compile using Xcode 6.4 

<!-- -->

-   Not Clang on your Mac. Not a later version of gcc.
-   Download tarball from here: <a href="http://intranet.greenplum.com/releng/tools/gcc/4.4.2/" class="uri" class="external-link">http://intranet.greenplum.com/releng/tools/gcc/4.4.2/</a>
-   The tarball actually contains a binary compiled for OSX 10.9, not 10.10.
    -   \[ SL: I used osx-gcc-4.4.2.tar.gz on 10.10 \] 

<!-- -->

-   Follow instructions therein to set the PATH and DYLD\_LIBRARY\_PATH
-   If you can compile 4.4.2 from scratch, feel free to do so. I couldn't get it to build using clang or latest gcc.
-   You need to have ssh keys set up for running git commands.
    -   Use "ssh-keygen -b 1024" to create the ssh key. 
        \[SL: I just did ssh-keygen. The public key is in the file that is listed by ssh-keygen, i.e. ~/.ssh/id\_rsa. \]

<!-- -->

-   Download the source code for GPDB 4.3 from Stash as follows:

<!-- -->

-   -   Use this link: <a href="https://stash.eng.pivotal.io/projects/GPDB/repos/main/browse" class="uri" class="external-link">https://stash.eng.pivotal.io/projects/GPDB/repos/main/browse</a>
    -   You will need to have a developer account (gives access to Stash, JIRA, Confluence)
    -   Do NOT use the version v6 which is "publicly" available (and the only thing you'll see if you don't have the account above)
    -   To access this code, you'll either need to be on the internal network (using "foothill" ethernet) or the VPN
        -   See VPN install instructions here: <a href="https://sites.google.com/a/pivotal.io/pivotal-it/networking/pivotal-vpn" class="uri" class="external-link">https://sites.google.com/a/pivotal.io/pivotal-it/networking/pivotal-vpn</a>
            \[ SL: For the O2 repository, I just searched in the "Find A Repository" for O2 and found the repository.
                     Once in the O2 repository, I clicked on the left (wide) border of the window below the Pull Requests menu item,
                     and a menu appeared in the border.  I clicked on "clone" near the top, and highlighted link with SSH before it appeared.
                     I copied that link. For me it was: <ssh://git@stash.eng.pivotal.io:2222/gpdb/o2.git>
                     For my git branches, I created the ~/gitdev directory.
                     To clone the repository, I did cd to ~/gitdev, and then issued:
                     git clone <ssh://git@stash.eng.pivotal.io:2222/gpdb/o2.git> 
                     The ~/gitdev/o2 directory then became my SRC\_DIR. (I set this in my .bashrc).
                     Then I did cd to $SRC\_DIR before running the git commands below. \]

 

-   -   <span style="line-height: 1.4285715;">Run:</span>
        <span style="line-height: 1.4285715;">git submodule init</span>
        <span style="line-height: 1.4285715;">git submodule update</span>
        <span style="line-height: 1.4285715;">
        </span>

<!-- -->

-   Install JDK and set JAVA\_HOME
    \[ SL: I did java -version. This caused installation of the JDK for me.  The java -version command then printed 1.8 for me.
            After installing the latest JDK, I added the following to my .bash\_profile: 
            export JAVA\_HOME=\`/usr/libexec/java\_home -v 1.8\` \]

<!-- -->

-   I couldn't get GPDB to build the pljava module using the latest JDK. So I turned off its compilation.
-   There's probably a way to turn off the --with-java flag. It was easier to just turn off mkpljava in extensions/Makefile.
-   Not sure if you need to do install JDK if you're not going to build pljava. However, you'll need to turn off the relevant check in extensions/Makefile in that case.
    \[SL:  I commented out all the actions for the mkpljava target in the $SRC\_DIR/extensions/Makefile.
             Note that you should not comment out the mkpljava target as the make would otherwise complain that it could not find the mkpljava target. \]

<!-- -->

-   I saw some compilation errors/warnings about gnutar or gtar. Just symlink it to /usr/bin/tar and it works.

<!-- -->

-   sudo ln -s /usr/local/bin/gtar /usr/bin/tar
    \[SL: I had to do the following:
           sudo ln -s /usr/bin/tar /usr/local/bin/gtar
           sudo ln -s /usr/bin/tar /usr/local/bin/gnutar \]

<!-- -->

-   Configure OS: shared memory, file limit

<!-- -->

-   Add these lines to /etc/sysctl.conf file (create one if it isn't there)
-   kern.sysv.shmmax=2147483648
    kern.sysv.shmmin=1
    kern.sysv.shmmni=64
    kern.sysv.shmseg=16
    kern.sysv.shmall=524288
    kern.maxfiles=65535
    kern.maxfilesperproc=65536
-   Run the following command as root for the changes to take effect:
    cat /etc/sysctl.conf | xargs -n1 sysctl -w
-   I remember getting an error for shmmni. Ignore and proceed. There's a JIRA ticket about this somewhere.
-   Change the file limit to avoid some warnings. You can put this in your .bash\_profile.
    ulimit -n 65536
    \[SL: I did not have to do this as ulimit command for me returned unlimited. \]

Build GPDB

The first time you build GPDB, run "make sync\_tools" to download some of the other items in the build tool chain.

\[SL: I decided to build in SRC\_DIR by doing the following:

        cd $SRC\_DIR

        make sync\_tools

        examine BUILD\_INSTRUCTIONS

       For debug build, I issued:

           make HOME=\`pwd\` devel

       For dist build one can do:

<span style="line-height: 1.4285715;">           make GPROOT=\`pwd\` dist </span><span style="line-height: 1.4285715;">\]</span>

Run either make dist or make devel. Set GPROOT to the location for the build output.
make GPROOT=\`pwd\`/gpdb-dist dist

\[SL: If you build in SRC\_DIR, then your GPHOME will become $SRC\_DIR/greenplum-db-devel \]

I ran into errors trying parallel compilation (make -j). Fortunately, the build only takes a few mins.

Do the ugly libz hack

libz comes as part of the gcc binary tarball we used earlier. But it doesn't play well with the (later) version of libPng in my system, which is used by GPDB/Postgres. Maybe it's fine to remove libz, but we used this hack instead.

Place a new directory /opt/hacky-libs in front of DYLD\_LIBRARY\_PATH and place a symlink there called libz.1.dylib pointing to the system libz (/usr/lib/libz.1.dylib).

\[SL: I did this by:

        cd /opt/hack-libs

        ln -s /usr/lib/libz.1.dylib \]     

        The GP installation also had libz.1.dylib pointing to its internal copy of libz.1.2.3.dylib.
        Therefore, I also had to point libz1.dylib in the GP installation to the one in /usr/lib:

        cd $GPHOME/lib

        You will see that libz.1.dylib points to libz.1.2.3.dylib in the same directory.

        So I did:

             mv libz.1.2.3.dylib libz.1.2.3.dylib.ORIG

             cp -f /usr/lib/libz.1.dylib libz.1.2.3.dylib

      That is, I overwrote the GPHOME libz.1.2.3.dylib with the one from /usr/lib.<span style="line-height: 1.4285715;"> </span>

<span style="line-height: 1.4285715;">       A better alternative suggested by Michael Roth is to <span>rename the file $GPHOME/lib/libz.1.dylib to $GPHOME/lib/libz.1.dylib.old.  </span></span><span style="line-height: 1.4285715;">This will result in usage of the system libz instead.</span><span style="line-height: 1.4285715;">\]</span>

Set up GPDB environment variables

Add the following (or equivalent) to your .bash\_profile (Note that OSX doesn't use .bashrc)

greenplum home and path
export GPHOME=~/Programming/gpdb/gpdb-dist/greenplum-db-devel
export GPDATA=~/gpdb-data
export PATH=$PATH:$GPHOME/bin

greenplum settings
if \[ -e $GPHOME/greenplum\_path.sh \]; then
source $GPHOME/greenplum\_path.sh
fi

GPHOME is the location of the build output (subdirectory of the GPROOT specified earlier).

GPDATA is the location where you will create the database data directories below.

The greenplum\_path.sh script changes your DYLD\_LIBRARY\_PATH. In case you're using libz hack I mentioned above, the change to the library path should be **after** the last line above.

\[SL: Since sourcing greenplus\_path.sh changes the DYLD\_LIBRARY\_PATH again,
        so, add /opt/hack-libs to your DYLD\_LIBRARY\_PATH after the
        source $GPHOME/greenplum\_path.sh \]

 

Create data directories

mkdir –p $GPDATA/master
mkdir –p $GPDATA/p1/primary
mkdir –p $GPDATA/p2/primary

You should have one p\* directory above for each segment, in addition to the one for master.

Set up hosts file

The default hostname in Mac OS is derived from the network you're connected to. You can set a hostname so that it doesn't keep changing.

Set the hostname:
sudo scutil --set HostName myhostname

Then append it to the system hosts /etc/hosts file to allow proper routing: 127.0.0.1 myhostname

Now create a hosts file in $GPDATA containing just myhostname from above
hostname &gt; $GPDATA/hosts

\[SL: I created hosts file in $GPDATA.  The hosts file just contained name of my machine, slarimac \]

Ensure that you're able to ssh to your machine. I didn't have to change System PReference/sharing/services but YMMV.
ssh myhostname
\[SL: To avoid having to type passwords several times, ensure that you can do ssh to your
        machine without having to type the password. I did the following to make that happen.
        cd
        ssh-add
        cat ~/.ssh/id\_rsa.pub | ssh slari@slarimac 'cat &gt;&gt; ~/.ssh/authorized\_keys'
        ssh slarimac \# no password was needed \]

Set up the gpinitsystem config file

Copy the template file from $GPHOME/docs/cli\_help/gpconfigs/gpinitsystem\_config into $GPDATA

Set the MASTER\_HOSTNAME var to myhostname from above

Set the MASTER\_DIRECTORY var to the master directory under $GPDATA you created above

\[SL: I set:

MASTER\_DIRECTORY=~/gpdb-data/master<span style="line-height: 1.4285715;"> \]</span>

<span style="line-height: 1.4285715;">
</span>

Set the DATA\_DIRECTORY var to a list of p\*/primary directories you created above under $GPDATA

\[SL: I set:

               declare -a DATA\_DIRECTORY=(~/gpdb-data/p1 ~/gpdb-data/p2)<span style="line-height: 1.4285715;"> \]</span>

 

Set the MACHINE\_LIST\_FILE var to the hosts file you created under $GPDATA

\[SL: I set:

               MACHINE\_LIST\_FILE=~/gpdb-data/hosts \]

<span>
</span>

You can set a default database using DATABASE\_NAME if you'd like (I created one called tpch for my experiments)

Run gpinitsystem

gpinitsystem -c $GPDATA/gpinitsystem\_config

Now set MASTER\_DATA\_DIRECTORY in .bashrc (following last para of output from gpinitsystem). It's just $GPDATA/master f or me.
\[SL: I set the MASTER\_DATA\_DIRECTORY to /Users/slari/gpdb-data/master/gpseg-1 \]

You can use "gpstate -b" to check that everything went well.

You can start and stop the server using gpstart and gpstop now.

Following an unclean exit, here's the cleanup process.

killall postgres
ipcclean
rm -f /tmp/.s\*

The last command above cleans any locks.

You may also need to clean up the master and segment data directories.
rm -r $GPDATA/master/\*
rm -r $GPDATA/p1/primary/\*
rm -r $GPDATA/p2/primary/\*

If all goes well, this should work: "psql postgres"

If you want to run TPCH queries, use the version in gpdemo, not the one in extensions.

Comments:
---------

<table>
<colgroup>
<col width="100%" />
</colgroup>
<tbody>
<tr class="odd">
<td align="left"><a href=""></a>
<p>Great, super helpful resource.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by aagrawal at Jul 14, 2015 10:55
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<p>One small problem I had was with the gcc binaries. For OSX there are multiple tarballs which can be downloaded from <a href="http://intranet.greenplum.com/releng/tools/gcc/4.4.2/" class="external-link">http://intranet.greenplum.com/releng/tools/gcc/4.4.2</a>/. I was only able to get the build to work with <a href="http://intranet.greenplum.com/releng/tools/gcc/4.4.2/osx106-gcc-4.4.2.tar.gz" class="external-link">osx106-gcc-4.4.2.tar.gz</a> tarball. The 109 versions did not work.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by asubramanya at Aug 10, 2015 14:05
</div></td>
</tr>
<tr class="odd">
<td align="left"><a href=""></a>
<p>I had originally used the binary for 109.  When my Mac had to be reinitialized, I used the osx-gcc-4.4.2.tar.gz tarball.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by slari at Aug 12, 2015 11:26
</div></td>
</tr>
<tr class="even">
<td align="left"><a href=""></a>
<p>There's and easier way to work around the libz issue - simply rename the file $GPHOME/lib/libz.1.dylib to $GPHOME/lib/libz.1.dylib.old.  This will result in gdb using the system libz instead.</p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by mroth at Aug 18, 2015 07:45
</div></td>
</tr>
<tr class="odd">
<td align="left"><a href=""></a>
<p>Hopefully this will save somebody:</p>
<p>If you get &quot;make sync_tools&quot; failures even with clean source tree, go to &quot;/opt/releng&quot; and move the &quot;tools/&quot; directory out of the way. Then go back to the source tree directory and do &quot;make sync_tools&quot;. It will recreate &quot;/opt/releng/tools/&quot; and pull everything again.</p>
<p>Thanks to Ed for the tip.</p>
<p> </p>
<div class="smallfont" align="left" style="color: #666666; width: 98%; margin-bottom: 10px;">
<img src="images/icons/contenttypes/comment_16.png" width="16" height="16" /> Posted by lpetrov at Sep 29, 2015 19:20
</div></td>
</tr>
</tbody>
</table>

Document generated by Confluence on May 17, 2016 19:14


