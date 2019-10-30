//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Model.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Model.h"
#include "RenderingContext.h"
#include "NodeCollection.h"
#include "BSPTree.h"

namespace ShapeShifter {

CModel::CModel()
{
}

CModel::~CModel()
{
}

HRESULT CModel::FinalConstruct()
{
	put_Name(tag_Model);

	HRESULT hr = CBSPTree::_CreatorClass::CreateInstance(NULL, IID_IBSPTree, (LPVOID*) &m_pBSPTree);

	if (SUCCEEDED(hr))
	{
		m_pBSPTree->SetVolume(m_bounds.left, m_bounds.right,
		                      m_bounds.top, m_bounds.bottom,
							  m_bounds.front, m_bounds.back);
		m_pBSPTree->Initialize();
	}

	return hr;
}

STDMETHODIMP CModel::OnChildUpdate(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode)
{
	if (m_bBSPTreeEnabled && m_pBSPTree != NULL)
	{
		IVisualPtr spVisual(pNode);

		if (nNotifyCode & ChildNotification::ADD)
		{
			if (spVisual != NULL)
			{
				m_pBSPTree->AddObject(spVisual, VARIANT_FALSE);
			}
		}
		else if (nNotifyCode & ChildNotification::REMOVE)
		{
			if (spVisual != NULL)
			{
				m_pBSPTree->RemoveObject(spVisual, VARIANT_FALSE);
			}
		}
#if 0
		else if (nNotifyCode & ChildNotification::REMOVE_ALL)
		{
			if (spVisual != NULL)
			{
				m_pBSPTree->RemoveObject(spVisual, VARIANT_FALSE);
			}
		}
#endif
		else if (nNotifyCode & ChildNotification::BOUNDS)
		{
			if (spVisual != NULL)
			{
				m_pBSPTree->UpdateObject(spVisual, VARIANT_FALSE);
			}
		}
	}

	return _ModelBase::OnChildUpdate(pNode, nNotifyCode);
}

STDMETHODIMP CModel::SetBoundingBox(/* [in] */ VARIANT varBox)
{
	_ModelBase::SetBoundingBox(varBox);

	if (m_pBSPTree != NULL)
	{
		m_pBSPTree->SetVolume(m_bounds.left,
							  m_bounds.right,
							  m_bounds.top,
							  m_bounds.bottom,
							  m_bounds.front,
							  m_bounds.back);

		m_pBSPTree->Initialize();
	}

	return S_OK;
}

STDMETHODIMP CModel::GetChildrenIntersectingRay(/* [in] */ INodes* pNodeCollection,
                                                /* [in] */ VARIANT varRayOrigin,
                                                /* [in] */ VARIANT varRayDirection,
                                                /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	if (m_bBSPTreeEnabled)
	{
		hr = m_pBSPTree->GetChildrenIntersectingRay(pNodeCollection,
		                                            varRayOrigin,
		                                            varRayDirection,
		                                            strategy);
	}
	else
	{
		hr = _ModelBase::GetChildrenIntersectingRay(pNodeCollection, varRayOrigin, varRayDirection, strategy);
	}

	return hr;
}

STDMETHODIMP CModel::GetChildrenIntersectingSphere(/* [in] */ INodes* pNodeCollection,
                                                   /* [in] */ VARIANT varSphere,
                                                   /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	if (m_bBSPTreeEnabled)
	{
		hr = m_pBSPTree->GetChildrenIntersectingSphere(pNodeCollection, varSphere, strategy);
	}
	else
	{
		hr = _ModelBase::GetChildrenIntersectingSphere(pNodeCollection, varSphere, strategy);
	}

	return hr;
}

STDMETHODIMP CModel::GetChildrenIntersectingBox(/* [in] */ INodes* pNodeCollection,
                                                /* [in] */ VARIANT varBox,
                                                /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	if (m_bBSPTreeEnabled)
	{
		hr = m_pBSPTree->GetChildrenIntersectingBox(pNodeCollection, varBox, strategy);
	}
	else
	{
		hr = _ModelBase::GetChildrenIntersectingBox(pNodeCollection, varBox, strategy);
	}

	return hr;
}

STDMETHODIMP CModel::GetChildrenIntersectingPlane(/* [in] */ INodes* pNodeCollection,
                                                  /* [in] */ VARIANT varPlane,
                                                  /* [in] */ enum HitTestStrategy strategy)
{
	return _ModelBase::GetChildrenIntersectingPlane(pNodeCollection, varPlane, strategy);
}

STDMETHODIMP CModel::InsertNode(/* [in] */ INode* pNode,
                                /* [in] */ BSTR strParent,
                                /* [in] */ long nChildIndex)
{
	HRESULT hr = _ModelBase::InsertNode(pNode, strParent, nChildIndex);

	if (SUCCEEDED(hr) && m_nEventLocks == 0)
	{
		Fire_OnAddNode(pNode);
	}

	return hr;
}

STDMETHODIMP CModel::AppendNodes(/* [in] */ INodes* pNodeCollection, /* [in] */ BSTR strParent)
{
	HRESULT hr = _ModelBase::AppendNodes(pNodeCollection, strParent);

	if (SUCCEEDED(hr) && m_nEventLocks == 0)
	{
		INodesAccessor<INode> nodes(pNodeCollection);
		INode* pCurNode = NULL;

		HRESULT hrLoop = nodes.GetFirst(&pCurNode);
		while (hrLoop == S_OK)
		{
			Fire_OnAddNode(pCurNode);
			hrLoop = nodes.GetNext(&pCurNode);
		}
	}

	return hr;
}

STDMETHODIMP CModel::DeleteNode(/* [in] */ INode* pNode)
{
	HRESULT hr = _ModelBase::DeleteNode(pNode);

	if (SUCCEEDED(hr) && m_nEventLocks == 0)
	{
		Fire_OnRemoveNode(pNode);
	}

	return hr;
}

STDMETHODIMP CModel::DeleteNodes(/* [in] */ INodes* pNodeCollection)
{
	HRESULT hr = _ModelBase::DeleteNodes(pNodeCollection);

	if (SUCCEEDED(hr) && m_nEventLocks == 0)
	{
		INodesAccessor<INode> nodes(pNodeCollection);
		INode* pCurNode = NULL;

		HRESULT hrLoop = nodes.GetFirst(&pCurNode);
		while (hrLoop == S_OK)
		{
			Fire_OnRemoveNode(pCurNode);
			hrLoop = nodes.GetNext(&pCurNode);
		}
	}

	return hr;
}

STDMETHODIMP CModel::MoveNodes(/* [in] */ INodes* pNodeCollection,
                               /* [in] */ VALUE3D tx,
                               /* [in] */ VALUE3D ty,
                               /* [in] */ VALUE3D tz)
{
	HRESULT hr = _ModelBase::MoveNodes(pNodeCollection, tx, ty, tz);

	if (SUCCEEDED(hr) && m_nEventLocks == 0)
	{
		INodesAccessor<INode> nodes(pNodeCollection);
		INode* pCurNode = NULL;

		HRESULT hrLoop = nodes.GetFirst(&pCurNode);
		while (hrLoop == S_OK)
		{
			Fire_OnMoveNode(pCurNode);
			hrLoop = nodes.GetNext(&pCurNode);
		}
	}

	return hr;
}

STDMETHODIMP CModel::Render(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
{
	HRESULT hr = S_OK;

	if (!m_bCompiled)
	{
		// Compile child nodes into lists of rendering and context object interfaces.
		CompilePipeline();
	}

	PushTransform(pDevice);

	// Apply ambient light
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(m_ambientLight.r, m_ambientLight.g, m_ambientLight.b, m_ambientLight.a));

	// Select context objects
	_ContextObjectVector::iterator itContextNode;

	for (itContextNode = m_contextNodes.begin(); hr == S_OK && itContextNode != m_contextNodes.end(); itContextNode++)
	{
		hr = (*itContextNode)->SelectObject(pDevice, pDevResources, 0);
	}

	// Render child nodes

	_RenderVector::iterator itRenderNode;

	for (itRenderNode = m_renderNodes.begin(); hr == S_OK && itRenderNode != m_renderNodes.end(); itRenderNode++)
	{
		hr = (*itRenderNode)->Render(pDevice, pDevResources);
	}

	PopTransform(pDevice);

	return hr;
}

STDMETHODIMP CModel::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Model;
	return S_OK;
}

STDMETHODIMP CModel::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;

	pArchive->GetContext(&nContext);

	BEGIN_XML(hr)
		hrXML = ReadElement(pArchive, nContext, tag_Name, &CModel::put_Name);
		hrXML = ReadObjectCollection(pArchive, nContext, tag_Children, &CModel::PutChildUnk);
	END_XML

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CModel::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;

	pArchive->GetContext(&nContext);

	BEGIN_XML(hr)
		hrXML = WriteElement(pArchive, nContext, tag_Name, &CModel::get_Name);
		hrXML = WriteObjectCollection(pArchive, nContext, tag_Children, &CModel::GetChildUnk);
	END_XML

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CModel::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IModel
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

IUnknown* CModel::GetChildUnk(const long nIdx)
{
	if (nIdx < (long) m_children.size())
	{
		return (INode*) m_children[nIdx];
	}
	return NULL;
}

bool CModel::PutChildUnk(IUnknown* pUnk)
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

