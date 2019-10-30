//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ShapeShifterUtil.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __SHAPESHIFTERUTIL_H__
#define __SHAPESHIFTERUTIL_H__

namespace ShapeShifter {

const FLOAT g_PI       =  3.14159265358979323846f; // Pi
const FLOAT g_2_PI     =  6.28318530717958623200f; // 2 * Pi

#define RGB_GETRED(rgb)         ((FLOAT) ((rgb) & 0xff) / 255.0f)
#define RGB_GETGREEN(rgb)       ((FLOAT) (((rgb) >> 8) & 0xff) / 255.0f)
#define RGB_GETBLUE(rgb)        ((FLOAT) (((rgb) >> 16) & 0xff) / 255.0f)

const FLOAT EPSILON    =  0.0001f;   // used for comparing floats for equality

inline bool IsEqualFloat(const FLOAT& f1, const FLOAT& f2)
{
	return (fabs(f1 - f2) < EPSILON);
}

inline void CopyFlexVertex(LPVOID pDest, DWORD dwDestFormat, LPVOID pSrc, DWORD dwSrcFormat)
{
	if (dwDestFormat == dwSrcFormat)
	{
		// Straight copy
		memcpy(pDest, pSrc, D3DXGetFVFVertexSize(dwDestFormat));
	}
	else
	{
		// Need a little magic here
	}
}

inline bool FindSeparator(BSTR str, SHORT sep, UINT& nSepPos)
{
	bool bSepFound = false;
	UINT nLen = ::SysStringLen(str);
	nSepPos = 0;

	while (!bSepFound && nSepPos < nLen)
	{
		if (str[nSepPos] == sep)
		{
			bSepFound = true;
		}
		else
		{
			nSepPos++;
		}
	}

	return bSepFound;
}

inline BSTR LeftSubString(BSTR str, const UINT nPos)
{
	BSTR strSub = NULL;

	if (nPos >= 0 && nPos < ::SysStringLen(str))
	{
		strSub = ::SysAllocStringLen(str, nPos + 1);
	}

	return strSub;
}

inline BSTR RightSubString(BSTR str, const UINT nPos)
{
	BSTR strSub = NULL;
	UINT nLen = ::SysStringLen(str);

	if (nPos >= 0 && nPos < nLen)
	{
		strSub = ::SysAllocStringLen(&str[nPos], nLen - nPos);
	}

	return strSub;
}

inline bool VariantIsEmpty(const VARIANT& var)
{
	return (var.vt == VT_EMPTY ||
	        (var.vt == VT_BSTR && (var.bstrVal == NULL || ::SysStringLen(var.bstrVal)) == 0));
}

/////////////////////////////////////////////////////////////////////////
// Search path variable for filename

extern bstr_t GetFullPath(const _bstr_t& strFileName,
                          const _bstr_t& strVarName);

/////////////////////////////////////////////////////////////////////////
// Parse node name into base name and relative path

extern bool ParseNodeName(const _bstr_t& strNodeName,
                          _bstr_t& strBaseName,
                          _bstr_t& strRelName);

/////////////////////////////////////////////////////////////////////////
// Routines to convert between Direct3D enumerations and
// ShapeShifter enumerations

extern D3DTEXTURESTAGESTATETYPE INT_TO_D3DTEXTURESTAGESTATETYPE(const short n);
extern D3DTEXTUREFILTERTYPE INT_TO_D3DTEXTUREFILTERTYPE(const short n);
extern D3DTEXTUREADDRESS INT_TO_D3DTEXTUREADDRESS(const short n);
extern D3DRENDERSTATETYPE INT_TO_D3DRENDERSTATETYPE(const short n);
extern D3DFORMAT INT_TO_D3DFORMAT(const short n);

/////////////////////////////////////////////////////////////////////////
// CInterfacePair

template <class _iface1, class _iface2>
class CInterfacePair
{
// Constructors/destructor
public:
	CInterfacePair(_iface1* pSrc = NULL) :
		m_p1(NULL),
		m_p2(NULL)
	{
		m_p1 = pSrc;

		if (m_p1 != NULL)
		{
			m_p1->AddRef();
			if (FAILED(m_p1->QueryInterface(__uuidof(_iface2), (LPVOID*) &m_p2)))
			{
				m_p2 = NULL;
			}
		}
	}

	CInterfacePair(const CInterfacePair<_iface1,_iface2>& src) :
		m_p1(NULL),
		m_p2(NULL)
	{
		m_p1 = src.m_p1;
		if (m_p1 != NULL)
		{
			m_p1->AddRef();
		}
		m_p2 = src.m_p2;
		if (m_p2 != NULL)
		{
			m_p2->AddRef();
		}
	}

	~CInterfacePair()
	{
		if (m_p2 != NULL)
		{
			m_p2->Release();
		}

		if (m_p1 != NULL)
		{
			m_p1->Release();
		}
	}

// Attributes
public:
	_iface1* m_p1;
	_iface2* m_p2;

// Operations
public:
	CInterfacePair<_iface1,_iface2>& operator=(const CInterfacePair<_iface1,_iface2>& src)
	{
		if (src.m_p1 != NULL)
		{
			src.m_p1->AddRef();
		}
		if (m_p1 != NULL)
		{
			m_p1->Release();
		}
		m_p1 = src.m_p1;

		if (src.m_p2 != NULL)
		{
			src.m_p2->AddRef();
		}
		if (m_p2 != NULL)
		{
			m_p2->Release();
		}
		m_p2 = src.m_p2;

		return *this;
	}

	operator _iface1*() const
	{
		return m_p1;
	}

	operator _iface2*() const
	{
		return m_p2;
	}

