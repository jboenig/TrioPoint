//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Namespace.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __NAMESPACE_H__
#define __NAMESPACE_H__

#pragma once

#include <vector>

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CNamespace

class ATL_NO_VTABLE CNamespace : public CComObjectRootEx<CComSingleThreadModel>,
                                 public CComCoClass<CNamespace, &CLSID_Namespace>,
                                 public ICompositeNodeImpl< IDispatchPropConImpl<ICompositeNode, &__uuidof(ICompositeNode), &LIBID_ShapeShifter>, INodePtr >,
                                 public IPersistXMLImpl<CNamespace>
{
public:
// Embedded types
	typedef ICompositeNodeImpl< IDispatchPropConImpl<ICompositeNode, &__uuidof(ICompositeNode), &LIBID_ShapeShifter>, INodePtr > _NodeBase;

// Constructors/destructor
public:
	CNamespace();
	virtual ~CNamespace();

DECLARE_REGISTRY_RESOURCEID(IDR_NAMESPACE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Namespace)

// Interface map
BEGIN_COM_MAP(CNamespace)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

// Implementation
protected:
	IUnknown* __stdcall GetChildUnk(const long nIdx);
	bool __stdcall PutChildUnk(IUnknown* pUnk);
};

};  // namespace ShapeShifter

#endif // #ifndef __NAMESPACE_H__
