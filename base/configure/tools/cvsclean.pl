eval 'exec perl -S $0 ${1+"$@"}'  # -*- Mode: perl -*-
    if $running_under_some_shell; # cvsclean.pl
#*************************************************************************
# Copyright (c) 2002 The University of Chicago, as Operator of Argonne
# National Laboratory.
# Copyright (c) 2002 The Regents of the University of California, as
# Operator of Los Alamos National Laboratory.
# This file is distributed subject to a Software License Agreement found
# in the file LICENSE that is included with this distribution. 
#*************************************************************************

# cvsclean.pl,v 1.1.2.1 2004/03/24 20:14:02 jba Exp
#
# Find and delete cvs .#* files in all dirs of directory tree

use File::Find;

@ARGV = ('.') unless @ARGV;

find sub { unlink if -f && m/^\.\#/ }, @ARGV;

