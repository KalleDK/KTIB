<?php 

include 'mysql.php';
include 'lib/SensorOe.Class.php';

$id = $_POST['id'];

$s = new SensorOE($conn);

$s->address = $_POST['address'];
$s->karID = $_POST['id'];

$s->save();

header("Location: kar.php?id=".$id);
?>


