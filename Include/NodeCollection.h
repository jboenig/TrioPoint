//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: NodeCollection.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __NODECOLLECTION_H__
#define __NODECOLLECTION_H__

#pragma once

#include <list>

#include "ShapeShifter.h"

namespace ShapeShifter {

template <class T>
class _CopyVariantFromInterface
{
public:
	static HRESULT copy(VARIANT* pV, T** pIP)
	{
		HRESULT hr = (*pIP)->QueryInterface(IID_IDispatch, (void**)&pV->pdispVal);
		if (SUCCEEDED(hr))
		{
			pV->vt = VT_DISPATCH;
		}
		else
		{
			(*pIP)->AddRef();
			pV->vt = VT_UNKNOWN;
			pV->punkVal = (*pIP);
		}
		return S_OK;
	}

	static void init(VARIANT* p) { VariantInit(p); }
	static void destroy(VARIANT* p) { VariantClear(p); }
};

/////////////////////////////////////////////////////////////////////////
// CEnumNodeList

typedef CComEnumOnSTL< IEnumNodes, &IID_IEnumNodes, INode*, _CopyInterface<INode>, std::list<INode*> > IEnumNodesOnListImpl;

class CEnumNodeList : public IEnumNodesOnListImpl,
                      public CComCoClass<CEnumNodeList, &CLSID_EnumNodeList>
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_ENUMNODELIST)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CEnumNodeList)
	COM_INTERFACE_ENTRY_IID(IID_IEnumNodes, IEnumNodes)
END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////
// CEnumVariantNodeList

typedef CComEnumOnSTL< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _CopyVariantFromInterface<INode>, std::list<INode*> > IEnumVariantNodesOnListImpl;

class CEnumVariantNodeList : public IEnumVariantNodesOnListImpl,
                             public CComCoClass<CEnumVariantNodeList, &CLSID_EnumVariantNodeList>
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_ENUMVARIANTNODELIST)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CEnumVariantNodeList)
	COM_INTERFACE_ENTRY_IID(IID_IEnumVARIANT, IEnumVARIANT)
END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////
// CNodeList

typedef ICollectionOnSTLImpl< INodes, std::list<INode*>, INode*, _CopyInterface<INode>, CEnumVariantNodeList > INodeOnListImpl;

class CNodeList : public CComObjectRootEx<CComSingleThreadModel>,
                  public CComCoClass<CNodeList, &CLSID_NodeList>,
                  public IDispatchImpl<INodeOnListImpl, &__uuidof(INodes), &LIBID_ShapeShifter>
{
public:
	~CNodeList()
	{
		Clear();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_NODELIST)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CNodeList)
	COM_INTERFACE_ENTRY(INodes)
END_COM_MAP()

