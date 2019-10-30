//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ShapeShifterUtil.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "ShapeShifter.h"
#include "ShapeShifterUtil.h"

namespace ShapeShifter {

bstr_t GetFullPath(const _bstr_t& strFileName, const _bstr_t& strVarName)
{
	bstr_t strFullName;
	bool bInSearchPath = false;
	HKEY hKeyDirs;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\TrioPoint Software\\ShapeShifter\\Directories"),
	                 0, KEY_READ, &hKeyDirs) == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwMaxSearchPathLen = 1024;
		LPTSTR lpszSearchPath = new TCHAR[dwMaxSearchPathLen];
		LPTSTR lpszFullName = new TCHAR[_MAX_PATH];
		LPTSTR lpszFileNamePart = NULL;

		if (RegQueryValueEx(hKeyDirs, (LPCTSTR) strVarName, NULL, &dwType, (LPBYTE) lpszSearchPath, &dwMaxSearchPathLen) == ERROR_SUCCESS)
		{
			if (::SearchPath(lpszSearchPath, strFileName, NULL, _MAX_PATH, lpszFullName, &lpszFileNamePart) != 0)
			{
				strFullName = lpszFullName;
				bInSearchPath = true;
			}
		}

		delete[] lpszFullName;
		delete[] lpszSearchPath;

		RegCloseKey(hKeyDirs);
	}

	if (!bInSearchPath)
	{
		strFullName = strFileName;
	}

	return strFullName;
}

bool ParseNodeName(const _bstr_t& strNodeName, _bstr_t& strBaseName, _bstr_t& strRelName)
{
	bool bNameSplit = false;
	UINT nSepPos;

	if (FindSeparator(strNodeName, (SHORT) '.', nSepPos))
	{
		BSTR strLeft = LeftSubString(strNodeName, nSepPos-1);
		if (strLeft != NULL)
		{
			strBaseName.Assign(strLeft);
			::SysFreeString(strLeft);
		}

		BSTR strRight = RightSubString(strNodeName, nSepPos+1);
		if (strRight != NULL)
		{
			strRelName.Assign(strRight);
			::SysFreeString(strRight);
		}

		bNameSplit = true;
	}
	else
	{
		strRelName.Assign(strNodeName);
	}

	return bNameSplit;
}

D3DTEXTURESTAGESTATETYPE INT_TO_D3DTEXTURESTAGESTATETYPE(const short n)
{
	static D3DTEXTURESTAGESTATETYPE t[] =
	{
		D3DTSS_FORCE_DWORD,
		D3DTSS_COLOROP,
		D3DTSS_COLORARG1,
		D3DTSS_COLORARG2,
		D3DTSS_ALPHAOP,
		D3DTSS_ALPHAARG1,
		D3DTSS_ALPHAARG2,
		D3DTSS_BUMPENVMAT00,
		D3DTSS_BUMPENVMAT01,
		D3DTSS_BUMPENVMAT10,
		D3DTSS_BUMPENVMAT11,
		D3DTSS_TEXCOORDINDEX,
#if(DIRECT3D_VERSION > 0x0700)
		D3DTSS_FORCE_DWORD,
#else
		D3DTSS_ADDRESS,
#endif
		D3DTSS_ADDRESSU,
		D3DTSS_ADDRESSV,
		D3DTSS_BORDERCOLOR,
		D3DTSS_MAGFILTER,
		D3DTSS_MINFILTER,
		D3DTSS_MIPFILTER,
		D3DTSS_MIPMAPLODBIAS,
		D3DTSS_MAXMIPLEVEL,
		D3DTSS_MAXANISOTROPY,
		D3DTSS_BUMPENVLSCALE,
		D3DTSS_BUMPENVLOFFSET,
		D3DTSS_TEXTURETRANSFORMFLAGS,
#if(DIRECT3D_VERSION > 0x0700)
		D3DTSS_ADDRESSW,
		D3DTSS_COLORARG0,
		D3DTSS_ALPHAARG0,
		D3DTSS_RESULTARG
#else
		D3DTSS_FORCE_DWORD,
		D3DTSS_FORCE_DWORD,
		D3DTSS_FORCE_DWORD,
		D3DTSS_FORCE_DWORD
#endif
	};

	_ASSERTE(n < sizeof(t));

	return t[n];
}

D3DTEXTUREFILTERTYPE INT_TO_D3DTEXTUREFILTERTYPE(const short n)
{
	static D3DTEXTUREFILTERTYPE t[] =
	{
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
		D3DTEXF_FLATCUBIC,
		D3DTEXF_GAUSSIANCUBIC
	};

	_ASSERTE(n < sizeof(t));

	return t[n];
}

D3DTEXTUREADDRESS INT_TO_D3DTEXTUREADDRESS(const short n)
{
	static D3DTEXTUREADDRESS t[] =
	{
		D3DTADDRESS_FORCE_DWORD,
		D3DTADDRESS_WRAP,
		D3DTADDRESS_MIRROR,
		D3DTADDRESS_CLAMP,
		D3DTADDRESS_BORDER
	};

	_ASSERTE(n < sizeof(t));

	return t[n];
}

