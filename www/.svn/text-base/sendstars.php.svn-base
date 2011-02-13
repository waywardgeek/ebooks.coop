<?php
require "start.php";
$title = "Send Stars";
require("top.php");
if(isset($_POST['userName'])) {
    $userName=$_POST['userName'];
    $amount=$_POST['amount'];
    $note=$_POST['note'];
    $response = command($sock, "send " . urlencode($userName) . " " . $amount . " " . urlencode($note));
    $regs = array();
    if(ereg("Transaction completed successfully", $response, $regs)) {
        echo "<h1>You sent ", $userName, " ", $amount, " stars.</h1>\n";
        require("bottom.php");
        exit;
    }
    echo "<h1>$response</h1>\n";
}
?>

<h1>Send Stars</h1>
<form name="form1" method="post" action="sendstars.php">
<table width="300" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td>
<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td width="78">Recipient's Username</td>
<td width="6">:</td>
<td width="294"><input name="userName" type="text" id="userName"></td>
</tr>
<tr>
<td>Amount</td>
<td>:</td>
<td><input name="amount" type="text" id="amount"></td>
</tr>
<tr>
<td>Note</td>
<td>:</td>
<td><input name="note" type="text" id="note"></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td><input type="submit" name="Send Stars" value="Send Stars"></td>
</tr>
</table>
</td>
</tr>
</table>
</form>

<?php
require("bottom.php");
?>
