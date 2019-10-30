//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: RenderingContext.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "RenderingContext.h"

namespace ShapeShifter {

CRenderingContext::CRenderingContext()
{
}

CRenderingContext::~CRenderingContext()
{
}

HRESULT CRenderingContext::FinalConstruct()
{
	return put_Name(tag_RenderingContext);
}

STDMETHODIMP CRenderingContext::GetRenderState(/* [in] */ short nStateId,
                                               /* [out,retval] */ long* pStateVal)
{
	HRESULT hr = S_FALSE;

	D3DRENDERSTATETYPE d3dStateType = INT_TO_D3DRENDERSTATETYPE(nStateId);

	_StateMapType::iterator itMap = m_stateMap.find(d3dStateType);

	if (itMap != m_stateMap.end())
	{
		if (pStateVal != NULL)
		{
			*pStateVal = static_cast<long>(itMap->second);
		}
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CRenderingContext::SetRenderState(/* [in] */ short nStateId,
                                               /* [in] */ long nStateVal)
{
	D3DRENDERSTATETYPE d3dStateType = INT_TO_D3DRENDERSTATETYPE(nStateId);
	DWORD dwStateVal = static_cast<DWORD>(nStateVal);

	_StateMapType::iterator itMap = m_stateMap.find(d3dStateType);

	if (itMap == m_stateMap.end())
	{
		// Not found in map. Add the ID/Value pair now.
		m_stateMap[d3dStateType] = dwStateVal;
	}
	else
	{
		itMap->second = dwStateVal;
	}

	return S_OK;
}

STDMETHODIMP CRenderingContext::RemoveRenderState(/* [in] */ short nStateId)
{
	D3DRENDERSTATETYPE d3dStateType = INT_TO_D3DRENDERSTATETYPE(nStateId);
	_StateMapType::iterator itMap = m_stateMap.find(d3dStateType);

	HRESULT hr = S_FALSE;

	if (itMap != m_stateMap.end())
	{
		m_stateMap.erase(itMap);
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CRenderingContext::SelectObject(LPDIRECT3DDEVICE8 pDevice,
                                             IResourceManagerDX8* pDevResources,
                                             DWORD dwTextureStage)
{
	HRESULT hr = S_OK;

	_StateMapType::iterator itMap = m_stateMap.begin();

	while (hr == S_OK && itMap != m_stateMap.end())
	{
		hr = pDevice->SetRenderState(itMap->first, itMap->second);
		itMap++;
	}

	_NodeVector::iterator itChild;
	IContextObject* pContextObj = NULL;

	for (itChild = m_children.begin(); hr == S_OK && itChild != m_children.end(); itChild++)
	{
		pContextObj = (IContextObject*) *itChild;
		if (pContextObj != NULL)
		{
			hr = pContextObj->SelectObject(pDevice, pDevResources, dwTextureStage);
		}
	}

	return hr;
}

STDMETHODIMP CRenderingContext::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_RenderingContext;
	return S_OK;
}

STDMETHODIMP CRenderingContext::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;
	_variant_t varType;
	_variant_t varVal;

	pArchive->GetContext(&nContext);

	hr = ReadElement(pArchive, nContext, tag_Name, &CRenderingContext::put_Name);

	if (pArchive->GotoNode(tag_RenderStates) == S_OK)
	{
		HRESULT hrXML = pArchive->GotoFirstChild();

		while (hrXML == S_OK)
		{
			pArchive->ReadAttribute(tag_Type, &varType);
			pArchive->ReadAttribute(tag_Value, &varVal);
			SetRenderState(varType, varVal);
			hrXML = pArchive->GotoNextSibling();
		}
	}

	pArchive->SetContext(nContext);

	ReadObjectCollection(pArchive, nContext, tag_ContextObjects, &CRenderingContext::PutObjectUnk);

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CRenderingContext::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;
	pArchive->GetContext(&nContext);

	hr = WriteElement(pArchive, nContext, tag_Name, &CRenderingContext::get_Name);

	if (m_stateMap.size() > 0)
	{
		if (SUCCEEDED(pArchive->CreateElement(tag_RenderStates)))
		{
			_StateMapType::iterator itMap = m_stateMap.begin();

			while (hr == S_OK && itMap != m_stateMap.end())
			{
				pArchive->CreateElement(tag_RenderState);
				pArchive->WriteAttribute(tag_Type, _variant_t((long) itMap->first));
				pArchive->WriteAttribute(tag_Value, _variant_t((long) itMap->second));
				itMap++;
			}
		}
	}

	pArchive->SetContext(nContext);

	WriteObjectCollection(pArchive, nContext, tag_ContextObjects, &CRenderingContext::GetObjectUnk);

	pArchive->FreeContext(nContext);

	return hr;
}

IUnknown* CRenderingContext::GetObjectUnk(const long nIdx)
{
	if (nIdx < (long) m_children.size())
	{
		return (INode*) m_children[nIdx];
	}
	return NULL;
}

bool CRenderingContext::PutObjectUnk(IUnknown* pUnk)
{
	bool bSuccess = false;
	INode* pContextNode = NULL;

	if (pUnk != NULL)
	{
		if (SUCCEEDED(pUnk->QueryInterface(IID_INode, (LPVOID*) &pContextNode)))
		{
			if (SUCCEEDED(AppendChild(pContextNode)))
			{
				bSuccess = true;
			}
			pContextNode->Release();
		}
	}

	return bSuccess;
}

};  // namespace ShapeShifter

