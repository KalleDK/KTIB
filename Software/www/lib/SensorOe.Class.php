<?php 

class SensorOE
{
    private $dbConn;
    public $created = false;
    
    public $id;
    public $address;
    public $karID;
    
    public function SensorOE($dbConn)
    {
		$this->dbConn = $dbConn;
    }
    
    public function loadFromDB($ID)
    {
		$result = $this->dbConn->query("SELECT * FROM SensorOe WHERE id=".$ID);
		if($result->num_rows) {
			$row = $result->fetch_assoc();
			$this->id = $row['id'];
			$this->address = $row['address'];
			$this->karID = $row['karID'];
			$this->created = true;
			return true;
		}
		return false;
    }
    
	public function save()
    {
		if($this->created){
			$sql = "UPDATE SensorOE SET address = '".$this->address."', karID = '".$this->karID."' WHERE id = ".$this->id;
			echo $sql;
		} else {
			$sql = "INSERT INTO SensorOe(address, karID) VALUES ('".$this->address."', '".$this->karID."')";
			$this->created = TRUE;
			$this->id = $dbConn->insert_id;
			echo $sql;
		}
		$this->dbConn->query($sql);
    }
    
    public function delete()
    {
		if($this->created){
			$sql = "DELETE FROM SensorOe WHERE id=".$this->id;
			echo $sql;
			$this->dbConn->query($sql);
		}else{
			$this->dbConn->error;
		}
    }
	
	public function getData()
	{
		$result = $this->dbConn->query("
			SELECT t1.* FROM OeSensorData t1
			JOIN (SELECT type, MAX(time) time FROM OeSensorData WHERE OeID=".$this->id." GROUP BY type) t2
			ON t1.type = t2.type AND t1.time = t2.time ORDER BY type;
		");
	
		$osData = array();
		while($row = $result->fetch_assoc()) {
			$osData[] = $row;
		}
		return $osData; 
	}
} 
?>