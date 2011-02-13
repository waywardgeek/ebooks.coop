<?php
require "start.php";
$title = "Validate";
require "top.php";

if(isset($_GET['key']) && isset($_GET['user'])) {
    $key = $_GET['key'];
    $user = urlencode($_GET['user']);
    $validationResponse = command($sock, "validateUser " . $user . " " . $key);
    $regs = array();
    if(ereg("User .* validated", $validationResponse, $regs)) {
        echo '<h1>You have succesfully validated your account, and may now <a href="login.php">login</a>.</h1>';
    } else {
        $subString = substr($validationResponse, 0, strpos($validationResponse, "."));
        echo $subString;
    }
} else {
    echo "<h1>Key and user name expected.</h1>";
}
require("bottom.php");
?>
