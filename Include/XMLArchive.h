//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: XMLArchive.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __XMLARCHIVE_H__
#define __XMLARCHIVE_H__

#pragma once

#include <vector>
#include <map>

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CXMLArchive

class ATL_NO_VTABLE CXMLArchive : public CComObjectRootEx<CComSingleThreadModel>,
                                  public CComCoClass<CXMLArchive, &CLSID_XMLArchive>,
                                  public IDispatchImpl<IXMLArchive, &__uuidof(IXMLArchive), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CXMLArchive();
	virtual ~CXMLArchive();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_XML_ARCHIVE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CXMLArchive)
	COM_INTERFACE_ENTRY(IXMLArchive)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Embedded types
protected:
	struct Context
	{
		bool m_bFree;
		IXMLDOMNode* m_pCurNode;

		Context(CXMLArchive* pArchive);
		Context(const Context& src);
		~Context();
		Context& operator=(const Context& src);
		void Free();
	};

	friend struct Context;

	typedef std::vector<Context> _ContextTable;
	typedef std::map<_bstr_t,IPersistXML*> _ObjectMap;

// Attributes
protected:
	IXMLDOMDocument* m_pXMLDoc;
	IXMLDOMNode* m_pCurNode;
	_ContextTable m_contextTable;
	BSTR m_strNamespace;
	_ObjectMap m_objCache;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IXMLArchive interface

	STDMETHOD(get_DOMDocument)(/* [out, retval] */ IXMLDOMDocument** ppVal);
	STDMETHOD(get_CurrentNode)(/* [out, retval] */ IXMLDOMNode** ppVal);
	STDMETHOD(putref_CurrentNode)(/* [in] */ IXMLDOMNode* pNewVal);
	STDMETHOD(get_Namespace)(/* [out, retval] */ BSTR* pVal);
	STDMETHOD(put_Namespace)(/* [in] */ BSTR newVal);
	STDMETHOD(Load)(/* [in] */ VARIANT varSource);
	STDMETHOD(Save)(/* [in] */ VARIANT varDestination);
	STDMETHOD(Clear)();
	STDMETHOD(CreateElement)(/* [in] */ BSTR strTag);
	STDMETHOD(GotoNode)(/* [in] */ BSTR strXPath);
	STDMETHOD(GotoNodeByID)(/* [in] */ BSTR strNodeID);
	STDMETHOD(GotoFirstChild)();
	STDMETHOD(GotoNextSibling)();
	STDMETHOD(GotoPreviousSibling)();
	STDMETHOD(GotoParent)();
	STDMETHOD(CompareCurrentTag)(/* [in] */ BSTR strTag);
	STDMETHOD(GetContext)(/* [out,retval] */ long* pContextHandle);
	STDMETHOD(SetContext)(/* [in] */ long nContextHandle);
	STDMETHOD(FreeContext)(/* [in] */ long nContextHandle);
	STDMETHOD(ReadElement)(/* [in] */ VARTYPE vtDataType, /* [out,retval] */ VARIANT* pValue);
	STDMETHOD(WriteElement)(/* [in] */ VARIANT varValue, /* [in] */ BSTR strFormat);
	STDMETHOD(ReadAttribute)(/* [in] */ BSTR strTag, /* [out,retval] */ VARIANT* pValue);
	STDMETHOD(WriteAttribute)(/* [in] */ BSTR strTag, /* [in] */ VARIANT varValue);
	STDMETHOD(ReadObject)(/* [out,retval] */ IUnknown** ppObj);
	STDMETHOD(WriteObject)(/* [in] */ IUnknown* pObj);
	STDMETHOD(ReadObjectValue)(/* [out,retval] */ IUnknown** ppObj);
	STDMETHOD(WriteObjectValue)(/* [in] */ IUnknown* pObj);
	STDMETHOD(ReadObjectReference)(/* [out,retval] */ IUnknown** ppObj);
	STDMETHOD(WriteObjectReference)(/* [in] */ IUnknown* pObj);

// Implementation
protected:
	HRESULT CreateDOMDocument();
	HRESULT CreateObjectNode(BSTR strTag, REFCLSID clsid, BSTR strObjectID, IXMLDOMNode** ppObjNode);
	BSTR FormatValue(const VARIANT& val, BSTR strFormat);
	void ClearObjectCache();
	IPersistXML* LookupObject(const _bstr_t& strID) const;
	bool ObjectIsCached(IUnknown* pObj) const;
	bool CacheObject(const _bstr_t& strID, IPersistXML* pObjPersist);
};

};  // namespace ShapeShifter

#endif // #ifndef __XMLARCHIVE_H__

