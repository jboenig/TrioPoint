VERSION 5.00
Begin VB.Form MainForm 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Solar System Explorer"
   ClientHeight    =   7200
   ClientLeft      =   5010
   ClientTop       =   4215
   ClientWidth     =   9600
   BeginProperty Font 
      Name            =   "Courier New"
      Size            =   15.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   480
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   640
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame TargetStatistics 
      BackColor       =   &H004A0000&
      BorderStyle     =   0  'None
      ForeColor       =   &H00000000&
      Height          =   645
      Left            =   90
      TabIndex        =   7
      Top             =   6480
      Visible         =   0   'False
      Width           =   4785
      Begin VB.Label TargetDistFromSun 
         BackStyle       =   0  'Transparent
         Caption         =   "Label2"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   2340
         TabIndex        =   11
         Top             =   360
         Width           =   2415
      End
      Begin VB.Label TargetDiameter 
         BackStyle       =   0  'Transparent
         Caption         =   "Label2"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   2340
         TabIndex        =   10
         Top             =   90
         Width           =   2415
      End
      Begin VB.Label Label5 
         BackStyle       =   0  'Transparent
         Caption         =   "Distance From Sun:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   285
         Left            =   90
         TabIndex        =   9
         Top             =   360
         Width           =   2175
      End
      Begin VB.Label Label4 
         BackStyle       =   0  'Transparent
         Caption         =   "Diameter:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   285
         Left            =   1170
         TabIndex        =   8
         Top             =   90
         Width           =   1095
      End
   End
   Begin VB.Frame HUD 
      BackColor       =   &H004A0000&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   975
      Left            =   4860
      TabIndex        =   0
      Top             =   90
      Width           =   4665
      Begin VB.Label VelocityField 
         BackStyle       =   0  'Transparent
         Caption         =   "Label4"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   1440
         TabIndex        =   6
         Top             =   630
         Width           =   3015
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Velocity:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   180
         TabIndex        =   5
         Top             =   600
         Width           =   1215
      End
      Begin VB.Label DistanceField 
         BackStyle       =   0  'Transparent
         Caption         =   "Label3"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   1440
         TabIndex        =   4
         Top             =   360
         Width           =   2775
      End
      Begin VB.Label Label2 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Distance:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   180
         TabIndex        =   3
         Top             =   360
         Width           =   1215
      End
      Begin VB.Label TargetField 
         BackStyle       =   0  'Transparent
         Caption         =   "Label2"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   1440
         TabIndex        =   2
         Top             =   120
         Width           =   2415
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Target:"
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000C000&
         Height          =   255
         Left            =   180
         TabIndex        =   1
         Top             =   120
         Width           =   1215
      End
   End
   Begin VB.Menu TargetMenu 
      Caption         =   "Target"
      Begin VB.Menu Target_Statistics 
         Caption         =   "Statistics"
         Checked         =   -1  'True
      End
      Begin VB.Menu Target_Sun 
         Caption         =   "Sun"
      End
      Begin VB.Menu Target_Mercury 
         Caption         =   "Mercury"
      End
      Begin VB.Menu Target_Venus 
         Caption         =   "Venus"
      End
      Begin VB.Menu Target_Earth 
         Caption         =   "Earth"
      End
      Begin VB.Menu Target_Mars 
         Caption         =   "Mars"
      End
      Begin VB.Menu Target_Jupiter 
         Caption         =   "Jupiter"
      End
      Begin VB.Menu Target_Saturn 
         Caption         =   "Saturn"
      End
      Begin VB.Menu Target_Uranus 
         Caption         =   "Uranus"
      End
      Begin VB.Menu Target_Neptune 
         Caption         =   "Neptune"
      End
      Begin VB.Menu Target_Pluto 
         Caption         =   "Pluto"
      End
   End
   Begin VB.Menu AnimationMenu 
      Caption         =   "Animation"
      Begin VB.Menu Animation_On 
         Caption         =   "On"
      End
   End
   Begin VB.Menu ViewMenu 
      Caption         =   "View"
      Begin VB.Menu View_HUD 
         Caption         =   "HUD"
         Shortcut        =   {F1}
      End
   End
   Begin VB.Menu HelpMenu 
      Caption         =   "Help"
      Begin VB.Menu Help_About 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Type Planet
    Name As String
    DiameterKm As Double
    DistanceKmFromSun As Double
