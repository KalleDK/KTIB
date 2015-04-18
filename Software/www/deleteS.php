<?php 

include 'mysql.php';
include 'lib/SensorOe.Class.php';

$s = new SensorOE($conn);

$id = $_GET['id'];

if($s->loadFromDB($id) == TRUE){
  $s->delete();
}

header("Location: kar.php?id=".$s->karID);
?>