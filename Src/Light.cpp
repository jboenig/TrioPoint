//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Light.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Light.h"
#include "XMLTags.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CDirectionalLight

CDirectionalLight::CDirectionalLight()
{
    m_light.Direction.x =  1.0f;
    m_light.Direction.y = -1.0f;
    m_light.Direction.z =  1.0f;
}

CDirectionalLight::~CDirectionalLight()
{
}

HRESULT CDirectionalLight::FinalConstruct()
{
	return put_Name(tag_Light);
}

STDMETHODIMP CDirectionalLight::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_DirectionalLight;
	return S_OK;
}

STDMETHODIMP CDirectionalLight::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;
	long nContext;
	VARIANT val;

	pArchive->GetContext(&nContext);

	hr = ReadElement(pArchive, nContext, tag_Name, &CDirectionalLight::put_Name);

	hr = ReadElement(pArchive, nContext, tag_Index, VT_I4, &val);
	if (SUCCEEDED(hr))
	{
		m_nLightIdx = val.lVal;
	}

	hr = ReadColor(pArchive, nContext, tag_Diffuse, m_light.Diffuse);
	hr = ReadColor(pArchive, nContext, tag_Specular, m_light.Specular);
	hr = ReadColor(pArchive, nContext, tag_Ambient, m_light.Ambient);

	hr = ReadVector(pArchive, nContext, tag_Position, m_light.Position.x, m_light.Position.y, m_light.Position.z);
	hr = ReadVector(pArchive, nContext, tag_Direction, m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);

	hr = ReadElement(pArchive, nContext, tag_Range, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Range = val.fltVal;
	}

	hr = ReadElement(pArchive, nContext, tag_Falloff, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Falloff = val.fltVal;
	}

	hr = ReadElement(pArchive, nContext, tag_Attenuation0, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Attenuation0 = val.fltVal;
	}

	hr = ReadElement(pArchive, nContext, tag_Attenuation1, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Attenuation1 = val.fltVal;
	}

	hr = ReadElement(pArchive, nContext, tag_Attenuation2, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Attenuation2 = val.fltVal;
	}

	hr = ReadElement(pArchive, nContext, tag_Theta, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Theta = val.fltVal;
	}

	hr = ReadElement(pArchive, nContext, tag_Phi, VT_R4, &val);
	if (SUCCEEDED(hr))
	{
		m_light.Phi = val.fltVal;
	}

	pArchive->FreeContext(nContext);

	return hr;
}

STDMETHODIMP CDirectionalLight::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nContext;

	pArchive->GetContext(&nContext);

	hr = WriteElement(pArchive, nContext, tag_Name, &CDirectionalLight::get_Name);

	hr = WriteElement(pArchive, nContext, tag_Index, _variant_t((long)m_nLightIdx), NULL);

	hr = WriteColor(pArchive, nContext, tag_Diffuse, m_light.Diffuse);
	hr = WriteColor(pArchive, nContext, tag_Specular, m_light.Specular);
	hr = WriteColor(pArchive, nContext, tag_Ambient, m_light.Ambient);

	hr = WriteVector(pArchive, nContext, tag_Position, m_light.Position.x, m_light.Position.y, m_light.Position.z);

	hr = WriteVector(pArchive, nContext, tag_Direction, m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);

	hr = WriteElement(pArchive, nContext, tag_Range, _variant_t(m_light.Range), NULL);
	hr = WriteElement(pArchive, nContext, tag_Falloff, _variant_t(m_light.Falloff), NULL);
	hr = WriteElement(pArchive, nContext, tag_Attenuation0, _variant_t(m_light.Attenuation0), NULL);
	hr = WriteElement(pArchive, nContext, tag_Attenuation1, _variant_t(m_light.Attenuation1), NULL);
	hr = WriteElement(pArchive, nContext, tag_Attenuation2, _variant_t(m_light.Attenuation2), NULL);
	hr = WriteElement(pArchive, nContext, tag_Theta, _variant_t(m_light.Theta), NULL);
	hr = WriteElement(pArchive, nContext, tag_Phi, _variant_t(m_light.Phi), NULL);

	pArchive->FreeContext(nContext);

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// CPointLight

CPointLight::CPointLight()
{
	m_light.Attenuation0 = 1.0f;
}

HRESULT CPointLight::FinalConstruct()
{
	return put_Name(tag_Light);
}

STDMETHODIMP CPointLight::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_PointLight;
	return S_OK;
}

STDMETHODIMP CPointLight::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = E_FAIL;

	pArchive;

	return hr;
}

STDMETHODIMP CPointLight::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	pArchive;

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// CSpotLight

CSpotLight::CSpotLight()
{
	m_light.Attenuation0 = 1.0f;
	m_light.Falloff = 1.0f;
}

HRESULT CSpotLight::FinalConstruct()
{
	return put_Name(tag_Light);
}

STDMETHODIMP CSpotLight::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_SpotLight;
	return S_OK;
}

STDMETHODIMP CSpotLight::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = E_FAIL;

	pArchive;

	return hr;
}

STDMETHODIMP CSpotLight::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	pArchive;

	return hr;
}

};  // namespace ShapeShifter