End Type

Dim ShapeShifterEngine As ShapeShifter.Engine
Dim ShapeShifterDevice As ShapeShifter.Device
Dim WndMessageSource As ShapeShifter.WndMessageSource
Dim Viewport As ShapeShifter.Viewport
Dim Model As ShapeShifter.Model
Dim AnimationLoop As ShapeShifter.AnimationLoop
Dim WithEvents Controller As ShapeShifter.FlightController
Attribute Controller.VB_VarHelpID = -1
Dim TargetObj As ShapeShifter.Shape
Dim CurUnits As ShapeShifter.UnitOfMeasure
Dim Velocity_Km_Per_Second(7) As Single
Dim CurVelocity As Integer
Dim DistToTarget As Single
Dim PlanetInfo(9) As Planet

Private Sub Form_Initialize()
    'SAD If we pass an integer on the command line,
    'then only make that many planets. Valid numbers of planets
    'are 1 - 9. Note that the menus are static, and if less than
    'the full amount of planets are created, the app will bomb
    'Always create earth first
    
    PlanetInfo(0).Name = "Earth"
    PlanetInfo(0).DiameterKm = 12753
    PlanetInfo(0).DistanceKmFromSun = 146000000
    
    PlanetInfo(1).Name = "Mercury"
    PlanetInfo(1).DiameterKm = 4878
    PlanetInfo(1).DistanceKmFromSun = 45900000
    
    PlanetInfo(2).Name = "Venus"
    PlanetInfo(2).DiameterKm = 12140
    PlanetInfo(2).DistanceKmFromSun = 108000000
    
    PlanetInfo(3).Name = "Mars"
    PlanetInfo(3).DiameterKm = 6785
    PlanetInfo(3).DistanceKmFromSun = 205000000
    
    PlanetInfo(4).Name = "Jupiter"
    PlanetInfo(4).DiameterKm = 142800
    PlanetInfo(4).DistanceKmFromSun = 483000000
    
    PlanetInfo(5).Name = "Saturn"
    PlanetInfo(5).DiameterKm = 119871
    PlanetInfo(5).DistanceKmFromSun = 1350000000
    
    PlanetInfo(6).Name = "Uranus"
    PlanetInfo(6).DiameterKm = 51488
    PlanetInfo(6).DistanceKmFromSun = 2700000000#
    
    PlanetInfo(7).Name = "Neptune"
    PlanetInfo(7).DiameterKm = 49493
    PlanetInfo(7).DistanceKmFromSun = 4460000000#
    
    PlanetInfo(8).Name = "Pluto"
    PlanetInfo(8).DiameterKm = 2301
    PlanetInfo(8).DistanceKmFromSun = 4340000000#
    
    Velocity_Km_Per_Second(0) = 0
    Velocity_Km_Per_Second(1) = 1000
    Velocity_Km_Per_Second(2) = 10000
    Velocity_Km_Per_Second(3) = 100000
    Velocity_Km_Per_Second(4) = 1000000
    Velocity_Km_Per_Second(5) = 10000000
    Velocity_Km_Per_Second(6) = 100000000
End Sub

Private Sub Form_Load()
    '
    ' Set initial unit of measurement
    '
    CurUnits = KILOMETERS
    
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
    Set ShapeShifterDevice = ShapeShifterEngine.GetDefaultDevice()
    
    '
    ' Create and load the model
    '
    LoadModel
    
    '
    ' Create and initialize the viewport
    '
    InitializeViewport
    
    '
    ' Initialize device
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
    
    SplashScrn.Status.Caption = "Initializing Viewport..."
    SplashScrn.Refresh
    
    '
    ' Select initial target and update UI
    '
    SelectTarget "Earth"
    ShowTargetStatistics False
    UpdateDistanceToTarget
    Target_Earth.Checked = True
    
    '
    ' Select initial speed
    '
    SetVelocity 2
    
    '
    ' Show heads up display
    '
    View_HUD.Checked = True
    HUD.Visible = True
