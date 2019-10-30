//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Property.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#pragma once

#include <map>

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CPropertyAccessor

template <class T>
class CPropertyAccessor
{
// Embedded types
public:
	typedef T _SourceClass;

	enum AccessorType
	{
		UNDEFINED_ACCESSOR,
		VARIANT_ACCESSOR,
		UCHAR_ACCESSOR,
		SHORT_ACCESSOR,
		LONG_ACCESSOR,
		FLOAT_ACCESSOR,
		DOUBLE_ACCESSOR,
		BSTR_ACCESSOR,
		BOOL_ACCESSOR
	};

	typedef HRESULT (T::*VARIANTGET)(VARIANT* pVal);
	typedef HRESULT (T::*VARIANTPUT)(VARIANT val);

	typedef HRESULT (T::*UCHARGET)(unsigned char* pVal);
	typedef HRESULT (T::*UCHARPUT)(unsigned char val);

	typedef HRESULT (T::*SHORTGET)(short* pVal);
	typedef HRESULT (T::*SHORTPUT)(short val);

	typedef HRESULT (T::*LONGGET)(long* pVal);
	typedef HRESULT (T::*LONGPUT)(long val);

	typedef HRESULT (T::*FLOATGET)(float* pVal);
	typedef HRESULT (T::*FLOATPUT)(float val);

	typedef HRESULT (T::*DOUBLEGET)(double* pVal);
	typedef HRESULT (T::*DOUBLEPUT)(double val);

	typedef HRESULT (T::*BSTRGET)(BSTR* pVal);
	typedef HRESULT (T::*BSTRPUT)(BSTR val);

	typedef HRESULT (T::*BOOLGET)(VARIANT_BOOL* pVal);
	typedef HRESULT (T::*BOOLPUT)(VARIANT_BOOL val);

// Constructors/destructor
public:
	CPropertyAccessor()
	{
		m_type = UNDEFINED_ACCESSOR;
	}

	CPropertyAccessor(const CPropertyAccessor& src)
	{
		m_type = src.m_type;
		m_get = src.m_get;
		m_put = src.m_put;
	}

	CPropertyAccessor(UCHARGET pUCharGet, UCHARPUT pUCharPut)
	{
		m_type = UCHAR_ACCESSOR;
		m_get.pUChar = pUCharGet;
		m_put.pUChar = pUCharPut;
	}

	CPropertyAccessor(SHORTGET pShortGet, SHORTPUT pShortPut)
	{
		m_type = SHORT_ACCESSOR;
		m_get.pShort = pShortGet;
		m_put.pShort = pShortPut;
	}

	CPropertyAccessor(LONGGET pLongGet, LONGPUT pLongPut)
	{
		m_type = LONG_ACCESSOR;
		m_get.pLong = pLongGet;
		m_put.pLong = pLongPut;
	}

	CPropertyAccessor(FLOATGET pFloatGet, FLOATPUT pFloatPut)
	{
		m_type = FLOAT_ACCESSOR;
		m_get.pFloat = pFloatGet;
		m_put.pFloat = pFloatPut;
	}

	CPropertyAccessor(DOUBLEGET pDoubleGet, DOUBLEPUT pDoublePut)
	{
		m_type = DOUBLE_ACCESSOR;
		m_get.pDouble = pDoubleGet;
		m_put.pDouble = pDoublePut;
	}

	CPropertyAccessor(BSTRGET pBStrGet, BSTRPUT pBStrPut)
	{
		m_type = BSTR_ACCESSOR;
		m_get.pBStr = pBStrGet;
		m_put.pBStr = pBStrPut;
	}

// Attributes
public:
	AccessorType m_type;

	union
	{
		VARIANTGET pVariant;
		UCHARGET pUChar;
		SHORTGET pShort;
		LONGGET pLong;
		FLOATGET pFloat;
		DOUBLEGET pDouble;
		BSTRGET pBStr;
		BOOLGET pBool;
	} m_get;

