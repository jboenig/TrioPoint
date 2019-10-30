//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: MeshEditor.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MESHEDITOR_H__
#define __MESHEDITOR_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CMeshEditor

class ATL_NO_VTABLE CMeshEditor : public CComObjectRootEx<CComSingleThreadModel>,
                                  public CComCoClass<CMeshEditor, &CLSID_MeshEditor>,
								  public IDispatchImpl<IMeshEditor, &IID_IMeshEditor, &LIBID_ShapeShifter>
{
public:
	CMeshEditor();
	~CMeshEditor();

DECLARE_REGISTRY_RESOURCEID(IDR_MESHEDITOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CMeshEditor)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IMeshEditor)
END_COM_MAP()

// Attributes
protected:
	IMeshEx* m_pMesh;
	PrimitiveIndex m_curPrimIdx;

// Operations
public:
	STDMETHOD(BeginEdit)(/* [in] */ IMesh* pMesh);

	STDMETHOD(SelectPrimitive)(/* [in] */ PrimitiveIndex nPrimIdx);

	STDMETHOD(AppendVertex)(/* [in] */ enum VertexField Field,
	                        /* [in] */ VARIANT varVal);

	STDMETHOD(UpdateVertex)(/* [in] */ VertexIndex nVertIdx,
                            /* [in] */ enum VertexField Field,
	                        /* [in] */ VARIANT varVal);

	STDMETHOD(RemoveVertex)(/* [in] */ VertexIndex nVertIdx);

	STDMETHOD(UpdateVertexPosition)(/* [in] */ VertexIndex nVertIdx,
	                                /* [in] */ VALUE3D x,
	                                /* [in] */ VALUE3D y,
	                                /* [in] */ VALUE3D z);

	STDMETHOD(UpdateTextureCoordinate)(/* [in] */ VertexIndex nVertIdx,
	                                   /* [in] */ VALUE3D tu,
	                                   /* [in] */ VALUE3D tv);

	STDMETHOD(AppendFace)(/* [in] */ VARIANT varTriangle);

	STDMETHOD(UpdateFaceNormal)(/* [in] */ FaceIndex nFaceIdx,
	                            /* [in] */ VALUE3D NX,
	                            /* [in] */ VALUE3D NY,
	                            /* [in] */ VALUE3D NZ);

	STDMETHOD(MakeCube)(/* [in] */ VALUE3D cx, /* [in] */ VALUE3D cy, /* [in] */ VALUE3D cz);

	STDMETHOD(MakePyramid)(/* [in] */ VALUE3D fBase,
	                       /* [in] */ VALUE3D fHeight);

	STDMETHOD(MakeSphere)(/* [in] */ VALUE3D Radius, /* [in] */ short LatitudeCount, /* [in] */ short LongitudeCount);

	STDMETHOD(MakeCylinder)(/* [in] */ VALUE3D fRadius,
	                        /* [in] */ VALUE3D fHeight,
	                        /* [in] */ short nNumSlices);

	STDMETHOD(MakeTube)(/* [in] */ VALUE3D fInnerRadius,
	                    /* [in] */ VALUE3D fOuterRadius,
	                    /* [in] */ VALUE3D fHeight,
	                    /* [in] */ short nNumSlices);

	STDMETHOD(MakeCone)(/* [in] */ VALUE3D BaseRadius,
	                    /* [in] */ VALUE3D Height,
	                    /* [in] */ short nNumSlices);

	STDMETHOD(EndEdit)();
};

};  // namespace ShapeShifter

#endif // #ifndef __MESHEDITOR_H__
