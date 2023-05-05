#!/bin/sh
#set -x
AUTOCONF=""
NAMES="autoconf autoconf2.69 autoconf-2.69"
for i in $NAMES
do
  AC=`which $i 2>/dev/null`
  ret=$?
  if [ $ret -eq 0 ] ; then
    VER=`$i --version 2>&1`
    case $VER in
    *version*2.69*)
      AUTOCONF=$i
      ;;
    esac
  fi
done
if [ ! "$AUTOCONF" ] ; then
  "ERROR: autoconf 2.69 not detected. Tested names are: $NAMES"
  exit 1
fi
echo "autoconf 2.69 detected as: \`$AUTOCONF'"
echo "generating configure..."
$AUTOCONF
echo "processing configure..."
sed -e 's/^extern "C" void exit(int);/#include <stdlib.h>/g' < configure > configure.tmp
mv configure.tmp configure
chmod +x configure
