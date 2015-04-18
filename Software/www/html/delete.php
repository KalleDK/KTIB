<?php 

include 'mysql.php';
include 'lib/Kar.Class.php';

$k = new Kar($conn);

$id = $_GET['id'];

if($k->loadFromDB($id) == TRUE){
  $k->delete();
}



header("Location: index.php");
?>