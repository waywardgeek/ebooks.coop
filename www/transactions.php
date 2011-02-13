<?php
require "start.php";
$title = "Transactions";
require("top.php");

$transactionList = command($sock, "transactions");
$transactions = split("\n", $transactionList);
$numTransactions = count($transactions) - 1;
$regs = array();
echo "<dl>\n";
for($i = 0; $i < $numTransactions; $i++) {
    $transaction = $transactions[$i];
    if(ereg("^(../../..) you sent (.+) ([0-9]+) - (.*)", $transaction, $regs)) {
        echo "<dt>", $regs[1], " ", $regs[2], " (", $regs[3], ")</dt><dd>", $regs[4], "</dd>\n";
    } else if(ereg("^(../../..) (.+) sent you ([0-9]+) - (.*)", $transaction, $regs)) {
        echo "<dt>", $regs[1], " ", $regs[2], " ", $regs[3], "</dt><dd>", $regs[4], "</dd>\n";
    }
}
echo "</dl>\n";

require("bottom.php"); ?>
?>
