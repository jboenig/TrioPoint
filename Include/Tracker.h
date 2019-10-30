//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Tracker.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __TRACKER_H__
#define __TRACKER_H__

#pragma once

#include "ShapeShifter.h"
#include "Visual.h"
#include "Property.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CTracker

class ATL_NO_VTABLE CTracker : public CComObjectRootEx<CComSingleThreadModel>,
                               public CComCoClass<CTracker, &CLSID_Tracker>,
                               public IDispatchPropConImpl< ICompositeVisualImpl< IVisual, INodePtr >, &__uuidof(IVisual), &LIBID_ShapeShifter>,
                               public IRenderDX8
{
// Embedded types
public:
	typedef IDispatchPropConImpl< ICompositeVisualImpl< IVisual, INodePtr >, &__uuidof(IVisual), &LIBID_ShapeShifter> _VisualBase;

// Constructors/destructor
public:
	CTracker();
	virtual ~CTracker();

DECLARE_REGISTRY_RESOURCEID(IDR_TRACKER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CTracker)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IVisual)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
END_COM_MAP()

// Attributes
protected:
	BOX3D m_boundingBox;
	D3DCOLOR m_trackingColor;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice,
	                  IResourceManagerDX8* pDevResources);

	//////////////////////////////////////////////////////////////
	// ICompositeNode interface

	STDMETHOD(OnChildUpdate)(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode);

	//////////////////////////////////////////////////////////////
	// IVisual interface

	STDMETHOD(get_Left)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Right)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Top)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Bottom)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Front)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(get_Back)(/*[out, retval]*/ VALUE3D *pVal);
	STDMETHOD(GetOrigin)(/* [out,retval] */ VARIANT* pVarPoint);
	STDMETHOD(GetBoundingBox)(/* [out,retval] */ VARIANT* pVarBox);
	STDMETHOD(Translate)(/* [in] */ VALUE3D tx, /* [in] */ VALUE3D ty, /* [in] */ VALUE3D tz);
	STDMETHOD(Scale)(/* [in] */ VALUE3D sx, /* [in] */ VALUE3D sy, /* [in] */ VALUE3D sz);
	STDMETHOD(Rotate)(/* [in] */ VALUE3D fRadians,
	                  /* [in] */ VALUE3D fAxisX,
	                  /* [in] */ VALUE3D fAxisY,
	                  /* [in] */ VALUE3D fAxisZ);
	STDMETHOD(HitTestRay)(/* [in] */ VARIANT varRayOrigin,
	                      /* [in] */ VARIANT varRayDirection,
	                      /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(HitTestSphere)(/* [in] */ VARIANT varSphere,
	                         /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(HitTestBox)(/* [in] */ VARIANT varBox,
	                      /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(HitTestPlane)(/* [in] */ VARIANT varPlane,
	                        /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy);
	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy);

// Implementation
protected:
	virtual bool CanOwnChildren() const
	{
		return false;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __TRACKER_H__
