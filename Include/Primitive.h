//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Primitive.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#pragma once

#include "ShapeShifter.h"
#include "DynamicArray.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CPrimitive

class CPrimitive
{
// Embedded types
public:
	typedef CDynamicArray<WORD, VertexIndex, VertexCount, 12, 3> _VertexArrayType;

// Constructors/destructor
public:
	CPrimitive(const enum PrimitiveType primType = SSCL_TRIANGLELIST)
	{
		SetType(primType);
	}

	CPrimitive(const CPrimitive& src)
	{
		m_primType = src.m_primType;
		if (src.m_vertexArray.GetElementCount() > 0)
		{
			m_vertexArray.CopyArray(src.m_vertexArray, src.m_vertexArray.GetElementCount());
		}
	}

// Attributes
public:
	D3DPRIMITIVETYPE m_primType;
	_VertexArrayType m_vertexArray;
	UINT m_nIBOffset;

// Operations
public:
	CPrimitive& operator=(const CPrimitive& src)
	{
		m_primType = src.m_primType;
		m_vertexArray.CopyArray(src.m_vertexArray, src.m_vertexArray.GetElementCount());
		return *this;
	}

	enum PrimitiveType GetType() const
	{
		switch (m_primType)
		{
		case D3DPT_POINTLIST:
			return SSCL_POINTLIST;

		case D3DPT_LINELIST:
			return SSCL_LINELIST;

		case D3DPT_LINESTRIP:
			return SSCL_LINESTRIP;

		case D3DPT_TRIANGLELIST:
			return SSCL_TRIANGLELIST;

		case D3DPT_TRIANGLESTRIP:
			return SSCL_TRIANGLESTRIP;

		case D3DPT_TRIANGLEFAN:
			return SSCL_TRIANGLEFAN;
		}

		return SSCL_UNKNOWN_PRIMITIVE;
	}

	void SetType(enum PrimitiveType primType)
	{
		switch (primType)
		{
		case SSCL_POINTLIST:
			m_primType = D3DPT_POINTLIST;
			break;

		case SSCL_LINELIST:
			m_primType = D3DPT_LINELIST;
			break;

		case D3DPT_LINESTRIP:
			m_primType = D3DPT_LINESTRIP;
			break;

		case SSCL_TRIANGLELIST:
			m_primType = D3DPT_TRIANGLELIST;
			break;

		case D3DPT_TRIANGLESTRIP:
			m_primType = D3DPT_TRIANGLESTRIP;
			break;

		case D3DPT_TRIANGLEFAN:
			m_primType = D3DPT_TRIANGLEFAN;
			break;
		}
	}

	inline FaceCount GetFaceCount() const
	{
		switch (m_primType)
		{
		case D3DPT_TRIANGLELIST:
			return (m_vertexArray.GetElementCount() / 3);

		case D3DPT_TRIANGLESTRIP:
		case D3DPT_TRIANGLEFAN:
			return (m_vertexArray.GetElementCount() - 2);
		}

		return 0;
	}

	inline FaceCount GetPrimitiveCount() const
	{
		switch (m_primType)
		{
		case D3DPT_TRIANGLELIST:
			return (m_vertexArray.GetElementCount() / 3);

		case D3DPT_TRIANGLESTRIP:
		case D3DPT_TRIANGLEFAN:
			return (m_vertexArray.GetElementCount() - 2);
		}

		return 0;
	}

	HRESULT GetFace(/* [in] */ FaceIndex nFaceIdx,
	                /* [out] */ VertexHandle* pVertex1,
	                /* [out] */ VertexHandle* pVertex2,
	                /* [out] */ VertexHandle* pVertex3)
	{
		VertexIndex nStartIdx = static_cast<VertexIndex>(nFaceIdx * 3);

		if ((nStartIdx + 2) >= m_vertexArray.GetElementCount())
		{
			return E_INVALIDARG;
		}

		if (pVertex1 != NULL)
		{
			*pVertex1 = static_cast<VertexHandle>(m_vertexArray[nStartIdx]);
		}

		if (pVertex2 != NULL)
		{
			*pVertex2 = static_cast<VertexHandle>(m_vertexArray[nStartIdx + (VertexIndex)1]);
		}

		if (pVertex3 != NULL)
		{
			*pVertex3 = static_cast<VertexHandle>(m_vertexArray[nStartIdx + (VertexIndex)2]);
		}

		return S_OK;
	}

