<?php   
	include 'Smarty/libs/Smarty.class.php';
	include "mysql.php";
	include "lib/Kar.Class.php";

	$id = $_GET['id'];
	
	// Kar connection
	$kar = new Kar($conn);
	
	// Load kar with $id
	if(!$kar->loadFromDB($id))
	{
		die("Ikke fundet!");
	}
	
	$statusses = array(
		'mwstatus' => (int) $kar->MWSTATUS,
		'ivalvestatus' => (int) $kar->IVALVESTATUS,
		'ovalvestatus' => (int) $kar->OVALVESTATUS,
	);
	
	echo json_encode($statusses);
?>

