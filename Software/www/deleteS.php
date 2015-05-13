<?php 

include 'mysql.php';
include 'lib/SensorOe.Class.php';

$s = new SensorOE($conn);

$id = $_GET['id'];

if($s->loadFromDB($id) == TRUE){
  $s->delete();
}

// New client
$client = new Client('localhost', 5555);        //PORT 5555
$client->DeleteSonsorOe($s->karID, $id);


header("Location: kar.php?id=".$s->karID);
?>