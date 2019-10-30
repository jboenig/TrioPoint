//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Viewport.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Viewport.h"
#include "ViewportDecorator.h"
#include "Property.h"
#include "RenderingContext.h"
#include "Camera.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CViewport
/////////////////////////////////////////////////////////////////////////

// Property table for supporting late-bound properties

SSCL_DEFINE_PROPERTY_ACCESSOR(CViewport, FillMode)
SSCL_DEFINE_PROPERTY_ACCESSOR(CViewport, ZEnable)
SSCL_DEFINE_PROPERTY_BOOL_ACCESSOR(CViewport, LightingEnabled)

SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(CViewport)
	SSCL_RUNTIME_PROPERTY_ENTRY(FillMode, SSCLID_FILLMODE, VT_I4)
	SSCL_RUNTIME_PROPERTY_ENTRY(ZEnable, SSCLID_ZENABLE, VT_I4)
	SSCL_RUNTIME_PROPERTY_ENTRY(LightingEnabled, SSCLID_LIGHTINGENABLED, VT_BOOL)
	SSCL_RUNTIME_PROPERTY_ENTRY_NULL
SSCL_END_RUNTIME_PROPERTY_TABLE

CViewport::CViewport() :
	m_pPresentationMgr(NULL),
	m_bInvalidateWindow(false)
{
	m_presentParams.Windowed = TRUE;
	m_presentParams.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
	m_presentParams.EnableAutoDepthStencil = TRUE;
	m_presentParams.BackBufferCount = 1;
}

CViewport::~CViewport()
{
}

