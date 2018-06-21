:man_page: mongoc_installing

Installing the MongoDB C Driver (libmongoc) and BSON library (libbson)
======================================================================

The following guide will step you through the process of downloading, building, and installing the current release of the MongoDB C Driver (libmongoc) and BSON library (libbson).

Supported Platforms
-------------------

The MongoDB C Driver is `continuously tested <https://evergreen.mongodb.com/waterfall/mongo-c-driver>`_ on variety of platforms including:

- Archlinux
- Debian 8.1
- macOS 10.10
- Microsoft Windows Server 2008
- RHEL 7.0, 7.1, 7.2
- SUSE 12
- Ubuntu 12.04, 14.04, 16.04
- Clang 3.4, 3.5, 3.7, 3.8
- GCC 4.6, 4.8, 4.9, 5.3
- MinGW-W64
- Visual Studio 2010, 2013, 2015
- x86, x86_64, ARM (aarch64), Power8 (ppc64le), zSeries (s390x)

Install libmongoc with a Package Manager
----------------------------------------

Several Linux distributions provide packages for libmongoc and its dependencies. One advantage of installing libmongoc with a package manager is that its dependencies (including libbson) will be installed automatically.

The libmongoc package is available on recent versions of Debian and Ubuntu.

.. code-block:: none

  $ apt-get install libmongoc-1.0-0

On Fedora, a mongo-c-driver package is available in the default repositories and can be installed with:

.. code-block:: none

  $ dnf install mongo-c-driver

On recent Red Hat systems, such as CentOS and RHEL 7, a mongo-c-driver package is available in the `EPEL <https://fedoraproject.org/wiki/EPEL>`_ repository. To check which version is available, see `https://apps.fedoraproject.org/packages/mongo-c-driver <https://apps.fedoraproject.org/packages/mongo-c-driver>`_. The package can be installed with:

.. code-block:: none

  $ yum install mongo-c-driver

Install libbson with a Package Manager
--------------------------------------

The libbson package is available on recent versions of Debian and Ubuntu. If you have installed libmongoc, then libbson will have already been installed as a dependency. It is also possible to install libbson without libmongoc.

.. code-block:: none

  $ apt-get install libbson-1.0

On Fedora, a libbson package is available in the default repositories and can be installed with:

.. code-block:: none

  $ dnf install libbson

On recent Red Hat systems, such as CentOS and RHEL 7, a libbson package
is available in the `EPEL <https://fedoraproject.org/wiki/EPEL>`_ repository. To check
which version is available, see `https://apps.fedoraproject.org/packages/libbson <https://apps.fedoraproject.org/packages/libbson>`_.
The package can be installed with:

.. code-block:: none

  $ yum install libbson

Building on Unix
----------------

Prerequisites for libmongoc
^^^^^^^^^^^^^^^^^^^^^^^^^^^

OpenSSL is required for authentication or for SSL connections to MongoDB. Kerberos or LDAP support requires Cyrus SASL.

To install all optional dependencies on RedHat / Fedora:

.. code-block:: none

  $ sudo yum install cmake openssl-devel cyrus-sasl-devel

On Debian / Ubuntu:

.. code-block:: none

  $ sudo apt-get install cmake libssl-dev libsasl2-dev

On FreeBSD:

.. code-block:: none

  $ su -c 'pkg install cmake openssl cyrus-sasl'

Prerequisites for libbson
^^^^^^^^^^^^^^^^^^^^^^^^^

The only prerequisite for building libbson is ``cmake``. The command lines above can be adjusted to install only ``cmake``.

Building from a release tarball
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Unless you intend to contribute to mongo-c-driver and/or libbson, you will want to build from a release tarball.

The most recent release of libmongoc and libbson, both of which are included in mongo-c-driver, is |release| and can be :release:`downloaded here <>`. The instructions in this document utilize ``cmake``'s out-of-source build feature to keep build artifacts separate from source files.

The following snippet will download and extract the driver, and configure it:

.. parsed-literal::

  $ wget |release_download|
  $ tar xzf mongo-c-driver-|release|.tar.gz
  $ cd mongo-c-driver-|release|
  $ mkdir cmake-build
  $ cd cmake-build
  $ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..

The ``-DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF`` option is recommended, see :doc:`init-cleanup`. Another useful ``cmake`` option is ``-DCMAKE_BUILD_TYPE=Release`` for a release optimized build and ``-DCMAKE_BUILD_TYPE=Debug`` for a debug build. For a list of all configure options, run ``cmake -L ..``.

If ``cmake`` completed successfully, you will see a considerable amount of output describing your build configuration. The final line of output should look something like this:

.. parsed-literal::

  -- Build files have been written to: /home/user/mongo-c-driver-|release|/cmake-build

