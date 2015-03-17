

Module mainModule
	Sub Main()
		ReadAndWriteRasterExportResolution
	End Sub
	
	
Sub ReadAndWriteRasterExportResolution() 
    ' Access the Visio ApplicationSettings object. 
    Dim appSettings As Visio.ApplicationSettings 
    Set appSettings = Application.Settings 
    
    ' Write out current Raster Export Resolution settings. 
    System.Diagnostics.Debug.Print("*** Current Settings")
    WriteOutCurrentExportResolution(appSettings)
     
    ' Use the new SetRasterExportResolution method 
    ' to change the current Raster Export Resolution settings 
    ' to a custom setting of 800x600 using Pixels per Centimeter as the 
    ' units of measurement. 
    appSettings.SetRasterExportResolution(visRasterUseCustomResolution, 800, 600, visRasterPixelsPerCm )
     
    ' Write out current Raster Export Resolution settings 
    ' after changing the values to a custom setting. 
    System.Diagnostics.Debug.Print("*** Settings after changing") 
    WriteOutCurrentExportResolution(appSettings)
     
    ' Reset the Raster Export Resolution settings 
    ' to the default (the screen). 
    appSettings.SetRasterExportResolution(visRasterUseScreenResolution)
     
    ' Write out current settings after changing 
    ' the values back to the default. 
    System.Diagnostics.Debug.Print("*** Settings after resetting")
    WriteOutCurrentExportResolution(appSettings) 
End Sub 
 
Private Sub WriteOutCurrentExportResolution(appSettings As Visio.ApplicationSettings) 
    Dim res As VisRasterExportResolution 
    Dim width As Double 
    Dim height As Double 
    Dim units As VisRasterExportResolutionUnits 
     
    ' GetRasterExportResolution is a new method to 
    ' let you access the Raster Export Resolution data. 
    appSettings.GetRasterExportResolution(res, width, height, units)
     
    ' Figure out what value was returned for 
    ' VisRasterExportResolution and write out 
    ' the information to the Immediate Window. 
    Select Case res 
        Case VisRasterExportResolution.visRasterUseScreenResolution 
            System.Diagnostics.Debug.Print("RasterExportResolution: visRasterUseScreenResolution")
        Case VisRasterExportResolution.visRasterUsePrinterResolution 
            System.Diagnostics.Debug.Print("RasterExportResolution: visRasterUseScreenResolution")
        Case VisRasterExportResolution.visRasterUseSourceResolution 
            System.Diagnostics.Debug.Print("RasterExportResolution: visRasterUseSourceResolution")
        Case VisRasterExportResolution.visRasterUseCustomResolution 
            System.Diagnostics.Debug.Print("Export Resolution: visRasterUseCustomResolution")
            ' Only if the VisRasterExportResolution value is visRasterUseCustomResolution 
            ' will the other values return something other than null. 
            System.Diagnostics.Debug.Print("Width: " & width)
            System.Diagnostics.Debug.Print("Height: " & height)
             
            Select Case units 
                Case VisRasterExportResolutionUnits.visRasterPixelsPerInch 
                    System.Diagnostics.Debug.Print("Export Resolution Units: visRasterPixelsPerInch")
                Case VisRasterExportResolutionUnits.visRasterPixelsPerCm 
                    System.Diagnostics.Debug.Print("Export Resolution Units: visRasterPixelsPerCm")
            End Select 
    End Select 
End Sub
End Module
