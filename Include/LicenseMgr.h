//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: LicenseMgr.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __LICENSEMGR_H__
#define __LICENSEMGR_H__

#pragma once

#include "ShapeShifter.h"

#define SSCL_EVAL_NUM_NAG_DAYS          7     /* Number of days to nag user before expiration */

namespace ShapeShifter {

class CLicenseManager
{
public:
	// The following three functions are required by CComClassFactory2
	static BOOL VerifyLicenseKey(BSTR strKey);
	static BOOL GetLicenseKey(DWORD dwReserved, BSTR* pStrKey);
	static BOOL IsLicenseValid();

	static bool LoadLicenseFromFile(LPCTSTR pszLicFileName);
	static void NagUser(const UINT nDaysLeft);
	static void WarnUser();
};

};  // namespace ShapeShifter

#endif // #ifndef __LICENSEMGR_H__
