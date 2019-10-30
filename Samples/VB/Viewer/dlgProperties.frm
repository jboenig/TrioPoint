VERSION 5.00
Begin VB.Form dlgProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Node Properties"
   ClientHeight    =   1725
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5295
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1725
   ScaleWidth      =   5295
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   3750
      TabIndex        =   0
      Top             =   150
      Width           =   1215
   End
   Begin VB.Label NodeName 
      Height          =   225
      Left            =   1710
      TabIndex        =   8
      Top             =   150
      Width           =   1440
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Right Justify
      Caption         =   "Node Name:"
      Height          =   225
      Left            =   300
      TabIndex        =   7
      Top             =   150
      Width           =   1275
   End
   Begin VB.Label VertexCount 
      Height          =   225
      Left            =   1725
      TabIndex        =   6
      Top             =   1365
      Width           =   1440
   End
   Begin VB.Label FaceCount 
      Height          =   225
      Left            =   1740
      TabIndex        =   5
      Top             =   960
      Width           =   1440
   End
   Begin VB.Label PrimitiveCount 
      Height          =   225
      Left            =   1710
      TabIndex        =   4
      Top             =   510
      Width           =   1440
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "Vertex Count:"
      Height          =   225
      Left            =   525
      TabIndex        =   3
      Top             =   1350
      Width           =   1065
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "Face Count:"
      Height          =   225
      Left            =   315
      TabIndex        =   2
      Top             =   930
      Width           =   1275
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Primitive Count:"
      Height          =   225
      Left            =   315
      TabIndex        =   1
      Top             =   510
      Width           =   1275
   End
End
Attribute VB_Name = "dlgProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Dim Status As Integer

Public Sub PutNode(Node As ShapeShifter.INode)
    Dim Shape As ShapeShifter.Shape
    Dim PrimIdx As Integer
    Dim NumFaces As Integer
    Dim NumVertices As Integer
    Set Shape = Node
    NodeName = Node.Name
    PrimitiveCount = Str(Shape.Mesh.GetPrimitiveCount)

    NumFaces = 0
    NumVertices = 0
    For PrimIdx = 0 To PrimitiveCount - 1
        NumFaces = NumFaces + Shape.Mesh.GetPrimitiveFaceCount(PrimIdx)
        NumVertices = NumVertices + Shape.Mesh.GetPrimitiveVertexCount(PrimIdx)
    Next PrimIdx
    
    FaceCount = Str(NumFaces)
    VertexCount = Str(NumVertices)
End Sub

Public Function DoModal(Parent As Form) As Integer
    Me.Show vbModal, Parent
    DoModal = Status
End Function

Private Sub OKButton_Click()
    Me.Hide
    Status = vbOK
End Sub


