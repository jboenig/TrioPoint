//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReaderVRML.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "ModelReaderVRML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReaderVRML

CModelReaderVRML::CModelReaderVRML()
{
}

CModelReaderVRML::~CModelReaderVRML()
{
}

HRESULT CModelReaderVRML::FinalConstruct()
{
	return S_OK;
}

STDMETHODIMP CModelReaderVRML::Read(/* [in] */ VARIANT varDataSource,
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
