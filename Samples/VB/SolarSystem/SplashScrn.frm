VERSION 5.00
Begin VB.Form SplashScrn 
   BackColor       =   &H00C00000&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   1500
   ClientLeft      =   15
   ClientTop       =   15
   ClientWidth     =   4560
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   Moveable        =   0   'False
   NegotiateMenus  =   0   'False
   ScaleHeight     =   1500
   ScaleWidth      =   4560
   StartUpPosition =   2  'CenterScreen
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Solar System Explorer"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FF00&
      Height          =   375
      Left            =   480
      TabIndex        =   1
      Top             =   120
      Width           =   3735
   End
   Begin VB.Label Status 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FF00&
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   960
      Width           =   4335
   End
End
Attribute VB_Name = "SplashScrn"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
