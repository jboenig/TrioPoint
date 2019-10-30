//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Visual.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __VISUAL_H__
#define __VISUAL_H__

#pragma once

#include <stack>

#include "ShapeShifter.h"
#include "Node.h"
#include "MatrixStack.h"
#include "StyleFlags.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IVisualImpl

template <class base_class>
class IVisualImpl : public base_class
{
// Constructors/destructor
public:
	IVisualImpl()
	{
		D3DXMatrixIdentity(&m_mat);
		m_bVisible = TRUE;
		m_nStyles = StyleFlag::ALL;
	}

// Attributes
protected:
	D3DXMATRIX m_mat;
	bool m_bVisible;
	ULONG m_nStyles;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IVisual interface

	STDMETHOD(get_Visible)(/* [out,retval] */ VARIANT_BOOL* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = m_bVisible ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(put_Visible)(/* [in] */ VARIANT_BOOL newVal)
	{
		m_bVisible = newVal ? true : false;
		return S_OK;
	}

	STDMETHOD(get_Left)(/*[out, retval]*/ VALUE3D *pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_Right)(/*[out, retval]*/ VALUE3D *pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_Top)(/*[out, retval]*/ VALUE3D *pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_Bottom)(/*[out, retval]*/ VALUE3D *pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_Front)(/*[out, retval]*/ VALUE3D *pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_Back)(/*[out, retval]*/ VALUE3D *pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_OriginX)(/* [out,retval] */ VALUE3D* pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_OriginY)(/* [out,retval] */ VALUE3D* pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_OriginZ)(/* [out,retval] */ VALUE3D* pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(get_Radius)(/* [out,retval] */ VALUE3D* pVal)
	{
		pVal;
		return E_NOTIMPL;
	}

	STDMETHOD(PushTransform)()
	{
		ShapeShifter::MatrixStack::Push(m_mat);
		return S_OK;
	}

	STDMETHOD(PopTransform)()
	{
		ShapeShifter::MatrixStack::Pop();
		return S_OK;
	}

	STDMETHOD(GetOrigin)(/* [out,retval] */ VARIANT* pVarPoint)
	{
		pVarPoint;
		return E_NOTIMPL;
	}

	STDMETHOD(GetBoundingBox)(/* [out,retval] */ VARIANT* pVarBox)
	{
		pVarBox;
		return E_NOTIMPL;
	}

	STDMETHOD(SetBoundingBox)(/* [in] */ VARIANT varBox)
	{
		varBox;
		return E_NOTIMPL;
	}

	STDMETHOD(GetBoundingSphere)(/* [out,retval] */ VARIANT* pVarSphere)
	{
		pVarSphere;
		return E_NOTIMPL;
	}

	STDMETHOD(GetAxes)(/* [in] */ VALUE3D fScale,
	                   /* [out] */ VARIANT* varXAxis,
	                   /* [out] */ VARIANT* varYAxis,
	                   /* [out] */ VARIANT* varZAxis)
	{

		fScale;
		varXAxis;
		varYAxis;
		varZAxis;
		return E_NOTIMPL;
	}

	STDMETHOD(Translate)(/* [in] */ float tx, /* [in] */ float ty, /* [in] */ float tz)
	{
		D3DXMATRIX translateMatrix;
		D3DXMatrixTranslation(&translateMatrix, tx, ty, tz);
		m_mat = m_mat * translateMatrix;
		NotifyParent(ChildNotification::BOUNDS);
		return S_OK;
	}

	STDMETHOD(Scale)(/* [in] */ float sx, /* [in] */ float sy, /* [in] */ float sz)
	{
		D3DXMATRIX scaleMatrix;
		D3DXMatrixScaling(&scaleMatrix, sx, sy, sz);
		m_mat = m_mat * scaleMatrix;
		NotifyParent(ChildNotification::BOUNDS);
		return S_OK;
	}

	STDMETHOD(Rotate)(/* [in] */ VALUE3D fRadians,
	                  /* [in] */ VALUE3D fAxisX,
	                  /* [in] */ VALUE3D fAxisY,
	                  /* [in] */ VALUE3D fAxisZ)
	{
		D3DXMATRIX rotateMatrix;
		D3DXVECTOR3 rotateAxis(fAxisX, fAxisY, fAxisZ);
		D3DXMatrixRotationAxis(&rotateMatrix, &rotateAxis, fRadians);
		m_mat = m_mat * rotateMatrix;
		NotifyParent(ChildNotification::BOUNDS);
		return S_OK;
	}

	STDMETHOD(HitTestRay)(/* [in] */ VARIANT varRayOrigin,
	                      /* [in] */ VARIANT varRayDirection,
	                      /* [in] */ enum HitTestStrategy strategy)
	{
		varRayOrigin;
		varRayDirection;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(HitTestSphere)(/* [in] */ VARIANT varSphere,
	                         /* [in] */ enum HitTestStrategy strategy)
	{
		varSphere;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(HitTestBox)(/* [in] */ VARIANT varBox,
	                      /* [in] */ enum HitTestStrategy strategy)
	{
		varBox;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(HitTestPlane)(/* [in] */ VARIANT varPlane,
	                        /* [in] */ enum HitTestStrategy strategy)
	{
		varPlane;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy)
	{
		pNodeCollection;
		varRayOrigin;
		varRayDirection;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy)
	{
		pNodeCollection;
		varSphere;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy)
	{
		pNodeCollection;
		varBox;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy)
	{
		pNodeCollection;
		varPlane;
		strategy;
		return S_FALSE;
	}

	STDMETHOD(TestStyleFlags)(/* [in] */ long nMask, /* [out,retval] */ VARIANT_BOOL* pEnabled)
	{
		if (pEnabled == NULL)
		{
			return E_POINTER;
		}

		*pEnabled = (m_nStyles & static_cast<ULONG>(nMask)) ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(SetStyleFlags)(/* [in] */ long nMask, /* [in] */ VARIANT_BOOL bEnabled)
	{
		if (bEnabled)
		{
			m_nStyles |= static_cast<ULONG>(nMask);
		}
		else
		{
			m_nStyles &= ~(static_cast<ULONG>(nMask));
		}
		return S_OK;
	}

// Implementation
protected:
	inline void PushTransform(LPDIRECT3DDEVICE8 pDevice)
	{
		D3DXMATRIX matTop = ShapeShifter::MatrixStack::Push(m_mat);
		if (pDevice != NULL)
		{
			pDevice->SetTransform(D3DTS_WORLD, &matTop);
		}
	}

	inline void PushParentTransforms(LPDIRECT3DDEVICE8 pDevice)
	{
		INodePtr spCurNode(this);
		INode* pParent = NULL;
		std::stack<IVisualPtr> parents;

		// Push each visual parent onto a stack

		while (spCurNode != NULL && spCurNode->get_Parent(&pParent) == S_OK)
		{
			IVisualPtr spVisual(pParent);

			if (spVisual != NULL)
			{
				parents.push(spVisual);
			}

			spCurNode = pParent;
			pParent->Release();
		}

		// Pop each parent off the stack and push its transform

		while (!parents.empty())
		{
			IVisualPtr spVisual = parents.top();
			spVisual->PushTransform();
			parents.pop();
		}

		if (pDevice != NULL)
		{
			D3DXMATRIX matTop = MatrixStack::Peek();
			pDevice->SetTransform(D3DTS_WORLD, &matTop);
		}
	}

	inline void PopTransform(LPDIRECT3DDEVICE8 pDevice)
	{
		D3DXMATRIX matTop = ShapeShifter::MatrixStack::Pop();

		if (pDevice != NULL)
		{
			pDevice->SetTransform(D3DTS_WORLD, &matTop);
		}
	}

	inline void ClearTransforms()
	{
		MatrixStack::Clear();
	}
};

/////////////////////////////////////////////////////////////////////////
// ILeafVisualImpl

template <class base_class>
class ILeafVisualImpl : public IVisualImpl< INodeImpl<base_class> >
{
	//////////////////////////////////////////////////////////////
	// ICompositeNode interface

	STDMETHOD(GetChildCount)(/* [out,retval] */ long*)
	{
		return E_FAIL;
	}

	STDMETHOD(GetChildIndex)(/* [in] */ BSTR, /* [out,retval] */ long*)
	{
		return E_FAIL;
	}

	STDMETHOD(GetChild)(/* [in] */ long, /* [out,retval] */ INode**)
	{
		return E_FAIL;
	}

	STDMETHOD(GetChildByName)(/* [in] */ BSTR, /* [out,retval] */ INode**)
	{
		return E_FAIL;
	}

	STDMETHOD(AppendChild)(/* [in] */ INode*)
	{
		return E_FAIL;
	}

	STDMETHOD(InsertChild)(/* [in] */ INode*, /* [in] */ long)
	{
		return E_FAIL;
	}

	STDMETHOD(RemoveChild)(/* [in] */ INode*)
	{
		return E_FAIL;
	}

	STDMETHOD(RemoveAllChildren)()
	{
		return E_FAIL;
	}

	STDMETHOD(OnChildUpdate)(/* [in] */ INode*, /* [in] */ long)
	{
		return E_FAIL;
	}
};

/////////////////////////////////////////////////////////////////////////
// ICompositeVisualImpl

template <class base_class, class child_entry>
class ICompositeVisualImpl : public IVisualImpl< ICompositeNodeImpl<base_class, child_entry> >
{
// Operations
public:
	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy)
	{
		if (pNodeCollection == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		PushTransform(NULL);

		_NodeVector::iterator itChildren;

		for (itChildren = m_children.begin(); itChildren != m_children.end(); itChildren++)
		{
			IVisual* pVisual = NULL;
			if (SUCCEEDED((*itChildren)->QueryInterface(IID_IVisual, (LPVOID*) &pVisual)))
			{
				HRESULT hrTest = pVisual->HitTestRay(varRayOrigin, varRayDirection, strategy);

				if (hrTest == S_OK)
				{
					// S_OK indicates the test was positive
					pNodeCollection->Add(pVisual);
					hr = S_OK;
				}

				// Call recursively to children
				hrTest = pVisual->GetChildrenIntersectingRay(pNodeCollection, varRayOrigin, varRayDirection, strategy);

				if (hrTest == S_OK)
				{
					hr = S_OK; // S_OK indicates at least one child was hit
				}

				pVisual->Release();
			}
		}

		PopTransform(NULL);

		return hr;
	}

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy)
	{
		if (pNodeCollection == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;
		HRESULT hrTest;

		PushTransform(NULL);

		_NodeVector::iterator itChildren;

		for (itChildren = m_children.begin(); itChildren != m_children.end(); itChildren++)
		{
			IVisual* pVisual = NULL;
			if (SUCCEEDED((*itChildren)->QueryInterface(IID_IVisual, (LPVOID*) &pVisual)))
			{
				hrTest = pVisual->HitTestSphere(varSphere, strategy);

				if (hrTest == S_OK)
				{
					// S_OK indicates the test was positive
					pNodeCollection->Add(pVisual);
					hr = S_OK;
				}

				// Call recursively to children
				hrTest = pVisual->GetChildrenIntersectingSphere(pNodeCollection, varSphere, strategy);

				if (hrTest == S_OK)
				{
					hr = S_OK; // S_OK indicates at least one child was hit
				}

				pVisual->Release();
			}
		}

		PopTransform(NULL);

		return hr;
	}

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy)
	{
		if (pNodeCollection == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;
		HRESULT hrTest;

		PushTransform(NULL);

		_NodeVector::iterator itChildren;

		for (itChildren = m_children.begin(); itChildren != m_children.end(); itChildren++)
		{
			IVisual* pVisual = NULL;
			if (SUCCEEDED((*itChildren)->QueryInterface(IID_IVisual, (LPVOID*) &pVisual)))
			{
				hrTest = pVisual->HitTestBox(varBox, strategy);

				if (hrTest == S_OK)
				{
					// S_OK indicates the test was positive
					pNodeCollection->Add(pVisual);
					hr = S_OK;
				}

				// Call recursively to children
				hrTest = pVisual->GetChildrenIntersectingBox(pNodeCollection, varBox, strategy);

				if (hrTest == S_OK)
				{
					hr = S_OK; // S_OK indicates at least one child was hit
				}

				pVisual->Release();
			}
		}

		PopTransform(NULL);

		return hr;
	}

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy)
	{
		if (pNodeCollection == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;
		HRESULT hrTest;

		PushTransform(NULL);

		_NodeVector::iterator itChildren;

		for (itChildren = m_children.begin(); itChildren != m_children.end(); itChildren++)
		{
			IVisual* pVisual = NULL;
			if (SUCCEEDED((*itChildren)->QueryInterface(IID_IVisual, (LPVOID*) &pVisual)))
			{
				hrTest = pVisual->HitTestPlane(varPlane, strategy);

				if (hrTest == S_OK)
				{
					// S_OK indicates the test was positive
					pNodeCollection->Add(pVisual);
					hr = S_OK;
				}

				// Call recursively to children
				hrTest = pVisual->GetChildrenIntersectingPlane(pNodeCollection, varPlane, strategy);

				if (hrTest == S_OK)
				{
					hr = S_OK; // S_OK indicates at least one child was hit
				}

				pVisual->Release();
			}
		}

		PopTransform(NULL);

		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////
// Global helper functions

extern BOX3D& UnionBoundingBox(INode* pNode, BOX3D& boundingBox);
extern SPHERE3D& UnionBoundingSphere(INode* pNode, SPHERE3D& boundingSphere);

};  // namespace ShapeShifter

#endif // #ifndef __VISUAL_H__
