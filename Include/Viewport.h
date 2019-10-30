//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Viewport.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#pragma once

#include <math.h>
#include <vector>

#include "ShapeShifter.h"
#include "Node.h"
#include "Visual2D.h"
#include "Viewport.h"
#include "Property.h"
#include "DevicePresentation.h"
#include "NodeCollection.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IViewportImpl

template <class base_class>
class IViewportImpl : public base_class
{
// Constructors/destructor
public:
	IViewportImpl() :
		m_pDevice(NULL),
		m_pModel(NULL),
		m_pCamera(NULL),
		m_pSelectionList(NULL),
		m_pContext(NULL)
	{
		m_vpInfo.X = 0;
		m_vpInfo.Y = 0;
		m_vpInfo.Width = 0;
		m_vpInfo.Height = 0;
		m_vpInfo.MinZ = 0.0;
		m_vpInfo.MaxZ = 1.0;

		m_rcBounds.left = m_rcBounds.top = 0;
		m_rcBounds.right = m_rcBounds.bottom = 0;

		m_clrBackground.r = 0.0f;
		m_clrBackground.g = 0.0f;
		m_clrBackground.b = 1.0f;
		m_clrBackground.a = 0.0f;

		m_fFOV = 0.0f;
		m_fAspect = 0.0f;
		m_fNearPlane = 0.0f;
		m_fFarPlane = 0.0f;
	}

	virtual ~IViewportImpl()
	{
		if (m_pSelectionList != NULL)
		{
			m_pSelectionList->Release();
		}

		if (m_pContext != NULL)
		{
			m_pContext->Release();
		}

		if (m_pCamera != NULL)
		{
			m_pCamera->Release();
		}

		if (m_pModel != NULL)
		{
			m_pModel->Release();
		}

		if (m_pDevice != NULL)
		{
			m_pDevice->Release();
		}
	}

// Attributes
protected:
	IDevice* m_pDevice;
	D3DVIEWPORT8 m_vpInfo;
	ICameraEx* m_pCamera;
	IModel* m_pModel;
	INodes* m_pSelectionList;

	// Viewport properties
	D3DCOLORVALUE m_clrBackground;

	// Frustum values
	FLOAT m_fFOV;
	FLOAT m_fAspect;
	FLOAT m_fNearPlane;
	FLOAT m_fFarPlane;

	// Device render state settings
	IRenderingContext* m_pContext;

// Operations
public:
	STDMETHOD(_InitObject)(/* [in] */ IDevice* pDevice)
	{
		if (pDevice == NULL)
		{
			return E_POINTER;
		}

		if (m_pDevice != NULL)
		{
			E_UNEXPECTED;
		}

		m_pDevice = pDevice;
		m_pDevice->AddRef();

		return S_OK;
	}

	STDMETHOD(SetSelectionList)(INodes* pSelectionList)
	{
		if (pSelectionList != NULL)
		{
			pSelectionList->AddRef();
		}

		if (m_pSelectionList != NULL)
		{
			m_pSelectionList->Release();
		}

		m_pSelectionList = pSelectionList;

		return S_OK;
	}

	STDMETHOD(SetFrustum)(FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane, FLOAT fFarPlane)
	{
		m_fFOV = fFOV;
		m_fAspect = fAspect;
		m_fNearPlane = fNearPlane;
		m_fFarPlane = fFarPlane;
		return S_OK;
	}

	STDMETHOD(GetFrustum)(FLOAT* pFOV, FLOAT* pAspect, FLOAT* pNearPlane, FLOAT* pFarPlane)
	{
		if (pFOV != NULL)
		{
			*pFOV = m_fFOV;
		}

		if (pAspect != NULL)
		{
			*pAspect = m_fAspect;
		}

		if (pNearPlane != NULL)
		{
			*pNearPlane = m_fNearPlane;
		}

		if (pFarPlane != NULL)
		{
			*pFarPlane = m_fFarPlane;
		}

		return S_OK;
	}

