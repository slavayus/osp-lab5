#!/usr/bin/perl -w -T

use strict;

use IO::Socket::UNIX qw( SOCK_STREAM );

my $socket_path = '/tmp/temp_data_p';

my $socket = IO::Socket::UNIX->new(
    Type => SOCK_STREAM,
    Peer => $socket_path,
)
    or die("Can't connect to server: $!\n");

chomp( my $line = <$socket> );
print qq{$line\n};
close($socket)