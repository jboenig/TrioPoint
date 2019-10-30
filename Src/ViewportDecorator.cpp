//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ViewportDecorator.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "ViewportDecorator.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CAxisGuide

CAxisGuide::CAxisGuide()
{
	m_fLen = 50.0f;
	m_clrX = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.0f);
	m_clrY = D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 0.0f);
	m_clrZ = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.0f);
	m_name = ::SysAllocString(L"AxisGuide");
}

CAxisGuide::~CAxisGuide()
{
}

STDMETHODIMP CAxisGuide::get_AxisLength(/* [out, retval] */ VALUE3D* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_fLen;

	return S_OK;
}

STDMETHODIMP CAxisGuide::put_AxisLength(/* [in] */ VALUE3D newVal)
{
	m_fLen = newVal;
	return S_OK;
}

STDMETHODIMP CAxisGuide::Render(LPDIRECT3DDEVICE8 pDevice,
                                IResourceManagerDX8*)
{
	if (m_bVisible)
	{
		DWORD dwFillState;
		DWORD dwLightingState;
		VALUE3D fMinVal = -(m_fLen / 2);

		pDevice->GetRenderState(D3DRS_FILLMODE, &dwFillState);
		pDevice->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		LVERTEX3D vertexArray[6];
		vertexArray[0].x = fMinVal;
		vertexArray[0].y = 0;
		vertexArray[0].z = 0;
		vertexArray[1].x = vertexArray[0].x + m_fLen;
		vertexArray[1].y = 0;
		vertexArray[1].z = 0;
		vertexArray[2].x = 0;
		vertexArray[2].y = fMinVal;
		vertexArray[2].z = 0;
		vertexArray[3].x = 0;
		vertexArray[3].y = vertexArray[2].y + m_fLen;
		vertexArray[3].z = 0;
		vertexArray[4].x = 0;
		vertexArray[4].y = 0;
		vertexArray[4].z = fMinVal;
		vertexArray[5].x = 0;
		vertexArray[5].y = 0;
		vertexArray[5].z = vertexArray[4].z + m_fLen;

		vertexArray[0].color = m_clrX;
		vertexArray[1].color = m_clrX;
		vertexArray[2].color = m_clrY;
		vertexArray[3].color = m_clrY;
		vertexArray[4].color = m_clrZ;
		vertexArray[5].color = m_clrZ;

		pDevice->SetVertexShader(D3DFVF_LVERTEX);
		pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 3, vertexArray, sizeof(LVERTEX3D));

		pDevice->SetRenderState(D3DRS_FILLMODE, dwFillState);
		pDevice->SetRenderState(D3DRS_LIGHTING, dwLightingState);
	}

	return S_OK;
}

};  // namespace ShapeShifter

