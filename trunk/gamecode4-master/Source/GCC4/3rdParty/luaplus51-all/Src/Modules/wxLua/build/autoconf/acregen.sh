#!/bin/bash
#
# Author: Francesco Montorsi
# RCS-ID: $Id: acregen.sh,v 1.6 2008/01/10 00:06:53 jrl1 Exp $
# Creation date: 14/9/2005
#
# A simple script to generate the configure script for a wxCode component
# Some features of this version:
# - automatic test for aclocal version
# - able to be called from any folder
#   (i.e. you can call it typing 'build/acregen.sh', not only './acregen.sh')


# called when an old version of aclocal is found
function aclocalold()
{
    echo "Your aclocal version is  $aclocal_maj.$aclocal_min.$aclocal_rel"
    echo "Your automake installation is too old; please install automake >= $aclocal_minimal_maj.$aclocal_minimal_min.$aclocal_minimal_rel"
    echo "You can download automake from ftp://sources.redhat.com/pub/automake/"
    exit 1
}

# called when an old version of autoconf is found
function autoconfold()
{
    echo "Your autoconf version is  $aclocal_maj.$aclocal_min.$aclocal_rel"
    echo "Your automake installation is too old; please install automake >= $aclocal_minimal_maj.$aclocal_minimal_min.$aclocal_minimal_rel"
    echo "You can download automake from ftp://sources.redhat.com/pub/automake/"
    exit 1
}

# first check if we have an ACLOCAL version recent enough
aclocal_verfull=$(aclocal --version)
aclocal_maj=`echo $aclocal_verfull | sed 's/aclocal (GNU automake) \([0-9]*\).\([0-9]*\).\([0-9]*\).*/\1/'`
aclocal_min=`echo $aclocal_verfull | sed 's/aclocal (GNU automake) \([0-9]*\).\([0-9]*\).\([0-9]*\).*/\2/'`
aclocal_rel=`echo $aclocal_verfull | sed 's/aclocal (GNU automake) \([0-9]*\).\([0-9]*\).\([0-9]*\).*/\3/'`

# the version may only be 1.10 instead of 1.10.0
if [[ "x$aclocal_rel" = "x" ]]; then aclocal_rel=0; fi

aclocal_minimal_maj=1
aclocal_minimal_min=9
aclocal_minimal_rel=6

majok=$(expr $aclocal_maj \- $aclocal_minimal_maj)
minok=$(expr $aclocal_min \- $aclocal_minimal_min)
relok=$(expr $aclocal_rel \- $aclocal_minimal_rel)

if [[ $majok < 0 ]]; then aclocalold; fi
if [[ $majok = 0 && $minok < 0 ]]; then aclocalold; fi
if [[ $majok = 0 && $minok = 0 && $relok < 0 ]]; then aclocalold; fi


# check if we have an AUTOCONF version recent enough
autoconf_verfull=$(autoconf --version)
autoconf_maj=`echo $autoconf_verfull | sed 's/autoconf (GNU Autoconf) \([0-9]*\).\([0-9]*\).*/\1/'`
autoconf_min=`echo $autoconf_verfull | sed 's/autoconf (GNU Autoconf) \([0-9]*\).\([0-9]*\).*/\2/'`

autoconf_minimal_maj=2
autoconf_minimal_min=60

majok=$(expr $autoconf_maj \- $autoconf_minimal_maj)
minok=$(expr $autoconf_min \- $autoconf_minimal_min)

if [[ $majok < 0 ]]; then autoconfold; fi
if [[ $majok = 0 && $minok < 0 ]]; then autoconfold; fi


# we can safely proceed
me=$(basename $0)
path=${0%%/$me}        # path from which the script has been launched
current=$(pwd)
cd $path
aclocal -I . && autoconf && mv configure ../..
cd $current
