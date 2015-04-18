<?php /* Smarty version Smarty-3.1.21-dev, created on 2015-04-17 14:16:43
         compiled from "templates/index.html" */ ?>
<?php /*%%SmartyHeaderCode:1442946506551cec1ce1d557-09256658%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '4f28c517d643b5f8d4c8220e10679a01ba7cbe3b' => 
    array (
      0 => 'templates/index.html',
      1 => 1429273000,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '1442946506551cec1ce1d557-09256658',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.21-dev',
  'unifunc' => 'content_551cec1ce48160_79562911',
  'variables' => 
  array (
    'kars' => 0,
    'kar' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_551cec1ce48160_79562911')) {function content_551cec1ce48160_79562911($_smarty_tpl) {?><html>
<head>
    <title>Automatisk Vandingssystem</title>
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
			<h1>Automatisk Vandingssystem</h1>
	</div>
<hr>

    <div class="row">
		<div class="col-md-6 col-lg-6">
			<div role="tabpanel">
				<!-- Nav tabs -->
				<ul class="nav nav-tabs" role="tablist">
					<li role="presentation" class="active"><a href="#home" aria-controls="home" role="tab" data-toggle="tab">Home</a></li>
					<li role="presentation"><a href="#service" aria-controls="service" role="tab" data-toggle="tab">Service</a></li>
				</ul>

				<!-- Tab panes -->
				<div class="tab-content">
					<div role="tabpanel" class="tab-pane active" id="home">
						<br>
						<ul class="list-group">
							<a href="#" class="list-group-item disabled">
								Kar
							</a>
							<?php  $_smarty_tpl->tpl_vars['kar'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['kar']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['kars']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['kar']->key => $_smarty_tpl->tpl_vars['kar']->value) {
$_smarty_tpl->tpl_vars['kar']->_loop = true;
?>
								<li class="list-group-item">
									<div class="row">
										<div class="col-md-3">
											<a href="kar.php?id=<?php echo $_smarty_tpl->tpl_vars['kar']->value['id'];?>
"><?php echo $_smarty_tpl->tpl_vars['kar']->value['name'];?>
</a>
										</div>
										
										<div class="col-md-6" style="text-align: center;">
											<?php if ($_smarty_tpl->tpl_vars['kar']->value['OVALVESTATUS']) {?>
												<span class="label label-warning">Tømmer kar</span>
											<?php }?>
									
											<?php if ($_smarty_tpl->tpl_vars['kar']->value['IVALVESTATUS']) {?>
												<span class="label label-success">Fylder kar</span>
											<?php }?>
										
											<?php if ($_smarty_tpl->tpl_vars['kar']->value['MWSTATUS']) {?>
												<span class="label label-info">Vander</span>
											<?php }?>
										</div>
										
										<div class="col-md-3">
											<button onclick="delFunction(<?php echo $_smarty_tpl->tpl_vars['kar']->value['id'];?>
)" type="button" class="btn btn-default btn-sm"  style="float: right;" data-toggle="tooltip" data-placement="top" title="Delete">
											<span class="glyphicon glyphicon-trash"> </span>
										</button>
					  
										<button type="button" class="btn btn-default btn-sm" data-toggle="modal" data-target=".bs-example-modal-sm<?php echo $_smarty_tpl->tpl_vars['kar']->value['id'];?>
" style="float: right;" data-toggle="tooltip" data-placement="top" title="Edit name"> 
											<span class="glyphicon glyphicon-edit"> </span>
										</button>
										</div>
										
										
									</div>
									<!-- Edit modal -->
									<div class="modal fade bs-example-modal-sm<?php echo $_smarty_tpl->tpl_vars['kar']->value['id'];?>
" tabindex="-1" role="dialog" aria-labelledby="mySmallModalLabel" aria-hidden="true">
										<div class="modal-dialog modal-sm">
											<div class="modal-content">
											<form action="edit.php?id=<?php echo $_smarty_tpl->tpl_vars['kar']->value['id'];?>
" class="form-horizontal" method="POST">
												<div class="modal-header">
													<button type="button" class="close" data-dismiss="modal" aria-label="Close">
														<span aria-hidden="true">&times;</span>
													</button>
													<h4 class="modal-title" id="myModalLabel">Edit name</h4>
												</div>
												<div class="modal-body">
													<div class="form-group">
														<label for="inputName3" class="col-sm-2 control-label">Navn </label>
														<div class="col-sm-10">
															<input name="name" type="text" class="form-control" id="inputName3" placeholder="Navn">
														</div>
													</div>
												</div>
												<div class="modal-footer">
													<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
													<button type="submit" name="formSubmit" class="btn btn-primary">Save changes</button>
												</div>
											</form>
											</div>
										</div>
									</div>
								</li>
							<?php } ?>
						</ul>
					</div>
					<!-- Service -->
					<div role="tabpanel" class="tab-pane" id="service">
						<br>
						<div class="control-group text-center">
							<button type="button" class="btn btn-primary btn-lg" data-toggle="modal" data-target="#myModal">
								Opret kar
							</button>
						</div>
					</div>
				</div>
			</div>

			<!-- Opret kar Modal -->
			<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
	    <div class="modal-dialog">
		<div class="modal-content">
		    <div class="modal-header">
			<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
			<h4 class="modal-title" id="myModalLabel">Opret kar</h4>
		    </div>
		    <div class="modal-body">
			<form action="create.php" class="form-horizontal" method="POST">
			    <div class="form-group">
				<label for="inputName3" class="col-sm-2 control-label">Navn </label>
				<div class="col-sm-10">
				    <input name="name" type="text" class="form-control" id="inputName3" placeholder="Navn">
				</div>
			    </div>
			    <div class="form-group">
				<label for="inputAddress3" class="col-sm-2 control-label">Adresse</label>
				<div class="col-sm-10">
				    <input name="address" type="text" class="form-control" id="inputAddress3" placeholder="Adresse">
				</div>
			    </div>
			    <div class="form-group">
				<div class="col-sm-offset-2 col-sm-10">
				
				    <button type="submit" name="formSubmit" class="btn btn-primary">Opret kar</button>
				</div>
			    </div>
			</form>
		    </div>
		    <div class="modal-footer">
			<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
		    </div>
		</div>
	    </div>
	</div>
		</div>
			<div class="col-md-6 col-lg-6" style="margin-top:45px">
				  <img src="medie/vand.PNG" class="img-responsive center-block" alt="Responsive image"/>
			</div>
    </div>
	
<?php echo '<script'; ?>
>
	function delFunction(id) 
	{
		if(confirm("Er du sikker på du vil slette dette kar"))
		  document.location = 'delete.php?id=' + id;
	}
<?php echo '</script'; ?>
>

</body>
</html><?php }} ?>
