//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: XMLArchive.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include <d3d.h>
#include <comdef.h>
#include <tchar.h>

#include "XMLArchive.h"
#include "XMLTags.h"

namespace ShapeShifter {

static const LPOLESTR strXMLComment = L"ShapeShifter";
static const LPOLESTR strDefaultNamespace = L"urn:tripoint-shapeshifter1";
static const int MAX_NUM_ELEMENT_LEN = 120;

/////////////////////////////////////////////////////////////////////////
// CXMLArchive::Context

CXMLArchive::Context::Context(CXMLArchive* pArchive)
{
	m_bFree = false;

	m_pCurNode = pArchive->m_pCurNode;
	if (m_pCurNode != NULL)
	{
		m_pCurNode->AddRef();
	}
}

CXMLArchive::Context::Context(const Context& src)
{
	m_pCurNode = NULL;
	*this = src;
}

CXMLArchive::Context::~Context()
{
	if (m_pCurNode != NULL)
	{
		m_pCurNode->Release();
	}
}

CXMLArchive::Context& CXMLArchive::Context::operator=(const CXMLArchive::Context& src)
{
	Free();

	m_pCurNode = src.m_pCurNode;
	if (m_pCurNode != NULL)
	{
		m_pCurNode->AddRef();
	}

	m_bFree = src.m_bFree;

	return *this;
}

void CXMLArchive::Context::Free()
{
	if (m_pCurNode != NULL)
	{
		m_pCurNode->Release();
		m_pCurNode = NULL;
	}
	m_bFree = true;
}

/////////////////////////////////////////////////////////////////////////
// CXMLArchive

CXMLArchive::CXMLArchive() :
	m_pXMLDoc(NULL),
	m_pCurNode(NULL),
	m_strNamespace(NULL)
{
}

CXMLArchive::~CXMLArchive()
{
	ClearObjectCache();

	if (m_strNamespace != NULL)
	{
		::SysFreeString(m_strNamespace);
	}

	if (m_pCurNode != NULL)
	{
		m_pCurNode->Release();
	}

	if (m_pXMLDoc != NULL)
	{
		m_pXMLDoc->Release();
	}
}

HRESULT CXMLArchive::FinalConstruct()
{
	m_strNamespace = ::SysAllocString(strDefaultNamespace);
	return CreateDOMDocument();
}

STDMETHODIMP CXMLArchive::get_DOMDocument(/* [out, retval] */ IXMLDOMDocument** ppVal)
{
	if (ppVal != NULL)
	{
		*ppVal = m_pXMLDoc;
		if (*ppVal != NULL)
		{
			(*ppVal)->AddRef();
		}
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CXMLArchive::get_CurrentNode(/* [out, retval] */ IXMLDOMNode** ppVal)
{
	if (ppVal != NULL)
	{
		*ppVal = m_pCurNode;
		if (*ppVal != NULL)
		{
			(*ppVal)->AddRef();
		}
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CXMLArchive::putref_CurrentNode(/* [in] */ IXMLDOMNode* pNewVal)
{
	if (pNewVal != NULL)
	{
		pNewVal->AddRef();
	}

	if (m_pCurNode != NULL)
	{
		m_pCurNode->Release();
	}

	m_pCurNode = pNewVal;

	return S_OK;
}

STDMETHODIMP CXMLArchive::get_Namespace(/* [out, retval] */ BSTR* pVal)
{
	if (pVal != NULL)
	{
		*pVal = ::SysAllocString(m_strNamespace);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CXMLArchive::put_Namespace(/* [in] */ BSTR newVal)
{
	if (m_strNamespace != NULL)
	{
		::SysFreeString(m_strNamespace);
		m_strNamespace = NULL;
	}

	if (newVal != NULL)
	{
		m_strNamespace = ::SysAllocString(newVal);
	}

	return S_OK;
}

STDMETHODIMP CXMLArchive::Load(/* [in] */ VARIANT varSource)
{
	HRESULT hr = E_FAIL;
	VARIANT_BOOL bSuccess;

	if (m_pXMLDoc != NULL)
	{
		hr = m_pXMLDoc->load(varSource, &bSuccess);

		if (hr == S_OK && bSuccess)
		{
			IXMLDOMElement* pDocElement = NULL;
			hr = m_pXMLDoc->get_documentElement(&pDocElement);
			if (hr == S_OK)
			{
				putref_CurrentNode(pDocElement);
				pDocElement->Release();
			}
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::Save(/* [in] */ VARIANT varDestination)
{
	HRESULT hr = E_FAIL;

	if (m_pXMLDoc != NULL)
	{
		hr = m_pXMLDoc->save(varDestination);
	}

	return hr;
}

STDMETHODIMP CXMLArchive::Clear()
{
	if (m_pXMLDoc != NULL)
	{
		m_pXMLDoc->Release();
		m_pXMLDoc = NULL;
	}

	return CreateDOMDocument();
}

STDMETHODIMP CXMLArchive::CreateElement(/* [in] */ BSTR strTag)
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pNewNode = NULL;
	IXMLDOMNode* pParentNode = NULL;

	if (m_pXMLDoc == NULL)
	{
		return E_FAIL;
	}

	if (m_pCurNode != NULL)
	{
		pParentNode = m_pCurNode;
		pParentNode->AddRef();
	}
	else
	{
		pParentNode = m_pXMLDoc;
		pParentNode->AddRef();
	}

	if (pParentNode != NULL)
	{
		hr = m_pXMLDoc->createNode(_variant_t((short)NODE_ELEMENT), strTag, m_strNamespace, &pNewNode);

		if (SUCCEEDED(hr))
		{
			IXMLDOMNode* pResNode = NULL;

			hr = pParentNode->appendChild(pNewNode, &pResNode);

			if (SUCCEEDED(hr))
			{
				putref_CurrentNode(pResNode);
				pResNode->Release();
			}

			pNewNode->Release();
		}

		pParentNode->Release();
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GotoNode(/* [in] */ BSTR strXPath)
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pNode = NULL;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->selectSingleNode(strXPath, &pNode);

		if (hr == S_OK)
		{
			putref_CurrentNode(pNode);
			pNode->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GotoNodeByID(/* [in] */ BSTR strNodeID)
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pNode = NULL;

	if (m_pXMLDoc != NULL)
	{
		hr = m_pXMLDoc->nodeFromID(strNodeID, &pNode);

		if (hr == S_OK)
		{
			putref_CurrentNode(pNode);
			pNode->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GotoFirstChild()
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pChild = NULL;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->get_firstChild(&pChild);

		if (hr == S_OK)
		{
			hr = putref_CurrentNode(pChild);
			pChild->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GotoNextSibling()
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pSibling = NULL;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->get_nextSibling(&pSibling);

		if (hr == S_OK)
		{
			hr = putref_CurrentNode(pSibling);
			pSibling->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GotoPreviousSibling()
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pSibling = NULL;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->get_previousSibling(&pSibling);

		if (hr == S_OK)
		{
			hr = putref_CurrentNode(pSibling);
			pSibling->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GotoParent()
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pParent = NULL;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->get_parentNode(&pParent);

		if (hr == S_OK)
		{
			hr = putref_CurrentNode(pParent);
			pParent->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::CompareCurrentTag(/* [in] */ BSTR strTag)
{
	HRESULT hr = E_FAIL;
	BSTR strCurTag = NULL;

	if (m_pCurNode != NULL)
	{
		if (m_pCurNode->get_baseName(&strCurTag) == S_OK)
		{
			if (_bstr_t(strTag) == _bstr_t(strCurTag))
			{
				hr = S_OK;
			}
			else
			{
				hr = S_FALSE;
			}

			::SysFreeString(strCurTag);
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::GetContext(/* [out,retval] */ long* pContextHandle)
{
	Context curContext(this);
	bool bFreeSlotFound = false;

	for (ULONG nIdx = 0; nIdx < m_contextTable.size() && !bFreeSlotFound; nIdx++)
	{
		if (m_contextTable[nIdx].m_bFree)
		{
			m_contextTable[nIdx] = curContext;
			*pContextHandle = nIdx;
			bFreeSlotFound = true;
		}
	}

	if (!bFreeSlotFound)
	{
		m_contextTable.push_back(curContext);
		*pContextHandle = m_contextTable.size() - 1;
	}

	return S_OK;
}

STDMETHODIMP CXMLArchive::SetContext(/* [in] */ long nContextHandle)
{
	if ((ULONG) nContextHandle < m_contextTable.size() && !m_contextTable[nContextHandle].m_bFree)
	{
		if (m_pCurNode != NULL)
		{
			m_pCurNode->Release();
		}

		m_pCurNode = m_contextTable[nContextHandle].m_pCurNode;

		if (m_pCurNode != NULL)
		{
			m_pCurNode->AddRef();
		}

		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP CXMLArchive::FreeContext(/* [in] */ long nContextHandle)
{
	if ((ULONG) nContextHandle < m_contextTable.size())
	{
		m_contextTable[nContextHandle].Free();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CXMLArchive::ReadElement(/* [in] */ VARTYPE vtDataType, /* [out,retval] */ VARIANT* pValue)
{
	USES_CONVERSION;
	HRESULT hr = E_FAIL;
	BSTR strVal;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->get_text(&strVal);

		if (SUCCEEDED(hr))
		{
			pValue->vt = vtDataType;

			switch (vtDataType)
			{
			case VT_BSTR:
			case VT_EMPTY:
				pValue->bstrVal = ::SysAllocString(strVal);
				break;

			case VT_I2:
				pValue->iVal = static_cast<short>(_ttoi(OLE2T(strVal)));
				break;

			case VT_I4:
				pValue->lVal = _ttoi(OLE2T(strVal));
				break;

			case VT_R4:
				pValue->fltVal = static_cast<float>(_tcstod(OLE2T(strVal), NULL));
				break;

			case VT_R8:
				pValue->dblVal = _tcstod(OLE2T(strVal), NULL);
				break;

			default:
				pValue->vt = VT_EMPTY;
				break;
			}

			::SysFreeString(strVal);
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::WriteElement(/* [in] */ VARIANT varValue, /* [in] */ BSTR strFormat)
{
	USES_CONVERSION;

	HRESULT hr = E_FAIL;
	IXMLDOMElement* pElement = NULL;

	if (m_pCurNode != NULL)
	{
		BSTR strValue = FormatValue(varValue, strFormat);

		if (strValue != NULL && ::SysStringLen(strValue) > 0)
		{
			hr = m_pCurNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pElement);

			if (SUCCEEDED(hr))
			{
				hr = pElement->put_text(strValue);
				pElement->Release();
			}

			::SysFreeString(strValue);
		}
		else
		{
			hr = S_FALSE;
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::ReadAttribute(/* [in] */ BSTR strTag, /* [out,retval] */ VARIANT* pValue)
{
	HRESULT hr = E_FAIL;
	IXMLDOMElement* pElement;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pElement);

		if (SUCCEEDED(hr))
		{
			hr = pElement->getAttribute(strTag, pValue);
			pElement->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::WriteAttribute(/* [in] */ BSTR strTag, /* [in] */ VARIANT varValue)
{
	HRESULT hr = E_FAIL;
	IXMLDOMElement* pElement;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pElement);

		if (SUCCEEDED(hr))
		{
			hr = pElement->setAttribute(strTag, varValue);
			pElement->Release();
		}
	}

	return hr;
}

STDMETHODIMP CXMLArchive::ReadObject(/* [out,retval] */ IUnknown** ppObj)
{
	HRESULT hr = E_FAIL;

	// First, try to read as an object reference. If that fails,
	// then read the object by value.
	if (ReadObjectReference(ppObj) == S_OK)
	{
		return S_OK;
	}
	else
	{
		hr = ReadObjectValue(ppObj);
	}

	return hr;
}

STDMETHODIMP CXMLArchive::WriteObject(/* [in] */ IUnknown* pObj)
{
	if (ObjectIsCached(pObj))
	{
		return WriteObjectReference(pObj);
	}
	return WriteObjectValue(pObj);
}

STDMETHODIMP CXMLArchive::ReadObjectValue(/* [out,retval] */ IUnknown** ppObj)
{
	USES_CONVERSION;
	HRESULT hr = E_FAIL;
	CLSID clsid;
	VARIANT varClassID;
	IXMLDOMElement* pObjElement = NULL;
	IPersistXML* pObjPersist = NULL;
	VARIANT varObjectID;
	_bstr_t strObjectID;

	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pObjElement);

		if (SUCCEEDED(hr))
		{
			// Get the object ID attribute
			if (pObjElement->getAttribute(tag_ID, &varObjectID) == S_OK)
			{
				strObjectID = varObjectID.bstrVal;
				// Lookup the object in the cache
				pObjPersist = LookupObject(strObjectID);
				::VariantClear(&varObjectID);
			}

			if (pObjPersist == NULL)
			{
				// Object not found in cache. Create and load it now.

				hr = pObjElement->getAttribute(tag_ClassID, &varClassID);

				if (hr == S_OK)
				{
					if (varClassID.vt == VT_BSTR)
					{
						hr = ::CLSIDFromString(varClassID.bstrVal, &clsid);
					}

					::VariantClear(&varClassID);
				}

				if (SUCCEEDED(hr))
				{
					// Create the object
					hr = ::CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IPersistXML, (LPVOID*) &pObjPersist);

					if (SUCCEEDED(hr))
					{
						// Put object into the cache
						CacheObject(strObjectID, pObjPersist);
						// Load the object from the XML DOM
						hr = pObjPersist->LoadXML(this);
					}
				}
			}

			pObjElement->Release();
		}
	}

	if (pObjPersist != NULL)
	{
		// Return IUnknown to caller
		if (ppObj != NULL)
		{
			hr = pObjPersist->QueryInterface(IID_IUnknown, (LPVOID*) ppObj);
		}
		pObjPersist->Release();
	}

	return hr;
}

STDMETHODIMP CXMLArchive::WriteObjectValue(/* [in] */ IUnknown* pObj)
{
	HRESULT hr = E_FAIL;
	HRESULT hrObjID = S_FALSE;
	IPersistXML* pPersist = NULL;
	IXMLDOMNode* pObjNode = NULL;
	CLSID clsid;
	BSTR strTag = NULL;
	BSTR strObjectID = NULL;
	long nContextHandle;

	if (pObj != NULL && SUCCEEDED(pObj->QueryInterface(IID_IPersistXML, (LPVOID*) &pPersist)))
	{
		// Get tag from the object
		hr = pPersist->GetTag(&strTag);

		if (SUCCEEDED(hr))
		{
			// Get class ID from the object
			hr = pPersist->GetClassID(&clsid);
		}

		if (SUCCEEDED(hr))
		{
			// Get the object ID from the object
			hrObjID = pPersist->GetObjectID(&strObjectID);

			if (hrObjID == S_OK)
			{
				// Check for duplicate. If this object is in the cache, then
				// it has already been written to the archive. Set error code
				// and exit if the object is already in the archive.
				IPersistXML* pCachedObj = LookupObject(_bstr_t(strObjectID));
				if (pCachedObj != NULL)
				{
					hr = E_FAIL;
					pCachedObj->Release();
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			// Create a node in the document to hold the object
			hr = CreateObjectNode(strTag, clsid, strObjectID, &pObjNode);

			if (SUCCEEDED(hr))
			{
				// Save current position
				GetContext(&nContextHandle);

				// Make the new node the current node
				putref_CurrentNode(pObjNode);

				// Have the object save its contents to the node
				hr = pPersist->SaveXML(this);

				// Cache the object to prevent it from being written multiple times
				if (SUCCEEDED(hr) && hrObjID == S_OK)
				{
					CacheObject(_bstr_t(strObjectID), pPersist);
				}

				// Restore previous position
				SetContext(nContextHandle);
				FreeContext(nContextHandle);

				pObjNode->Release();
			}

			::SysFreeString(strTag);
		}

		pPersist->Release();
	}

	return hr;
}

STDMETHODIMP CXMLArchive::ReadObjectReference(/* [out,retval] */ IUnknown** ppObj)
{
	HRESULT hr = E_FAIL;
	IXMLDOMElement* pElement = NULL;
	VARIANT varObjectID;

	::VariantClear(&varObjectID);

	// Get the object address from the current object reference node
	if (m_pCurNode != NULL)
	{
		hr = m_pCurNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pElement);

		if (SUCCEEDED(hr))
		{
			hr = pElement->getAttribute(tag_IDREF, &varObjectID);
			pElement->Release();
		}
	}

	if (hr == S_OK)
	{
		// Address successfully retrieved from current node. First,
		// check to see if this object has already been loaded. If
		// the object is already in the cache, then just return a
		// pointer to it. Otherwise, navigate to the given address
		// and load the object.

		// Save current position
		long nContextHandle;
		GetContext(&nContextHandle);

		_bstr_t strPathExpr;
		strPathExpr = L"//*[@ID=\"";
		strPathExpr += _bstr_t(varObjectID);
		strPathExpr += L"\"]";

		hr = GotoNode(strPathExpr);

		if (hr == S_OK)
		{
			hr = ReadObjectValue(ppObj);
		}

		// Restore previous position
		SetContext(nContextHandle);
		FreeContext(nContextHandle);
	}

	return hr;
}

STDMETHODIMP CXMLArchive::WriteObjectReference(/* [in] */ IUnknown* pObj)
{
	HRESULT hr = E_FAIL;
	CComQIPtr<IPersistXML> spPersist(pObj);
	BSTR strTag = NULL;
	BSTR strObjectID = NULL;
	IXMLDOMNode* pNewNode = NULL;
	IXMLDOMNode* pParentNode = NULL;
	IXMLDOMElement* pElement = NULL;
	IXMLDOMNode* pObjNode = NULL;

	if (m_pXMLDoc == NULL)
	{
		return E_FAIL;
	}

	if (m_pCurNode != NULL)
	{
		pParentNode = m_pCurNode;
		pParentNode->AddRef();
	}
	else
	{
		pParentNode = m_pXMLDoc;
		pParentNode->AddRef();
	}

	if (pParentNode != NULL)
	{
		if (spPersist != NULL)
		{
			// Get tag from the object
			hr = spPersist->GetTag(&strTag);

			if (hr == S_OK)
			{
				hr = spPersist->GetObjectID(&strObjectID);
			}

			if (hr == S_OK)
			{
				hr = m_pXMLDoc->createNode(_variant_t((short)NODE_ELEMENT),
				                           strTag,
				                           m_strNamespace,
				                           &pNewNode);

				if (SUCCEEDED(hr))
				{
					hr = pNewNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pElement);

					if (SUCCEEDED(hr))
					{
						hr = pParentNode->appendChild(pElement, &pObjNode);

						if (SUCCEEDED(hr) && pObjNode != NULL)
						{
							pObjNode->Release();
						}

						if (SUCCEEDED(hr))
						{
							hr = pElement->setAttribute(tag_IDREF, _variant_t(strObjectID));
						}

						pElement->Release();
					}

					pNewNode->Release();
				}

				::SysFreeString(strObjectID);
			}
		}

		pParentNode->Release();
	}

	return hr;
}

HRESULT CXMLArchive::CreateDOMDocument()
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pNewNode = NULL;

	if (m_pXMLDoc == NULL)
	{
		hr = ::CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		                        IID_IXMLDOMDocument, (LPVOID*) &m_pXMLDoc);

		if (SUCCEEDED(hr))
		{
			// Add XML version number to document
			IXMLDOMProcessingInstruction* pXMLVersionNode = NULL;
			hr = m_pXMLDoc->createProcessingInstruction(L"xml", L"version='1.0'", &pXMLVersionNode);
			if (SUCCEEDED(hr))
			{
				hr = m_pXMLDoc->appendChild(pXMLVersionNode, &pNewNode);
				if (SUCCEEDED(hr))
				{
					pNewNode->Release();
				}
				pXMLVersionNode->Release();
			}
		}

		if (SUCCEEDED(hr))
		{
			// Add comment
			IXMLDOMComment* pComment = NULL;
			hr = m_pXMLDoc->createComment(strXMLComment, &pComment);
			if (SUCCEEDED(hr))
			{
				m_pXMLDoc->appendChild(pComment, NULL);
				pComment->Release();
			}
		}
	}

	return hr;
}

HRESULT CXMLArchive::CreateObjectNode(BSTR strTag,  REFCLSID clsid, BSTR strObjectID, IXMLDOMNode** ppObjNode)
{
	HRESULT hr = E_FAIL;
	IXMLDOMNode* pParentNode = NULL;
	IXMLDOMNode* pNewNode = NULL;
	IXMLDOMElement* pElement = NULL;

	if (m_pXMLDoc == NULL)
	{
		return E_FAIL;
	}

	if (m_pCurNode != NULL)
	{
		pParentNode = m_pCurNode;
		pParentNode->AddRef();
	}
	else
	{
		pParentNode = m_pXMLDoc;
		pParentNode->AddRef();
	}

	if (pParentNode != NULL)
	{
		hr = m_pXMLDoc->createNode(_variant_t((short)NODE_ELEMENT), strTag, m_strNamespace, &pNewNode);

		if (SUCCEEDED(hr))
		{
			hr = pNewNode->QueryInterface(IID_IXMLDOMElement, (LPVOID*) &pElement);

			if (SUCCEEDED(hr))
			{
				// Set the class ID attribute
				LPOLESTR pStrClsID;
				if (SUCCEEDED(StringFromCLSID(clsid, &pStrClsID)))
				{
					hr = pElement->setAttribute(tag_ClassID, _variant_t(pStrClsID));
					::CoTaskMemFree(pStrClsID);
				}

				if (SUCCEEDED(hr))
				{
					hr = pParentNode->appendChild(pElement, ppObjNode);

					if (SUCCEEDED(hr) && strObjectID != NULL && ::SysStringLen(strObjectID) > 0)
					{
						hr = pElement->setAttribute(tag_ID, _variant_t(strObjectID));
					}
				}

				pElement->Release();
			}

			pNewNode->Release();
		}

		pParentNode->Release();
	}

	return hr;
}

BSTR CXMLArchive::FormatValue(const VARIANT& val, BSTR strFormat)
{
	USES_CONVERSION;
	BSTR strValue = NULL;
	TCHAR* cvtbuf = NULL;
	UINT nFormatLen = 0;
	UINT nCvtBufSize = 0;

	// Get length of format string
	if (strFormat != NULL)
	{
		nFormatLen = ::SysStringLen(strFormat);
	}

	// Calculate size required for conversion buffer
	if (val.vt == VT_BSTR)
	{
		// Conversion buffer only required for strings if
		// format string supplied
		if (nFormatLen > 0)
		{
			nCvtBufSize = nFormatLen + ::SysStringLen(val.bstrVal) + 1;
		}
	}
	else if (val.vt == VT_BOOL)
	{
		nCvtBufSize = 0;
	}
	else
	{
		// Calculate size large enough to hold most numbers
		// plus format string, if one is provided.
		nCvtBufSize = nFormatLen + 40;
	}

	// Allocate conversion buffer if required
	if (nCvtBufSize > 0)
	{
		cvtbuf = new TCHAR[nCvtBufSize];
	}

	// Format value into conversion buffer
	switch (val.vt)
	{
	case VT_BSTR:
		if (nFormatLen > 0)
		{
			_stprintf(cvtbuf, OLE2T(strFormat), OLE2T(val.bstrVal));
		}
		else
		{
			// Copy string without formatting
			strValue = ::SysAllocString(val.bstrVal);
		}
		break;

	case VT_I2:
		if (nFormatLen > 0)
		{
			_stprintf(cvtbuf, OLE2T(strFormat), val.iVal);
		}
		else
		{
			_stprintf(cvtbuf, _T("%d"), val.iVal);
		}
		break;

	case VT_I4:
		if (nFormatLen > 0)
		{
			_stprintf(cvtbuf, OLE2T(strFormat), val.lVal);
		}
		else
		{
			_stprintf(cvtbuf, _T("%d"), val.lVal);
		}
		break;

	case VT_R4:
		if (nFormatLen > 0)
		{
			_stprintf(cvtbuf, OLE2T(strFormat), val.fltVal);
		}
		else
		{
			_stprintf(cvtbuf, _T("%f"), val.fltVal);
		}
		break;

	case VT_R8:
		if (nFormatLen > 0)
		{
			_stprintf(cvtbuf, OLE2T(strFormat), val.dblVal);
		}
		else
		{
			_stprintf(cvtbuf, _T("%f"), val.dblVal);
		}
		break;
	}

	if (cvtbuf)
	{
		strValue = ::SysAllocString(T2OLE(cvtbuf));
		delete[] cvtbuf;
	}

	return strValue;
}

void CXMLArchive::ClearObjectCache()
{
	_ObjectMap::iterator iObj = m_objCache.begin();
	while (iObj != m_objCache.end())
	{
		IPersistXML* pObjPersist = iObj->second;
		if (pObjPersist != NULL)
		{
			pObjPersist->Release();
		}
		iObj++;
	}
	m_objCache.clear();
}

IPersistXML* CXMLArchive::LookupObject(const _bstr_t& strID) const
{
	IPersistXML* pObjPersist = NULL;

	_ObjectMap::const_iterator iObj = m_objCache.find(strID);
	if (iObj != m_objCache.end())
	{
		pObjPersist = (*iObj).second;
		if (pObjPersist != NULL)
		{
			pObjPersist->AddRef();
		}
	}

	return pObjPersist;
}

bool CXMLArchive::ObjectIsCached(IUnknown* pObj) const
{
	bool bFound = false;
	IPersistXML* pPersist = NULL;
	BSTR strObjectID = NULL;

	if (pObj != NULL && SUCCEEDED(pObj->QueryInterface(IID_IPersistXML, (LPVOID*) &pPersist)))
	{
		if (pPersist->GetObjectID(&strObjectID) == S_OK)
		{
			IPersistXML* pCachedObj = LookupObject(_bstr_t(strObjectID));

			if (pCachedObj != NULL)
			{
				bFound = true;
				pCachedObj->Release();
			}

			::SysFreeString(strObjectID);
		}

		pPersist->Release();
	}

	return bFound;
}

bool CXMLArchive::CacheObject(const _bstr_t& strID, IPersistXML* pObjPersist)
{
	if (pObjPersist != NULL && strID.length() > 0)
	{
		pObjPersist->AddRef();
		m_objCache[strID] = pObjPersist;
		return true;
	}
	return false;
}

};  // namespace ShapeShifter
