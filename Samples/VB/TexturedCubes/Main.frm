VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "Comdlg32.ocx"
Begin VB.Form Main 
   Caption         =   "ShapeShifter Sample - Textured Cubes"
   ClientHeight    =   7125
   ClientLeft      =   8415
   ClientTop       =   6120
   ClientWidth     =   7560
   LinkTopic       =   "Form1"
   ScaleHeight     =   475
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   504
   StartUpPosition =   2  'CenterScreen
   Begin MSComDlg.CommonDialog dlgTextureFile 
      Left            =   6930
      Top             =   630
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
      DialogTitle     =   "Open Texture"
      FileName        =   "*.bmp"
      Filter          =   "BMP - Windows Bitmap|*.bmp|JPG - JPEG Files|*.jpg|PNG - Portable Network Graphics|*.png|DDS - Direct Draw Surface|*.dds"
      InitDir         =   "..\Textures"
   End
   Begin MSComDlg.CommonDialog DlgXMLFile 
      Left            =   6930
      Top             =   90
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      FileName        =   "*.xml"
      Filter          =   "XML files|*.xml"
   End
   Begin VB.Menu File 
      Caption         =   "File"
      Begin VB.Menu OpenTexture 
         Caption         =   "Open Texture"
      End
      Begin VB.Menu FileSave 
         Caption         =   "Save"
      End
   End
End
Attribute VB_Name = "Main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim ShapeShifterEngine As ShapeShifter.Engine
Dim ShapeShifterDevice As ShapeShifter.Device
Dim WndMessageSource As ShapeShifter.WndMessageSource
Dim Viewport As ShapeShifter.Viewport
Dim Model As ShapeShifter.Model
Dim WithEvents Controller As ShapeShifter.BrowseController
Attribute Controller.VB_VarHelpID = -1

Private Sub Controller_OnNodeDblClick(ByVal Node As ShapeShifter.INode)
    MsgBox Node.Name + " double clicked"
End Sub

Private Sub FileSave_Click()
    DlgXMLFile.ShowSave
    
    Screen.MousePointer = vbHourglass
    
    Dim WriterXML As ShapeShifter.ModelWriterXML
    Set WriterXML = New ShapeShifter.ModelWriterXML
    WriterXML.Write DlgXMLFile.FileName, Model

    Screen.MousePointer = vbNormal
End Sub

Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
    If (KeyCode = vbKeyF1) Then
        Controller.GetBehavior("DragDrop").BeginMove
    End If
    
    If (KeyCode = vbKeyF2) Then
        Controller.GetBehavior("DragDrop").EndMove
    End If
End Sub

Private Sub Form_Load()
    '
    ' Connect the form to message source object. This subclasses
    ' the window, intercepts all messages sent to the window, and
    ' forwards them to any ShapeShifter controllers that are
    ' attached.
    '
    Set WndMessageSource = New ShapeShifter.WndMessageSource
    WndMessageSource.Connect hWnd

    '
    ' Create and initialize the ShapeShifter engine
    '
    Set ShapeShifterEngine = New ShapeShifter.Engine
    ShapeShifterEngine.InitializeEngine

    '
    ' Create a device and initialize it
    '
    Set ShapeShifterDevice = ShapeShifterEngine.GetDefaultDevice()
    
    '
    ' Create and load the model
    '
    LoadModel
    
    '
    ' Create and initialize the viewport
    '
    InitializeViewport
    
    ShapeShifterDevice.Initialize hWnd

    '
    ' Load device resources in the model onto the device
    ' before rendering
    '
    Model.LoadDevice ShapeShifterDevice
    
    '
    ' Render the opening frame
    '
    Viewport.Render
End Sub

