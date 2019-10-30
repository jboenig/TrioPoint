VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form MainForm 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Maze"
   ClientHeight    =   7170
   ClientLeft      =   -60
   ClientTop       =   510
   ClientWidth     =   9570
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   478
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   638
   StartUpPosition =   2  'CenterScreen
   Begin MSComDlg.CommonDialog dlgTextureFile 
      Left            =   7560
      Top             =   90
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
      DialogTitle     =   "Open Texture"
      FileName        =   "*.bmp"
      Filter          =   "BMP - Windows Bitmap|*.bmp|JPG - JPEG Files|*.jpg|PNG - Portable Network Graphics|*.png|DDS - Direct Draw Surface|*.dds"
      InitDir         =   "..\Textures"
   End
   Begin VB.Menu Texture 
      Caption         =   "Texture"
      Begin VB.Menu TextureFloor 
         Caption         =   "Floor"
      End
      Begin VB.Menu TextureWall 
         Caption         =   "Wall"
      End
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim ShapeShifterEngine As ShapeShifter.Engine
Dim ShapeShifterDevice As ShapeShifter.Device
Dim WndMessageSource As ShapeShifter.WndMessageSource
Dim Viewport As ShapeShifter.Viewport
Dim Model As ShapeShifter.Model
Dim Controller As ShapeShifter.BrowseController
Const MazeSize As Single = 250
Const HalfMazeSize As Single = MazeSize / 2
Const HallWidth As Single = 20
Const WallHeight As Single = 8
Const HalfWallHeight As Single = WallHeight / 2
Const NumLevels As Single = 4
Const NumOuterWallTiles As Single = 16

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    If (KeyCode = vbKeyF12) Then
        ToggleViewport
    End If
    If (KeyCode = vbKeyEscape) Then
        Unload Me
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
    ' Get a device and initialize it
    '
    Set ShapeShifterDevice = ShapeShifterEngine.GetDefaultDevice
    
    '
    ' Create and load the model
    '
    LoadModel
    
    '
    ' Create and initialize the viewport
    '
    InitializeViewport
    
    '
    ' Initialize the device
    '
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
    Dim Floor As ShapeShifter.Shape
    Dim MazeMaterial As ShapeShifter.Material
    Dim FloorTexture As ShapeShifter.TextureStage
    Dim OuterWallMesh As ShapeShifter.Mesh
    Dim CurWall As ShapeShifter.Shape
    Dim CurWallWidth As Single
    Dim CurWallNumTiles As Single
    Dim WallTexture As ShapeShifter.TextureStage
    Dim Light1 As ShapeShifter.DirectionalLight
    Dim N As Integer
    
    '
    ' Create the model
    '
    Set Model = New ShapeShifter.Model
    Model.MeasurementScale.UnitOfMeasure = FEET
    Model.MeasurementScale.Measurement = 1
    
    '
    ' Set the bounds of the model
    '
    Dim bounds As ShapeShifter.BOX3D
    bounds.back = HalfMazeSize
    bounds.front = -HalfMazeSize
    bounds.Left = -HalfMazeSize
    bounds.Right = HalfMazeSize
    bounds.Top = 100
    bounds.bottom = -100
    Model.SetBoundingBox bounds

    '
    ' Create namespaces for various types of objects
    '
    Set Namespace = Model.CreateNode("Meshes", "ShapeShifter.Namespace", "")
    Set Namespace = Model.CreateNode("Textures", "ShapeShifter.Namespace", "")
    Set Namespace = Model.CreateNode("Materials", "ShapeShifter.Namespace", "")
    Set Namespace = Model.CreateNode("Lights", "ShapeShifter.Namespace", "")

    '
    ' Create a mesh editor
    '
    Set MeshEditor = New ShapeShifter.MeshEditor
    
    '
    ' Create the floor
    '
    Set Floor = Model.CreateNode("Floor", "ShapeShifter.CompositeShape", "")
    
    '
    ' Edit the floor mesh
    '
    MeshEditor.BeginEdit Floor.Mesh
    MeshEditor.MakeCube MazeSize, 0, MazeSize

    MeshEditor.UpdateTextureCoordinate 24, 0, 40
    MeshEditor.UpdateTextureCoordinate 25, 0, 0
    MeshEditor.UpdateTextureCoordinate 26, 40, 40
    MeshEditor.UpdateTextureCoordinate 27, 40, 40
    MeshEditor.UpdateTextureCoordinate 28, 0, 0
    MeshEditor.UpdateTextureCoordinate 29, 40, 0

    MeshEditor.EndEdit
    
    '
    ' Create basic white material for textured surfaces
    '
    Set MazeMaterial = Model.CreateNode("Maze Texture", "ShapeShifter.Material", "Materials")
    MazeMaterial.SetDiffuseColor 1#, 1#, 1#, 1#
    MazeMaterial.SetAmbientColor 1#, 1#, 1#, 1#
    MazeMaterial.SetSpecularColor 1#, 1#, 1#, 1#
    MazeMaterial.SetEmissiveColor 1#, 1#, 1#, 1#
    
    '
    ' Add texture to floor
    '
    Set FloorTexture = Model.CreateNode("Floor Texture", "ShapeShifter.TextureStage", "Textures")
    FloorTexture.MagFilter = SSCLTEXF_LINEAR
    FloorTexture.TextureAddressU = SSCLTADDRESS_MIRROR
    FloorTexture.TextureAddressV = SSCLTADDRESS_MIRROR
    FloorTexture.Texture.FileName = "arccleanblue.jpg"
    Floor.GetContext(0).AppendChild MazeMaterial
    Floor.GetContext(0).AppendChild FloorTexture

    '
    ' Create outer walls
    '
    Set OuterWallMesh = Model.CreateNode("Outer Wall", "ShapeShifter.Mesh", "Meshes")
    MeshEditor.BeginEdit OuterWallMesh
    MeshEditor.MakeCube MazeSize, WallHeight, 1
    SetWallTextureCoordinates MeshEditor, NumOuterWallTiles, 1
    MeshEditor.EndEdit
    
    Set WallTexture = Model.CreateNode("Wall Texture", "ShapeShifter.TextureStage", "Textures")
    WallTexture.MagFilter = SSCLTFG_LINEAR
    WallTexture.TextureAddressU = SSCLTADDRESS_MIRROR
    WallTexture.TextureAddressV = SSCLTADDRESS_MIRROR
    WallTexture.Texture.FileName = "reliefgoldmetal.jpg"
    
    Dim RotateDir As ShapeShifter.VECTOR3D
    RotateDir.y = 1
    
    Set CurWall = Model.CreateNode("NorthWall", "ShapeShifter.CompositeShape", "")
    Set CurWall.Mesh = OuterWallMesh
    CurWall.GetContext(0).AppendChild MazeMaterial
    CurWall.GetContext(0).AppendChild WallTexture
    CurWall.Translate 0, HalfWallHeight, HalfMazeSize
    
    Set CurWall = Model.CreateNode("SouthWall", "ShapeShifter.CompositeShape", "")
    Set CurWall.Mesh = OuterWallMesh
    CurWall.GetContext(0).AppendChild MazeMaterial
    CurWall.GetContext(0).AppendChild WallTexture
    CurWall.Translate 0, HalfWallHeight, -HalfMazeSize
    
    Set CurWall = Model.CreateNode("EastWall", "ShapeShifter.CompositeShape", "")
    Set CurWall.Mesh = OuterWallMesh
    CurWall.GetContext(0).AppendChild MazeMaterial
    CurWall.GetContext(0).AppendChild WallTexture
    CurWall.Rotate 1.5708, RotateDir.x, RotateDir.y, RotateDir.z
    CurWall.Translate HalfMazeSize, HalfWallHeight, 0
    
    Set CurWall = Model.CreateNode("WestWall", "ShapeShifter.CompositeShape", "")
    Set CurWall.Mesh = OuterWallMesh
    CurWall.GetContext(0).AppendChild MazeMaterial
    CurWall.GetContext(0).AppendChild WallTexture
    CurWall.Rotate 1.5708, RotateDir.x, RotateDir.y, RotateDir.z
    CurWall.Translate -HalfMazeSize, HalfWallHeight, 0
    
    '
    ' Create maze walls
    '
    Dim LevelNum As Integer
    For LevelNum = 1 To NumLevels Step 1
        ' First wall
        Set CurWall = Model.CreateNode("Level" + Str(LevelNum) + "_Wall_A", "ShapeShifter.CompositeShape", "")
        MeshEditor.BeginEdit CurWall.Mesh
        CurWallWidth = (MazeSize - (((LevelNum * 2) - 1) * HallWidth))
        MeshEditor.MakeCube CurWallWidth, WallHeight, 1
        SetWallTextureCoordinates MeshEditor, CalcWallTileCount(CurWallWidth), 1
        MeshEditor.EndEdit
        CurWall.Rotate 1.5708, RotateDir.x, RotateDir.y, RotateDir.z
        CurWall.Translate (LevelNum * HallWidth) - HalfMazeSize, HalfWallHeight, -(HallWidth / 2)
        CurWall.GetContext(0).AppendChild MazeMaterial
        CurWall.GetContext(0).AppendChild WallTexture
        ' Second wall
        Set CurWall = Model.CreateNode("Level" + Str(LevelNum) + "_Wall_B", "ShapeShifter.CompositeShape", "")
        MeshEditor.BeginEdit CurWall.Mesh
        CurWallWidth = (MazeSize - ((LevelNum * 2) * HallWidth))
        MeshEditor.MakeCube CurWallWidth, WallHeight, 1
        SetWallTextureCoordinates MeshEditor, CalcWallTileCount(CurWallWidth), 1
        MeshEditor.EndEdit
        CurWall.Translate 0, HalfWallHeight, (HalfMazeSize - (HallWidth * LevelNum))
        CurWall.GetContext(0).AppendChild MazeMaterial
        CurWall.GetContext(0).AppendChild WallTexture
        ' Third wall
        Set CurWall = Model.CreateNode("Level" + Str(LevelNum) + "_Wall_C", "ShapeShifter.CompositeShape", "")
        MeshEditor.BeginEdit CurWall.Mesh
        CurWallWidth = (MazeSize - ((LevelNum * 2) * HallWidth))
        MeshEditor.MakeCube CurWallWidth, WallHeight, 1
        SetWallTextureCoordinates MeshEditor, CalcWallTileCount(CurWallWidth), 1
        MeshEditor.EndEdit
        CurWall.Rotate 1.5708, RotateDir.x, RotateDir.y, RotateDir.z
        CurWall.Translate HalfMazeSize - (LevelNum * HallWidth), HalfWallHeight, 0
        CurWall.GetContext(0).AppendChild MazeMaterial
        CurWall.GetContext(0).AppendChild WallTexture
        ' Fourth wall
        Set CurWall = Model.CreateNode("Level" + Str(LevelNum) + "_Wall_D", "ShapeShifter.CompositeShape", "")
        MeshEditor.BeginEdit CurWall.Mesh
        CurWallWidth = (MazeSize - (((LevelNum * 2) + 1) * HallWidth))
        MeshEditor.MakeCube CurWallWidth, WallHeight, 1
        SetWallTextureCoordinates MeshEditor, CalcWallTileCount(CurWallWidth), 1
        MeshEditor.EndEdit
        CurWall.Translate (HallWidth / 2), HalfWallHeight, (LevelNum * HallWidth) - HalfMazeSize
        CurWall.GetContext(0).AppendChild MazeMaterial
        CurWall.GetContext(0).AppendChild WallTexture
    Next LevelNum
        
    '
    ' Add lighting to the model
    '
    Set Light1 = Model.CreateNode("Light1", "ShapeShifter.DirectionalLight", "Lights")
    Light1.LightIndex = 0
    Light1.SetAmbientColor 0.5, 0.5, 0.5
    Light1.SetDiffuseColor 0.5, 0.5, 0.5
    Light1.SetSpecularColor 0.5, 0.5, 0.5
    Light1.SetDirection -0.5, 1, 1
    Model.AppendChild Light1
    
    Set Light2 = Model.CreateNode("Light2", "ShapeShifter.DirectionalLight", "Lights")
    Light2.LightIndex = 1
    Light2.SetAmbientColor 0.2, 0.2, 0.2
    Light2.SetDiffuseColor 0.2, 0.2, 0.2
    Light2.SetSpecularColor 0.2, 0.2, 0.2
    Light2.SetDirection 0#, 0#, -1#
    Model.AppendChild Light2
    
    Model.LoadBSPTree
    Model.BSPTreeEnabled = True
