; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHPFEDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HPFE.h"

ClassCount=4
Class1=CHPFEApp
Class2=CHPFEDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_HPFE_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_EDIT_DIALOG
Resource4=IDD_ABOUTBOX
Class4=CHPFEEditDlg
Resource5=IDR_MENU1

[CLS:CHPFEApp]
Type=0
HeaderFile=HPFE.h
ImplementationFile=HPFE.cpp
Filter=N
LastObject=CHPFEApp

[CLS:CHPFEDlg]
Type=0
HeaderFile=HPFEDlg.h
ImplementationFile=HPFEDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CHPFEDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=HPFEDlg.h
ImplementationFile=HPFEDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_HPFE_DIALOG]
Type=1
Class=CHPFEDlg
ControlCount=2
Control1=IDC_LIST1,SysListView32,1350631433
Control2=IDC_EDIT_BUTTON,button,1342242816

[MNU:IDR_MENU1]
Type=1
Class=CHPFEDlg
Command1=ID_FILE_EXIT
Command2=ID_HELP_ABOUT
CommandCount=2

[DLG:IDD_EDIT_DIALOG]
Type=1
Class=CHPFEEditDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CHPFEEditDlg]
Type=0
HeaderFile=HPFEEditDlg.h
ImplementationFile=HPFEEditDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDCANCEL

