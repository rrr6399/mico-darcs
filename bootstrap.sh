#!/bin/sh
#set -x
AC=`which autoconf 2>/dev/null`
if test -z $AC; then
  echo "ERROR: autoconf2.13 is missing"
  exit 1
fi
VER=`autoconf --version 2>&1`
case $VER in
*version*2.13*)
  AUTOCONF=autoconf
  ;;
*)
  AC=`which autoconf2.13 2>/dev/null`
  if test -z $AC; then
    echo "ERROR: autoconf2.13 is missing"
    exit 1
  fi
  VER2=`autoconf2.13 --version 2>&1`
  case $VER2 in
  *autoconf*2.13*)
    AUTOCONF=autoconf2.13
  ;;
  esac
esac
echo "autoconf 2.13 detected as: \`$AUTOCONF'"
echo "generating configure..."
$AUTOCONF
echo "processing configure..."
sed -e 's/^extern "C" void exit(int);/#include <stdlib.h>/g' < configure > configure.tmp
mv configure.tmp configure
chmod +x configure
