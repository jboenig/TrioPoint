//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Group.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __GROUP_H__
#define __GROUP_H__

#pragma once

#include <list>
#include <map>

#include "ShapeShifter.h"
#include "Node.h"
#include "Visual.h"
#include "Property.h"
#include "RenderingContext.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CGroup

class ATL_NO_VTABLE CGroup : public CComObjectRootEx<CComSingleThreadModel>,
                             public CComCoClass<CGroup, &CLSID_Group>,
                             public IDispatchRTPropConImpl<CGroup, IVisualImpl< ICompositeNodeImpl< IVisual, CInterfacePair<INode,IRenderDX8> > >, &__uuidof(IVisual), &LIBID_ShapeShifter>,
                             public IRenderDX8,
                             public IPersistXMLImpl<CGroup>
{
// Constructors/destructor
public:
	CGroup();
	virtual ~CGroup();

DECLARE_REGISTRY_RESOURCEID(IDR_GROUP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Group)

// Interface map
BEGIN_COM_MAP(CGroup)
	COM_INTERFACE_ENTRY(IVisual)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CGroup, 32)

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IVisual interface

	STDMETHOD(get_Left)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Right)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Top)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Bottom)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Front)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Back)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_OriginX)(/* [out,retval] */ VALUE3D* pVal);
	STDMETHOD(get_OriginY)(/* [out,retval] */ VALUE3D* pVal);
	STDMETHOD(get_OriginZ)(/* [out,retval] */ VALUE3D* pVal);
	STDMETHOD(get_Radius)(/* [out,retval] */ VALUE3D* pVal);
	STDMETHOD(GetOrigin)(/* [out,retval] */ VARIANT* pVarPoint);
	STDMETHOD(GetBoundingBox)(/* [out,retval] */ VARIANT* pVarBox);
	STDMETHOD(GetBoundingSphere)(/* [out,retval] */ VARIANT* pVarSphere);
	STDMETHOD(HitTestRay)(/* [in] */ VARIANT varRayOrigin,
	                      /* [in] */ VARIANT varRayDirection,
	                      /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(HitTestSphere)(/* [in] */ VARIANT varSphere,
	                         /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(HitTestBox)(/* [in] */ VARIANT varBox,
	                      /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(HitTestPlane)(/* [in] */ VARIANT varPlane,
	                         /* [in] */ enum HitTestStrategy strategy);

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

// Implementation
protected:
	HRESULT CalcBoundingBox(BOX3D& boundingBox);
	HRESULT CalcBoundingSphere(SPHERE3D& boundingSphere);
	IUnknown* __stdcall GetChildUnk(const long nIdx);
	bool __stdcall PutChildUnk(IUnknown* pUnk);
};

};  // namespace ShapeShifter

#endif // #ifndef __GROUP_H__
