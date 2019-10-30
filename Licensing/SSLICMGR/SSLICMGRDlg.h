// SSLICMGRDlg.h : header file
//

#if !defined(AFX_SSLICMGRDLG_H__0814A1BA_76DD_4E0B_B60D_2639169205D9__INCLUDED_)
#define AFX_SSLICMGRDLG_H__0814A1BA_76DD_4E0B_B60D_2639169205D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSSLICMGRDlg dialog

class CSSLICMGRDlg : public CDialog
{
// Construction
public:
	CSSLICMGRDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSSLICMGRDlg)
	enum { IDD = IDD_SSLICMGR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSLICMGRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSSLICMGRDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGenerate();
	afx_msg void OnEval();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSLICMGRDLG_H__0814A1BA_76DD_4E0B_B60D_2639169205D9__INCLUDED_)
