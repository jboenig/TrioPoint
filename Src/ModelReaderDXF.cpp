//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReaderDXF.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "ModelReaderDXF.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReaderDXF

CModelReaderDXF::CModelReaderDXF()
{
}

CModelReaderDXF::~CModelReaderDXF()
{
}

HRESULT CModelReaderDXF::FinalConstruct()
{
	return S_OK;
}

STDMETHODIMP CModelReaderDXF::Read(/* [in] */ VARIANT varDataSource,
                                   /* [in] */ IModel* pModel)
{
	if (pModel == NULL)
	{
		return E_POINTER;
	}

	varDataSource;

	return E_NOTIMPL;
}

};  // namespace ShapeShifter
