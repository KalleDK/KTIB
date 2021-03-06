<?php

class Client 
{
    private $dbConn;
    private $created = false;
        
    private $socket;
    private $ip;
    private $port;
        
    public function Client($ip, $port)
    {
        $this->ip = $ip;
        $this->port = $port;
    }
        
    //Connection 
    public function connect()
	{
                //Create socket
                $this->socket = socket_create(AF_INET, SOCK_STREAM, 0)
                        or die("Client: socket_create(): error creating socket");
                
                //connect to server
                socket_connect($this->socket, $this->ip, $this->port)
                        or die("Client: socket_connect(): error connecting to socket");
	}
        
	public function close()
	{
		//close socket
		socket_close($this->socket);
	}
    
	/*******************Manual vandning*********************/
	public function MWStart($karID){
		$message = "MWSTART ".$karID."\r\n";
		
		 //send status to server
		socket_write($this->socket, $message) 
			or die("Could not send data to server<br>");
	}
	
	public function MWStop($karID){
		$message = "MWSTOP ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
			or die("Could not send data to server<br>");	
	}
        
	/*******************Ventil status Indl�b*********************/
	public function IValveOpen($karID){
		$message = "IVALVEOPEN ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message)
			or die("Could not send data to server<br>");
	}
	
	public function IValveClose($karID){
		$message = "IVALVECLOSE ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message)
			or die("Could not send data to server<br>");
	}

        
	/*******************Ventil status afl�b*********************/
	public function OValveOpen($karID){
		$message = "OVALVEOPEN ".$karID."\r\n";
			
		//send status to server
		socket_write($this->socket, $message)
			or die("Could not send data to server<br>");
	}
	
	public function OValveClose($karID){
		$message = "OVALVECLOSE ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
			or die("Could not send data to server<br>");
	}	
	
	/**************************Sensor�e opret og slet***********************************/
	public function AddSonsorOe($karID, $SOeID){
		$message  = "ADDSENSOROE ".$karID." ".$SOeID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
			or die("Could not send data to server<br>");
	}
	
		public function DeleteSonsorOe($karID, $SOeID){
		$message  = "DELETESENSOROE ".$karID." ".$SOeID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
			or die("Could not send data to server<br>");	
	}
	
	 /*************************Just a test**********************************************/
	public function manualWatering($status){
		echo "Status inside function: ".$status."<br>";
		if($status == 1){
				echo "Client: Start watering : ".$status."<br>";
		}else if ($status == 2){
				echo "Client: Stop watering: ".$status."<br>";
		}else{
				echo "Client: Invalid value: ".$status."<br>";
		}
		
		$this->connect();
		
		//send status to server
		socket_write($this->socket, $status) 
				or die("Could not send data to server<br>");
		
		// get server response
		$this->result = socket_read ($this->socket, 1024) 
				or die("Could not read server response<br>");
		
		echo "Reply From Server  :".$this->result;
		
		// close socket
		socket_close($this->socket);
	}
	
	
	/*tidliger brug status*/
	
	public function MWStatus($karID){
		$message = "MWSTATUS ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
			or die("Could not send data to server<br>");
		
		// get server response
		$data = socket_read ($this->socket, 1024) 
			or die("Could not read server response<br>");
		
		echo "Reply From Server  :".$data."<br>";
		
		// Return true if data = MWSTATUS 1
		return ($data == "MWSTATUS 1\r\n");
	}
	
		
	public function IValveStatus($karID){
		$message = "IVALVESTATUS ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
				or die("Could not send data to server<br>");
		
		// get server response
		$data = socket_read ($this->socket, 1024) 
				or die("Could not read server response<br>");
		
		echo "Reply From Server  :".$data."<br>";
		
		// Return true if data = IVALVESTATUS 1
		return ($data == "IVALVESTATUS 1\r\n");
	}
	
	public function OValveStatus($karID){
		$message = "OVALVESTATUS ".$karID."\r\n";
		
		//send status to server
		socket_write($this->socket, $message) 
				or die("Could not send data to server<br>");
		
		// get server response
		$data = socket_read ($this->socket, 1024) 
				or die("Could not read server response<br>");
		
		echo "Reply From Server  :".$data."<br>";
		
		// Return true if data = OVALVESTATUS 1
		return ($data == "OVALVESTATUS 1\r\n");
	}
	
	/*************************Just a test**********************************************/
	public function test(){
		$message = "Hello Server";
		echo "Message To server :".$message."<br>";
		
		$this->connect();

		// send string to server
		socket_write($this->socket, $message, strlen($message)) or die("Could not send data to server<br>");
		
		// get server response
		$this->result = socket_read ($this->socket, 1024) or die("Could not read server response<br>");
				
		echo "Reply From Server  :".$this->result;

		// close socket
		socket_close($this->socket);
    }
}
?>