	_iface1* operator->() const
	{
		if (m_p1 == NULL)
		{
			_com_issue_error(E_POINTER);
		}

		return m_p1; 
	}

	ULONG Release()
	{
		ULONG nRtn = 0;

		if (m_p1 != NULL)
		{
			nRtn = m_p1->Release();
			m_p1 = NULL;
		}

		if (m_p2 != NULL)
		{
			m_p2->Release();
			m_p2 = NULL;
		}

		return nRtn;
	}

	ULONG AddRef()
	{
		ULONG nRtn = 0;

		if (m_p1 != NULL)
		{
			nRtn = m_p1->AddRef();
		}

		if (m_p2 != NULL)
		{
			m_p2->AddRef();
		}

		return nRtn;
	}
};

// DECLARE_LIFETIME_OWNERSHIP ties the lifetime of a contained object to the
// lifetime of its parent. This is useful avoiding deadlocks when dealing
// with circular references. Use this macro when contained object has a back
// pointer to the parent that is not reference counted (in order to avoid
// deadlocks). This macro ensures that the parent object is never destroyed
// before the contained object by forwarding all AddRef's and Release's on
// the contained object to the parent. This is basically a subset of COM
// aggregration, where the lifetime of the contained object is subsumed by
// the parent but not its identity.
#define DECLARE_LIFETIME_OWNERSHIP(ownerObj)            \
	ULONG InternalAddRef()                              \
	{                                                   \
		if (ownerObj != NULL)                           \
		{                                               \
			return ownerObj->AddRef();                  \
		}                                               \
		ATLASSERT(m_dwRef != -1L);                      \
		return _ThreadModel::Increment(&m_dwRef);       \
	}                                                   \
	ULONG InternalRelease()                             \
	{                                                   \
		if (ownerObj != NULL)                           \
		{                                               \
			return ownerObj->Release();                 \
		}                                               \
		ATLASSERT(m_dwRef > 0);                         \
		return _ThreadModel::Decrement(&m_dwRef);       \
	}                                                   \


#ifdef _DEBUG

template <class T>
class CComSingleThreadModel_Debug
{
public:
	static ULONG WINAPI Increment(LPLONG p) {ULONG refCount = ++(*p); T::OnDbgIncrement(refCount); return refCount;}
	static ULONG WINAPI Decrement(LPLONG p) {ULONG refCount = --(*p); T::OnDbgDecrement(refCount); return refCount;}
	typedef CComFakeCriticalSection AutoCriticalSection;
	typedef CComFakeCriticalSection CriticalSection;
	typedef CComSingleThreadModel ThreadModelNoCS;
};

#endif

/////////////////////////////////////////////////////////////////////////
// vtDISPLAYMODEINFO

class vtDISPLAYMODEINFO : public CVtRecord<DISPLAYMODEINFO>
{
// Constructors/destructor
public:
	vtDISPLAYMODEINFO()
	{
		m_spRecordInfo = NULL;
		Width = 0;
		Height = 0;
		RefreshRate = 0;
		Format = 0;
	}

	vtDISPLAYMODEINFO(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtDISPLAYMODEINFO(const DISPLAYMODEINFO& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

// Operations
public:

	vtDISPLAYMODEINFO& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

		VARIANT varVal;
		varVal.vt = VT_I2;

		// Copy first point

		if (FAILED(var.pRecInfo->GetField(var.pvRecord, L"Width", &varVal)))
		{
			throw new ECopyError();
		}
		Width = varVal.iVal;

		if (FAILED(var.pRecInfo->GetField(var.pvRecord, L"Height", &varVal)))
		{
			throw new ECopyError();
		}
		Height = varVal.iVal;

		if (FAILED(var.pRecInfo->GetField(var.pvRecord, L"RefreshRate", &varVal)))
		{
			throw new ECopyError();
		}
		RefreshRate = varVal.iVal;

		if (FAILED(var.pRecInfo->GetField(var.pvRecord, L"Format", &varVal)))
		{
			throw new ECopyError();
		}
		Format = varVal.iVal;

		return *this;
	}

	vtDISPLAYMODEINFO& operator=(const DISPLAYMODEINFO& val)
	{
		Width = val.Width;
		Height = val.Height;
		RefreshRate = val.RefreshRate;
		Format = val.Format;
		return *this;
	}

	operator VARIANT() const
	{
		VARIANT var;
		::VariantClear(&var);

		IRecordInfoPtr spRecInfo = const_cast< vtDISPLAYMODEINFO* >(this)->GetRecordInfo();

		if (spRecInfo != NULL)
		{
			var.vt = VT_RECORD;
			var.pRecInfo = spRecInfo;
			var.pRecInfo->AddRef();

			var.vt = VT_RECORD;
			var.pRecInfo = spRecInfo;
			var.pRecInfo->AddRef();

			var.pvRecord = var.pRecInfo->RecordCreate();
			VARIANT varVal;
			varVal.vt = VT_I2;

			varVal.iVal = Width;
			var.pRecInfo->PutFieldNoCopy(INVOKE_PROPERTYPUT, var.pvRecord, L"Width", &varVal);

			varVal.iVal = Height;
			var.pRecInfo->PutFieldNoCopy(INVOKE_PROPERTYPUT, var.pvRecord, L"Height", &varVal);

			varVal.iVal = RefreshRate;
			var.pRecInfo->PutFieldNoCopy(INVOKE_PROPERTYPUT, var.pvRecord, L"RefreshRate", &varVal);

			varVal.iVal = Format;
			var.pRecInfo->PutFieldNoCopy(INVOKE_PROPERTYPUT, var.pvRecord, L"Format", &varVal);
		}

		return var;
	}
};

};  // namespace ShapeShifter

#endif  // #ifndef __SHAPESHIFTERUTIL_H__
