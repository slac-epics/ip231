eval 'exec perl -S $0 ${1+"$@"}'  # -*- Mode: perl -*-
    if $running_under_some_shell; # EpicsHostArch.pl
#*************************************************************************
# Copyright (c) 2002 The University of Chicago, as Operator of Argonne
#     National Laboratory.
# Copyright (c) 2002 The Regents of the University of California, as
#     Operator of Los Alamos National Laboratory.
# EPICS BASE Versions 3.13.7
# and higher are distributed subject to a Software License Agreement found
# in file LICENSE that is included with this distribution.
#*************************************************************************

# EpicsHostArch.pl,v 1.5.2.6 2006/12/19 17:03:02 jba Exp
# Returns the Epics host architecture suitable
# for assigning to the EPICS_HOST_ARCH variable

use Config;
use POSIX;

$suffix="";
$suffix="-".$ARGV[0] if ($ARGV[0] ne "");

$EpicsHostArch = GetEpicsHostArch();
print "$EpicsHostArch$suffix";

sub GetEpicsHostArch { # no args
    $arch=$Config{'archname'};
    if ($arch =~ /sun4-solaris/)       { return "solaris-sparc";
    } elsif ($arch =~ m/i86pc-solaris/) { return "solaris-x86";
    } elsif ($arch =~ m/sun4-sunos/)    { return "sun4-68k";
    } elsif ($arch =~ m/i[3-6]86-linux/)    { return "linux-x86";
    } elsif ($arch =~ m/x86_64-linux/)    { return "linux-x86_64";
    } elsif ($arch =~ m/MSWin32-x86/)   { return "win32-x86";
    } elsif ($arch =~ m/cygwin/)        { return "cygwin-x86";
    } elsif ($arch =~ m/PA-RISC1.1/)    { return "hpux-parisc";
    } elsif ($arch =~ m/PA-RISC2.0/)    { return "hpux-parisc";
    } elsif ($arch =~ m/alpha-dec_osf/)    { return "osf-alpha";
    } elsif ($arch =~ m/darwin/)        {
            my($kernel, $hostname, $release, $version, $cpu) = POSIX::uname();
            if ($cpu =~ m/Power Macintosh/) { return "darwin-ppc";  }
            elsif ($cpu =~ m/i386/)         { return "darwin-x86";  }
            else                            { return "unsupported"; }
    } else { return "unsupported"; }
}

#EOF EpicsHostArch.pl

