<?php /* Smarty version Smarty-3.1.21-dev, created on 2015-05-16 11:36:54
         compiled from "templates/kar.html" */ ?>
<?php /*%%SmartyHeaderCode:918358552553239eca38922-20514724%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    'c52bdc9b6e7ef0d6fc630cc620f168179bc398c1' => 
    array (
      0 => 'templates/kar.html',
      1 => 1431541533,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '918358552553239eca38922-20514724',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.21-dev',
  'unifunc' => 'content_553239ece3c292_12428252',
  'variables' => 
  array (
    'kar' => 0,
    'soeer' => 0,
    'soe' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_553239ece3c292_12428252')) {function content_553239ece3c292_12428252($_smarty_tpl) {?><html>
<head>
    <title><?php echo $_smarty_tpl->tpl_vars['kar']->value->name;?>
</title>
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.2/css/bootstrap.min.css">
    <?php echo '<script'; ?>
 type="text/javascript" src="https://code.jquery.com/jquery-2.1.3.min.js"><?php echo '</script'; ?>
>
    <?php echo '<script'; ?>
 type="text/javascript" src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.2/js/bootstrap.min.js"><?php echo '</script'; ?>
>
	<?php echo '<script'; ?>
  type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.0/jquery.min.js"><?php echo '</script'; ?>
>
    <style type="text/css">
        body { margin: 0 15px; }
    </style>
</head>
<body>
	<div class="masthead">
		<ul class="nav nav-pills pull-right">
			<li><a href="index.php">Home</a></li>
            <li><a href="#">About</a></li>
            <li><a href="#">Contact</a></li>
        </ul>
			<h2 class="muted"><?php echo $_smarty_tpl->tpl_vars['kar']->value->name;?>
</h2>
    </div>
<hr>

<!-- photo + data -->
<div class="row">
    <div class="col-md-6 col-lg-6 text-center">
            <img id="image" src="" class="img-responsive center-block" alt="Vandhane"/>
	</div> 
	 
    <div class="col-md-6 col-lg-6">
	    <div class="row">
			<div class="col-md-12 col-lg-12">
				<div class="panel panel-default">
					<div class="panel-heading">
						<h3 class="panel-title">Kar data</h3>
					</div>
					<div class="panel-body">
                        <form action="updateData.php" class="form-horizontal" method="POST">
                        <input type="hidden" name="id" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
">
                                <table class="table table-striped">
									<tr>
                                        <th>Sensor</th>
                                        <th>Målt værdi</th>
                                        <th>Indtastet værdi</th>
									</tr>
									<tr>
                                        <td>pH-værdi (0-14)</td>
                                        <td class="text-center" id="phData"></td>
                                        <td>
                                                <input name="ph" type="text" class="form-control" id="inputpH3" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->ph;?>
">
                                        </td>
									</tr>
									<tr>
                                        <td>Vandniveau (l)</td>
                                        <td class="text-center" id="volumenData"></td>
                                        <td>
                                                <input name="volumen" type="text" class="form-control" id="inputVand33" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->volumen;?>
">
                                        </td>
									</tr>
									<tr>
                                        <td>Jordfugtighed (%)</td>
                                        <td class="text-center" id="humidityData"></td>
                                        <td>
                                                <input name="humidity" type="text" class="form-control" id="inputJord33" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->humidity;?>
">
                                        </td>
									</tr>
                                </table>
                                <div class="text-right">
									<button type="submit" class="btn btn-primary">
										Gem data
									</button>
                                </div> 
						</form>
					</div> 
				</div> 
			</div> 
        </div> 
    </div>
</div> <!-- Row -->


<!-- SensorØ -->
<div class="row"> 
    <div class="col-md-12 col-lg-12">
		<div class="panel panel-default">
			<div class="panel-heading">
				<h3 class="panel-title">Sensorøer</h3>
			</div>
			<div class="panel-body">
				<table class="table table-striped">
					<tr>
						<th>#</th>
						<th>Jordfugtighed (%)</th>
						<th></th>
					</tr>
						<?php  $_smarty_tpl->tpl_vars['soe'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['soe']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['soeer']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['soe']->key => $_smarty_tpl->tpl_vars['soe']->value) {
$_smarty_tpl->tpl_vars['soe']->_loop = true;
?>
							<tr>
								<td><?php echo $_smarty_tpl->tpl_vars['soe']->value->address;?>
</td>
								<td class="asd" data-oe_id="<?php echo $_smarty_tpl->tpl_vars['soe']->value->id;?>
"><font size='1' color='red'>
								<td> 
									<button onclick="delFunction(<?php echo $_smarty_tpl->tpl_vars['soe']->value->id;?>
)" type="button" class="btn btn-default btn-sm"  style="float: right;" data-toggle="tooltip" data-placement="top" title="Delete">
										<span class="glyphicon glyphicon-trash">  </span>
									</button>
								</td>
							</tr>
						<?php } ?>
				</table>
	
				<!-- Button Opret Sensorø knap modal -->
				<div class="pull-right">
					<button type="button" class="btn btn-primary btn-md" data-toggle="modal" data-target="#SensorOe">
						Opret Sensorø
					</button>
				</div>
			</div> 
		</div>	
    </div>
</div>

<br>

<!-- Ventilstyring + manuel vanding -->
<div class="row">
	<div class="col-md-6 col-lg-6">
		<div class="panel panel-default">
			<div class="panel-heading">
				<h3 class="panel-title">Ventilstyring</h3>
			</div>
			<div class="panel-body text-center">
				<form action="kar.php?id=<?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
" method="post">
				<input type="hidden" name="id" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
"/>
					<div class="col-md-6">
						<h4>Afløb<h4>	
						<button id="OVALVESTATUS" type="submit" name="" value="1" class="" ></button>
					</div>
					<div class="col-md-6">
						<h4>Indløb<h4>	
						<button id="IVALVESTATUS" type="submit" name="" value="1" class="" ></button>
					</div>
				</form>  
			</div>
		</div>
	</div>
	<div class="col-md-6 col-lg-6">
		<div class="panel panel-default">
			<div class="panel-heading">
				<h3 class="panel-title">Manuel vanding</h3>
			</div>
			<div class="panel-body text-center">
				<h4> Manuel vandning status</h4>
				<form action="kar.php?id=<?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
" method="post">
					<button id="MWSTATUS" type="submit" name="" value="1" class=""></button>
				</form>
			</div>
		</div>
	</div>
</div> <!-- Row -->        

<!-- Modal Opret Sensorø -->
<div class="modal fade" id="SensorOe" tabindex="-1" role="dialog" aria-labelledby="mySensorOeLabel" aria-hidden="true">
    <div class="modal-dialog">
		<div class="modal-content">
			<form action="createSO.php" class="form-horizontal" method="POST">
			<input type="hidden" name="id" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true">&times;</span>
					</button>
					<h4 class="modal-title" id="myModalLabel">Opret Sensorø</h4>
				</div>
				<div class="modal-body">
					<div class="form-group">
						<label for="inputAddress3" class="col-sm-2 control-label">Adresse</label>
							<div class="col-sm-10">
								<input name="address" type="text" class="form-control" id="inputAddress3" placeholder="Adresse">
							</div>
					</div>
				<div class="modal-footer">
					<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
					<button type="submit" name="formSubmit" class="btn btn-primary">Save changes</button>
				</div>
				</div>
			</form>  
		</div>
    </div>
</div>
</body>

<?php echo '<script'; ?>
>
	function refreshStatuses(){
		$.ajax({
			url: "auto_refresh_kar.php",
			method: "GET",
			data: { id: <?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
 },
			dataType: "json",
			success: function( data ) {
				if(data.mwstatus == 1){
					$("#MWSTATUS" )
					.html("Stop manuel vandning")
					.removeClass('btn btn-primary btn-md')
					.addClass('btn btn-default btn-md')
					.attr("name","MWSTOP");
					$("#image").attr("src","medie/VHV.png");
				}else{
					$("#MWSTATUS" ).html( "Start manuel vandning")
					.removeClass('btn btn-default btn-md')
					.addClass('btn btn-primary btn-md')
					.attr("name","MWSTART");
					$("#image").attr("src","medie/VH.png");
				}
				
				if(data.ovalvestatus == 1){
					$("#OVALVESTATUS" ).html("Luk afløbsventil")
					.removeClass('btn btn-primary btn-md')
					.addClass('btn btn-default btn-md')
					.attr("name","ClOSEOVALVE");
				}else{
					$("#OVALVESTATUS" ).html( "Åben afløbsventil")
					.removeClass('btn btn-default btn-md')
					.addClass('btn btn-primary btn-md')
					.attr("name","OPENOVALVE");
				}
				
				if(data.ivalvestatus == 1){
					$("#IVALVESTATUS" ).html("Luk indløbsventil")
					.removeClass('btn btn-primary btn-md')
					.addClass('btn btn-default btn-md')
					.attr("name","ClOSEIVALVE");
				}else{
					$("#IVALVESTATUS" ).html( "Åben indløbsventil")
					.removeClass('btn btn-default btn-md')
					.addClass('btn btn-primary btn-md')
					.attr("name","OPENIVALVE");
				}
			}
		})
	}
	
	function refreshKarSensorData(){
		$.ajax({
			url: "auto_refresh_kar.php",
			method: "GET",
			data: { id: <?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
 },
			dataType: "json",
			success: function( data ) {
				if(data.ph >= 0 ){
					$("#phData").html(data.ph);
				} else {
					$("#phData").html("<font size='1' color='red'><i>error: can't read value</i></font>");
				}
			
				if(data.volumen >= 0 ){
					$("#volumenData").html(data.volumen);
				} else {
					$("#volumenData").html("<font size='1' color='red'><i>error: can't read value</i></font>");
				}
			
				if(data.humidity >= 0 ){
					$("#humidityData").html(data.humidity);
				} else {
					$("#humidityData").html("<font size='1' color='red'><i>error: can't read value</i></font>");
				}
				
		
				$(data.sensorOeData).each(function(i, e) {
					var td = $('td.asd[data-oe_id="' + e.oe_id + '"]');
					td.html(e.measure);
				});
				
			}
		})
	}

	$(function() {									// call function when page is loaded
		refreshStatuses();
		refreshKarSensorData();
		setInterval(refreshStatuses, 1000); 		// refresh every second
		setInterval(refreshKarSensorData, 5000);	
	});

	function delFunction(id) 
	{
		if(confirm("Er du sikker på du vil slette denne sensorø?"))
		  document.location = 'deleteS.php?id=' + id;
	}
	
	
<?php echo '</script'; ?>
>

</html><?php }} ?>
