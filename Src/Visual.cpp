//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Visual.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Visual.h"

namespace ShapeShifter {

BOX3D& UnionBoundingBox(INode* pNode, BOX3D& boundingBox)
{
	IVisualPtr spVisual(pNode);
	VARIANT varBounds;
	long nChildCount = 0;
	long nChildIdx = 0;
	INode* pCurChild = NULL;

	if (spVisual != NULL)
	{
		spVisual->GetBoundingBox(&varBounds);
		vtBOX3D curBounds(varBounds);
		boundingBox = boundingBox.Union(curBounds);
		::VariantClear(&varBounds);
	}

	if (spVisual->GetChildCount(&nChildCount) == S_OK)
	{
		while (nChildIdx < nChildCount)
		{
			if (spVisual->GetChild(nChildIdx, &pCurChild) == S_OK)
			{
				UnionBoundingBox(pCurChild, boundingBox);
				pCurChild->Release();
			}
			nChildIdx++;
		}
	}

	return boundingBox;
}

SPHERE3D& UnionBoundingSphere(INode* pNode, SPHERE3D& boundingSphere)
{
	IVisualPtr spVisual(pNode);
	VARIANT varBounds;
	long nChildCount = 0;
	long nChildIdx = 0;
	INode* pCurChild = NULL;

	if (spVisual != NULL)
	{
		spVisual->GetBoundingSphere(&varBounds);
		vtSPHERE3D curBounds(varBounds);
		boundingSphere = boundingSphere.Union(curBounds);
		::VariantClear(&varBounds);
	}

	if (spVisual->GetChildCount(&nChildCount) == S_OK)
	{
		while (nChildIdx < nChildCount)
		{
			if (spVisual->GetChild(nChildIdx, &pCurChild) == S_OK)
			{
				UnionBoundingSphere(pCurChild, boundingSphere);
				pCurChild->Release();
			}
			nChildIdx++;
		}
	}

	return boundingSphere;
}

};  // namespace ShapeShifter
