// HPFEEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HPFE.h"
#include "HPFEEditDlg.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHPFEEditDlg dialog


CHPFEEditDlg::CHPFEEditDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHPFEEditDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CHPFEEditDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void CHPFEEditDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CHPFEEditDlg)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHPFEEditDlg, CDialog)
//{{AFX_MSG_MAP(CHPFEEditDlg)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHPFEEditDlg message handlers
