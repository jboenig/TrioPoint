VERSION 5.00
Begin VB.Form dlgAmbientColor 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Ambient Color"
   ClientHeight    =   1650
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4455
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1650
   ScaleWidth      =   4455
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox Blue 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0.00"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   320
      Left            =   1320
      TabIndex        =   6
      Top             =   960
      Width           =   1215
   End
   Begin VB.TextBox Green 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0.00"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   320
      Left            =   1320
      TabIndex        =   4
      Top             =   600
      Width           =   1215
   End
   Begin VB.TextBox Red 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0.00"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   320
      Left            =   1320
      TabIndex        =   2
      Top             =   240
      Width           =   1215
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3000
      TabIndex        =   1
      Top             =   720
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   3000
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "Blue:"
      Height          =   300
      Left            =   360
      TabIndex        =   7
      Top             =   960
      Width           =   735
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "Green:"
      Height          =   300
      Left            =   360
      TabIndex        =   5
      Top             =   600
      Width           =   735
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Red:"
      Height          =   300
      Left            =   360
      TabIndex        =   3
      Top             =   240
      Width           =   735
   End
End
Attribute VB_Name = "dlgAmbientColor"
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
