//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Shape.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Shape.h"
#include "XMLArchive.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CShape

SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(CShape)
	SSCL_RUNTIME_PROPERTY_ENTRY_NULL
SSCL_END_RUNTIME_PROPERTY_TABLE

CShape::CShape()
{
}

CShape::~CShape()
{
}

HRESULT CShape::FinalConstruct()
{
	return put_Name(tag_Shape);
}

STDMETHODIMP CShape::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Shape;
	return S_OK;
}

STDMETHODIMP CShape::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nObjRoot;
	pArchive->GetContext(&nObjRoot);

	hr = ReadElement(pArchive, nObjRoot, tag_Name, &CShape::put_Name);

	// Read the transformation matrix
	hr = ReadTransformationMatrix(pArchive, nObjRoot, tag_TransformationMatrix, m_mat);

	if (SUCCEEDED(hr))
	{
		// Read pointer to the mesh object
		IUnknown* pMeshUnk = NULL;
		hr = ReadObject(pArchive, tag_Mesh, &pMeshUnk);
		if (hr == S_OK)
		{
			IMesh* pMesh = NULL;
			hr = pMeshUnk->QueryInterface(IID_IMesh, (LPVOID*) &pMesh);
			if (SUCCEEDED(hr))
			{
				putref_Mesh(pMesh);
			}
		}
	}

	// Read the context objects

	HRESULT hrXML = pArchive->GotoFirstChild();

	while (hrXML == S_OK)
	{
		if (pArchive->CompareCurrentTag(tag_Context) == S_OK)
		{
			long nContext;
			pArchive->GetContext(&nContext);

			// Read primitive index
			VARIANT varAttr;
			hr = pArchive->ReadAttribute(tag_PrimitiveIndex, &varAttr);
			if (hr == S_OK)
			{
				pArchive->GotoFirstChild();

				_variant_t varPrimitiveIdx = varAttr;
				varPrimitiveIdx.ChangeType(VT_I4);
				PrimitiveIndex nPrimIdx = static_cast<PrimitiveIndex>((long) varPrimitiveIdx);

				IUnknown* pContextObjUnk = NULL;
				hr = pArchive->ReadObject(&pContextObjUnk);
				if (hr == S_OK && pContextObjUnk != NULL)
				{
					IContextObject* pContextObj = NULL;
					if (SUCCEEDED(pContextObjUnk->QueryInterface(IID_IContextObject, (LPVOID*) &pContextObj)))
					{
						m_contexts[nPrimIdx] = pContextObj;
					}
					pContextObjUnk->Release();
				}
			}

			SetAndFreeContext(pArchive, nContext);
		}

		hrXML = pArchive->GotoNextSibling();
	}

	SetAndFreeContext(pArchive, nObjRoot);

	return hr;
}

STDMETHODIMP CShape::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nObjRoot;
	pArchive->GetContext(&nObjRoot);

	hr = WriteElement(pArchive, nObjRoot, tag_Name, &CShape::get_Name);

	// Write the transformation matrix
	hr = pArchive->CreateElement(tag_TransformationMatrix);
	hr = WriteTransformationMatrix(pArchive, m_mat);
	hr = pArchive->SetContext(nObjRoot);

	// Write pointer to mesh object
	if (hr == S_OK)
	{
		if (m_pMesh != NULL)
		{
			hr = pArchive->WriteObject(m_pMesh);
		}
	}

	// Write the context object map

	if (hr == S_OK)
	{
		long nContext2;
		pArchive->GetContext(&nContext2);

		// Iterate through each context object list in the map
		_ContextMap::iterator itContext = m_contexts.begin();
		while (itContext != m_contexts.end())
		{
			PrimitiveIndex nPrimIdx = itContext->first;
			IContextObject* pContextObj = itContext->second;
			IPersistXML* pObjPersist = NULL;

			if (SUCCEEDED(pContextObj->QueryInterface(IID_IPersistXML, (LPVOID*) &pObjPersist)))
			{
				// Create a node for the context object list
				hr = pArchive->CreateElement(tag_Context);

				if (SUCCEEDED(hr))
				{
					hr = pArchive->WriteAttribute(tag_PrimitiveIndex, _variant_t(nPrimIdx));
				}

				if (SUCCEEDED(hr))
				{
					hr = pArchive->WriteObject(pObjPersist);
				}

				pObjPersist->Release();
			}

			pArchive->SetContext(nContext2);

			itContext++;
		}

		pArchive->FreeContext(nContext2);
	}

	SetAndFreeContext(pArchive, nObjRoot);

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CCompositeShape

SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(CCompositeShape)
	SSCL_RUNTIME_PROPERTY_ENTRY_NULL
SSCL_END_RUNTIME_PROPERTY_TABLE

CCompositeShape::CCompositeShape()
{
}

CCompositeShape::~CCompositeShape()
{
}

HRESULT CCompositeShape::FinalConstruct()
{
	return put_Name(tag_Shape);
}

