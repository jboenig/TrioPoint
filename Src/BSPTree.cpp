//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: BSPTree.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "BSPTree.h"
#include "NodeCollection.h"
#include "Visual.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CBSPLeafNode

CBSPLeafNode::CBSPLeafNode(BOX3D& volume) :
	CBSPNode(volume)
{
}

CBSPLeafNode::~CBSPLeafNode()
{
}

void CBSPLeafNode::AddEntry(CBSPBox& entry)
{
	m_entries.push_back(entry);
}

#if 0
void CBSPLeafNode::AddEntries(CBSPBoxArray& entries)
{
	CBSPBoxArray::iterator itEntries;

	for (itEntries = entries.begin(); itEntries != entries.end(); itEntries++)
	{
		m_entries.push_back(*itEntries);
	}
}
#endif

STDMETHODIMP CBSPLeafNode::GetChildrenIntersectingRay(/* [in] */ INodes* pNodeCollection,
                                                      /* [in] */ VARIANT varRayOrigin,
                                                      /* [in] */ VARIANT varRayDirection,
                                                      /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	CBSPBoxArray::iterator itEntries;

	for (itEntries = m_entries.begin(); itEntries != m_entries.end(); itEntries++)
	{
		if (pNodeCollection->ContainsNode(itEntries->m_pObj) != S_OK)
		{
			if (itEntries->m_pObj->HitTestRay(varRayOrigin, varRayDirection, strategy) == S_OK)
			{
				pNodeCollection->Add(itEntries->m_pObj);
				hr = S_OK;
			}
		}
	}

	return hr;
}

STDMETHODIMP CBSPLeafNode::GetChildrenIntersectingSphere(/* [in] */ INodes* pNodeCollection,
                                                         /* [in] */ VARIANT varSphere,
                                                         /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	CBSPBoxArray::iterator itEntries;

	for (itEntries = m_entries.begin(); itEntries != m_entries.end(); itEntries++)
	{
		if (pNodeCollection->ContainsNode(itEntries->m_pObj) != S_OK)
		{
			if (itEntries->m_pObj->HitTestSphere(varSphere, strategy) == S_OK)
			{
				pNodeCollection->Add(itEntries->m_pObj);
				hr = S_OK;
			}
		}
	}

	return hr;
}

STDMETHODIMP CBSPLeafNode::GetChildrenIntersectingBox(/* [in] */ INodes* pNodeCollection,
                                                      /* [in] */ VARIANT varBox,
                                                      /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	CBSPBoxArray::iterator itEntries;

	for (itEntries = m_entries.begin(); itEntries != m_entries.end(); itEntries++)
	{
		if (pNodeCollection->ContainsNode(itEntries->m_pObj) != S_OK)
		{
			if (itEntries->m_pObj->HitTestBox(varBox, strategy) == S_OK)
			{
				pNodeCollection->Add(itEntries->m_pObj);
				hr = S_OK;
			}
		}
	}

	return hr;
}

STDMETHODIMP CBSPLeafNode::GetChildrenIntersectingPlane(/* [in] */ INodes* pNodeCollection,
                                                        /* [in] */ VARIANT varPlane,
                                                        /* [in] */ enum HitTestStrategy strategy)
{
	pNodeCollection;
	varPlane;
	strategy;
	return E_NOTIMPL;
}

bool CBSPLeafNode::ContainsEntry(IVisual* pObj)
{
	bool bFound = false;
	CBSPBoxArray::iterator itEntries;

	if (pObj != NULL)
	{
		for (itEntries = m_entries.begin(); itEntries != m_entries.end() && !bFound; itEntries++)
		{
			if (itEntries->CompareObject(pObj))
			{
				bFound = true;
			}
			else
			{
				itEntries++;
			}
		}
	}

	return bFound;
}

bool CBSPLeafNode::RemoveEntry(IVisual* pObj)
{
	bool bRemoved = false;
	CBSPBoxArray::iterator itEntries;

	if (pObj != NULL)
	{
		itEntries = m_entries.begin();

		while (itEntries != m_entries.end() && !bRemoved)
		{
			if (itEntries->CompareObject(pObj))
			{
				itEntries = m_entries.erase(itEntries);
				bRemoved = true;
			}
			else
			{
				itEntries++;
			}
		}
	}

	return bRemoved;
}

void CBSPLeafNode::RemoveAllEntries()
{
	m_entries.clear();
}

