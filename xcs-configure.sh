#
# See: http://belion.tumblr.com/post/36151777927/ptam-compilation-notes
#

TooN_ROOT="$PWD/../../Build/TooN"
export CPPFLAGS="-D_REENTRANT -I$TooN_ROOT/include"
export CXXFLAGS="-Wno-unused-variable -fomit-frame-pointer"
export LDFLAGS=-L$TooN_ROOT/lib

./configure --without-ffmpeg \
   --prefix=$PWD/../../Build/libcvd \
   --without-v4l1buffer \
   --with-linux=`uname -r`
