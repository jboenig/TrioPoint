//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Shape.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __SHAPE_H__
#define __SHAPE_H__

#pragma once

#include <list>
#include <map>

#include "ShapeShifter.h"
#include "Node.h"
#include "Visual.h"
#include "Property.h"
#include "Mesh.h"
#include "RenderingContext.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IShapeImpl

template <class base_class>
class IShapeImpl : public base_class,
                   public IContextProvider,
                   public IRenderDX8
{
// Embedded types
protected:
	typedef std::map<PrimitiveIndex, IContextObject*> _ContextMap;

// Constructors/destructor
public:
	IShapeImpl() :
		m_pMesh(NULL)
	{
	}

	virtual ~IShapeImpl()
	{
		FreeContextObjects();

		if (m_pMesh != NULL)
		{
			m_pMesh->Release();
		}
	}

// Attributes
protected:
	IMeshEx* m_pMesh;
	IDrawMeshDX8Ptr m_spMeshDraw;
	_ContextMap m_contexts;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IVisual interface

	STDMETHOD(get_Left)(/*[out, retval]*/ VALUE3D *pVal)
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

	STDMETHOD(get_Right)(/*[out, retval]*/ VALUE3D *pVal)
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

	STDMETHOD(get_Top)(/*[out, retval]*/ VALUE3D *pVal)
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

	STDMETHOD(get_Bottom)(/*[out, retval]*/ VALUE3D *pVal)
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

	STDMETHOD(get_Front)(/*[out, retval]*/ VALUE3D *pVal)
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

	STDMETHOD(get_Back)(/*[out, retval]*/ VALUE3D *pVal)
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

	STDMETHOD(get_OriginX)(/* [out,retval] */ VALUE3D* pVal)
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

	STDMETHOD(get_OriginY)(/* [out,retval] */ VALUE3D* pVal)
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

	STDMETHOD(get_OriginZ)(/* [out,retval] */ VALUE3D* pVal)
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

	STDMETHOD(get_Radius)(/* [out,retval] */ VALUE3D* pVal)
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

	STDMETHOD(GetOrigin)(/* [out,retval] */ VARIANT* pVarPoint)
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

	STDMETHOD(GetBoundingBox)(/* [out,retval] */ VARIANT* pVarBox)
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

	STDMETHOD(SetBoundingBox)(/* [in] */ VARIANT varBox)
	{
		varBox;
		return E_FAIL;
	}

	STDMETHOD(GetBoundingSphere)(/* [out,retval] */ VARIANT* pVarSphere)
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

	STDMETHOD(GetAxes)(/* [in] */ VALUE3D fScale,
	                   /* [out] */ VARIANT* varXAxis,
	                   /* [out] */ VARIANT* varYAxis,
	                   /* [out] */ VARIANT* varZAxis)
	{
		SPHERE3D boundingSphere;
		HRESULT hr = CalcBoundingSphere(boundingSphere);
		D3DXVECTOR3 origin(boundingSphere.x, boundingSphere.y, boundingSphere.z);
		D3DXVECTOR3 pts[6];

		if (hr == S_OK)
		{
			PushParentTransforms(NULL);
			PushTransform(NULL);
			D3DXMATRIX& matTop = ShapeShifter::MatrixStack::Peek();

			pts[0] = origin + D3DXVECTOR3(boundingSphere.radius * (-fScale), 0.0f, 0.0f);
			pts[1] = origin + D3DXVECTOR3(boundingSphere.radius * fScale, 0.0f, 0.0f);
			pts[2] = origin + D3DXVECTOR3(0.0f, boundingSphere.radius * fScale, 0.0f);
			pts[3] = origin + D3DXVECTOR3(0.0f, boundingSphere.radius * (-fScale), 0.0f);
			pts[4] = origin + D3DXVECTOR3(0.0f, 0.0f, boundingSphere.radius * (-fScale));
			pts[5] = origin + D3DXVECTOR3(0.0f, 0.0f, boundingSphere.radius * fScale);

			D3DXVec3TransformCoord(&pts[0], &pts[0], &matTop);
			D3DXVec3TransformCoord(&pts[1], &pts[1], &matTop);
			D3DXVec3TransformCoord(&pts[2], &pts[2], &matTop);
			D3DXVec3TransformCoord(&pts[3], &pts[3], &matTop);
			D3DXVec3TransformCoord(&pts[4], &pts[4], &matTop);
			D3DXVec3TransformCoord(&pts[5], &pts[5], &matTop);

			*varXAxis = vtLINE3D(pts[0], pts[1]);
			*varYAxis = vtLINE3D(pts[2], pts[3]);
			*varZAxis = vtLINE3D(pts[4], pts[5]);

			ClearTransforms();

			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(HitTestRay)(/* [in] */ VARIANT varRayOrigin,
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

	STDMETHOD(HitTestSphere)(/* [in] */ VARIANT varSphere,
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

	STDMETHOD(HitTestBox)(/* [in] */ VARIANT varBox,
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

	STDMETHOD(HitTestPlane)(/* [in] */ VARIANT varPlane,
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

	//////////////////////////////////////////////////////////////
	// IShape interface

	STDMETHOD(get_Mesh)(/* [out, retval] */ IMesh** ppVal)
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

		return hr;
	}

	STDMETHOD(putref_Mesh)(/* [in] */ IMesh* pMesh)
	{
		HRESULT hr = S_OK;
		IMeshEx* pMeshEx = NULL;

		if (pMesh != NULL)
		{
			hr = pMesh->QueryInterface(IID_IMeshEx, (LPVOID*) &pMeshEx);
		}

		if (SUCCEEDED(hr))
		{
			if (m_pMesh != NULL)
			{
				m_pMesh->Release();
			}

			m_pMesh = pMeshEx;
			m_spMeshDraw = m_pMesh;

			NotifyParent(ChildNotification::BOUNDS);
		}

		return hr;
	}

	STDMETHOD(SetContext)(/* [in] */ PrimitiveIndex nPrimIdx,
	                      /* [in] */ INode* pContextNode)
	{
		if (pContextNode == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = E_FAIL;
		IContextObject* pContextObj = NULL;
		hr = pContextNode->QueryInterface(IID_IContextObject, (LPVOID*) &pContextObj);

		if (SUCCEEDED(hr))
		{
			_ContextMap::iterator itContext = m_contexts.find(nPrimIdx);

			if (itContext != m_contexts.end())
			{
				(itContext->second)->Release();
				m_contexts.erase(itContext);
			}

			m_contexts[nPrimIdx] = pContextObj;
		}

		return S_OK;
	}

	STDMETHOD(GetContext)(/* [in] */ PrimitiveIndex nPrimIdx,
	                      /* [out,retval] */ INode** ppContextNode)
	{
		if (ppContextNode == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		_ContextMap::iterator itContext = m_contexts.find(nPrimIdx);

		if (itContext != m_contexts.end())
		{
			hr = (itContext->second)->QueryInterface(IID_INode, (LPVOID*) ppContextNode);
		}

		return hr;
	}

	STDMETHOD(ClearContext)(/* [in] */ PrimitiveIndex nPrimIdx)
	{
		HRESULT hr = S_FALSE;

		_ContextMap::iterator itContext = m_contexts.find(nPrimIdx);

		if (itContext != m_contexts.end())
		{
			IContextObject* pContextObj = itContext->second;
			if (pContextObj != NULL)
			{
				pContextObj->Release();
			}
			m_contexts.erase(itContext);

			hr = S_OK;
		}

		return hr;
	}

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

#if 0
	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
	{
		HRESULT hr = E_FAIL;

		hr = PushTransform(pDevice);

		if (hr == S_OK)
		{
			if (m_spMeshDraw != NULL)
			{
				hr = m_spMeshDraw->Draw(pDevice, pDevResources, this);
			}

			if (PopTransform(pDevice) != S_OK)
			{
				hr = E_UNEXPECTED;
			}
		}

		return hr;
	}
#else
	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources)
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

#if 0				// Optimized out for now. Left in interface in case it's needed later
				m_spMeshDraw->EndDraw(pDevice, pDevResources);
#endif

				pDevice->DeleteStateBlock(dwCurState);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		PopTransform(pDevice);

		return hr;
	}
#endif

	//////////////////////////////////////////////////////////////
	// IContextProvider interface

	STDMETHOD(SelectContext)(LPDIRECT3DDEVICE8 pDevice,
	                         IResourceManagerDX8* pDevResources,
	                         long nContextId)
	{
		HRESULT hr = S_FALSE;

		_ContextMap::iterator itContext = m_contexts.find(nContextId);

		if (itContext != m_contexts.end())
		{
			hr = (itContext->second)->SelectObject(pDevice, pDevResources, 0);
		}

		return hr;
	}

// Implementation
protected:
	void FreeContextObjects()
	{
		_ContextMap::iterator itContext = m_contexts.begin();

		while (itContext != m_contexts.end())
		{
			itContext->second->Release();
			itContext++;
		}
		
		m_contexts.clear();
	}

	D3DXVECTOR3* GetTransformedPoints(VertexCount& nNumPoints) const
	{
		if (m_pMesh == NULL)
		{
			return NULL;
		}

		D3DXVECTOR3* pPoints = NULL;
		HRESULT hr;
		LPBYTE pVertexBuf = NULL;
		UINT nVertSize;

		hr = m_pMesh->_GetVertexCount(&nNumPoints);

		if (FAILED(hr))
		{
			_com_issue_error(hr);
		}

		hr = m_pMesh->_GetVertexBuffer((LPVOID*) &pVertexBuf);

		if (FAILED(hr))
		{
			_com_issue_error(hr);
		}

		hr = m_pMesh->_GetVertexSize(&nVertSize);

		if (FAILED(hr))
		{
			_com_issue_error(hr);
		}

		if (nNumPoints > 0)
		{
			D3DXMATRIX& matTop = ShapeShifter::MatrixStack::Peek();

			pPoints = new D3DXVECTOR3[nNumPoints];

			for (VertexIndex nPointIdx = 0; nPointIdx < nNumPoints; nPointIdx++)
			{
				D3DXVECTOR3 curPoint;
				curPoint.x = ((D3DVECTOR*) pVertexBuf)->x;
				curPoint.y = ((D3DVECTOR*) pVertexBuf)->y;
				curPoint.z = ((D3DVECTOR*) pVertexBuf)->z;
				D3DXVec3TransformCoord(&pPoints[nPointIdx], &curPoint, &matTop);
				pVertexBuf += nVertSize;
			}
		}

		return pPoints;
	}

// Implementation
protected:
	HRESULT CalcBoundingBox(BOX3D& boundingBox)
	{
		HRESULT hr = S_OK;
		VertexCount nNumPoints;
		D3DXVECTOR3* pPoints = NULL;

		try
		{
			pPoints = GetTransformedPoints(nNumPoints);

			if (pPoints != NULL)
			{
				boundingBox.CalculateBounds(pPoints, nNumPoints);
				delete[] pPoints;
				hr = S_OK;
			}
		}
		catch (_com_error& e)
		{
			hr = e.Error();
		}

		return hr;
	}

	HRESULT CalcBoundingSphere(SPHERE3D& boundingSphere)
	{
		HRESULT hr = S_OK;

		// Calculate the bounding sphere of the mesh
		boundingSphere.radius = 0.0f;
		boundingSphere.x = 0.0f;
		boundingSphere.y = 0.0f;
		boundingSphere.z = 0.0f;

		VertexCount nNumPoints;
		D3DXVECTOR3* pPoints = NULL;

		try
		{
			pPoints = GetTransformedPoints(nNumPoints);
		}
		catch (_com_error& e)
		{
			hr = e.Error();
		}

		if (hr == S_OK && pPoints != NULL)
		{
			boundingSphere.CalculateBounds(pPoints, nNumPoints);
			delete[] pPoints;
		}

		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////
// CShape

class ATL_NO_VTABLE CShape : public CComObjectRootEx<CComSingleThreadModel>,
                             public CComCoClass<CShape, &CLSID_Shape>,
                             public IDispatchRTPropConImpl<CShape, IShapeImpl< ILeafVisualImpl<IShape> >, &__uuidof(IShape), &LIBID_ShapeShifter>,
                             public IPersistXMLImpl<CShape>
{
// Constructors/destructor
public:
	CShape();
	virtual ~CShape();

DECLARE_REGISTRY_RESOURCEID(IDR_SHAPE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Shape)

// Interface map
BEGIN_COM_MAP(CShape)
	COM_INTERFACE_ENTRY(IShape)
	COM_INTERFACE_ENTRY(IVisual)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY_IID(IID_IContextProvider, IContextProvider)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CShape, 32)

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);
};

/////////////////////////////////////////////////////////////////////////
// CCompositeShape

class ATL_NO_VTABLE CCompositeShape : public CComObjectRootEx<CComSingleThreadModel>,
                                      public CComCoClass<CCompositeShape, &CLSID_CompositeShape>,
                                      public IDispatchRTPropConImpl<CCompositeShape, IShapeImpl< ICompositeVisualImpl< IShape, CInterfacePair<INode,IRenderDX8> > >, &__uuidof(IShape), &LIBID_ShapeShifter>,
                                      public IPersistXMLImpl<CCompositeShape>
{
// Embedded types
public:
	typedef IDispatchRTPropConImpl<CCompositeShape, IShapeImpl< ICompositeVisualImpl< IShape, CInterfacePair<INode,IRenderDX8> > >, &__uuidof(IShape), &LIBID_ShapeShifter> _ShapeBase;

// Constructors/destructor
public:
	CCompositeShape();
	virtual ~CCompositeShape();

DECLARE_REGISTRY_RESOURCEID(IDR_COMPOSITE_SHAPE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Shape)

	HRESULT FinalConstruct();

// Interface map
BEGIN_COM_MAP(CCompositeShape)
	COM_INTERFACE_ENTRY(IShape)
	COM_INTERFACE_ENTRY(IVisual)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY_IID(IID_IContextProvider, IContextProvider)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CCompositeShape, 32)

// Operations
public:

	//////////////////////////////////////////////////////////////
	// IShape interface

	STDMETHOD(get_Mesh)(/* [out, retval] */ IMesh** ppVal);
	STDMETHOD(GetContext)(/* [in] */ PrimitiveIndex nPrimIdx,
	                      /* [out,retval] */ INode** ppContextNode);

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

// Implementation
protected:
	IUnknown* __stdcall GetChildUnk(const long nIdx);
	bool __stdcall PutChildUnk(IUnknown* pUnk);
};

};  // namespace ShapeShifter

#endif // #ifndef __SHAPE_H__