	union
	{
		VARIANTPUT pVariant;
		UCHARPUT pUChar;
		SHORTPUT pShort;
		LONGPUT pLong;
		FLOATPUT pFloat;
		DOUBLEPUT pDouble;
		BSTRPUT pBStr;
		BOOLPUT pBool;
	} m_put;

// Operations
public:
	HRESULT GetValue(const _SourceClass* pObj, VARIANT& propVal) const
	{
		USES_CONVERSION;

		HRESULT hr = E_FAIL;

		switch (m_type)
		{
		case UCHAR_ACCESSOR:
			propVal.vt = VT_UI1;
			if (m_get.pUChar != NULL)
			{
				hr = (pObj->*m_get.pUChar)(&propVal.bVal);
			}
			break;

		case SHORT_ACCESSOR:
			propVal.vt = VT_I2;
			if (m_get.pShort != NULL)
			{
				hr = (pObj->*m_get.pShort)(&propVal.iVal);
			}
			break;

		case LONG_ACCESSOR:
			propVal.vt = VT_I4;
			if (m_get.pLong != NULL)
			{
				hr = (pObj->*m_get.pLong)(&propVal.lVal);
			}
			break;

		case FLOAT_ACCESSOR:
			propVal.vt = VT_R4;
			if (m_get.pFloat != NULL)
			{
				hr = (pObj->*m_get.pFloat)(&propVal.fltVal);
			}
			break;

		case DOUBLE_ACCESSOR:
			propVal.vt = VT_R8;
			if (m_get.pDouble != NULL)
			{
				hr = (pObj->*m_get.pDouble)(&propVal.dblVal);
			}
			break;

		case BSTR_ACCESSOR:
			propVal.vt = VT_BSTR;
			if (m_get.pBStr != NULL)
			{
				hr = (pObj->*m_get.pBStr)(&propVal.bstrVal);
			}
			break;

		case BOOL_ACCESSOR:
			propVal.vt = VT_BOOL;
			if (m_get.pBool != NULL)
			{
				hr = (pObj->*m_get.pBool)(&propVal.boolVal);
			}
			break;
		}

		return hr;
	}

	HRESULT PutValue(_SourceClass* pObj, const VARIANT& propVal)
	{
		USES_CONVERSION;

		HRESULT hr = E_FAIL;

		switch (m_type)
		{
		case UCHAR_ACCESSOR:
			if (m_put.pUChar != NULL && propVal.vt == VT_UI1)
			{
				hr = (pObj->*m_put.pUChar)(propVal.bVal);
			}
			break;

		case SHORT_ACCESSOR:
			if (m_put.pShort != NULL && propVal.vt == VT_I2)
			{
				hr = (pObj->*m_put.pShort)(propVal.iVal);
			}
			break;

		case LONG_ACCESSOR:
			if (m_put.pLong != NULL && propVal.vt == VT_I4)
			{
				hr = (pObj->*m_put.pLong)(propVal.lVal);
			}
			break;

		case FLOAT_ACCESSOR:
			if (m_put.pFloat != NULL && propVal.vt == VT_R4)
			{
				hr = (pObj->*m_put.pFloat)(propVal.fltVal);
			}
			break;

		case DOUBLE_ACCESSOR:
			if (m_put.pDouble != NULL && propVal.vt == VT_R8)
			{
				hr = (pObj->*m_put.pDouble)(propVal.dblVal);
			}
			break;

		case BSTR_ACCESSOR:
			if (m_put.pBStr != NULL && propVal.vt == VT_BSTR)
			{
				hr = (pObj->*m_put.pBStr)(propVal.bstrVal);
			}
			break;

		case BOOL_ACCESSOR:
			if (m_put.pBool != NULL && propVal.vt == VT_BOOL)
			{
				hr = (pObj->*m_put.pBool)(propVal.boolVal);
			}
			break;
		}

		return hr;
	}
};

template <class T>
class CPropertyBoolAccessor : public CPropertyAccessor<T>
{
public:
	CPropertyBoolAccessor(BOOLGET pBoolGet, BOOLPUT pBoolPut)
	{
		m_type = BOOL_ACCESSOR;
		m_get.pBool = pBoolGet;
		m_put.pBool = pBoolPut;
	}
};

/////////////////////////////////////////////////////////////////////////
// Macros for defining property accessors

#define SSCL_DEFINE_PROPERTY_ACCESSOR(ClsName,PropName)  \
	static CPropertyAccessor<ClsName> g_Accessor_##PropName(&ClsName::get_##PropName,&ClsName::put_##PropName);

#define SSCL_DEFINE_PROPERTY_READONLY_ACCESSOR(ClsName,PropName)  \
	static CPropertyAccessor<ClsName> g_Accessor_##PropName(&ClsName::get_##PropName,NULL);

#define SSCL_DEFINE_PROPERTY_BOOL_ACCESSOR(ClsName,PropName)  \
	static CPropertyBoolAccessor<ClsName> g_Accessor_##PropName(&ClsName::get_##PropName,&ClsName::put_##PropName);

#define SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(ClsName,PropName)  \
	static CPropertyBoolAccessor<ClsName> g_Accessor_##PropName(&ClsName::get_##PropName,NULL);

