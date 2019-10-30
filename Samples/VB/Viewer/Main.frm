VERSION 5.00
Object = "{D72BD7D1-5F7C-41B4-ADD0-36EF7878440B}#1.0#0"; "SSCL.dll"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form Main 
   Caption         =   "ShapeShifter Sample - Viewer"
   ClientHeight    =   7200
   ClientLeft      =   8415
   ClientTop       =   6120
   ClientWidth     =   9600
   LinkTopic       =   "Form1"
   ScaleHeight     =   480
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   640
   StartUpPosition =   2  'CenterScreen
   Begin ShapeShifterCtl.ShapeShifterCtl ShapeShifterCtl1 
      Height          =   5955
      Left            =   90
      TabIndex        =   0
      Top             =   90
      Width           =   7935
      _cx             =   13996
      _cy             =   10504
      BackColor       =   16711680
      ModelType       =   "ShapeShifter.Model.1"
      ViewportType    =   "ShapeShifter.Viewport.1"
      ControllerType  =   "ShapeShifter.BrowseController.1"
      ModelFile       =   ""
      ModelReaderType =   "ShapeShifter.ModelReaderXML.1"
      FOV             =   1.57
      Aspect          =   1.332
      NearPlane       =   1
      FarPlane        =   10000
      CameraLocationX =   0
      CameraLocationY =   0
      CameraLocationZ =   -10
      CameraAimX      =   0
      CameraAimY      =   0
      CameraAimZ      =   1
      CameraUpX       =   0
      CameraUpY       =   1
      CameraUpZ       =   0
      AnimationInterval=   0
      AnimationEnabled=   0   'False
      CollisionsEnabled=   0   'False
      InternetEnabled =   0   'False
   End
   Begin MSComDlg.CommonDialog DlgFileOpen 
      Left            =   6930
      Top             =   90
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
      FileName        =   "*.xml"
      Filter          =   "XML files|*.xml|3D Studio files|*.3ds"
   End
   Begin MSComDlg.CommonDialog DlgFileSave 
      Left            =   6930
      Top             =   630
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      FileName        =   "*.xml"
      Filter          =   "XML files|*.xml"
   End
   Begin VB.Menu File 
      Caption         =   "File"
      Begin VB.Menu FileOpen 
         Caption         =   "Open"
      End
      Begin VB.Menu FileSave 
         Caption         =   "Save"
      End
      Begin VB.Menu FileExit 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu Edit 
      Caption         =   "Edit"
      Begin VB.Menu EditProperties 
         Caption         =   "Properties"
      End
   End
   Begin VB.Menu View 
      Caption         =   "View"
      Begin VB.Menu ViewAxes 
         Caption         =   "Axes"
      End
      Begin VB.Menu Sep1 
         Caption         =   "-"
      End
      Begin VB.Menu ViewFront 
         Caption         =   "Front"
      End
      Begin VB.Menu ViewBack 
         Caption         =   "Back"
      End
      Begin VB.Menu ViewLeft 
         Caption         =   "Left"
      End
      Begin VB.Menu ViewRight 
         Caption         =   "Right"
      End
      Begin VB.Menu ViewTop 
         Caption         =   "Top"
      End
      Begin VB.Menu ViewBottom 
         Caption         =   "Bottom"
      End
   End
   Begin VB.Menu Options 
      Caption         =   "Options"
      Begin VB.Menu OptionsRendering 
         Caption         =   "Rendering"
      End
      Begin VB.Menu EditNavigation 
         Caption         =   "Navigation"
      End
   End
   Begin VB.Menu Help 
      Caption         =   "Help"
      Begin VB.Menu HelpAbout 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "Main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub FileExit_Click()
    Unload Me
End Sub

Private Sub FileOpen_Click()
    ' Display file open dialog
    On Error GoTo OpenCancelled
    DlgFileOpen.ShowOpen
    On Error GoTo 0

    Screen.MousePointer = vbHourglass

    ShapeShifterCtl1.ModelFile = DlgFileOpen.FileName
    ShapeShifterCtl1.LoadFile
    ShapeShifterCtl1.ReloadDevice
    
    Screen.MousePointer = vbNormal
    
