//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: PersistXML.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __PERSISTXML_H__
#define __PERSISTXML_H__

#pragma once

#include "ShapeShifter.h"
#include "XMLTags.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IPersistXMLImplBase

class IPersistXMLImplBase : public IPersistXML
{
public:
	STDMETHOD(GetObjectID)(/* [out,retval] */ BSTR* strObjectID)
	{
		HRESULT hr = S_FALSE;
		IPersistXML* pPersistXML = static_cast<IPersistXML*>(this);
		INode* pNode = NULL;

		HRESULT hrNodeQI = pPersistXML->QueryInterface(IID_INode, (LPVOID*) &pNode);
		if (SUCCEEDED(hrNodeQI))
		{
			hr = pNode->get_FullName(strObjectID);

			if (hr == S_OK)
			{
				if (::SysStringLen(*strObjectID) == 0)
				{
					hr = S_FALSE;
				}
			}
		}

		return hr;
	}

	HRESULT ReadElement(IXMLArchive* pArchive, const long nContext, BSTR strXPath, VARTYPE vtDataType, VARIANT* pValue)
	{
		HRESULT hr = pArchive->GotoNode(strXPath);

		if (hr == S_OK)
		{
			hr = pArchive->ReadElement(vtDataType, pValue);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	HRESULT ReadVector(IXMLArchive* pArchive, VALUE3D& x, VALUE3D& y, VALUE3D& z)
	{
		HRESULT hr = E_FAIL;
		VARIANT val;
		long nContext;

		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = ReadElement(pArchive, nContext, tag_X, VT_R4, &val);

			if (SUCCEEDED(hr))
			{
				x = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Y, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				y = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Z, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				z = val.fltVal;
				hr = S_OK;
			}

			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	HRESULT ReadVector(IXMLArchive* pArchive, const long nContext, BSTR strXPath, VALUE3D& x, VALUE3D& y, VALUE3D& z)
	{
		HRESULT hr = pArchive->GotoNode(strXPath);

		if (SUCCEEDED(hr))
		{
			hr = ReadVector(pArchive, x, y, z);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	template <typename vertex_type>
	HRESULT ReadVertex(IXMLArchive* pArchive, VertexIndex& vertIdx, vertex_type& v)
	{
		HRESULT hr = E_FAIL;
		VARIANT val;
		long nContext;

		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = pArchive->ReadAttribute(tag_Index, &val);

			if (SUCCEEDED(hr))
			{
				vertIdx = val.lVal;
				hr = ReadElement(pArchive, nContext, tag_X, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.x = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Y, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.y = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Z, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.z = val.fltVal;
				hr = S_OK;
			}

			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	template <>
	HRESULT ReadVertex(IXMLArchive* pArchive, VertexIndex& vertIdx, VERTEX3D& v)
	{
		HRESULT hr = E_FAIL;
		VARIANT val;
		long nContext;

		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = pArchive->ReadAttribute(tag_Index, &val);

			if (SUCCEEDED(hr))
			{
				vertIdx = val.lVal;
				hr = ReadElement(pArchive, nContext, tag_X, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.x = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Y, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.y = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Z, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.z = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_NX, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.nx = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_NY, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.ny = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_NZ, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.nz = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_TU, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.tu = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_TV, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				v.tv = val.fltVal;
				hr = S_OK;
			}

			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	HRESULT ReadColor(IXMLArchive* pArchive, D3DCOLORVALUE& clr)
	{
		HRESULT hr = E_FAIL;
		VARIANT val;
		long nContext;

		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = ReadElement(pArchive, nContext, tag_Red, VT_R4, &val);

			if (SUCCEEDED(hr))
			{
				clr.r = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Green, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				clr.g = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Blue, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				clr.b = val.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Alpha, VT_R4, &val);
			}

			if (SUCCEEDED(hr))
			{
				clr.a = val.fltVal;
				hr = S_OK;
			}

			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	HRESULT ReadColor(IXMLArchive* pArchive, const long nContext, BSTR strXPath, D3DCOLORVALUE& clr)
	{
		HRESULT hr = pArchive->GotoNode(strXPath);

		if (SUCCEEDED(hr))
		{
			hr = ReadColor(pArchive, clr);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	HRESULT ReadTransformationMatrix(IXMLArchive* pArchive, D3DMATRIX& mat)
	{
		VARIANT var;
		long nContext;

		HRESULT hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = ReadElement(pArchive, nContext, tag_Matrix_1_1, VT_R4, &var);

			if (SUCCEEDED(hr))
			{
				mat._11 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_1_2, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._12 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_1_3, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._13 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_1_4, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._14 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_2_1, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._21 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_2_2, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._22 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_2_3, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._23 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_2_4, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._24 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_3_1, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._31 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_3_2, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._32 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_3_3, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._33 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_3_4, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._34 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_4_1, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._41 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_4_2, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._42 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_4_3, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._43 = var.fltVal;
				hr = ReadElement(pArchive, nContext, tag_Matrix_4_4, VT_R4, &var);
			}
			if (SUCCEEDED(hr))
			{
				mat._44 = var.fltVal;
				hr = S_OK;
			}

			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	HRESULT ReadTransformationMatrix(IXMLArchive* pArchive, const long nContext, BSTR strXPath, D3DMATRIX& mat)
	{
		HRESULT hr = pArchive->GotoNode(strXPath);

		if (SUCCEEDED(hr))
		{
			hr = ReadTransformationMatrix(pArchive, mat);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	HRESULT ReadObject(IXMLArchive* pArchive, BSTR strTag, IUnknown** ppObjUnk)
	{
		HRESULT hr = S_OK;

		long nContext;
		pArchive->GetContext(&nContext);

		_bstr_t strNodePath(strTag);

		if (strNodePath.length() > 0)
		{
			hr = pArchive->GotoNode(strNodePath);
		}

		if (hr == S_OK)
		{
			hr = pArchive->ReadObject(ppObjUnk);
		}

		SetAndFreeContext(pArchive, nContext);

		return hr;
	}

	HRESULT ReadObjectReference(IXMLArchive* pArchive, BSTR strTag, IUnknown** ppObjUnk)
	{
		HRESULT hr = S_OK;

		long nContext;
		pArchive->GetContext(&nContext);

		_bstr_t strNodePath(strTag);

		if (strNodePath.length() > 0)
		{
			hr = pArchive->GotoNode(strNodePath);
		}

		if (hr == S_OK)
		{
			hr = pArchive->ReadObjectReference(ppObjUnk);
		}

		SetAndFreeContext(pArchive, nContext);

		return hr;
	}

	HRESULT WriteElement(IXMLArchive* pArchive, const long nContext, BSTR strTag, VARIANT varValue, BSTR strFormat)
	{
		HRESULT hr = E_FAIL;

		if (VariantIsEmpty(varValue))
		{
			hr = S_FALSE;
		}
		else
		{
			hr = pArchive->CreateElement(strTag);

			if (hr == S_OK)
			{
				hr = pArchive->WriteElement(varValue, strFormat);
				pArchive->SetContext(nContext);
			}
		}

		return hr;
	}

	HRESULT WriteVector(IXMLArchive* pArchive, const VALUE3D& x, const VALUE3D& y, const VALUE3D& z)
	{
		HRESULT hr = E_FAIL;
		long nContext;

		hr = pArchive->GetContext(&nContext);

		if (SUCCEEDED(hr))
		{
			hr = pArchive->CreateElement(tag_X);

			if (SUCCEEDED(hr))
			{
				hr = pArchive->WriteElement(_variant_t(x), NULL);
				pArchive->SetContext(nContext);
			}

			if (SUCCEEDED(hr))
			{
				hr = pArchive->CreateElement(tag_Y);

				if (SUCCEEDED(hr))
				{
					hr = pArchive->WriteElement(_variant_t(y), NULL);
					pArchive->SetContext(nContext);
				}
			}

			if (SUCCEEDED(hr))
			{
				hr = pArchive->CreateElement(tag_Z);

				if (SUCCEEDED(hr))
				{
					hr = pArchive->WriteElement(_variant_t(z), NULL);
				}
			}

			SetAndFreeContext(pArchive, nContext);
		}

		return hr;
	}

	HRESULT WriteVector(IXMLArchive* pArchive, const long nContext, BSTR strTag, const VALUE3D& x, const VALUE3D& y, const VALUE3D& z)
	{
		HRESULT hr = pArchive->CreateElement(strTag);

		if (SUCCEEDED(hr))
		{
			hr = WriteVector(pArchive, x, y, z);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	template <typename vertex_type>
	HRESULT WriteVertex(IXMLArchive* pArchive, const VertexIndex vertIdx, const vertex_type& v)
	{
		HRESULT hr;

		long nContext;
		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = pArchive->WriteAttribute(tag_Index, _variant_t(vertIdx));

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_X, _variant_t(v.x), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Y, _variant_t(v.y), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Z, _variant_t(v.z), NULL);
			}

			pArchive->SetContext(nContext);
			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	template <>
	HRESULT WriteVertex(IXMLArchive* pArchive, const VertexIndex vertIdx, const VERTEX3D& v)
	{
		HRESULT hr;

		long nContext;
		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = pArchive->WriteAttribute(tag_Index, _variant_t(vertIdx));

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_X, _variant_t(v.x), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Y, _variant_t(v.y), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Z, _variant_t(v.z), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_NX, _variant_t(v.nx), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_NY, _variant_t(v.ny), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_NZ, _variant_t(v.nz), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_TU, _variant_t(v.tu), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_TV, _variant_t(v.tv), NULL);
			}

			pArchive->SetContext(nContext);
			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	HRESULT WriteColor(IXMLArchive* pArchive, const D3DCOLORVALUE& clr)
	{
		HRESULT hr;

		long nContext;
		hr = pArchive->GetContext(&nContext);

		if (hr == S_OK)
		{
			hr = WriteElement(pArchive, nContext, tag_Red, _variant_t(clr.r), NULL);

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Green, _variant_t(clr.g), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Blue, _variant_t(clr.b), NULL);
			}

			if (SUCCEEDED(hr))
			{
				hr = WriteElement(pArchive, nContext, tag_Alpha, _variant_t(clr.a), NULL);
			}

			pArchive->SetContext(nContext);
			pArchive->FreeContext(nContext);
		}

		return hr;
	}

	HRESULT WriteColor(IXMLArchive* pArchive, const long nContext, BSTR strTag, const D3DCOLORVALUE& clr)
	{
		HRESULT hr = pArchive->CreateElement(strTag);

		if (SUCCEEDED(hr))
		{
			hr = WriteColor(pArchive, clr);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	HRESULT WriteTransformationMatrix(IXMLArchive* pArchive, const D3DMATRIX& mat)
	{
		HRESULT hr = S_OK;

		long nContext;
		hr = pArchive->GetContext(&nContext);

		WriteElement(pArchive, nContext, L"_1_1", _variant_t(mat._11), NULL);
		WriteElement(pArchive, nContext, L"_1_2", _variant_t(mat._12), NULL);
		WriteElement(pArchive, nContext, L"_1_3", _variant_t(mat._13), NULL);
		WriteElement(pArchive, nContext, L"_1_4", _variant_t(mat._14), NULL);
		WriteElement(pArchive, nContext, L"_2_1", _variant_t(mat._21), NULL);
		WriteElement(pArchive, nContext, L"_2_2", _variant_t(mat._22), NULL);
		WriteElement(pArchive, nContext, L"_2_3", _variant_t(mat._23), NULL);
		WriteElement(pArchive, nContext, L"_2_4", _variant_t(mat._24), NULL);
		WriteElement(pArchive, nContext, L"_3_1", _variant_t(mat._31), NULL);
		WriteElement(pArchive, nContext, L"_3_2", _variant_t(mat._32), NULL);
		WriteElement(pArchive, nContext, L"_3_3", _variant_t(mat._33), NULL);
		WriteElement(pArchive, nContext, L"_3_4", _variant_t(mat._34), NULL);
		WriteElement(pArchive, nContext, L"_4_1", _variant_t(mat._41), NULL);
		WriteElement(pArchive, nContext, L"_4_2", _variant_t(mat._42), NULL);
		WriteElement(pArchive, nContext, L"_4_3", _variant_t(mat._43), NULL);
		WriteElement(pArchive, nContext, L"_4_4", _variant_t(mat._44), NULL);

		pArchive->SetContext(nContext);
		pArchive->FreeContext(nContext);

		return hr;
	}

	HRESULT SetAndFreeContext(IXMLArchive* pArchive, const long nContext)
	{
		HRESULT hr = pArchive->SetContext(nContext);

		if (hr == S_OK)
		{
			hr = pArchive->FreeContext(nContext);
		}

		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////
// IPersistXMLImpl

template <class T>
class IPersistXMLImpl : public IPersistXMLImplBase
{
public:
	HRESULT ReadElement(IXMLArchive* pArchive, const long nContext, BSTR strXPath, VARTYPE vtDataType, VARIANT* pValue)
	{
		return IPersistXMLImplBase::ReadElement(pArchive, nContext, strXPath, vtDataType, pValue);
	}

	HRESULT ReadElement(IXMLArchive* pArchive, const long nContext, BSTR strXPath, HRESULT (__stdcall T::*ACCESSOR)(float))
	{
		HRESULT hr;
		VARIANT val;
		hr = ReadElement(pArchive, nContext, strXPath, VT_R4, &val);
		if (SUCCEEDED(hr))
		{
			hr = ((T*)this->*ACCESSOR)(val.fltVal);
			::VariantClear(&val);
		}
		return hr;
	}

	HRESULT ReadElement(IXMLArchive* pArchive, const long nContext, BSTR strXPath, HRESULT (__stdcall T::*ACCESSOR)(BSTR))
	{
		HRESULT hr;
		VARIANT val;
		hr = ReadElement(pArchive, nContext, strXPath, VT_BSTR, &val);
		if (hr == S_OK)
		{
			hr = ((T*)this->*ACCESSOR)(val.bstrVal);
			::VariantClear(&val);
		}
		return hr;
	}

	HRESULT ReadObjectCollection(IXMLArchive* pArchive, bool (__stdcall T::*ACCESSOR)(IUnknown*))
	{
		HRESULT hr = S_OK;
		IUnknown* pObjUnk = NULL;
		bool bContinue = true;

		HRESULT hrLoop = pArchive->GotoFirstChild();

		while (bContinue && hrLoop == S_OK)
		{
			if (pArchive->ReadObject(&pObjUnk) == S_OK)
			{
				bContinue = ((T*)this->*ACCESSOR)(pObjUnk);
				pObjUnk->Release();
			}

			if (bContinue)
			{
				hrLoop = pArchive->GotoNextSibling();
			}
		}

		return hr;
	}

	HRESULT ReadObjectCollection(IXMLArchive* pArchive, const long nContext, BSTR strXPath, bool (__stdcall T::*ACCESSOR)(IUnknown*))
	{
		HRESULT hr = pArchive->GotoNode(strXPath);

		if (SUCCEEDED(hr))
		{
			hr = ReadObjectCollection(pArchive, ACCESSOR);
			pArchive->SetContext(nContext);
		}

		return hr;
	}

	HRESULT WriteElement(IXMLArchive* pArchive, const long nContext, BSTR strTag, VARIANT varValue, BSTR strFormat)
	{
		return IPersistXMLImplBase::WriteElement(pArchive, nContext, strTag, varValue, strFormat);
	}

	HRESULT WriteElement(IXMLArchive* pArchive, const long nContext, BSTR strTag, HRESULT (__stdcall T::*ACCESSOR)(float*), BSTR strFormat = NULL)
	{
		HRESULT hr;
		float val;
		hr = ((T*)this->*ACCESSOR)(&val);
		if (SUCCEEDED(hr))
		{
			hr = WriteElement(pArchive, nContext, strTag, _variant_t(val), strFormat);
		}
		return hr;
	}

	HRESULT WriteElement(IXMLArchive* pArchive, const long nContext, BSTR strTag, HRESULT (__stdcall T::*ACCESSOR)(BSTR*), BSTR strFormat = NULL)
	{
		HRESULT hr;
		BSTR strVal;
		hr = ((T*)this->*ACCESSOR)(&strVal);
		if (SUCCEEDED(hr))
		{
			hr = WriteElement(pArchive, nContext, strTag, _variant_t(strVal), strFormat);
			::SysFreeString(strVal);
		}
		return hr;
	}

	HRESULT WriteObjectCollection(IXMLArchive* pArchive, IUnknown* (__stdcall T::*ACCESSOR)(const long))
	{
		HRESULT hr = S_OK;

		long nCollIdx = 0;
		IUnknown* pCurUnk = NULL;

		while (SUCCEEDED(hr) && (pCurUnk = ((T*)this->*ACCESSOR)(nCollIdx++)) != NULL)
		{
			hr = pArchive->WriteObject(pCurUnk);
		}

		return hr;
	}

	HRESULT WriteObjectCollection(IXMLArchive* pArchive, const long nContext, BSTR strTag, IUnknown* (__stdcall T::*ACCESSOR)(const long))
	{
		HRESULT hr = pArchive->CreateElement(strTag);

		if (SUCCEEDED(hr))
		{
			hr = WriteObjectCollection(pArchive, ACCESSOR);
		}

		pArchive->SetContext(nContext);

		return hr;
	}
};

#define BEGIN_XML(resvar)   { HRESULT& hrXML = resvar;

#define READ_XML_ELEMENT(pArchive, nContext, strXPath, accessor)   \
	if (SUCCEEDED(hrXML)) hrXML = ReadElement(pArchive, nContext, strXPath, accessor);

#define WRITE_XML_ELEMENT(pArchive, nContext, strXPath, accessor)   \
	if (SUCCEEDED(hrXML)) hrXML = WriteElement(pArchive, nContext, strXPath, accessor);

#define WRITE_XML_ELEMENT_FMT(pArchive, strTag, accessor, strFormat)   \
	if (SUCCEEDED(hrXML)) hrXML = WriteElement(pArchive, nContext, strXPath, accessor, strFormat);

#define END_XML }

#define DEFINE_XML_TAG(t)     \
	public: \
		STDMETHOD(GetTag)(/* [out,retval] */ BSTR* pStrTag) \
		{ \
			if (pStrTag != NULL) \
			{ \
				*pStrTag = ::SysAllocString(t); \
				return S_OK; \
			} \
			return E_FAIL; \
		} \

//#define DEFINE_XML_TAG(c,t) const 

};  // namespace ShapeShifter

#endif // #ifndef __PERSISTXML_H__

