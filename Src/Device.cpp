//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Device.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Device.h"
#include "Viewport.h"
#include "DevicePresentation.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CDevice

CDevice::CDevice() :
	m_pD3D8(NULL),
	m_nDeviceIdx(0),
	m_driverMode(D3DDEVTYPE_HAL),
	m_pD3DDevice8(NULL),
	m_pDepthStencil(NULL)
{
}

CDevice::~CDevice()
{
	m_presentations.clear();

	FreeAllResources(D3DRTYPE_FORCE_DWORD);

	if (m_pD3DDevice8 != NULL)
	{
		m_pD3DDevice8->Release();
	}

	if (m_pD3D8 != NULL)
	{
		m_pD3D8->Release();
	}
}

STDMETHODIMP CDevice::_InitObject(/* [in] */ IEngine* pEngine, /* [in] */ short nDeviceIdx)
{
	if (pEngine == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	m_nDeviceIdx = nDeviceIdx;

	if (m_pD3D8 != NULL)
	{
		m_pD3D8->Release();
		m_pD3D8 = NULL;
	}

	IUnknown* pD3D8Unk = NULL;
	hr = pEngine->GetD3DEngine(&pD3D8Unk);

	if (hr == S_OK)
	{
		hr = pD3D8Unk->QueryInterface(IID_IDirect3D8, (LPVOID*) &m_pD3D8);
		pD3D8Unk->Release();
	}

	if (SUCCEEDED(hr) && m_pD3D8 != NULL)
	{
		hr = m_pD3D8->GetAdapterIdentifier(m_nDeviceIdx, 0, &m_adapterId);
	}

	if (SUCCEEDED(hr))
	{
		hr = GetCurrentDisplayModeIndex(m_nDisplayMode);
	}

	return hr;
}

STDMETHODIMP CDevice::Initialize(/* [in] */ long focusWnd)
{
	if (m_pD3DDevice8 != NULL)
	{
		return S_FALSE;
	}

	if (m_driverMode == D3DDEVTYPE_FORCE_DWORD)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	D3DPRESENT_PARAMETERS d3dpp;

	if (m_pD3D8 == NULL)
	{
		return E_UNEXPECTED;
	}

	// Create iterator for presentation buffer list. The first
	// entry in the list is the primary presentation buffer,
	// which corresponds to the device's implicit swap chain.
	_DevicePresentationList::iterator itPres = m_presentations.begin();

	if (itPres != m_presentations.end())
	{
		// Use the primary presentation buffer parameters
		// for the implicit swap chain.
		(*itPres)->GetPresentationParameters(&d3dpp);
	}
	else
	{
		// No presentation buffers have been added to the device
		// yet.  Use default presentation parameters for the
		// implicit swap chain.
		ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
	}

	// Update parameters with device-wide settings
	GetDeviceParameters(&d3dpp);

	// Create the device (and implicit swap chain)
	hr = m_pD3D8->CreateDevice(m_nDeviceIdx,
		  					   m_driverMode,
							   (HWND) focusWnd,
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							   &d3dpp,
							   &m_pD3DDevice8);

	if (hr == D3D_OK)
	{
		// Create a depth stencil to be shared by all device presentations.

		if (m_pDepthStencil != NULL)
		{
			m_pDepthStencil->Release();
			m_pDepthStencil = NULL;
		}

		D3DDISPLAYMODE dispMode;
		if (SUCCEEDED(m_pD3DDevice8->GetDisplayMode(&dispMode)))
		{
			hr = m_pD3DDevice8->CreateDepthStencilSurface(dispMode.Width,
			                                              dispMode.Height,
			                                              D3DFMT_D16,
			                                              d3dpp.MultiSampleType,
			                                              &m_pDepthStencil);
		}
		else
		{
			hr = E_UNEXPECTED;
		}
	}

	if (hr == D3D_OK)
	{
		// Initialize the presentation buffers. The first one in the list
		// is the primary buffer, which corresponds to the device's implicit
		// swap chain. The remaining buffers use swap chains created by
		// IDirect3DDevice8::CreateAdditionalSwapChain().

		if (itPres != m_presentations.end())
		{
			(*itPres)->SetPresentationBuffers(m_pD3DDevice8, NULL, m_pDepthStencil);
			(*itPres)->SetPresentationParameters(&d3dpp);
			itPres++;

			// Initialize the remaining presentation buffers
			// by creating swap chains for the device
			while (SUCCEEDED(hr) && itPres != m_presentations.end())
			{
				hr = InitPresentation(*itPres);
				itPres++;
			}
		}
	}

	return hr;
}

STDMETHODIMP CDevice::GetD3DDevice(/* [out,retval] */ IUnknown** ppUnk)
{
	if (ppUnk == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	*ppUnk = m_pD3DDevice8;

	if (*ppUnk != NULL)
	{
		(*ppUnk)->AddRef();
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::CreateViewport(/* [in] */ VARIANT varProgId,
                                     /* [out,retval] */ IViewport** ppViewport)
{
	if (ppViewport == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	*ppViewport = NULL;

	if (varProgId.vt == VT_EMPTY)
	{
		// No type information supplied. Use default viewport class.
		hr = CViewport::_CreatorClass::CreateInstance(NULL, IID_IViewport, (LPVOID*) ppViewport);
	}
	else if (varProgId.vt == VT_BSTR && varProgId.bstrVal != NULL &&
	         ::SysStringLen(varProgId.bstrVal) > 0)
	{
		// Create device from class ID passed in.
		CLSID viewportClass;
		hr = CLSIDFromProgID(varProgId.bstrVal, &viewportClass);

		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(viewportClass, NULL, CLSCTX_INPROC_SERVER, IID_IViewport, (LPVOID*) ppViewport);
		}
	}
	else
	{
		hr = E_INVALIDARG;
	}

	if (SUCCEEDED(hr) && *ppViewport != NULL)
	{
		hr = (*ppViewport)->_InitObject(this);
	}

	return hr;
}

STDMETHODIMP CDevice::GetDisplayModeCount(/* [out,retval] */ short* pNumModes)
{
	if (pNumModes == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_pD3D8 != NULL)
	{
		*pNumModes = (short) m_pD3D8->GetAdapterModeCount(m_nDeviceIdx);
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::GetDisplayModeInfo(/* [in] */ short nModeIdx,
                                         /* [out,retval] */ VARIANT* pDispModeInfo)
{
	if (pDispModeInfo == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;
	D3DDISPLAYMODE d3dDispMode;

	if (m_pD3D8 != NULL)
	{
		hr = m_pD3D8->EnumAdapterModes(m_nDeviceIdx, (UINT) nModeIdx, &d3dDispMode);

		if (hr == D3D_OK)
		{
			vtDISPLAYMODEINFO varDispModeInfo;
			varDispModeInfo.Width = (short) d3dDispMode.Width;
			varDispModeInfo.Height = (short) d3dDispMode.Height;
			varDispModeInfo.RefreshRate = (short) d3dDispMode.RefreshRate;
			varDispModeInfo.Format = (short) d3dDispMode.Format;
			*pDispModeInfo = varDispModeInfo;
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CDevice::GetDeviceGUID(/* [out] */ GUID* pDeviceGuid)
{
	if (pDeviceGuid == NULL)
	{
		return E_POINTER;
	}

	*pDeviceGuid = m_adapterId.DeviceIdentifier;

	return S_OK;
}

STDMETHODIMP CDevice::GetMonitor(/* [out] */ long* pMonitorHandle)
{
	if (pMonitorHandle == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_pD3D8 != NULL)
	{
		*pMonitorHandle = (long) m_pD3D8->GetAdapterMonitor(m_nDeviceIdx);
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::get_DisplayMode(/* [out,retval] */ short *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_nDisplayMode;

	return S_OK;
}

STDMETHODIMP CDevice::put_DisplayMode(/* [in] */ short newVal)
{
	if (m_pD3DDevice8 != NULL && m_nDisplayMode != newVal)
	{
		return E_FAIL;
	}

	m_nDisplayMode = newVal;

	return S_OK;
}

STDMETHODIMP CDevice::get_DriverMode(/* [out, retval] */ enum DriverMode *pMode)
{
	if (pMode == NULL)
	{
		return E_POINTER;
	}

	switch (m_driverMode)
	{
	case D3DDEVTYPE_HAL:
		*pMode = SSCL_HAL;
		break;

	case D3DDEVTYPE_REF:
		*pMode = SSCL_REF;
		break;

	case D3DDEVTYPE_SW:
		*pMode = SSCL_SW;
		break;
	}

	return S_OK;
}

STDMETHODIMP CDevice::put_DriverMode(/* [in] */ enum DriverMode mode)
{
	if (m_pD3DDevice8 != NULL)
	{
		return E_FAIL;
	}

	switch (mode)
	{
	case SSCL_HAL:
		m_driverMode = D3DDEVTYPE_HAL;
		break;

	case SSCL_REF:
		m_driverMode = D3DDEVTYPE_REF;
		break;

	case SSCL_SW:
		m_driverMode = D3DDEVTYPE_SW;
		break;
	}

	return S_OK;
}

STDMETHODIMP CDevice::get_FocusWindow(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;
	D3DDEVICE_CREATION_PARAMETERS parms;

	if (m_pD3DDevice8 != NULL)
	{
		hr = m_pD3DDevice8->GetCreationParameters(&parms);
		if (SUCCEEDED(hr))
		{
			*pVal = (long) parms.hFocusWindow;
		}
	}

	return hr;
}

STDMETHODIMP CDevice::get_DriverName(/* [out,retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_bstr_t strVal(m_adapterId.Driver);
	*pVal = strVal.copy();

	return S_OK;
}

STDMETHODIMP CDevice::get_DriverDesc(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_bstr_t strVal(m_adapterId.Description);
	*pVal = strVal.copy();

	return S_OK;
}

STDMETHODIMP CDevice::get_Product(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(HIWORD(m_adapterId.DriverVersion.HighPart));

	return S_OK;
}

STDMETHODIMP CDevice::get_Version(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(LOWORD(m_adapterId.DriverVersion.HighPart));

	return S_OK;
}

STDMETHODIMP CDevice::get_SubVersion(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(HIWORD(m_adapterId.DriverVersion.LowPart));

	return S_OK;
}

STDMETHODIMP CDevice::get_Build(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(LOWORD(m_adapterId.DriverVersion.LowPart));

	return S_OK;
}

STDMETHODIMP CDevice::get_VendorId(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(m_adapterId.VendorId);

	return S_OK;
}

STDMETHODIMP CDevice::get_DeviceId(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(m_adapterId.DeviceId);

	return S_OK;
}

STDMETHODIMP CDevice::get_SubsysId(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(m_adapterId.SubSysId);

	return S_OK;
}

STDMETHODIMP CDevice::get_Revision(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(m_adapterId.Revision);

	return S_OK;
}

STDMETHODIMP CDevice::get_WHQLLevel(/* [out,retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = static_cast<long>(m_adapterId.WHQLLevel);

	return S_OK;
}

STDMETHODIMP CDevice::AddPresentation(IDevicePresentationDX8* pPresentation)
{
	if (pPresentation == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	if (m_pD3DDevice8 != NULL)
	{
		hr = InitPresentation(pPresentation);
	}

	if (hr == S_OK)
	{
		// Append the presentation to the list
		m_presentations.push_back(pPresentation);  // Not AddRef'd to avoid circular references
	}

	return hr;
}

STDMETHODIMP CDevice::RemovePresentation(IDevicePresentationDX8* pPresentation)
{
	if (pPresentation == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;
	IUnknown* pPresUnk = NULL;

	if (FAILED(pPresentation->QueryInterface(IID_IUnknown, (LPVOID*) &pPresUnk)))
	{
		return E_UNEXPECTED;
	}

	_DevicePresentationList::iterator itPres = m_presentations.begin();

	while (hr == S_FALSE && itPres != m_presentations.end())
	{
		IUnknown* pCurPresUnk = NULL;
		hr = (*itPres)->QueryInterface(IID_IUnknown, (LPVOID*) &pCurPresUnk);
		if (SUCCEEDED(hr))
		{
			if (pCurPresUnk == pPresUnk)
			{
				hr = S_OK;
				m_presentations.erase(itPres);  // Not ref counted to avoid circular references
			}
			pCurPresUnk->Release();
		}
		itPres++;
	}

	pPresUnk->Release();

	return hr;
}

STDMETHODIMP CDevice::InitPresentation(IDevicePresentationDX8* pPresentation)
{
	if (pPresentation == NULL)
	{
		return E_POINTER;
	}

	if (m_pD3DDevice8 == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;

	try
	{
		D3DPRESENT_PARAMETERS d3dpp;

		// Get the presentation parameters from the presentation object
		if (FAILED(pPresentation->GetPresentationParameters(&d3dpp)))
		{
			_com_issue_error(E_FAIL);
		}

		// Merge the device settings with the presentation parameters
		if (FAILED(GetDeviceParameters(&d3dpp)))
		{
			_com_issue_error(E_FAIL);
		}

		// Copy the merged presentation parameters back to the presentation
		// object so that it is synched up with the device.
		if (FAILED(pPresentation->SetPresentationParameters(&d3dpp)))
		{
			_com_issue_error(E_FAIL);
		}

		if (m_presentations.size() == 0)
		{
			// First presentation added always uses the device's
			// implicit swap chain
			hr = pPresentation->SetPresentationBuffers(m_pD3DDevice8, NULL, m_pDepthStencil);

			if (SUCCEEDED(hr))
			{
				// All device resources must be released before the device is
				// reset, otherwise reset will fail.
				FreeAllResources(D3DRTYPE_FORCE_DWORD);

				// Hold on tight! Freaky things will probably happen now...
				if (FAILED(m_pD3DDevice8->Reset(&d3dpp)))
				{
					_com_issue_error(E_FAIL);
				}
			}
		}
		else
		{
			// Create a new swap chain on the device for this
			// presentation to use
			LPDIRECT3DSWAPCHAIN8 pSwapChain = NULL;
			hr = m_pD3DDevice8->CreateAdditionalSwapChain(&d3dpp, &pSwapChain);

			if (hr == D3D_OK)
			{
				hr = pPresentation->SetPresentationBuffers(m_pD3DDevice8, pSwapChain, m_pDepthStencil);
				pSwapChain->Release();
			}
		}
	}
	catch (_com_error& e)
	{
		hr = e.Error();
	}

	return hr;
}

STDMETHODIMP CDevice::GetDeviceParameters(D3DPRESENT_PARAMETERS* pPresentParams)
{
	if (m_pD3D8 == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;

	// Update the presentation to be compatible with
	// the overriding device settings.

	D3DDISPLAYMODE d3dDispMode;
	pPresentParams->EnableAutoDepthStencil = FALSE;
	pPresentParams->AutoDepthStencilFormat = D3DFMT_D16;

	if (pPresentParams->Windowed)
	{
		// Windowed mode must use current display mode settings
		if (m_pD3D8->GetAdapterDisplayMode(m_nDeviceIdx, &d3dDispMode) == D3D_OK)
		{
			pPresentParams->BackBufferFormat = d3dDispMode.Format;
			hr = S_OK;
		}
	}
	else
	{
		// Get the settings for the selected display mode and apply to presentation parameters
		if (m_pD3D8->EnumAdapterModes(m_nDeviceIdx, m_nDisplayMode, &d3dDispMode) == D3D_OK)
		{
			pPresentParams->BackBufferWidth = d3dDispMode.Width;
			pPresentParams->BackBufferHeight = d3dDispMode.Height;
			pPresentParams->BackBufferFormat = d3dDispMode.Format;
			pPresentParams->FullScreen_RefreshRateInHz = d3dDispMode.RefreshRate;
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CDevice::GetD3DDevice8(LPDIRECT3DDEVICE8* ppD3DDevice8)
{
	if (ppD3DDevice8 == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	*ppD3DDevice8 = m_pD3DDevice8;

	if (*ppD3DDevice8 != NULL)
	{
		(*ppD3DDevice8)->AddRef();
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::GenerateResourceId(D3DRESOURCETYPE resType, DeviceResourceId* pResId)
{
	HRESULT hr = S_FALSE;
	UINT nNumFreeIds = 0;

	if (pResId == NULL)
	{
		return E_POINTER;
	}

	switch (resType)
	{
	case D3DRTYPE_TEXTURE:
		{
			// Check to see if there are any free resource IDs
			nNumFreeIds = m_freeTextureIds.GetElementCount();
			if (nNumFreeIds > 0)
			{
				// Grab the free resource ID at the end of the free list
				// then remove it from the list
				*pResId = m_freeTextureIds[nNumFreeIds-1];
				m_freeTextureIds.RemoveElements(nNumFreeIds-1, nNumFreeIds-1);
			}
			else
			{
				// Append an empty element to the array and use the index as
				// the resource ID
				*pResId = m_textures.AppendElement(NULL);
			}
			hr = S_OK;
		}
		break;

	case D3DRTYPE_VERTEXBUFFER:
		{
			// Check to see if there are any free resource IDs
			nNumFreeIds = m_freeVertexBufferIds.GetElementCount();
			if (nNumFreeIds > 0)
			{
				// Grab the free resource ID at the end of the free list
				// then remove it from the list
				*pResId = m_freeVertexBufferIds[nNumFreeIds-1];
				m_freeVertexBufferIds.RemoveElements(nNumFreeIds-1, nNumFreeIds-1);
			}
			else
			{
				// Append an empty element to the array and use the index as
				// the resource ID
				*pResId = m_vertexBuffers.AppendElement(NULL);
			}
			hr = S_OK;
		}
		break;

	case D3DRTYPE_INDEXBUFFER:
		{
			// Check to see if there are any free resource IDs
			nNumFreeIds = m_freeIndexBufferIds.GetElementCount();
			if (nNumFreeIds > 0)
			{
				// Grab the free resource ID at the end of the free list
				// then remove it from the list
				*pResId = m_freeIndexBufferIds[nNumFreeIds-1];
				m_freeIndexBufferIds.RemoveElements(nNumFreeIds-1, nNumFreeIds-1);
			}
			else
			{
				// Append an empty element to the array and use the index as
				// the resource ID
				*pResId = m_indexBuffers.AppendElement(NULL);
			}
			hr = S_OK;
		}
		break;
	}

	return hr;
}

STDMETHODIMP CDevice::ReserveResourceId(D3DRESOURCETYPE resType, DeviceResourceId nResId)
{
	HRESULT hr = S_FALSE;

	switch (resType)
	{
	case D3DRTYPE_TEXTURE:
		{
			// Check to see if the ID is in the list of free texture IDs.
			UINT idIdx;
			if (m_freeTextureIds.FindElement(nResId, idIdx))
			{
				// Resource ID found in free list. Just remove it from the list
				_ASSERTE(m_textures[nResId] == NULL);  // It better be NULL if it's free
				m_freeTextureIds.RemoveElements(idIdx, idIdx);
				hr = S_OK;
			}
			else
			{
				// Two possibilities exist now: either it's already reserved or the
				// resource ID is outside of the bounds of the current set of IDs.
				if (nResId < m_textures.GetElementCount())
				{
					// The resource ID is within the bounds of the current set of IDs.
					// Since it wasn't in the free list it must be reserved.
					hr = S_FALSE;
				}
				else
				{
					// The resource ID falls outside of the bounds of the current set
					// of IDs. Let's grow the array to include it and then reserve it
					// by excluding it from the free list.
					DeviceResourceId nNextFreeId = m_textures.GetElementCount();
					m_textures.GrowArray(nResId + 1);

					// Loop from first new ID up to but not including the requested
					// ID and add each ID to the free list.
					while (nNextFreeId < nResId)
					{
						m_freeTextureIds.AppendElement(nNextFreeId);
						m_textures[nNextFreeId] = NULL;
						nNextFreeId++;
					}

					// Make sure pointer is NULL
					m_textures[nResId] = NULL;

					// Indicate ID successfully reserved
					hr = S_OK;
				}
			}
		}
		break;

	case D3DRTYPE_VERTEXBUFFER:
		{
			// Check to see if the ID is in the list of free texture IDs.
			UINT idIdx;
			if (m_freeVertexBufferIds.FindElement(nResId, idIdx))
			{
				// Resource ID found in free list. Just remove it from the list
				_ASSERTE(m_vertexBuffers[nResId] == NULL);  // It better be NULL if it's free
				m_freeVertexBufferIds.RemoveElements(idIdx, idIdx);
				hr = S_OK;
			}
			else
			{
				// Two possibilities exist now: either it's already reserved or the
				// resource ID is outside of the bounds of the current set of IDs.
				if (nResId < m_vertexBuffers.GetElementCount())
				{
					// The resource ID is within the bounds of the current set of IDs.
					// Since it wasn't in the free list it must be reserved.
					hr = S_FALSE;
				}
				else
				{
					// The resource ID falls outside of the bounds of the current set
					// of IDs. Let's grow the array to include it and then reserve it
					// by excluding it from the free list.
					DeviceResourceId nNextFreeId = m_vertexBuffers.GetElementCount();
					m_vertexBuffers.GrowArray(nResId + 1);

					// Loop from first new ID up to but not including the requested
					// ID and add each ID to the free list.
					while (nNextFreeId < nResId)
					{
						m_freeVertexBufferIds.AppendElement(nNextFreeId);
						m_vertexBuffers[nNextFreeId] = NULL;
						nNextFreeId++;
					}

					// Make sure pointer is NULL
					m_vertexBuffers[nResId] = NULL;

					// Indicate ID successfully reserved
					hr = S_OK;
				}
			}
		}
		break;

	case D3DRTYPE_INDEXBUFFER:
		{
			// Check to see if the ID is in the list of free texture IDs.
			UINT idIdx;
			if (m_freeIndexBufferIds.FindElement(nResId, idIdx))
			{
				// Resource ID found in free list. Just remove it from the list
				_ASSERTE(m_vertexBuffers[nResId] == NULL);  // It better be NULL if it's free
				m_freeIndexBufferIds.RemoveElements(idIdx, idIdx);
				hr = S_OK;
			}
			else
			{
				// Two possibilities exist now: either it's already reserved or the
				// resource ID is outside of the bounds of the current set of IDs.
				if (nResId < m_indexBuffers.GetElementCount())
				{
					// The resource ID is within the bounds of the current set of IDs.
					// Since it wasn't in the free list it must be reserved.
					hr = S_FALSE;
				}
				else
				{
					// The resource ID falls outside of the bounds of the current set
					// of IDs. Let's grow the array to include it and then reserve it
					// by excluding it from the free list.
					DeviceResourceId nNextFreeId = m_indexBuffers.GetElementCount();
					m_indexBuffers.GrowArray(nResId + 1);

					// Loop from first new ID up to but not including the requested
					// ID and add each ID to the free list.
					while (nNextFreeId < nResId)
					{
						m_freeIndexBufferIds.AppendElement(nNextFreeId);
						m_indexBuffers[nNextFreeId] = NULL;
						nNextFreeId++;
					}

					// Make sure pointer is NULL
					m_indexBuffers[nResId] = NULL;

					// Indicate ID successfully reserved
					hr = S_OK;
				}
			}
		}
		break;
	}

	return hr;
}

STDMETHODIMP CDevice::SetSurface(DeviceResourceId nResId, LPDIRECT3DSURFACE8 pSurface)
{
	nResId;
	pSurface;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::GetSurface(DeviceResourceId nResId, LPDIRECT3DSURFACE8* ppSurface)
{
	nResId;
	ppSurface;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::SetVolume(DeviceResourceId nResId, LPDIRECT3DVOLUME8 pVolume)
{
	nResId;
	pVolume;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::GetVolume(DeviceResourceId nResId, LPDIRECT3DVOLUME8* ppVolume)
{
	nResId;
	ppVolume;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::SetTexture(DeviceResourceId nResId, LPDIRECT3DTEXTURE8 pTexture)
{
	if (pTexture == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	if (nResId < m_textures.GetElementCount())
	{
		if (m_textures[nResId] != NULL)
		{
			m_textures[nResId]->Release();
		}

		pTexture->AddRef();
		m_textures[nResId] = pTexture;

		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::GetTexture(DeviceResourceId nResId, LPDIRECT3DTEXTURE8* ppTexture)
{
	_ASSERTE(ppTexture != NULL);

	if (nResId < 0 || nResId >= m_textures.GetElementCount())
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_FALSE;

	*ppTexture = m_textures[nResId];

	if ((*ppTexture) != NULL)
	{
		(*ppTexture)->AddRef();
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::SetVolumeTexture(DeviceResourceId nResId, LPDIRECT3DVOLUMETEXTURE8 pVolTexture)
{
	nResId;
	pVolTexture;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::GetVolumeTexture(DeviceResourceId nResId, LPDIRECT3DVOLUMETEXTURE8* ppVolTexture)
{
	nResId;
	ppVolTexture;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::SetCubeTexture(DeviceResourceId nResId, LPDIRECT3DCUBETEXTURE8 pCubeTexture)
{
	nResId;
	pCubeTexture;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::GetCubeTexture(DeviceResourceId nResId, LPDIRECT3DCUBETEXTURE8* ppCubeTexture)
{
	nResId;
	ppCubeTexture;
	return E_NOTIMPL;
}

STDMETHODIMP CDevice::SetVertexBuffer(DeviceResourceId nResId, LPDIRECT3DVERTEXBUFFER8 pVB)
{
	if (pVB == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	if (nResId < m_vertexBuffers.GetElementCount())
	{
		if (m_vertexBuffers[nResId] != NULL)
		{
			m_vertexBuffers[nResId]->Release();
		}

		pVB->AddRef();
		m_vertexBuffers[nResId] = pVB;

		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::GetVertexBuffer(DeviceResourceId nResId, LPDIRECT3DVERTEXBUFFER8* ppVB)
{
	_ASSERTE(ppVB != NULL);

	if (nResId < 0 || nResId >= m_vertexBuffers.GetElementCount())
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_FALSE;

	*ppVB = m_vertexBuffers[nResId];

	if ((*ppVB) != NULL)
	{
		(*ppVB)->AddRef();
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::SetIndexBuffer(DeviceResourceId nResId, LPDIRECT3DINDEXBUFFER8 pIB)
{
	if (pIB == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	if (nResId < m_indexBuffers.GetElementCount())
	{
		if (m_indexBuffers[nResId] != NULL)
		{
			m_indexBuffers[nResId]->Release();
		}

		pIB->AddRef();
		m_indexBuffers[nResId] = pIB;

		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::GetIndexBuffer(DeviceResourceId nResId, LPDIRECT3DINDEXBUFFER8* ppIB)
{
	_ASSERTE(ppIB != NULL);

	if (nResId < 0 || nResId >= m_indexBuffers.GetElementCount())
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_FALSE;

	*ppIB = m_indexBuffers[nResId];

	if ((*ppIB) != NULL)
	{
		(*ppIB)->AddRef();
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CDevice::FreeResource(D3DRESOURCETYPE resType, DeviceResourceId nResId)
{
	HRESULT hr = E_FAIL;

	switch (resType)
	{
	case D3DRTYPE_TEXTURE:
		{
			if (nResId < m_textures.GetElementCount())
			{
				if (m_textures[nResId] != NULL)
				{
					m_textures[nResId]->Release();
					m_textures[nResId] = NULL;
				}

				UINT idIdx;
				if (!m_freeTextureIds.FindElement(nResId, idIdx))
				{
					m_freeTextureIds.AppendElement(nResId);
				}

				hr = S_OK;
			}
		}
		break;

	case D3DRTYPE_VERTEXBUFFER:
		{
			if (nResId < m_vertexBuffers.GetElementCount())
			{
				if (m_vertexBuffers[nResId] != NULL)
				{
					m_vertexBuffers[nResId]->Release();
					m_vertexBuffers[nResId] = NULL;
				}

				UINT idIdx;
				if (!m_freeVertexBufferIds.FindElement(nResId, idIdx))
				{
					m_freeVertexBufferIds.AppendElement(nResId);
				}

				hr = S_OK;
			}
		}
		break;

	case D3DRTYPE_INDEXBUFFER:
		{
			if (nResId < m_indexBuffers.GetElementCount())
			{
				if (m_indexBuffers[nResId] != NULL)
				{
					m_indexBuffers[nResId]->Release();
					m_indexBuffers[nResId] = NULL;
				}

				UINT idIdx;
				if (!m_freeIndexBufferIds.FindElement(nResId, idIdx))
				{
					m_freeIndexBufferIds.AppendElement(nResId);
				}

				hr = S_OK;
			}
		}
		break;
	}

	return hr;
}

STDMETHODIMP CDevice::FreeAllResources(D3DRESOURCETYPE resType)
{
	DeviceResourceId nCurId;

	if (resType == D3DRTYPE_TEXTURE || resType == D3DRTYPE_FORCE_DWORD)
	{
		for (nCurId = 0; nCurId < m_textures.GetElementCount(); nCurId++)
		{
			if (m_textures[nCurId] != NULL)
			{
				m_textures[nCurId]->Release();
			}
		}
		m_textures.ClearArray();
		m_freeTextureIds.ClearArray();
	}

	if (resType == D3DRTYPE_VERTEXBUFFER || resType == D3DRTYPE_FORCE_DWORD)
	{
		for (nCurId = 0; nCurId < m_vertexBuffers.GetElementCount(); nCurId++)
		{
			if (m_vertexBuffers[nCurId] != NULL)
			{
				m_vertexBuffers[nCurId]->Release();
			}
		}
		m_vertexBuffers.ClearArray();
		m_freeVertexBufferIds.ClearArray();
	}

	if (resType == D3DRTYPE_INDEXBUFFER || resType == D3DRTYPE_FORCE_DWORD)
	{
		for (nCurId = 0; nCurId < m_indexBuffers.GetElementCount(); nCurId++)
		{
			if (m_indexBuffers[nCurId] != NULL)
			{
				m_indexBuffers[nCurId]->Release();
			}
		}
		m_indexBuffers.ClearArray();
		m_freeIndexBufferIds.ClearArray();
	}

	// Free swap chains, back buffers, and depth
	// stencils held by device presentations
	_DevicePresentationList::iterator itPres = m_presentations.begin();
	while (itPres != m_presentations.end())
	{
		(*itPres)->FreePresentationBuffers();
	}

	// Release depth buffer
	if (m_pDepthStencil != NULL)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = NULL;
	}

	return S_OK;
}

HRESULT CDevice::GetCurrentDisplayModeIndex(short& nDisplayModeIdx)
{
	HRESULT hr = E_FAIL;
	UINT nCurModeIdx = 0;
	D3DDISPLAYMODE dispMode;
	D3DDISPLAYMODE dispModeCur;
	bool bFound = false;

	if (m_pD3D8 != NULL)
	{
		hr = m_pD3D8->GetAdapterDisplayMode(m_nDeviceIdx, &dispMode);

		while (hr == D3D_OK && nCurModeIdx < m_pD3D8->GetAdapterModeCount(m_nDeviceIdx) && !bFound)
		{
			hr = m_pD3D8->EnumAdapterModes(m_nDeviceIdx, nCurModeIdx, &dispModeCur);

			if (hr == D3D_OK)
			{
				if (dispModeCur.Width == dispMode.Width &&
					dispModeCur.Height == dispMode.Height &&
					dispModeCur.RefreshRate == dispMode.RefreshRate &&
					dispModeCur.Format == dispMode.Format)
				{
					bFound = true;
					nDisplayModeIdx = (short) nCurModeIdx;
					hr = S_OK;
				}
			}

			nCurModeIdx++;
		}
	}

	return hr;
}

};  // namespace ShapeShifter
