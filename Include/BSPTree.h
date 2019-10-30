//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: BSPTree.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __BSPTREE_H__
#define __BSPTREE_H__

#pragma once

#include <vector>
#include <list>

#include "ShapeShifter.h"

namespace ShapeShifter {

const UINT MAX_BSP_LEAFNODES = 512;

/////////////////////////////////////////////////////////////////////////
// CBSPBox

class CBSPBox : public BOX3D
{
// Constructors/destructor
public:

	CBSPBox()
	{
		m_pObj = NULL;
		left = 0.0f;
		right = 0.0f;
		top = 0.0f;
		bottom = 0.0f;
		front = 0.0f;
		back = 0.0f;
	}

	CBSPBox(const CBSPBox& src)
	{
		left = src.left;
		right = src.right;
		top = src.top;
		bottom = src.bottom;
		front = src.front;
		back = src.back;
		m_pObj = src.m_pObj;
	}

// Attributes
public:
	IVisualPtr m_pObj;

// Operations
public:

	CBSPBox& operator=(const BOX3D& src)
	{
		left = src.left;
		right = src.right;
		top = src.top;
		bottom = src.bottom;
		front = src.front;
		back = src.back;
		return *this;
	}

	CBSPBox& operator=(const CBSPBox& src)
	{
		left = src.left;
		right = src.right;
		top = src.top;
		bottom = src.bottom;
		front = src.front;
		back = src.back;
		m_pObj = src.m_pObj;
		return *this;
	}

	bool CompareObject(IVisual* pObj) const
	{
		if (pObj == NULL && m_pObj == NULL)
		{
			return true;
		}
		else if ((pObj != NULL && m_pObj == NULL) ||
		         (pObj == NULL && m_pObj != NULL))
		{
			return false;
		}

		bool bIsEqual = false;
		IUnknown* pThisUnk = NULL;
		IUnknown* pObjUnk = NULL;

		if (SUCCEEDED(m_pObj->QueryInterface(IID_IUnknown, (LPVOID*) &pThisUnk)))
		{
			if (SUCCEEDED(pObj->QueryInterface(IID_IUnknown, (LPVOID*) &pObjUnk)))
			{
				bIsEqual = (pThisUnk == pObjUnk);
				pObjUnk->Release();
			}
			pThisUnk->Release();
		}

		return bIsEqual;
	}
};

typedef std::vector<CBSPBox> CBSPBoxArray;

/////////////////////////////////////////////////////////////////////////
// CBSPNode

class CBSPNode
{
// Constructors/destructor
public:
	CBSPNode()
	{
	}

	CBSPNode(BOX3D& volume) :
		m_volume(volume)
	{
	}

	virtual ~CBSPNode()
	{
	}

// Attributes
protected:
	BOX3D m_volume;

// Operations
public:
	BOX3D& GetVolume()
	{
		return m_volume;
	}

	virtual void AddEntry(CBSPBox& entry) = 0;
#if 0
	virtual void AddEntries(CBSPBoxArray& entries) = 0;
#endif

	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy) = 0;

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy) = 0;

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy) = 0;

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy) = 0;
};

/////////////////////////////////////////////////////////////////////////
// CBSPLeafNode

class CBSPLeafNode : public CBSPNode
{
// Constructors/destructor
public:
	CBSPLeafNode(BOX3D& volume);
	virtual ~CBSPLeafNode();

// Attributes
protected:
	CBSPBoxArray m_entries;

// Operations
public:
	virtual void AddEntry(CBSPBox& entry);
#if 0
	virtual void AddEntries(CBSPBoxArray& entries);
#endif

	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy);

	bool ContainsEntry(IVisual* pObj);
	bool RemoveEntry(IVisual* pObj);
	void RemoveAllEntries();
};

typedef std::list<CBSPLeafNode*> CBSPLeafNodeList;

/////////////////////////////////////////////////////////////////////////
// CBSPNonLeafNode

class CBSPNonLeafNode : public CBSPNode
{
// Constructors/destructor
public:
	CBSPNonLeafNode();
	CBSPNonLeafNode(BOX3D& volume);
	virtual ~CBSPNonLeafNode();

// Attributes
protected:
	PLANE3D m_partition;
	CBSPNode* m_pBackNode;
	CBSPNode* m_pFrontNode;

// Operations
public:
	void CreateNodes(const D3DXVECTOR3& threshold, CBSPLeafNodeList& leafNodes);
	void DestroyNodes();

	void SetVolume(const BOX3D& volume);

	virtual void AddEntry(CBSPBox& entry);
#if 0
	virtual void AddEntries(CBSPBoxArray& entries);
#endif

	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy);

// Implementation
protected:
	void UpdatePartition();
	bool SplitBox(const BOX3D& src, BOX3D& front, BOX3D& back);
};

/////////////////////////////////////////////////////////////////////////
// CBSPTree

class ATL_NO_VTABLE CBSPTree : public CComObjectRootEx<CComSingleThreadModel>,
                               public CComCoClass<CBSPTree, &CLSID_BSPTree>,
                               public IDispatchImpl<IBSPTree, &__uuidof(IBSPTree), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CBSPTree();
	virtual ~CBSPTree();

DECLARE_REGISTRY_RESOURCEID(IDR_BSPTREE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CBSPTree)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IBSPTree)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	CBSPNonLeafNode m_rootNode;
	CBSPLeafNodeList m_leafList;
	UINT m_nNumXPartitions;
	UINT m_nNumYPartitions;
	UINT m_nNumZPartitions;

// Operations
public:
	STDMETHOD(Initialize)();

	STDMETHOD(SetVolume)(/* [in] */ VALUE3D fLeft,
	                     /* [in] */ VALUE3D fRight,
	                     /* [in] */ VALUE3D fTop,
	                     /* [in] */ VALUE3D fBottom,
	                     /* [in] */ VALUE3D fFront,
	                     /* [in] */ VALUE3D fBack);

	STDMETHOD(SetPartitions)(/* [in] */ long nNumXPartitions, 
	                         /* [in] */ long nNumYPartitions, 
	                         /* [in] */ long nNumZPartitions);

	STDMETHOD(AddObject)(/* [in] */ INode* pObject,
	                     /* [in] */ VARIANT_BOOL bRecursive);

	STDMETHOD(UpdateObject)(/* [in] */ INode* pObject,
	                        /* [in] */ VARIANT_BOOL bRecursive);

	STDMETHOD(RemoveObject)(/* [in] */ INode* pObject,
	                        /* [in] */ VARIANT_BOOL bRecursive);

	STDMETHOD(RemoveAllObjects)();

	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy);
};

};  // namespace ShapeShifter

#endif // #ifndef __BSPTREE_H__
