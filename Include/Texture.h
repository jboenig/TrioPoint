//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Texture.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma once

#include <list>

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CTexture

class ATL_NO_VTABLE CTexture : public CComObjectRootEx<CComSingleThreadModel>,
                               public CComCoClass<CTexture, &CLSID_Texture>,
                               public INodeImpl< IDispatchPropConImpl<ITexture, &__uuidof(ITexture), &LIBID_ShapeShifter> >,
                               public IContextObject,
                               public IPersistXMLImpl<CTexture>
{
// Embedded types
public:
	enum TextureSourceType
	{
		TEXTURE_NONE,
		TEXTURE_FILE,
		TEXTURE_RESOURCE,
		TEXTURE_MEMORY,
		TEXTURE_URL
	};

// Constructors/destructor
public:
	CTexture();
	virtual ~CTexture();

DECLARE_REGISTRY_RESOURCEID(IDR_TEXTURE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Texture)

// Interface map
BEGIN_COM_MAP(CTexture)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ITexture)
	COM_INTERFACE_ENTRY(IDeviceResource)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	DeviceResourceId m_idTexture;
	UINT m_nWidth;
	UINT m_nHeight;
	UINT m_nLevels;
	D3DFORMAT m_format;
	TextureSourceType m_sourceType;
	union
	{
		BSTR fileName;
		BSTR resStr;
		HANDLE hDIB;
	} m_imageSrc;
	HMODULE m_hResModule;

// Operations
public:
	/////////////////////////////////////////////////////////////////////
	// IContextObject

	STDMETHOD(SelectObject)(LPDIRECT3DDEVICE8 pDevice,
	                        IResourceManagerDX8* pDevResources,
	                        DWORD dwTextureStage);

	//////////////////////////////////////////////////////////////
	// IDeviceResource interface

	STDMETHOD(LoadDevice)(/* [in] */ IDevice* pDevice);
	STDMETHOD(UnloadDevice)(/* [in] */ IDevice* pDevice);

	//////////////////////////////////////////////////////////////
	// ITexture interface

	STDMETHOD(get_Width)(/* [out, retval] */ long *pVal);
	STDMETHOD(put_Width)(/* [in] */ long newVal);
	STDMETHOD(get_Height)(/* [out, retval] */ long *pVal);
	STDMETHOD(put_Height)(/* [in] */ long newVal);
	STDMETHOD(get_FileName)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(put_FileName)(/* [in] */ BSTR newVal);
	STDMETHOD(get_TextureId)(/* [out, retval] */ DeviceResourceId *pVal);
	STDMETHOD(put_TextureId)(/* [in] */ DeviceResourceId newVal);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

// Implementation
protected:
	void FreeImageSrc();
};

};  // namespace ShapeShifter

#endif // #ifndef __TEXTURE_H__
