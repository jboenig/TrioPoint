//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Device.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __DEVICE_H__
#define __DEVICE_H__

#pragma once

#include <list>

#include "ShapeShifter.h"
#include "DynamicArray.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CDevice

class CDevice : public CComObjectRootEx< CComSingleThreadModel >,
                public CComCoClass<CDevice, &CLSID_Device>,
                public IDispatchImpl<IDevice, &__uuidof(IDevice), &LIBID_ShapeShifter>,
                public IPresentationManager,
                public IResourceManagerDX8
{
// Embedded types
public:
	typedef std::list<IDevicePresentationDX8*> _DevicePresentationList;
	typedef CDynamicArray<DeviceResourceId, UINT, UINT, 20, 20> _FreeResourceIds;
	typedef CDynamicArray<LPDIRECT3DSURFACE8, DeviceResourceId, DeviceResourceId, 20, 20> _SurfaceResources;
	typedef CDynamicArray<LPDIRECT3DVOLUME8, DeviceResourceId, DeviceResourceId, 20, 20> _VolumeResources;
	typedef CDynamicArray<LPDIRECT3DTEXTURE8, DeviceResourceId, DeviceResourceId, 20, 20> _TextureResources;
	typedef CDynamicArray<LPDIRECT3DVOLUMETEXTURE8, DeviceResourceId, DeviceResourceId, 20, 20> _VolumeTextureResources;
	typedef CDynamicArray<LPDIRECT3DCUBETEXTURE8, DeviceResourceId, DeviceResourceId, 20, 20> _CubeTextureResources;
	typedef CDynamicArray<LPDIRECT3DVERTEXBUFFER8, DeviceResourceId, DeviceResourceId, 20, 20> _VertexBufferResources;
	typedef CDynamicArray<LPDIRECT3DINDEXBUFFER8, DeviceResourceId, DeviceResourceId, 20, 20> _IndexBufferResources;

// Constructors/destructor
public:
	CDevice();
	virtual ~CDevice();

DECLARE_REGISTRY_RESOURCEID(IDR_DEVICE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CDevice)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDevice)
	COM_INTERFACE_ENTRY_IID(IID_IPresentationManager, IPresentationManager)
	COM_INTERFACE_ENTRY_IID(IID_IResourceManagerDX8, IResourceManagerDX8)
END_COM_MAP()

