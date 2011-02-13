<?php
require("start.php");
$title="Sharealot.org";
require("top.php");
?>

<p>All across America, people need help.  Do you have time and skills that you
can donate to help others?  Do you need help?  If you have time, consider
donating your skills to help people through this recession.</p>

<p>ShareALot.org is a charity where you can offer services to others who need
them.  To participate, create an account and look through the services wanted
listings below.  If you find a task you can perform, get to work!  If you have
services you can offer, or services you need, list them here.  In return for
your efforts, you will earn 'stars' which you can use later to reward others
for tasks they complete for you.</p>

<p>Listed below are services offered or wanted by current members.
ShareALot.org currently only operates in Carrboro and Chapel Hill, in North
Carolina.  For a short time, new members will start with 111 stars.</p>

<?php
$categoryList = command($sock, "list");
$categories = split("\n", $categoryList);
$numCategories = count($categories) - 1;
for($i = 0; $i < $numCategories; $i++) {
    $category = $categories[$i];
    if(ereg("^(.*) \(([0-9]+)\)", $category, $regs)) {
        $listingsList = command($sock, "list " . urlencode($regs[1]));
        if(!ereg("^There are no listings in this category", $listingsList)) {
            $listings = split("\n", $listingsList);
            $numListings = count($listings) - 1;
            echo "<hr><h1>", $regs[1], "</h1><dl>";
    	    for($j = 0; $j < $numListings; $j++) {
                $listing = $listings[$j];
                if(ereg("^(.*) listingID=([0-9]+)", $listing, $regs)) {
                    $details = command($sock, "show " . $regs[2]);
    		    if(ereg("^(Offered|Wanted): ([^ ]*) (rate|cost)=([0-9]*) user=([^ ]*) category=([^ ]*) listingID=([0-9]+)\n(.*)\n>",
                            $details, $regs)) {
                        echo "<dt>", $regs[1], " by ", urldecode($regs[5]), " - ", urldecode($regs[2]), " - ";
                        if($regs[3] == "rate") {
                            echo $regs[4], " stars/hr</dt>\n";
                        } else {
                            echo $regs[4], " stars total</dt>\n";
                        }
                        echo "<dd>", $regs[8];
                        if($regs[5] == $user || $regionManager) {
                            echo '<form name="form1" method="post" action="newlisting.php">
    <p><input type="submit" name="edit" value="Edit"> <input type="submit" name="delete" value="Delete">
    <input type="hidden" name="listingID" value="' . $regs[7] . '"></p>
    </form>';
                        }
                        echo "</dd>\n";
                    }
                }
            }
            echo "</dl>";
        }
    }
}

require("bottom.php"); ?>
