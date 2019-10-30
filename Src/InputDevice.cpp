//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: InputDevice.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "InputDevice.h"
#include "Property.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// Property table for supporting late-bound properties

SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, MouseAbsolute)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, MouseFingerStick)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, MouseTouchPad)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, MouseTrackBall)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, MouseTraditional)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardPCXT)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardOlivetti)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardPCAT)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardPCEnh)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardNokia1050)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardNokia9140)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardNEC98)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardNEC98Laptop)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardNEC98106)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardJapan106)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardJapanAx)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, KeyboardJ3100)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, JoystickLimited)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, JoystickStandard)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, GamepadLimited)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, GamepadStandard)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, GamepadTilt)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, FlightLimited)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, FlightRC)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, FlightStick)
SSCL_DEFINE_PROPERTY_BOOL_READONLY_ACCESSOR(CInputDevice, FlightYoke)

SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(CInputDevice)
	SSCL_RUNTIME_PROPERTY_ENTRY(MouseAbsolute, SSCLID_MOUSE_ABSOLUTE, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(MouseFingerStick, SSCLID_MOUSE_FINGERSTICK, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(MouseTouchPad, SSCLID_MOUSE_TOUCHPAD, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(MouseTrackBall, SSCLID_MOUSE_TRACKBALL, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(MouseTraditional, SSCLID_MOUSE_TRADITIONAL, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardPCXT, SSCLID_KEYBOARD_PCXT, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardOlivetti, SSCLID_KEYBOARD_OLIVETTI, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardPCAT, SSCLID_KEYBOARD_PCAT, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardPCEnh, SSCLID_KEYBOARD_PCENH, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardNokia1050, SSCLID_KEYBOARD_NOKIA1050, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardNokia9140, SSCLID_KEYBOARD_NOKIA9140, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardNEC98, SSCLID_KEYBOARD_NEC98, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardNEC98Laptop, SSCLID_KEYBOARD_NECLAPTOP, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardNEC98106, SSCLID_KEYBOARD_NEC98106, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardJapan106, SSCLID_KEYBOARD_JAPAN106, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardJapanAx, SSCLID_KEYBOARD_JAPANAX, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(KeyboardJ3100, SSCLID_KEYBOARD_J3100, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(JoystickLimited, SSCLID_JOYSTICK_LIMITED, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(JoystickStandard, SSCLID_JOYSTICK_STANDARD, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(GamepadLimited, SSCLID_GAMEPAD_LIMITED, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(GamepadStandard, SSCLID_GAMEPAD_STANDARD, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(GamepadTilt, SSCLID_GAMEPAD_TILT, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(FlightLimited, SSCLID_FLIGHT_LIMITED, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(FlightRC, SSCLID_FLIGHT_RC, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(FlightStick, SSCLID_FLIGHT_STICK, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY(FlightYoke, SSCLID_FLIGHT_YOKE, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY_NULL
SSCL_END_RUNTIME_PROPERTY_TABLE

/////////////////////////////////////////////////////////////////////////////
// CInputDevice

CInputDevice::CInputDevice() :
	m_pDI8(NULL),
	m_pDevice(NULL)
{
	memset(&m_deviceInfo, 0, sizeof(DIDEVICEINSTANCE));
	m_deviceInfo.dwSize = sizeof(DIDEVICEINSTANCE);
}

CInputDevice::~CInputDevice()
{
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
	}

	if (m_pDI8 != NULL)
	{
		m_pDI8->Release();
	}
}

STDMETHODIMP CInputDevice::_InitObject(LPDIRECTINPUT8 pDI8, LPCDIDEVICEINSTANCE pDeviceInfo)
{
	if (pDI8 == NULL)
	{
		return E_POINTER;
	}

	if (m_pDI8 != NULL)
	{
		return E_FAIL;
	}

	memcpy(&m_deviceInfo, pDeviceInfo, sizeof(DIDEVICEINSTANCE));
	m_pDI8 = pDI8;
	m_pDI8->AddRef();

	return S_OK;
}

STDMETHODIMP CInputDevice::GetDIDevice(/* [out,retval] */ IUnknown** ppDIDevice)
{
	if (ppDIDevice == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_pDevice == NULL)
	{
		hr = CreateDIDevice(&m_pDevice);

		if (SUCCEEDED(hr))
		{
			hr = m_pDevice->QueryInterface(IID_IUnknown, (LPVOID*) ppDIDevice);
		}
	}
	else
	{
		hr = m_pDevice->QueryInterface(IID_IUnknown, (LPVOID*) ppDIDevice);
	}

	return hr;
}

STDMETHODIMP CInputDevice::get_DeviceCategory(/* [out,retval] */ enum InputDeviceCategory *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	DWORD dwPrimaryType = GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType);

	if (dwPrimaryType == DI8DEVTYPE_KEYBOARD)
	{
		*pVal = SSCL_KEYBOARD;
	}
	else if (dwPrimaryType == DI8DEVTYPE_MOUSE)
	{
		*pVal = SSCL_MOUSE;
	}
	else if (dwPrimaryType == DI8DEVTYPE_JOYSTICK)
	{
		*pVal = SSCL_JOYSTICK;
	}
	else if (dwPrimaryType == DI8DEVTYPE_GAMEPAD)
	{
		*pVal = SSCL_GAMEPAD;
	}
	else if (dwPrimaryType == DI8DEVTYPE_FLIGHT)
	{
		*pVal = SSCL_FLIGHT;
	}
	else if (dwPrimaryType == DI8DEVTYPE_DRIVING)
	{
		*pVal = SSCL_DRIVING;
	}
	else if (dwPrimaryType == DI8DEVTYPE_1STPERSON)
	{
		*pVal = SSCL_1ST_PERSON;
	}
	else if (dwPrimaryType == DI8DEVTYPE_SCREENPOINTER)
	{
		*pVal = SSCL_SCREEN_POINTER;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

STDMETHODIMP CInputDevice::get_DeviceType(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(m_deviceInfo.dwDevType);

	return S_OK;
}

STDMETHODIMP CInputDevice::get_DeviceName(/* [out,retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_bstr_t strDevName(m_deviceInfo.tszInstanceName);
	*pVal = strDevName.copy();

	return S_OK;
}

STDMETHODIMP CInputDevice::get_ProductName(/* [out,retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_bstr_t strProductName(m_deviceInfo.tszProductName);
	*pVal = strProductName.copy();

	return S_OK;
}

STDMETHODIMP CInputDevice::get_Axes(/* [out,retval] */ short *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	DIDEVCAPS caps;

	if (m_pDevice == NULL)
	{
		hr = CreateDIDevice(&m_pDevice);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDevice->GetCapabilities(&caps);

		if (SUCCEEDED(hr))
		{
			*pVal = static_cast<short>(caps.dwAxes);
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CInputDevice::get_Buttons(/* [out,retval] */ short *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	DIDEVCAPS caps;

	if (m_pDevice == NULL)
	{
		hr = CreateDIDevice(&m_pDevice);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDevice->GetCapabilities(&caps);

		if (SUCCEEDED(hr))
		{
			*pVal = static_cast<short>(caps.dwButtons);
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CInputDevice::get_POVs(/* [out,retval] */ short *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	DIDEVCAPS caps;

	if (m_pDevice == NULL)
	{
		hr = CreateDIDevice(&m_pDevice);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDevice->GetCapabilities(&caps);

		if (SUCCEEDED(hr))
		{
			*pVal = static_cast<short>(caps.dwPOVs);
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CInputDevice::get_FFSamplePeriod(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	DIDEVCAPS caps;

	if (m_pDevice == NULL)
	{
		hr = CreateDIDevice(&m_pDevice);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDevice->GetCapabilities(&caps);

		if (SUCCEEDED(hr))
		{
			*pVal = static_cast<long>(caps.dwFFSamplePeriod);
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CInputDevice::get_FFMinTimeResolution(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	DIDEVCAPS caps;

	if (m_pDevice == NULL)
	{
		hr = CreateDIDevice(&m_pDevice);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDevice->GetCapabilities(&caps);

		if (SUCCEEDED(hr))
		{
			*pVal = static_cast<long>(caps.dwFFMinTimeResolution);
			hr = S_OK;
		}
	}

	return hr;
}

HRESULT CInputDevice::get_KeyboardPCXT(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_PCXT)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardOlivetti(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_OLIVETTI)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardPCAT(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_PCAT)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardPCEnh(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_PCENH)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardNokia1050(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_NOKIA1050)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardNokia9140(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_NOKIA9140)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardNEC98(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_NEC98)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardNEC98Laptop(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_NEC98LAPTOP)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardNEC98106(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_NEC98106)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardJapan106(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_JAPAN106)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardJapanAx(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_JAPANAX)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_KeyboardJ3100(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_KEYBOARD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEKEYBOARD_J3100)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_MouseAbsolute(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_MOUSE &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEMOUSE_ABSOLUTE)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_MouseFingerStick(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_MOUSE &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEMOUSE_FINGERSTICK)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_MouseTouchPad(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_MOUSE &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEMOUSE_TOUCHPAD)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_MouseTrackBall(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_MOUSE &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEMOUSE_TRACKBALL)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_MouseTraditional(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_MOUSE &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEMOUSE_TRADITIONAL)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_JoystickLimited(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_JOYSTICK &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEJOYSTICK_LIMITED)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_JoystickStandard(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_JOYSTICK &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEJOYSTICK_STANDARD)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_GamepadLimited(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_GAMEPAD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEGAMEPAD_LIMITED)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_GamepadStandard(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_GAMEPAD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEGAMEPAD_STANDARD)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_GamepadTilt(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_GAMEPAD &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEGAMEPAD_TILT)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_FlightLimited(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_FLIGHT &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEFLIGHT_LIMITED)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_FlightRC(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_FLIGHT &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEFLIGHT_RC)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_FlightStick(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_FLIGHT &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEFLIGHT_STICK)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::get_FlightYoke(VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType) == DI8DEVTYPE_FLIGHT &&
	    GET_DIDEVICE_SUBTYPE(m_deviceInfo.dwDevType) == DI8DEVTYPEFLIGHT_YOKE)
	{
		*pVal = TRUE;
	}
	else
	{
		*pVal = FALSE;
	}

	return S_OK;
}

HRESULT CInputDevice::CreateDIDevice(LPDIRECTINPUTDEVICE8* ppDIDevice)
{
	if (ppDIDevice == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;
	*ppDIDevice = NULL;

	if (m_pDI8 != NULL)
	{
		hr = m_pDI8->CreateDevice(m_deviceInfo.guidInstance, ppDIDevice, NULL);

		if (SUCCEEDED(hr))
		{
			switch (GET_DIDEVICE_TYPE(m_deviceInfo.dwDevType))
			{
			case DI8DEVTYPE_KEYBOARD:
				hr = (*ppDIDevice)->SetDataFormat(&c_dfDIKeyboard);
				break;

			case DI8DEVTYPE_MOUSE:
				hr = (*ppDIDevice)->SetDataFormat(&c_dfDIMouse);
				break;
			}
		}
	}

	return hr;
}

};  // namespace ShapeShifter
