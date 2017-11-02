VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "VxComm Import Config File Demo"
   ClientHeight    =   1824
   ClientLeft      =   48
   ClientTop       =   336
   ClientWidth     =   5532
   LinkTopic       =   "Form1"
   ScaleHeight     =   1824
   ScaleWidth      =   5532
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnImport 
      Caption         =   "Import"
      Height          =   372
      Left            =   3840
      TabIndex        =   8
      Top             =   1200
      Width           =   1452
   End
   Begin VB.TextBox tReboot 
      BackColor       =   &H0080FFFF&
      Enabled         =   0   'False
      Height          =   288
      Left            =   4440
      TabIndex        =   7
      Text            =   "0"
      Top             =   720
      Width           =   852
   End
   Begin VB.TextBox tErrorLine 
      BackColor       =   &H0080FFFF&
      Enabled         =   0   'False
      Height          =   288
      Left            =   1560
      TabIndex        =   6
      Text            =   "0"
      Top             =   720
      Width           =   852
   End
   Begin VB.TextBox tReturn 
      BackColor       =   &H0080FFFF&
      Enabled         =   0   'False
      Height          =   288
      Left            =   1560
      TabIndex        =   5
      Text            =   "0"
      Top             =   1080
      Width           =   852
   End
   Begin VB.TextBox tConfigFile 
      BackColor       =   &H0000C000&
      Height          =   288
      Left            =   1560
      TabIndex        =   1
      Text            =   "c:\Sample.Cfg"
      Top             =   240
      Width           =   3732
   End
   Begin VB.Label Label4 
      Caption         =   "Required To Reboot : "
      Height          =   252
      Left            =   2640
      TabIndex        =   4
      Top             =   720
      Width           =   1692
   End
   Begin VB.Label Label3 
      Caption         =   "Last Error Line : "
      Height          =   252
      Left            =   240
      TabIndex        =   3
      Top             =   720
      Width           =   1092
   End
   Begin VB.Label Label2 
      Caption         =   "Return Code : "
      Height          =   252
      Left            =   240
      TabIndex        =   2
      Top             =   1080
      Width           =   1092
   End
   Begin VB.Label Label1 
      Caption         =   "Config File : "
      Height          =   252
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1092
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub btnImport_Click()
    Dim iReturn As Integer, iErrorLine As Integer, iReboot As Integer
    Dim szFile As String
    
    szFile = tConfigFile.Text
    iReturn = VxC_ImportConfigFile(szFile, iErrorLine, iReboot)
    
    tReturn.Text = Str(iReturn)
    tErrorLine.Text = Str(iErrorLine)
    tReboot.Text = Str(iReboot)
    
    If 0 <> iReturn Then
        MsgBox "Something Error!"
    Else
        MsgBox "Import OK!"
    End If
    
End Sub