// Operations
public:
	STDMETHOD(get__NewNodeEnum)(/* [out,retval] */ LPUNKNOWN* ppEnum)
	{
		if (ppEnum == NULL)
			return E_POINTER;
		*ppEnum = NULL;
		HRESULT hRes = S_OK;
		CComObject<CEnumNodeList>* p;
		hRes = CComObject<CEnumNodeList>::CreateInstance(&p);
		if (SUCCEEDED(hRes))
		{
			hRes = p->Init(this, m_coll);
			if (hRes == S_OK)
				hRes = p->QueryInterface(IID_IUnknown, (void**)ppEnum);
		}
		if (hRes != S_OK)
			delete p;
		return hRes;
	}

	STDMETHOD(Clear)()
	{
		while (m_coll.size() > 0)
		{
			INode* pNode = m_coll.back();
			pNode->Release();
			m_coll.pop_back();
		}
		return S_OK;
	}

	STDMETHOD(Add)(INode* pNode)
	{
		if (pNode != NULL)
		{
			pNode->AddRef();
			m_coll.push_back(pNode);
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(SortByDistance)(/* [in] */ VARIANT varPoint)
	{
		HRESULT hr = S_OK;

		vtPOINT3D refPt(varPoint);

		std::list<INode*>::iterator curSubset = m_coll.begin();
		std::list<INode*>::iterator curNode;
		std::list<INode*>::iterator curNearest;

		UINT nSubsetSize = m_coll.size();

		while (curSubset != m_coll.end() && nSubsetSize > 1)
		{
			FLOAT fNearestDist = FLT_MAX;

			curNode = curSubset;
			curNearest = m_coll.end();

			while (curNode != m_coll.end())
			{
				IVisual* pCurVisual;
				if (SUCCEEDED((*curNode)->QueryInterface(IID_IVisual, (LPVOID*) &pCurVisual)))
				{
					VARIANT varOrigin;
					if (SUCCEEDED(pCurVisual->GetOrigin(&varOrigin)))
					{
						vtPOINT3D ptOrigin(varOrigin);
						D3DXVECTOR3 distVect = (D3DXVECTOR3) ptOrigin - (D3DXVECTOR3) refPt;
						FLOAT fCurDist = D3DXVec3Length(&distVect);

						if (fCurDist < fNearestDist)
						{
							fNearestDist = fCurDist;
							curNearest = curNode;
						}

						::VariantClear(&varOrigin);
					}
					pCurVisual->Release();
				}

				curNode++;
			}

			if (curNearest != m_coll.end())
			{
				INode* pTemp = (*curNearest);
				m_coll.erase(curNearest);
				curSubset = m_coll.insert(curSubset, pTemp);
			}

			curSubset++;
			nSubsetSize--;
		}

		return hr;
	}

	STDMETHOD(GetNearestNode)(/* [in] */ VARIANT varPoint,
	                          /* [in] */ long nStyleMask,
	                          /* [out,retval] */ INode** ppNode)
	{
		if (ppNode == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;
		vtPOINT3D refPt(varPoint);
		INode* pNearest = NULL;
		FLOAT fNearestDist = 0.0f;

		std::list<INode*>::iterator iter = m_coll.begin();
		while (iter != m_coll.end())
		{
			IVisual* pCurVisual;
			if (SUCCEEDED((*iter)->QueryInterface(IID_IVisual, (LPVOID*) &pCurVisual)))
			{
				VARIANT_BOOL bEnabled;
				HRESULT hrPerm = pCurVisual->TestStyleFlags(nStyleMask, &bEnabled);

				if (hrPerm == S_OK && bEnabled)
				{
					VARIANT varOrigin;
					if (SUCCEEDED(pCurVisual->GetOrigin(&varOrigin)))
					{
						vtPOINT3D ptOrigin(varOrigin);
						D3DXVECTOR3 distVect = (D3DXVECTOR3) ptOrigin - (D3DXVECTOR3) refPt;
						FLOAT fCurDist = D3DXVec3Length(&distVect);

						if ((pNearest == NULL) || (fCurDist < fNearestDist))
						{
							fNearestDist = fCurDist;
							pNearest = (*iter);
							pNearest->AddRef();
							hr = S_OK;
						}

						::VariantClear(&varOrigin);
					}
				}
				pCurVisual->Release();
			}
			iter++;
		}

		if (pNearest != NULL)
		{
			*ppNode = pNearest;
		}

		return hr;
	}

	STDMETHOD(ContainsNode)(/* [in] */ INode* pNode)
	{
		if (pNode == NULL)
		{
			return S_FALSE;
		}

		HRESULT hr = S_FALSE;

		IUnknown* pNodeUnk = NULL;
		if (SUCCEEDED(pNode->QueryInterface(IID_IUnknown, (LPVOID*) &pNodeUnk)))
		{
			std::list<INode*>::iterator iter = m_coll.begin();
			while (hr == S_FALSE && iter != m_coll.end())
			{
				IUnknown* pCurUnk = NULL;

				(*iter)->QueryInterface(IID_IUnknown, (LPVOID*) &pCurUnk);

				if (pCurUnk == pNodeUnk)
				{
					hr = S_OK;
				}

				pCurUnk->Release();

				iter++;
			}

			pNodeUnk->Release();
		}

		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////
// INodesAccessor

template <typename rtn_iface>
class INodesAccessor
{
// Constructors/destructor
public:
	INodesAccessor(INodes* pNodes) :
		m_pNodes(NULL),
		m_pEnumNodes(NULL)
	{
		m_pNodes = pNodes;

		if (m_pNodes != NULL)
		{
			m_pNodes->AddRef();

			IUnknown* pEnumUnk = NULL;
			if (SUCCEEDED(m_pNodes->get__NewNodeEnum(&pEnumUnk)) && pEnumUnk != NULL)
			{
				if (FAILED(pEnumUnk->QueryInterface(IID_IEnumNodes, (LPVOID*) &m_pEnumNodes)))
				{
					m_pEnumNodes = NULL;
				}
				pEnumUnk->Release();
			}
		}
	}

	~INodesAccessor()
	{
		if (m_pEnumNodes != NULL)
		{
			m_pEnumNodes->Release();
		}

		if (m_pNodes != NULL)
		{
			m_pNodes->Release();
		}
	}

// Attributes
protected:
	INodes* m_pNodes;
	IEnumNodes* m_pEnumNodes;

// Operations
public:

	long GetNodeCount() const
	{
		long nNumNodes = 0;

		if (m_pNodes != NULL)
		{
			m_pNodes->get_Count(&nNumNodes);
		}

		return nNumNodes;
	}

	rtn_iface* GetNodeAt(const long idx)
	{
		INode* pNode = NULL;
		rtn_iface* pRtnIface = NULL;

		if (m_pNodes != NULL)
		{
			if (SUCCEEDED(m_pNodes->get_Item(idx, &pNode)))
			{
				if (FAILED(pNode->QueryInterface(__uuidof(rtn_iface), (LPVOID*) &pRtnIface)))
				{
					pRtnIface = NULL;
				}
				pNode->Release();
			}
		}

		return pRtnIface;
	}

	HRESULT GetFirst(rtn_iface** ppRtnIface)
	{
		if (ppRtnIface == NULL)
		{
			return E_POINTER;
		}

		if (m_pEnumNodes == NULL)
		{
			return E_FAIL;
		}

		if (*ppRtnIface != NULL)
		{
			(*ppRtnIface)->Release();
			*ppRtnIface = NULL;
		}

		INode* pNode = NULL;
		ULONG nFetched = 0;

		m_pEnumNodes->Reset();

		HRESULT hr = m_pEnumNodes->Next(1, &pNode, &nFetched);
		
		if (hr == S_OK && nFetched > 0)
		{
			hr = pNode->QueryInterface(__uuidof(rtn_iface), (LPVOID*) ppRtnIface);
			pNode->Release();
		}

		return hr;
	}

	HRESULT GetNext(rtn_iface** ppRtnIface)
	{
		if (ppRtnIface == NULL)
		{
			return E_POINTER;
		}

		if (m_pEnumNodes == NULL)
		{
			return E_FAIL;
		}

		if (*ppRtnIface != NULL)
		{
			(*ppRtnIface)->Release();
			*ppRtnIface = NULL;
		}

		INode* pNode = NULL;
		ULONG nFetched = 0;

		HRESULT hr = m_pEnumNodes->Next(1, &pNode, &nFetched);
		
		if (hr == S_OK && nFetched > 0)
		{
			hr = pNode->QueryInterface(__uuidof(rtn_iface), (LPVOID*) ppRtnIface);
			pNode->Release();
		}

		return hr;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __NODECOLLECTION_H__
