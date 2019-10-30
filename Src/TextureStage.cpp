//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: TextureStage.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "TextureStage.h"
#include "Texture.h"
#include "Material.h"
#include "Property.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// Property table for supporting late-bound properties

SSCL_DEFINE_PROPERTY_ACCESSOR(CTextureStage, MagFilter)
SSCL_DEFINE_PROPERTY_ACCESSOR(CTextureStage, MinFilter)
SSCL_DEFINE_PROPERTY_ACCESSOR(CTextureStage, MipFilter)
SSCL_DEFINE_PROPERTY_ACCESSOR(CTextureStage, TextureAddressU)
SSCL_DEFINE_PROPERTY_ACCESSOR(CTextureStage, TextureAddressV)

SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(CTextureStage)
	SSCL_RUNTIME_PROPERTY_ENTRY(MagFilter, SSCLID_MAGFILTER, VT_I2)
	SSCL_RUNTIME_PROPERTY_ENTRY(MinFilter, SSCLID_MINFILTER, VT_I2)
	SSCL_RUNTIME_PROPERTY_ENTRY(MipFilter, SSCLID_MIPFILTER, VT_I2)
	SSCL_RUNTIME_PROPERTY_ENTRY(TextureAddressU, SSCLID_TEXTURE_ADDRESS_U, VT_I2)
	SSCL_RUNTIME_PROPERTY_ENTRY(TextureAddressV, SSCLID_TEXTURE_ADDRESS_V, VT_I2)
	SSCL_RUNTIME_PROPERTY_ENTRY_NULL
SSCL_END_RUNTIME_PROPERTY_TABLE

/////////////////////////////////////////////////////////////////////////
// CTextureStage::_State

CTextureStage::_State::_State(const D3DTEXTURESTAGESTATETYPE d3dTypeIn,
                              const DWORD dwValIn)
{
	d3dStateType = d3dTypeIn;
	dwStateValue = dwValIn;
}

CTextureStage::_State::_State(const CTextureStage::_State& src)
{
	d3dStateType = src.d3dStateType;
	dwStateValue = src.dwStateValue;
}

CTextureStage::_State& CTextureStage::_State::operator=(const CTextureStage::_State& src)
{
	d3dStateType = src.d3dStateType;
	dwStateValue = src.dwStateValue;
	return *this;
}

/////////////////////////////////////////////////////////////////////////
// CTextureStage

const LPOLESTR TEXTURE_CHILD_NAME = L"Texture";

CTextureStage::CTextureStage() :
	m_dwStageNum(0),
	m_pTexture(NULL),
	m_bOwnsTexture(false)
{
}

CTextureStage::~CTextureStage()
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
	}
}

HRESULT CTextureStage::FinalConstruct()
{
	return put_Name(tag_TextureStage);
}