End Sub

Private Sub LoadModel()
    SplashScrn.Status.Caption = "Loading Solar System Model..."
    SplashScrn.Refresh
    
    '
    ' Create the model
    '
    Set Model = New ShapeShifter.Model

    '
    ' Each logical unit in the model is equivalent to 100 kilometers
    '
    Set Model.MeasurementScale = New ShapeShifter.MeasurementScale
    Model.MeasurementScale.UnitOfMeasure = KILOMETERS
    Model.MeasurementScale.Measurement = 100

    LoadSun

    Dim strCmdLine As String, nPlanets As Integer
    strCmdLine = Command()
    nPlanets = 0
    
    If Len(strCmdLine) > 0 And IsNumeric(strCmdLine) Then
        nPlanets = CInt(strCmdLine)
    End If
    
    If nPlanets < 1 Or Planets > 9 Then nPlanets = 9
    
    Dim n As Integer
    For n = 0 To nPlanets - 1
        LoadPlanet PlanetInfo(n).Name, PlanetInfo(n).DiameterKm, PlanetInfo(n).DistanceKmFromSun
    Next
    
    '
    ' Add lighting to the model
    '
    Model.SetAmbientLight 0.1, 0.1, 0.1, 0
End Sub

Private Sub LoadSun()
    Dim Sun As ShapeShifter.Shape
    Dim MeshEditor As ShapeShifter.MeshEditor
    Dim SunMaterial As ShapeShifter.Material
    Dim Light As ShapeShifter.PointLight

    SplashScrn.Status.Caption = "Loading Sun..."
    SplashScrn.Refresh
    
    Set Sun = New ShapeShifter.Shape
    Sun.Name = "Sun"
    Set Sun.Mesh = Model.CreateNode("", "ShapeShifter.Mesh", "")

    Sun.AddProperty "Diameter", 0
    Sun.AddProperty "DistanceFromSun", 0
    
    Set MeshEditor = New ShapeShifter.MeshEditor
    MeshEditor.BeginEdit Sun.Mesh
    MeshEditor.MakeSphere Model.MeasurementScale.ConvertMeasurement(KILOMETERS, 700000), 16, 16
    MeshEditor.EndEdit
    
    Set SunMaterial = New ShapeShifter.Material
    SunMaterial.SetDiffuseColor 1#, 1#, 0.5, 1#
    SunMaterial.SetAmbientColor 1#, 1#, 0.5, 1#
    SunMaterial.SetSpecularColor 1#, 1#, 0.5, 1#
    SunMaterial.SetEmissiveColor 1#, 1#, 0.5, 1#
    Sun.SetContext 0, SunMaterial

    Model.AppendChild Sun
End Sub
 
Private Sub LoadPlanet(PlanetName As String, DiameterKm As Double, DistanceKmFromSun As Double)
    Dim Planet As ShapeShifter.CompositeShape
    Dim MeshEditor As ShapeShifter.MeshEditor
    Dim TextureFilename As String
    Dim PlanetMaterial As ShapeShifter.Material
    Dim PlanetTextureStage As ShapeShifter.TextureStage
    
    SplashScrn.Status.Caption = "Loading " + PlanetName + "..."
    SplashScrn.Refresh
    
    Set Planet = Model.CreateNode(PlanetName, "ShapeShifter.CompositeShape", "")
    
    ' Custom properties
    Planet.AddProperty "Diameter", DiameterKm
    Planet.AddProperty "DistanceFromSun", DistanceKmFromSun
    
    Set MeshEditor = New ShapeShifter.MeshEditor
    MeshEditor.BeginEdit Planet.Mesh
    MeshEditor.MakeSphere Model.MeasurementScale.ConvertMeasurement(KILOMETERS, DiameterKm / 2), 24, 24
    MeshEditor.EndEdit
    
    Set PlanetMaterial = Model.CreateNode(PlanetName + "Material", "ShapeShifter.Material", "")
    PlanetMaterial.SetDiffuseColor 1#, 1#, 1#, 1#
    PlanetMaterial.SetAmbientColor 1#, 1#, 1#, 1#
    PlanetMaterial.SetSpecularColor 1#, 1#, 1#, 1#
    PlanetMaterial.SetEmissiveColor 1#, 1#, 1#, 1#
    
    Set PlanetTextureStage = Model.CreateNode(PlanetName + "TextureStage", "ShapeShifter.TextureStage", "")
    PlanetTextureStage.MagFilter = SSCLTEXF_LINEAR
    On Error GoTo SkipTexture
    TextureFilename = PlanetName + ".dds"
    PlanetTextureStage.Texture.FileName = TextureFilename
       
    Planet.GetContext(0).AppendChild PlanetMaterial
    Planet.GetContext(0).AppendChild PlanetTextureStage
    
