//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReaderVRML.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MODELREADERVRML_H__
#define __MODELREADERVRML_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReaderVRML

class ATL_NO_VTABLE CModelReaderVRML : public CComObjectRootEx<CComSingleThreadModel>,
                                       public CComCoClass<CModelReaderVRML, &CLSID_ModelReaderVRML>,
                                       public IDispatchImpl<IModelReader, &__uuidof(IModelReader), &LIBID_ShapeShifter>
{
// Embedded types
public:

// Constructors/destructor
public:
	CModelReaderVRML();
	virtual ~CModelReaderVRML();

DECLARE_REGISTRY_RESOURCEID(IDR_MODEL_READER_VRML)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CModelReaderVRML)
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

#endif // #ifndef __MODELREADERVRML_H__
