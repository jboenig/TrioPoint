//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Group.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Group.h"
#include "XMLArchive.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CGroup

SSCL_BEGIN_RUNTIME_PROPERTY_TABLE(CGroup)
	SSCL_RUNTIME_PROPERTY_ENTRY_NULL
SSCL_END_RUNTIME_PROPERTY_TABLE

CGroup::CGroup()
{
}

CGroup::~CGroup()
{
}

HRESULT CGroup::FinalConstruct()
{
	return put_Name(tag_Group);
}

STDMETHODIMP CGroup::get_Left(/*[out, retval]*/ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVal = boundingBox.left;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_Right(/*[out, retval]*/ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVal = boundingBox.right;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_Top(/*[out, retval]*/ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVal = boundingBox.top;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_Bottom(/*[out, retval]*/ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVal = boundingBox.bottom;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_Front(/*[out, retval]*/ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVal = boundingBox.front;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_Back(/*[out, retval]*/ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVal = boundingBox.back;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_OriginX(/* [out,retval] */ VALUE3D* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	SPHERE3D boundingSphere;
	HRESULT hr = CalcBoundingSphere(boundingSphere);

	if (hr == S_OK)
	{
		*pVal = boundingSphere.x;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_OriginY(/* [out,retval] */ VALUE3D* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	SPHERE3D boundingSphere;
	HRESULT hr = CalcBoundingSphere(boundingSphere);

	if (hr == S_OK)
	{
		*pVal = boundingSphere.y;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_OriginZ(/* [out,retval] */ VALUE3D* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	SPHERE3D boundingSphere;
	HRESULT hr = CalcBoundingSphere(boundingSphere);

	if (hr == S_OK)
	{
		*pVal = boundingSphere.y;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::get_Radius(/* [out,retval] */ VALUE3D* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	SPHERE3D boundingSphere;
	HRESULT hr = CalcBoundingSphere(boundingSphere);

	if (hr == S_OK)
	{
		*pVal = boundingSphere.radius;
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::GetOrigin(/* [out,retval] */ VARIANT* pVarPoint)
{
	if (pVarPoint == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	SPHERE3D boundingSphere;
	HRESULT hr = CalcBoundingSphere(boundingSphere);

	if (hr == S_OK)
	{
		*pVarPoint = vtPOINT3D(boundingSphere.x, boundingSphere.y, boundingSphere.z);
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::GetBoundingBox(/* [out,retval] */ VARIANT* pVarBox)
{
	if (pVarBox == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	// Calculate the bounding box of this node
	BOX3D boundingBox;
	HRESULT hr = CalcBoundingBox(boundingBox);

	if (hr == S_OK)
	{
		*pVarBox = vtBOX3D(boundingBox);
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::GetBoundingSphere(/* [out,retval] */ VARIANT* pVarSphere)
{
	if (pVarSphere == NULL)
	{
		return E_POINTER;
	}

	// Push all world transforms onto the stack
	PushParentTransforms(NULL);
	PushTransform(NULL);

	// Calculate the bounding sphere of this node
	SPHERE3D boundingSphere;
	HRESULT hr = CalcBoundingSphere(boundingSphere);

	if (hr == S_OK)
	{
		*pVarSphere = vtSPHERE3D(boundingSphere);
	}

	// Clear the transform stack
	ClearTransforms();

	return hr;
}

STDMETHODIMP CGroup::HitTestRay(/* [in] */ VARIANT varRayOrigin,
                                /* [in] */ VARIANT varRayDirection,
                                /* [in] */ enum HitTestStrategy strategy)
{
	// If shape is not solid, return negative result for hit test

	VARIANT_BOOL bIsSolid = VARIANT_TRUE;

	if (SUCCEEDED(TestStyleFlags(StyleFlag::SOLID, &bIsSolid)) && !bIsSolid)
	{
		return S_FALSE;
	}

	HRESULT hr = S_FALSE;

	vtPOINT3D rayOrigin(varRayOrigin);
	vtVECTOR3D rayDirection(varRayDirection);

	PushTransform(NULL);

	switch (strategy)
	{
	case SSCL_TEST_BOUNDING_SPHERE:
		{
			SPHERE3D boundingSphere;
			hr = CalcBoundingSphere(boundingSphere);

			if (SUCCEEDED(hr))
			{
				if (boundingSphere.HitTestRay(rayOrigin, rayDirection))
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	case SSCL_TEST_BOUNDING_BOX:
		{
			BOX3D boundingBox;
			hr = CalcBoundingBox(boundingBox);

			if (SUCCEEDED(hr))
			{
				if (boundingBox.HitTestRay(rayOrigin, rayDirection))
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	default:
		{
			hr = E_NOTIMPL;
		}
	}

	PopTransform(NULL);

	return hr;
}

STDMETHODIMP CGroup::HitTestSphere(/* [in] */ VARIANT varSphere,
                                   /* [in] */ enum HitTestStrategy strategy)
{
	// If shape is not solid, return negative result for hit test

	VARIANT_BOOL bIsSolid = VARIANT_TRUE;

	if (SUCCEEDED(TestStyleFlags(StyleFlag::SOLID, &bIsSolid)) && !bIsSolid)
	{
		return S_FALSE;
	}

	HRESULT hr = S_FALSE;
	vtSPHERE3D sphere(varSphere);

	PushTransform(NULL);

	switch (strategy)
	{
	case SSCL_TEST_BOUNDING_SPHERE:
		{
			SPHERE3D boundingSphere;
			hr = CalcBoundingSphere(boundingSphere);

			if (SUCCEEDED(hr))
			{
				if (boundingSphere.HitTestSphere(sphere))
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	case SSCL_TEST_BOUNDING_BOX:
		{
			BOX3D boundingBox;
			hr = CalcBoundingBox(boundingBox);

			if (SUCCEEDED(hr))
			{
				if (sphere.HitTestBox(boundingBox))
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	default:
		{
			hr = E_NOTIMPL;
		}
	}

	PopTransform(NULL);

	return hr;
}

STDMETHODIMP CGroup::HitTestBox(/* [in] */ VARIANT varBox,
                                /* [in] */ enum HitTestStrategy strategy)
{
	// If shape is not solid, return negative result for hit test

	VARIANT_BOOL bIsSolid = VARIANT_TRUE;

	if (SUCCEEDED(TestStyleFlags(StyleFlag::SOLID, &bIsSolid)) && !bIsSolid)
	{
		return S_FALSE;
	}

	HRESULT hr = S_FALSE;
	vtBOX3D box(varBox);

	PushTransform(NULL);

	switch (strategy)
	{
	case SSCL_TEST_BOUNDING_SPHERE:
		{
			SPHERE3D boundingSphere;
			hr = CalcBoundingSphere(boundingSphere);

			if (SUCCEEDED(hr))
			{
				if (boundingSphere.HitTestBox(box))
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	case SSCL_TEST_BOUNDING_BOX:
		{
			BOX3D boundingBox;
			hr = CalcBoundingBox(boundingBox);

			if (SUCCEEDED(hr))
			{
				if (boundingBox.HitTestBox(box))
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	default:
		{
			hr = E_NOTIMPL;
		}
	}

	PopTransform(NULL);

	return hr;
}

STDMETHODIMP CGroup::HitTestPlane(/* [in] */ VARIANT varPlane,
                                  /* [in] */ enum HitTestStrategy strategy)
{
	// If shape is not solid, return negative result for hit test

	VARIANT_BOOL bIsSolid = VARIANT_TRUE;

	if (SUCCEEDED(TestStyleFlags(StyleFlag::SOLID, &bIsSolid)) && !bIsSolid)
	{
		return S_FALSE;
	}

	HRESULT hr = S_FALSE;
	vtPLANE3D plane(varPlane);

	PushTransform(NULL);

	switch (strategy)
	{
	case SSCL_TEST_BOUNDING_SPHERE:
		{
			SPHERE3D boundingSphere;
			hr = CalcBoundingSphere(boundingSphere);

			if (SUCCEEDED(hr))
			{
				if (plane.TestSphere(boundingSphere) == SPANNING)
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	case SSCL_TEST_BOUNDING_BOX:
		{
			BOX3D boundingBox;
			hr = CalcBoundingBox(boundingBox);

			if (SUCCEEDED(hr))
			{
				if (plane.TestBox(boundingBox) == SPANNING)
				{
					hr = S_OK;
				}
				else
				{
					hr = S_FALSE;
				}
			}
		}
		break;

	default:
		{
			hr = E_NOTIMPL;
		}
	}

	PopTransform(NULL);

	return hr;
}

STDMETHODIMP CGroup::Render(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
{
	HRESULT hr = S_OK;

	PushTransform(pDevice);

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

STDMETHODIMP CGroup::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Group;
	return S_OK;
}

STDMETHODIMP CGroup::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nObjRoot;
	pArchive->GetContext(&nObjRoot);

	// Read name
	hr = ReadElement(pArchive, nObjRoot, tag_Name, &CGroup::put_Name);

	// Read the transformation matrix
	if (SUCCEEDED(hr))
	{
		hr = ReadTransformationMatrix(pArchive, nObjRoot, tag_TransformationMatrix, m_mat);
	}

	// Read children
	if (SUCCEEDED(hr))
	{
		hr = ReadObjectCollection(pArchive, nObjRoot, tag_Children, &CGroup::PutChildUnk);
	}

	SetAndFreeContext(pArchive, nObjRoot);

	return hr;
}

STDMETHODIMP CGroup::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nObjRoot;

	pArchive->GetContext(&nObjRoot);

	hr = WriteElement(pArchive, nObjRoot, tag_Name, &CGroup::get_Name);

	// Write the transformation matrix
	if (SUCCEEDED(hr))
	{
		hr = pArchive->CreateElement(tag_TransformationMatrix);
	}

	if (SUCCEEDED(hr))
	{
		hr = WriteTransformationMatrix(pArchive, m_mat);
	}

	pArchive->SetContext(nObjRoot);

	if (SUCCEEDED(hr))
	{
		hr = WriteObjectCollection(pArchive, nObjRoot, tag_Children, &CGroup::GetChildUnk);
	}

	SetAndFreeContext(pArchive, nObjRoot);

	return hr;
}

HRESULT CGroup::CalcBoundingBox(BOX3D& boundingBox)
{
	_NodeVector::iterator itChild;

	for (itChild = m_children.begin(); itChild != m_children.end(); itChild++)
	{
		UnionBoundingBox(*itChild, boundingBox);
	}

	return S_OK;
}

HRESULT CGroup::CalcBoundingSphere(SPHERE3D& boundingSphere)
{
	_NodeVector::iterator itChild;

	for (itChild = m_children.begin(); itChild != m_children.end(); itChild++)
	{
		UnionBoundingSphere(*itChild, boundingSphere);
	}

	return S_OK;
}

IUnknown* CGroup::GetChildUnk(const long nIdx)
{
	if (nIdx < (long) m_children.size())
	{
		return (INode*) m_children[nIdx];
	}
	return NULL;
}

bool CGroup::PutChildUnk(IUnknown* pUnk)
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

