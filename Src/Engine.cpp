//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Engine.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "ShapeShifter.h"
#include "Engine.h"
#include "Device.h"
#include "InputDevice.h"
#include "Internet.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CEngine

CEngine::CEngine() :
	m_pD3D8(NULL),
	m_pDI8(NULL),
	m_bInternetEnabled(false)
{
}

void CEngine::FinalRelease()
{
	InternetEndSession();
	FreeInputDevices();

	if (m_pDI8 != NULL)
	{
		m_pDI8->Release();
	}

	if (m_pD3D8 != NULL)
	{
		m_pD3D8->Release();
	}
}

BOOL CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	CEngine* pEngine = (CEngine*) pvRef;
	pEngine->RegisterInputDevice(lpddi);
	return DIENUM_CONTINUE;
}

STDMETHODIMP CEngine::InitializeEngine()
{
	HRESULT hr = S_OK;

	if (m_pD3D8 == NULL && m_pDI8 == NULL)
	{
		m_pD3D8 = Direct3DCreate8(D3D_SDK_VERSION);

		if (m_pD3D8 != NULL)
		{
			hr = S_OK;
		}
		else
		{
			hr = Error::E_DIRECTX_INIT;
		}

		if (hr == S_OK)
		{
			hr = DirectInput8Create(_Module.m_hInst, DIRECTINPUT_VERSION,
			                        IID_IDirectInput8, (VOID**)&m_pDI8, NULL);

			if (SUCCEEDED(hr))
			{
				m_pDI8->EnumDevices(DI8DEVCLASS_ALL, DIEnumDevicesCallback, (CEngine*) this, DIEDFL_ALLDEVICES);
				hr = S_OK;
			}
			else
			{
				hr = Error::E_DIRECTX_INIT;
			}
		}

		if (hr == S_OK)
		{
			if (m_bInternetEnabled)
			{
				hr = InternetStartSession(m_strProxyName, m_strProxyBypass);

				if (FAILED(hr))
				{
					hr = Error::E_WININET_INIT;
				}
				else
				{
					hr = S_OK;
				}
			}
		}
	}
	else
	{
		hr  = Error::E_ENGINE_ALREADY_INIT;
	}

	if (FAILED(hr))
	{
		Error::SetInfo(CLSID_Engine, IID_IEngine, hr);
	}

	return hr;
}

