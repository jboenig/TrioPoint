//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Tracker.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Tracker.h"

namespace ShapeShifter {

CTracker::CTracker()
{
	m_trackingColor = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.0f);
}

CTracker::~CTracker()
{
}

STDMETHODIMP CTracker::Render(LPDIRECT3DDEVICE8 pDevice,
                              IResourceManagerDX8* pDevResources)
{
	pDevResources;

	D3DXVECTOR3 minPt;
	m_boundingBox.GetMinPoint(minPt);
	D3DXVec3TransformCoord(&minPt, &minPt, &m_mat);

	D3DXVECTOR3 maxPt;
	m_boundingBox.GetMaxPoint(maxPt);
	D3DXVec3TransformCoord(&maxPt, &maxPt, &m_mat);

	BOX3D trackerBox;
	trackerBox.SetMinMax(minPt, maxPt);

	DWORD dwFillState;
	DWORD dwLightingState;

	pDevice->GetRenderState(D3DRS_FILLMODE, &dwFillState);
	pDevice->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	LVERTEX3D vertexArray[8];
	trackerBox.GetPoints(vertexArray);

	vertexArray[0].color = m_trackingColor;
	vertexArray[1].color = m_trackingColor;
	vertexArray[2].color = m_trackingColor;
	vertexArray[3].color = m_trackingColor;
	vertexArray[4].color = m_trackingColor;
	vertexArray[5].color = m_trackingColor;
	vertexArray[6].color = m_trackingColor;
	vertexArray[7].color = m_trackingColor;

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

	return S_OK;
}

STDMETHODIMP CTracker::get_Left(/*[out, retval]*/ VALUE3D *pVal)
{
	_ASSERTE(pVal != NULL);
	*pVal = m_boundingBox.left;
	return S_OK;
}

STDMETHODIMP CTracker::get_Right(/*[out, retval]*/ VALUE3D *pVal)
{
	_ASSERTE(pVal != NULL);
	*pVal = m_boundingBox.right;
	return S_OK;
}

STDMETHODIMP CTracker::get_Top(/*[out, retval]*/ VALUE3D *pVal)
{
	_ASSERTE(pVal != NULL);
	*pVal = m_boundingBox.top;
	return S_OK;
}

STDMETHODIMP CTracker::get_Bottom(/*[out, retval]*/ VALUE3D *pVal)
{
	_ASSERTE(pVal != NULL);
	*pVal = m_boundingBox.bottom;
	return S_OK;
}

STDMETHODIMP CTracker::get_Front(/*[out, retval]*/ VALUE3D *pVal)
{
	_ASSERTE(pVal != NULL);
	*pVal = m_boundingBox.front;
	return S_OK;
}

STDMETHODIMP CTracker::get_Back(/*[out, retval]*/ VALUE3D *pVal)
{
	_ASSERTE(pVal != NULL);
	*pVal = m_boundingBox.back;
	return S_OK;
}

STDMETHODIMP CTracker::GetOrigin(/* [out,retval] */ VARIANT* pVarPoint)
{
	HRESULT hr = E_FAIL;

	if (pVarPoint != NULL)
	{
		POINT3D ptCenter;
		m_boundingBox.GetCenter(ptCenter);
		*pVarPoint = vtPOINT3D(ptCenter);
		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CTracker::GetBoundingBox(/* [out,retval] */ VARIANT* pVarBox)
{
	if (pVarBox != NULL)
	{
		*pVarBox = vtBOX3D(m_boundingBox);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CTracker::Translate(/* [in] */ VALUE3D tx, /* [in] */ VALUE3D ty, /* [in] */ VALUE3D tz)
{
	D3DXMatrixIdentity(&m_mat);
	return _VisualBase::Translate(tx, ty, tz);
}

STDMETHODIMP CTracker::Scale(/* [in] */ VALUE3D, /* [in] */ VALUE3D, /* [in] */ VALUE3D)
{
	return E_FAIL;
}

STDMETHODIMP CTracker::Rotate(/* [in] */ VALUE3D,
                              /* [in] */ VALUE3D,
                              /* [in] */ VALUE3D,
                              /* [in] */ VALUE3D)
{
	return E_FAIL;
}

STDMETHODIMP CTracker::HitTestRay(/* [in] */ VARIANT varRayOrigin,
                                  /* [in] */ VARIANT varRayDirection,
                                  /* [in] */ enum HitTestStrategy strategy)
{
	varRayOrigin;
	varRayDirection;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::HitTestSphere(/* [in] */ VARIANT varSphere,
                                     /* [in] */ enum HitTestStrategy strategy)
{
	varSphere;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::HitTestBox(/* [in] */ VARIANT varBox,
                                  /* [in] */ enum HitTestStrategy strategy)
{
	varBox;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::HitTestPlane(/* [in] */ VARIANT varPlane,
                                    /* [in] */ enum HitTestStrategy strategy)
{
	varPlane;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::GetChildrenIntersectingRay(/* [in] */ INodes* pNodeCollection,
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

STDMETHODIMP CTracker::GetChildrenIntersectingSphere(/* [in] */ INodes* pNodeCollection,
                                                     /* [in] */ VARIANT varSphere,
                                                     /* [in] */ enum HitTestStrategy strategy)
{
	pNodeCollection;
	varSphere;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::GetChildrenIntersectingBox(/* [in] */ INodes* pNodeCollection,
                                                  /* [in] */ VARIANT varBox,
                                                  /* [in] */ enum HitTestStrategy strategy)
{
	pNodeCollection;
	varBox;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::GetChildrenIntersectingPlane(/* [in] */ INodes* pNodeCollection,
                                                    /* [in] */ VARIANT varPlane,
                                                    /* [in] */ enum HitTestStrategy strategy)
{
	pNodeCollection;
	varPlane;
	strategy;
	return S_FALSE;
}

STDMETHODIMP CTracker::OnChildUpdate(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode)
{
	pNode;

	if ((nNotifyCode & ChildNotification::ADD) ||
	    (nNotifyCode & ChildNotification::REMOVE) ||
	    (nNotifyCode & ChildNotification::REMOVE_ALL))
	{
		_NodeVector::iterator itChild = m_children.begin();

		while (itChild != m_children.end())
		{
			IVisual* pCurVisual = NULL;

			if (SUCCEEDED((*itChild)->QueryInterface(IID_IVisual, (LPVOID*) &pCurVisual)))
			{
				_variant_t varBounds;
				pCurVisual->GetBoundingBox(&varBounds);
				m_boundingBox = vtBOX3D(varBounds);
				pCurVisual->Release();
			}

			itChild++;
		}
	}

	return S_OK;
}

};  // namespace ShapeShifter