	HRESULT SetFace(/* [in] */ FaceIndex nFaceIdx,
	                /* [in] */ VertexHandle Vertex1,
	                /* [in] */ VertexHandle Vertex2,
	                /* [in] */ VertexHandle Vertex3)
	{
		VertexIndex nStartIdx = static_cast<VertexIndex>(nFaceIdx * 3);

		if ((nStartIdx + 2) >= m_vertexArray.GetElementCount())
		{
			return E_INVALIDARG;
		}

		m_vertexArray[nStartIdx] = static_cast<WORD>(Vertex1);
		m_vertexArray[nStartIdx + (VertexIndex)1] = static_cast<WORD>(Vertex2);
		m_vertexArray[nStartIdx + (VertexIndex)2] = static_cast<WORD>(Vertex3);

		return S_OK;
	}

	HRESULT AddFace(VertexHandle vertex1,
	                VertexHandle vertex2,
	                VertexHandle vertex3,
	                FaceIndex* pFaceIdx)
	{
		m_vertexArray.AppendElement(static_cast<WORD>(vertex1));
		m_vertexArray.AppendElement(static_cast<WORD>(vertex2));
		m_vertexArray.AppendElement(static_cast<WORD>(vertex3));

		if (pFaceIdx != NULL)
		{
			*pFaceIdx = (m_vertexArray.GetElementCount() / 3) - 1;
		}

		return S_OK;
	}

	HRESULT RemoveFace(FaceIndex nFaceIdx)
	{
		VertexIndex nStartIdx = static_cast<VertexIndex>(nFaceIdx * 3);
		VertexIndex nEndIdx = nStartIdx + 2;

		if (nEndIdx >= m_vertexArray.GetElementCount())
		{
			return E_INVALIDARG;
		}

		m_vertexArray.RemoveElements(nStartIdx, nEndIdx);

		return S_OK;
	}

	VertexCount GetVertexCount() const
	{
		return static_cast<VertexCount>(m_vertexArray.GetElementCount());
	}

	HRESULT GetVertex(VertexIndex nVertIdx, VertexHandle* pVertex)
	{
		if (nVertIdx >= m_vertexArray.GetElementCount())
		{
			return E_INVALIDARG;
		}

		if (pVertex == NULL)
		{
			return E_POINTER;
		}

		*pVertex = m_vertexArray[nVertIdx];

		return S_OK;
	}

	HRESULT SetVertex(VertexIndex nVertIdx, VertexHandle vertex)
	{
		if (nVertIdx >= m_vertexArray.GetElementCount())
		{
			return E_INVALIDARG;
		}

		m_vertexArray[nVertIdx] = static_cast<WORD>(vertex);

		return S_OK;
	}

	HRESULT SetVertexRange(VertexIndex nStartIdx,
	                       VertexCount nNumVertices,
	                       VertexHandle nFirstVert)
	{
		VertexHandle curVertex = nFirstVert;
		VertexIndex nEndIdx = nStartIdx + nNumVertices - 1;

		m_vertexArray.GrowArray(nStartIdx + nNumVertices);

		for (VertexIndex i = nStartIdx; i <= nEndIdx; i++)
		{
			m_vertexArray[i] = static_cast<WORD>(curVertex++);
		}

		return S_OK;
	}

	HRESULT AppendVertex(VertexHandle vertex)
	{
		m_vertexArray.AppendElement(static_cast<WORD>(vertex));
		return S_OK;
	}

	HRESULT InsertVertex(VertexIndex /* nVertIdx */, VertexHandle /* vertex */)
	{
		return E_NOTIMPL;
	}

	HRESULT RemoveVertex(VertexIndex /* nVertIdx */)
	{
		return E_NOTIMPL;
	}

	HRESULT Clear()
	{
		m_vertexArray.ClearArray();
		return S_OK;
	}

	void GetVertexUsage(USHORT* pVertexRefCount)
	{
		VertexIndex nVertIdx;

		for (nVertIdx = 0; nVertIdx < m_vertexArray.GetElementCount(); nVertIdx++)
		{
			VertexHandle vertex = m_vertexArray[nVertIdx];
			pVertexRefCount[vertex]++;
		}
	}

	void AttachVertexArray(WORD* pArray, const VertexCount nNumVertices)
	{
		m_vertexArray.AttachArray(pArray, nNumVertices, nNumVertices);
	}

	void GetIndexBufferSize(UINT* pSize)
	{
		*pSize = *pSize + m_vertexArray.m_nElementCount * sizeof(WORD);
	}

