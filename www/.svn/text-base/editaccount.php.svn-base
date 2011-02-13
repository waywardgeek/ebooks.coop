<?php
require "start.php";
// username and password sent from form
$title = "Edit Account";
require("top.php");
unset($updateResponse);
if(isset($_POST['submit'])) {
    $userName=$_POST['userName'];
    $oldPassword=$_POST['oldPassword'];
    $password=$_POST['password'];
    $passwordAgain=$_POST['passwordAgain'];
    $email=$_POST['email'];
    if($password != $passwordAgain) {
        echo "<h1>Passwords do not match.</h1>";
    } else {
        if($password != "") {
            $updateResponse = command($sock, "updateAccount " . urlencode($userName) . " Carrboro/Chapel+Hill " .
                urlencode($email) . " " .  urlencode($oldPassword) . " " . urlencode($password));
        } else {
            $updateResponse = command($sock, "updateAccount " . urlencode($userName) . " Carrboro/Chapel+Hill " .
                urlencode($email) . " " .  urlencode($oldPassword));
        }
        $regs = array();
        if(ereg("^Account updated succesfully", $updateResponse, $regs)) {
            echo "<h1>Your account ", $userName, " has been updated.</h1>";
            require("bottom.php");
            exit;
        } else {
            echo "Failed too parse response:", $updateResponse;
        }
    }
} else {
    $response = command($sock, "accountSettings");
    if(ereg("User ([^ ]*), email ([^ ]*), region ([^ ]*)", $response, $regs)) {
        $userName = $regs[1];
        $email = $regs[2];
    }
}

if(isset($updateResponse)) {
    $subString = substr($updateResponse, 0, strrpos($updateResponse, "."));
    echo "<h1>", $subString, "</h1>";
}

?>

<form name="form1" method="post" action="editaccount.php">
<table width="300" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td>
<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr><td><h1 align="center">Account Settings</h1></td></tr>
<tr>
<td width="78">Username</td>
<td width="6">:</td>
<td width="294"><input name="userName" type="text" id="userName" <?php isset($userName) && print ' value="' . $userName . '"';?>></td>
</tr>
<tr>
<td>Email Address</td>
<td>:</td>
<td><input name="email" type="text" id="email"<?php isset($email) && print ' value="' . $email . '"';?>></td>
</tr>
<tr>
<td>Old Password</td>
<td>:</td>
<td><input name="oldPassword" type="password" id="oldPassword"></td>
</tr>
<tr>
<td>Password</td>
<td>:</td>
<td><input name="password" type="password" id="password"></td>
</tr>
<tr>
<td>Password Again</td>
<td>:</td>
<td><input name="passwordAgain" type="password" id="passwordAgain"></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td><input type="submit" name="submit" value="Update Account"></td>
</tr>
</table>
</td>
</tr>
</table>
</form>

<?php
require("bottom.php");
?>
