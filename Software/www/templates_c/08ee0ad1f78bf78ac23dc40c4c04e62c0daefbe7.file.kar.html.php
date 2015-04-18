<?php /* Smarty version Smarty-3.1.21-dev, created on 2015-04-17 20:55:36
         compiled from "templates/kar.html" */ ?>
<?php /*%%SmartyHeaderCode:1905965956551ce9dc831040-47719617%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '08ee0ad1f78bf78ac23dc40c4c04e62c0daefbe7' => 
    array (
      0 => 'templates/kar.html',
      1 => 1429296935,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '1905965956551ce9dc831040-47719617',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.21-dev',
  'unifunc' => 'content_551ce9dc85eb26_39789490',
  'variables' => 
  array (
    'kar' => 0,
    'MW' => 0,
    'soeer' => 0,
    'soe' => 0,
    'ovalveopen' => 0,
    'ivalveopen' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_551ce9dc85eb26_39789490')) {function content_551ce9dc85eb26_39789490($_smarty_tpl) {?><html>
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
        <?php if ($_smarty_tpl->tpl_vars['MW']->value) {?>
            <img id="image" src="medie/VHV.png" class="img-responsive center-block" alt="Vandhane"/>
        <?php } else { ?>
            <img id="image" src="medie/VH.png" class="img-responsive center-block" alt="Vandhane"/>
        <?php }?>
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
                                        <td>6.5</td>
                                        <td>
                                                <input name="ph" type="text" class="form-control" id="inputpH3" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->ph;?>
">
                                        </td>
									</tr>
									<tr>
                                        <td>Vandniveau (l)</td>
                                        <td>4</td>
                                        <td>
                                                <input name="volume" type="text" class="form-control" id="inputVand33" value="<?php echo $_smarty_tpl->tpl_vars['kar']->value->volume;?>
">
                                        </td>
									</tr>
									<tr>
                                        <td>Jordfugtighed (%)</td>
                                        <td>80</td>
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
						<th>Jordfugtihed (%)</th>
						<th></th>
					</tr>
						<?php  $_smarty_tpl->tpl_vars['soe'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['soe']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['soeer']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['soe']->key => $_smarty_tpl->tpl_vars['soe']->value) {
$_smarty_tpl->tpl_vars['soe']->_loop = true;
?>
							<tr>
								<td><?php echo $_smarty_tpl->tpl_vars['soe']->value['address'];?>
</td>
								<td>----</td>
								<td> 
									<button onclick="delFunction(<?php echo $_smarty_tpl->tpl_vars['soe']->value['id'];?>
)" type="button" class="btn btn-default btn-sm"  style="float: right;" data-toggle="tooltip" data-placement="top" title="Delete">
										<span class="glyphicon glyphicon-trash"> </span>
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
						<?php if ($_smarty_tpl->tpl_vars['ovalveopen']->value) {?>
							<button type="submit" name="ClOSEOVALVE" value="1" class="btn btn-default btn-md" >
								Luk afløbsventil
							</button>
							
						<?php } else { ?>
							<button type="submit" name="OPENOVALVE" value="1" class="btn btn-primary btn-md" >
								Åben afløbsventil
							</button>
						<?php }?>
					</div>
					<div class="col-md-6">
						<h4>Indløb<h4>
						<?php if ($_smarty_tpl->tpl_vars['ivalveopen']->value) {?>
							<button type="submit" name="CLOSEIVALVE" value="1" class="btn btn-default btn-md">
								Luk Indløbsventil
							</button>
						<?php } else { ?>
							<button type="submit" name="OPENIVALVE" value="1" class="btn btn-primary btn-md">
								Åben Indløbsventil
							</button>
						<?php }?>
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
				<form action="kar.php?id=<?php echo $_smarty_tpl->tpl_vars['kar']->value->id;?>
" method="post">
					<h4>Manuel vanding er i gang<h4>
					<?php if ($_smarty_tpl->tpl_vars['MW']->value) {?>
						<button type="submit" name="MWSTOP" value="1" class="btn btn-default btn-md">Stop manuel vandning</button>
					<?php } else { ?>
						<button type="submit" name="MWSTART" value="1" class="btn btn-primary btn-md">Start manuel vandning</button>
					<?php }?>
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

<?php echo '<script'; ?>
>
function delFunction(id) 
{
    if(confirm("Er du sikker på du vil slette denne sensorø?"))
      document.location = 'deleteS.php?id=' + id;
}
<?php echo '</script'; ?>
>

</body>
</html><?php }} ?>
