<?php
require "start.php";

if(isset($_POST['submit'])) {
    $userName=$_POST['userName'];
    $password=$_POST['password'];
    $loginResponse = command($sock, "login " . urlencode($userName) . " " . urlencode($password));
    $regs = array();
    if(ereg("Login successful", $loginResponse, $regs)) {
        header('Location:index.php');
        exit;
    }
} else {
    unset($userName);
    unset($loginResponse);
}
$title = "Login";
require("top.php");
if(isset($loginResponse)) {
    $subString = substr($loginResponse, 0, strpos($loginResponse, "."));
    echo "<h1>", $subString, "</h1>";
}
?>

<form name="form1" method="post" action="login.php">
<table width="300" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td>
<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td colspan="3"><strong>Member Login </strong></td>
</tr>
<tr>
<td width="78">Username</td>
<td width="6">:</td>
<td width="294"><input name="userName" type="text" id="userName"<?php isset($userName) && print ' value="' . $userName . '"';?>></td>
</tr>
<tr>
<td>Password</td>
<td>:</td>
<td><input name="password" type="password" id="password"></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td><input type="submit" name="submit" value="Login"></td>
</tr>
<tr>
<td>&nbsp</td>
<td>&nbsp</td>
<td><a href="mailpasswd.php">Forgot your password</a>?</td>
</tr>
</table>
</td>
</tr>
</table>
</form>

<?php
require("bottom.php");
?>
