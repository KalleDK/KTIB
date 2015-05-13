<?php 

include 'mysql.php';
include 'lib/SensorOe.Class.php';

$id = $_POST['id'];

$s = new SensorOE($conn);

$s->address = $_POST['address'];
$s->karID = $_POST['id'];

$s->save();

// New client
$client = new Client('localhost', 5555);        //PORT 5555
$client->AddSonsorOe($id, $s->id);

header("Location: kar.php?id=".$id);
?>