End Sub

Private Sub SetWallTextureCoordinates(MeshEditor As ShapeShifter.MeshEditor, MaxU As Single, MaxV As Single)
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
    ' Setup the viewport to be full-screen
    '
    Viewport.Windowed = False
    Viewport.Left = 0
    Viewport.Top = 0
    Viewport.Width = Screen.Width / Screen.TwipsPerPixelX
    Viewport.Height = Screen.Height / Screen.TwipsPerPixelY
'    Viewport.Windowed = True
'    Viewport.Left = MainForm.ScaleLeft
'    Viewport.Top = MainForm.ScaleTop
'    Viewport.Width = MainForm.ScaleWidth
'    Viewport.Height = MainForm.ScaleHeight

    '
    ' Assign the window handle of the form to the viewport
    '
    Viewport.ContainerWindow = hWnd
    
    '
    ' Set the camera position and direction
    '
    Viewport.Camera.SetLocation 0, 4, 0
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
    Set Controller = New ShapeShifter.BrowseController
    Controller.GetBehavior("MouseSelect").Enabled = False
    Controller.GetBehavior("CameraStep").MoveIncrement = 3
    Set Controller.Viewport = Viewport
    
    '
    ' Add the controller as a receiver of messages
    ' to the message source object
    '
    WndMessageSource.AddReceiver Controller
