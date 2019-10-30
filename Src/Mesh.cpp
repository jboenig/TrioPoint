//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Mesh.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "Mesh.h"
#include "XMLArchive.h"
#include "XMLTags.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CMesh

HRESULT CMesh::FinalConstruct()
{
	return put_Name(tag_Mesh);
}

STDMETHODIMP CMesh::GetVertex(/* [in] */ VertexHandle Vertex,
                             /* [in] */ enum VertexField Field,
                             /* [out,retval] */ VARIANT* pVal)
{
	HRESULT hr = E_FAIL;

	if (Vertex < m_vertices.GetElementCount())
	{
		switch (Field)
		{
		case SR_VERTEX_POSITION:
			{
				vtPOINT3D pt;
				pt.x = m_vertices[Vertex].x;
				pt.y = m_vertices[Vertex].y;
				pt.z = m_vertices[Vertex].z;
				*pVal = pt;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_NORMAL:
			{
				vtVECTOR3D v;
				v.x = m_vertices[Vertex].nx;
				v.y = m_vertices[Vertex].ny;
				v.z = m_vertices[Vertex].nz;
				*pVal = v;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_TEXTURE_COORDINATE_1:
			{
				vtTEXTURECOORDINATE3D txcoord;
				txcoord.tu = m_vertices[Vertex].tu;
				txcoord.tv = m_vertices[Vertex].tv;
				*pVal = txcoord;
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
}

STDMETHODIMP CMesh::SetVertex(/* [in] */ VertexHandle Vertex,
                              /* [in] */ enum VertexField Field,
                              /* [in] */ VARIANT Val)
{
	HRESULT hr = E_FAIL;

	if (Vertex < m_vertices.GetElementCount())
	{
		switch (Field)
		{
		case SR_VERTEX_POSITION:
			{
				vtPOINT3D pt(Val);
				m_vertices[Vertex].x = pt.x;
				m_vertices[Vertex].y = pt.y;
				m_vertices[Vertex].z = pt.z;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_NORMAL:
			{
				vtVECTOR3D nv(Val);
				m_vertices[Vertex].nx = nv.x;
				m_vertices[Vertex].ny = nv.y;
				m_vertices[Vertex].nz = nv.z;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_TEXTURE_COORDINATE_1:
			{
				vtTEXTURECOORDINATE3D txcoord(Val);
				m_vertices[Vertex].tu = txcoord.tu;
				m_vertices[Vertex].tv = txcoord.tv;
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
}

STDMETHODIMP CMesh::AddVertex(/* [in] */ VertexField Field,
                              /* [in] */ VARIANT varVal,
                              /* [out,retval] */ VertexHandle* pVertexOut)
{
	HRESULT hr = E_FAIL;

	_VertexType v;
	memset(&v, 0, sizeof(_VertexType));

	switch (Field)
	{
	case SR_VERTEX_POSITION:
		{
			vtPOINT3D pt(varVal);
			v.x = pt.x;
			v.y = pt.y;
			v.z = pt.z;
			*pVertexOut = m_vertices.PutElement(v);
			hr = S_OK;
		}
		break;

	case SR_VERTEX_NORMAL:
		{
			vtVECTOR3D nv(varVal);
			v.nx = nv.x;
			v.ny = nv.y;
			v.nz = nv.z;
			*pVertexOut = m_vertices.PutElement(v);
			hr = S_OK;
		}
		break;

	case SR_VERTEX_TEXTURE_COORDINATE_1:
		{
			vtTEXTURECOORDINATE3D txcoord(varVal);
			v.tu = txcoord.tu;
			v.tv = txcoord.tv;
			*pVertexOut = m_vertices.PutElement(v);
			hr = S_OK;
		}
		break;
	}

	return hr;
}

STDMETHODIMP CMesh::UpdateVertex(/* [in] */ VertexField Field,
                                 /* [in] */ VARIANT varVal,
                                 /* [in] */ VertexHandle VertexIn,
                                 /* [out] */ VertexHandle* pVertexOut)
{
#if 1
	HRESULT hr = E_FAIL;

	if (VertexIn < m_vertices.GetElementCount())
	{
		_VertexType v = m_vertices[VertexIn];

		switch (Field)
		{
		case SR_VERTEX_POSITION:
			{
				vtPOINT3D pt(varVal);
				v.x = pt.x;
				v.y = pt.y;
				v.z = pt.z;
				*pVertexOut = m_vertices.PutElement(v);
				hr = S_OK;
			}
			break;

		case SR_VERTEX_NORMAL:
			{
				vtVECTOR3D nv(varVal);
				v.nx = nv.x;
				v.ny = nv.y;
				v.nz = nv.z;
				*pVertexOut = m_vertices.PutElement(v);
				hr = S_OK;
			}
			break;

		case SR_VERTEX_TEXTURE_COORDINATE_1:
			{
				vtTEXTURECOORDINATE3D txcoord(varVal);
				v.tu = txcoord.tu;
				v.tv = txcoord.tv;
				*pVertexOut = m_vertices.PutElement(v);
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
#else
	*pVertexOut = VertexIn;
	return S_OK;
#endif
}

STDMETHODIMP CMesh::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Mesh;
	return S_OK;
}

STDMETHODIMP CMesh::LoadXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nContext;
	hr = pArchive->GetContext(&nContext);

	if (hr == S_OK)
	{
		hr = ReadElement(pArchive, nContext, tag_Name, &CMesh::put_Name);

		// Read the list of vertices
		hr = pArchive->GotoNode(tag_Vertices);

		if (hr == S_OK)
		{
			HRESULT hrLoop = pArchive->GotoFirstChild();

			while (hrLoop == S_OK)
			{
				_VertexType v;
				VertexIndex vIdx;
				memset(&v, 0, sizeof(_VertexType));

				if (ReadVertex(pArchive, vIdx, v) == S_OK)
				{
					m_vertices.AppendElement(v);
				}

				hrLoop = pArchive->GotoNextSibling();
			}

			pArchive->SetContext(nContext);
		}

		if (hr == S_OK)
		{
			if (pArchive->GotoNode(tag_Primitives) == S_OK)
			{
				HRESULT hrPrimNode = pArchive->GotoFirstChild();

				while (hrPrimNode == S_OK)
				{
					if (pArchive->CompareCurrentTag(tag_Primitive) == S_OK)
					{
						CPrimitive prim;
						prim.LoadXML(pArchive);
						m_primitives.push_back(prim);
					}
					hrPrimNode = pArchive->GotoNextSibling();
				}
			}
		}

		pArchive->SetContext(nContext);
		pArchive->FreeContext(nContext);
	}

	return hr;
}

STDMETHODIMP CMesh::SaveXML(/* [in] */ IXMLArchive* pArchive)
{
	HRESULT hr = S_OK;

	long nContext;
	pArchive->GetContext(&nContext);

	hr = WriteElement(pArchive, nContext, tag_Name, &CMesh::get_Name);

	// Write out the list of vertices
	hr = pArchive->CreateElement(tag_Vertices);

	long nVerticesContext;
	pArchive->GetContext(&nVerticesContext);

	if (SUCCEEDED(hr))
	{
		for (VertexIndex i = 0; i < m_vertices.GetElementCount(); i++)
		{
			_VertexType v = m_vertices[i];
			hr = pArchive->CreateElement(tag_Vertex);
			WriteVertex(pArchive, i, v);
			pArchive->SetContext(nVerticesContext);
		}
	}

	pArchive->FreeContext(nVerticesContext);

	pArchive->SetContext(nContext);

	if (SUCCEEDED(hr))
	{
		hr = pArchive->CreateElement(tag_Primitives);

		if (hr == S_OK)
		{
			long nPrimitivesContext;
			pArchive->GetContext(&nPrimitivesContext);

			_PrimitiveVector::iterator iPrimitive;

			for (iPrimitive = m_primitives.begin(); hr == S_OK && iPrimitive != m_primitives.end(); iPrimitive++)
			{
				hr = pArchive->CreateElement(tag_Primitive);

				if (hr == S_OK)
				{
					hr = iPrimitive->SaveXML(pArchive);
				}

				pArchive->SetContext(nPrimitivesContext);
			}

			pArchive->FreeContext(nPrimitivesContext);
		}
	}

	SetAndFreeContext(pArchive, nContext);

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// CPrelitMesh

HRESULT CPrelitMesh::FinalConstruct()
{
	return put_Name(L"Mesh");
}

STDMETHODIMP CPrelitMesh::GetVertex(/* [in] */ VertexHandle Vertex,
                             /* [in] */ enum VertexField Field,
                             /* [out,retval] */ VARIANT* pVal)
{
	HRESULT hr = E_FAIL;

	if (Vertex < m_vertices.GetElementCount())
	{
		switch (Field)
		{
		case SR_VERTEX_POSITION:
			{
				vtPOINT3D pt;
				pt.x = m_vertices[Vertex].x;
				pt.y = m_vertices[Vertex].y;
				pt.z = m_vertices[Vertex].z;
				*pVal = pt;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_TEXTURE_COORDINATE_1:
			{
				vtTEXTURECOORDINATE3D txcoord;
				txcoord.tu = m_vertices[Vertex].tu;
				txcoord.tv = m_vertices[Vertex].tv;
				*pVal = txcoord;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_COLOR:
			break;

		case SR_VERTEX_SPECULAR:
			break;
		}
	}

	return hr;
}

STDMETHODIMP CPrelitMesh::SetVertex(/* [in] */ VertexHandle Vertex,
                                    /* [in] */ enum VertexField Field,
                                    /* [in] */ VARIANT Val)
{
	HRESULT hr = E_FAIL;

	if (Vertex < m_vertices.GetElementCount())
	{
		switch (Field)
		{
		case SR_VERTEX_POSITION:
			{
				vtPOINT3D pt(Val);
				m_vertices[Vertex].x = pt.x;
				m_vertices[Vertex].y = pt.y;
				m_vertices[Vertex].z = pt.z;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_TEXTURE_COORDINATE_1:
			{
				vtTEXTURECOORDINATE3D txcoord(Val);
				m_vertices[Vertex].tu = txcoord.tu;
				m_vertices[Vertex].tv = txcoord.tv;
				hr = S_OK;
			}
			break;

		case SR_VERTEX_COLOR:
			break;

		case SR_VERTEX_SPECULAR:
			break;
		}
	}

	return hr;
}

STDMETHODIMP CPrelitMesh::AddVertex(/* [in] */ VertexField Field,
                                    /* [in] */ VARIANT varVal,
                                    /* [out,retval] */ VertexHandle* pVertexOut)
{
	HRESULT hr = E_FAIL;

	_VertexType v;
	memset(&v, 0, sizeof(_VertexType));

	switch (Field)
	{
	case SR_VERTEX_POSITION:
		{
			vtPOINT3D pt(varVal);
			v.x = pt.x;
			v.y = pt.y;
			v.z = pt.z;
			*pVertexOut = m_vertices.PutElement(v);
			hr = S_OK;
		}
		break;

	case SR_VERTEX_TEXTURE_COORDINATE_1:
		{
			vtTEXTURECOORDINATE3D txcoord(varVal);
			v.tu = txcoord.tu;
			v.tv = txcoord.tv;
			*pVertexOut = m_vertices.PutElement(v);
			hr = S_OK;
		}
		break;

	case SR_VERTEX_COLOR:
		break;

	case SR_VERTEX_SPECULAR:
		break;
	}

	return hr;
}

STDMETHODIMP CPrelitMesh::UpdateVertex(/* [in] */ VertexField Field,
                                       /* [in] */ VARIANT varVal,
                                       /* [in] */ VertexHandle VertexIn,
                                       /* [out] */ VertexHandle* pVertexOut)
{
	HRESULT hr = E_FAIL;

	if (VertexIn < m_vertices.GetElementCount())
	{
		_VertexType v = m_vertices[VertexIn];

		switch (Field)
		{
		case SR_VERTEX_POSITION:
			{
				vtPOINT3D pt(varVal);
				v.x = pt.x;
				v.y = pt.y;
				v.z = pt.z;
				*pVertexOut = m_vertices.PutElement(v);
				hr = S_OK;
			}
			break;

		case SR_VERTEX_TEXTURE_COORDINATE_1:
			{
				vtTEXTURECOORDINATE3D txcoord(varVal);
				v.tu = txcoord.tu;
				v.tv = txcoord.tv;
				*pVertexOut = m_vertices.PutElement(v);
				hr = S_OK;
			}
			break;

		case SR_VERTEX_COLOR:
			break;

		case SR_VERTEX_SPECULAR:
			break;
		}
	}

	return hr;
}

};  // namespace ShapeShifter
