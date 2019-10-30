VERSION 5.00
Begin VB.Form dlgNavigation 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Navigation"
   ClientHeight    =   1350
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4740
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1350
   ScaleWidth      =   4740
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox MoveIncrement 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0.00"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   350
      Left            =   1680
      TabIndex        =   2
      Top             =   360
      Width           =   1215
   End
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3210
      TabIndex        =   1
      Top             =   600
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   3210
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Move Increment:"
      Height          =   225
      Left            =   360
      TabIndex        =   3
      Top             =   420
      Width           =   1215
   End
End
Attribute VB_Name = "dlgNavigation"
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

