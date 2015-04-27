<?php 

class SensorOE
{
    private $dbConn;
    private $created = false;
    
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
} 
?>