STDMETHODIMP CEngine::GetD3DEngine(/* [out,retval] */ IUnknown** ppUnk)
{
	if (ppUnk == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	*ppUnk = m_pD3D8;

	if (*ppUnk != NULL)
	{
		(*ppUnk)->AddRef();
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}

STDMETHODIMP CEngine::GetDeviceCount(/* [out,retval] */ short* pCount)
{
	if (pCount == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_pD3D8 != NULL)
	{
		*pCount = (short) m_pD3D8->GetAdapterCount();
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CEngine::GetDevice(/* [in] */ short nDeviceIdx,
                                /* [out,retval] */ IDevice** ppDevice)
{
	if (ppDevice == NULL)
	{
		return E_POINTER;
	}

	if (m_pD3D8 == NULL)
	{
		return E_FAIL;
	}

	if (nDeviceIdx >= (short) m_pD3D8->GetAdapterCount())
	{
		return E_INVALIDARG;
	}

	HRESULT hr = E_FAIL;
	*ppDevice = NULL;

	_DeviceMap::iterator itDev = m_devices.find(nDeviceIdx);

	if (itDev != m_devices.end())
	{
		*ppDevice = itDev->second;
		(*ppDevice)->AddRef();
		hr = S_OK;
	}
	else
	{
		hr = CDevice::_CreatorClass::CreateInstance(NULL, IID_IDevice, (LPVOID*) ppDevice);

		if (SUCCEEDED(hr))
		{
			hr = (*ppDevice)->_InitObject(this, nDeviceIdx);
		}
	}

	return hr;
}

STDMETHODIMP CEngine::GetDefaultDevice(/* [out,retval] */ IDevice** ppDevice)
{
	return GetDevice(D3DADAPTER_DEFAULT, ppDevice);
}

STDMETHODIMP CEngine::GetInputDeviceCount(/* [in] */ enum InputDeviceCategory devCat,
                                          /* [out,retval] */ short* pCount)
{
	if (pCount == NULL)
	{
		return E_POINTER;
	}

	*pCount = 0;

	_InputDevices::iterator itInputDev = m_inputDevices.begin();
	enum InputDeviceCategory curDevCat;

	while (itInputDev != m_inputDevices.end())
	{
		(*itInputDev)->get_DeviceCategory(&curDevCat);
		if (curDevCat == devCat)
		{
			*pCount++;
		}
		itInputDev++;
	}

	return S_OK;
}

STDMETHODIMP CEngine::GetInputDevice(/* [in] */ enum InputDeviceCategory devCat,
                                     /* [in] */ short nInputDevIdx,
                                     /* [out,retval] */ IInputDevice** ppInputDev)
{
	if (ppInputDev == NULL)
	{
		return E_POINTER;
	}

	*ppInputDev = NULL;

	HRESULT hr = S_FALSE;
	short nCount = 0;
	_InputDevices::iterator itInputDev = m_inputDevices.begin();
	enum InputDeviceCategory curDevCat;

	while (itInputDev != m_inputDevices.end() && *ppInputDev == NULL)
	{
		(*itInputDev)->get_DeviceCategory(&curDevCat);

		if (curDevCat == devCat)
		{
			if (nCount == nInputDevIdx)
			{
				*ppInputDev = (*itInputDev);
				(*ppInputDev)->AddRef();
				hr = S_OK;
			}
			else
			{
				nCount++;
			}
		}
		itInputDev++;
	}

	return hr;
}

STDMETHODIMP CEngine::get_Direct3DVersion(/* [out, retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}
	*pVal = D3D_SDK_VERSION;
	return S_OK;
}

HRESULT CEngine::get_InternetEnabled(/* [out, retval] */ VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_bInternetEnabled ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

HRESULT CEngine::put_InternetEnabled(/* [in] */ VARIANT_BOOL newVal)
{
	m_bInternetEnabled = newVal ? true : false;
	return S_OK;
}

HRESULT CEngine::get_ProxyName(/* [out, retval] */ BSTR* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_strProxyName.copy();

	return S_OK;
}

HRESULT CEngine::put_ProxyName(/* [in] */ BSTR newVal)
{
	m_strProxyName = newVal;
	return S_OK;
}

HRESULT CEngine::get_ProxyBypass(/* [out, retval] */ BSTR* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_strProxyBypass.copy();

	return S_OK;
}

HRESULT CEngine::put_ProxyBypass(/* [in] */ BSTR newVal)
{
	m_strProxyBypass = newVal;
	return S_OK;
}

STDMETHODIMP CEngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEngine
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CEngine::RegisterInputDevice(LPCDIDEVICEINSTANCE pDeviceInfo)
{
	IInputDevice* pInputDev = NULL;
	HRESULT hr = CInputDevice::_CreatorClass::CreateInstance(NULL, IID_IInputDevice, (LPVOID*) &pInputDev);
	if (SUCCEEDED(hr))
	{
		IInitInputDeviceDX8* pInitInputDev = NULL;
		hr = pInputDev->QueryInterface(IID_IInitInputDeviceDX8, (LPVOID*) &pInitInputDev);
		if (SUCCEEDED(hr))
		{
			hr = pInitInputDev->_InitObject(m_pDI8, pDeviceInfo);
			pInitInputDev->Release();
		}

		if (SUCCEEDED(hr))
		{
			pInputDev->AddRef();
			m_inputDevices.push_back(pInputDev);
		}

		pInputDev->Release();
	}

	return hr;
}

void CEngine::FreeInputDevices()
{
	_InputDevices::iterator itInputDev = m_inputDevices.begin();

	while (itInputDev != m_inputDevices.end())
	{
		(*itInputDev)->Release();
		itInputDev++;
	}

	m_inputDevices.clear();
}

};  // namespace ShapeShifter
