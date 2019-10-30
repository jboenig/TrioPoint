//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: LicenseMgr.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <time.h>

#include "LicenseMgr.h"
#include "..\Licensing\LicenseUtil.h"

namespace ShapeShifter {

static char g_szLicenseKey[SSCL_LICENSEKEY_LENGTH+1];
static bool g_bLicenseFileLoaded = false;
static bool g_bUserAnnoyed = false;

BOOL CLicenseManager::VerifyLicenseKey(BSTR strKey)
{
	_bstr_t strKeyBuf(strKey);
	const char* lpszKey = strKeyBuf;

	if (strlen(lpszKey) == 0)
	{
		// There is no license key. Put up a warning dialog but
		// allow component to run.
		WarnUser();
		return TRUE;
	}

	if (strlen(lpszKey) != SSCL_LICENSEKEY_LENGTH)
	{
		return FALSE;   // Key is invalid
	}

	BOOL bIsValid = CheckLicenseKey(lpszKey);

	if (bIsValid)
	{
		if (IsEvalKey(lpszKey))
		{
			NagUser(GetDaysUntilExpiration(lpszKey));
		}
	}

	return bIsValid;
}

BOOL CLicenseManager::GetLicenseKey(DWORD /* dwReserved */, BSTR* pStrKey)
{
	TCHAR lpszLicenseFile[FILENAME_MAX+1];

	if (!g_bLicenseFileLoaded)
	{
		if (::LoadString(_Module.m_hInst, IDS_LICENSE_FILENAME, lpszLicenseFile, FILENAME_MAX) > 0)
		{
			if (LoadLicenseFromFile(lpszLicenseFile))
			{
				g_bLicenseFileLoaded = true;
			}
		}
	}

	if (g_bLicenseFileLoaded)
	{
		*pStrKey = _bstr_t(g_szLicenseKey).copy();
		return TRUE;
	}

	return FALSE;
}

BOOL CLicenseManager::IsLicenseValid()
{
	TCHAR lpszLicenseFile[FILENAME_MAX+1];

	if (!g_bLicenseFileLoaded)
	{
		if (::LoadString(_Module.m_hInst, IDS_LICENSE_FILENAME, lpszLicenseFile, FILENAME_MAX) > 0)
		{
			if (LoadLicenseFromFile(lpszLicenseFile))
			{
				g_bLicenseFileLoaded = true;
			}
		}
	}

	return VerifyLicenseKey(_bstr_t(g_szLicenseKey));
}

bool CLicenseManager::LoadLicenseFromFile(LPCTSTR pszLicFileName)
{
	bool bSuccess = false;

	TCHAR szPathName[_MAX_PATH];
	::GetModuleFileName(_Module.m_hInst, szPathName, _MAX_PATH);
	LPTSTR pszFileName = _tcsrchr(szPathName, '\\') + 1;
	lstrcpy(pszFileName, pszLicFileName);

	FILE* pFile = _tfopen(szPathName, _T("rb"));

	if (pFile != NULL)
	{
		memset(g_szLicenseKey, 0, sizeof(char) * (SSCL_LICENSEKEY_LENGTH+1));
		size_t nActual = fread(g_szLicenseKey, sizeof(char), SSCL_LICENSEKEY_LENGTH, pFile);
		if (nActual == SSCL_LICENSEKEY_LENGTH)
		{
			bSuccess = true;
		}
		fclose(pFile);
	}

	return bSuccess;
}

void CLicenseManager::NagUser(const UINT nDaysLeft)
{
	TCHAR resBuf[200];
	TCHAR outBuf[200];
	TCHAR dlgTitle[30];

	if (g_bUserAnnoyed)
		return;

	if (nDaysLeft == 0)
	{
		::LoadString(_Module.m_hInst, IDS_LICENSE_MSGBOX_TITLE, dlgTitle, 30);
		::LoadString(_Module.m_hInst, IDS_LICENSE_EXPIRED_MSG, resBuf, 200);
		::MessageBox(NULL, resBuf, dlgTitle, MB_OK);
		g_bUserAnnoyed = true;
	}
	else if (nDaysLeft < SSCL_EVAL_NUM_NAG_DAYS)
	{
		::LoadString(_Module.m_hInst, IDS_LICENSE_MSGBOX_TITLE, dlgTitle, 30);
		::LoadString(_Module.m_hInst, IDS_LICENSE_COUNTDOWN_MSG, resBuf, 200);
		_stprintf(outBuf, resBuf, nDaysLeft);
		::MessageBox(NULL, outBuf, dlgTitle, MB_OK);
		g_bUserAnnoyed = true;
	}
}

void CLicenseManager::WarnUser()
{
	if (g_bUserAnnoyed)
		return;

	TCHAR resBuf[200];
	TCHAR dlgTitle[30];
	::LoadString(_Module.m_hInst, IDS_LICENSE_MSGBOX_TITLE, dlgTitle, 30);
	::LoadString(_Module.m_hInst, IDS_LICENSE_MISSING_MSG, resBuf, 200);
	::MessageBox(NULL, resBuf, dlgTitle, MB_OK);
	g_bUserAnnoyed = true;
}

};  // namespace ShapeShifter