STDMETHODIMP CTextureStage::get_StageNum(/* [out, retval] */ long *pVal)
{
	if (pVal != NULL)
	{
		*pVal = static_cast<long>(m_dwStageNum);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CTextureStage::put_StageNum(/* [in] */ long newVal)
{
	m_dwStageNum = static_cast<DWORD>(newVal);
	return S_OK;
}

STDMETHODIMP CTextureStage::get_Texture(/* [out, retval] */ ITexture** ppTexture)
{
	HRESULT hr = S_OK;

	if (ppTexture == NULL)
	{
		hr = E_POINTER;
	}
	else
	{
		if (m_pTexture == NULL)
		{
			// Create a default texture if one doesn't exist
			hr = CTexture::_CreatorClass::CreateInstance(NULL, IID_ITexture, (LPVOID*) &m_pTexture);
		}

		if (SUCCEEDED(hr) && m_pTexture != NULL)
		{
			m_bOwnsTexture = true;
			m_pTexture->putref_Parent(this);
			m_pTexture->put_Name(_bstr_t(TEXTURE_CHILD_NAME));
			*ppTexture = m_pTexture;
			(*ppTexture)->AddRef();
		}
	}

	return hr;
}

STDMETHODIMP CTextureStage::putref_Texture(/* [in] */ ITexture* pTexture)
{
	if (pTexture != NULL)
	{
		pTexture->AddRef();
	}

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
	}

	m_pTexture = pTexture;
	m_bOwnsTexture = false;

	return S_OK;
}

STDMETHODIMP CTextureStage::SetTextureState(/* [in] */ short nStateType,
                                            /* [in] */ long nStateValue)
{
	HRESULT hr = S_OK;

	D3DTEXTURESTAGESTATETYPE d3dStateType = INT_TO_D3DTEXTURESTAGESTATETYPE(nStateType);
	DWORD dwStateValue = static_cast<DWORD>(nStateValue);

	_StateArray::iterator itStates = FindTextureStageState(d3dStateType);

	if (itStates != m_states.end())
	{
		itStates->dwStateValue = dwStateValue;
	}
	else
	{
		m_states.push_back(_State(d3dStateType, dwStateValue));
	}

	return hr;
}

STDMETHODIMP CTextureStage::SetTextureStateFloat(/* [in] */ short nStateType,
                                                 /* [in] */ float fStateValue)
{
	HRESULT hr = S_OK;

	D3DTEXTURESTAGESTATETYPE d3dStateType = INT_TO_D3DTEXTURESTAGESTATETYPE(nStateType);
	DWORD dwStateValue = *((LPDWORD) (&fStateValue));

	_StateArray::iterator itStates = FindTextureStageState(d3dStateType);

	if (itStates != m_states.end())
	{
		itStates->dwStateValue = dwStateValue;
	}
	else
	{
		m_states.push_back(_State(d3dStateType, dwStateValue));
	}

	return hr;
}

STDMETHODIMP CTextureStage::SetTextureStateRGBA(/* [in] */ short nStateType,
                                                /* [in] */ VALUE3D fRed,
                                                /* [in] */ VALUE3D fGreen,
                                                /* [in] */ VALUE3D fBlue,
                                                /* [in] */ VALUE3D fAlpha)
{
	HRESULT hr = S_OK;

	D3DTEXTURESTAGESTATETYPE d3dStateType = INT_TO_D3DTEXTURESTAGESTATETYPE(nStateType);
	DWORD dwStateValue = D3DCOLOR_COLORVALUE(fRed, fGreen, fBlue, fAlpha);

	_StateArray::iterator itStates = FindTextureStageState(d3dStateType);

	if (itStates != m_states.end())
	{
		itStates->dwStateValue = dwStateValue;
	}
	else
	{
		m_states.push_back(_State(d3dStateType, dwStateValue));
	}

	return hr;
}

STDMETHODIMP CTextureStage::ClearTextureState(/* [in] */ short nStateType)
{
	HRESULT hr = E_FAIL;

	D3DTEXTURESTAGESTATETYPE d3dStateType = INT_TO_D3DTEXTURESTAGESTATETYPE(nStateType);

	_StateArray::iterator itStates = FindTextureStageState(d3dStateType);

	if (itStates != m_states.end())
	{
		m_states.erase(itStates);
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CTextureStage::ClearAllTextureStates()
{
	m_states.clear();
	return S_OK;
}

STDMETHODIMP CTextureStage::GetChildCount(/* [out,retval] */ long* pNumChildren)
{
	if (pNumChildren == NULL)
	{
		return E_POINTER;
	}

	if (m_bOwnsTexture)
	{
		*pNumChildren = 1;
	}
	else
	{
		*pNumChildren = 0;
	}

	return S_OK;
}

STDMETHODIMP CTextureStage::GetChildIndex(/* [in] */ BSTR strName, /* [out,retval] */ long* pChildIndex)
{
	HRESULT hr = S_FALSE;

	if (m_bOwnsTexture && m_pTexture != NULL)
	{
		BSTR strTextureName;
		if (m_pTexture->get_Name(&strTextureName) == S_OK)
		{
			if (_bstr_t(strTextureName) == _bstr_t(strName))
			{
				hr = S_OK;

				if (*pChildIndex != NULL)
				{
					*pChildIndex = 0;
				}
			}

			::SysFreeString(strTextureName);
		}
	}

	return hr;
}

STDMETHODIMP CTextureStage::GetChild(/* [in] */ long nChildIndex, /* [out,retval] */ INode** ppChild)
{
	if (ppChild == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_bOwnsTexture && nChildIndex == 0)
	{
		*ppChild = m_pTexture;

		if (*ppChild != NULL)
		{
			(*ppChild)->AddRef();
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CTextureStage::GetChildByName(/* [in] */ BSTR strName, /* [out,retval] */ INode** ppNode)
{
	if (ppNode == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_bOwnsTexture && m_pTexture != NULL)
	{
		BSTR strTextureName;
		if (SUCCEEDED(m_pTexture->get_Name(&strTextureName)))
		{
			if (_bstr_t(strName) == _bstr_t(strTextureName))
			{
				hr = m_pTexture->QueryInterface(IID_INode, (LPVOID*) ppNode);
			}
			::SysFreeString(strTextureName);
		}
	}

	return hr;
}

STDMETHODIMP CTextureStage::AppendChild(/* [in] */ INode* pChild)
{
	pChild;
	return E_FAIL;
}

STDMETHODIMP CTextureStage::InsertChild(/* [in] */ INode* pChild, /* [in] */ long nChildIndex)
{
	pChild;
	nChildIndex;
	return E_FAIL;
}

STDMETHODIMP CTextureStage::RemoveChild(/* [in] */ INode* pChild)
{
	pChild;
	return E_FAIL;
}

STDMETHODIMP CTextureStage::RemoveAllChildren()
{
	return E_FAIL;
}

STDMETHODIMP CTextureStage::OnChildUpdate(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode)
{
	pNode;
	nNotifyCode;
	return S_OK;
}

STDMETHODIMP CTextureStage::LoadDevice(/* [in] */ IDevice* pDevice)
{
	HRESULT hr = S_OK;

	if (m_bOwnsTexture && m_pTexture != NULL)
	{
		hr = m_pTexture->LoadDevice(pDevice);
	}

	return hr;
}

STDMETHODIMP CTextureStage::UnloadDevice(/* [in] */ IDevice* pDevice)
{
	HRESULT hr = S_OK;

	if (m_bOwnsTexture && m_pTexture != NULL)
	{
		hr = m_pTexture->UnloadDevice(pDevice);
	}

	return hr;
}

STDMETHODIMP CTextureStage::SelectObject(LPDIRECT3DDEVICE8 pDevice,
										 IResourceManagerDX8* pDevResources,
										 DWORD dwTextureStage)
{
	HRESULT hr = S_OK;

	dwTextureStage;

	_StateArray::iterator itStates = m_states.begin();

	while (hr == S_OK && itStates != m_states.end())
	{
		hr = pDevice->SetTextureStageState(0, itStates->d3dStateType, itStates->dwStateValue);
		itStates++;
	}

	IContextObject* pTextureContext = NULL;

	if (hr == S_OK && m_pTexture != NULL)
	{
		if (SUCCEEDED(m_pTexture->QueryInterface(IID_IContextObject, (LPVOID*) &pTextureContext)))
		{
			hr = pTextureContext->SelectObject(pDevice, pDevResources, m_dwStageNum);
			pTextureContext->Release();
		}
	}

	return hr;
}

STDMETHODIMP CTextureStage::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_TextureStage;
	return S_OK;
}

STDMETHODIMP CTextureStage::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	VARIANT val;
	long nContext;

	pArchive->GetContext(&nContext);

	m_bOwnsTexture = false;
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	hr = ReadElement(pArchive, nContext, tag_Name, &CTextureStage::put_Name);

	if (SUCCEEDED(ReadElement(pArchive, nContext, tag_StageNum, VT_I4, &val)))
	{
		m_dwStageNum = val.lVal;
	}

	IUnknown* pTextureUnk = NULL;

	if (SUCCEEDED(hr))
	{
		if (pArchive->GotoNode(tag_Children) == S_OK)
		{
			hr = ReadObject(pArchive, tag_Texture, &pTextureUnk);

			if (hr == S_OK)
			{
				m_bOwnsTexture = true;
			}
		}
	}

	pArchive->SetContext(nContext);

	if (SUCCEEDED(hr))
	{
		if (pTextureUnk == NULL)
		{
			hr = ReadObjectReference(pArchive, tag_Texture, &pTextureUnk);
		}

		if (pTextureUnk != NULL)
		{
			hr = pTextureUnk->QueryInterface(IID_ITexture, (LPVOID*) &m_pTexture);
			pTextureUnk->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		if (pArchive->GotoNode(tag_TextureStageStates) == S_OK)
		{
			HRESULT hrLoop = pArchive->GotoFirstChild();

			while (hrLoop == S_OK)
			{
				hrLoop = pArchive->GotoNextSibling();
			}
		}
	}

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CTextureStage::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;
	pArchive->GetContext(&nContext);

	hr = WriteElement(pArchive, nContext, tag_Name, &CTextureStage::get_Name);

	WriteElement(pArchive, nContext, tag_StageNum, _variant_t((long)m_dwStageNum), NULL);

	pArchive->WriteObjectReference(m_pTexture);

	if (m_bOwnsTexture)
	{
		if (pArchive->CreateElement(tag_Children) == S_OK)
		{
			pArchive->WriteObject(m_pTexture);
		}
	}

	pArchive->SetContext(nContext);

	// Write each object in the texture stage
	hr = pArchive->CreateElement(tag_TextureStageStates);

	if (hr == S_OK)
	{
		long nContext2;
		pArchive->GetContext(&nContext2);

		_StateArray::iterator itStates = m_states.begin();

		while (hr == S_OK && itStates != m_states.end())
		{
			pArchive->CreateElement(tag_TextureStageState);
			pArchive->WriteAttribute(tag_Type, _variant_t((long)itStates->d3dStateType));
			pArchive->WriteAttribute(tag_Value, _variant_t((long)itStates->dwStateValue));
			itStates++;
			pArchive->SetContext(nContext2);
		}

		pArchive->FreeContext(nContext2);
	}

	pArchive->FreeContext(nContext);

	return hr;
}

CTextureStage::_StateArray::iterator CTextureStage::FindTextureStageState(const D3DTEXTURESTAGESTATETYPE d3dStateType)
{
	_StateArray::iterator itStates = m_states.begin();

	while (itStates != m_states.end())
	{
		if (itStates->d3dStateType == d3dStateType)
		{
			return itStates;
		}
		itStates++;
	}

	return m_states.end();
}

HRESULT CTextureStage::get_MagFilter(short* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_StateArray::iterator itStates = FindTextureStageState(D3DTSS_MAGFILTER);

	if (itStates != m_states.end())
	{
		*pVal = static_cast<short>(itStates->dwStateValue);
		return S_OK;
	}

	return S_FALSE;
}

HRESULT CTextureStage::put_MagFilter(short newVal)
{
	return SetTextureState(D3DTSS_MAGFILTER, INT_TO_D3DTEXTUREFILTERTYPE(newVal));
}

HRESULT CTextureStage::get_MinFilter(short* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_StateArray::iterator itStates = FindTextureStageState(D3DTSS_MINFILTER);

	if (itStates != m_states.end())
	{
		*pVal = static_cast<short>(itStates->dwStateValue);
		return S_OK;
	}

	return S_FALSE;
}

HRESULT CTextureStage::put_MinFilter(short newVal)
{
	return SetTextureState(D3DTSS_MINFILTER, INT_TO_D3DTEXTUREFILTERTYPE(newVal));
}

HRESULT CTextureStage::get_MipFilter(short* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_StateArray::iterator itStates = FindTextureStageState(D3DTSS_MIPFILTER);

	if (itStates != m_states.end())
	{
		*pVal = static_cast<short>(itStates->dwStateValue);
		return S_OK;
	}

	return S_FALSE;
}

HRESULT CTextureStage::put_MipFilter(short newVal)
{
	return SetTextureState(D3DTSS_MIPFILTER, INT_TO_D3DTEXTUREFILTERTYPE(newVal));
}

HRESULT CTextureStage::get_TextureAddressU(short* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_StateArray::iterator itStates = FindTextureStageState(D3DTSS_ADDRESSU);

	if (itStates != m_states.end())
	{
		*pVal = static_cast<short>(itStates->dwStateValue);
		return S_OK;
	}

	return S_FALSE;
}

HRESULT CTextureStage::put_TextureAddressU(short newVal)
{
	return SetTextureState(D3DTSS_ADDRESSU, INT_TO_D3DTEXTUREADDRESS(newVal));
}

HRESULT CTextureStage::get_TextureAddressV(short* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	_StateArray::iterator itStates = FindTextureStageState(D3DTSS_ADDRESSV);

	if (itStates != m_states.end())
	{
		*pVal = static_cast<short>(itStates->dwStateValue);
		return S_OK;
	}

	return S_FALSE;
}

HRESULT CTextureStage::put_TextureAddressV(short newVal)
{
	return SetTextureState(D3DTSS_ADDRESSV, INT_TO_D3DTEXTUREADDRESS(newVal));
}

};  // namespace ShapeShifter
