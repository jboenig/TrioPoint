//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReaderXML.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "ModelReaderXML.h"
#include "Model.h"
#include "XMLArchive.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReaderXML

CModelReaderXML::CModelReaderXML() :
	m_pArchive(NULL)
{
}

CModelReaderXML::~CModelReaderXML()
{
	if (m_pArchive != NULL)
	{
		m_pArchive->Release();
	}
}

HRESULT CModelReaderXML::FinalConstruct()
{
	return CXMLArchive::_CreatorClass::CreateInstance(NULL, IID_IXMLArchive, (LPVOID*) &m_pArchive);
}

STDMETHODIMP CModelReaderXML::Read(/* [in] */ VARIANT varDataSource,
                                   /* [in] */ IModel* pModel)
{
	if (pModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	_ASSERTE(m_pArchive != NULL);

	hr = m_pArchive->Load(varDataSource);

	if (hr == S_OK)
	{
		hr = m_pArchive->GotoNode(_bstr_t("/") + _bstr_t(tag_Model));

		if (hr == S_OK)
		{
			IPersistXML* pObjPersist = NULL;
			hr = pModel->QueryInterface(IID_IPersistXML, (LPVOID*) &pObjPersist);

			if (SUCCEEDED(hr))
			{
				hr = pObjPersist->LoadXML(m_pArchive);
				pObjPersist->Release();
			}
		}
	}

	return hr;
}

};  // namespace ShapeShifter
