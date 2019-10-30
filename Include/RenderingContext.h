//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: RenderingContext.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __RENDERINGCONTEXT_H__
#define __RENDERINGCONTEXT_H__

#pragma once

#include <map>

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CRenderingContext

class ATL_NO_VTABLE CRenderingContext : public CComObjectRootEx<CComSingleThreadModel>,
                                        public CComCoClass<CRenderingContext, &CLSID_RenderingContext>,
                                        public IDispatchPropConImpl< ICompositeNodeImpl< IRenderingContext, CInterfacePair<INode,IContextObject> >, &IID_IRenderingContext, &LIBID_ShapeShifter>,
                                        public IContextObject,
                                        public IPersistXMLImpl<CRenderingContext>
{
// Embedded types
public:
	typedef IDispatchPropConImpl< ICompositeNodeImpl< IRenderingContext, CInterfacePair<INode,IContextObject> >, &IID_IRenderingContext, &LIBID_ShapeShifter> _BaseClass;
	typedef std::map<D3DRENDERSTATETYPE,DWORD> _StateMapType;

// Constructors/destructor
public:
	CRenderingContext();
	virtual ~CRenderingContext();

DECLARE_REGISTRY_RESOURCEID(IDR_RENDERING_CONTEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_RenderingContext)

// Interface map
BEGIN_COM_MAP(CRenderingContext)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IDeviceResource)
	COM_INTERFACE_ENTRY(IRenderingContext)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	_StateMapType m_stateMap;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IRenderingContext interface

	STDMETHOD(GetRenderState)(/* [in] */ short nStateId,
	                          /* [out,retval] */ long* pStateVal);

	STDMETHOD(SetRenderState)(/* [in] */ short nStateId,
	                          /* [in] */ long nStateVal);

	STDMETHOD(RemoveRenderState)(/* [in] */ short nStateId);

	/////////////////////////////////////////////////////////////////////
	// IContextObject

	STDMETHOD(SelectObject)(LPDIRECT3DDEVICE8 pDevice,
	                        IResourceManagerDX8* pDevResources,
							DWORD dwTextureStage);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

// Implementation
protected:
	IUnknown* __stdcall GetObjectUnk(const long nIdx);
	bool __stdcall PutObjectUnk(IUnknown* pUnk);
};

};  // namespace ShapeShifter

#endif // #ifndef __RENDERINGCONTEXT_H__
