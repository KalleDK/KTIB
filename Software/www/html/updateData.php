<?php 

include 'mysql.php';
include 'lib/Kar.Class.php';

$k = new Kar($conn);

$id = $_POST['id'];

if(!$k->loadFromDB($id)){
  die("Ikke fundet: ".$id);
}

$k->ph = $_POST['ph'];
$k->volume = $_POST['volume'];
$k->humidity = $_POST['humidity'];

$k->save();

header("Location: kar.php?id=".$id);
?>

