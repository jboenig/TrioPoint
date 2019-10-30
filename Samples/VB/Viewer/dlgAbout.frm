VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form dlgAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About ShapeShifter Viewer"
   ClientHeight    =   3255
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5730
   ClipControls    =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2246.659
   ScaleMode       =   0  'User
   ScaleWidth      =   5380.766
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin RichTextLib.RichTextBox RichTextBox1 
      Height          =   1470
      Left            =   540
      TabIndex        =   3
      Top             =   975
      Width           =   4815
      _ExtentX        =   8493
      _ExtentY        =   2593
      _Version        =   393217
      BackColor       =   -2147483633
      ReadOnly        =   -1  'True
      ScrollBars      =   2
      OLEDropMode     =   0
      FileName        =   "D:\TrioPoint\SHAPESHIFTER\Samples\VB\Viewer\Instructions.rtf"
      TextRTF         =   $"dlgAbout.frx":0000
   End
   Begin VB.CommandButton cmdOK 
      Cancel          =   -1  'True
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   345
      Left            =   4305
      TabIndex        =   0
      Top             =   2775
      Width           =   1260
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      BorderStyle     =   6  'Inside Solid
      Index           =   1
      X1              =   84.515
      X2              =   5309.398
      Y1              =   1801.468
      Y2              =   1801.468
   End
   Begin VB.Label lblTitle 
      Alignment       =   2  'Center
      Caption         =   "ShapeShifter Viewer"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   375
      Left            =   60
      TabIndex        =   1
      Top             =   75
      Width           =   5625
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00FFFFFF&
      BorderWidth     =   2
      Index           =   0
      X1              =   112.686
      X2              =   5323.484
      Y1              =   1811.822
      Y2              =   1811.822
   End
   Begin VB.Label lblVersion 
      Alignment       =   2  'Center
      Caption         =   "Instructions"
      Height          =   225
      Left            =   1035
      TabIndex        =   2
      Top             =   660
      Width           =   3885
   End
End
Attribute VB_Name = "dlgAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim Status As Integer

Private Sub cmdOK_Click()
  Status = vbOK
  Unload Me
End Sub

Public Function DoModal(Parent As Form) As Integer
    Me.Show vbModal, Parent
    DoModal = Status
End Function

