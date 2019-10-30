//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: DevicePresentation.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __DEVICEPRESENTATION_H__
#define __DEVICEPRESENTATION_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IDevicePresentationDX8Impl

class IDevicePresentationDX8Impl : public IDevicePresentationDX8
{
// Constructors/destructor
public:
	IDevicePresentationDX8Impl() :
		m_pD3DDevice(NULL),
		m_pD3DSwapChain(NULL),
		m_pCurBackBuffer(NULL),
		m_pDepthStencil(NULL)
	{
		ZeroMemory(&m_presentParams, sizeof(D3DPRESENT_PARAMETERS));
		m_presentParams.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
		m_presentParams.EnableAutoDepthStencil = TRUE;
		m_presentParams.BackBufferCount = 1;
		m_presentParams.Windowed = TRUE;
	}

	virtual ~IDevicePresentationDX8Impl()
	{
		FreePresentationBuffers();
	}

// Attributes
protected:
	D3DPRESENT_PARAMETERS m_presentParams;
	LPDIRECT3DDEVICE8 m_pD3DDevice;
	LPDIRECT3DSWAPCHAIN8 m_pD3DSwapChain;
	LPDIRECT3DSURFACE8 m_pCurBackBuffer;
	LPDIRECT3DSURFACE8 m_pDepthStencil;

// Operations
public:

	//////////////////////////////////////////////////////////////
	// IDevicePresentationDX8 interface

	STDMETHOD(SetPresentationParameters)(D3DPRESENT_PARAMETERS* pPresentParams)
	{
		if (pPresentParams == NULL)
		{
			return E_POINTER;
		}

		memcpy(&m_presentParams, pPresentParams, sizeof(D3DPRESENT_PARAMETERS));

		return S_OK;
	}

	STDMETHOD(GetPresentationParameters)(D3DPRESENT_PARAMETERS* pPresentParams)
	{
		if (pPresentParams == NULL)
		{
			return E_POINTER;
		}

		memcpy(pPresentParams, &m_presentParams, sizeof(D3DPRESENT_PARAMETERS));

		return S_OK;
	}

	STDMETHOD(SetPresentationBuffers)(LPDIRECT3DDEVICE8 pDevice,
	                                  LPDIRECT3DSWAPCHAIN8 pSwapChain,
	                                  LPDIRECT3DSURFACE8 pDepthStencil)
	{
		if (pDevice != NULL)
		{
			pDevice->AddRef();
		}

		if (m_pD3DDevice != NULL)
		{
			m_pD3DDevice->Release();
		}

		m_pD3DDevice = pDevice;

		if (pSwapChain != NULL)
		{
			pSwapChain->AddRef();
		}

		if (m_pD3DSwapChain != NULL)
		{
			m_pD3DSwapChain->Release();
		}

		m_pD3DSwapChain = pSwapChain;

		if (pDepthStencil != NULL)
		{
			pDepthStencil->AddRef();
		}

		if (m_pDepthStencil != NULL)
		{
			m_pDepthStencil->Release();
		}

		m_pDepthStencil = pDepthStencil;

		// Select the first back buffer as a default
		return SelectBackBuffer(0, D3DBACKBUFFER_TYPE_MONO);
	}

	STDMETHOD(SharePresentationBuffers)(IDevicePresentationDX8* pShareWith)
	{
		LPDIRECT3DDEVICE8 pDevice = NULL;
		LPDIRECT3DSWAPCHAIN8 pSwapChain = NULL;
		LPDIRECT3DSURFACE8 pDepthStencil = NULL;

		pShareWith->GetDevice(&pDevice);
		pShareWith->GetSwapChain(&pSwapChain);
		pShareWith->GetDepthStencil(&pDepthStencil);
		SetPresentationBuffers(pDevice, pSwapChain, pDepthStencil);

		return S_OK;
	}

	STDMETHOD(FreePresentationBuffers)()
	{
		if (m_pDepthStencil != NULL)
		{
			m_pDepthStencil->Release();
			m_pDepthStencil = NULL;
		}

		if (m_pCurBackBuffer != NULL)
		{
			m_pCurBackBuffer->Release();
			m_pCurBackBuffer = NULL;
		}

		if (m_pD3DSwapChain != NULL)
		{
			m_pD3DSwapChain->Release();
			m_pD3DSwapChain = NULL;
		}

		if (m_pD3DDevice != NULL)
		{
			m_pD3DDevice->Release();
			m_pD3DDevice = NULL;
		}

		return S_OK;
	}

