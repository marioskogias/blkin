#blkin

## By Hand

See the RPM and Debian sections below for installing build
dependencies.

If building the Babeltrace plugins and the configure 
script cannot find the babeltrace python module, then 
set the following enviornment variables.

```
export PYTHON="python3"
export PYTHON_CONFIG="/usr/bin/python3-config"
```

After untarring the distribution and changing into its 
directory.

```
autoreconf -fi
./configure
make
sudo make install
```

## RPM

If you have cloned directly from the git repo,
then you have to generate the Makefile and 
tarball first. But if you have the tarball, 
then skip to rpmbuild.

Note: Fedora does not yet package the Babeltrace Python bindings, 
so the BlkKin rpm installation excludes the Babeltrace plugins.

```
sudo yum install boost-devel lttng-tools liblttng-ust-devel
autoreconf -fi
./configure
make dist
rpmbuild -ta blkin-*.xz
cd ~/rpmbuild
sudo rpm -ivh $(ls RPMS/x86_64/blkin* | grep -v debuginfo)
```

## Debian

If you have cloned directly from the git repo,
then you have to generate the Makefile first. 
Otherwise, skip to debuild.

```
sudo apt-get install libboost-all-dev lttng-tools liblttng-ust-dev python3-babeltrace
autoreconf -fi
debuild -i -us -uc -b -j9
sudo dpkg -i ../*.deb
```
