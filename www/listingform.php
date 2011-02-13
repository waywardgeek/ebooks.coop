<table width="500" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<form name="form1" method="post" action="newlisting.php">
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
<td><input name="title" type="text" id="note"<?php isset($title) && print(' value="' . $title . '"');?>></td>
</tr>
<tr>
<td>Description</td>
<td>:</td>
<td><textarea rows=10 cols=40 name="description" id="note"><?php isset($description) && print($description);?></textarea></td>
</tr>
<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td><input type="submit" name="post" value="Post Listing"></td>
</tr>
</table>
</td>
</form>
</tr>
</table>
