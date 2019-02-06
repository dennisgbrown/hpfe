#if !defined(AFX_HPFEEDITDLG_H__A7FE1782_C045_11D3_A16C_0020182CF9F1__INCLUDED_)
#define AFX_HPFEEDITDLG_H__A7FE1782_C045_11D3_A16C_0020182CF9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HPFEEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHPFEEditDlg dialog

class CHPFEEditDlg : public CDialog
{
  // Construction
public:
  CHPFEEditDlg(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CHPFEEditDlg)
  enum
  {
    IDD = IDD_EDIT_DIALOG
  };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHPFEEditDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CHPFEEditDlg)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HPFEEDITDLG_H__A7FE1782_C045_11D3_A16C_0020182CF9F1__INCLUDED_)
