<?php
require "start.php";
$title = "Recover Password";
require("top.php");

unset($resetResponse);
if(isset($_POST['submit'])) {
    $userName=$_POST['userName'];
    $resetResponse = command($sock, "resetPassword " . urlencode($userName));
    $regs = array();
    if(ereg("^Password reset", $resetResponse, $regs)) {
        echo "<h1>Your temporary password has been e-mailed to you.</h1>";
        require("bottom.php");
        exit;
    }
}
if(isset($resetResponse)) {
    $subString = substr($resetResponse, 0, strrpos($resetResponse, "."));
    echo "<h1>", $subString, "</h1>";
}
?>

<table width="300" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr><h1>This will reset your password.  Yor new password will be mailed to you.</h1></tr>
<tr>
<form name="form1" method="post" action="mailpasswd.php">
<td>
<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td width="78">Username</td>
<td width="6">:</td>
<td width="294"><input name="userName" type="text" id="userName"></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td><input type="submit" name="submit" value="Reset Password"></td>
</table>
</td>
</form>
</tr>
</table>

<?php
require("bottom.php"); ?>
?>