// Attributes
protected:
	LPDIRECT3D8 m_pD3D8;
	short m_nDeviceIdx;
	D3DADAPTER_IDENTIFIER8 m_adapterId;

	LPDIRECT3DDEVICE8 m_pD3DDevice8;
	D3DDEVTYPE m_driverMode;
	short m_nDisplayMode;
	_DevicePresentationList m_presentations;  // Weak references - interfaces not reference counted
	LPDIRECT3DSURFACE8 m_pDepthStencil;

	// Texture resource management
	_TextureResources m_textures;
	_FreeResourceIds m_freeTextureIds;

	// Vertex buffer manager
	_VertexBufferResources m_vertexBuffers;
	_FreeResourceIds m_freeVertexBufferIds;

	// Index buffer manager
	_IndexBufferResources m_indexBuffers;
	_FreeResourceIds m_freeIndexBufferIds;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IDevice interface

	STDMETHOD(_InitObject)(/* [in] */ IEngine* pEngine, /* [in] */ short nDeviceIdx);
	STDMETHOD(Initialize)(/* [in] */ long focusWnd);
	STDMETHOD(GetD3DDevice)(/* [out,retval] */ IUnknown** ppUnk);
	STDMETHOD(CreateViewport)(/* [in] */ VARIANT varProgId,
	                          /* [out,retval] */ IViewport** ppViewport);
	STDMETHOD(GetDisplayModeCount)(/* [out,retval] */ short* pNumModes);
	STDMETHOD(GetDisplayModeInfo)(/* [in] */ short nModeIdx,
	                              /* [out,retval] */ VARIANT* pDispModeInfo);
	STDMETHOD(GetDeviceGUID)(/* [out] */ GUID* pDeviceGuid);
	STDMETHOD(GetMonitor)(/* [out] */ long* pMonitorHandle);
	STDMETHOD(get_DisplayMode)(/* [out,retval] */ short *pVal);
	STDMETHOD(put_DisplayMode)(/* [in] */ short newVal);
	STDMETHOD(get_DriverMode)(/* [out,retval] */ enum DriverMode *pMode);
	STDMETHOD(put_DriverMode)(/* [in] */ enum DriverMode mode);
	STDMETHOD(get_FocusWindow)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_DriverName)(/* [out,retval] */ BSTR *pVal);
	STDMETHOD(get_DriverDesc)(/* [out,retval] */ BSTR *pVal);
	STDMETHOD(get_Product)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_Version)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_SubVersion)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_Build)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_VendorId)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_DeviceId)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_SubsysId)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_Revision)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_WHQLLevel)(/* [out,retval] */ long *pVal);

	//////////////////////////////////////////////////////////////
	// IPresentationManager interface

	STDMETHOD(AddPresentation)(IDevicePresentationDX8* pPresentation);
	STDMETHOD(RemovePresentation)(IDevicePresentationDX8* pPresentation);
	STDMETHOD(InitPresentation)(IDevicePresentationDX8* pPresentation);
	STDMETHOD(GetDeviceParameters)(D3DPRESENT_PARAMETERS* pPresentParams);

	//////////////////////////////////////////////////////////////
	// IResourceManagerDX8 interface

	STDMETHOD(GetD3DDevice8)(LPDIRECT3DDEVICE8* ppD3DDevice8);
	STDMETHOD(GenerateResourceId)(D3DRESOURCETYPE resType, DeviceResourceId* pResId);
	STDMETHOD(ReserveResourceId)(D3DRESOURCETYPE resType, DeviceResourceId nResId);
	STDMETHOD(SetSurface)(DeviceResourceId nResId, LPDIRECT3DSURFACE8 pSurface);
	STDMETHOD(GetSurface)(DeviceResourceId nResId, LPDIRECT3DSURFACE8* ppSurface);
	STDMETHOD(SetVolume)(DeviceResourceId nResId, LPDIRECT3DVOLUME8 pVolume);
	STDMETHOD(GetVolume)(DeviceResourceId nResId, LPDIRECT3DVOLUME8* ppVolume);
	STDMETHOD(SetTexture)(DeviceResourceId nResId, LPDIRECT3DTEXTURE8 pTexture);
	STDMETHOD(GetTexture)(DeviceResourceId nResId, LPDIRECT3DTEXTURE8* ppTexture);
	STDMETHOD(SetVolumeTexture)(DeviceResourceId nResId, LPDIRECT3DVOLUMETEXTURE8 pVolTexture);
	STDMETHOD(GetVolumeTexture)(DeviceResourceId nResId, LPDIRECT3DVOLUMETEXTURE8* ppVolTexture);
	STDMETHOD(SetCubeTexture)(DeviceResourceId nResId, LPDIRECT3DCUBETEXTURE8 pCubeTexture);
	STDMETHOD(GetCubeTexture)(DeviceResourceId nResId, LPDIRECT3DCUBETEXTURE8* ppCubeTexture);
	STDMETHOD(SetVertexBuffer)(DeviceResourceId nResId, LPDIRECT3DVERTEXBUFFER8 pVB);
	STDMETHOD(GetVertexBuffer)(DeviceResourceId nResId, LPDIRECT3DVERTEXBUFFER8* ppVB);
	STDMETHOD(SetIndexBuffer)(DeviceResourceId nResId, LPDIRECT3DINDEXBUFFER8 pIB);
	STDMETHOD(GetIndexBuffer)(DeviceResourceId nResId, LPDIRECT3DINDEXBUFFER8* ppIB);
	STDMETHOD(FreeResource)(D3DRESOURCETYPE resType, DeviceResourceId nResId);
	STDMETHOD(FreeAllResources)(D3DRESOURCETYPE resType);

// Implementation
protected:
	HRESULT GetCurrentDisplayModeIndex(short& nDisplayMode);
};

};  // namespace ShapeShifter

#endif // #ifndef __DEVICE_H__
