<?php
require "start.php";
$title = "Show Posts";
require "top.php";

if(isset($_GET['thread'])) {
    $threadID = $_GET['thread'];
    $postList = command($sock, "listPosts " . $threadID);
    $posts = split("\n", $postList);
    $numPosts = count($posts) - 1;
    $regs = array();
    for($i = 0; $i < $numPosts; $i++) {
        $post = $posts[$i];
        if(ereg("^([0-9]+) (.*)$", $post, $regs)) {
            $message = command($sock, "showPost " . $regs[1]);
            echo "<h3>", $regs[2], "</h3>\n";
            $lines = split("\n", $message);
            $numLines = count($lines) - 1;
            for($j = 0; $j < $numLines; $j++) {
                echo $lines[$j], "<br>";
            }
        } else {
            echo "Failed to parse post";
        }
        echo "<hr>\n";
    }
} else {
    echo "<h1>Thread ID expected.</h1>";
}
if(!$loggedIn) {
    echo '<p>You must be <a href="login.php">logged in</a> to post a reply.</p>';
} else {
    echo'<h3>Reply (HTML formatted)</h3>
<form name="form1" method="post" action="reply.php">
<textarea rows=10 cols=40 name="message" id="message"></textarea>
<p><input type="submit" name="post" value="Post Reply"></p>
<p><input type="hidden" name="thread" value="' . $threadID . '"></p>
</form>';
}
require("bottom.php");
?>
