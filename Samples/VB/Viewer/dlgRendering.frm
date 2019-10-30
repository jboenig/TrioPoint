VERSION 5.00
Begin VB.Form dlgRendering 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Rendering Options"
   ClientHeight    =   2940
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6810
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2940
   ScaleWidth      =   6810
   ShowInTaskbar   =   0   'False
   Begin VB.CheckBox Lighting 
      Caption         =   "Lighting"
      Height          =   195
      Left            =   975
      TabIndex        =   17
      Top             =   1905
      Width           =   1260
   End
   Begin VB.Frame Frame3 
      Caption         =   "Frustum"
      Height          =   1320
      Left            =   135
      TabIndex        =   10
      Top             =   165
      Width           =   2535
      Begin VB.TextBox FOV 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   285
         Left            =   1335
         TabIndex        =   15
         Top             =   900
         Width           =   960
      End
      Begin VB.TextBox FarPlane 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   285
         Left            =   1335
         TabIndex        =   13
         Top             =   555
         Width           =   960
      End
      Begin VB.TextBox NearPlane 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   285
         Left            =   1335
         TabIndex        =   11
         Top             =   210
         Width           =   960
      End
      Begin VB.Label Label2 
         Alignment       =   1  'Right Justify
         Caption         =   "Field of view:"
         Height          =   225
         Left            =   180
         TabIndex        =   16
         Top             =   900
         Width           =   1020
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Caption         =   "Far plane:"
         Height          =   240
         Left            =   210
         TabIndex        =   14
         Top             =   570
         Width           =   975
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         Caption         =   "Near plane:"
         Height          =   195
         Left            =   180
         TabIndex        =   12
         Top             =   270
         Width           =   1020
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Cull Mode"
      Height          =   1155
      Left            =   2940
      TabIndex        =   5
      Top             =   1470
      Width           =   2175
      Begin VB.OptionButton CullModeCW 
         Caption         =   "Clockwise"
         Height          =   240
         Left            =   360
         TabIndex        =   8
         Top             =   840
         Width           =   1290
      End
      Begin VB.OptionButton CullModeCCW 
         Caption         =   "Counter Clockwise"
         Height          =   240
         Left            =   360
         TabIndex        =   7
         Top             =   525
         Width           =   1665
      End
      Begin VB.OptionButton CullModeNone 
         Caption         =   "None"
         Height          =   195
         Left            =   360
         TabIndex        =   6
         Top             =   240
         Width           =   1665
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Fill Mode"
      Height          =   1155
      Left            =   2940
      TabIndex        =   1
      Top             =   165
      Width           =   2175
      Begin VB.OptionButton FillModePoint 
         Caption         =   "Point"
         Height          =   195
         Left            =   360
         TabIndex        =   4
         Top             =   840
         Width           =   1215
      End
      Begin VB.OptionButton FillModeWireFrame 
         Caption         =   "Wire Frame"
         Height          =   195
         Left            =   360
         TabIndex        =   3
         Top             =   540
         Width           =   1140
      End
      Begin VB.OptionButton FillModeSolid 
         Caption         =   "Solid"
         Height          =   195
         Left            =   360
         TabIndex        =   2
         Top             =   240
         Width           =   990
      End
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   5445
      TabIndex        =   9
      Top             =   705
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   5445
      TabIndex        =   0
      Top             =   225
      Width           =   1215
   End
End
Attribute VB_Name = "dlgRendering"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim Status As Integer

Public Function DoModal(Parent As Form) As Integer
    Me.Show vbModal, Parent
    DoModal = Status
End Function

Private Sub CancelButton_Click()
    Me.Hide
    Status = vbCancel
End Sub

Private Sub OKButton_Click()
    Me.Hide
    Status = vbOK
End Sub


