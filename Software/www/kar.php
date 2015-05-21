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


/******************** Manual wattering *************************/
if(array_key_exists('MWSTART',$_POST)){
        $client->connect();
	//Manual watering start
	$client -> MWStart($id);
} 

if(array_key_exists('MWSTOP',$_POST)){
        $client->connect();
	//Manual watering stop
	$client -> MWStop($id);
}

/********************** In Valve ***************************/
if(array_key_exists('CLOSEIVALVE',$_POST)){
        $client->connect();
	//In valve close
	$client -> IValveClose($id);
}

if(array_key_exists('OPENIVALVE',$_POST)){
        $client->connect();
	//In valve open
	$client -> IValveOpen($id);
}

/********************** OUT Valve ***************************/
if(array_key_exists('CLOSEOVALVE',$_POST)){
        $client->connect();
	//Out valve close
	$client -> OValveClose($id);
}

if(array_key_exists('OPENOVALVE',$_POST)){
        $client->connect();
	//Out valve open
	$client -> OValveOpen($id);
}

// Render page
$smarty = new Smarty;
$smarty->assign('soeer', $kar->getSensorOer());
$smarty->assign('kar', $kar);
$smarty->display('templates/kar.html');

?>

