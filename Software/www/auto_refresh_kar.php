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
	
	//KarSensorData
	$ksData = $kar->getKarSensorData();
	
	//Default if we can't read 
	$ph = -1;							
	$volumen = -1;
	$humidity = -1;
	

	foreach ($ksData as $data) {
		if($data['type']==1){
			$ph = $data['measure'];
		}
		
		if($data['type']==2){
			$volumen = $data['measure'];
		}
		if($data['type']==3){
			$humidity = $data['measure'];
		}
	}
	
	//OeSensorData
	$soer = $kar->getSensorOer();
	$data = array();
	
	foreach($soer as $soe) {
			$soData[] = array(
				'oe_id' => $soe->id,
				'measure' => @$soe->getData()[0]['measure']
			);
		
	}
	
	//print_r($data);
	//die;
	
	//Status for ventil + vandning + sensor
	$statusses = array(
		'mwstatus' => (int) $kar->MWSTATUS,
		'ivalvestatus' => (int) $kar->IVALVESTATUS,
		'ovalvestatus' => (int) $kar->OVALVESTATUS,
		'ph' => (double) $ph,
		'volumen' => (int) $volumen,
		'humidity' => (int) $humidity,
		'sensorOeData' => $soData,
	);
	
	
	echo json_encode($statusses);
?>

