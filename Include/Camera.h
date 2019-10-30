//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Camera.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __CAMERA_H__
#define __CAMERA_H__

#pragma once

#include "ShapeShifter.h"
#include "Node.h"
#include "Property.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CCamera

class ATL_NO_VTABLE CCamera : public CComObjectRootEx<CComSingleThreadModel>,
                              public CComCoClass<CCamera, &CLSID_Camera>,
                              public INodeImpl< IDispatchPropConImpl<ICameraEx, &IID_ICamera, &LIBID_ShapeShifter> >,
                              public IPersistXMLImpl<CCamera>
{
// Constructors/destructor
public:
	CCamera();
	virtual ~CCamera();

DECLARE_REGISTRY_RESOURCEID(IDR_CAMERA)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Camera)

// Interface map
BEGIN_COM_MAP(CCamera)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICamera)
	COM_INTERFACE_ENTRY_IID(IID_ICameraEx, ICameraEx)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	D3DXVECTOR3 m_cameraPos;
	D3DXVECTOR3 m_cameraAim;
	D3DXVECTOR3 m_cameraUp;

// Operations
public:

	//////////////////////////////////////////////////////////////
	// ICamera interface

	STDMETHOD(SetLocation)(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z);
	STDMETHOD(GetLocation)(/* [out] */ VALUE3D* px, /* [out] */ VALUE3D* py, /* [out] */ VALUE3D* pz);

	STDMETHOD(SetAimVector)(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z);
	STDMETHOD(GetAimVector)(/* [out] */ VALUE3D* px, /* [out] */ VALUE3D* py, /* [out] */ VALUE3D* pz);

	STDMETHOD(SetUpVector)(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z);
	STDMETHOD(GetUpVector)(/* [out] */ VALUE3D* px, /* [out] */ VALUE3D* py, /* [out] */ VALUE3D* pz);

	STDMETHOD(PointAt)(/* [in] */ VALUE3D x, /* [in] */ VALUE3D y, /* [in] */ VALUE3D z);

	STDMETHOD(Move)(/* [in] */ enum CameraMovement moveType, /* [in] */ VALUE3D fAmount);

	STDMETHOD(Translate)(/* [in] */ VALUE3D dx, /* [in] */ VALUE3D dy, /* [in] */ VALUE3D dz);

	STDMETHOD(Rotate)(/* [in] */ VALUE3D fXRadians,
	                  /* [in] */ VALUE3D fYRadians,
	                  /* [in] */ VALUE3D fZRadians);

	//////////////////////////////////////////////////////////////
	// ICameraEx interface

	STDMETHOD(GetCameraState)(D3DVECTOR* pCameraPos,
	                          D3DVECTOR* pCameraAim,
	                          D3DVECTOR* pCameraUp);
	STDMETHOD(SetCameraState)(D3DVECTOR cameraPos,
	                          D3DVECTOR cameraAim,
	                          D3DVECTOR cameraUp);
	STDMETHOD(GetViewMatrix)(D3DXMATRIX* pViewMatrix);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);
};

};  // namespace ShapeShifter

#endif // #ifndef __CAMERA_H__
