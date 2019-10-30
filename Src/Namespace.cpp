//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Namespace.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Namespace.h"
#include "XMLArchive.h"

namespace ShapeShifter {

CNamespace::CNamespace()
{
}

CNamespace::~CNamespace()
{
}

HRESULT CNamespace::FinalConstruct()
{
	return put_Name(tag_Namespace);
}

STDMETHODIMP CNamespace::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Namespace;
	return S_OK;
}

STDMETHODIMP CNamespace::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = E_FAIL;
	long nContext;
	hr = pArchive->GetContext(&nContext);
	if (hr == S_OK)
	{
		hr = ReadElement(pArchive, nContext, tag_Name, &CNamespace::put_Name);
		hr = ReadObjectCollection(pArchive, nContext, tag_Children, &CNamespace::PutChildUnk);
	}
	return hr;
}

STDMETHODIMP CNamespace::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = E_FAIL;
	long nContext;
	hr = pArchive->GetContext(&nContext);
	if (hr == S_OK)
	{
		hr = WriteElement(pArchive, nContext, tag_Name, &CNamespace::get_Name);
		hr = WriteObjectCollection(pArchive, nContext, tag_Children, &CNamespace::GetChildUnk);
	}
	return hr;
}

IUnknown* CNamespace::GetChildUnk(const long nIdx)
{
	if (nIdx < (long) m_children.size())
	{
		return static_cast<INode*>(m_children[nIdx]);
	}
	return NULL;
}

bool CNamespace::PutChildUnk(IUnknown* pUnk)
{
	bool bSuccess = false;
	INode* pNode = NULL;

	if (pUnk != NULL)
	{
		if (SUCCEEDED(pUnk->QueryInterface(IID_INode, (LPVOID*) &pNode)))
		{
			if (SUCCEEDED(AppendChild(pNode)))
			{
				bSuccess = true;
			}
			pNode->Release();
		}
	}

	return bSuccess;
}

};  // namespace ShapeShifter

