<?php
require "start.php";

if(isset($_POST['message'])) {
    $message = urlencode($_POST['message']);
    if(isset($_POST['subject'])) {
        $subject = urlencode($_POST['subject']);
        $response = command($sock, "newPost " . $subject . " " . $message);
        ereg("^Created thread ([0-9]+)", $response, $regs);
        $thread = $regs[1];
    } else {
        $thread = $_POST['thread'];
        $response = command($sock, "reply " . $thread . " " . $message);
    }
    header('Location:showposts.php?thread=' . $thread);
    exit;
}
$title = "Reply";
require("top.php");
echo "<h1>Thread and message expected.</h1>";
require("bottom.php");
?>
