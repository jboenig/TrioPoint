//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Internet.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Internet.h"

#ifdef SSCL_INTERNET
#include <wininet.h>
#endif

namespace ShapeShifter {

#ifdef SSCL_INTERNET

/////////////////////////////////////////////////////////////////////////
// ShapeShifter Internet API

const INTERNET_TRANSFERBLOCK_SIZE = 4096;

static LPCTSTR g_lpszAgent = _T("ShapeShifter");
static HINTERNET g_hSession = NULL;
static UINT g_nSessionRefCount = 0;
static _bstr_t g_strProxyName;
static _bstr_t g_strProxyBypass;
static BYTE g_fileXferBuffer[INTERNET_TRANSFERBLOCK_SIZE];

BOOL InternetIsURL(BSTR str)
{
	_bstr_t strUrl(str);
	URL_COMPONENTS url;
	ZeroMemory(&url, sizeof(URL_COMPONENTS));
	url.dwStructSize = sizeof(URL_COMPONENTS);

	if (InternetCrackUrl(strUrl, strUrl.length(), 0, &url))
	{
		if (url.nScheme == INTERNET_SCHEME_HTTP ||
		    url.nScheme == INTERNET_SCHEME_HTTPS ||
		    url.nScheme == INTERNET_SCHEME_FTP)
		{
			return TRUE;
		}
	}

	return FALSE;
}

HRESULT InternetStartSession(BSTR strProxyName, BSTR strProxyBypass)
{
	HRESULT hr = E_FAIL;

	if (g_hSession != NULL)
	{
		if ((_bstr_t(strProxyName) != g_strProxyName) ||
			(_bstr_t(strProxyBypass) != g_strProxyBypass))
		{
			// Caller is requesting a different proxy setup than the
			// one already initialized
			hr = E_FAIL;
		}
		else
		{
			g_nSessionRefCount++;
			hr = S_FALSE;
		}
	}
	else
	{
		g_strProxyName = _bstr_t(strProxyName);
		g_strProxyBypass = _bstr_t(strProxyBypass);

		g_hSession = InternetOpen(g_lpszAgent,
								  INTERNET_OPEN_TYPE_DIRECT,
								  NULL, NULL,
								  0);

		if (g_hSession != NULL)
		{
			g_nSessionRefCount++;
			hr = S_OK;
		}
	}

	return S_OK;
}

HRESULT InternetLoadTexture(LPDIRECT3DDEVICE8 pDevice,
                                   LPCTSTR lpszURL,
                                   LPDIRECT3DTEXTURE8* ppTexture)
{
	if (g_hSession == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	CHAR   szHead[] = "Accept: */*\r\n\r\n";
	DWORD dwActualBytesRead;
	ULONG nBytesWritten;
	IStream* pStream = NULL;
	BOOL bReadFlag = TRUE;
	BOOL bDoneReading = FALSE;
	HGLOBAL hMemFile = NULL;
	UINT nFileSize = 0;

	HINTERNET hConnection = InternetOpenUrl(g_hSession, lpszURL,
	                                        szHead, lstrlen (szHead),
	                                        INTERNET_FLAG_DONT_CACHE, 0);

	if (hConnection != NULL)
	{
		hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);

		if (hr == S_OK)
		{
			while (hr == S_OK && !bDoneReading)
			{
				bReadFlag = InternetReadFile(hConnection,
				                             g_fileXferBuffer,
				                             INTERNET_TRANSFERBLOCK_SIZE,
				                             &dwActualBytesRead);

				if (bReadFlag && dwActualBytesRead > 0)
				{
					hr = pStream->Write(g_fileXferBuffer, dwActualBytesRead, &nBytesWritten);

					if (hr == S_OK)
					{
						nFileSize += nBytesWritten;
					}
				}
				else
				{
					bDoneReading = TRUE;
				}
			}

			if (GetHGlobalFromStream(pStream, &hMemFile) == S_OK)
			{
				LPBYTE pMemFile = (LPBYTE) ::GlobalLock(hMemFile);

				if (pMemFile != NULL)
				{
					hr = D3DXCreateTextureFromFileInMemory(pDevice, pMemFile, nFileSize, ppTexture);
					::GlobalUnlock(hMemFile);
				}

				::GlobalFree(hMemFile);
			}

			pStream->Release();
		}

		InternetCloseHandle(hConnection);
	}

	return hr;
}

HRESULT InternetEndSession()
{
	if (g_hSession != NULL)
	{
		if (--g_nSessionRefCount == 0)
		{
			InternetCloseHandle(g_hSession);
			g_hSession = NULL;
		}
	}

	return S_OK;
}

#else

/////////////////////////////////////////////////////////////////////////
// Stubs

BOOL InternetIsURL(BSTR)
{
	return FALSE;
}

HRESULT InternetStartSession(BSTR, BSTR)
{
	return E_FAIL;
}

HRESULT InternetLoadTexture(LPDIRECT3DDEVICE8,
                            LPCTSTR,
                            LPDIRECT3DTEXTURE8*)
{
	return E_FAIL;
}

HRESULT InternetEndSession()
{
	return E_FAIL;
}

#endif  // #ifdef SSCL_INTERNET

};  // namespace ShapeShifter

