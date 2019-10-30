//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReaderXML.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MODELREADERXML_H__
#define __MODELREADERXML_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReaderXML

class ATL_NO_VTABLE CModelReaderXML : public CComObjectRootEx<CComSingleThreadModel>,
                                      public CComCoClass<CModelReaderXML, &CLSID_ModelReaderXML>,
                                      public IDispatchImpl<IModelReader, &__uuidof(IModelReader), &LIBID_ShapeShifter>
{
// Embedded types
public:

// Constructors/destructor
public:
	CModelReaderXML();
	virtual ~CModelReaderXML();

DECLARE_REGISTRY_RESOURCEID(IDR_MODEL_READER_XML)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CModelReaderXML)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IModelReader)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	IXMLArchive* m_pArchive;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IModelReader interface

	STDMETHOD(Read)(/* [in] */ VARIANT varDataSource,
	                /* [in] */ IModel* pModel);
};

};  // namespace ShapeShifter

#endif // #ifndef __MODELREADERXML_H__
