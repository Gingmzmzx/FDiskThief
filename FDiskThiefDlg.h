// FDiskThiefDlg.h : header file
// Download by http://www.codefans.net

#include "afxwin.h"
#if !defined(AFX_FDISKTHIEFDLG_H__C2F2D2C7_E9B5_4C6C_A9CF_554FCA8AD884__INCLUDED_)
#define AFX_FDISKTHIEFDLG_H__C2F2D2C7_E9B5_4C6C_A9CF_554FCA8AD884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFDiskThiefDlg dialog

class CFDiskThiefDlg : public CDialog
{
	// Construction
public:
	BOOL IsRun;
	void CopyFile(CString dir);
	CString FindFdisk();
	CString m_NewFdisk;
	CString m_OldFdisk;
	CFDiskThiefDlg(CWnd *pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(CFDiskThiefDlg)
	enum { IDD = IDD_FDISKTHIEF_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDiskThiefDlg)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFDiskThiefDlg)
	virtual BOOL OnInitDialog();
	void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	BOOL ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags);
	afx_msg HCURSOR OnQueryDragIcon();
	void SetTuoIcon();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonHide();
	afx_msg LRESULT OnHotKey(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonTest();
	static int GetRand(int MIN, int MAX);
	static CString IntToCS(int intt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strPath;
	CButton m_BtnStart;
	CButton m_BtnStop;
	static UINT ThreadProCopyFile(LPVOID lpVoid);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDISKTHIEFDLG_H__C2F2D2C7_E9B5_4C6C_A9CF_554FCA8AD884__INCLUDED_)
