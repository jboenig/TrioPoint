Attribute VB_Name = "MainModule"

Private Sub Main()
    SplashScrn.Status.Caption = "Initializing..."
    SplashScrn.Show
    SplashScrn.Refresh
    MainForm.Show
    Unload SplashScrn
End Sub