OpenCancelled:
End Sub

Private Sub FileSave_Click()
    DlgFileSave.ShowSave
    
    Screen.MousePointer = vbHourglass
    
    Dim WriterXML As ShapeShifter.ModelWriterXML
    Set WriterXML = New ShapeShifter.ModelWriterXML
    WriterXML.Write DlgFileSave.FileName, ShapeShifterCtl1.Model

    Screen.MousePointer = vbNormal
End Sub

Private Sub Form_Resize()
    ShapeShifterCtl1.Left = Main.ScaleLeft
    ShapeShifterCtl1.Top = Main.ScaleTop
    ShapeShifterCtl1.Width = Main.ScaleWidth
    ShapeShifterCtl1.Height = Main.ScaleHeight
End Sub

Private Sub LightingAmbient_Click()
    Dim Red As Single
    Dim Green As Single
    Dim Blue As Single
    Dim Alpha As Single
    ShapeShifterCtl1.Model.GetAmbientLight Red, Green, Blue, Alpha
    dlgAmbientColor.Red = Str(Red)
    dlgAmbientColor.Green = Str(Green)
    dlgAmbientColor.Blue = Str(Blue)
    If (dlgAmbientColor.DoModal(Me) = vbOK) Then
        Red = Val(dlgAmbientColor.Red)
        Green = Val(dlgAmbientColor.Green)
        Blue = Val(dlgAmbientColor.Blue)
        ShapeShifterCtl1.Model.SetAmbientLight Red, Green, Blue, Alpha
        ShapeShifterCtl1.Viewport.Render
        ShapeShifterCtl1.Viewport.RefreshScreen
    End If
    Unload dlgAmbientColor
End Sub

Private Sub EditNavigation_Click()
    Dim MoveInc As Single
    MoveInc = ShapeShifterCtl1.Controller.GetBehavior("CameraStep").MoveIncrement
    dlgNavigation.MoveIncrement = Str(MoveInc)
    If (dlgNavigation.DoModal(Me) = vbOK) Then
        MoveInc = Val(dlgNavigation.MoveIncrement)
        ShapeShifterCtl1.Controller.GetBehavior("CameraStep").MoveIncrement = MoveInc
    End If
    Unload dlgNavigation
End Sub

Private Sub EditProperties_Click()
    If (ShapeShifterCtl1.Controller.SelectionList.Count > 0) Then
        dlgProperties.PutNode ShapeShifterCtl1.Controller.SelectionList.Item(0)
        dlgProperties.DoModal Me
        Unload dlgProperties
    End If
End Sub


Private Sub HelpAbout_Click()
    dlgAbout.DoModal Me
End Sub

