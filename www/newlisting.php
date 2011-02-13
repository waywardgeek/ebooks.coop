<?php
require "start.php";
if(isset($_POST['delete']) && isset($_POST['listingID'])) {
    $listingID = $_POST['listingID'];
    $response = command($sock, "deleteListing " . $listingID);
    header('Location:index.php' . $thread);
    exit;
}
$title = "New Listing";
require("top.php");
if(isset($_POST['edit'])) {
    $details = command($sock, "show " . $_POST['listingID']);
    if(ereg("^(Offered|Wanted): ([^ ]*) (rate|cost)=([0-9]*) user=([^ ]*) category=([^ ]*) listingID=([0-9]+)\n(.*)\n>", $details, $regs)) {
        $offered = $regs[1] == 'Offered'? 'offered' : 'wanted';
        $hourly = $regs[3] == 'rate'? 'hourly' : 'fixedprice';
        $title = urldecode($regs[2]);
        $selectedCategory = $regs[6];
        $amount = $regs[4];
        $description=$regs[8];
    }
    echo "<h1>New Listing</h1>";
} else if(isset($_POST['post']) || isset($_POST['postedits'])) {
    $offered = $_POST['offered'];
    $hourly = $_POST['hourly'];
    $title = $_POST['title'];
    $selectedCategory = $_POST['category'];
    $amount = $_POST['amount'];
    if($hourly == 'fixedrate') {
        $amount = "=" . $amount;
    }
    $description=$_POST['description'];
    if(isset($_POST['post'])) {
        $response = command($sock, "newListing " . $offered . " " . urlencode($selectedCategory) . " " . $amount . " " .
            urlencode($title) . " " .  urlencode($description));
        if(ereg("New listing created", $response, $regs)) {
            echo "<h1>Your listing has been published.</h1>\n";
            require("bottom.php");
            exit;
        }
        echo "<h1>New Listing</h1>";
    } else {
        $response = command($sock, "editListing " . $_POST['listingID'] . " " . $offered . " " . urlencode($selectedCategory) .
            " " . $amount . " " .  urlencode($title) . " " .  urlencode($description));
        if(ereg("Listing updated", $response, $regs)) {
            echo "<h1>Your listing has been updated.</h1>\n";
            require("bottom.php");
            exit;
        }
        echo "<h1>Edit Listing</h1>";
    }
    $response = substr($response, 0, strrpos($response, "\n"));
    echo "<h1>$response</h1>\n";
}
?>
<form name="form1" method="post" action="newlisting.php">
<table width="500" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td>
<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td>Offered/Wanted</td>
<td>:</td>
<td>
<input type='radio' name='offered'<?php (!isset($offered) || $offered == 'offered') && print(' checked="checked"');?> value='offered'>Offered
<input type='radio' name='offered'<?php isset($offered) && $offered == 'wanted' && print(' checked="checked"');?> value='wanted'>Wanted
</td>
</tr
<tr>
<td>Hourly/Fixed Price</td>
<td>:</td>
<td>
<input type='radio' name='hourly'<?php (!isset($hourly) || $hourly == 'hourly') && print(' checked="checked"');?> value='hourly'>Hourly
<input type='radio' name='hourly'<?php isset($hourly) && $hourly == 'fixedprice' && print(' checked="checked"');?> value='fixedprice'>Fixed Price
</td>
</tr
<tr>
<td>Category</td>
<td>:</td>
<td>
<select name="category">
<?php
$categoryList = command($sock, "list");
$categories = split("\n", $categoryList);
$numCategories = count($categories) - 1;
$regs = array();
for($i = 0; $i < $numCategories; $i++) {
    $category = $categories[$i];
    if(ereg("^(.*) \(([0-9]+)\)", $category, $regs)) {
        echo "<option";
        if(isset($selectedCategory) && $regs[1] == $selectedCategory) {
            echo " selected";
        }
        echo ">", $regs[1], "</option>";
    }
}
?>
</select>
</td>
</tr>
<tr>
<td>Amount</td>
<td>:</td>
<td><input name="amount" type="text" id="amount"<?php isset($amount) && print(' value="' . $amount . '"');?>></td>
</tr>
<tr>
<td>Title</td>
<td>:</td>
<td><input name="title" type="text" id="title"<?php isset($title) && print(' value="' . $title . '"');?>></td>
</tr>
<tr>
<td>Description</td>
<td>:</td>
<td><textarea rows=10 cols=40 name="description" id="note"><?php isset($description) && print($description);?></textarea></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<?php
if(isset($_POST['edit'])) {
    echo '<td><input type="submit" name="postedits" value="Publish Changes">';
    echo '<input type="hidden" name="listingID" value="' . $_POST['listingID'] . '"></td>';
} else {
    echo '<td><input type="submit" name="post" value="Post Listing"></td>';
} ?>
</tr>
</table>
</td>
</tr>
</table>
</form>
<?php
require("bottom.php");
?>