If ``cmake`` concludes with anything different, then there is likely an error or some other problem with the build. Review the output to identify and correct the problem.

mongo-c-driver contains a copy of libbson, in case your system does not already have libbson installed. The build will detect if libbson is not installed and use the bundled libbson.

Additionally, it is possible to build only libbson by setting the ``-DENABLE_BSON=ONLY`` option:

.. parsed-literal::

  $ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DENABLE_BSON=ONLY ..

A build configuration description similar to the one above will be displayed, though with fewer entries. Once the configuration is complete, the selected items can be built and installed with these commands:

.. code-block:: none

  $ make
  $ sudo make install

Building from git
^^^^^^^^^^^^^^^^^

Clone the repository and build the current master or a particular release tag:

.. code-block:: none

  $ git clone https://github.com/mongodb/mongo-c-driver.git
  $ cd mongo-c-driver
  $ git checkout x.y.z  # To build a particular release
  $ mkdir cmake-build
  $ cd cmake-build
  $ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
  $ make
  $ sudo make install

Generating the documentation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Install `Sphinx <http://www.sphinx-doc.org/>`_, then:

.. code-block:: none

  $ cmake -DENABLE_MAN_PAGES=ON -DENABLE_HTML_DOCS=ON ..
  $ make mongoc-doc

To build only the libbson documentation:

.. code-block:: none

  $ cmake -DENABLE_MAN_PAGES=ON -DENABLE_HTML_DOCS=ON ..
  $ make bson-doc

The ``-DENABLE_MAN_PAGES=ON`` and ``-DENABLE_HTML_DOCS=ON`` can also be added as options to a normal build from a release tarball or from git so that the documentation is built at the same time as other components.

Building on Mac OS X
--------------------

Install the XCode Command Line Tools::

  $ xcode-select --install

The ``cmake`` utility is also required. First `install Homebrew according to its instructions <https://brew.sh/>`_, then::

  $ brew install cmake

Download the latest release tarball:

.. parsed-literal::

  $ curl -LO |release_download|
  $ tar xzf mongo-c-driver-|release|.tar.gz
  $ cd mongo-c-driver-|release|

Build and install the driver:

.. code-block:: none

  $ mkdir cmake-build
  $ cd cmake-build
  $ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..

All of the same variations described above (e.g., building only libbson, building documentation, etc.) are available when building on Mac OS X.

.. _build-on-windows:

Building on Windows with Visual Studio
--------------------------------------

Building on Windows requires Windows Vista or newer and Visual Studio 2010 or newer. Additionally, ``cmake`` is required to generate Visual Studio project files.

Let's start by generating Visual Studio project files. The following assumes we are compiling for 64-bit Windows using Visual Studio 2015 Express, which can be freely downloaded from Microsoft. We will be utilizing ``cmake``'s out-of-source build feature to keep build artifacts separate from source files. The default build type is ``Debug``, so a release build is specified as you see below.

.. parsed-literal::

  cd mongo-c-driver-|release|
  mkdir cmake-build
  cd cmake-build
  cmake -G "Visual Studio 14 2015 Win64" \\
    "-DCMAKE_INSTALL_PREFIX=C:\\mongo-c-driver" \\
    "-DCMAKE_PREFIX_PATH=C:\\mongo-c-driver" \\
    ..

(Run ``cmake -LH ..`` for a list of other options.)

Now that we have project files generated, we can either open the project in Visual Studio or compile from the command line. Let's build using the command line program ``msbuild.exe``

.. code-block:: none

  msbuild.exe ALL_BUILD.vcxproj

Now that libmongoc and libbson are compiled, let's install them using msbuild. It will be installed to the path specified by ``CMAKE_INSTALL_PREFIX``.

.. code-block:: none

  msbuild.exe INSTALL.vcxproj

You should now see libmongoc and libbson installed in ``C:\mongo-c-driver``

To use the driver libraries in your program, see :doc:`visual-studio-guide`.

Building on Windows with MinGW-W64 and MSYS2
--------------------------------------------

Install MSYS2 from `msys2.github.io <http://msys2.github.io>`_. Choose the x86_64 version, not i686.

Open ``c:\msys64\ming64_shell.bat`` (not the msys2_shell). Install dependencies:

.. code-block:: none

  pacman --noconfirm -Syu
  pacman --noconfirm -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
  pacman --noconfirm -S mingw-w64-x86_64-extra-cmake-modules make tar
  pacman --noconfirm -S mingw64/mingw-w64-x86_64-cyrus-sasl

Download and untar the latest tarball, enter its directory, and build with CMake:

.. code-block:: none

  CC=/mingw64/bin/gcc.exe /mingw64/bin/cmake -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX="C:/mongo-c-driver" ..
  make
