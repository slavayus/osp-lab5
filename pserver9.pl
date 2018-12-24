#!/usr/bin/perl -w -T

use strict;
use English;
use POSIX ();

$| = 1;
my $start_time = time();

$SIG{HUP} = \&HUP_HANDLER;
$SIG{INT} = \&INT_HANDLER;
$SIG{TERM} = \&TERM_HANDLER;
$SIG{USR1} = \&USR1_HANDLER;
$SIG{USR2} = \&USR2_HANDLER;

$ENV{PATH} = "/usr/bin";

my $diff_time = 0;
my $avg_load = `uptime | awk '{print \$(NF-2)" "\$(NF-1)" "\$(NF-0)}'`;

while (1) {
    $diff_time = time() - $start_time;
    $avg_load = `uptime | awk '{print \$(NF-2)" "\$(NF-1)" "\$(NF-0)}'`;
    sleep(1);
}

sub HUP_HANDLER {
    print $PID . "\n"
}

sub INT_HANDLER {
    print $UID . "\n"
}

sub TERM_HANDLER {
    print POSIX::getgid() . "\n"
}

sub USR1_HANDLER {
    print $diff_time . "\n"
}

sub USR2_HANDLER {
    print $avg_load . "\n"
}