STDMETHODIMP CCompositeShape::get_Mesh(/* [out, retval] */ IMesh** ppVal)
{
	if (ppVal == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	if (m_pMesh != NULL)
	{
		hr = m_pMesh->QueryInterface(IID_IMesh, (LPVOID*) ppVal);
	}
	else
	{
		// Create a default mesh since one doesn't exist and
		// add it as a child

		hr = CMesh::_CreatorClass::CreateInstance(NULL, IID_IMeshEx, (LPVOID*) &m_pMesh);

		if (SUCCEEDED(hr) && m_pMesh != NULL)
		{
			m_spMeshDraw = m_pMesh;
			m_pMesh->put_Name(_bstr_t("Mesh"));
			hr = AppendChild(m_pMesh);

			if (hr == S_OK)
			{
				hr = m_pMesh->QueryInterface(IID_IMesh, (LPVOID*) ppVal);
			}
		}
	}

	return hr;
}

STDMETHODIMP CCompositeShape::GetContext(/* [in] */ PrimitiveIndex nPrimIdx,
                                         /* [out,retval] */ INode** ppContextNode)
{
	if (ppContextNode == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;
	INode* pContextNode = NULL;

	_ContextMap::iterator itContext = m_contexts.find(nPrimIdx);

	if (itContext != m_contexts.end())
	{
		hr = (itContext->second)->QueryInterface(IID_INode, (LPVOID*) ppContextNode);
	}
	else
	{
		// Create a context and return it
		IContextObject* pContextObj = NULL;
		hr = CRenderingContext::_CreatorClass::CreateInstance(NULL, IID_IContextObject, (LPVOID*) &pContextObj);
		if (SUCCEEDED(hr))
		{
			m_contexts[nPrimIdx] = pContextObj;
			hr = pContextObj->QueryInterface(IID_INode, (LPVOID*) &pContextNode);

			if (SUCCEEDED(hr) && pContextNode != NULL)
			{
				pContextNode->put_Name(_bstr_t("Context") + _bstr_t(nPrimIdx));

				hr = AppendChild(pContextNode);

				if (SUCCEEDED(hr))
				{
					*ppContextNode = pContextNode;
				}
			}
		}
	}

	return hr;
}

#if 0
STDMETHODIMP CCompositeShape::Render(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
{
	HRESULT hr = E_FAIL;

	hr = PushTransform(pDevice);

	if (hr == S_OK)
	{
		if (m_spMeshDraw != NULL)
		{
			hr = m_spMeshDraw->Draw(pDevice, pDevResources, this);
		}

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

		hr = PopTransform(pDevice);
	}

	return hr;
}
#else
STDMETHODIMP CCompositeShape::Render(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
{
	HRESULT hr = S_OK;

	PushTransform(pDevice);

	if (m_pMesh != NULL && m_spMeshDraw != NULL)
	{
		DWORD dwCurState;
		BOOL bBuffered = FALSE;

		hr = m_spMeshDraw->BeginDraw(pDevice, pDevResources, &bBuffered);

		if (SUCCEEDED(hr))
		{
			hr = pDevice->CreateStateBlock(D3DSBT_ALL, &dwCurState);

			if (SUCCEEDED(hr))
			{
				if (bBuffered)
				{
					PrimitiveCount nNumPrimitives;
					m_pMesh->GetPrimitiveCount(&nNumPrimitives);

					for (PrimitiveIndex nPrimIdx = 0; nPrimIdx < nNumPrimitives; nPrimIdx++)
					{
						_ContextMap::iterator itContext = m_contexts.find(nPrimIdx);

						if (itContext != m_contexts.end())
						{
							(itContext->second)->SelectObject(pDevice, pDevResources, 0);
						}

						m_spMeshDraw->DrawPrimitiveBuffered(pDevice, pDevResources, nPrimIdx);
						pDevice->ApplyStateBlock(dwCurState);
					}
				}
				else
				{
					PrimitiveCount nNumPrimitives;
					m_pMesh->GetPrimitiveCount(&nNumPrimitives);

					for (PrimitiveIndex nPrimIdx = 0; nPrimIdx < nNumPrimitives; nPrimIdx++)
					{
						_ContextMap::iterator itContext = m_contexts.find(nPrimIdx);

						if (itContext != m_contexts.end())
						{
							(itContext->second)->SelectObject(pDevice, pDevResources, 0);
						}

						m_spMeshDraw->DrawPrimitive(pDevice, pDevResources, nPrimIdx);
						pDevice->ApplyStateBlock(dwCurState);
					}
				}
			}

#if 0			// Optimized out for now. Left in interface in case it's needed later
			m_spMeshDraw->EndDraw(pDevice, pDevResources);
#endif

			pDevice->DeleteStateBlock(dwCurState);
		}
		else
		{
			hr = E_FAIL;
		}
	}

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

	PopTransform(pDevice);

	return hr;
}
#endif

STDMETHODIMP CCompositeShape::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_CompositeShape;
	return S_OK;
}

STDMETHODIMP CCompositeShape::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nObjRoot;
	pArchive->GetContext(&nObjRoot);

	hr = ReadElement(pArchive, nObjRoot, tag_Name, &CCompositeShape::put_Name);

	// Read the transformation matrix
	hr = ReadTransformationMatrix(pArchive, nObjRoot, tag_TransformationMatrix, m_mat);

	if (SUCCEEDED(hr))
	{
		hr = ReadObjectCollection(pArchive, nObjRoot, tag_Children, &CCompositeShape::PutChildUnk);
	}

	if (SUCCEEDED(hr))
	{
		// Read pointer to the mesh object
		IUnknown* pMeshUnk = NULL;
		hr = ReadObject(pArchive, tag_Mesh, &pMeshUnk);
		if (hr == S_OK)
		{
			IMesh* pMesh = NULL;
			hr = pMeshUnk->QueryInterface(IID_IMesh, (LPVOID*) &pMesh);
			if (SUCCEEDED(hr))
			{
				putref_Mesh(pMesh);
			}
		}
	}

	// Read the context objects

	HRESULT hrXML = pArchive->GotoFirstChild();

	while (hrXML == S_OK)
	{
		if (pArchive->CompareCurrentTag(tag_Context) == S_OK)
		{
			long nContext;
			pArchive->GetContext(&nContext);

			// Read primitive index
			VARIANT varAttr;
			hr = pArchive->ReadAttribute(tag_PrimitiveIndex, &varAttr);
			if (hr == S_OK)
			{
				pArchive->GotoFirstChild();

				_variant_t varPrimitiveIdx = varAttr;
				varPrimitiveIdx.ChangeType(VT_I4);
				PrimitiveIndex nPrimIdx = static_cast<PrimitiveIndex>((long) varPrimitiveIdx);

				IUnknown* pContextObjUnk = NULL;
				hr = pArchive->ReadObject(&pContextObjUnk);
				if (hr == S_OK && pContextObjUnk != NULL)
				{
					IContextObject* pContextObj = NULL;
					if (SUCCEEDED(pContextObjUnk->QueryInterface(IID_IContextObject, (LPVOID*) &pContextObj)))
					{
						m_contexts[nPrimIdx] = pContextObj;
					}
					pContextObjUnk->Release();
				}
			}

			SetAndFreeContext(pArchive, nContext);
		}

		hrXML = pArchive->GotoNextSibling();
	}

	SetAndFreeContext(pArchive, nObjRoot);

	return hr;
}

STDMETHODIMP CCompositeShape::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nObjRoot;
	pArchive->GetContext(&nObjRoot);

	hr = WriteElement(pArchive, nObjRoot, tag_Name, &CShape::get_Name);

	// Write the transformation matrix
	hr = pArchive->CreateElement(tag_TransformationMatrix);
	hr = WriteTransformationMatrix(pArchive, m_mat);
	hr = pArchive->SetContext(nObjRoot);

	hr = WriteObjectCollection(pArchive, nObjRoot, tag_Children, &CCompositeShape::GetChildUnk);

	pArchive->WriteObjectReference(m_pMesh);

	// Write the context object map

	if (hr == S_OK)
	{
		long nContext2;
		pArchive->GetContext(&nContext2);

		// Iterate through each context object list in the map
		_ContextMap::iterator itContext = m_contexts.begin();
		while (itContext != m_contexts.end())
		{
			PrimitiveIndex nPrimIdx = itContext->first;
			IContextObject* pContextObj = itContext->second;
			IPersistXML* pObjPersist = NULL;

			if (SUCCEEDED(pContextObj->QueryInterface(IID_IPersistXML, (LPVOID*) &pObjPersist)))
			{
				// Create a node for the context object list
				hr = pArchive->CreateElement(tag_Context);

				if (SUCCEEDED(hr))
				{
					hr = pArchive->WriteAttribute(tag_PrimitiveIndex, _variant_t(nPrimIdx));
				}

				if (SUCCEEDED(hr))
				{
					hr = pArchive->WriteObject(pObjPersist);
				}

				pObjPersist->Release();
			}

			pArchive->SetContext(nContext2);

			itContext++;
		}

		pArchive->FreeContext(nContext2);
	}

	SetAndFreeContext(pArchive, nObjRoot);

	return hr;
}

IUnknown* CCompositeShape::GetChildUnk(const long nIdx)
{
	if (nIdx < (long) m_children.size())
	{
		return (INode*) m_children[nIdx];
	}
	return NULL;
}

bool CCompositeShape::PutChildUnk(IUnknown* pUnk)
{
	bool bSuccess = false;
	INode* pNode = NULL;

	if (pUnk != NULL)
	{
		if (SUCCEEDED(pUnk->QueryInterface(IID_INode, (LPVOID*) &pNode)))
		{
			if (SUCCEEDED(AppendChild(pNode)))
			{
				bSuccess = true;
			}
			pNode->Release();
		}
	}

	return bSuccess;
}

};  // namespace ShapeShifter