/////////////////////////////////////////////////////////////////////////
// CBSPNonLeafNode

CBSPNonLeafNode::CBSPNonLeafNode() :
	m_pBackNode(NULL),
	m_pFrontNode(NULL)
{
}

CBSPNonLeafNode::CBSPNonLeafNode(BOX3D& volume) :
	CBSPNode(volume),
	m_pBackNode(NULL),
	m_pFrontNode(NULL)
{
	UpdatePartition();
}

CBSPNonLeafNode::~CBSPNonLeafNode()
{
	DestroyNodes();
}

void CBSPNonLeafNode::CreateNodes(const D3DXVECTOR3& threshold, CBSPLeafNodeList& leafNodes)
{
	DestroyNodes();

	BOX3D volumeFront;
	BOX3D volumeBack;
	SplitBox(m_volume, volumeFront, volumeBack);

	// Create back node

	D3DXVECTOR3 sizeBack;
	volumeBack.GetSize(sizeBack);

	if (sizeBack.x <= threshold.x &&
	    sizeBack.y <= threshold.y &&
	    sizeBack.z <= threshold.z)
	{
		// Create leaf
		m_pBackNode = new CBSPLeafNode(volumeBack);
		leafNodes.push_back(static_cast<CBSPLeafNode*>(m_pBackNode));
	}
	else
	{
		// Create non-leaf
		m_pBackNode = new CBSPNonLeafNode(volumeBack);
		static_cast<CBSPNonLeafNode*>(m_pBackNode)->CreateNodes(threshold, leafNodes);
	}

	// Create front node

	D3DXVECTOR3 sizeFront;
	volumeFront.GetSize(sizeFront);

	if (sizeFront.x <= threshold.x &&
	    sizeFront.y <= threshold.y &&
	    sizeFront.z <= threshold.z)
	{
		// Create leaf
		m_pFrontNode = new CBSPLeafNode(volumeFront);
		leafNodes.push_back(static_cast<CBSPLeafNode*>(m_pFrontNode));
	}
	else
	{
		// Create non-leaf
		m_pFrontNode = new CBSPNonLeafNode(volumeFront);
		static_cast<CBSPNonLeafNode*>(m_pFrontNode)->CreateNodes(threshold, leafNodes);
	}
}

void CBSPNonLeafNode::DestroyNodes()
{
	if (m_pBackNode != NULL)
	{
		delete m_pBackNode;
		m_pBackNode = NULL;
	}

	if (m_pFrontNode)
	{
		delete m_pFrontNode;
		m_pFrontNode = NULL;
	}
}

void CBSPNonLeafNode::SetVolume(const BOX3D& volume)
{
	DestroyNodes();
	m_volume = volume;
	UpdatePartition();
}

void CBSPNonLeafNode::AddEntry(CBSPBox& entry)
{
	// Determine the orientation of the object's bounding box with
	// respect to the partition and add the entry to either the front
	// node, back node, or both.

	switch (m_partition.TestBox(entry))
	{
	case IN_FRONT_OF:
		{
			m_pFrontNode->AddEntry(entry);
		}
		break;

	case IN_BACK_OF:
		{
			m_pBackNode->AddEntry(entry);
		}
		break;

	case COINCIDENT:
	case SPANNING:
		{
			CBSPBox frontEntry, backEntry;

			if (SplitBox(entry, frontEntry, backEntry))
			{
				frontEntry.m_pObj = backEntry.m_pObj = entry.m_pObj;
				m_pFrontNode->AddEntry(entry);
				m_pBackNode->AddEntry(entry);
			}
		}
		break;
	}
}

#if 0
void CBSPNonLeafNode::AddEntries(CBSPBoxArray& entries)
{
	CBSPBoxArray::iterator itEntries;
	CBSPBoxArray backEntries, frontEntries;

	for (itEntries = entries.begin(); itEntries != entries.end(); itEntries++)
	{
		// Determine the orientation of the object's bounding box with
		// respect to the partition

		switch (m_partition.TestBox(*itEntries))
		{
		case IN_FRONT_OF:
			{
				frontEntries.push_back(*itEntries);
			}
			break;

		case IN_BACK_OF:
			{
				backEntries.push_back(*itEntries);
			}
			break;

		case COINCIDENT:
		case SPANNING:
			{
				CBSPBox frontEntry, backEntry;

				if (SplitBox(*itEntries, frontEntry, backEntry))
				{
					frontEntry.m_pObj = backEntry.m_pObj = itEntries->m_pObj;
					frontEntries.push_back(frontEntry);
					backEntries.push_back(backEntry);
				}
			}
			break;
		}
	}

	if (frontEntries.size() > 0 && m_pFrontNode != NULL)
	{
		m_pFrontNode->AddEntries(frontEntries);
	}

	if (backEntries.size() > 0 && m_pBackNode != NULL)
	{
		m_pBackNode->AddEntries(backEntries);
	}
}
#endif

