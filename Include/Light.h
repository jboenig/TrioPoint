//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Light.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __LIGHT_H__
#define __LIGHT_H__

#pragma once

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// ILightImpl

template <class light_interface, D3DLIGHTTYPE light_type>
class ILightImpl : public INodeImpl< IDispatchPropConImpl<light_interface, &__uuidof(light_interface), &LIBID_ShapeShifter> >,
                   public IContextObject
{
// Constructors/destructor
public:
	ILightImpl()
	{
		ZeroMemory(&m_light, sizeof(D3DLIGHT8));
		m_light.Type = light_type;
		m_nLightIdx = 0;
		m_bEnabled = TRUE;
	}

// Attributes
protected:
	DWORD m_nLightIdx;
	D3DLIGHT8 m_light;
	bool m_bEnabled;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IContextObject interface

	STDMETHOD(SelectObject)(LPDIRECT3DDEVICE8 pDevice,
	                        IResourceManagerDX8* pDevResources,
	                        DWORD dwTextureStage)
	{
		pDevResources;
		dwTextureStage;

		HRESULT hr = pDevice->SetLight(m_nLightIdx, &m_light);

		if (SUCCEEDED(hr))
		{
			if (m_bEnabled)
			{
				pDevice->LightEnable(m_nLightIdx, TRUE);
			}
			else
			{
				pDevice->LightEnable(m_nLightIdx, FALSE);
			}

		}

		return hr;
	}

	//////////////////////////////////////////////////////////////
	// ILight interface

	STDMETHOD(GetDiffuseColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue)
	{
		*red = m_light.Diffuse.r;
		*green = m_light.Diffuse.g;
		*blue = m_light.Diffuse.b;
		return S_OK;
	}

	STDMETHOD(SetDiffuseColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue)
	{
		m_light.Diffuse.r = red;
		m_light.Diffuse.g = green;
		m_light.Diffuse.b = blue;
		return S_OK;
	}

	STDMETHOD(GetSpecularColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue)
	{
		*red = m_light.Specular.r;
		*green = m_light.Specular.g;
		*blue = m_light.Specular.b;
		return S_OK;
	}

	STDMETHOD(SetSpecularColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue)
	{
		m_light.Specular.r = red;
		m_light.Specular.g = green;
		m_light.Specular.b = blue;
		return S_OK;
	}

	STDMETHOD(GetAmbientColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue)
	{
		*red = m_light.Ambient.r;
		*green = m_light.Ambient.g;
		*blue = m_light.Ambient.b;
		return S_OK;
	}

	STDMETHOD(SetAmbientColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue)
	{
		m_light.Ambient.r = red;
		m_light.Ambient.g = green;
		m_light.Ambient.b = blue;
		return S_OK;
	}

	STDMETHOD(SetPosition)(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z)
	{
		m_light.Position.x = x;
		m_light.Position.y = y;
		m_light.Position.z = z;
		return S_OK;
	}

	STDMETHOD(SetDirection)(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z)
	{
		m_light.Direction.x = x;
		m_light.Direction.y = y;
		m_light.Direction.z = z;
		return S_OK;
	}

	STDMETHOD(get_LightIndex)(/* [out, retval] */ long *pVal)
	{
		*pVal = m_nLightIdx;
		return S_OK;
	}

	STDMETHOD(put_LightIndex)(/* [in] */ long newVal)
	{
		m_nLightIdx = newVal;
		return S_OK;
	}

	STDMETHOD(get_Enabled)(/* [out, retval] */ VARIANT_BOOL *pVal)
	{
		*pVal = m_bEnabled ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}

	STDMETHOD(put_Enabled)(/* [in] */ VARIANT_BOOL newVal)
	{
		m_bEnabled = newVal ? true : false;
		return S_OK;
	}

	STDMETHOD(get_XPosition)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Position.x;
		return S_OK;
	}

	STDMETHOD(put_XPosition)(/* [in] */ VALUE3D newVal)
	{
		m_light.Position.x = newVal;
		return S_OK;
	}

	STDMETHOD(get_YPosition)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Position.y;
		return S_OK;
	}

	STDMETHOD(put_YPosition)(/* [in] */ VALUE3D newVal)
	{
		m_light.Position.y = newVal;
		return S_OK;
	}

	STDMETHOD(get_ZPosition)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Position.z;
		return S_OK;
	}

	STDMETHOD(put_ZPosition)(/* [in] */ VALUE3D newVal)
	{
		m_light.Position.z = newVal;
		return S_OK;
	}

	STDMETHOD(get_XDirection)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Direction.x;
		return S_OK;
	}

	STDMETHOD(put_XDirection)(/* [in] */ VALUE3D newVal)
	{
		m_light.Direction.x = newVal;
		return S_OK;
	}

	STDMETHOD(get_YDirection)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Direction.y;
		return S_OK;
	}

	STDMETHOD(put_YDirection)(/* [in] */ VALUE3D newVal)
	{
		m_light.Direction.y = newVal;
		return S_OK;
	}

	STDMETHOD(get_ZDirection)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Direction.z;
		return S_OK;
	}

	STDMETHOD(put_ZDirection)(/* [in] */ VALUE3D newVal)
	{
		m_light.Direction.z = newVal;
		return S_OK;
	}

	STDMETHOD(get_Range)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Range;
		return S_OK;
	}

	STDMETHOD(put_Range)(/* [in] */ VALUE3D newVal)
	{
		m_light.Range = newVal;
		return S_OK;
	}

	STDMETHOD(get_ConstantAttenuation)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Attenuation0;
		return S_OK;
	}

	STDMETHOD(put_ConstantAttenuation)(/* [in] */ VALUE3D newVal)
	{
		m_light.Attenuation0 = newVal;
		return S_OK;
	}

	STDMETHOD(get_LinearAttenuation)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Attenuation1;
		return S_OK;
	}

	STDMETHOD(put_LinearAttenuation)(/* [in] */ VALUE3D newVal)
	{
		m_light.Attenuation1 = newVal;
		return S_OK;
	}

	STDMETHOD(get_QuadraticAttenuation)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Attenuation2;
		return S_OK;
	}

	STDMETHOD(put_QuadraticAttenuation)(/* [in] */ VALUE3D newVal)
	{
		m_light.Attenuation2 = newVal;
		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////
// CDirectionalLight

class ATL_NO_VTABLE CDirectionalLight : public CComObjectRootEx<CComSingleThreadModel>,
                                        public CComCoClass<CDirectionalLight, &CLSID_DirectionalLight>,
                                        public ILightImpl<ILight, D3DLIGHT_DIRECTIONAL>,
                                        public IPersistXMLImpl<CDirectionalLight>
{
// Constructors/destructor
public:
	CDirectionalLight();
	virtual ~CDirectionalLight();

DECLARE_REGISTRY_RESOURCEID(IDR_DIRECTIONAL_LIGHT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_DirectionalLight)

// Interface map
BEGIN_COM_MAP(CDirectionalLight)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY_IID(IID_ILight, ILight)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

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
// CPointLight

class ATL_NO_VTABLE CPointLight : public CComObjectRootEx<CComSingleThreadModel>,
                                  public CComCoClass<CPointLight, &CLSID_PointLight>,
                                  public ILightImpl<ILight, D3DLIGHT_POINT>,
                                  public IPersistXMLImpl<CPointLight>
{
// Constructors/destructor
public:
	CPointLight();

DECLARE_REGISTRY_RESOURCEID(IDR_POINT_LIGHT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_PointLight)

// Interface map
BEGIN_COM_MAP(CPointLight)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY_IID(IID_ILight, ILight)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

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
// CSpotLight

class ATL_NO_VTABLE CSpotLight : public CComObjectRootEx<CComSingleThreadModel>,
                                 public CComCoClass<CSpotLight, &CLSID_SpotLight>,
                                 public ILightImpl<ISpotLight, D3DLIGHT_SPOT>,
                                 public IPersistXMLImpl<CSpotLight>
{
// Constructors/destructor
public:
	CSpotLight();

DECLARE_REGISTRY_RESOURCEID(IDR_SPOT_LIGHT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_SpotLight)

// Interface map
BEGIN_COM_MAP(CSpotLight)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY_IID(IID_ILight, ILight)
	COM_INTERFACE_ENTRY_IID(IID_ISpotLight, ISpotLight)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ISpotLight interface

	STDMETHOD(get_FallOff)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Falloff;
		return S_OK;
	}

	STDMETHOD(put_FallOff)(/* [in] */ VALUE3D newVal)
	{
		m_light.Falloff = newVal;
		return S_OK;
	}

	STDMETHOD(get_InnerSpotlightConeAngle)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Theta;
		return S_OK;
	}

	STDMETHOD(put_InnerSpotlightConeAngle)(/* [in] */ VALUE3D newVal)
	{
		m_light.Theta = newVal;
		return S_OK;
	}

	STDMETHOD(get_OuterSpotlightConeAngle)(/* [out, retval] */ VALUE3D *pVal)
	{
		*pVal = m_light.Phi;
		return S_OK;
	}

	STDMETHOD(put_OuterSpotlightConeAngle)(/* [in] */ VALUE3D newVal)
	{
		m_light.Phi = newVal;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);
};

};  // namespace ShapeShifter

#endif // #ifndef __LIGHT_H__
