#!/bin/bash

#
# See: http://belion.tumblr.com/post/36151777927/ptam-compilation-notes
#

DIR="`cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd`"
TooN_ROOT="$DIR/../../Build/TooN"

export CPPFLAGS="-D_REENTRANT -I$TooN_ROOT/include"
export CXXFLAGS="-Wno-unused-variable -fomit-frame-pointer"
export LDFLAGS=-L$TooN_ROOT/lib

./configure --without-ffmpeg \
   --prefix=$DIR/../../Build/libcvd \
   --without-v4l1buffer \
   --with-linux=`uname -r`
