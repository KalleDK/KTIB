<?php
//phpinfo();
include 'Smarty/libs/Smarty.class.php';
include "mysql.php";
include "lib/Kar.Class.php";

// Get all Kar
$kars = getKars($conn);

// Render page
$smarty = new Smarty;
$smarty->assign('kars', $kars);
$smarty->display('templates/index.html');
?>
