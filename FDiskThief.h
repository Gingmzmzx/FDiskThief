// FDiskThief.h : main header file for the FDISKTHIEF application
// Download by http://www.codefans.net

#if !defined(AFX_FDISKTHIEF_H__41E0B6D7_A4E1_4463_A51F_093251E1C439__INCLUDED_)
#define AFX_FDISKTHIEF_H__41E0B6D7_A4E1_4463_A51F_093251E1C439__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFDiskThiefApp:
// See FDiskThief.cpp for the implementation of this class
//

class CFDiskThiefApp : public CWinApp
{
public:
	CFDiskThiefApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDiskThiefApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFDiskThiefApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDISKTHIEF_H__41E0B6D7_A4E1_4463_A51F_093251E1C439__INCLUDED_)