STDMETHODIMP CBSPNonLeafNode::GetChildrenIntersectingRay(/* [in] */ INodes* pNodeCollection,
                                                         /* [in] */ VARIANT varRayOrigin,
                                                         /* [in] */ VARIANT varRayDirection,
                                                         /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	vtPOINT3D rayOrigin(varRayOrigin);
	vtVECTOR3D rayDirection(varRayDirection);

	// In order to determine the position of the ray relative to the
	// partitioning plane, an arbitrary point on the ray must be
	// generated. The point generated is at a distance from the ray
	// origin that is twice the length of the largest dimension of
	// the volume of this node. This should ensure that the line
	// segment tested is long enough to intersect the plane if the
	// ray also intersects the plane.

	D3DXVECTOR3 sizeVolume;
	m_volume.GetSize(sizeVolume);

	FLOAT fRayScale;

	if (sizeVolume.x > sizeVolume.y && sizeVolume.x > sizeVolume.z)
	{
		fRayScale = sizeVolume.x;
	}
	else if (sizeVolume.y > sizeVolume.x && sizeVolume.y > sizeVolume.z)
	{
		fRayScale = sizeVolume.y;
	}
	else
	{
		fRayScale = sizeVolume.z;
	}

	fRayScale = fRayScale * 2.0f;

	D3DXVECTOR3 endPointVector(rayDirection);
	::D3DXVec3Normalize(&endPointVector, &endPointVector);
	::D3DXVec3Scale(&endPointVector, &endPointVector, fRayScale);
	D3DXVECTOR3 lineSeg[2];
	lineSeg[0] = rayOrigin;
	lineSeg[1] = (D3DXVECTOR3) rayOrigin + endPointVector;

	// Test the relative position of the line segment that was computed from
	// the ray. Recurse down to either the front node, back node, or both
	// depending on the position of the ray with respect to the paritioning
	// plane.

	RelativePositionToPlane rayPos = m_partition.TestPoints(lineSeg, 2);

	if ((rayPos == IN_BACK_OF || rayPos == SPANNING) && m_pBackNode)
	{
		HRESULT hrBack = m_pBackNode->GetChildrenIntersectingRay(pNodeCollection,
		                                                         varRayOrigin,
		                                                         varRayDirection,
		                                                         strategy);
		if (hrBack == S_OK)
		{
			hr = S_OK;
		}
	}

	if ((rayPos == IN_FRONT_OF || rayPos == SPANNING) && m_pFrontNode)
	{
		HRESULT hrFront = m_pFrontNode->GetChildrenIntersectingRay(pNodeCollection,
		                                                           varRayOrigin,
		                                                           varRayDirection,
		                                                           strategy);
		if (hrFront == S_OK)
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CBSPNonLeafNode::GetChildrenIntersectingSphere(/* [in] */ INodes* pNodeCollection,
                                                            /* [in] */ VARIANT varSphere,
                                                            /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	vtSPHERE3D sphere(varSphere);

	// Test the relative position of the sphere. Recurse down to either the
	// front node, back node, or both depending on the position of the
	// sphere with respect to the paritioning plane.

	RelativePositionToPlane spherePos = m_partition.TestSphere(sphere);

	if ((spherePos == IN_BACK_OF || spherePos == SPANNING) && m_pBackNode)
	{
		HRESULT hrBack = m_pBackNode->GetChildrenIntersectingSphere(pNodeCollection,
		                                                            varSphere,
		                                                            strategy);
		if (hrBack == S_OK)
		{
			hr = S_OK;
		}
	}

	if ((spherePos == IN_FRONT_OF || spherePos == SPANNING) && m_pFrontNode)
	{
		HRESULT hrFront = m_pFrontNode->GetChildrenIntersectingSphere(pNodeCollection,
		                                                              varSphere,
		                                                              strategy);
		if (hrFront == S_OK)
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CBSPNonLeafNode::GetChildrenIntersectingBox(/* [in] */ INodes* pNodeCollection,
                                                         /* [in] */ VARIANT varBox,
                                                         /* [in] */ enum HitTestStrategy strategy)
{
	HRESULT hr = S_FALSE;

	vtBOX3D box(varBox);

	// Test the relative position of the box. Recurse down to either the
	// front node, back node, or both depending on the position of the
	// box with respect to the paritioning plane.

	RelativePositionToPlane boxPos = m_partition.TestBox(box);

	if ((boxPos == IN_BACK_OF || boxPos == SPANNING) && m_pBackNode)
	{
		HRESULT hrBack = m_pBackNode->GetChildrenIntersectingBox(pNodeCollection,
		                                                         varBox,
		                                                         strategy);
		if (hrBack == S_OK)
		{
			hr = S_OK;
		}
	}

	if ((boxPos == IN_FRONT_OF || boxPos == SPANNING) && m_pFrontNode)
	{
		HRESULT hrFront = m_pFrontNode->GetChildrenIntersectingBox(pNodeCollection,
		                                                           varBox,
		                                                           strategy);
		if (hrFront == S_OK)
		{
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CBSPNonLeafNode::GetChildrenIntersectingPlane(/* [in] */ INodes* pNodeCollection,
                                                           /* [in] */ VARIANT varPlane,
                                                           /* [in] */ enum HitTestStrategy strategy)
{
	pNodeCollection;
	varPlane;
	strategy;
	return E_NOTIMPL;
}

void CBSPNonLeafNode::UpdatePartition()
{
	// Calculate the partition

	D3DXVECTOR3 volumeSize;
	D3DXVECTOR3 partitionPt;
	D3DXVECTOR3 partitionNormal;

	m_volume.GetCenter(partitionPt);
	m_volume.GetSize(volumeSize);

	if (volumeSize.y > volumeSize.x)
	{
		if (volumeSize.z > volumeSize.y)
		{
			partitionNormal.x = 0.0f;
			partitionNormal.y = 0.0f;
			partitionNormal.z = 1.0f;
		}
		else
		{
			partitionNormal.x = 0.0f;
			partitionNormal.y = 1.0f;
			partitionNormal.z = 0.0f;
		}
	}
	else
	{
		if (volumeSize.z > volumeSize.x)
		{
			partitionNormal.x = 0.0f;
			partitionNormal.y = 0.0f;
			partitionNormal.z = 1.0f;
		}
		else
		{
			partitionNormal.x = 1.0f;
			partitionNormal.y = 0.0f;
			partitionNormal.z = 0.0f;
		}
	}

	m_partition.FromPointNormal(partitionPt, partitionNormal);
}

bool CBSPNonLeafNode::SplitBox(const BOX3D& src, BOX3D& front, BOX3D& back)
{
	bool bSplit = false;

	D3DXVECTOR3 partitionPt;
	m_volume.GetCenter(partitionPt);

	// Determine the orientation of the plane with respect to the volume

	if (!IsEqualFloat(m_partition.a, 0.0f) &&
	    IsEqualFloat(m_partition.b, 0.0f) &&
	    IsEqualFloat(m_partition.c, 0.0f))
	{
		// Partition is parallel to the left and right sides of the volume
		// and intersects the X axis
		front = back = src;
		front.left = partitionPt.x;
		back.right = partitionPt.x;
		bSplit = true;
	}
	else if (IsEqualFloat(m_partition.a, 0.0f) &&
	         !IsEqualFloat(m_partition.b, 0.0f) &&
	         IsEqualFloat(m_partition.c, 0.0f))
	{
		// Partition is parallel to the top and bottom sides of the volume
		// and intersects the Y axis
		front = back = src;
		front.bottom = partitionPt.y;
		back.top = partitionPt.y;
		bSplit = true;
	}
	else if (IsEqualFloat(m_partition.a, 0.0f) &&
	         IsEqualFloat(m_partition.b, 0.0f) &&
	         !IsEqualFloat(m_partition.c, 0.0f))
	{
		// Partition is parallel to the front and back sides of the volume
		// and intersects the Z axis
		front = back = src;
		front.back = partitionPt.z;
		back.front = partitionPt.z;
		bSplit = true;
	}

	return bSplit;
}

/////////////////////////////////////////////////////////////////////////
// CBSPTree

CBSPTree::CBSPTree() :
	m_nNumXPartitions(1),
	m_nNumYPartitions(1),
	m_nNumZPartitions(1)
{
}

CBSPTree::~CBSPTree()
{
}

HRESULT CBSPTree::FinalConstruct()
{
	return S_OK;
}

STDMETHODIMP CBSPTree::Initialize()
{
	D3DXVECTOR3 threshold;
	D3DXVECTOR3 volumeSize;
	m_rootNode.GetVolume().GetSize(volumeSize);

	threshold.x = volumeSize.x / (FLOAT) (m_nNumXPartitions + 1);
	threshold.y = volumeSize.y / (FLOAT) (m_nNumYPartitions + 1);
	threshold.z = volumeSize.z / (FLOAT) (m_nNumZPartitions + 1);

	m_leafList.clear();

	m_rootNode.CreateNodes(threshold, m_leafList);

	return S_OK;
}

STDMETHODIMP CBSPTree::SetVolume(/* [in] */ VALUE3D fLeft,
                                 /* [in] */ VALUE3D fRight,
                                 /* [in] */ VALUE3D fTop,
                                 /* [in] */ VALUE3D fBottom,
                                 /* [in] */ VALUE3D fFront,
                                 /* [in] */ VALUE3D fBack)
{
	m_rootNode.SetVolume(BOX3D(fLeft, fRight, fTop, fBottom, fFront, fBack));
	return S_OK;
}

STDMETHODIMP CBSPTree::SetPartitions(/* [in] */ long nNumXPartitions, 
                                     /* [in] */ long nNumYPartitions, 
                                     /* [in] */ long nNumZPartitions)
{
	HRESULT hr = E_FAIL;

	// Limit the size of the tree for space and performance reasons

	UINT nLeafNodes = (nNumXPartitions + 1) * (nNumYPartitions + 1) * (nNumZPartitions + 1);

	if (nLeafNodes > 1 && nLeafNodes <= MAX_BSP_LEAFNODES)
	{
		m_nNumXPartitions = nNumXPartitions;
		m_nNumYPartitions = nNumYPartitions;
		m_nNumZPartitions = nNumZPartitions;
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CBSPTree::AddObject(/* [in] */ INode* pObject, /* [in] */ VARIANT_BOOL bRecursive)
{
	if (pObject == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;
	CBSPBox entry;
	VARIANT varBounds;
	VARIANT_BOOL bAllowAdd = VARIANT_FALSE;

	entry.m_pObj = pObject;

	if (entry.m_pObj != NULL)
	{
		entry.m_pObj->TestStyleFlags(StyleFlag::SOLID | StyleFlag::ALLOW_BSPTREE, &bAllowAdd);

		if (bAllowAdd)
		{
			if (entry.m_pObj->GetBoundingBox(&varBounds) == S_OK)
			{
				entry = vtBOX3D(varBounds);
				::VariantClear(&varBounds);
			}
			if (!entry.IsEmpty())
			{
				m_rootNode.AddEntry(entry);
			}
		}
	}

	if (bRecursive)
	{
		ICompositeNodePtr spComposite(pObject);
		long nChildCount = 0;
		INode* pCurChild = NULL;

		if (spComposite != NULL && SUCCEEDED(spComposite->GetChildCount(&nChildCount)))
		{
			for (long nChildIdx = 0; nChildIdx < nChildCount; nChildIdx++)
			{
				if (SUCCEEDED(spComposite->GetChild(nChildIdx, &pCurChild)))
				{
					AddObject(pCurChild, bRecursive);
					pCurChild->Release();
				}
			}
		}
	}

	return hr;
}

STDMETHODIMP CBSPTree::UpdateObject(/* [in] */ INode* pObject,
                                    /* [in] */ VARIANT_BOOL bRecursive)
{
	if (pObject == NULL)
	{
		return E_POINTER;
	}

	CBSPBox entry;
	VARIANT varBounds;
	VARIANT_BOOL bAllowAdd = VARIANT_FALSE;

	entry.m_pObj = pObject;

	if (entry.m_pObj != NULL)
	{
		// Remove the object

		CBSPLeafNodeList::iterator itLeaf;

		for (itLeaf = m_leafList.begin(); itLeaf != m_leafList.end(); itLeaf++)
		{
			(*itLeaf)->RemoveEntry(entry.m_pObj);
		}

		// Add the object

		entry.m_pObj->TestStyleFlags(StyleFlag::SOLID | StyleFlag::ALLOW_BSPTREE, &bAllowAdd);

		if (bAllowAdd)
		{
			if (entry.m_pObj->GetBoundingBox(&varBounds) == S_OK)
			{
				entry = vtBOX3D(varBounds);
				::VariantClear(&varBounds);
			}
			if (!entry.IsEmpty())
			{
				m_rootNode.AddEntry(entry);
			}
		}
	}

	if (bRecursive)
	{
		ICompositeNodePtr spComposite(pObject);
		long nChildCount = 0;
		INode* pCurChild = NULL;

		if (spComposite != NULL && SUCCEEDED(spComposite->GetChildCount(&nChildCount)))
		{
			for (long nChildIdx = 0; nChildIdx < nChildCount; nChildIdx++)
			{
				if (SUCCEEDED(spComposite->GetChild(nChildIdx, &pCurChild)))
				{
					UpdateObject(pCurChild, bRecursive);
					pCurChild->Release();
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CBSPTree::RemoveObject(/* [in] */ INode* pObject,
                                    /* [in] */ VARIANT_BOOL bRecursive)
{
	if (pObject == NULL)
	{
		return E_POINTER;
	}

	if (bRecursive)
	{
		ICompositeNodePtr spComposite(pObject);
		long nChildCount = 0;
		INode* pCurChild = NULL;

		if (spComposite != NULL && SUCCEEDED(spComposite->GetChildCount(&nChildCount)))
		{
			for (long nChildIdx = 0; nChildIdx < nChildCount; nChildIdx++)
			{
				if (SUCCEEDED(spComposite->GetChild(nChildIdx, &pCurChild)))
				{
					RemoveObject(pCurChild, bRecursive);
					pCurChild->Release();
				}
			}
		}
	}

	IVisualPtr spVisual(pObject);

	if (spVisual != NULL)
	{
		CBSPLeafNodeList::iterator itLeaf;

		for (itLeaf = m_leafList.begin(); itLeaf != m_leafList.end(); itLeaf++)
		{
			(*itLeaf)->RemoveEntry(spVisual);
		}
	}

	return S_OK;
}

STDMETHODIMP CBSPTree::RemoveAllObjects()
{
	CBSPLeafNodeList::iterator itLeaf;

	for (itLeaf = m_leafList.begin(); itLeaf != m_leafList.end(); itLeaf++)
	{
		(*itLeaf)->RemoveAllEntries();
	}

	return S_OK;
}

STDMETHODIMP CBSPTree::GetChildrenIntersectingRay(/* [in] */ INodes* pNodeCollection,
                                                  /* [in] */ VARIANT varRayOrigin,
                                                  /* [in] */ VARIANT varRayDirection,
                                                  /* [in] */ enum HitTestStrategy strategy)
{
	return m_rootNode.GetChildrenIntersectingRay(pNodeCollection,
	                                             varRayOrigin,
	                                             varRayDirection,
	                                             strategy);
}

STDMETHODIMP CBSPTree::GetChildrenIntersectingSphere(/* [in] */ INodes* pNodeCollection,
                                                     /* [in] */ VARIANT varSphere,
                                                     /* [in] */ enum HitTestStrategy strategy)
{
	return m_rootNode.GetChildrenIntersectingSphere(pNodeCollection,
	                                                varSphere,
	                                                strategy);
}

STDMETHODIMP CBSPTree::GetChildrenIntersectingBox(/* [in] */ INodes* pNodeCollection,
                                                  /* [in] */ VARIANT varBox,
                                                  /* [in] */ enum HitTestStrategy strategy)
{
	return m_rootNode.GetChildrenIntersectingBox(pNodeCollection,
	                                             varBox,
	                                             strategy);
}

STDMETHODIMP CBSPTree::GetChildrenIntersectingPlane(/* [in] */ INodes* pNodeCollection,
                                                    /* [in] */ VARIANT varPlane,
                                                    /* [in] */ enum HitTestStrategy strategy)
{
	return m_rootNode.GetChildrenIntersectingPlane(pNodeCollection,
	                                               varPlane,
	                                               strategy);
}

};  // namespace ShapeShifter
