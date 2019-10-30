//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Material.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#pragma once

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CMaterial

class ATL_NO_VTABLE CMaterial : public CComObjectRootEx<CComSingleThreadModel>,
                                public CComCoClass<CMaterial, &CLSID_Material>,
                                public INodeImpl< IDispatchPropConImpl<IMaterial, &__uuidof(IMaterial), &LIBID_ShapeShifter> >,
                                public IContextObject,
                                public IPersistXMLImpl<CMaterial>
{
// Constructors/destructor
public:
	CMaterial();
	virtual ~CMaterial();

DECLARE_REGISTRY_RESOURCEID(IDR_MATERIAL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Material)

// Interface map
BEGIN_COM_MAP(CMaterial)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(IMaterial)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	D3DMATERIAL8 m_material;

// Operations
public:

	//////////////////////////////////////////////////////////////
	// IContextObject interface

	STDMETHOD(SelectObject)(LPDIRECT3DDEVICE8 pDevice,
	                        IResourceManagerDX8* pDevResources,
	                        DWORD dwTextureStage);

	//////////////////////////////////////////////////////////////
	// IMaterial interface

	STDMETHOD(SetDiffuseColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue, /* [in] */ VALUE3D alpha);
	STDMETHOD(GetDiffuseColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue, /* [out] */ VALUE3D* alpha);
	STDMETHOD(SetSpecularColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue, /* [in] */ VALUE3D alpha);
	STDMETHOD(GetSpecularColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue, /* [out] */ VALUE3D* alpha);
	STDMETHOD(SetAmbientColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue, /* [in] */ VALUE3D alpha);
	STDMETHOD(GetAmbientColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue,/*  [out] */ VALUE3D* alpha);
	STDMETHOD(SetEmissiveColor)(/* [in] */ VALUE3D red, /* [in] */ VALUE3D green, /* [in] */ VALUE3D blue, /* [in] */ VALUE3D alpha);
	STDMETHOD(GetEmissiveColor)(/* [out] */ VALUE3D* red, /* [out] */ VALUE3D* green, /* [out] */ VALUE3D* blue, /* [out] */ VALUE3D* alpha);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);
};

};  // namespace ShapeShifter

#endif // #ifndef __MATERIAL_H__
