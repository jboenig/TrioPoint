//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Texture.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Texture.h"
#include "Internet.h"

namespace ShapeShifter {

const LPCTSTR TEXTURE_FORMAT_DEFAULT = _T("DEFAULT");
const LPCTSTR TEXTURE_FORMAT_R8G8B8 = _T("R8G8B8");
const LPCTSTR TEXTURE_FORMAT_A8R8G8B8 = _T("A8R8G8B8");
const LPCTSTR TEXTURE_FORMAT_X8R8G8B8 = _T("X8R8G8B8");
const LPCTSTR TEXTURE_FORMAT_R5G6B5 = _T("R5G6B5");
const LPCTSTR TEXTURE_FORMAT_R5G5B5 = _T("R5G5B5");
const LPCTSTR TEXTURE_FORMAT_PALETTE4 = _T("PALETTE4");
const LPCTSTR TEXTURE_FORMAT_PALETTE8 = _T("PALETTE8");
const LPCTSTR TEXTURE_FORMAT_A1R5G5B5 = _T("A1R5G5B5");
const LPCTSTR TEXTURE_FORMAT_X4R4G4B4 = _T("X4R4G4B4");
const LPCTSTR TEXTURE_FORMAT_A4R4G4B4 = _T("A4R4G4B4");
const LPCTSTR TEXTURE_FORMAT_L8 = _T("L8");
const LPCTSTR TEXTURE_FORMAT_A8L8 = _T("A8L8");
const LPCTSTR TEXTURE_FORMAT_U8V8 = _T("U8V8");
const LPCTSTR TEXTURE_FORMAT_U5V5L6 = _T("U5V5L6");
const LPCTSTR TEXTURE_FORMAT_U8V8L8 = _T("U8V8L8");
const LPCTSTR TEXTURE_FORMAT_UYVY = _T("UYVY");
const LPCTSTR TEXTURE_FORMAT_YUY2 = _T("YUY2");
const LPCTSTR TEXTURE_FORMAT_DXT1 = _T("DXT1");
const LPCTSTR TEXTURE_FORMAT_DXT3 = _T("DXT3");
const LPCTSTR TEXTURE_FORMAT_DXT5 = _T("DXT5");
const LPCTSTR TEXTURE_FORMAT_R3G3B2 = _T("R3G3B2");
const LPCTSTR TEXTURE_FORMAT_A8 = _T("A8");

/////////////////////////////////////////////////////////////////////////
// CTexture

CTexture::CTexture() :
	m_idTexture(SSCL_RESOURCEID_NULL),
	m_sourceType(TEXTURE_NONE),
	m_hResModule(NULL)
{
	m_nWidth = m_nHeight = 0;
	m_nLevels = 1;
	m_format = D3DFMT_UNKNOWN;
}

CTexture::~CTexture()
{
	FreeImageSrc();
}

HRESULT CTexture::FinalConstruct()
{
	return put_Name(tag_Texture);
}

STDMETHODIMP CTexture::SelectObject(LPDIRECT3DDEVICE8 pDevice,
									IResourceManagerDX8* pDevResources,
									DWORD dwTextureStage)
{
	HRESULT hr = E_FAIL;
	LPDIRECT3DTEXTURE8 pTexture = NULL;

	hr = pDevResources->GetTexture(m_idTexture, &pTexture);

	if (hr == S_OK)
	{
		hr = pDevice->SetTexture(dwTextureStage, pTexture);
		pTexture->Release();
	}

	return hr;
}

STDMETHODIMP CTexture::LoadDevice(/* [in] */ IDevice* pDevice)
{
	HRESULT hr = S_OK;
	LPDIRECT3DTEXTURE8 pTextureRes = NULL;
	LPDIRECT3DDEVICE8 pD3DDevice = NULL;
	D3DXIMAGE_INFO d3dxImageInfo;

	IResourceManagerDX8Ptr spResMgr(pDevice);

	if (spResMgr != NULL)
	{
		if (m_idTexture == SSCL_RESOURCEID_NULL)
		{
			hr = spResMgr->GenerateResourceId(D3DRTYPE_TEXTURE, &m_idTexture);
		}
		else
		{
			hr = spResMgr->ReserveResourceId(D3DRTYPE_TEXTURE, m_idTexture);
		}

		if (hr == S_OK)
		{
			hr = spResMgr->GetD3DDevice8(&pD3DDevice);

			if (hr == S_OK)
			{
				if (m_sourceType == TEXTURE_FILE)
				{
					_bstr_t strFullName = GetFullPath(_bstr_t(m_imageSrc.fileName), _bstr_t("Textures"));

					hr = D3DXCreateTextureFromFileExW(pD3DDevice,
					                                  strFullName,
					                                  m_nWidth,
					                                  m_nHeight,
					                                  m_nLevels,
					                                  0,
					                                  D3DFMT_UNKNOWN,
					                                  D3DPOOL_DEFAULT,
					                                  D3DX_DEFAULT,
					                                  D3DX_DEFAULT,
					                                  0,
					                                  &d3dxImageInfo,
					                                  NULL,
					                                  &pTextureRes);

					if (FAILED(hr))
					{
						// Failure to find the texture file should not prevent the
						// rest of the model from loading.
						hr = S_FALSE;
					}
				}
				else if (m_sourceType == TEXTURE_URL)
				{
					hr = InternetLoadTexture(pD3DDevice, _bstr_t(m_imageSrc.fileName), &pTextureRes);
				}
				else if (m_sourceType == TEXTURE_RESOURCE && m_hResModule != NULL && m_imageSrc.resStr != NULL)
				{
					hr = D3DXCreateTextureFromResourceExW(pD3DDevice,
														  m_hResModule,
														  m_imageSrc.resStr,
														  m_nWidth,
														  m_nHeight,
														  m_nLevels,
														  0,
														  D3DFMT_UNKNOWN,
														  D3DPOOL_DEFAULT,
														  D3DX_DEFAULT,
														  D3DX_DEFAULT,
														  0,
														  NULL,
														  NULL,
														  &pTextureRes);
				}
				else if (m_sourceType == TEXTURE_MEMORY && m_imageSrc.hDIB != NULL)
				{
					LPBITMAPINFO lpBitInfo = (LPBITMAPINFO) ::GlobalLock(m_imageSrc.hDIB);

					if (lpBitInfo != NULL)
					{
						hr = D3DXCreateTextureFromFileInMemoryEx(pD3DDevice,
																 lpBitInfo,
																 ::GlobalSize(m_imageSrc.hDIB),
																 m_nWidth,
																 m_nHeight,
																 m_nLevels,
																 0,
																 D3DFMT_UNKNOWN,
																 D3DPOOL_DEFAULT,
																 D3DX_DEFAULT,
																 D3DX_DEFAULT,
																 0,
																 NULL,
																 NULL,
																 &pTextureRes);
						::GlobalUnlock(m_imageSrc.hDIB);
					}
				}

				pD3DDevice->Release();

				if (pTextureRes != NULL)
				{
					if (hr == D3D_OK)
					{
						hr = spResMgr->SetTexture(m_idTexture, pTextureRes);
					}
					pTextureRes->Release();
				}
			}
		}
	}

	return hr;
}

STDMETHODIMP CTexture::UnloadDevice(/* [in] */ IDevice* pDevice)
{
	HRESULT hr = S_OK;
	IResourceManagerDX8Ptr spResMgr(pDevice);

	if (spResMgr != NULL)
	{
		hr = spResMgr->FreeResource(D3DRTYPE_TEXTURE, m_idTexture);
	}

	return hr;
}

STDMETHODIMP CTexture::get_Width(/* [out, retval] */ long *pVal)
{
	*pVal = static_cast<long>(m_nWidth);
	return S_OK;
}

STDMETHODIMP CTexture::put_Width(/* [in] */ long newVal)
{
	m_nWidth = static_cast<DWORD>(newVal);
	return S_OK;
}

STDMETHODIMP CTexture::get_Height(/* [out, retval] */ long *pVal)
{
	*pVal = static_cast<long>(m_nHeight);
	return S_OK;
}

STDMETHODIMP CTexture::put_Height(/* [in] */ long newVal)
{
	m_nHeight = static_cast<DWORD>(newVal);
	return S_OK;
}

STDMETHODIMP CTexture::get_FileName(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	if (m_sourceType == TEXTURE_FILE && m_imageSrc.fileName != NULL)
	{
		*pVal = ::SysAllocString(m_imageSrc.fileName);
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CTexture::put_FileName(/* [in] */ BSTR newVal)
{
	FreeImageSrc();

	if (InternetIsURL(newVal))
	{
		m_sourceType = TEXTURE_URL;
	}
	else
	{
		m_sourceType = TEXTURE_FILE;
	}

	m_imageSrc.fileName = ::SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CTexture::get_TextureId(/* [out, retval] */ DeviceResourceId *pVal)
{
	*pVal = m_idTexture;
	return S_OK;
}

STDMETHODIMP CTexture::put_TextureId(/* [in] */ DeviceResourceId newVal)
{
	m_idTexture = newVal;
	return S_OK;
}

STDMETHODIMP CTexture::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Texture;
	return S_OK;
}

STDMETHODIMP CTexture::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;
	VARIANT val;

	pArchive->GetContext(&nContext);

	hr = ReadElement(pArchive, nContext, tag_Name, &CTexture::put_Name);

	if (SUCCEEDED(ReadElement(pArchive, nContext, tag_ResourceID, VT_I4, &val)))
	{
		m_idTexture = val.lVal;
		::VariantClear(&val);
	}

	if (SUCCEEDED(ReadElement(pArchive, nContext, tag_FileName, VT_BSTR, &val)))
	{
		put_FileName(val.bstrVal);
		::VariantClear(&val);
	}

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CTexture::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;
	pArchive->GetContext(&nContext);

	hr = WriteElement(pArchive, nContext, tag_Name, &CTexture::get_Name);

	WriteElement(pArchive, nContext, tag_ResourceID, _variant_t(m_idTexture), NULL);

	if (m_sourceType == TEXTURE_FILE)
	{
		WriteElement(pArchive, nContext, tag_FileName, _variant_t(m_imageSrc.fileName), NULL);
	}

	pArchive->FreeContext(nContext);

	return hr;
}

void CTexture::FreeImageSrc()
{
	if (m_sourceType == TEXTURE_FILE && m_imageSrc.fileName != NULL)
	{
		::SysFreeString(m_imageSrc.fileName);
		m_imageSrc.fileName = NULL;
	}
	else if (m_sourceType == TEXTURE_RESOURCE && m_imageSrc.resStr != NULL)
	{
		::SysFreeString(m_imageSrc.resStr);
		m_imageSrc.resStr = NULL;
	}
	else if (m_sourceType == TEXTURE_MEMORY && m_imageSrc.hDIB != 0)
	{
		m_imageSrc.hDIB = 0;
	}

	m_sourceType = TEXTURE_NONE;
}

};  // namespace ShapeShifter