HRESULT CViewport::FinalConstruct()
{
	HRESULT hr = S_OK;

	hr = CRenderingContext::_CreatorClass::CreateInstance(NULL, IID_IRenderingContext, (LPVOID*) &m_pContext);

	if (SUCCEEDED(hr) && m_pContext != NULL)
	{
		m_pContext->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pContext->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pContext->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (SUCCEEDED(hr))
	{
		hr = CCamera::_CreatorClass::CreateInstance(NULL, IID_ICameraEx, (LPVOID*) &m_pCamera);
	}

	if (SUCCEEDED(hr))
	{
		IVisual* pAxisGuide = NULL;
		hr = CAxisGuide::_CreatorClass::CreateInstance(NULL, IID_IVisual, (LPVOID*) &pAxisGuide);

		if (SUCCEEDED(hr))
		{
			hr = AppendChild(pAxisGuide);
			pAxisGuide->put_Visible(FALSE);
			pAxisGuide->Release();
		}
	}

	return hr;
}

void CViewport::FinalRelease()
{
	if (m_pPresentationMgr != NULL)
	{
		IDevicePresentationDX8* pPresentation = static_cast<IDevicePresentationDX8*>(this);
		m_pPresentationMgr->RemovePresentation(pPresentation);
		m_pPresentationMgr->Release();
		m_pPresentationMgr = NULL;
	}
}

STDMETHODIMP CViewport::_InitObject(/* [in] */ IDevice* pDevice)
{
	HRESULT hr = _BaseClass::_InitObject(pDevice);

	if (hr == S_OK)
	{
		IDevicePresentationDX8* pPresentation = static_cast<IDevicePresentationDX8*>(this);

		if (m_pPresentationMgr != NULL)
		{
			m_pPresentationMgr->Release();
			m_pPresentationMgr = NULL;
		}

		hr = m_pDevice->QueryInterface(IID_IPresentationManager, (LPVOID*) &m_pPresentationMgr);

		if (SUCCEEDED(hr))
		{
			hr = m_pPresentationMgr->AddPresentation(pPresentation);
		}
	}

	return hr;
}

STDMETHODIMP CViewport::get_Windowed(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}
	*pVal = m_presentParams.Windowed ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CViewport::put_Windowed(/* [in] */ VARIANT_BOOL newVal)
{
	BOOL bNewVal = (newVal == VARIANT_FALSE) ? FALSE : TRUE;

	if (bNewVal != m_presentParams.Windowed)
	{
		FreePresentationBuffers();
	}

	m_presentParams.Windowed = bNewVal;

	return S_OK;
}

STDMETHODIMP CViewport::get_ContainerWindow(/*[out, retval]*/ long* pVal)
{
	*pVal = reinterpret_cast<long>(m_presentParams.hDeviceWindow);
	return S_OK;
}

STDMETHODIMP CViewport::put_ContainerWindow(/*[in]*/ long newVal)
{
	m_presentParams.hDeviceWindow = reinterpret_cast<HWND>(newVal);
	return S_OK;
}

STDMETHODIMP CViewport::put_InvalidateWindow(/* [in] */ VARIANT_BOOL newVal)
{
	m_bInvalidateWindow = newVal ? true : false;
	return S_OK;
}

STDMETHODIMP CViewport::get_InvalidateWindow(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	*pVal = m_bInvalidateWindow ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CViewport::Render()
{
	HRESULT hr = E_FAIL;

	if (m_pD3DDevice == NULL)
	{
		// Initialize the presentation buffers
		if (m_pPresentationMgr != NULL)
		{
			hr = m_pPresentationMgr->InitPresentation(this);

			if (FAILED(hr))
			{
				Error::SetInfo(CLSID_Viewport, IID_IViewport, Error::E_INIT_VIEWPORT);
				return Error::E_INIT_VIEWPORT;
			}
		}
	}

	if (m_pPresentationMgr != NULL)
	{
		hr = SetRenderTarget();

		if (FAILED(hr))
		{
			Error::SetInfo(CLSID_Viewport, IID_IViewport, Error::E_INIT_VIEWPORT);
			return Error::E_INIT_VIEWPORT;
		}
	}

	if (m_pD3DDevice != NULL)
	{
		// Prepare the device for rendering

		hr = m_pD3DDevice->SetViewport(&m_vpInfo);

		if (SUCCEEDED(hr) && m_pContext != NULL)
		{
			IContextObject* pContext = NULL;
			if (SUCCEEDED(m_pContext->QueryInterface(IID_IContextObject, (LPVOID*) &pContext)))
			{
				hr = pContext->SelectObject(m_pD3DDevice, NULL, 0);
				pContext->Release();
			}
		}

		if (SUCCEEDED(hr))
		{
			// The projection matrix defines how the 3D scene is "projected" onto the
			// 2D render target.
			D3DXMATRIX projMat;
			FillProjectionMatrix(projMat);
			m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projMat);
		}

		if (SUCCEEDED(hr))
		{
			IResourceManagerDX8* pDevResources = NULL;
			hr = m_pDevice->QueryInterface(IID_IResourceManagerDX8, (LPVOID*) &pDevResources);
			if (SUCCEEDED(hr))
			{
				hr = static_cast<IRenderDX8*>(this)->Render(m_pD3DDevice, pDevResources);
				pDevResources->Release();
			}
		}
	}

	return hr;
}

STDMETHODIMP CViewport::RefreshScreen()
{
	HRESULT hr = E_FAIL;

	if (m_presentParams.Windowed)
	{
		RECT rcSrc;
		rcSrc.left = rcSrc.top = 0;
		rcSrc.right = m_rcBounds.right - m_rcBounds.left;
		rcSrc.bottom = m_rcBounds.bottom - m_rcBounds.top;
		hr = Present(&rcSrc, &m_rcBounds, 0, NULL);
	}
	else
	{
		hr = Present(NULL, NULL, 0, NULL);
	}

	return hr;
}

STDMETHODIMP CViewport::Invalidate()
{
	HRESULT hr = Render();

	if (SUCCEEDED(hr))
	{
		if (m_presentParams.Windowed && m_bInvalidateWindow)
		{
			if (m_presentParams.hDeviceWindow != NULL && ::IsWindow(m_presentParams.hDeviceWindow))
			{
				::InvalidateRect(m_presentParams.hDeviceWindow, &m_rcBounds, FALSE);
			}
		}
		else
		{
			hr = RefreshScreen();
		}
	}

	return hr;
}

STDMETHODIMP CViewport::DrawTracking(/* [in] */ INodes* pTrackingNodes)
{
	HRESULT hr = E_FAIL;

	if (m_pD3DDevice == NULL)
	{
		// Initialize the presentation buffers
		if (m_pPresentationMgr != NULL)
		{
			hr = m_pPresentationMgr->InitPresentation(this);

			if (FAILED(hr))
			{
				return E_FAIL;
			}
		}
	}

	if (m_pPresentationMgr != NULL)
	{
		hr = SetRenderTarget();

		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}

	if (m_pD3DDevice != NULL)
	{
		// Prepare the device for rendering

		hr = m_pD3DDevice->SetViewport(&m_vpInfo);

		if (SUCCEEDED(hr) && m_pContext != NULL)
		{
			IContextObject* pContext = NULL;
			if (SUCCEEDED(m_pContext->QueryInterface(IID_IContextObject, (LPVOID*) &pContext)))
			{
				hr = pContext->SelectObject(m_pD3DDevice, NULL, 0);
				pContext->Release();
			}
		}

		if (SUCCEEDED(hr))
		{
			// The projection matrix defines how the 3D scene is "projected" onto the
			// 2D render target.
			D3DXMATRIX projMat;
			FillProjectionMatrix(projMat);
			m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projMat);
		}

		if (SUCCEEDED(hr))
		{
			IResourceManagerDX8* pDevResources = NULL;
			hr = m_pDevice->QueryInterface(IID_IResourceManagerDX8, (LPVOID*) &pDevResources);
			if (SUCCEEDED(hr))
			{
				BeginFrame(m_pD3DDevice, pDevResources);

				if (m_pModel != NULL)
				{
					IRenderDX8* pModelRender;
					if (SUCCEEDED(m_pModel->QueryInterface(IID_IRenderDX8, (LPVOID*) &pModelRender)))
					{
						hr = pModelRender->Render(m_pD3DDevice, pDevResources);
						pModelRender->Release();
					}
				}

				// Render viewport children (decorators)
				_NodeVector::iterator itChild;
				IRenderDX8* pRender = NULL;

				for (itChild = m_children.begin(); hr == S_OK && itChild != m_children.end(); itChild++)
				{
					pRender = (IRenderDX8*) *itChild;
					if (pRender != NULL)
					{
						hr = pRender->Render(m_pD3DDevice, pDevResources);
					}
				}

				IUnknown* pEnumUnk = NULL;
				if (SUCCEEDED(pTrackingNodes->get__NewNodeEnum(&pEnumUnk)) && pEnumUnk != NULL)
				{
					IEnumNodes* pEnumNodes;

					if (SUCCEEDED(pEnumUnk->QueryInterface(IID_IEnumNodes, (LPVOID*) &pEnumNodes)))
					{
						ULONG nFetched;
						INode* pCurNode;
						while (pEnumNodes->Next(1, &pCurNode, &nFetched) == S_OK && nFetched > 0)
						{
							IRenderDX8* pCurVisual;
							if (SUCCEEDED(pCurNode->QueryInterface(IID_IRenderDX8, (LPVOID*) &pCurVisual)))
							{
								pCurVisual->Render(m_pD3DDevice, pDevResources);
								pCurVisual->Release();
							}
							pCurNode->Release();
						}

						pEnumNodes->Release();
					}

					pEnumUnk->Release();
				}

				EndFrame(m_pD3DDevice, pDevResources);

				RefreshScreen();

				pDevResources->Release();
			}
		}
	}

	return hr;
}

