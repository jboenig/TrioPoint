//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ShapeShifterCtl.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "ShapeShifterCtl.h"
#include "ShapeShifterCP.h"
#include "Engine.h"
#include "Model.h"
#include "ModelReader3DS.h"
#include "AnimationLoop.h"
#include "AboutDlg.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CShapeShifterCtl

CShapeShifterCtl::CShapeShifterCtl() :
	m_pEngine(NULL),
	m_pDevice(NULL),
	m_bDeviceInitialized(false),
	m_pModel(NULL),
	m_pViewport(NULL),
	m_pController(NULL),
	m_pWndMsgRcvr(NULL),
	m_pAnimationLoop(NULL)
{
	m_bWindowOnly = TRUE;
	m_clsidModel = CLSID_Model;
	m_clsidViewport = CLSID_Viewport;
	m_clsidController = CLSID_BrowseController;
	m_clsidModelReader = CLSID_ModelReaderXML;
	m_clrBackColor = 0xFF0000;
	m_bAnimationEnabled = false;
	m_bAutoAspect = true;
}

CShapeShifterCtl::~CShapeShifterCtl()
{
	if (m_pAnimationLoop != NULL)
	{
		m_pAnimationLoop->Release();
	}

	if (m_pWndMsgRcvr != NULL)
	{
		m_pWndMsgRcvr->Release();
	}

	if (m_pController != NULL)
	{
		m_pController->Release();
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Release();
	}

	if (m_pModel != NULL)
	{
		m_pModel->Release();
	}

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
	}

	if (m_pEngine != NULL)
	{
		m_pEngine->Release();
	}
}

HRESULT CShapeShifterCtl::FinalConstruct()
{
	HRESULT hr = E_FAIL;

	// Create and initialize ShapeShifter engine

	hr = CEngine::_CreatorClass::CreateInstance(NULL, IID_IEngine, (LPVOID*) &m_pEngine);

	if (SUCCEEDED(hr))
	{
		hr = m_pEngine->InitializeEngine();
	}

	// Get the default device

	if (SUCCEEDED(hr))
	{
		hr = m_pEngine->GetDefaultDevice(&m_pDevice);
	}

	// Create the model

	if (SUCCEEDED(hr))
	{
		hr = CreateModel(CLSID_Model);
	}

	// Create the viewport

	if (SUCCEEDED(hr))
	{
		hr = CreateViewport(CLSID_Viewport);
	}

	// Create the controller

	if (SUCCEEDED(hr))
	{
		hr = CreateController(CLSID_BrowseController);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateAnimationLoop();
	}

	return hr;
}

LRESULT CShapeShifterCtl::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	if (IsRuntime())
	{
		RuntimeInit();
	}

	return 0;
}

LRESULT CShapeShifterCtl::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	if (m_pController != NULL)
	{
		_ControllerEventSink::DispEventUnadvise(m_pController);
	}

	return 0;
}

LRESULT CShapeShifterCtl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	uMsg;
	wParam;
	lParam;
	bHandled;

	if (m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);

		FLOAT fFOV;
		FLOAT fAspect;
		FLOAT fNearPlane;
		FLOAT fFarPlane;

		if (m_pViewport != NULL)
		{
			m_pViewport->SetBounds(&rcClient);

			// Set aspect ratio that is proportional to the window dimensions

			if (m_bAutoAspect)
			{
				if (SUCCEEDED(m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane)))
				{
					fAspect = (FLOAT) (rcClient.right - rcClient.left) / (FLOAT) (rcClient.bottom - rcClient.top);
					m_pViewport->SetFrustum(fFOV, fAspect, fNearPlane, fFarPlane);
				}
			}

			m_pViewport->Invalidate();
		}
	}

	return 0;
}

LRESULT CShapeShifterCtl::OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&)
{
	return 0;
}

