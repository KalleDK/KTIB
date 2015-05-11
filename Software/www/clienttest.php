

<?php
ini_set('display_errors', 'On');
error_reporting(E_ALL);

include 'lib/socketClient.Class.php';

$c = new Client('localhost', 5555);

/*$c->test()*/;                                         //Test function

$test = 1;

echo "Status outside function: ".$test."<br>";

$c->manualWatering($test);                               //Test Manual vandning

?>