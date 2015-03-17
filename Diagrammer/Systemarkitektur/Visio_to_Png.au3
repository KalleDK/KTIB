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

$dirs_to_convert = _FileListToArrayRec(@ScriptDir, '*',  $FLTAR_FOLDERS  , $FLTAR_NORECUR)
For $i = 1 to $dirs_to_convert[0]
	$files_to_convert = _FileListToArrayRec(@ScriptDir & "\" & $dirs_to_convert[$i], '*.vsdx',  $FLTAR_FILES  , $FLTAR_NORECUR)
	For $j = 1 to $files_to_convert[0]
		$visio_file = @ScriptDir & "\" & $dirs_to_convert[$i] & "\" & $files_to_convert[$j]
		$png_filename = StringRegExpReplace ($files_to_convert[$j] , ".vsdx", ".png")
		$png_file = $export_dir & "\" & $dirs_to_convert[$i] & "\" & $png_filename
		If FileExists ( $png_file ) Then
			If FileGetTime ( $png_file , $FT_MODIFIED , 1 ) > FileGetTime ( $visio_file , $FT_MODIFIED , 1 ) Then
				_ArrayAdd($aNewer, $png_filename)
				;ContinueLoop
			EndIf
		EndIf
		If $first_run Then
			Global $oVisio = ObjCreate("Visio.Application")
			$oVisio.Visible = True
			$first_run = false
		EndIf
		_ArrayAdd($aConverteret, $files_to_convert[$j])
		ConsoleWrite($visio_file)
		$oVSD = $oVisio.Documents.Open($visio_file)
		;$oVisio.SetRasterExportResolution("visRasterUsePrinterResolution")
		$oVisio.Settings.SetRasterExportResolution("visRasterUsePrinterResolution", "600#", "600#", "visRasterPixelsPerInch")
		$oPages = $oVisio.ActiveDocument.Pages
		For $oPage In $oPages
			$oPage.Export($png_file)
			ExitLoop
		Next
		$oVSD.close()
	Next
Next
If Not $first_run Then
	$oVisio.Quit
EndIf
_ArrayDisplay($aConverteret, "Converterede", "", 16)
_ArrayDisplay($aNewer, "PNG er allerede nyere", "", 16)