// AboutDlg.h : Declaration of the CAboutDlg

#ifndef __ABOUTDLG_H_
#define __ABOUTDLG_H_

#include "../resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
class CAboutDlg : 
	public CAxDialogImpl<CAboutDlg>
{
public:
	CAboutDlg()
	{
	}

	~CAboutDlg()
	{
	}

	enum { IDD = IDD_ABOUTDLG };

	_bstr_t m_driverName;
	_bstr_t m_driverDesc;

BEGIN_MSG_MAP(CAboutDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */)
	{
		EndDialog(wID);
		return 0;
	}

};

#endif //__ABOUTDLG_H_
