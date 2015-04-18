<?php 

ini_set('display_errors', 'On');
error_reporting(E_ALL);


$host = "localhost";
$username = "laerke";
$password = "stud";
$db = "testdb";


// Create connection
$conn = new mysqli($host, $username, $password, $db);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
  
//$conn->close();
?>