	STDMETHOD(GetSwapChainType)(DWORD* pFlag)
	{
		if (pFlag == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_OK;

		if (m_pD3DDevice == NULL && m_pD3DSwapChain == NULL)
		{
			*pFlag = SWAP_CHAIN_NONE;
		}
		else if (m_pD3DDevice != NULL && m_pD3DSwapChain == NULL)
		{
			*pFlag = SWAP_CHAIN_IMPLICIT;
		}
		else if (m_pD3DDevice != NULL && m_pD3DSwapChain != NULL)
		{
			*pFlag = SWAP_CHAIN_ADDITIONAL;
		}
		else
		{
			hr = E_UNEXPECTED;
		}

		return hr;
	}

	STDMETHOD(GetDevice)(LPDIRECT3DDEVICE8* ppDevice)
	{
		if (ppDevice == NULL)
		{
			return E_POINTER;
		}

		*ppDevice = m_pD3DDevice;

		if (*ppDevice != NULL)
		{
			(*ppDevice)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(GetSwapChain)(LPDIRECT3DSWAPCHAIN8* ppSwapChain)
	{
		if (ppSwapChain == NULL)
		{
			return E_POINTER;
		}

		*ppSwapChain = m_pD3DSwapChain;

		if (*ppSwapChain != NULL)
		{
			(*ppSwapChain)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(GetBackBuffer)(LPDIRECT3DSURFACE8* ppBackBuffer)
	{
		if (ppBackBuffer == NULL)
		{
			return E_POINTER;
		}

		*ppBackBuffer = m_pCurBackBuffer;

		if (*ppBackBuffer != NULL)
		{
			(*ppBackBuffer)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(GetDepthStencil)(LPDIRECT3DSURFACE8* ppDepthStencil)
	{
		if (ppDepthStencil == NULL)
		{
			return E_POINTER;
		}

		*ppDepthStencil = m_pDepthStencil;

		if (*ppDepthStencil != NULL)
		{
			(*ppDepthStencil)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(SelectBackBuffer)(UINT nBackBuffer, D3DBACKBUFFER_TYPE bbType)
	{
		HRESULT hr = E_FAIL;
		LPDIRECT3DSURFACE8 pBackBuffer = NULL;

		if (m_pD3DDevice != NULL)
		{
			if (m_pD3DSwapChain != NULL)
			{
				hr = m_pD3DSwapChain->GetBackBuffer(nBackBuffer, bbType, &pBackBuffer);
			}
			else
			{
				hr = m_pD3DDevice->GetBackBuffer(nBackBuffer, bbType, &pBackBuffer);
			}

			if (SUCCEEDED(hr) && pBackBuffer != NULL)
			{
				if (m_pCurBackBuffer != NULL)
				{
					m_pCurBackBuffer->Release();
				}

				m_pCurBackBuffer = pBackBuffer;

				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(SetRenderTarget)()
	{
#if _DEBUG
		if (m_pD3DDevice == NULL)
		{
			return E_FAIL;
		}
#endif
		return m_pD3DDevice->SetRenderTarget(m_pCurBackBuffer, m_pDepthStencil);
	}

	STDMETHOD(Present)(CONST RECT* pSourceRect,
	                   CONST RECT* pDestRect,
	                   HWND hDestWindowOverride,
	                   CONST RGNDATA* pDirtyRegion)
	{
		if (m_pD3DSwapChain != NULL)
		{
			return m_pD3DSwapChain->Present(pSourceRect,
											pDestRect,
											hDestWindowOverride,
											pDirtyRegion);
		}
		else if (m_pD3DDevice != NULL)
		{
			return m_pD3DDevice->Present(pSourceRect,
										 pDestRect,
										 hDestWindowOverride,
										 pDirtyRegion);
		}
		return S_FALSE;
	}

	//////////////////////////////////////////////////////////////
	// Helper functions for derived class

	HRESULT SetBackBufferSize(const UINT nWidth, const UINT nHeight)
	{
		if (m_presentParams.BackBufferWidth != nWidth)
		{
			m_presentParams.BackBufferWidth = nWidth;
			FreePresentationBuffers();
		}
		if (m_presentParams.BackBufferHeight != nHeight)
		{
			m_presentParams.BackBufferHeight = nHeight;
			FreePresentationBuffers();
		}
		return S_OK;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __DEVICEPRESENTATION_H__
