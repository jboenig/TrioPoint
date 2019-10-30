// AboutDlg.cpp : Implementation of CAboutDlg

#include "stdafx.h"
#include "AboutDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg

static BOOL AboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam) ;
static void AboutDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) ;

static DWORD FormatMessageFromString (LPCTSTR szFormat, LPTSTR  szBuffer, DWORD nSize, ...) ;

LRESULT CAboutDlg::OnInitDialog(UINT /* uMsg */, WPARAM /* wParam */, LPARAM /* lParam */, BOOL& /* bHandled */)
{
	CenterWindow();
	SetDlgItemText(IDC_DRIVER_NAME, (LPCTSTR) m_driverName);
	SetDlgItemText(IDC_DRIVER_DESC, (LPCTSTR) m_driverDesc);
	return TRUE;
}

static
void AboutDlg_OnCommand (HWND hwnd, int id, HWND /* hwndCtl */, UINT /* codeNotify */)
{
    switch (id) {
        case IDOK:                          // OK pushbutton/Enter keypress
        case IDCANCEL:                      // Esc keypress
            EndDialog (hwnd, TRUE) ;        // Dismiss the about dialog box
			break ;

		default:
			break ;
	}
}