/////////////////////////////////////////////////////////////////////////
// CProperty

template <class T>
struct CProperty
{
	typedef CPropertyAccessor<T> _AccessorType;
	_bstr_t propName;
	DISPID propId;
	VARTYPE propType;
	_AccessorType* pPropAccessor;
};

/////////////////////////////////////////////////////////////////////////
// CPropertyTable

template <class T, int PROP_COUNT>
struct CPropertyTable
{
	typedef CProperty<T> _PropertyType;
	_PropertyType m_props[PROP_COUNT];

	void Clear()
	{
		for (int i = 0; i < PROP_COUNT; i++)
		{
			m_props[i].propName = L"";
			m_props[i].propId = DISPID_UNKNOWN;
			m_props[i].propType = VT_EMPTY;
			m_props[i].pPropAccessor = NULL;
		}
	}

	_PropertyType& GetProperty(const DISPID dispId)
	{
		for (int i = 0; i < PROP_COUNT && m_props[i].propId != DISPID_UNKNOWN; i++)
		{
			if (dispId == m_props[i].propId)
			{
				return m_props[i];
			}
		}

		_com_issue_error(E_INVALIDARG);

		return m_props[0];  // Supresses warning only!
	}

	DISPID LookupId(const LPOLESTR propName)
	{
		for (int i = 0; i < PROP_COUNT && m_props[i].propId != DISPID_UNKNOWN; i++)
		{
			if (_bstr_t(m_props[i].propName) == _bstr_t(propName))
			{
				return m_props[i].propId;
			}
		}
		return DISPID_UNKNOWN;
	}

	void AddProperty(const _PropertyType& prop)
	{
		int nSlotIdx;

		if (!GetUnusedSlot(nSlotIdx))
		{
			_com_issue_error(E_FAIL);
		}

		m_props[nSlotIdx] = prop;
	}

	void SetProperty(const _PropertyType& prop, const int nSlotIdx)
	{
		m_props[nSlotIdx] = prop;
	}

	HRESULT PutProperty(T* pObj, const DISPID dispId, const VARIANT& val, bool& bHandled)
	{
		HRESULT hr = S_OK;

		_variant_t valBuf(val);

		try
		{
			_PropertyType& prop = GetProperty(dispId);
			_PropertyType::_AccessorType* pPropAccessor = prop.pPropAccessor;
			valBuf.ChangeType(prop.propType);

			if (pPropAccessor != NULL)
			{
				hr = pPropAccessor->PutValue(pObj, valBuf);
				bHandled = true;
			}
		}
		catch (_com_error& e)
		{
			hr = e.Error();
		}

		return hr;
	}

	HRESULT GetProperty(T* pObj, const DISPID dispId, VARIANT& val, bool& bHandled)
	{
		HRESULT hr = S_OK;

		try
		{
			_PropertyType& prop = GetProperty(dispId);
			_PropertyType::_AccessorType* pPropAccessor = prop.pPropAccessor;

			if (pPropAccessor != NULL)
			{
				hr = pPropAccessor->GetValue(pObj, val);
				bHandled = true;
			}
		}
		catch (_com_error& e)
		{
			hr = e.Error();
		}

		return hr;
	}

	bool GetUnusedSlot(int& nSlotIdx) const
	{
		for (nSlotIdx = 0; nSlotIdx < PROP_COUNT; nSlotIdx++)
		{
			if (m_props[nSlotIdx].propId == DISPID_UNKNOWN)
			{
				return true;
			}
		}

		return false;
	}
};

/////////////////////////////////////////////////////////////////////////
// Macros for declaring and initializing runtime property tables

#define SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(ClsName, PropCount) \
	public: typedef CPropertyTable<ClsName,PropCount> _PropertyTableType;  \
	static _PropertyTableType* GetRuntimePropertyTable();
	