	bool LoadIndexBuffer(LPBYTE pIBData, UINT *pStartIndex)
	{
		m_nIBOffset = *pStartIndex;
		*pStartIndex += m_vertexArray.m_nElementCount;

		UINT nStartBytes = m_nIBOffset * sizeof(WORD);
		UINT nNumBytes = m_vertexArray.m_nElementCount * sizeof(WORD);

		if (nNumBytes > 0)
		{
			memcpy(&pIBData[nStartBytes], m_vertexArray, nNumBytes);
			return true;
		}

		return false;
	}

	inline HRESULT DrawBuffered(LPDIRECT3DDEVICE8 pDevice,
	                            UINT nMinIndex,
	                            UINT nNumVertices)
	{
		return pDevice->DrawIndexedPrimitive(m_primType,
		                                     nMinIndex,
		                                     nNumVertices,
		                                     m_nIBOffset,
		                                     GetPrimitiveCount());
	}

	inline HRESULT Draw(LPDIRECT3DDEVICE8 pDevice, LPVOID lpvVertices, UINT nNumVertices, UINT nVertexStreamZeroStride)
	{
		return pDevice->DrawIndexedPrimitiveUP(m_primType,
		                                       0,
		                                       nNumVertices,
		                                       GetPrimitiveCount(),
		                                       m_vertexArray,
		                                       D3DFMT_INDEX16,
		                                       lpvVertices,
		                                       nVertexStreamZeroStride);
	}

	HRESULT LoadXML(IXMLArchive* pArchive)
	{
		HRESULT hr = S_OK;

		long nContext;
		pArchive->GetContext(&nContext);

		VARIANT varType;

		if (pArchive->ReadAttribute(tag_Type, &varType) == S_OK)
		{
			if (_bstr_t(varType) == _bstr_t(tag_PointList))
			{
				m_primType = D3DPT_POINTLIST;
			}
			else if (_bstr_t(varType) == _bstr_t(tag_LineList))
			{
				m_primType = D3DPT_LINELIST;
			}
			else if (_bstr_t(varType) == _bstr_t(tag_LineStrip))
			{
				m_primType = D3DPT_LINESTRIP;
			}
			else if (_bstr_t(varType) == _bstr_t(tag_TriangleList))
			{
				m_primType = D3DPT_TRIANGLELIST;
			}
			else if (_bstr_t(varType) == _bstr_t(tag_TriangleStrip))
			{
				m_primType = D3DPT_TRIANGLESTRIP;
			}
			else if (_bstr_t(varType) == _bstr_t(tag_TriangleFan))
			{
				m_primType = D3DPT_TRIANGLEFAN;
			}

			::VariantClear(&varType);
		}

		HRESULT hrXML = pArchive->GotoFirstChild();

		while (hrXML == S_OK)
		{
			VARIANT val;
			if (SUCCEEDED(pArchive->ReadElement(VT_I4, &val)))
			{
				m_vertexArray.AppendElement((unsigned short) val.lVal);
			}
			hrXML = pArchive->GotoNextSibling();
		}

		pArchive->SetContext(nContext);
		pArchive->FreeContext(nContext);

		return hr;
	}

	HRESULT SaveXML(IXMLArchive* pArchive)
	{
		HRESULT hr = S_OK;

		long nContext;
		pArchive->GetContext(&nContext);

		switch (m_primType)
		{
		case D3DPT_POINTLIST:
			pArchive->WriteAttribute(tag_Type, _variant_t(tag_PointList));
			break;

		case D3DPT_LINELIST:
			pArchive->WriteAttribute(tag_Type, _variant_t(tag_LineList));
			break;

		case D3DPT_LINESTRIP:
			pArchive->WriteAttribute(tag_Type, _variant_t(tag_LineStrip));
			break;

		case D3DPT_TRIANGLELIST:
			pArchive->WriteAttribute(tag_Type, _variant_t(tag_TriangleList));
			break;

		case D3DPT_TRIANGLESTRIP:
			pArchive->WriteAttribute(tag_Type, _variant_t(tag_TriangleStrip));
			break;

		case D3DPT_TRIANGLEFAN:
			pArchive->WriteAttribute(tag_Type, _variant_t(tag_TriangleFan));
			break;
		}

		for (VertexIndex i = 0; hr == S_OK && i < m_vertexArray.GetElementCount(); i++)
		{
			hr = pArchive->CreateElement(tag_Vertex);

			if (hr == S_OK)
			{
				hr = pArchive->WriteElement(_variant_t((long)m_vertexArray[i]), NULL);
				pArchive->SetContext(nContext);
			}
		}

		pArchive->FreeContext(nContext);

		return hr;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __PRIMITIVE_H__