SkipTexture:
    
    LoadMoons Planet
    
    Planet.Translate Model.MeasurementScale.ConvertMeasurement(KILOMETERS, DistanceKmFromSun), 0, 0
End Sub

Private Sub LoadMoons(Planet As CompositeShape)
    If (Planet.Name = "Earth") Then
        LoadMoon Planet, "Luna", 1737.4, 384400
    ElseIf (Planet.Name = "Jupiter") Then
        LoadMoon Planet, "Callisto", 4806, 1883000
        LoadMoon Planet, "Europa", 3138, 670900
        LoadMoon Planet, "Ganymede", 5268, 1070000
        LoadMoon Planet, "Io", 3630, 421600
    ElseIf (Planet.Name = "Mars") Then
        LoadMoon Planet, "Deimos", 7.5, 23460
        LoadMoon Planet, "Phobos", 22.2, 9378
    End If
End Sub

Private Sub LoadMoon(Planet As CompositeShape, MoonName As String, DiameterKm As Double, DistanceKmFromPlanet As Double)
    Dim Moon As ShapeShifter.Shape
    Dim MeshEditor As ShapeShifter.MeshEditor
    Dim TextureFilename As String
    Dim MoonMaterial As ShapeShifter.Material
    Dim MoonTextureStage As ShapeShifter.TextureStage
    
    Set Moon = Model.CreateNode(MoonName, "ShapeShifter.CompositeShape", "")
    
    Set MeshEditor = New ShapeShifter.MeshEditor
    MeshEditor.BeginEdit Moon.Mesh
    MeshEditor.MakeSphere Model.MeasurementScale.ConvertMeasurement(KILOMETERS, DiameterKm / 2), 16, 16
    MeshEditor.EndEdit
    
    Set MoonMaterial = Model.CreateNode(MoonName + "Material", "ShapeShifter.Material", "")
    MoonMaterial.SetDiffuseColor 1#, 1#, 1#, 1#
    MoonMaterial.SetAmbientColor 1#, 1#, 1#, 1#
    MoonMaterial.SetSpecularColor 1#, 1#, 1#, 1#
    MoonMaterial.SetEmissiveColor 1#, 1#, 1#, 1#

    Moon.GetContext(0).AppendChild MoonMaterial
    
    If (MoonName <> "Luna") Then GoTo SkipTexture
    Set MoonTextureStage = Model.CreateNode(MoonName + "TextureStage", "ShapeShifter.TextureStage", "")
    MoonTextureStage.MagFilter = SSCLTEXF_LINEAR
    On Error GoTo SkipTexture
    TextureFilename = MoonName + ".dds"
    MoonTextureStage.Texture.FileName = TextureFilename
    
    Moon.GetContext(0).AppendChild MoonTextureStage
    
SkipTexture:

    Moon.Translate Model.MeasurementScale.ConvertMeasurement(KILOMETERS, DistanceKmFromPlanet), 0, 0
    
    Planet.AppendChild Moon
End Sub

