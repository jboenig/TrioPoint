//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelWriterXML.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "ModelWriterXML.h"
#include "Model.h"
#include "XMLArchive.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelWriterXML

CModelWriterXML::CModelWriterXML() :
	m_pArchive(NULL)
{
}

CModelWriterXML::~CModelWriterXML()
{
	if (m_pArchive != NULL)
	{
		m_pArchive->Release();
	}
}

HRESULT CModelWriterXML::FinalConstruct()
{
	return CXMLArchive::_CreatorClass::CreateInstance(NULL, IID_IXMLArchive, (LPVOID*) &m_pArchive);
}

STDMETHODIMP CModelWriterXML::Write(/* [in] */ VARIANT varDataSource,
                                    /* [in] */ IModel* pModel)
{
	HRESULT hr = S_OK;

	if (pModel == NULL)
	{
		return E_INVALIDARG;
	}

	_ASSERTE(m_pArchive != NULL);

	hr = m_pArchive->Clear();

	if (hr == S_OK)
	{
		hr = m_pArchive->WriteObject(pModel);

		if (hr == S_OK)
		{
			hr = m_pArchive->Save(varDataSource);
		}
	}

	return hr;
}

};  // namespace ShapeShifter
