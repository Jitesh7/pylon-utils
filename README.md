# pylon-utils
Test code to interface with Basler's Pylon API/Library and OpenCV

Building
========

To build the software, you will need these libraries:
 
  - OpenCV
  - libpng
  - libjpeg

You must install cmake to build the software as well.  To build:

    cd /path/to/pylon-utils
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make

Before running the `test_encoding` program, you will want to extract the test images:

    cd /path/to/pylon-utils
    tar -xjf images.tar.bz2
