//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: InputDevice.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __INPUTDEVICE_H__
#define __INPUTDEVICE_H__

#pragma once

#include "ShapeShifter.h"
#include "Property.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CInputDevice

class CInputDevice : public CComObjectRootEx<CComSingleThreadModel>,
                     public CComCoClass<CInputDevice, &CLSID_InputDevice>,
                     public IDispatchRTPropConImpl<CInputDevice, IInputDevice, &__uuidof(IInputDevice), &LIBID_ShapeShifter>,
                     public IInitInputDeviceDX8
{
// Embedded types
public:
	typedef IDispatchRTPropConImpl<CInputDevice, IInputDevice, &__uuidof(IInputDevice), &LIBID_ShapeShifter> _BaseClass;

// Constructors/destructor
public:
	CInputDevice();
	virtual ~CInputDevice();

DECLARE_REGISTRY_RESOURCEID(IDR_INPUT_DEVICE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CInputDevice)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IInputDevice)
	COM_INTERFACE_ENTRY_IID(IID_IInitInputDeviceDX8, IInitInputDeviceDX8)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CInputDevice, 32)

// Attributes
protected:
	LPDIRECTINPUT8 m_pDI8;
	DIDEVICEINSTANCE m_deviceInfo;
	LPDIRECTINPUTDEVICE8 m_pDevice;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IInputDeviceInitDX8 interface

	STDMETHOD(_InitObject)(LPDIRECTINPUT8 pDI8, LPCDIDEVICEINSTANCE pDeviceInfo);

	//////////////////////////////////////////////////////////////
	// IInputDevice interface

	STDMETHOD(GetDIDevice)(/* [out,retval] */ IUnknown** ppDIDevice);
	STDMETHOD(get_DeviceCategory)(/* [out,retval] */ enum InputDeviceCategory *pVal);
	STDMETHOD(get_DeviceType)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_DeviceName)(/* [out,retval] */ BSTR *pVal);
	STDMETHOD(get_ProductName)(/* [out,retval] */ BSTR *pVal);
	STDMETHOD(get_Axes)(/* [out,retval] */ short *pVal);
	STDMETHOD(get_Buttons)(/* [out,retval] */ short *pVal);
	STDMETHOD(get_POVs)(/* [out,retval] */ short *pVal);
	STDMETHOD(get_FFSamplePeriod)(/* [out,retval] */ long *pVal);
	STDMETHOD(get_FFMinTimeResolution)(/* [out,retval] */ long *pVal);

	//////////////////////////////////////////////////////////////
	// Keyboard sub-type property accessors

	HRESULT get_KeyboardPCXT(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardOlivetti(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardPCAT(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardPCEnh(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardNokia1050(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardNokia9140(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardNEC98(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardNEC98Laptop(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardNEC98106(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardJapan106(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardJapanAx(VARIANT_BOOL* pVal);
	HRESULT get_KeyboardJ3100(VARIANT_BOOL* pVal);

	//////////////////////////////////////////////////////////////
	// Mouse sub-type property accessors

	HRESULT get_MouseAbsolute(VARIANT_BOOL* pVal);
	HRESULT get_MouseFingerStick(VARIANT_BOOL* pVal);
	HRESULT get_MouseTouchPad(VARIANT_BOOL* pVal);
	HRESULT get_MouseTrackBall(VARIANT_BOOL* pVal);
	HRESULT get_MouseTraditional(VARIANT_BOOL* pVal);

	//////////////////////////////////////////////////////////////
	// Joystick sub-type property accessors

	HRESULT get_JoystickLimited(VARIANT_BOOL* pVal);
	HRESULT get_JoystickStandard(VARIANT_BOOL* pVal);

	//////////////////////////////////////////////////////////////
	// Game pad sub-type property accessors

	HRESULT get_GamepadLimited(VARIANT_BOOL* pVal);
	HRESULT get_GamepadStandard(VARIANT_BOOL* pVal);
	HRESULT get_GamepadTilt(VARIANT_BOOL* pVal);

	//////////////////////////////////////////////////////////////
	// Flight controller sub-type property accessors

	HRESULT get_FlightLimited(VARIANT_BOOL* pVal);
	HRESULT get_FlightRC(VARIANT_BOOL* pVal);
	HRESULT get_FlightStick(VARIANT_BOOL* pVal);
	HRESULT get_FlightYoke(VARIANT_BOOL* pVal);

// Implementation
protected:
	HRESULT CreateDIDevice(LPDIRECTINPUTDEVICE8* ppDIDevice);
};

/////////////////////////////////////////////////////////////////////////
// CInputDevicesDX8

struct CInputDevicesDX8
{
	CInputDevicesDX8()
	{
		for (int i = 0; i < NUM_INPUT_DEVICE_CATEGORIES; i++)
		{
			m_pInputDevices[i] = NULL;
		}
	}

	~CInputDevicesDX8()
	{
		for (int i = 0; i < NUM_INPUT_DEVICE_CATEGORIES; i++)
		{
			if (m_pInputDevices[i] != NULL)
			{
				m_pInputDevices[i]->Release();
			}
		}
	}

	inline LPDIRECTINPUTDEVICE8 operator[](enum InputDeviceCategory devCat) const
	{
		return m_pInputDevices[devCat];
	}

	inline void SetDevice(enum InputDeviceCategory devCat, LPDIRECTINPUTDEVICE8 pDIDevice)
	{
		if (pDIDevice != NULL)
		{
			pDIDevice->AddRef();
		}

		if (m_pInputDevices[devCat] != NULL)
		{
			m_pInputDevices[devCat]->Release();
		}

		m_pInputDevices[devCat] = pDIDevice;
	}

	LPDIRECTINPUTDEVICE8 m_pInputDevices[NUM_INPUT_DEVICE_CATEGORIES];
};

};  // namespace ShapeShifter

#endif // #ifndef __INPUTDEVICE_H__