HRESULT CViewport::BeginFrame(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8*)
{
	if (pDevice == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	// Clear the viewport and z-buffer
	D3DCOLOR dwBackgroundClr = D3DCOLOR_COLORVALUE(m_clrBackground.r, m_clrBackground.g, m_clrBackground.b, m_clrBackground.a);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, dwBackgroundClr, 1.0f, 0L);

	// Begin the scene
	if (FAILED(pDevice->BeginScene()))
		return E_FAIL;

	// Create and set the view matrix

	D3DXMATRIX viewMatrix;
	FillViewMatrix(viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

	return hr;
}

HRESULT CViewport::EndFrame(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8*)
{
	pDevice->EndScene();
	return S_OK;
}

STDMETHODIMP CViewport::Render(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
{
	if (pDevice == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	// Clear the viewport and z-buffer
	D3DCOLOR dwBackgroundClr = D3DCOLOR_COLORVALUE(m_clrBackground.r, m_clrBackground.g, m_clrBackground.b, m_clrBackground.a);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, dwBackgroundClr, 1.0f, 0L);

	// Begin the scene
	if (FAILED(pDevice->BeginScene()))
		return E_FAIL;

	// Create and set the view matrix

	D3DXMATRIX viewMatrix;
	FillViewMatrix(viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

	if (m_pModel != NULL)
	{
		// Render the model
		IRenderDX8* pModelRender;
		if (SUCCEEDED(m_pModel->QueryInterface(IID_IRenderDX8, (LPVOID*) &pModelRender)))
		{
			hr = pModelRender->Render(pDevice, pDevResources);
			pModelRender->Release();
		}

		// Render viewport children (decorators)
		_NodeVector::iterator itChild;
		IRenderDX8* pRender = NULL;

		for (itChild = m_children.begin(); hr == S_OK && itChild != m_children.end(); itChild++)
		{
			pRender = (IRenderDX8*) *itChild;
			if (pRender != NULL)
			{
				hr = pRender->Render(pDevice, pDevResources);
			}
		}

		HighlightSelection(pDevice, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.0f));
	}

	// End the scene.
	pDevice->EndScene();

	return hr;
}

STDMETHODIMP CViewport::SetBounds(LPRECT pBounds)
{
	HRESULT hr = _BaseClass::SetBounds(pBounds);

	if (SUCCEEDED(hr))
	{
		UINT nWidth = m_rcBounds.right - m_rcBounds.left;
		UINT nHeight = m_rcBounds.bottom - m_rcBounds.top;

		if (nWidth > m_presentParams.BackBufferWidth)
		{
			m_presentParams.BackBufferWidth = nWidth;
			FreePresentationBuffers();
		}

		if (nHeight > m_presentParams.BackBufferHeight)
		{
			m_presentParams.BackBufferHeight = nHeight;
			FreePresentationBuffers();
		}
	}

	return hr;
}

HRESULT CViewport::get_FillMode(/*[out, retval]*/ long* pVal)
{
	if (m_pContext != NULL)
	{
		return m_pContext->GetRenderState(D3DRS_FILLMODE, pVal);
	}
	return E_FAIL;
}

HRESULT CViewport::put_FillMode(/*[in]*/ long newVal)
{
	if (m_pContext != NULL)
	{
		return m_pContext->SetRenderState(D3DRS_FILLMODE, newVal);
	}
	return E_FAIL;
}

HRESULT CViewport::get_ZEnable(/*[out, retval]*/ long* pVal)
{
	if (m_pContext != NULL)
	{
		return m_pContext->GetRenderState(D3DRS_ZENABLE, pVal);
	}
	return E_FAIL;
}

HRESULT CViewport::put_ZEnable(/*[in]*/ long newVal)
{
	if (m_pContext != NULL)
	{
		return m_pContext->SetRenderState(D3DRS_ZENABLE, newVal);
	}
	return E_FAIL;
}

HRESULT CViewport::get_LightingEnabled(/*[out, retval]*/ VARIANT_BOOL* pVal)
{
	HRESULT hr = E_FAIL;

	if (m_pContext != NULL)
	{
		long lVal;
		hr = m_pContext->GetRenderState(D3DRS_LIGHTING, &lVal);

		if (SUCCEEDED(hr))
		{
			*pVal = (lVal) ? (VARIANT_TRUE) : (VARIANT_FALSE);
		}
	}

	return hr;
}

HRESULT CViewport::put_LightingEnabled(/*[in]*/ VARIANT_BOOL newVal)
{
	HRESULT hr = E_FAIL;

	if (m_pContext != NULL)
	{
		BOOL bEnable = (newVal == VARIANT_TRUE) ? (TRUE) : (FALSE);

		if (SUCCEEDED(m_pContext->SetRenderState(D3DRS_LIGHTING, bEnable)))
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CViewport::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IViewport
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

};  // namespace ShapeShifter