HRESULT CShapeShifterCtl::OnDraw(ATL_DRAWINFO& di)
{
	BOOL bUserMode = TRUE;

	HRESULT hrAmbientUserMode = GetAmbientUserMode(bUserMode);

	if (FAILED(hrAmbientUserMode) || bUserMode)
	{
		// In run mode

		if (m_pViewport != NULL)
		{
			m_pViewport->RefreshScreen();
		}
	}
	else
	{
		// In design mode show logo.

		RECT& rc = *(RECT*)di.prcBounds;

		LOGBRUSH logBrush;
		ZeroMemory(&logBrush, sizeof(LOGBRUSH));
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = m_clrBackColor;
		HBRUSH hBkgnd = ::CreateBrushIndirect(&logBrush);

		if (hBkgnd != NULL)
		{
			FillRect(di.hdcDraw, &rc, hBkgnd);
			::DeleteObject(hBkgnd);
		}

		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		SetBkMode(di.hdcDraw, TRANSPARENT);
 		LPCTSTR pszText = _T("ShapeShifter : 3D for Applications!");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::DoVerb(LONG iVerb, LPMSG pMsg, IOleClientSite* pActiveSite,
                                      LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
{
	HRESULT hr = IOleObjectImpl<CShapeShifterCtl>::DoVerb(iVerb, pMsg, pActiveSite,
	                                                      lindex, hwndParent, lprcPosRect);

	if (SUCCEEDED(hr))
	{
		if (iVerb == OLEIVERB_UIACTIVATE || iVerb == OLEIVERB_INPLACEACTIVATE)
		{
			if (m_pDevice != NULL && !m_bDeviceInitialized)
			{
				// Initialize the device
				hr = m_pDevice->Initialize((long) m_hWnd);

				if (SUCCEEDED(hr) && m_pModel != NULL)
				{
					m_bDeviceInitialized = true;
					hr = m_pModel->LoadDevice(m_pDevice);
				}
			}

			if (SUCCEEDED(hr) && m_pViewport != NULL)
			{
				// Render initial frame
				hr = m_pViewport->Render();

				if (SUCCEEDED(hr))
				{
					hr = m_pViewport->RefreshScreen();
				}
			}

			if (FAILED(hr))
			{
				hr = E_FAIL;  // Map all failures to a single error
			}
			else
			{
				hr = S_OK;
			}
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::OnAmbientPropertyChange(DISPID dispid)
{
	if (dispid == DISPID_AMBIENT_USERMODE)
	{
		if (IsRuntime())
		{
			RuntimeInit();
		}
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::Update(void)
{
	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::IsUpToDate(void)
{
	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::GetMiscStatus(DWORD dwAspect, DWORD* pdwStatus)
{
	ATLTRACE2(atlTraceControls, 2, _T("IOleObjectImpl::GetMiscStatus\n"));
	return OleRegGetMiscStatus(GetObjectCLSID(), dwAspect, pdwStatus);
}

STDMETHODIMP CShapeShifterCtl::GetControlInfo(LPCONTROLINFO pCI)
{
	if (!pCI)
	{
		return E_POINTER;
	}

	pCI->hAccel = NULL;
	pCI->cAccel = 0;
	pCI->dwFlags = 0;

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::TranslateAccelerator(LPMSG pMsg)
{
	if (((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST)) &&
		((VK_TAB == pMsg->wParam) || (VK_RETURN == pMsg->wParam)))
	{
		CComQIPtr<IOleControlSite, &IID_IOleControlSite> spCtrlSite(m_spClientSite);

		if (spCtrlSite != NULL)
		{
			DWORD km = 0;
			km |= GetKeyState(VK_SHIFT)     < 0 ? 0x00000001   : 0;
			km |= GetKeyState(VK_CONTROL)   < 0 ? 0x00000002   : 0;
			km |= GetKeyState(VK_MENU)      < 0 ? 0x00000004   : 0;

			return spCtrlSite->TranslateAccelerator(pMsg, km);
		}
	}

	return S_FALSE;
}

STDMETHODIMP CShapeShifterCtl::get_DriverMode(/* [out,retval] */ enum DriverMode *pMode)
{
	if (pMode == NULL)
	{
		return E_POINTER;
	}

	if (m_pDevice == NULL)
	{
		return E_FAIL;
	}

	return m_pDevice->get_DriverMode(pMode);
}

STDMETHODIMP CShapeShifterCtl::put_DriverMode(/* [in] */ enum DriverMode mode)
{
	if (m_pDevice == NULL)
	{
		return E_FAIL;
	}

	// Return S_OK if new value is the same as the current value

	enum DriverMode curMode;

	if (m_pDevice->get_DriverMode(&curMode))
	{
		if (mode == curMode)
		{
			return S_OK;
		}
	}

	if (FireOnRequestEdit(SSCLID_DRIVERMODE) == S_FALSE)
	{
		return S_FALSE;
	}

	HRESULT hr = m_pDevice->put_DriverMode(mode);

	if (SUCCEEDED(hr))
	{
		m_bRequiresSave = TRUE;
		FireOnChanged(SSCLID_DRIVERMODE);
		FireViewChange();
		SendOnDataChange(NULL);
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::get_ModelType(BSTR *pVal)
{
	return ::ProgIDFromCLSID(m_clsidModel, pVal);
}

STDMETHODIMP CShapeShifterCtl::put_ModelType(BSTR newVal)
{
	CLSID newClsid;

	HRESULT hr = ::CLSIDFromProgID(newVal, &newClsid);

	if (FAILED(hr))
	{
		return E_INVALIDARG;
	}

	// Return S_OK if new value is the same as the current value

	if (::InlineIsEqualGUID(newClsid, m_clsidModel))
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_MODEL_TYPE) == S_FALSE)
	{
		return S_FALSE;
	}

	// Create new model using new CLSID
	hr = CreateModel(newClsid);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_MODEL_TYPE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_ViewportType(BSTR *pVal)
{
	return ::ProgIDFromCLSID(m_clsidViewport, pVal);
}

STDMETHODIMP CShapeShifterCtl::put_ViewportType(BSTR newVal)
{
	CLSID newClsid;

	HRESULT hr = ::CLSIDFromProgID(newVal, &newClsid);

	if (FAILED(hr))
	{
		return E_INVALIDARG;
	}

	// Return S_OK if new value is the same as the current value

	if (::InlineIsEqualGUID(newClsid, m_clsidViewport))
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_VIEWPORT_TYPE) == S_FALSE)
	{
		return S_FALSE;
	}

	// Create new viewport using new CLSID
	hr = CreateViewport(newClsid);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_VIEWPORT_TYPE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_ControllerType(BSTR *pVal)
{
	return ::ProgIDFromCLSID(m_clsidController, pVal);
}

STDMETHODIMP CShapeShifterCtl::put_ControllerType(BSTR newVal)
{
	CLSID newClsid;

	HRESULT hr = ::CLSIDFromProgID(newVal, &newClsid);

	if (FAILED(hr))
	{
		return E_INVALIDARG;
	}

	// Return S_OK if new value is the same as the current value

	if (::InlineIsEqualGUID(newClsid, m_clsidController))
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CONTROLLER_TYPE) == S_FALSE)
	{
		return S_FALSE;
	}

	// Create new controller using new CLSID
	hr = CreateController(newClsid);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CONTROLLER_TYPE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Engine(IEngine **pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_pEngine;

	if (*pVal != NULL)
	{
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Device(/* [out, retval] */ IDevice** pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_pDevice;

	if (*pVal != NULL)
	{
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Model(IModel **pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_pModel;

	if (*pVal != NULL)
	{
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Viewport(IViewport **pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_pViewport;

	if (*pVal != NULL)
	{
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Controller(IController **pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_pController;

	if (*pVal != NULL)
	{
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_ModelFile(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_strModelFile.copy();

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::put_ModelFile(/* [in] */ BSTR newVal)
{
	if (_bstr_t(newVal) == m_strModelFile)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_MODEL_FILE) == S_FALSE)
	{
		return S_FALSE;
	}

	m_strModelFile = newVal;
	DefaultModelReaderType();

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_MODEL_FILE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_ModelReaderType(BSTR *pVal)
{
	return ::ProgIDFromCLSID(m_clsidModelReader, pVal);
}

STDMETHODIMP CShapeShifterCtl::put_ModelReaderType(BSTR newVal)
{
	CLSID newClsid;

	HRESULT hr = ::CLSIDFromProgID(newVal, &newClsid);

	if (FAILED(hr))
	{
		return E_INVALIDARG;
	}

	// Return S_OK if new value is the same as the current value

	if (::InlineIsEqualGUID(newClsid, m_clsidModelReader))
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_MODEL_READER_TYPE) == S_FALSE)
	{
		return S_FALSE;
	}

	// Assign new CLSID and create new controller
	m_clsidModelReader = newClsid;

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_MODEL_READER_TYPE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::put_BackColor(/* [in] */ OLE_COLOR clr)
{
	if (clr == m_clrBackColor)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(DISPID_BACKCOLOR) == S_FALSE)
	{
		return S_FALSE;
	}

	m_clrBackColor = clr;

	if (m_pViewport != NULL)
	{
		m_pViewport->SetBackgroundColor(RGB_GETRED(m_clrBackColor),
		                                RGB_GETGREEN(m_clrBackColor),
		                                RGB_GETBLUE(m_clrBackColor),
		                                1.0f);
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(DISPID_BACKCOLOR);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Windowed(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	return m_pViewport->get_Windowed(pVal);
}

STDMETHODIMP CShapeShifterCtl::put_Windowed(/* [in] */ VARIANT_BOOL newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	VARIANT_BOOL curVal;

	if (SUCCEEDED(m_pViewport->get_Windowed(&curVal)) && curVal == newVal)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_WINDOWED) == S_FALSE)
	{
		return S_FALSE;
	}

	HRESULT hr = m_pViewport->put_Windowed(newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_WINDOWED);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_FOV(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		*pVal = fFOV;
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_FOV(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		if (newVal == fFOV)
		{
			return S_OK;
		}
	}

	if (FireOnRequestEdit(SSCLID_FOV) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = m_pViewport->SetFrustum(newVal, fAspect, fNearPlane, fFarPlane);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_FOV);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_Aspect(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		*pVal = fAspect;
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_Aspect(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		if (newVal == fAspect)
		{
			return S_OK;
		}
	}

	if (FireOnRequestEdit(SSCLID_ASPECT) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = m_pViewport->SetFrustum(fFOV, newVal, fNearPlane, fFarPlane);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_ASPECT);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_AutoAspect(/* [out, retval] */ VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_bAutoAspect)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::put_AutoAspect(/* [in] */ VARIANT_BOOL newVal)
{
	bool bNewAutoAspect;

	if (newVal)
	{
		bNewAutoAspect = true;
	}
	else
	{
		bNewAutoAspect = false;
	}

	if (bNewAutoAspect == m_bAutoAspect)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_AUTO_ASPECT) == S_FALSE)
	{
		return S_FALSE;
	}

	m_bAutoAspect = bNewAutoAspect;

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_AUTO_ASPECT);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_NearPlane(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		*pVal = fNearPlane;
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_NearPlane(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		if (newVal == fNearPlane)
		{
			return S_OK;
		}
	}

	if (FireOnRequestEdit(SSCLID_NEARPLANE) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = m_pViewport->SetFrustum(fFOV, fAspect, newVal, fFarPlane);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_NEARPLANE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_FarPlane(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		*pVal = fFarPlane;
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_FarPlane(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	FLOAT fFOV;
	FLOAT fAspect;
	FLOAT fNearPlane;
	FLOAT fFarPlane;

	HRESULT hr = m_pViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane);

	if (SUCCEEDED(hr))
	{
		if (newVal == fFarPlane)
		{
			return S_OK;
		}
	}

	if (FireOnRequestEdit(SSCLID_FARPLANE) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = m_pViewport->SetFrustum(fFOV, fAspect, fNearPlane, newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_FARPLANE);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraLocationX(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraLocY, fCameraLocZ;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetLocation(pVal, &fCameraLocY, &fCameraLocZ)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraLocationX(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraLocX, fCameraLocY, fCameraLocZ;

	hr = spCamera->GetLocation(&fCameraLocX, &fCameraLocY, &fCameraLocZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraLocX)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_LOCATION_X) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetLocation(newVal, fCameraLocY, fCameraLocZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_LOCATION_X);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraLocationY(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraLocX, fCameraLocZ;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetLocation(&fCameraLocX, pVal, &fCameraLocZ)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraLocationY(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraLocX, fCameraLocY, fCameraLocZ;

	hr = spCamera->GetLocation(&fCameraLocX, &fCameraLocY, &fCameraLocZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraLocY)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_LOCATION_Y) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetLocation(fCameraLocX, newVal, fCameraLocZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_LOCATION_Y);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraLocationZ(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraLocX, fCameraLocY;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetLocation(&fCameraLocX, &fCameraLocY, pVal)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraLocationZ(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraLocX, fCameraLocY, fCameraLocZ;

	hr = spCamera->GetLocation(&fCameraLocX, &fCameraLocY, &fCameraLocZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraLocZ)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_LOCATION_Z) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetLocation(fCameraLocX, fCameraLocY, newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_LOCATION_Z);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraAimX(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraAimY, fCameraAimZ;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetAimVector(pVal, &fCameraAimY, &fCameraAimZ)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraAimX(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraAimX, fCameraAimY, fCameraAimZ;

	hr = spCamera->GetAimVector(&fCameraAimX, &fCameraAimY, &fCameraAimZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraAimX)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_AIM_X) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetAimVector(newVal, fCameraAimY, fCameraAimZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_AIM_X);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraAimY(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraAimX, fCameraAimZ;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetAimVector(&fCameraAimX, pVal, &fCameraAimZ)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraAimY(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraAimX, fCameraAimY, fCameraAimZ;

	hr = spCamera->GetAimVector(&fCameraAimX, &fCameraAimY, &fCameraAimZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraAimY)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_AIM_Y) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetAimVector(fCameraAimX, newVal, fCameraAimZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_AIM_Y);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraAimZ(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraAimX, fCameraAimY;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetAimVector(&fCameraAimX, &fCameraAimY, pVal)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraAimZ(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraAimX, fCameraAimY, fCameraAimZ;

	hr = spCamera->GetAimVector(&fCameraAimX, &fCameraAimY, &fCameraAimZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraAimZ)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_AIM_Z) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetAimVector(fCameraAimX, fCameraAimY, newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_AIM_Z);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraUpX(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraUpY, fCameraUpZ;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetUpVector(pVal, &fCameraUpY, &fCameraUpZ)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraUpX(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraUpX, fCameraUpY, fCameraUpZ;

	hr = spCamera->GetUpVector(&fCameraUpX, &fCameraUpY, &fCameraUpZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraUpX)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_UP_X) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetAimVector(newVal, fCameraUpY, fCameraUpZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_UP_X);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraUpY(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraUpX, fCameraUpZ;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetUpVector(&fCameraUpX, pVal, &fCameraUpZ)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraUpY(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraUpX, fCameraUpY, fCameraUpZ;

	hr = spCamera->GetUpVector(&fCameraUpX, &fCameraUpY, &fCameraUpZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraUpY)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_UP_Y) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetAimVector(fCameraUpX, newVal, fCameraUpZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_UP_Y);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CameraUpZ(/* [out, retval] */ FLOAT* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;
	ICameraPtr spCamera;
	FLOAT fCameraUpX, fCameraUpY;

	if (SUCCEEDED(m_pViewport->get_Camera(&spCamera)))
	{
		if (SUCCEEDED(spCamera->GetUpVector(&fCameraUpX, &fCameraUpY, pVal)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::put_CameraUpZ(/* [in] */ FLOAT newVal)
{
	if (m_pViewport == NULL)
	{
		return E_FAIL;
	}

	// Get a pointer to the camera
	ICameraPtr spCamera;
	HRESULT hr = m_pViewport->get_Camera(&spCamera);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	FLOAT fCameraUpX, fCameraUpY, fCameraUpZ;

	hr = spCamera->GetUpVector(&fCameraUpX, &fCameraUpY, &fCameraUpZ);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == fCameraUpZ)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_CAMERA_UP_Z) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = spCamera->SetAimVector(fCameraUpX, fCameraUpY, newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_CAMERA_UP_Z);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_AnimationInterval(/* [out, retval] */ long *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pAnimationLoop == NULL)
	{
		return E_FAIL;
	}

	return m_pAnimationLoop->get_Interval(pVal);
}

STDMETHODIMP CShapeShifterCtl::put_AnimationInterval(/* [in] */ long newVal)
{
	if (m_pAnimationLoop == NULL)
	{
		return E_FAIL;
	}

	long curInterval;
	HRESULT hr = m_pAnimationLoop->get_Interval(&curInterval);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == curInterval)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_ANIMATION_INTERVAL) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = m_pAnimationLoop->put_Interval(newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_ANIMATION_INTERVAL);
	FireViewChange();
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_AnimationEnabled(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_bAnimationEnabled)
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::put_AnimationEnabled(/* [in] */ VARIANT_BOOL newVal)
{
	if ((newVal && m_bAnimationEnabled) ||
		(!newVal && !m_bAnimationEnabled))
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_ANIMATION_ENABLED) == S_FALSE)
	{
		return S_FALSE;
	}

	if (newVal)
	{
		if (IsRuntime() && m_pAnimationLoop != NULL)
		{
			m_pAnimationLoop->Start();
		}
		m_bAnimationEnabled = true;
	}
	else
	{
		if (IsRuntime() && m_pAnimationLoop != NULL)
		{
			m_pAnimationLoop->Stop();
		}
		m_bAnimationEnabled = false;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_ANIMATION_ENABLED);
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_CollisionsEnabled(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = VARIANT_FALSE;

	IFirstPersonControllerPtr spFPCtlr(m_pController);

	if (spFPCtlr != NULL)
	{
		spFPCtlr->get_CollisionsEnabled(pVal);
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::put_CollisionsEnabled(/* [in] */ VARIANT_BOOL newVal)
{
	IFirstPersonControllerPtr spFPCtlr(m_pController);

	if (spFPCtlr == NULL)
	{
		return S_FALSE;
	}

	VARIANT_BOOL bCurVal;
	spFPCtlr->get_CollisionsEnabled(&bCurVal);

	if (newVal == bCurVal)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_COLLISIONS_ENABLED) == S_FALSE)
	{
		return S_FALSE;
	}

	spFPCtlr->put_CollisionsEnabled(newVal);

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_COLLISIONS_ENABLED);
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::get_InternetEnabled(/* [out, retval] */ VARIANT_BOOL* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	if (m_pEngine == NULL)
	{
		return E_FAIL;
	}

	return m_pEngine->get_InternetEnabled(pVal);
}

STDMETHODIMP CShapeShifterCtl::put_InternetEnabled(/* [in] */ VARIANT_BOOL newVal)
{
	if (m_pEngine == NULL)
	{
		return E_FAIL;
	}

	VARIANT_BOOL bCurVal;

	HRESULT hr = m_pEngine->get_InternetEnabled(&bCurVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	if (newVal == bCurVal)
	{
		return S_OK;
	}

	if (FireOnRequestEdit(SSCLID_INTERNET_ENABLED) == S_FALSE)
	{
		return S_FALSE;
	}

	hr = m_pEngine->put_InternetEnabled(newVal);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_bRequiresSave = TRUE;
	FireOnChanged(SSCLID_INTERNET_ENABLED);
	SendOnDataChange(NULL);

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::LoadFile()
{
	HRESULT hr = E_FAIL;
	IModelReader* pModelReader = NULL;

	if (m_strModelFile.length() == 0)
	{
		return S_FALSE;
	}

	hr = CoCreateInstance(m_clsidModelReader, NULL, CLSCTX_INPROC_SERVER, IID_IModelReader, (LPVOID*) &pModelReader);

	if (SUCCEEDED(hr) && pModelReader != NULL)
	{
		hr = pModelReader->Read(_variant_t(m_strModelFile), m_pModel);
		pModelReader->Release();
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::ReloadDevice()
{
	HRESULT hr = E_FAIL;

	if (m_pDevice != NULL && m_pModel != NULL)
	{
		hr = m_pModel->LoadDevice(m_pDevice);

		if (SUCCEEDED(hr) && m_pViewport != NULL)
		{
			m_pViewport->Render();
			m_pViewport->RefreshScreen();
		}
	}

	return hr;
}

STDMETHODIMP CShapeShifterCtl::Refresh()
{
	if (m_pViewport != NULL)
	{
		m_pViewport->Render();
		m_pViewport->RefreshScreen();
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::AboutBox()
{
	CAboutDlg dlg;

	if (m_pDevice != NULL)
	{
		BSTR strDriverName;
		BSTR strDriverDesc;

		if (SUCCEEDED(m_pDevice->get_DriverName(&strDriverName)))
		{
			dlg.m_driverName = strDriverName;
			::SysFreeString(strDriverName);
		}
		if (SUCCEEDED(m_pDevice->get_DriverDesc(&strDriverDesc)))
		{
			dlg.m_driverDesc = strDriverDesc;
			::SysFreeString(strDriverDesc);
		}
	}

	dlg.DoModal();

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// ICategorizeProperties method overrides

STDMETHODIMP CShapeShifterCtl::MapPropertyToCategory(/* [in] */ DISPID dispid, /* [out] */ PROPCAT* ppropcat)
{
	if (ppropcat == NULL)
	{
		return E_POINTER;
	}

	switch (dispid)
	{
	case DISPID_BACKCOLOR:
		*ppropcat = PROPCAT_Appearance;
		break;

	case SSCLID_MODEL_TYPE:
	case SSCLID_VIEWPORT_TYPE:
	case SSCLID_CONTROLLER_TYPE:
	case SSCLID_MODEL_READER_TYPE:
		*ppropcat = PROPCAT_Types;
		break;

	case SSCLID_DRIVERMODE:
		*ppropcat = PROPCAT_Device;
		break;

	case SSCLID_FOV:
	case SSCLID_ASPECT:
	case SSCLID_AUTO_ASPECT:
	case SSCLID_NEARPLANE:
	case SSCLID_FARPLANE:
	case SSCLID_CAMERA_LOCATION_X:
	case SSCLID_CAMERA_LOCATION_Y:
	case SSCLID_CAMERA_LOCATION_Z:
	case SSCLID_CAMERA_AIM_X:
	case SSCLID_CAMERA_AIM_Y:
	case SSCLID_CAMERA_AIM_Z:
	case SSCLID_CAMERA_UP_X:
	case SSCLID_CAMERA_UP_Y:
	case SSCLID_CAMERA_UP_Z:
	case SSCLID_WINDOWED:
		*ppropcat = PROPCAT_Viewport;
		break;

	case SSCLID_ANIMATION_ENABLED:
	case SSCLID_ANIMATION_INTERVAL:
	case SSCLID_COLLISIONS_ENABLED:
		*ppropcat = PROPCAT_Behavior;
		break;

	default:
		*ppropcat = PROPCAT_Misc;
	}

	return S_OK;
}

STDMETHODIMP CShapeShifterCtl::GetCategoryName(/* [in] */ PROPCAT propcat, /* [in] */ LCID, /* [out] */ BSTR* pbstrName)
{
	HRESULT hr = E_FAIL;

	switch (propcat)
	{
	case PROPCAT_Types:
		*pbstrName = ::SysAllocString(L"Types");
		hr = S_OK;
		break;

	case PROPCAT_Device:
		*pbstrName = ::SysAllocString(L"Device");
		hr = S_OK;
		break;

	case PROPCAT_Viewport:
		*pbstrName = ::SysAllocString(L"Viewport");
		hr = S_OK;
		break;
	}

	return hr;
}

HRESULT CShapeShifterCtl::CreateDevice()
{
	HRESULT hr = E_FAIL;

	if (m_pDevice == NULL && m_pEngine != NULL)
	{
		hr = m_pEngine->GetDefaultDevice(&m_pDevice);
	}

	return hr;
}

HRESULT CShapeShifterCtl::CreateModel(const CLSID& clsid)
{
	if (m_pModel != NULL)
	{
		m_pModel->Release();
		m_pModel = NULL;
	}

	HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IModel, (LPVOID*) &m_pModel);

	if (SUCCEEDED(hr))
	{
		m_clsidModel = clsid;

		if (m_pViewport != NULL)
		{
			m_pViewport->putref_Model(m_pModel);
		}
	}

	return hr;
}

HRESULT CShapeShifterCtl::CreateViewport(const CLSID& clsid)
{
	HRESULT hr = E_FAIL;
	IViewportPtr spOldViewport(m_pViewport);
	BSTR strViewportProgID;

	if (m_pDevice == NULL)
	{
		return E_FAIL;
	}

	if (FAILED(::ProgIDFromCLSID(clsid, &strViewportProgID)))
	{
		return E_FAIL;
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Release();
		m_pViewport = NULL;
	}

	hr = m_pDevice->CreateViewport(_variant_t(strViewportProgID), &m_pViewport);

	if (SUCCEEDED(hr))
	{
		m_clsidViewport = clsid;

		m_pViewport->put_ContainerWindow((long) m_hWnd);

		// Enable Z buffering
		IRenderingContext* pContext = NULL;
		if (SUCCEEDED(m_pViewport->get_Context(&pContext)))
		{
			pContext->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pContext->Release();
		}

		if (spOldViewport != NULL)
		{
			// Copy values from old viewport to new viewport

			FLOAT fFOV;
			FLOAT fAspect;
			FLOAT fNearPlane;
			FLOAT fFarPlane;
			FLOAT fCameraLocX;
			FLOAT fCameraLocY;
			FLOAT fCameraLocZ;
			FLOAT fCameraAimX;
			FLOAT fCameraAimY;
			FLOAT fCameraAimZ;
			FLOAT fCameraUpX;
			FLOAT fCameraUpY;
			FLOAT fCameraUpZ;

			if (SUCCEEDED(spOldViewport->GetFrustum(&fFOV, &fAspect, &fNearPlane, &fFarPlane)))
			{
				m_pViewport->SetFrustum(fFOV, fAspect, fNearPlane, fFarPlane);
			}

			ICameraPtr spOldCamera = NULL;
			ICameraPtr spNewCamera = NULL;

			if (SUCCEEDED(spOldViewport->get_Camera(&spOldCamera)) &&
			    SUCCEEDED(m_pViewport->get_Camera(&spNewCamera)))
			{
				spOldCamera->GetLocation(&fCameraLocX, &fCameraLocY, &fCameraLocZ);
				spNewCamera->SetLocation(fCameraLocX, fCameraLocY, fCameraLocZ);
				spOldCamera->GetAimVector(&fCameraAimX, &fCameraAimY, &fCameraAimZ);
				spNewCamera->SetAimVector(fCameraAimX, fCameraAimY, fCameraAimZ);
				spOldCamera->GetUpVector(&fCameraUpX, &fCameraUpY, &fCameraUpZ);
				spNewCamera->SetUpVector(fCameraUpX, fCameraUpY, fCameraUpZ);
			}
		}
		else
		{
			// Assign default values

			// Set initial camera location and orientation
			ICamera* pCamera = NULL;
			if (SUCCEEDED(m_pViewport->get_Camera(&pCamera)))
			{
				pCamera->SetLocation(0.0f, 0.0f, -8.0f);
				pCamera->SetAimVector(0.0f, 0.0f, 1.0f);
				pCamera->Release();
			}

			// Assign default frustum settings
			m_pViewport->SetFrustum(1.57f /* 45 degrees */, 1.0f, 0.0f, 1.0f);
		}

		m_pViewport->SetBackgroundColor(RGB_GETRED(m_clrBackColor),
										RGB_GETGREEN(m_clrBackColor),
										RGB_GETBLUE(m_clrBackColor),
										1.0f);

		// Attach the model to the viewport
		if (m_pModel != NULL)
		{
			m_pViewport->putref_Model(m_pModel);
		}

		// Attach the controller to the viewport
		if (m_pController != NULL)
		{
			m_pController->putref_Viewport(m_pViewport);
		}
	}

	::SysFreeString(strViewportProgID);

	return hr;
}

HRESULT CShapeShifterCtl::CreateController(const CLSID& clsid)
{
	if (m_pController != NULL)
	{
		_ControllerEventSink::DispEventUnadvise(m_pController);
		m_pController->Release();
		m_pController = NULL;
	}

	if (m_pWndMsgRcvr != NULL)
	{
		m_pWndMsgRcvr->Release();
		m_pWndMsgRcvr = NULL;
	}

	HRESULT hr = CoCreateInstance(m_clsidController, NULL, CLSCTX_INPROC_SERVER, IID_IController, (LPVOID*) &m_pController);

	if (SUCCEEDED(hr))
	{
		m_clsidController = clsid;

		// Make sure we receive events fired by the controller so we can forward them on
		hr = _ControllerEventSink::DispEventAdvise(m_pController);
	}

	if (SUCCEEDED(hr))
	{
		if (FAILED(m_pController->QueryInterface(IID_IWndMessageReceiver, (LPVOID*) &m_pWndMsgRcvr)))
		{
			m_pWndMsgRcvr = NULL;
		}

		if (m_pViewport != NULL)
		{
			m_pController->putref_Viewport(m_pViewport);
		}

		if (m_pAnimationLoop != NULL)
		{
			IAnimate* pAnimator = NULL;

			if (SUCCEEDED(m_pController->QueryInterface(IID_IAnimate, (LPVOID*) &pAnimator)))
			{
				m_pAnimationLoop->AddAnimator(pAnimator);
				pAnimator->Release();
			}
		}
	}

	return hr;
}

HRESULT CShapeShifterCtl::CreateAnimationLoop()
{
	if (m_pAnimationLoop != NULL)
	{
		m_pAnimationLoop->Release();
		m_pAnimationLoop = NULL;
	}

	HRESULT hr = CAnimationLoop::_CreatorClass::CreateInstance(NULL, IID_IAnimationLoop, (LPVOID*) &m_pAnimationLoop);

	if (SUCCEEDED(hr) && m_pController != NULL)
	{
		IAnimate* pAnimator = NULL;

		if (SUCCEEDED(m_pController->QueryInterface(IID_IAnimate, (LPVOID*) &pAnimator)))
		{
			m_pAnimationLoop->AddAnimator(pAnimator);
			pAnimator->Release();
		}
	}

	return hr;
}

HRESULT CShapeShifterCtl::DefaultModelReaderType()
{
	UINT nFileNameLen = m_strModelFile.length();

	if (nFileNameLen > 0)
	{
		LPTSTR lpszFileName = new TCHAR[nFileNameLen+1];
		_tcscpy(lpszFileName, m_strModelFile);
		LPTSTR lpszFileExt = _tcschr(lpszFileName, '.');
		if (lpszFileExt != NULL)
		{
			lpszFileExt++;

			for (int i = 0; i < 3 && lpszFileExt[i] != 0; i++)
			{
				lpszFileExt[i] = (TCHAR) toupper(lpszFileExt[i]);
			}

			if (_tcsncmp(lpszFileExt, _T("3DS"), 3) == 0)
			{
				m_clsidModelReader = CLSID_ModelReader3DS;
			}
			else
			{
				m_clsidModelReader = CLSID_ModelReaderXML;
			}
		}

		delete[] lpszFileName;
	}
	else
	{
		m_clsidModelReader = CLSID_ModelReaderXML;
	}

	return S_OK;
}

bool CShapeShifterCtl::IsRuntime() const
{
	BOOL bUserMode = TRUE;
	HRESULT hrAmbientUserMode = const_cast<CShapeShifterCtl*>(this)->GetAmbientUserMode(bUserMode);

	if (FAILED(hrAmbientUserMode) || bUserMode)
	{
		return true;
	}

	return false;
}

HRESULT CShapeShifterCtl::RuntimeInit()
{
	HRESULT hr = S_OK;

	if (FAILED(LoadFile()))
	{
		hr = E_FAIL;
	}

	if (m_pAnimationLoop != NULL && m_bAnimationEnabled)
	{
		hr = m_pAnimationLoop->Start();
	}

	return hr;
}

void CShapeShifterCtl::OnSelectionChange(IDispatch* pSelectionList)
{
	INodes* pSelectionNodes = NULL;
	if (SUCCEEDED(pSelectionList->QueryInterface(IID_INodes, (LPVOID*)&pSelectionNodes)))
	{
		Fire_OnSelectionChange(pSelectionNodes);
		pSelectionNodes->Release();
	}
}

};  // namespace ShapeShifter

