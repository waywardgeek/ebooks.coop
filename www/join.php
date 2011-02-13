<?php
require "start.php";
$title = "Join Sharealot.org";
require("top.php");
unset($newUserResponse);
if(isset($_POST['submit']) && isset($_POST['agree'])) {
    $userName=$_POST['userName'];
    $password=$_POST['password'];
    $passwordAgain=$_POST['passwordAgain'];
    if($password != $passwordAgain) {
        echo "<h1>Passwords to not match.</h1>";
    } else {
        $email=$_POST['email'];
        $newUserResponse = command($sock, "newUser " . urlencode($userName) . " Carrboro/Chapel+Hill " . urlencode($email) . " " .
            urlencode($password));
        $regs = array();
        if(ereg("New user .* created with validation key (.*)\.", $newUserResponse, $regs)) {
            echo "<h1>Your new account ", $userName, " has been created.  You will recieve a validation e-mail shortly.</h1>";
            require("bottom.php");
            exit;
        } else {
            echo "Failed too parse response:", $newUserResponse;
        }
    }
}

if(isset($_POST['submit'])) {
    echo "<h1>Please indicate that you agree to our Terms of Service by checking the box.</h1>\n";
}
if(isset($newUserResponse)) {
    $subString = substr($newUserResponse, 0, strrpos($newUserResponse, "."));
    echo "<h1>", $subString, "</h1>";
}
?>

<h1>Account Registration</h1>
<p>You must agree to our Terms of Service to use this site.  In short:</p>
<ul>
<li>You may not buy or sell stars, or trade them for items.</li>
<li>Stars are like Monopoly money, and have no actual value.</li>
<li>Be courteous on the forums.</li>
<li>Only post legal services.</li>
<li>Keep content appropriate for viewing by children.</li>
</ul>
<form id="form1" method="post" action="join.php">
<p><input name="agree" type="checkbox" value="checkbox">I agree to the Terms of Service</p>
<table width="300" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td>
<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td width="78">Desired Username</td>
<td width="6">:</td>
<td width="294"><input name="userName" type="text"></td>
</tr>
<tr>
<td>Email Address</td>
<td>:</td>
<td><input name="email" type="text"></td>
</tr>
<tr>
<td>Password</td>
<td>:</td>
<td><input name="password" type="password"></td>
</tr>
<tr>
<td>Password Again</td>
<td>:</td>
<td><input name="passwordAgain" type="password"></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td><input type="submit" name="submit" value="Create Account"></td>
</tr>
</table>
</td>
</tr>
</table>
</form>

<?php
require("bottom.php");
?>
