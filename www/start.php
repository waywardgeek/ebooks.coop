<?php
session_start();

function readResponse($sock)
{
    $response = "";
    $c = socket_read($sock, 1, PHP_BINARY_READ);
    while(ord($c) != 0) {
	$response .= $c;
	$c = socket_read($sock, 1, PHP_BINARY_READ);
    }
    return $response;
}

function command($sock, $line)
{
    socket_write($sock, $line . chr(0));
    return readResponse($sock);
}

$sock = socket_create(AF_UNIX, SOCK_STREAM, 0);
socket_connect($sock, "/tmp/sharealot");
$response = command($sock, session_id());
$regs = array();
$response = command($sock, "status");
$loggedIn = false;
$supremeLeader = false;
$regionManager = false;
if(ereg("^Logged in as user ([^ ]*)\.\n> $", $response, $regs)) {
    $loggedIn = true;
    $user = urldecode($regs[1]);
} else if(ereg("^Logged in as user ([^ ]*)\.  You are the supreme leader\.\n> $", $response, $regs)) {
    $loggedIn = true;
    $user = urldecode($regs[1]);
    $supremeLeader = true;
    $regionManager = true;
} else if(ereg("^Logged in as user ([^ ]*)\.  You manage region ([^ ]*)\.\n> $", $response, $regs)) {
    $loggedIn = true;
    $user = urldecode($regs[1]);
    $regionManager = true;
}
?>
