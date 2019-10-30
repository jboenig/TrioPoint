//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelWriterXML.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MODELWRITERXML_H__
#define __MODELWRITERXML_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelWriterXML

class ATL_NO_VTABLE CModelWriterXML : public CComObjectRootEx<CComSingleThreadModel>,
                                      public CComCoClass<CModelWriterXML, &CLSID_ModelWriterXML>,
                                      public IDispatchImpl<IModelWriter, &__uuidof(IModelWriter), &LIBID_ShapeShifter>
{
// Embedded types
public:

// Constructors/destructor
public:
	CModelWriterXML();
	virtual ~CModelWriterXML();

DECLARE_REGISTRY_RESOURCEID(IDR_MODEL_WRITER_XML)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CModelWriterXML)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IModelWriter)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	IXMLArchive* m_pArchive;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IModelWriter interface

	STDMETHOD(Write)(/* [in] */ VARIANT varDataSource,
	                 /* [in] */ IModel* pModel);
};

};  // namespace ShapeShifter

#endif // #ifndef __MODELWRITERXML_H__