	STDMETHOD(DistanceToPoint)(/* [in] */ VALUE3D x,
	                           /* [in] */ VALUE3D y,
	                           /* [in] */ VALUE3D z,
	                           /* [out,retval] */ VALUE3D* pDistance)
	{
		if (pDistance == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = E_FAIL;
		D3DXVECTOR3 toPt(x,y,z);

		if (m_pCamera != NULL)
		{
			D3DXVECTOR3 cameraLoc;
			m_pCamera->GetLocation(&cameraLoc.x, &cameraLoc.y, &cameraLoc.z);
			D3DXVECTOR3 distVector(D3DXVECTOR3(x,y,z) - cameraLoc);
			*pDistance = D3DXVec3Length(&distVector);
			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(SetBounds)(LPRECT pBounds)
	{
		if (pBounds == NULL)
		{
			return E_POINTER;
		}

		if (pBounds->left < 0 || pBounds->right < pBounds->left)
		{
			return E_INVALIDARG;
		}

		if (pBounds->top < 0 || pBounds->bottom < pBounds->top)
		{
			return E_INVALIDARG;
		}

		m_rcBounds = *pBounds;
		m_vpInfo.Width = m_rcBounds.right - m_rcBounds.left;
		m_vpInfo.Height = m_rcBounds.bottom - m_rcBounds.top;

		return S_OK;
	}

	STDMETHOD(GetOrigin)(POINT* pt)
	{
		if (pt != NULL)
		{
			pt->x = m_rcBounds.left;
			pt->y = m_rcBounds.top;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(GetSize)(SIZE* sz)
	{
		if (sz != NULL)
		{
			sz->cx = m_rcBounds.right - m_rcBounds.left;
			sz->cy = m_rcBounds.bottom - m_rcBounds.top;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(ScreenToWorld)(int nScreenX, int nScreenY, D3DXVECTOR3* rayOrigin, D3DXVECTOR3* rayDirection)
	{
		D3DXVECTOR3 screenPt;
		D3DXMATRIX projMatrix;
		FillProjectionMatrix(projMatrix);
		D3DXMATRIX viewMatrix;
		FillViewMatrix(viewMatrix);
		D3DXMATRIX worldMatrix;
		D3DXMatrixIdentity(&worldMatrix);

		screenPt.x = (FLOAT) nScreenX;
		screenPt.y = (FLOAT) nScreenY;
		screenPt.z = 0.0f;

		D3DXVec3Unproject(rayOrigin, &screenPt, &m_vpInfo, &projMatrix, &viewMatrix, &worldMatrix);

		if (m_pCamera != NULL)
		{
			D3DXVECTOR3 cameraLoc;
			m_pCamera->GetLocation(&cameraLoc.x, &cameraLoc.y, &cameraLoc.z);
			*rayDirection = *rayOrigin - cameraLoc;
			D3DXVec3Normalize(rayDirection, rayDirection);
		}

		return S_OK;
	}

	STDMETHOD(ScreenToWorld)(/* [in] */ int nScreenX, /* [in] */ int nScreenY,
	                         /* [out] */ VARIANT* pVarRayOrigin,
	                         /* [out] */ VARIANT* pVarRayDirection)
	{
		D3DXVECTOR3 rayOrigin, rayDirection;

		HRESULT hr = ScreenToWorld(nScreenX, nScreenY, &rayOrigin, &rayDirection);

		if (hr == S_OK)
		{
			*pVarRayOrigin = vtPOINT3D(rayOrigin);
			*pVarRayDirection = vtVECTOR3D(rayDirection);
		}

		return hr;
	}

	STDMETHOD(WorldToScreen)(/* [in] */ VARIANT varWorldPoint,
	                         /* [out] */ int* pScreenX,
	                         /* [out] */ int* pScreenY)
	{
		vtVECTOR3D vtWorldPt(varWorldPoint);
		D3DXVECTOR3 worldPt((D3DXVECTOR3) vtWorldPt);
		D3DXVECTOR3 outVector;
		D3DXMATRIX projMatrix;
		FillProjectionMatrix(projMatrix);
		D3DXMATRIX viewMatrix;
		FillViewMatrix(viewMatrix);
		D3DXMATRIX worldMatrix;
		D3DXMatrixIdentity(&worldMatrix);

		D3DXVec3Project(&outVector, &worldPt, &m_vpInfo, &projMatrix, &viewMatrix, &worldMatrix);

		*pScreenX = (int) outVector.x;
		*pScreenY = (int) outVector.y;

		return S_OK;
	}

	STDMETHOD(SetBackgroundColor)(/* [in] */ VALUE3D Red,
	                              /* [in] */ VALUE3D Green,
	                              /* [in] */ VALUE3D Blue,
	                              /* [in] */ VALUE3D Alpha)
	{
		m_clrBackground.r = Red;
		m_clrBackground.g = Green;
		m_clrBackground.b = Blue;
		m_clrBackground.a = Alpha;
		return S_OK;
	}

	STDMETHOD(GetBackgroundColor)(/* [in] */ VALUE3D* pRed,
	                              /* [in] */ VALUE3D* pGreen,
	                              /* [in] */ VALUE3D* pBlue,
	                              /* [in] */ VALUE3D* pAlpha)
	{
		*pRed = m_clrBackground.r;
		*pGreen = m_clrBackground.g;
		*pBlue = m_clrBackground.b;
		*pAlpha = m_clrBackground.a;
		return S_OK;
	}

// Property Accessors
public:
	STDMETHOD(get_Left)(/*[out, retval]*/ long *pVal)
	{
		*pVal = m_rcBounds.left;
		return S_OK;
	}

	STDMETHOD(put_Left)(/*[in]*/ long newVal)
	{
		RECT rc;
		rc.left = newVal;
		rc.right = m_rcBounds.right;
		rc.top = m_rcBounds.top;
		rc.bottom = m_rcBounds.bottom;
		return SetBounds(&rc);
	}

	STDMETHOD(get_Top)(/*[out, retval]*/ long *pVal)
	{
		*pVal = m_rcBounds.top;
		return S_OK;
	}

	STDMETHOD(put_Top)(/*[in]*/ long newVal)
	{
		RECT rc;
		rc.left = m_rcBounds.left;
		rc.right = m_rcBounds.right;
		rc.top = newVal;
		rc.bottom = m_rcBounds.bottom;
		return SetBounds(&rc);
	}

	STDMETHOD(get_Width)(/*[out, retval]*/ long *pVal)
	{
		*pVal = m_rcBounds.right - m_rcBounds.left;
		return S_OK;
	}

	STDMETHOD(put_Width)(/*[in]*/ long newVal)
	{
		RECT rc;
		rc.left = m_rcBounds.left;
		rc.right = m_rcBounds.left + newVal;
		rc.top = m_rcBounds.top;
		rc.bottom = m_rcBounds.bottom;
		return SetBounds(&rc);
	}

	STDMETHOD(get_Height)(/*[out, retval]*/ long *pVal)
	{
		*pVal = m_rcBounds.bottom - m_rcBounds.top;
		return S_OK;
	}

	STDMETHOD(put_Height)(/*[in]*/ long newVal)
	{
		RECT rc;
		rc.left = m_rcBounds.left;
		rc.right = m_rcBounds.right;
		rc.top = m_rcBounds.top;
		rc.bottom = m_rcBounds.top + newVal;
		return SetBounds(&rc);
	}

	STDMETHOD(get_Device)(/* [out, retval] */ IDevice** ppDevice)
	{
		if (ppDevice == NULL)
		{
			return E_POINTER;
		}

		*ppDevice = m_pDevice;

		if (*ppDevice != NULL)
		{
			(*ppDevice)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(putref_Model)(/* [in] */ IModel* pNewVal)
	{
		if (pNewVal != NULL)
		{
			pNewVal->AddRef();
		}
		if (m_pModel != NULL)
		{
			m_pModel->Release();
		}
		m_pModel = pNewVal;

		return S_OK;
	}

	STDMETHOD(get_Model)(/* [out,retval] */ IModel** ppVal)
	{
		if (ppVal == NULL)
		{
			return E_POINTER;
		}

		*ppVal = m_pModel;

		if (*ppVal != NULL)
		{
			(*ppVal)->AddRef();
			return S_OK;
		}

		return S_FALSE;
	}

	STDMETHOD(get_Context)(/* [out,retval] */ IRenderingContext** ppVal)
	{
		if (ppVal != NULL)
		{
			*ppVal = m_pContext;
			if (*ppVal != NULL)
			{
				(*ppVal)->AddRef();
			}
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(putref_Context)(/* [in] */ IRenderingContext* pNewVal)
	{
		if (pNewVal != NULL)
		{
			pNewVal->AddRef();
		}
		if (m_pContext != NULL)
		{
			m_pContext->Release();
		}
		m_pContext = pNewVal;

		return S_OK;
	}

	STDMETHOD(get_Camera)(/* [out,retval] */ ICamera** ppVal)
	{
		if (ppVal == NULL)
		{
			return E_POINTER;
		}

		if (m_pCamera != NULL)
		{
			return m_pCamera->QueryInterface(IID_ICamera, (LPVOID*) ppVal);
		}

		return S_FALSE;
	}

	STDMETHOD(putref_Camera)(/* [in] */ ICamera* pNewVal)
	{
		ICameraEx* pCameraEx = NULL;

		if (pNewVal != NULL)
		{
			if (FAILED(pNewVal->QueryInterface(IID_ICameraEx, (LPVOID*) &pCameraEx)))
			{
				return E_FAIL;
			}
		}

		if (m_pCamera != NULL)
		{
			m_pCamera->Release();
		}

		m_pCamera = pCameraEx;

		return S_OK;
	}

// Implementation
protected:
	void DrawBox(LPDIRECT3DDEVICE8 pDevice, BOX3D& bounds, D3DCOLOR clr)
	{
		DWORD dwFillState;
		DWORD dwLightingState;

		pDevice->GetRenderState(D3DRS_FILLMODE, &dwFillState);
		pDevice->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		LVERTEX3D vertexArray[8];
		bounds.GetPoints(vertexArray);

		vertexArray[0].color = clr;
		vertexArray[1].color = clr;
		vertexArray[2].color = clr;
		vertexArray[3].color = clr;
		vertexArray[4].color = clr;
		vertexArray[5].color = clr;
		vertexArray[6].color = clr;
		vertexArray[7].color = clr;

		WORD indexArray[24];
		indexArray[0] = 0;
		indexArray[1] = 1;
		indexArray[2] = 1;
		indexArray[3] = 3;
		indexArray[4] = 3;
		indexArray[5] = 2;
		indexArray[6] = 2;
		indexArray[7] = 0;
		indexArray[8] = 1;
		indexArray[9] = 7;
		indexArray[10] = 7;
		indexArray[11] = 5;
		indexArray[12] = 5;
		indexArray[13] = 3;
		indexArray[14] = 7;
		indexArray[15] = 6;
		indexArray[16] = 6;
		indexArray[17] = 4;
		indexArray[18] = 4;
		indexArray[19] = 5;
		indexArray[20] = 4;
		indexArray[21] = 2;
		indexArray[22] = 6;
		indexArray[23] = 0;

		pDevice->SetVertexShader(D3DFVF_LVERTEX);
		pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 24, 12, indexArray,
		                                D3DFMT_INDEX16, vertexArray, sizeof(LVERTEX3D));

		pDevice->SetRenderState(D3DRS_FILLMODE, dwFillState);
		pDevice->SetRenderState(D3DRS_LIGHTING, dwLightingState);
	}

	void DrawNodeBounds(LPDIRECT3DDEVICE8 pDevice,IVisual* pVisual, D3DCOLOR clr)
	{
		VARIANT varCurBoundingBox;

		if (pVisual != NULL)
		{
			if (SUCCEEDED(pVisual->GetBoundingBox(&varCurBoundingBox)))
			{
				vtBOX3D boundingBox(varCurBoundingBox);
				DrawBox(pDevice, boundingBox, clr);
				::VariantClear(&varCurBoundingBox);
			}
		}
	}

	void DrawNodeAxes(LPDIRECT3DDEVICE8 pDevice, IVisual* pVisual, D3DCOLOR clr)
	{
		LVERTEX3D vertexArray[6];
		VARIANT varXAxis, varYAxis, varZAxis;

		pVisual->GetAxes(1.2f, &varXAxis, &varYAxis, &varZAxis);

		vtLINE3D xAxis(varXAxis);
		vtLINE3D yAxis(varYAxis);
		vtLINE3D zAxis(varZAxis);

		vertexArray[0].x = xAxis.x1;
		vertexArray[0].y = xAxis.y1;
		vertexArray[0].z = xAxis.z1;
		vertexArray[0].color = clr;

		vertexArray[1].x = xAxis.x2;
		vertexArray[1].y = xAxis.y2;
		vertexArray[1].z = xAxis.z2;
		vertexArray[1].color = clr;

		vertexArray[2].x = yAxis.x1;
		vertexArray[2].y = yAxis.y1;
		vertexArray[2].z = yAxis.z1;
		vertexArray[2].color = clr;

		vertexArray[3].x = yAxis.x2;
		vertexArray[3].y = yAxis.y2;
		vertexArray[3].z = yAxis.z2;
		vertexArray[3].color = clr;

		vertexArray[4].x = zAxis.x1;
		vertexArray[4].y = zAxis.y1;
		vertexArray[4].z = zAxis.z1;
		vertexArray[4].color = clr;

		vertexArray[5].x = zAxis.x2;
		vertexArray[5].y = zAxis.y2;
		vertexArray[5].z = zAxis.z2;
		vertexArray[5].color = clr;

		::VariantClear(&varXAxis);
		::VariantClear(&varYAxis);
		::VariantClear(&varZAxis);

		DWORD dwFillState;
		DWORD dwLightingState;

		pDevice->GetRenderState(D3DRS_FILLMODE, &dwFillState);
		pDevice->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		pDevice->SetVertexShader(D3DFVF_LVERTEX);
		pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, vertexArray, sizeof(LVERTEX3D));

		pDevice->SetRenderState(D3DRS_FILLMODE, dwFillState);
		pDevice->SetRenderState(D3DRS_LIGHTING, dwLightingState);
	}

	void HighlightSelection(LPDIRECT3DDEVICE8 pDevice, D3DCOLOR clr)
	{
		if (m_pSelectionList == NULL)
			return;

		INodesAccessor<IVisual> selList(m_pSelectionList);

		IVisual* pCurVisual = NULL;

		HRESULT hrLoop = selList.GetFirst(&pCurVisual);

		while (hrLoop == S_OK)
		{
			DrawNodeBounds(pDevice, pCurVisual, clr);

			pCurVisual->Release();
			pCurVisual = NULL;

			hrLoop = selList.GetNext(&pCurVisual);
		}
	}

	bool FillViewMatrix(D3DXMATRIX& viewMatrix)
	{
		if (m_pCamera != NULL)
		{
			m_pCamera->GetViewMatrix(&viewMatrix);
			return true;
		}
		return false;
	}

	bool FillProjectionMatrix(D3DXMATRIX& projectionMatrix)
	{
		D3DXMatrixPerspectiveFovLH(&projectionMatrix,
		                           m_fFOV,
		                           m_fAspect,
		                           m_fNearPlane,
		                           m_fFarPlane);
		return true;
	}

	void GetLogicalSize(const VALUE3D& z, VALUE3D& width, VALUE3D& height)
	{
		VALUE3D angle = m_fFOV / 2.0f;
		if (angle < 90.0f)
		{
			width = static_cast<VALUE3D>(tan(angle) * z) * 2;
			height = static_cast<VALUE3D>(tan(angle) * z) * 2;
			// TODO: Scale width and height by aspect ratio
		}
	}
};

/////////////////////////////////////////////////////////////////////////
// CViewport

class ATL_NO_VTABLE CViewport : public CComObjectRootEx< CComSingleThreadModel >,
                                        public CComCoClass<CViewport, &CLSID_Viewport>,
                                        public IDispatchRTPropConImpl<CViewport, IViewportImpl< IVisual2DImpl< ICompositeNodeImpl< IViewport, CInterfacePair<INode,IRenderDX8> > > >, &__uuidof(IViewport), &LIBID_ShapeShifter>,
                                        public IRenderDX8,
                                        public IDevicePresentationDX8Impl,
                                        public ISupportErrorInfo
{
// Embedded types
public:
	typedef IDispatchRTPropConImpl<CViewport, IViewportImpl< IVisual2DImpl< ICompositeNodeImpl< IViewport, CInterfacePair<INode,IRenderDX8> > > >, &__uuidof(IViewport), &LIBID_ShapeShifter> _BaseClass;

// Constructors/destructor
public:
	CViewport();
	virtual ~CViewport();

DECLARE_REGISTRY_RESOURCEID(IDR_VIEWPORT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CViewport)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
	COM_INTERFACE_ENTRY_IID(IID_IDevicePresentationDX8, IDevicePresentationDX8)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(IViewport)
	COM_INTERFACE_ENTRY(IVisual2D)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CViewport, 32)

	HRESULT FinalConstruct();
	void FinalRelease();

// Attributes
protected:
	IPresentationManager* m_pPresentationMgr;
	bool m_bInvalidateWindow;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IViewport interface

	STDMETHOD(_InitObject)(/* [in] */ IDevice* pDevice);
	STDMETHOD(get_Windowed)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_Windowed)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(get_ContainerWindow)(/*[out, retval]*/ long* pVal);
	STDMETHOD(put_ContainerWindow)(/*[in]*/ long newVal);
	STDMETHOD(get_InvalidateWindow)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_InvalidateWindow)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(Render)();
	STDMETHOD(RefreshScreen)();
	STDMETHOD(Invalidate)();
	STDMETHOD(DrawTracking)(/* [in] */ INodes* pTrackingNodes);

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

	//////////////////////////////////////////////////////////////
	// IVisual2D interface

	STDMETHOD(SetBounds)(LPRECT pBounds);

	//////////////////////////////////////////////////////////////
	// ISupportsErrorInfo interface

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// Implementation
protected:
	HRESULT BeginFrame(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);
	HRESULT EndFrame(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

public:
	HRESULT get_FillMode(/*[out, retval]*/ long* pVal);
	HRESULT put_FillMode(/*[in]*/ long newVal);
	HRESULT get_ZEnable(/*[out, retval]*/ long* pVal);
	HRESULT put_ZEnable(/*[in]*/ long newVal);
	HRESULT get_LightingEnabled(/*[out, retval]*/ VARIANT_BOOL* pVal);
	HRESULT put_LightingEnabled(/*[in]*/ VARIANT_BOOL newVal);
};

};  // namespace ShapeShifter

#endif // #ifndef __VIEWPORT_H__
