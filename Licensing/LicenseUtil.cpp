//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: LicenseUtil.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <time.h>

#include "LicenseUtil.h"
#include "..\Include\ShapeShifterVer.h"

namespace ShapeShifter {

void LicenseKeyStripFormatting(LPSTR lpszKeyOut, LPCTSTR lpszKeyIn)
{
	UINT i, j;

	for (i = 0, j = 0; i < _tcslen(lpszKeyIn) && j < SSCL_LICENSEKEY_LENGTH; i++)
	{
		if ((lpszKeyIn[i] >= 'A' && lpszKeyIn[i] <= 'Z') || (lpszKeyIn[i] >= '0' && lpszKeyIn[i] <= '9'))
		{
			lpszKeyOut[j++] = lpszKeyIn[i];
		}
	}

	lpszKeyOut[j] = '\0';
}

int __stdcall ScrambleOut(const char c)
{
	int n = 0;

	switch (c)
	{
	case 'A': n = 12; break;
	case 'B': n = 4;  break;
	case 'C': n = 3;  break;
	case 'D': n = 1;  break;
	case 'E': n = 11; break;
	case 'F': n = 5;  break;
	case 'G': n = 19; break;
	case 'H': n = 18; break;
	case 'I': n = 16; break;
	case 'J': n = 17; break;
	case 'K': n = 24; break;
	case 'L': n = 26; break;
	case 'M': n = 28; break;
	case 'N': n = 25; break;
	case 'O': n = 27; break;
	case 'P': n = 29; break;
	case 'Q': n = 2;  break;
	case 'R': n = 6;  break;
	case 'S': n = 8;  break;
	case 'T': n = 7;  break;
	case 'U': n = 9;  break;
	case 'V': n = 13; break;
	case 'W': n = 15; break;
	case 'X': n = 14; break;
	case 'Y': n = 20; break;
	case 'Z': n = 22; break;
	case '0': n = 21; break;
	case '1': n = 23; break;
	case '2': n = 34; break;
	case '3': n = 32; break;
	case '4': n = 30; break;
	case '5': n = 33; break;
	case '6': n = 31; break;
	case '7': n = 0;  break;
	case '8': n = 35; break;
	case '9': n = 10; break;
	}

	return n;
}

char __stdcall ScrambleIn(const int n)
{
	char c = 'A';

	switch (n)
	{
	case  12: c = 'A'; break;
	case  4:  c = 'B'; break;
	case  3:  c = 'C'; break;
	case  1:  c = 'D'; break;
	case  11: c = 'E'; break;
	case  5:  c = 'F'; break;
	case  19: c = 'G'; break;
	case  18: c = 'H'; break;
	case  16: c = 'I'; break;
	case  17: c = 'J'; break;
	case  24: c = 'K'; break;
	case  26: c = 'L'; break;
	case  28: c = 'M'; break;
	case  25: c = 'N'; break;
	case  27: c = 'O'; break;
	case  29: c = 'P'; break;
	case  2:  c = 'Q'; break;
	case  6:  c = 'R'; break;
	case  8:  c = 'S'; break;
	case  7:  c = 'T'; break;
	case  9:  c = 'U'; break;
	case  13: c = 'V'; break;
	case  15: c = 'W'; break;
	case  14: c = 'X'; break;
	case  20: c = 'Y'; break;
	case  22: c = 'Z'; break;
	case  21: c = '0'; break;
	case  23: c = '1'; break;
	case  34: c = '2'; break;
	case  32: c = '3'; break;
	case  30: c = '4'; break;
	case  33: c = '5'; break;
	case  31: c = '6'; break;
	case  0:  c = '7'; break;
	case  35: c = '8'; break;
	case  10: c = '9'; break;
	}

	return c;
}

BOOL __stdcall CheckLicenseKey(LPCTSTR lpszKey)
{
	TCHAR lpszRawKey[SSCL_LICENSEKEY_LENGTH+1];
	LicenseKeyStripFormatting(lpszRawKey, lpszKey);

	if (strlen(lpszRawKey) != SSCL_LICENSEKEY_LENGTH)
	{
		return FALSE;   // Key is invalid
	}

	// Get major version number
	int nMajorVer = ScrambleOut(lpszRawKey[4]);

	if (nMajorVer != SSCL_MAJORVERSION)
	{
		// Major version number doesn't match. Key is invalid.
		return FALSE;
	}

	if (IsEvalKey(lpszRawKey))
	{
		return TRUE;
	}

	// Check combination
	if (!((lpszRawKey[0] == '8') && (lpszRawKey[2] == 'C') && (lpszRawKey[3] == '5') && (lpszRawKey[10] == '6') ||
	      (lpszRawKey[0] == '9') && (lpszRawKey[2] == '1') && (lpszRawKey[3] == 'A') && (lpszRawKey[10] == '9') ||
	      (lpszRawKey[0] == 'A') && (lpszRawKey[2] == 'F') && (lpszRawKey[3] == '2') && (lpszRawKey[10] == 'E') ||
	      (lpszRawKey[0] == 'B') && (lpszRawKey[2] == 'A') && (lpszRawKey[3] == '1') && (lpszRawKey[10] == '2') ||
	      (lpszRawKey[1] == 'P') && (lpszRawKey[2] == '2') && (lpszRawKey[3] == 'M') && (lpszRawKey[10] == 'X') ||
	      (lpszRawKey[1] == 'W') && (lpszRawKey[2] == 'H') && (lpszRawKey[3] == '8') && (lpszRawKey[10] == '1') ||
	      (lpszRawKey[1] == '7') && (lpszRawKey[2] == 'X') && (lpszRawKey[3] == 'Z') && (lpszRawKey[10] == '3') ||
	      (lpszRawKey[1] == '4') && (lpszRawKey[2] == '5') && (lpszRawKey[3] == '6') && (lpszRawKey[10] == 'L')))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL __stdcall IsEvalKey(LPCTSTR lpszKey)
{
	TCHAR lpszRawKey[SSCL_LICENSEKEY_LENGTH+1];
	LicenseKeyStripFormatting(lpszRawKey, lpszKey);

	if ((lpszRawKey[0] == 'Q') && (lpszRawKey[1] == '9') && (lpszRawKey[2] == '4') && (lpszRawKey[3] == 'B') && (lpszRawKey[10] == '1'))
	{
		return TRUE;
	}

	return FALSE;
}

UINT __stdcall GetDaysUntilExpiration(LPCTSTR lpszKey)
{
	UINT nDays = 0;

	time_t curTime;
	time(&curTime);

	if (strlen(lpszKey) == SSCL_LICENSEKEY_LENGTH)
	{
		int nYear = (ScrambleOut(lpszKey[11]) * 10) + ScrambleOut(lpszKey[12]) + 100;
		int nMonth = ScrambleOut(lpszKey[13]);
		int nDayOfMonth = ScrambleOut(lpszKey[14]);
		struct tm evalExpirationDate = { 0, 0, 0, nDayOfMonth, nMonth, nYear, 0, 0, 0 };
		time_t timeEvalExpiration = mktime(&evalExpirationDate);
		double timeDiff = difftime(timeEvalExpiration, curTime);
		nDays = (UINT) (timeDiff / 86400.0f);
	}

	return nDays;
}

BOOL __stdcall WriteLicenseFile(LPCTSTR lpszDirectory, LPCTSTR lpszFilename, LPCTSTR lpszKey)
{
	TCHAR lpszRawKey[SSCL_LICENSEKEY_LENGTH+1];
	LicenseKeyStripFormatting(lpszRawKey, lpszKey);

	UINT nKeyLen = _tcslen(lpszRawKey);
	UINT nPathLen = _tcslen(lpszDirectory) + _tcslen(lpszFilename) + 3;
	LPSTR lpszPathName = new TCHAR[nPathLen];
	_tcscpy(lpszPathName, lpszDirectory);
	_tcscat(lpszPathName, _T("\\"));
	_tcscat(lpszPathName, lpszFilename);

	FILE* pLicFile = _tfopen(lpszPathName, _T("wb"));

	if (pLicFile != NULL)
	{
		if (nKeyLen > 0)
		{
			fwrite(lpszRawKey, sizeof(TCHAR), nKeyLen, pLicFile);
		}

		fclose(pLicFile);
	}

	delete[] lpszPathName;

	return TRUE;
}

void __stdcall GenerateLicenseKey(LPTSTR lpszKey, int nMajorVer,
                                  bool bEval, UINT nEvalDuration)
{
	// Initialize key with random characters and null terminate
	for (int i = 0; i < SSCL_LICENSEKEY_LENGTH; i++)
	{
		lpszKey[i] = ScrambleIn(rand() % 36);
	}
	lpszKey[SSCL_LICENSEKEY_LENGTH] = '\0';

	// Set major version number
	lpszKey[4] = ScrambleIn(nMajorVer);

	if (bEval)
	{
		// Put eval key combination
		lpszKey[0] = 'Q';
		lpszKey[1] = '9';
		lpszKey[2] = '4';
		lpszKey[3] = 'B';
		lpszKey[10] = '1';

		// Put expiration date
		time_t expTime;
		time(&expTime);
		expTime = expTime + (nEvalDuration * 86400);
		struct tm *expiration = localtime(&expTime);

		lpszKey[11] = ScrambleIn((expiration->tm_year - 100) / 10);
		lpszKey[12] = ScrambleIn((expiration->tm_year - 100) % 10);
		lpszKey[13] = ScrambleIn(expiration->tm_mon);
		lpszKey[14] = ScrambleIn(expiration->tm_mday);

		return;
	}

	// Put key combination

	switch (rand() % 8)
	{
	case 0:
		lpszKey[0] = '8'; lpszKey[2] = 'C'; lpszKey[3] = '5'; lpszKey[10] = '6';
		break;
	case 1:
		lpszKey[0] = '9'; lpszKey[2] = '1'; lpszKey[3] = 'A'; lpszKey[10] = '9';
		break;
	case 2:
		lpszKey[0] = 'A'; lpszKey[2] = 'F'; lpszKey[3] = '2'; lpszKey[10] = 'E';
		break;
	case 3:
		lpszKey[0] = 'B'; lpszKey[2] = 'A'; lpszKey[3] = '1'; lpszKey[10] = '2';
		break;
	case 4:
		lpszKey[1] = 'P'; lpszKey[2] = '2'; lpszKey[3] = 'M'; lpszKey[10] = 'X';
		break;
	case 5:
		lpszKey[1] = 'W'; lpszKey[2] = 'H'; lpszKey[3] = '8'; lpszKey[10] = '1';
		break;
	case 6:
		lpszKey[1] = '7'; lpszKey[2] = 'X'; lpszKey[3] = 'Z'; lpszKey[10] = '3';
		break;
	case 7:
		lpszKey[1] = '4'; lpszKey[2] = '5'; lpszKey[3] = '6'; lpszKey[10] = 'L';
		break;
	}
}

};  // namespace ShapeShifter
