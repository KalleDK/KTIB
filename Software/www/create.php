<?php 

include 'mysql.php';
include 'lib/Kar.Class.php';

$k = new Kar($conn);

$k->name = $_POST['name'];
$k->address = $_POST['address'];

$k->save();

header("Location: index.php");
?>