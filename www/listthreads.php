<?php
require "start.php";
$title = "View Threads";
require("top.php");

if(isset($_POST['subscribe'])) {
    command($sock, "followThread " . $_POST['threadID']);
} else if(isset($_POST['unsubscribe'])) {
    command($sock, "leaveThread " . $_POST['threadID']);
}
$threadList = command($sock, "listThreads");
if(!ereg("^No threads", $threadList)) {
    $threads = split("\n", $threadList);
    $numThreads = count($threads) - 1;
    echo '<table style="width:600px">';
    for($i = 0; $i < $numThreads; $i++) {
        $thread = $threads[$i];
        if(ereg("^(\+)?([0-9]+) (.*)$", $thread, $regs)) {
            echo '<tr><td><a href="showposts.php?thread=', $regs[2], '">', $regs[3], "</a></td>";
            if($loggedIn) {
                echo '<td><form method="post" action="listthreads.php">';
                echo '<input type="hidden" name="threadID" value="' . $regs[2] . '">';
                if($regs[1] == "+") {
                    echo '<input type="submit" name="unsubscribe" value="Leave Thread">';
                } else {
                    echo '<input type="submit" name="subscribe" value="Follow Thread">';
                }
                echo "</form></td>\n";
            }
            echo "</tr>\n";
        } else {
            echo "Failed to parse thread";
        }
    }
    echo "</table>\n";
}

if(!$loggedIn) {
    echo '<p>You must be <a href="login.php">logged in</a> to create a new thread.</p>';
} else {
    echo'<h3>New Message (HTML formatted)</h3>
<form name="form1" method="post" action="reply.php">
<p>Subject: <input name="subject" type="text" id="subject"></p>
<textarea rows=10 cols=40 name="message" id="message"></textarea>
<p><input type="submit" name="post" value="Post Message"></p>
</form>';
}

require("bottom.php");
?>
