//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReaderDXF.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MODELREADERDXF_H__
#define __MODELREADERDXF_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReaderDXF

class ATL_NO_VTABLE CModelReaderDXF : public CComObjectRootEx<CComSingleThreadModel>,
                                      public CComCoClass<CModelReaderDXF, &CLSID_ModelReaderDXF>,
                                      public IDispatchImpl<IModelReader, &__uuidof(IModelReader), &LIBID_ShapeShifter>
{
// Embedded types
public:

// Constructors/destructor
public:
	CModelReaderDXF();
	virtual ~CModelReaderDXF();

DECLARE_REGISTRY_RESOURCEID(IDR_MODEL_READER_DXF)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CModelReaderDXF)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IModelReader)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IModelReader interface

	STDMETHOD(Read)(/* [in] */ VARIANT varDataSource,
	                /* [in] */ IModel* pModel);

// Implementation
protected:
};

};  // namespace ShapeShifter

#endif // #ifndef __MODELREADERDXF_H__