Private Sub LoadModel()
    Dim Namespace As ShapeShifter.Namespace
    Dim MeshEditor As ShapeShifter.MeshEditor
    Dim CubeMesh As ShapeShifter.Mesh
    Dim CubeShape As ShapeShifter.Shape
    Dim CubeMaterial As ShapeShifter.Material
    Dim TextureStage As ShapeShifter.TextureStage
    Dim RenderingContext As ShapeShifter.RenderingContext
    Dim Light1 As ShapeShifter.DirectionalLight
    Dim N As Integer
    
    '
    ' Create the model
    '
    Set Model = New ShapeShifter.Model
    Model.BSPTreeEnabled = True
    
    '
    ' Create namespaces for various types of objects
    '
    Set Namespace = Model.CreateNode("Meshes", "ShapeShifter.Namespace", "")
    Set Namespace = Model.CreateNode("Context Objects", "ShapeShifter.Namespace", "")

    '
    ' Create a mesh editor
    '
    Set MeshEditor = New ShapeShifter.MeshEditor
    
    '
    ' Create a mesh and configure it as a cube
    '
    Set CubeMesh = Model.CreateNode("", "ShapeShifter.Mesh", "Meshes")
    MeshEditor.BeginEdit CubeMesh
    MeshEditor.MakeCube 3, 3, 3
    SetCubeTextureCoordinates MeshEditor, 1, 1
    MeshEditor.EndEdit
        
    '
    ' Create and initialize a material
    '
    Set CubeMaterial = Model.CreateNode("Cube Material", "ShapeShifter.Material", "Context Objects")
    CubeMaterial.SetDiffuseColor 1#, 1#, 1#, 1#
    CubeMaterial.SetAmbientColor 1#, 1#, 1#, 1#
    CubeMaterial.SetSpecularColor 1#, 1#, 1#, 1#
    CubeMaterial.SetEmissiveColor 1#, 1#, 1#, 1#
    
    '
    ' Create and initialize a texture stage
    '
    Set TextureStage = Model.CreateNode("Cube Texture", "ShapeShifter.TextureStage", "Context Objects")
    TextureStage.MagFilter = SSCLTEXF_LINEAR
    TextureStage.Texture.FileName = "brkgrnredstain.jpg"
    
    '
    ' Create a rendering context
    '
    Set RenderingContext = Model.CreateNode("Cube Context", "ShapeShifter.RenderingContext", "Context Objects")
    RenderingContext.AppendChild CubeMaterial
    RenderingContext.AppendChild TextureStage
    
    '
    ' Create first shape
    '
    Set CubeShape = Model.CreateNode("Center Cube", "ShapeShifter.CompositeShape", "")
    Set CubeShape.Mesh = CubeMesh
    CubeShape.SetContext 0, RenderingContext
    CubeShape.SetStyleFlags 1, False
    CubeShape.SetStyleFlags 2, False
    
    '
    ' Create shapes on left side of the center cube
    '
    For N = 1 To 4 Step 1
        Set CubeShape = Model.CreateNode("Left Cube " + FormatNumber(N, 0), "ShapeShifter.Shape", "")
        Set CubeShape.Mesh = CubeMesh
        CubeShape.Translate -N * 10, 0, 0
        CubeShape.SetContext 0, RenderingContext
    Next
    
    '
    ' Create shapes on right side of the center cube
    '
    For N = 1 To 4 Step 1
        Set CubeShape = Model.CreateNode("Right Cube " + FormatNumber(N, 0), "ShapeShifter.Shape", "")
        Set CubeShape.Mesh = CubeMesh
        CubeShape.Translate N * 10, 0, 0
        CubeShape.SetContext 0, RenderingContext
    Next
    
    Set RenderingContext = Nothing
    Set CubeShape = Nothing
    
    '
    ' Add lighting to the model
    '
    Set Light1 = Model.CreateNode("Light1", "ShapeShifter.DirectionalLight", "")
    Light1.LightIndex = 0
    Light1.SetAmbientColor 0.5, 0.5, 0.5
    Light1.SetDiffuseColor 0.5, 0.5, 0.5
    Light1.SetSpecularColor 0.5, 0.5, 0.5
    Light1.SetDirection -0.5, 1, 1
    
    Set Light2 = Model.CreateNode("Light2", "ShapeShifter.DirectionalLight", "")
    Light2.LightIndex = 1
    Light2.SetAmbientColor 0.2, 0.2, 0.2
    Light2.SetDiffuseColor 0.2, 0.2, 0.2
    Light2.SetSpecularColor 0.2, 0.2, 0.2
    Light2.SetDirection 0#, 0#, -1#
