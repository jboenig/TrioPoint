//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: FaceArray.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __FACEARRAY_H__
#define __FACEARRAY_H__

#pragma once

#include <vector>

#include "ShapeShifter.h"
#include "DynamicArray.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CFaceArray

class ATL_NO_VTABLE CFaceArray : public CComObjectRootEx<CComSingleThreadModel>,
                                 public CComCoClass<CFaceArray, &CLSID_FaceArray>,
                                 public IDispatchImpl<IFaceArray, &IID_IFaceArray, &LIBID_ShapeShifter>
{
// Embedded types
public:
	struct _Face
	{
		PrimitiveIndex m_nPrimIdx;
		FaceIndex m_nFaceIdx;

		_Face(const PrimitiveIndex nPrimIdx, const FaceIndex nFaceIdx)
		{
			m_nPrimIdx = nPrimIdx;
			m_nFaceIdx = nFaceIdx;
		}
	};

	typedef std::vector<_Face> _FaceArray;

// Constructors/destructor
public:
	CFaceArray();
	virtual ~CFaceArray();

DECLARE_REGISTRY_RESOURCEID(IDR_FACEARRAY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CFaceArray)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IFaceArray)
END_COM_MAP()

// Attributes
protected:
	_FaceArray m_faces;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IFaceArray interface

	STDMETHOD(GetFaceCount)(/* [out,retval] */ long* pNumFaces);

	STDMETHOD(GetFacePrimitiveIndex)(/* [in] */ long nArrayIdx,
	                                 /* [out,retval] */ PrimitiveIndex* pPrimIdx);

	STDMETHOD(GetFaceIndex)(/* [in] */ long nArrayIdx,
	                        /* [out,retval] */ FaceIndex* pFaceIdx);

	STDMETHOD(AddFace)(/* [in] */ PrimitiveIndex nPrimIdx,
	                   /* [in] */ FaceIndex nFaceIdx);

	STDMETHOD(Clear)();
};

};  // namespace ShapeShifter

#endif // #ifndef __FACEARRAY_H__
