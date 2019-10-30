//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Engine.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __ENGINE_H_
#define __ENGINE_H_

#include <list>
#include <map>

#include "ShapeShifter.h"
#include "LicenseMgr.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CEngine

class ATL_NO_VTABLE CEngine : public CComObjectRootEx<CComSingleThreadModel>,
                              public CComCoClass<CEngine, &CLSID_Engine>,
                              public IDispatchImpl<IEngine, &IID_IEngine, &LIBID_ShapeShifter>,
                              public ISupportErrorInfo
{
// Embedded types
public:
	typedef std::map<short, IDevicePtr> _DeviceMap;
	typedef std::list<IInputDevice*> _InputDevices;

// Constructors/destructor
public:
	CEngine();

	void FinalRelease();

DECLARE_CLASSFACTORY2(CLicenseManager)

DECLARE_REGISTRY_RESOURCEID(IDR_ENGINE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEngine)
	COM_INTERFACE_ENTRY(IEngine)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// Attributes
public:
	LPDIRECT3D8 m_pD3D8;
	LPDIRECTINPUT8 m_pDI8;
	_DeviceMap m_devices;
	_InputDevices m_inputDevices;
	bool m_bInternetEnabled;
	_bstr_t m_strProxyName;
	_bstr_t m_strProxyBypass;

// Operations
public:
	STDMETHOD(InitializeEngine)();

	STDMETHOD(GetD3DEngine)(/* [out,retval] */ IUnknown** ppUnk);

	STDMETHOD(GetDeviceCount)(/* [out,retval] */ short* pCount);

	STDMETHOD(GetDevice)(/* [in] */ short nDeviceIdx,
	                     /* [out,retval] */ IDevice** ppDevice);

	STDMETHOD(GetDefaultDevice)(/* [out,retval] */ IDevice** ppDevice);

	STDMETHOD(GetInputDeviceCount)(/* [in] */ enum InputDeviceCategory devCat,
	                               /* [out,retval] */ short* pCount);

	STDMETHOD(GetInputDevice)(/* [in] */ enum InputDeviceCategory devCat,
	                          /* [in] */ short nInputDevIdx,
	                          /* [out,retval] */ IInputDevice** ppInputDev);

	STDMETHOD(get_Direct3DVersion)(/* [out, retval] */ long *pVal);

	STDMETHOD(get_InternetEnabled)(/* [out, retval] */ VARIANT_BOOL* pVal);
	STDMETHOD(put_InternetEnabled)(/* [in] */ VARIANT_BOOL newVal);

	STDMETHOD(get_ProxyName)(/* [out, retval] */ BSTR* pVal);
	STDMETHOD(put_ProxyName)(/* [in] */ BSTR newVal);

	STDMETHOD(get_ProxyBypass)(/* [out, retval] */ BSTR* pVal);
	STDMETHOD(put_ProxyBypass)(/* [in] */ BSTR newVal);

	//////////////////////////////////////////////////////////////
	// ISupportsErrorInfo interface

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// Implementation
public:
	HRESULT RegisterInputDevice(LPCDIDEVICEINSTANCE pDeviceInfo);
	void FreeInputDevices();
};

};  // namespace ShapeShifter

#endif //__ENGINE_H_
