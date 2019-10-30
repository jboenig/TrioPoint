//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: LicenseUtil.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __LICENSEUTIL_H__
#define __LICENSEUTIL_H__

#pragma once

#define SSCL_LICENSEKEY_LENGTH          15

namespace ShapeShifter {

extern int __stdcall ScrambleOut(const char c);
extern char __stdcall ScrambleIn(const int n);
extern BOOL __stdcall CheckLicenseKey(LPCTSTR lpszKey);
extern BOOL __stdcall IsEvalKey(LPCTSTR lpszKey);
extern UINT __stdcall GetDaysUntilExpiration(LPCTSTR lpszKey);
extern BOOL __stdcall WriteLicenseFile(LPCTSTR lpszDirectory,
                                       LPCTSTR lpszFilename,
                                       LPCTSTR lpszSerialNum);
extern void __stdcall GenerateLicenseKey(LPTSTR lpszKey, int nMajorVer,
                                         bool bEval, UINT nEvalDuration);

};  // namespace ShapeShifter

#endif // #ifndef __LICENSEUTIL_H__
