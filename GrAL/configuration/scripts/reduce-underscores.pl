#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-


#!perl -p -i

s/top____/top_/g;
s/____/__/g;