Private Sub InitializeViewport()
    '
    ' Create viewport
    '
    Set Viewport = ShapeShifterDevice.CreateViewport(Nil)
    
    '
    ' Set the viewport bounds so that it occupies
    ' the entire client area of the form
    '
    Viewport.Left = MainForm.ScaleLeft
    Viewport.Top = MainForm.ScaleTop
    Viewport.Width = MainForm.ScaleWidth
    Viewport.Height = MainForm.ScaleHeight
    
    '
    ' Assign the window handle of the form to the viewport
    '
    Viewport.ContainerWindow = hWnd
    
    '
    ' Set the camera position and direction
    '
    Viewport.Camera.SetLocation Model.MeasurementScale.ConvertMeasurement(KILOMETERS, 145990000), 0, 0
    
    '
    ' Set the viewing volume
    '
    Viewport.SetFrustum 1.57, MainForm.ScaleWidth / MainForm.ScaleHeight, 0.5, 1000000000
    
    '
    ' Set the background color
    '
    Viewport.SetBackgroundColor 0, 0, 0.3, 0
    
    '
    ' Attach the model to the viewport
    '
    Set Viewport.Model = Model
            
    '
    ' Create a controller and attach the viewport to it
    '
    Set Controller = New ShapeShifter.FlightController
    Controller.AttachInputDevice ShapeShifterEngine.GetInputDevice(SSCL_KEYBOARD, 0)
    Controller.CollisionsEnabled = False
    Set Controller.Viewport = Viewport
    
    Set AnimationLoop = New ShapeShifter.AnimationLoop
    AnimationLoop.Interval = 50
    AnimationLoop.AddAnimator Controller
    
    Controller.GetBehavior("CameraFly").VelocityDistance = Model.MeasurementScale.ConvertMeasurement(KILOMETERS, 1000000)
    Controller.GetBehavior("CameraFly").VelocityTime = 1000

    '
    ' Add the controller as a receiver of messages
    ' to the message source object
    '
    WndMessageSource.AddReceiver Controller
End Sub

Private Sub SelectTarget(TargetName As String)
    Set TargetObj = Model.GetChildByName(TargetName)
    TargetField.Caption = TargetObj.Name
    TargetDiameter.Caption = Format(TargetObj.Diameter, "#,###,###,##0.00")
    TargetDistFromSun.Caption = Format(TargetObj.DistanceFromSun, "#,###,###,##0.00")
    Viewport.Camera.PointAt TargetObj.OriginX, TargetObj.OriginY, TargetObj.OriginZ
    Viewport.Invalidate
End Sub

Private Sub ShowTargetStatistics(Flag As Boolean)
    TargetStatistics.Visible = Flag
    Target_Statistics.Checked = Flag
End Sub

Private Sub UpdateDistanceToTarget()
    DistToTarget = Viewport.DistanceToPoint(TargetObj.OriginX, TargetObj.OriginY, TargetObj.OriginZ)
    If (CurUnits = KILOMETERS) Then
        DistanceField.Caption = Format(Model.MeasurementScale.ConvertLogicalUnits(DistToTarget, KILOMETERS), "#,###,###,##0.0")
    End If
End Sub

Private Sub SetVelocity(NewVelocity As Integer)
    Dim VDist As Single
    Dim VTime As Integer
    Dim VSuffix As String
    
    If (NewVelocity >= 0 And NewVelocity <= 6) Then
        CurVelocity = NewVelocity
    
        If (CurUnits = KILOMETERS) Then
            VDist = Velocity_Km_Per_Second(CurVelocity)
            VTime = 1000
            VSuffix = "Km/Second"
        End If
    
        Controller.GetBehavior("CameraFly").VelocityDistance = Model.MeasurementScale.ConvertMeasurement(KILOMETERS, VDist)
        Controller.GetBehavior("CameraFly").VelocityTime = VTime
        VelocityField.Caption = Format(VDist, "##,###,###,##0.0")
        VelocityField.Caption = VelocityField.Caption + " " + VSuffix
    End If
    
End Sub

Private Sub IncreaseVelocity()
    SetVelocity (CurVelocity + 1)