End Sub

Private Sub SetCubeTextureCoordinates(MeshEditor As ShapeShifter.MeshEditor, MaxU As Single, MaxV As Single)
    MeshEditor.UpdateTextureCoordinate 0, 0, MaxV
    MeshEditor.UpdateTextureCoordinate 1, 0, 0
    MeshEditor.UpdateTextureCoordinate 2, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 3, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 4, 0, 0
    MeshEditor.UpdateTextureCoordinate 5, MaxU, 0

    MeshEditor.UpdateTextureCoordinate 6, 0, MaxV
    MeshEditor.UpdateTextureCoordinate 7, 0, 0
    MeshEditor.UpdateTextureCoordinate 8, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 9, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 10, 0, 0
    MeshEditor.UpdateTextureCoordinate 11, MaxU, 0

    MeshEditor.UpdateTextureCoordinate 12, 0, MaxV
    MeshEditor.UpdateTextureCoordinate 13, 0, 0
    MeshEditor.UpdateTextureCoordinate 14, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 15, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 16, 0, 0
    MeshEditor.UpdateTextureCoordinate 17, MaxU, 0
    
    MeshEditor.UpdateTextureCoordinate 18, 0, MaxV
    MeshEditor.UpdateTextureCoordinate 19, 0, 0
    MeshEditor.UpdateTextureCoordinate 20, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 21, MaxU, MaxV
    MeshEditor.UpdateTextureCoordinate 22, 0, 0
    MeshEditor.UpdateTextureCoordinate 23, MaxU, 0
End Sub

Private Sub InitializeViewport()
    '
    ' Create viewport
    '
    Set Viewport = ShapeShifterDevice.CreateViewport("ShapeShifter.Viewport")
    
    '
    ' Set the viewport bounds so that it occupies
    ' the entire client area of the form
    '
    Viewport.Left = Main.ScaleLeft
    Viewport.Top = Main.ScaleTop
    Viewport.Width = Main.ScaleWidth
    Viewport.Height = Main.ScaleHeight
    
    '
    ' Assign the window handle of the form to the viewport
    '
    Viewport.ContainerWindow = hWnd
    
    '
    ' Set the camera position and direction
    '
    Viewport.Camera.SetLocation 0, 0, -8
    Viewport.Camera.SetAimVector 0, 0, 1
    
    '
    ' Set the viewing volume
    '
    Viewport.SetFrustum 1.57, 1, 1, 1000
    
    ' Enable Z buffering
    Viewport.ZEnable = 1
    
    '
    ' Attach the model to the viewport
    '
    Set Viewport.Model = Model
            
    '
    ' Create a controller and attach the viewport to it
    '
    Set Controller = New ShapeShifter.BrowseEditController
    Controller.GetBehavior("MouseSelect").MultiSelect = True
    Set Controller.Viewport = Viewport
    
    '
    ' Add the controller as a receiver of messages
    ' to the message source object
    '
    WndMessageSource.AddReceiver Controller
End Sub

Private Sub Form_Paint()
    Viewport.RefreshScreen
End Sub

Private Sub Form_Resize()
'    Viewport.Left = Main.ScaleLeft
'    Viewport.Top = Main.ScaleTop
'    Viewport.Width = Main.ScaleWidth
'    Viewport.Height = Main.ScaleHeight
'    Viewport.Render
End Sub

Private Sub OpenTexture_Click()
    On Error GoTo ExitSub
    dlgTextureFile.ShowOpen
    Set CubeTexture = Model.GetChildByName("Context Objects.Cube Texture.Texture")
    CubeTexture.UnloadDevice ShapeShifterDevice
    CubeTexture.FileName = dlgTextureFile.FileName
    CubeTexture.LoadDevice ShapeShifterDevice
    Viewport.Render
ExitSub:
End Sub
