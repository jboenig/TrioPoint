//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Material.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Material.h"

namespace ShapeShifter {

CMaterial::CMaterial()
{
    ZeroMemory(&m_material, sizeof(D3DMATERIAL8));
}

CMaterial::~CMaterial()
{
}

HRESULT CMaterial::FinalConstruct()
{
	return put_Name(tag_Material);
}

STDMETHODIMP CMaterial::SelectObject(LPDIRECT3DDEVICE8 pDevice, 
									 IResourceManagerDX8* pDevResources,
									 DWORD dwTextureStage)
{
	pDevResources;
	dwTextureStage;
	return pDevice->SetMaterial(&m_material);
}

STDMETHODIMP CMaterial::SetDiffuseColor(VALUE3D red, VALUE3D green, VALUE3D blue, VALUE3D alpha)
{
	m_material.Diffuse.r = red;
	m_material.Diffuse.g = green;
	m_material.Diffuse.b = blue;
	m_material.Diffuse.a = alpha;
	return S_OK;
}

STDMETHODIMP CMaterial::GetDiffuseColor(VALUE3D* /* pRed */, VALUE3D* /* pGreen */, VALUE3D* /* pBlue */, VALUE3D* /* pAlpha */)
{
	return E_FAIL;
}

STDMETHODIMP CMaterial::SetAmbientColor(VALUE3D red, VALUE3D green, VALUE3D blue, VALUE3D alpha)
{
	m_material.Ambient.r = red;
	m_material.Ambient.g = green;
	m_material.Ambient.b = blue;
	m_material.Ambient.a = alpha;
	return S_OK;
}

STDMETHODIMP CMaterial::GetAmbientColor(VALUE3D* /* pRed */, VALUE3D* /* pGreen */, VALUE3D* /* pBlue */, VALUE3D* /* pAlpha */)
{
	return E_FAIL;
}

STDMETHODIMP CMaterial::SetSpecularColor(VALUE3D red, VALUE3D green, VALUE3D blue, VALUE3D alpha)
{
	m_material.Specular.r = red;
	m_material.Specular.g = green;
	m_material.Specular.b = blue;
	m_material.Specular.a = alpha;
	return S_OK;
}

STDMETHODIMP CMaterial::GetSpecularColor(VALUE3D* /* pRed */, VALUE3D* /* pGreen */, VALUE3D* /* pBlue */, VALUE3D* /* pAlpha */)
{
	return E_FAIL;
}

STDMETHODIMP CMaterial::SetEmissiveColor(VALUE3D red, VALUE3D green, VALUE3D blue, VALUE3D alpha)
{
	m_material.Emissive.r = red;
	m_material.Emissive.g = green;
	m_material.Emissive.b = blue;
	m_material.Emissive.a = alpha;
	return S_OK;
}

STDMETHODIMP CMaterial::GetEmissiveColor(VALUE3D* /* pRed */, VALUE3D* /* pGreen */, VALUE3D* /* pBlue */, VALUE3D* /* pAlpha */)
{
	return E_FAIL;
}

STDMETHODIMP CMaterial::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Material;
	return S_OK;
}

STDMETHODIMP CMaterial::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nContext;
	pArchive->GetContext(&nContext);

	hr = ReadElement(pArchive, nContext, tag_Name, &CMaterial::put_Name);
	hr = ReadColor(pArchive, nContext, tag_Diffuse, m_material.Diffuse);
	hr = ReadColor(pArchive, nContext, tag_Ambient, m_material.Ambient);
	hr = ReadColor(pArchive, nContext, tag_Specular, m_material.Specular);
	hr = ReadColor(pArchive, nContext, tag_Emissive, m_material.Emissive);

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CMaterial::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nContext;

	pArchive->GetContext(&nContext);

	hr = WriteElement(pArchive, nContext, tag_Name, &CMaterial::get_Name);
	hr = WriteColor(pArchive, nContext, tag_Diffuse, m_material.Diffuse);
	hr = WriteColor(pArchive, nContext, tag_Ambient, m_material.Ambient);
	hr = WriteColor(pArchive, nContext, tag_Specular, m_material.Specular);
	hr = WriteColor(pArchive, nContext, tag_Emissive, m_material.Emissive);

	pArchive->FreeContext(nContext);

	return hr;
}

};  // namespace ShapeShifter