End Sub

Private Sub DecreaseVelocity()
    SetVelocity (CurVelocity - 1)
End Sub

Private Sub Target_Statistics_Click()
    If (Target_Statistics.Checked) Then
        ShowTargetStatistics False
    Else
        ShowTargetStatistics True
    End If
End Sub

Private Sub Target_Sun_Click()
    ClearTargetMenuChecks
    SelectTarget "Sun"
    UpdateDistanceToTarget
    Target_Sun.Checked = True
End Sub

Private Sub Target_Mercury_Click()
    ClearTargetMenuChecks
    SelectTarget "Mercury"
    UpdateDistanceToTarget
    Target_Mercury.Checked = True
End Sub

Private Sub Target_Venus_Click()
    ClearTargetMenuChecks
    SelectTarget "Venus"
    UpdateDistanceToTarget
    Target_Venus.Checked = True
End Sub

Private Sub Target_Earth_Click()
    ClearTargetMenuChecks
    SelectTarget "Earth"
    UpdateDistanceToTarget
    Target_Earth.Checked = True
End Sub

Private Sub Target_Mars_Click()
    ClearTargetMenuChecks
    SelectTarget "Mars"
    UpdateDistanceToTarget
    Target_Mars.Checked = True
End Sub

Private Sub Target_Jupiter_Click()
    ClearTargetMenuChecks
    SelectTarget "Jupiter"
    UpdateDistanceToTarget
    Target_Jupiter.Checked = True
End Sub

Private Sub Target_Saturn_Click()
    ClearTargetMenuChecks
    SelectTarget "Saturn"
    UpdateDistanceToTarget
    Target_Saturn.Checked = True
End Sub

Private Sub Target_Uranus_Click()
    ClearTargetMenuChecks
    SelectTarget "Uranus"
    UpdateDistanceToTarget
    Target_Uranus.Checked = True
End Sub

Private Sub Target_Neptune_Click()
    ClearTargetMenuChecks
    SelectTarget "Neptune"
    UpdateDistanceToTarget
    Target_Neptune.Checked = True
End Sub

Private Sub Target_Pluto_Click()
    ClearTargetMenuChecks
    SelectTarget "Pluto"
    UpdateDistanceToTarget
    Target_Pluto.Checked = True
End Sub

Private Sub ClearTargetMenuChecks()
    Target_Sun.Checked = False
    Target_Mercury.Checked = False
    Target_Venus.Checked = False
    Target_Earth.Checked = False
    Target_Mars.Checked = False
    Target_Jupiter.Checked = False
    Target_Saturn.Checked = False
    Target_Uranus.Checked = False
    Target_Neptune.Checked = False
    Target_Pluto.Checked = False
End Sub

Private Sub ClearMeasurementMenuChecks()
    Measurements_Kilometers.Checked = False
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)

    If (KeyCode = vbKeyAdd) Then
        IncreaseVelocity
    End If

    If (KeyCode = vbKeySubtract) Then
        DecreaseVelocity
    End If

    If (KeyCode = vbKeyNumpad0) Then
        SetVelocity 0
    End If
    
End Sub

Private Sub Animation_On_Click()
    If (AnimationLoop.Running) Then
        AnimationLoop.Stop
        Animation_On.Checked = False
    Else
        AnimationLoop.Start
        Animation_On.Checked = True
    End If
End Sub

Private Sub View_HUD_Click()
    If (HUD.Visible = True) Then
        HUD.Visible = False
        View_HUD.Checked = False
    Else
        HUD.Visible = True
        View_HUD.Checked = True
    End If
End Sub

Private Sub Controller_OnAnimate(ByVal Time As Long, ByVal FirstFrame As Boolean)
    UpdateDistanceToTarget
End Sub

Private Sub Controller_OnSelectionChange(ByVal SelectionList As ShapeShifter.INodes)
    If (SelectionList.Count > 0) Then
        SelectTarget SelectionList(1).Name
    End If
End Sub

Private Sub Help_About_Click()
    frmAbout.Show
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

