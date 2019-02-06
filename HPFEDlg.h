// HPFEDlg.h : header file
//

#if !defined(AFX_HPFEDLG_H__4CCA4D86_A75D_11D3_A16C_0020182CF9F1__INCLUDED_)
#define AFX_HPFEDLG_H__4CCA4D86_A75D_11D3_A16C_0020182CF9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// My added stuff...
#include "sharedVars.h"
#include "HPFEData.h"

/////////////////////////////////////////////////////////////////////////////
// CHPFEDlg dialog

class CHPFEDlg : public CDialog
{
  // Construction
public:
  CHPFEDlg(CWnd* pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CHPFEDlg)
  enum
  {
    IDD = IDD_HPFE_DIALOG
  };
  CButton EDIT_BUTTON;
  CListCtrl ROMLIST;
  //}}AFX_DATA

  // My added stuff...
  HPFEData* mainData;  // main piece of data for the front end

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHPFEDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  HICON m_hIcon;

  // Generated message map functions
  //{{AFX_MSG(CHPFEDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnHelpAbout();
  afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEditButton();
  afx_msg void OnFileExit();
  afx_msg void OnClose();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HPFEDLG_H__4CCA4D86_A75D_11D3_A16C_0020182CF9F1__INCLUDED_)