Private Sub OptionsRendering_Click()
    If (ShapeShifterCtl1.Viewport.FillMode = 1) Then
        dlgRendering.FillModePoint.Value = True
    ElseIf (ShapeShifterCtl1.Viewport.FillMode = 2) Then
        dlgRendering.FillModeWireFrame.Value = True
    ElseIf (ShapeShifterCtl1.Viewport.FillMode = 3) Then
        dlgRendering.FillModeSolid.Value = True
    End If
    
    Dim RenderingContext As ShapeShifter.RenderingContext
    Set RenderingContext = ShapeShifterCtl1.Model.GetChildByName("RenderingContext")
    
    If (RenderingContext Is Nothing) Then
        dlgRendering.CullModeNone.Enabled = False
        dlgRendering.CullModeCW.Enabled = False
        dlgRendering.CullModeCCW.Enabled = False
    Else
        If (RenderingContext.GetRenderState(22) = 1) Then
            dlgRendering.CullModeNone.Value = True
        ElseIf (RenderingContext.GetRenderState(22) = 2) Then
            dlgRendering.CullModeCW.Value = True
        ElseIf (RenderingContext.GetRenderState(22) = 3) Then
            dlgRendering.CullModeCCW.Value = True
        End If
    End If
    
    dlgRendering.NearPlane.Text = ShapeShifterCtl1.NearPlane
    dlgRendering.FarPlane.Text = ShapeShifterCtl1.FarPlane
    dlgRendering.FOV.Text = ShapeShifterCtl1.FOV
    
    If (ShapeShifterCtl1.Viewport.LightingEnabled) Then
        dlgRendering.Lighting.Value = 1
    Else
        dlgRendering.Lighting.Value = 0
    End If
    
    If (dlgRendering.DoModal(Me) = vbOK) Then
        If (dlgRendering.FillModePoint.Value = True) Then
            ShapeShifterCtl1.Viewport.FillMode = 1
        ElseIf (dlgRendering.FillModeWireFrame.Value = True) Then
            ShapeShifterCtl1.Viewport.FillMode = 2
        ElseIf (dlgRendering.FillModeSolid.Value = True) Then
            ShapeShifterCtl1.Viewport.FillMode = 3
        End If
        
        If Not (RenderingContext Is Nothing) Then
            If (dlgRendering.CullModeNone.Value = True) Then
                RenderingContext.SetRenderState 22, 1
            ElseIf (dlgRendering.CullModeCW.Value = True) Then
                RenderingContext.SetRenderState 22, 2
            ElseIf (dlgRendering.CullModeCCW.Value = True) Then
                RenderingContext.SetRenderState 22, 3
            End If
        End If
        
        ShapeShifterCtl1.NearPlane = Val(dlgRendering.NearPlane.Text)
        ShapeShifterCtl1.FarPlane = Val(dlgRendering.FarPlane.Text)
        ShapeShifterCtl1.FOV = Val(dlgRendering.FOV.Text)
        
        If (dlgRendering.Lighting.Value = 0) Then
            ShapeShifterCtl1.Viewport.LightingEnabled = False
        Else
            ShapeShifterCtl1.Viewport.LightingEnabled = True
        End If
        
        ShapeShifterCtl1.Viewport.Render
        ShapeShifterCtl1.Viewport.RefreshScreen
    End If
    
    Unload dlgRendering
End Sub

Private Sub ViewAxes_Click()
    Dim AxisGuide As IVisual
    Set AxisGuide = ShapeShifterCtl1.Viewport.GetChildByName("AxisGuide")
    If (AxisGuide.Visible) Then
        AxisGuide.Visible = False
        ViewAxes.Checked = False
    Else
        AxisGuide.Visible = True
        ViewAxes.Checked = True
    End If
    ShapeShifterCtl1.Viewport.Render
    ShapeShifterCtl1.Viewport.RefreshScreen
End Sub

Private Sub ViewFront_Click()
    ShapeShifterCtl1.Controller.MoveCamera SSCL_CAMERA_GOTO_ZAXIS, -500
    ShapeShifterCtl1.Controller.Update 0, Nothing
End Sub

Private Sub ViewBack_Click()
    ShapeShifterCtl1.Controller.MoveCamera SSCL_CAMERA_GOTO_ZAXIS, 500
    ShapeShifterCtl1.Controller.Update 0, Nothing
End Sub

Private Sub ViewLeft_Click()
    ShapeShifterCtl1.Controller.MoveCamera SSCL_CAMERA_GOTO_XAXIS, -500
    ShapeShifterCtl1.Controller.Update 0, Nothing
End Sub

Private Sub ViewRight_Click()
    ShapeShifterCtl1.Controller.MoveCamera SSCL_CAMERA_GOTO_XAXIS, 500
    ShapeShifterCtl1.Controller.Update 0, Nothing
End Sub

Private Sub ViewTop_Click()
    ShapeShifterCtl1.Controller.MoveCamera SSCL_CAMERA_GOTO_YAXIS, 500
    ShapeShifterCtl1.Controller.Update 0, Nothing
End Sub

Private Sub ViewBottom_Click()
    ShapeShifterCtl1.Controller.MoveCamera SSCL_CAMERA_GOTO_YAXIS, -500
    ShapeShifterCtl1.Controller.Update 0, Nothing
End Sub


