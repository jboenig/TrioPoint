//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: WindowedViewport.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __WINDOWEDVIEWPORT_H__
#define __WINDOWEDVIEWPORT_H__

#pragma once

#include <vector>

#include "ShapeShifter.h"
#include "Viewport.h"
#include "Property.h"
#include "DevicePresentation.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CWindowedViewport

class ATL_NO_VTABLE CWindowedViewport : public CComObjectRootEx< CComSingleThreadModel >,
                                        public CComCoClass<CWindowedViewport, &CLSID_WindowedViewport>,
                                        public IDispatchRTPropConImpl<CWindowedViewport, IViewportImpl< IVisual2DImpl< ICompositeNodeImpl< IViewport, CInterfacePair<INode,IRenderDX8> > > >, &__uuidof(IViewport), &LIBID_ShapeShifter>,
                                        public IRenderDX8,
                                        public IDevicePresentationDX8Impl
{
// Embedded types
public:
	typedef IDispatchRTPropConImpl<CWindowedViewport, IViewportImpl< IVisual2DImpl< ICompositeNodeImpl< IViewport, CInterfacePair<INode,IRenderDX8> > > >, &__uuidof(IViewport), &LIBID_ShapeShifter> _BaseClass;

// Constructors/destructor
public:
	CWindowedViewport();
	virtual ~CWindowedViewport();

DECLARE_REGISTRY_RESOURCEID(IDR_WINDOWED_VIEWPORT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CWindowedViewport)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
	COM_INTERFACE_ENTRY_IID(IID_IDevicePresentationDX8, IDevicePresentationDX8)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(IViewport)
	COM_INTERFACE_ENTRY(IVisual2D)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CWindowedViewport, 32)

	HRESULT FinalConstruct();
	void FinalRelease();

// Attributes
protected:
	IPresentationManager* m_pPresentationMgr;
	bool m_bInvalidateWindow;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IViewport interface

	STDMETHOD(_InitObject)(/* [in] */ IDevice* pDevice);
	STDMETHOD(get_Windowed)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_Windowed)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(get_ContainerWindow)(/*[out, retval]*/ long* pVal);
	STDMETHOD(put_ContainerWindow)(/*[in]*/ long newVal);
	STDMETHOD(get_InvalidateWindow)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_InvalidateWindow)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(Render)();
	STDMETHOD(RefreshScreen)();
	STDMETHOD(Invalidate)();
	STDMETHOD(DrawTracking)(/* [in] */ INodes* pTrackingNodes);

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

	//////////////////////////////////////////////////////////////
	// IVisual2D interface

	STDMETHOD(SetBounds)(LPRECT pBounds);

// Implementation
protected:
	HRESULT BeginFrame(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);
	HRESULT EndFrame(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

public:
	HRESULT get_FillMode(/*[out, retval]*/ long* pVal);
	HRESULT put_FillMode(/*[in]*/ long newVal);
	HRESULT get_ZEnable(/*[out, retval]*/ long* pVal);
	HRESULT put_ZEnable(/*[in]*/ long newVal);
	HRESULT get_LightingEnabled(/*[out, retval]*/ VARIANT_BOOL* pVal);
	HRESULT put_LightingEnabled(/*[in]*/ VARIANT_BOOL newVal);
};

};  // namespace ShapeShifter

#endif // #ifndef __WINDOWEDVIEWPORT_H__
