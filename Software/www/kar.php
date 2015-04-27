<?php   
include 'Smarty/libs/Smarty.class.php';
include "mysql.php";
include "lib/Kar.Class.php";
include "lib/socketClient.Class.php";

$id = $_GET['id'];

// Kar connection
$kar = new Kar($conn);

// New client
$client = new Client('localhost', 5555);        //PORT 5555


// Load kar with $id
if(!$kar->loadFromDB($id))
{
  die("Ikke fundet!");
}

// Render page
$soeer = $kar->getSensorOer();

// Til test!!!!!!!!!!!!!!!!!!!!!
$sdata = $kar->getSensorData();
echo $sdata->ph;
// Til test!!!!!!!!!!!!!!!!!!!!!

$smarty = new Smarty;
$smarty->assign('soeer', $soeer);
$smarty->assign('kar', $kar);

/******************** Manual wattering *************************/
if(array_key_exists('MWSTART',$_POST)){
	//Manual watering start
	$client -> MWStart($id);
} 

if(array_key_exists('MWSTOP',$_POST)){
	//Manual watering stop
	$client -> MWStop($id);
}

/********************** In Valve ***************************/
if(array_key_exists('CLOSEIVALVE',$_POST)){
	//In valve close
	$client -> IValveClose($id);
}

if(array_key_exists('OPENIVALVE',$_POST)){
	//In valve open
	$client -> IValveOpen($id);
}

/********************** OUT Valve ***************************/
if(array_key_exists('ClOSEOVALVE',$_POST)){
	//Out valve close
	$client -> OValveClose($id);
}

if(array_key_exists('OPENOVALVE',$_POST)){
	//Out valve open
	$client -> OValveOpen($id);
}

$smarty->display('templates/kar.html');
?>

