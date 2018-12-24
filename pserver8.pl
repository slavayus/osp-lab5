#! /usr/bin/perl -w -T

use strict;
use English;
use IO::Socket::UNIX qw(SOCK_STREAM SOMAXCONN);
use POSIX ();

my $start_time = time();

my $socket_path = '/tmp/temp_data_p';
unlink($socket_path);

my $listner = IO::Socket::UNIX->new(
    Type   => SOCK_STREAM,
    Local  => $socket_path,
    Listen => SOMAXCONN,
)
    or die("Can't create server socket: $!\n");

$ENV{PATH} = "/usr/bin";
while (1) {
    next unless my $connection = $listner->accept()
        or die("Can't accept connection: $!\n");
    if (fork() == 0) {
        $connection->autoflush(1);
        while (1) {
            print $connection
                "pid=" . $PID .
                    "; uid=" . $UID .
                    "; gid=" . POSIX::getgid() .
                    "; load_time=" . (time() - $start_time) .
                    "; load_avg=" . `uptime | awk '{print \$(NF-2)" "\$(NF-1)" "\$(NF-0)}'` . "\n";
            sleep(1);
        }
        close($connection);
        exit;
    }
}
