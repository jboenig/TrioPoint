//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ViewportDecorator.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __VIEWPORTDECORATOR_H__
#define __VIEWPORTDECORATOR_H__

#pragma once

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "Visual.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CAxisGuide

class ATL_NO_VTABLE CAxisGuide : public CComObjectRootEx<CComSingleThreadModel>,
                                 public CComCoClass<CAxisGuide, &CLSID_AxisGuide>,
                                 public IDispatchPropConImpl<IVisualImpl< ILeafVisualImpl<IAxisGuide> >, &__uuidof(IAxisGuide), &LIBID_ShapeShifter>,
                                 public IRenderDX8
{
// Constructors/destructor
public:
	CAxisGuide();
	virtual ~CAxisGuide();

DECLARE_REGISTRY_RESOURCEID(IDR_AXIS_GUIDE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CAxisGuide)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(IVisual)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
END_COM_MAP()

// Attributes
protected:
	FLOAT m_fLen;
	D3DCOLOR m_clrX;
	D3DCOLOR m_clrY;
	D3DCOLOR m_clrZ;

// Operations
public:

	//////////////////////////////////////////////////////////////
	// IAxisGuide interface

	STDMETHOD(get_AxisLength)(/* [out, retval] */ VALUE3D* pVal);
	STDMETHOD(put_AxisLength)(/* [in] */ VALUE3D newVal);

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice,
	                  IResourceManagerDX8* pDevResources);
};

};  // namespace ShapeShifter

#endif // #ifndef __VIEWPORTDECORATOR_H__
