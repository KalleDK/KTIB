#include <File.au3>
#include <Array.au3>
#include <MsgBoxConstants.au3>


Global $oVSD
Global $oPages
Global $export_dir = @ScriptDir ;& "/../Rapport/Systemarkitektur"
Global $visio_file
Global $png_file
Global $first_run = true

Global $aConverteret[0]
Global $aNewer[0]

Global $oVisio = ObjCreate("Visio.Application")
Global $oSettings = ObjCreate("Visio.Application.Settings")

$oVisio.Visible = True
ConsoleWrite($oVisio)
$oVSD = $oVisio.Documents.Open(@ScriptDir & "/flaf.vsdx")
$oVisio.Settings.SetRasterExportResolution("visRasterUsePrinterResolution")
$oSettings.SetRasterExportResolution(1)

$oPages = $oVisio.ActiveDocument.Pages
For $oPage In $oPages
	$oPage.Export(@ScriptDir & "/flaf.jpg")
	ExitLoop
Next
$oVSD.close()
$oVisio.Quit