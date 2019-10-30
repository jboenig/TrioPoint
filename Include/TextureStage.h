//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: TextureStage.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __TEXTURESTAGE_H__
#define __TEXTURESTAGE_H__

#pragma once

#include <list>

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CTextureStage

class ATL_NO_VTABLE CTextureStage : public CComObjectRootEx<CComSingleThreadModel>,
                                    public CComCoClass<CTextureStage, &CLSID_TextureStage>,
									public INodeImpl< IDispatchRTPropConImpl<CTextureStage, ITextureStage, &__uuidof(ITextureStage), &LIBID_ShapeShifter> >,
									public IContextObject,
									public IPersistXMLImpl<CTextureStage>
{
// Embedded types
public:

	typedef INodeImpl< IDispatchRTPropConImpl<CTextureStage, ITextureStage, &__uuidof(ITextureStage), &LIBID_ShapeShifter> > _BaseClass;

	struct _State
	{
		D3DTEXTURESTAGESTATETYPE d3dStateType;
		DWORD dwStateValue;

		_State(const D3DTEXTURESTAGESTATETYPE d3dTypeIn,
		       const DWORD dwValIn);
		_State(const _State& src);
		_State& operator=(const _State& src);
	};

	typedef std::vector<_State> _StateArray;

// Constructors/destructor
public:
	CTextureStage();
	virtual ~CTextureStage();

DECLARE_REGISTRY_RESOURCEID(IDR_TEXTURE_STAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_TextureStage)

// Interface map
BEGIN_COM_MAP(CTextureStage)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(ITextureStage)
	COM_INTERFACE_ENTRY(IDeviceResource)
	COM_INTERFACE_ENTRY_IID(IID_IContextObject, IContextObject)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

SSCL_DECLARE_RUNTIME_PROPERTY_TABLE(CTextureStage, 32)

	HRESULT FinalConstruct();

// Attributes
protected:
	DWORD m_dwStageNum;
	ITexture* m_pTexture;
	bool m_bOwnsTexture;
	_StateArray m_states;

// Operations
public:
	/////////////////////////////////////////////////////////////////////
	// ITextureStage

	STDMETHOD(get_StageNum)(/* [out, retval] */ long *pVal);
	STDMETHOD(put_StageNum)(/* [in] */ long newVal);

	STDMETHOD(get_Texture)(/* [out, retval] */ ITexture** ppTexture);
	STDMETHOD(putref_Texture)(/* [in] */ ITexture* pTexture);

	STDMETHOD(SetTextureState)(/* [in] */ short nStateType,
	                           /* [in] */ long nStateValue);

	STDMETHOD(SetTextureStateFloat)(/* [in] */ short nStateType,
	                                /* [in] */ float fStateValue);

	STDMETHOD(SetTextureStateRGBA)(/* [in] */ short nStateType,
	                               /* [in] */ VALUE3D fRed,
	                               /* [in] */ VALUE3D fGreen,
	                               /* [in] */ VALUE3D fBlue,
	                               /* [in] */ VALUE3D fAlpha);

	STDMETHOD(ClearTextureState)(/* [in] */ short nStateType);
	STDMETHOD(ClearAllTextureStates)();

	//////////////////////////////////////////////////////////////
	// ICompositeNode interface

	STDMETHOD(GetChildCount)(/* [out,retval] */ long* pNumChildren);
	STDMETHOD(GetChildIndex)(/* [in] */ BSTR strName, /* [out,retval] */ long* pChildIndex);
	STDMETHOD(GetChild)(/* [in] */ long nChildIndex, /* [out,retval] */ INode** ppChild);
	STDMETHOD(GetChildByName)(/* [in] */ BSTR strName, /* [out,retval] */ INode** ppNode);
	STDMETHOD(AppendChild)(/* [in] */ INode* pChild);
	STDMETHOD(InsertChild)(/* [in] */ INode* pChild, /* [in] */ long nChildIndex);
	STDMETHOD(RemoveChild)(/* [in] */ INode* pChild);
	STDMETHOD(RemoveAllChildren)();
	STDMETHOD(OnChildUpdate)(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode);

	//////////////////////////////////////////////////////////////
	// IDeviceResource interface

	STDMETHOD(LoadDevice)(/* [in] */ IDevice* pDevice);
	STDMETHOD(UnloadDevice)(/* [in] */ IDevice* pDevice);

	/////////////////////////////////////////////////////////////////////
	// IContextObject

	STDMETHOD(SelectObject)(LPDIRECT3DDEVICE8 pDevice,
	                        IResourceManagerDX8* pDevResources,
							DWORD dwTextureStage);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

	//////////////////////////////////////////////////////////////
	// Property accessors

	HRESULT get_MagFilter(short* pVal);
	HRESULT put_MagFilter(short newVal);
	HRESULT get_MinFilter(short* pVal);
	HRESULT put_MinFilter(short newVal);
	HRESULT get_MipFilter(short* pVal);
	HRESULT put_MipFilter(short newVal);
	HRESULT get_TextureAddressU(short* pVal);
	HRESULT put_TextureAddressU(short newVal);
	HRESULT get_TextureAddressV(short* pVal);
	HRESULT put_TextureAddressV(short newVal);

// Implementation
protected:
	_StateArray::iterator FindTextureStageState(const D3DTEXTURESTAGESTATETYPE d3dStateType);
};

};  // namespace ShapeShifter

#endif // #ifndef __TEXTURESTAGE_H__