D3DRENDERSTATETYPE INT_TO_D3DRENDERSTATETYPE(const short n)
{
	static D3DRENDERSTATETYPE t1[] =
	{
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_ZENABLE,
		D3DRS_FILLMODE,
		D3DRS_SHADEMODE,
		D3DRS_LINEPATTERN,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_ZWRITEENABLE,
		D3DRS_ALPHATESTENABLE,
		D3DRS_LASTPIXEL,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_SRCBLEND,
		D3DRS_DESTBLEND,
		D3DRS_FORCE_DWORD,
		D3DRS_CULLMODE,
		D3DRS_ZFUNC,
		D3DRS_ALPHAREF,
		D3DRS_ALPHAFUNC,
		D3DRS_DITHERENABLE,
		D3DRS_ALPHABLENDENABLE,
		D3DRS_FOGENABLE,
		D3DRS_SPECULARENABLE,
		D3DRS_ZVISIBLE,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FOGCOLOR,
		D3DRS_FOGTABLEMODE,
		D3DRS_FOGSTART,
		D3DRS_FOGEND,
		D3DRS_FOGDENSITY,
		D3DRS_FORCE_DWORD,
		D3DRS_EDGEANTIALIAS,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_ZBIAS,
		D3DRS_RANGEFOGENABLE,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_STENCILENABLE,
		D3DRS_STENCILFAIL,
		D3DRS_STENCILZFAIL,
		D3DRS_STENCILPASS,
		D3DRS_STENCILFUNC,
		D3DRS_STENCILREF,
		D3DRS_STENCILMASK,
		D3DRS_STENCILWRITEMASK,
		D3DRS_TEXTUREFACTOR
	};

	static D3DRENDERSTATETYPE t2[] =
	{
		D3DRS_WRAP0,
		D3DRS_WRAP1,
		D3DRS_WRAP2,
		D3DRS_WRAP3,
		D3DRS_WRAP4,
		D3DRS_WRAP5,
		D3DRS_WRAP6,
		D3DRS_WRAP7,
		D3DRS_CLIPPING,
		D3DRS_LIGHTING,
		D3DRS_FORCE_DWORD,
		D3DRS_AMBIENT,
		D3DRS_FOGVERTEXMODE,
		D3DRS_COLORVERTEX,
		D3DRS_LOCALVIEWER,
		D3DRS_NORMALIZENORMALS,
		D3DRS_FORCE_DWORD,
		D3DRS_DIFFUSEMATERIALSOURCE,
		D3DRS_SPECULARMATERIALSOURCE,
		D3DRS_AMBIENTMATERIALSOURCE,
		D3DRS_EMISSIVEMATERIALSOURCE,
		D3DRS_FORCE_DWORD,
		D3DRS_FORCE_DWORD,
		D3DRS_VERTEXBLEND,
		D3DRS_CLIPPLANEENABLE
	};

	_ASSERTE(n <= 152);

	if (n >= 128)
	{
		return t2[n-128];
	}

	return t1[n];
}

D3DFORMAT INT_TO_D3DFORMAT(const short n)
{
	static D3DFORMAT t1[] =
	{
		D3DFMT_R8G8B8,
		D3DFMT_A8R8G8B8,
		D3DFMT_X8R8G8B8,
		D3DFMT_R5G6B5,
		D3DFMT_X1R5G5B5,
		D3DFMT_A1R5G5B5,
		D3DFMT_A4R4G4B4,
		D3DFMT_R3G3B2,
		D3DFMT_A8,
		D3DFMT_A8R3G3B2,
		D3DFMT_X4R4G4B4
	};

	D3DFORMAT res = D3DFMT_FORCE_DWORD;

	if (n >= 20 && n <= 30)
	{
		res = t1[n-20];
	}
	else
	{
		switch (n)
		{
		case 40:
			res = D3DFMT_A8P8;
			break;
		case 41:
			res = D3DFMT_P8;
			break;
		case 50:
			res = D3DFMT_L8;
			break;
		case 51:
			res = D3DFMT_A8L8;
			break;
		case 52:
			res = D3DFMT_A4L4;
			break;
		case 60:
			res = D3DFMT_V8U8;
			break;
		case 61:
			res = D3DFMT_L6V5U5;
			break;
		case 62:
			res = D3DFMT_X8L8V8U8;
			break;
		case 63:
			res = D3DFMT_Q8W8V8U8;
			break;
		case 64:
			res = D3DFMT_V16U16;
			break;
		case 65:
			res = D3DFMT_W11V11U10;
			break;
		case MAKEFOURCC('U', 'Y', 'V', 'Y'):
			res = D3DFMT_UYVY;
			break;
		case MAKEFOURCC('Y', 'U', 'Y', '2'):
			res = D3DFMT_YUY2;
			break;
		case MAKEFOURCC('D', 'X', 'T', '1'):
			res = D3DFMT_DXT1;
			break;
		case MAKEFOURCC('D', 'X', 'T', '2'):
			res = D3DFMT_DXT2;
			break;
		case MAKEFOURCC('D', 'X', 'T', '3'):
			res = D3DFMT_DXT3;
			break;
		case MAKEFOURCC('D', 'X', 'T', '4'):
			res = D3DFMT_DXT4;
			break;
		case MAKEFOURCC('D', 'X', 'T', '5'):
			res = D3DFMT_DXT5;
			break;
		case 70:
			res = D3DFMT_D16_LOCKABLE;
			break;
		case 71:
			res = D3DFMT_D32;
			break;
		case 73:
			res = D3DFMT_D15S1;
			break;
		case 75:
			res = D3DFMT_D24S8;
			break;
		case 80:
			res = D3DFMT_D16;
			break;
		case 77:
			res = D3DFMT_D24X8;
			break;
		case 79:
			res = D3DFMT_D24X4S4;
			break;
		case 100:
			res = D3DFMT_VERTEXDATA;
			break;
		case 101:
			res = D3DFMT_INDEX16;
			break;
		case 102:
			res = D3DFMT_INDEX32;
			break;
		}
	}

	return res;
}

};  // namespace ShapeShifter
