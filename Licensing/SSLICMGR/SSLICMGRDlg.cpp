// SSLICMGRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSLICMGR.h"
#include "SSLICMGRDlg.h"

using namespace ShapeShifter;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSLICMGRDlg dialog

CSSLICMGRDlg::CSSLICMGRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSLICMGRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSLICMGRDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSLICMGRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSLICMGRDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSLICMGRDlg, CDialog)
	//{{AFX_MSG_MAP(CSSLICMGRDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GENERATE, OnGenerate)
	ON_BN_CLICKED(IDEXIT, OnOK)
	ON_BN_CLICKED(IDC_EVAL, OnEval)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSLICMGRDlg message handlers

BOOL CSSLICMGRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_OUTPUTFILE, _T("sscl_lic.dat"));
	SetDlgItemInt(IDC_NUMKEYS, 1);
	SetDlgItemInt(IDC_EVALDAYS, 30);
	GetDlgItem(IDC_EVALDAYS)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSLICMGRDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSSLICMGRDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSSLICMGRDlg::OnGenerate() 
{
	CFile licFile;
	TCHAR licKey[SSCL_LICENSEKEY_LENGTH+1];
	bool bEval = false;
	CString strFileName;
	GetDlgItemText(IDC_OUTPUTFILE, strFileName);
	CButton* pEvalCheckbox = (CButton*) GetDlgItem(IDC_EVAL);
	UINT nEvalDays = 0;

	if (pEvalCheckbox->GetCheck() == 0)
	{
		bEval = false;
	}
	else
	{
		bEval = true;
		nEvalDays = GetDlgItemInt(IDC_EVALDAYS);
	}

	if (licFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		int nKeys = GetDlgItemInt(IDC_NUMKEYS);
		for (int i = 0; i < nKeys; i++)
		{
			GenerateLicenseKey(licKey, 1, bEval, nEvalDays);
			licFile.Write(licKey, SSCL_LICENSEKEY_LENGTH);
			licFile.Write("\r\n", 2);
		}
		licFile.Close();
	}
	else
	{
		CString strMsg;
		strMsg = "Unable to open " + strFileName;
		MessageBox(strMsg, MB_OK);
	}
}

void CSSLICMGRDlg::OnEval() 
{
	CButton* pEvalCheckbox = (CButton*) GetDlgItem(IDC_EVAL);
	if (pEvalCheckbox->GetCheck() == 0)
	{
		GetDlgItem(IDC_EVALDAYS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EVALDAYS)->EnableWindow(TRUE);
	}
}