End Sub

Private Sub ToggleViewport()
    If (Viewport.Windowed) Then
        Viewport.Windowed = False
        Viewport.Left = MainForm.ScaleLeft
        Viewport.Top = MainForm.ScaleTop
        Viewport.Width = MainForm.ScaleWidth
        Viewport.Height = MainForm.ScaleHeight
        MainForm.Visible = False
    Else
        Viewport.Windowed = True
        Viewport.Left = 0
        Viewport.Top = 0
        Viewport.Width = Screen.Width / Screen.TwipsPerPixelX
        Viewport.Height = Screen.Height / Screen.TwipsPerPixelY
        MainForm.Visible = True
    End If
    Model.LoadDevice ShapeShifterDevice
    Viewport.Render
    Viewport.RefreshScreen
End Sub

Private Sub Form_Paint()
    Viewport.RefreshScreen
End Sub

Private Sub Form_Resize()
    Viewport.Left = MainForm.ScaleLeft
    Viewport.Top = MainForm.ScaleTop
    Viewport.Width = MainForm.ScaleWidth
    Viewport.Height = MainForm.ScaleHeight
    Viewport.Render
End Sub

Private Function CalcWallTileCount(WallWidth As Single) As Single
    CalcWallTileCount = Round(NumOuterWallTiles * (WallWidth / MazeSize), 0)
End Function

Private Sub TextureFloor_Click()
    On Error GoTo ExitSub
    dlgTextureFile.ShowOpen
    Set FloorTexture = Model.GetChildByName("Textures.Floor Texture")
    FloorTexture.UnloadDevice ShapeShifterDevice
    FloorTexture.Texture.FileName = dlgTextureFile.FileName
    FloorTexture.LoadDevice ShapeShifterDevice
    Viewport.Render
ExitSub:
End Sub

Private Sub TextureWall_Click()
    On Error GoTo ExitSub
    dlgTextureFile.ShowOpen
    Set WallTexture = Model.GetChildByName("Textures.Wall Texture")
    WallTexture.UnloadDevice ShapeShifterDevice
    WallTexture.Texture.FileName = dlgTextureFile.FileName
    WallTexture.LoadDevice ShapeShifterDevice
    Viewport.Render
ExitSub:
End Sub