#define SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(ClsName)  \
	ClsName::_PropertyTableType* ClsName::GetRuntimePropertyTable() { \
		static _PropertyTableType propTable;   \
		static bool bInitDone = false;  \
		if (!bInitDone) { _PropertyTableType::_PropertyType prop; int nSlotIdx = 0; propTable.Clear(); \

#define SSCL_RUNTIME_PROPERTY_ENTRY(PropName, PropId, PropType)  \
	prop.propName = L#PropName; prop.propId = PropId; prop.propType = PropType; prop.pPropAccessor = &g_Accessor_##PropName; \
	propTable.SetProperty(prop, nSlotIdx++);  \

#define SSCL_RUNTIME_PROPERTY_ENTRY_ACCESSOR(PropName, PropId, PropType, PropAccessor)  \
	{ L#PropName, PropId, PropType, PropAccessor },

#define SSCL_RUNTIME_PROPERTY_ENTRY_NULL    \
	prop.propName = L""; prop.propId = DISPID_UNKNOWN; prop.propType = VT_EMPTY; prop.pPropAccessor = NULL; \
	propTable.SetProperty(prop, nSlotIdx++);  \

#define SSCL_END_RUNTIME_PROPERTY_TABLE   bInitDone = true; } return &propTable; }

/////////////////////////////////////////////////////////////////////////////
// IDispatchPropConImpl

template <class base_class, const IID* piid,
          const GUID* plibid = &CComModule::m_libid,
          WORD wMajor = 1, WORD wMinor = 0,
          class tihclass = CComTypeInfoHolder>

class IDispatchPropConImpl : public base_class
{
public:
	typedef tihclass _tihclass;

	// IDispatch
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
	{
		*pctinfo = 1;
		return S_OK;
	}
	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
	{
		return _tih.GetTypeInfo(itinfo, lcid, pptinfo);
	}
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid)
	{
		HRESULT hr = _tih.GetIDsOfNames(riid, rgszNames, cNames, lcid, rgdispid);

		if (hr == DISP_E_UNKNOWNNAME)
		{
			for (UINT nIdx = 0; nIdx < cNames; nIdx++)
			{
				if (GetPropertyId(rgszNames[nIdx], &rgdispid[nIdx]) == S_OK)
				{
					hr = S_OK;
				}
			}
		}

		return hr;
	}
	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* pexcepinfo, UINT* puArgErr)
	{
		HRESULT hr = _tih.Invoke((IDispatch*)this, dispidMember,
		                         riid, lcid,
		                         wFlags, pdispparams,
		                         pvarResult, pexcepinfo,
		                         puArgErr);

		if (hr == DISP_E_MEMBERNOTFOUND)
		{
			if (wFlags & DISPATCH_PROPERTYPUT)
			{
				hr = PutProperty(dispidMember, pdispparams->rgvarg[0]);
			}
			else if (wFlags & DISPATCH_PROPERTYGET)
			{
				hr = GetProperty(dispidMember, pvarResult);
			}
		}

		return hr;
	}

	// IPropertyContainer interface

	STDMETHOD(GetPropertyId)(/* [in] */ BSTR strPropertyName,
	                         /* [out,retval] */ DISPID* pPropertyId)
	{
		return _tih.GetIDsOfNames(IID_NULL, &strPropertyName, 1, LOCALE_SYSTEM_DEFAULT, pPropertyId);
	}

	STDMETHOD(GetProperty)(/* [in] */ DISPID propertyId,
	                       /* [out,retval] */ VARIANT* pValue)
	{
		DISPPARAMS params = {NULL, NULL, 0, 0};
		return _tih.Invoke((IDispatch*)this, propertyId, IID_NULL, LOCALE_SYSTEM_DEFAULT,
		                    DISPATCH_PROPERTYGET, &params, pValue, NULL, NULL);
	}

	STDMETHOD(PutProperty)(/* [in] */ DISPID propertyId,
	                       /* [in] */ VARIANT varValue)
	{
		DISPPARAMS params = {&varValue, NULL, 1, 0};
		CComVariant varResult;
		return _tih.Invoke((IDispatch*)this, propertyId, IID_NULL, LOCALE_SYSTEM_DEFAULT,
		                   DISPATCH_PROPERTYPUT, &params, &varResult, NULL, NULL);
	}

	STDMETHOD(AddProperty)(/* [in] */ BSTR strPropertyName,
	                       /* [in] */ VARIANT varValue,
	                       /* [out,retval] */ DISPID* pPropertyId)
	{
		strPropertyName;
		varValue;
		pPropertyId;
		return E_NOTIMPL;
	}

	STDMETHOD(RemoveProperty)(/* [in] */ DISPID PropertyId)
	{
		PropertyId;
		return E_NOTIMPL;
	}

protected:
	static _tihclass _tih;
	static HRESULT GetTI(LCID lcid, ITypeInfo** ppInfo)
	{
		return _tih.GetTI(lcid, ppInfo);
	}
};

template <class base_class, const IID* piid, const GUID* plibid, WORD wMajor, WORD wMinor, class tihclass>
IDispatchPropConImpl<base_class, piid, plibid, wMajor, wMinor, tihclass>::_tihclass
IDispatchPropConImpl<base_class, piid, plibid, wMajor, wMinor, tihclass>::_tih =
	{piid, plibid, wMajor, wMinor, NULL, 0, NULL, 0};

/////////////////////////////////////////////////////////////////////////
// IDispatchRTPropConImpl

template <class T, class base_class, const IID* piid,
          const GUID* plibid = &CComModule::m_libid,
          WORD wMajor = 1, WORD wMinor = 0,
          class tihclass = CComTypeInfoHolder>

class IDispatchRTPropConImpl : public IDispatchPropConImpl<base_class, piid, plibid, wMajor, wMinor, tihclass>
{
// Embedded types
public:
	typedef IDispatchPropConImpl<base_class, piid, plibid, wMajor, wMinor, tihclass> _BaseClass;
	typedef std::map<DISPID,_variant_t> _PropertyValueMap;

// Attributes
public:
	_PropertyValueMap m_propVals;

// Operations
public:
	// IPropertyContainer interface

	STDMETHOD(GetPropertyId)(/* [in] */ BSTR strPropertyName,
	                         /* [out,retval] */ DISPID* pPropertyId)
	{
		if (pPropertyId == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = _BaseClass::GetPropertyId(strPropertyName, pPropertyId);

		if (hr == DISP_E_UNKNOWNNAME)
		{
			T::_PropertyTableType* pTable = T::GetRuntimePropertyTable();

			*pPropertyId = pTable->LookupId(strPropertyName);

			if (*pPropertyId == DISPID_UNKNOWN)
			{
				hr = DISP_E_UNKNOWNNAME;
			}
			else
			{
				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(GetProperty)(/* [in] */ DISPID propertyId,
	                       /* [out,retval] */ VARIANT* pValue)
	{
		if (pValue == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = _BaseClass::GetProperty(propertyId, pValue);


		if (hr == DISP_E_MEMBERNOTFOUND)
		{
			T::_PropertyTableType* pTable = T::GetRuntimePropertyTable();
			bool bHandled = false;
			hr = pTable->GetProperty(static_cast<T*>(this), propertyId, *pValue, bHandled);

			if (!bHandled)
			{
				// No accessor in property table to handle the get. Get the value from
				// the property value map.
				_PropertyValueMap::iterator itPropVal = m_propVals.find(propertyId);

				if (itPropVal != m_propVals.end())
				{
					*pValue = itPropVal->second;
					hr = S_OK;
				}
			}
		}

		return hr;
	}

	STDMETHOD(PutProperty)(/* [in] */ DISPID propertyId,
	                       /* [in] */ VARIANT varValue)
	{
		HRESULT hr = _BaseClass::PutProperty(propertyId, varValue);

		if (hr == DISP_E_MEMBERNOTFOUND)
		{
			T::_PropertyTableType* pTable = T::GetRuntimePropertyTable();
			bool bHandled = false;
			hr = pTable->PutProperty(static_cast<T*>(this), propertyId, varValue, bHandled);

			if (!bHandled)
			{
				// No accessor in property table to handle the put. Store the value in the
				// property value map.
				m_propVals[propertyId] = _variant_t(varValue);
				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(AddProperty)(/* [in] */ BSTR strPropertyName,
	                       /* [in] */ VARIANT varValue,
	                       /* [out,retval] */ DISPID* pPropertyId)
	{
		HRESULT hr = S_OK;

		T::_PropertyTableType::_PropertyType prop;
		prop.propId = DISPID_UNKNOWN;

		if (GetPropertyId(strPropertyName, &prop.propId) != S_OK)
		{
			T::_PropertyTableType* pTable = T::GetRuntimePropertyTable();

			try
			{
				prop.propName = strPropertyName;
				prop.propId = GetNextCustomPropertyID();
				prop.propType = VT_VARIANT;
				prop.pPropAccessor = NULL;
				pTable->AddProperty(prop);
			}
			catch (_com_error& e)
			{
				hr = e.Error();
			}
		}

		if (hr == S_OK)
		{
			hr = PutProperty(prop.propId, varValue);
		}

		if (pPropertyId != NULL)
		{
			*pPropertyId = prop.propId;
		}

		return hr;
	}

	STDMETHOD(RemoveProperty)(/* [in] */ DISPID PropertyId)
	{
		PropertyId;
		return E_NOTIMPL;
	}

// Implementation
protected:

	static DISPID GetNextCustomPropertyID()
	{
		static DISPID idNext = SSCLID_USERDEFINED_MIN;

		if (idNext > SSCLID_USERDEFINED_MAX)
		{
			_com_issue_error(E_FAIL);
		}

		return idNext++;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __PROPERTY_H__
