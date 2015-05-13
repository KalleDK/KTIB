<?php 

include 'mysql.php';
include 'lib/Kar.Class.php';

$k = new Kar($conn);

$id = $_GET['id'];

$k->loadFromDB($id);
$k->name = $_POST['name'];

$k->save();

header("Location: index.php");
?>