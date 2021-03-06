<?php
include "lib/SensorOe.Class.php";

class Kar
{
    private $dbConn;
    private $created = false;
    
    public $id;
    public $name;
    public $address;
    public $ph;
    public $volumen;
    public $humidity;
    public $MWSTATUS;
	public $IVALVESTATUS;
	public $OVALVESTATUS;
	
    public function Kar($dbConn)
    {
	$this->dbConn = $dbConn;
    }
    
    public function loadFromDB($ID)
    {
		$result = $this->dbConn->query("SELECT * FROM Kar WHERE id=".$ID);
		if($result->num_rows) {
			$row = $result->fetch_assoc();
			$this->id = $row['id'];
			$this->name = $row['name'];
			$this->address = $row['address'];
			$this->ph = $row['ph'];
			$this->volumen= $row['volumen'];
			$this->humidity = $row['humidity'];
			$this->MWSTATUS = $row['MWSTATUS'];
			$this->IVALVESTATUS = $row['IVALVESTATUS'];
			$this->OVALVESTATUS = $row['OVALVESTATUS'];
			$this->created = true;
			return true;
		}	
		return false;
    }
    
    public function save()
    {
		if($this->created){
			$sql = "UPDATE Kar SET name = '".$this->name."', address = '".$this->address."', ph = '".$this->ph."', volumen = '".$this->volumen."', humidity = '".$this->humidity."', MWSTATUS = '".$this->MWSTATUS."', IVALVESTATUS = '".$this->IVALVESTATUS."', OVALVESTATUS = '".$this->OVALVESTATUS."' WHERE id = ".$this->id;
			echo $sql;	
		} else {
			$sql = "INSERT INTO Kar(name, address, ph, volumen, humidity, MWSTATUS, IVALVESTATUS, OVALVESTATUS) VALUES ('".$this->name."','".$this->address."','".$this->pH."','".$this->volumen."', '".$this->humidity."', '".$this->MWSTATUS."', '".$this->IVALVESTATUS."', '".$this->OVALVESTATUS."')";
			$this->created = TRUE;
			echo $sql;
		}
		$this->dbConn->query($sql);
    }
    
    public function delete()
    {
		if($this->created){
			$sql = "DELETE FROM Kar WHERE id=".$this->id;
			echo $sql;
			$this->dbConn->query($sql);
		}else{
			$this->dbConn->error;
		}
    }
    
    public function getSensorOer()
    {
		$result = $this->dbConn->query("SELECT * FROM SensorOe WHERE KarID=".$this->id);
		$soeer = array();
		while($row = $result->fetch_assoc()) {
			$obj = new SensorOe($this->dbConn);
			$obj->id = $row['id'];
			$obj->address = $row['address'];
			$obj->karID = $row['karID'];
			
			$obj->created = true;
  
			$soeer[] = $obj;
		}
		
		return $soeer;
    }
	
	public function getKarSensorData()
	{
		$result = $this->dbConn->query("
			SELECT t1.* FROM KarSensorData t1
			JOIN (SELECT type, MAX(time) time FROM KarSensorData WHERE KarID=".$this->id." GROUP BY type) t2
			ON t1.type = t2.type AND t1.time = t2.time ORDER BY type;
		");
	
		$ksData = array();
		while($row = $result->fetch_assoc()) {
			$ksData[] = $row;
		}
		return $ksData; 
	}
	
}


function getKars($conn) {
    $res = $conn->query("SELECT * FROM Kar");
    $kars = array();
    while($row = $res->fetch_assoc()) 
    {
		$kars[] = $row;
    }
    return $kars;
}

?>