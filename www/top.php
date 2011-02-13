<!-- This file is the ShareALot.org home page. -->
<!DOCTYPE HTML PUBLIC
"-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<link rel="stylesheet" type="text/css" href="style.css">
<?php isset($title) && print "<title>" . $title . "</title>\n";?>
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<meta name="description" content="Putting the unemployed to work for each other">
<meta name="keywords" content="unemployed, unemployment, charity">
</head>
<body>

<!-- Top bar with logo and stars -->
<table style="width:902px;margin-top:0px;margin-bottom:0px;background-color:#ffffff;" border="0" cellpadding="0" cellspacing="0">
<tr><td align="center"><a name="top"></a>
<table width="890" border="0" cellpadding="0" cellspacing="0"
style="margin-top:1px;background-color:#ffffff;border-bottom:1px solid black">
<tr style="height:48px">
<td width="15">&nbsp;</td>
<td width="203" valign="middle" style="text-align:left">
<h1><a href="index.php"><img style="margin-top:6px" border="0" src="images/sharealot_logo.png" width="32" height="32" alt="sharealot.org" title="sharealot.org">
ShareALot.org</a></h1>
</td>
<td align="left" valign="bottom" style="color:white;font-size:10px;font-weight:bold">
</td>
<td>
<?php
if(!$loggedIn) {
    echo '
<img style="margin-top:6px" border="0" src="images/unicorn.gif" width="50" height="40" alt="unicorn" title="unicorn">
<img style="margin-top:6px" border="0" src="images/butterfly.gif" width="40" height="40" alt="butterfly" title="butterfly">
<img style="margin-top:6px" border="0" src="images/butterfly.gif" width="40" height="40" alt="butterfly" title="butterfly">
<img style="margin-top:6px" border="0" src="images/moon.png" width="40" height="40" alt="moon" title="moon">
<img style="margin-top:6px" border="0" src="images/moon.png" width="40" height="40" alt="moon" title="moon">
<img style="margin-top:6px" border="0" src="images/moon.png" width="40" height="40" alt="moon" title="moon">
<img style="margin-top:6px" border="0" src="images/star.png" width="40" height="40" alt="star" title="star">
<img style="margin-top:6px" border="0" src="images/star.png" width="40" height="40" alt="star" title="star">
<img style="margin-top:6px" border="0" src="images/star.png" width="40" height="40" alt="star" title="star">
<img style="margin-top:6px" border="0" src="images/star.png" width="40" height="40" alt="star" title="star">';
} else {
    $response = command($sock, "balance");
    if(ereg("([0-9]+) unicorn", $response, $regs)) {
	    for($i = 0; $i < $regs[1]; $i++) {
		echo '<img style="margin-top:6px" border="0" src="images/unicorn.gif" width="50" height="40" alt="unicorn" title="unicorn">';
	    }
    }
    if(ereg("([0-9]+) butterfl", $response, $regs)) {
	    for($i = 0; $i < $regs[1]; $i++) {
		echo '<img style="margin-top:6px" border="0" src="images/butterfly.gif" width="40" height="40" alt="butterfly" title="butterfly">';
	    }
    }
    if(ereg("([0-9]+) moon", $response, $regs)) {
	    for($i = 0; $i < $regs[1]; $i++) {
		echo '<img style="margin-top:6px" border="0" src="images/moon.png" width="40" height="40" alt="moon" title="moon">';
	    }
    }
    if(ereg("([0-9]+) star", $response, $regs)) {
	    for($i = 0; $i < $regs[1]; $i++) {
		echo '<img style="margin-top:6px" border="0" src="images/star.png" width="40" height="40" alt="star" title="star">';
	    }
    }
}
?>
</td>
</tr>
</table>

<!-- Menu bar -->
<table width="890" border="0" cellpadding="0" cellspacing="0" style="background-color:#606060">
<tr style="height:20px">
<td class="blacknav" width="70" style="border-left:none"><a class="m_item" href="index.php">Home</a></td>
<?php
if(!$loggedIn) {
    echo '<td class="blacknav" width="70"><a class="m_item" href="login.php">Login</a></td>';
    echo '<td class="blacknav" width="70"><a class="m_item" href="join.php">Join</a></td>';
} else {
    echo '<td class="blacknav" width="110"><a class="m_item" href="newlisting.php">New Listing</a></td>';
    echo '<td class="blacknav" width="100"><a class="m_item" href="sendstars.php">Send Stars</a></td>';
    echo '<td class="blacknav" width="110"><a class="m_item" href="transactions.php">Tranactions</a></td>';
    echo '<td class="blacknav" width="110"><a class="m_item" href="editaccount.php">Edit Account</a></td>';
    echo '<td class="blacknav" width="80"><a class="m_item" href="logout.php">Logout</a></td>';
}
?>
<td class="blacknav">&nbsp;</td>
<td class="blacknav" width="70"><a class="m_item" href="listthreads.php">Forum</a></td>
<td class="blacknav" width="70" style="border-right:none"><a class="m_item" href="contact.php">Contact</a></td>
<td class="blacknav" width="70" style="border-right:none"><a class="m_item" href="about.php">About</a></td>
</tr>
</table>

<table width="890" border="0" cellpadding="0" cellspacing="0">

<!-- Left bar -->
<tr><td class="left_container" width="155" align="left" valign="top">
<table width="100%" cellpadding="0" cellspacing="0">

<tr><td class="content" align="left" valign="top"><h1>News</h1>
<?php
$announcementList = command($sock, "listAnnouncements");
if(!ereg("^No announcements", $announcementList)) {
    $announcements = split("\n", $announcementList);
    $numAnnouncements = count($announcements) - 1;
    for($i = 0; $i < $numAnnouncements; $i++) {
        $announcement = $announcements[$i];
        if(ereg("^([0-9]+) (.*)$", $announcement, $regs)) {
            echo "<h3>", urldecode($regs[2]), "</h3>";
            $message = command($sock, "showAnnouncement " . $regs[1]);
            $message = substr($message, 0, strrpos($message, "\n"));
            echo "<p>", $message, "</p>\n";
        } else {
            echo "Failed to parse announcement";
        }
    }
}
?>
</td></tr></table>

<!-- Middle area -->
</td>
<td width="6">&nbsp;</td>
<td align="left" valign="top">
<table class="page" width="560" border="0" cellpadding="0" cellspacing="0">
<tr>
<td style="width:8px;background-image:url('images/pagetop_topleft.gif');background-color:transparent;background-repeat:no-repeat;"></td>
<td style="background-image:url('images/pagetop_gradient.gif');background-color:transparent;background-repeat:repeat-x;padding-top:10px;">

