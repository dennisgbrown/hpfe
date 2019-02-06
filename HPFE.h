// HPFE.h : main header file for the HPFE application
//

#if !defined(AFX_HPFE_H__4CCA4D84_A75D_11D3_A16C_0020182CF9F1__INCLUDED_)
#define AFX_HPFE_H__4CCA4D84_A75D_11D3_A16C_0020182CF9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHPFEApp:
// See HPFE.cpp for the implementation of this class
//

class CHPFEApp : public CWinApp
{
public:
  CHPFEApp();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHPFEApp)
public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CHPFEApp)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HPFE_H__4CCA4D84_A75D_11D3_A16C_0020182CF9F1__INCLUDED_)
