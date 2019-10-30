//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Mesh.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MESH_H__
#define __MESH_H__

#pragma once

#pragma warning(disable: 4786)

#include <vector>

#include "ShapeShifter.h"
#include "Engine.h"
#include "DynamicArray.h"
#include "Node.h"
#include "Property.h"
#include "Primitive.h"
#include "PersistXML.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IMeshImpl

template <typename _vertex_type, DWORD _vertex_format>
class IMeshImpl : public INodeImpl< IDispatchPropConImpl<IMeshEx, &IID_IMeshEx, &LIBID_ShapeShifter> >,
                  public IDrawMeshDX8
{
// Embedded types
public:
	typedef IMeshImpl<_vertex_type,_vertex_format> _ThisClass;
	typedef _vertex_type _VertexType;
	typedef CDynamicArray<_vertex_type, VertexIndex, VertexCount, 3, 3> _VertexArrayType;
	typedef std::vector<CPrimitive> _PrimitiveVector;

// Constructors/destructor
public:
	IMeshImpl() :
		m_idVertexBuffer(SSCL_RESOURCEID_NULL),
		m_idIndexBuffer(SSCL_RESOURCEID_NULL)
	{
	}

	virtual ~IMeshImpl()
	{
	}

// Attributes
protected:
	_PrimitiveVector m_primitives;
	_VertexArrayType m_vertices;
	DeviceResourceId m_idVertexBuffer;
	DeviceResourceId m_idIndexBuffer;

// Operations
public:

	STDMETHOD(GetPrimitiveCount)(/* [out,retval] */ PrimitiveCount* pNumPrimitives)
	{
		if (pNumPrimitives == NULL)
		{
			return E_POINTER;
		}
		*pNumPrimitives = static_cast<long>(m_primitives.size());
		return S_OK;
	}

	STDMETHOD(AppendPrimitive)(/* [in] */ enum PrimitiveType primType, /* [out] */ PrimitiveIndex* pPrimIdx)
	{
		m_primitives.push_back(CPrimitive(primType));

		if (pPrimIdx != NULL)
		{
			*pPrimIdx = static_cast<long>(m_primitives.size() - 1);
		}

		return S_OK;
	}

	STDMETHOD(InsertPrimitive)(/* [in] */ enum PrimitiveType /* primType */, /* [in] */ PrimitiveIndex /* nPrimIdx */)
	{
		HRESULT hr = E_NOTIMPL;
		return hr;
	}

	STDMETHOD(RemovePrimitive)(/* [in] */ PrimitiveIndex nPrimIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		_PrimitiveVector::iterator iPrimitive = m_primitives.begin() + nPrimIdx;
		m_primitives.erase(iPrimitive);

		return S_OK;
	}

	STDMETHOD(RemoveAllPrimitives)()
	{
		m_primitives.clear();
		return S_OK;
	}

	STDMETHOD(GetPrimitiveVertexCount)(/* [in] */ PrimitiveIndex nPrimIdx,
	                                   /* [out,retval] */ VertexCount* pNumVertices)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		if (pNumVertices == NULL)
		{
			return E_POINTER;
		}

		*pNumVertices = m_primitives[nPrimIdx].GetVertexCount();

		return S_OK;
	}

	STDMETHOD(GetPrimitiveVertex)(/* [in] */ PrimitiveIndex nPrimIdx,
	                              /* [in] */ VertexIndex nVertIdx,
	                              /* [out,retval] */ VertexHandle* pVertex)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].GetVertex(nVertIdx, pVertex);
	}

	STDMETHOD(SetPrimitiveVertex)(/* [in] */ PrimitiveIndex nPrimIdx,
	                              /* [in] */ VertexIndex nVertIdx,
	                              /* [in] */ VertexHandle vertex)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].SetVertex(nVertIdx, vertex);
	}

	STDMETHOD(SetPrimitiveVertexRange)(/* [in] */ PrimitiveIndex nPrimIdx,
	                                   /* [in] */ VertexIndex nStartIdx,
	                                   /* [in] */ VertexCount nNumVertices,
	                                   /* [in] */ VertexHandle firstVert)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].SetVertexRange(nStartIdx, nNumVertices, firstVert);
	}

	STDMETHOD(AppendPrimitiveVertex)(/* [in] */ PrimitiveIndex nPrimIdx,
	                                 /* [in] */ VertexHandle vertex)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].AppendVertex(vertex);
	}

	STDMETHOD(InsertPrimitiveVertex)(/* [in] */ PrimitiveIndex nPrimIdx,
	                                 /* [in] */ VertexIndex nVertIdx,
	                                 /* [in] */ VertexHandle vertex)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].InsertVertex(nVertIdx, vertex);
	}

	STDMETHOD(RemovePrimitiveVertex)(/* [in] */ PrimitiveIndex nPrimIdx,
	                                 /* [in] */ VertexIndex nVertIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].RemoveVertex(nVertIdx);
	}

	STDMETHOD(ClearPrimitive)(/* [in] */ PrimitiveIndex nPrimIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].Clear();
	}

	STDMETHOD(GetPrimitiveFaceCount)(/* [in] */ PrimitiveIndex nPrimIdx,
	                                 /* [out,retval] */ FaceCount* pNumFaces)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		if (pNumFaces == NULL)
		{
			return E_POINTER;
		}

		*pNumFaces = m_primitives[nPrimIdx].GetFaceCount();

		return S_OK;
	}

	STDMETHOD(GetPrimitiveFace)(/* [in] */ PrimitiveIndex nPrimIdx,
	                            /* [in] */ FaceIndex nFaceIdx,
	                            /* [out] */ VertexHandle* pVertex1,
	                            /* [out] */ VertexHandle* pVertex2,
	                            /* [out] */ VertexHandle* pVertex3)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].GetFace(nFaceIdx, pVertex1, pVertex2, pVertex3);
	}

	STDMETHOD(SetPrimitiveFace)(/* [in] */ PrimitiveIndex nPrimIdx,
	                            /* [in] */ FaceIndex nFaceIdx,
	                            /* [in] */ VertexHandle vertex1,
	                            /* [in] */ VertexHandle vertex2,
	                            /* [in] */ VertexHandle vertex3)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].SetFace(nFaceIdx, vertex1, vertex2, vertex3);
	}

	STDMETHOD(AddPrimitiveFace)(/* [in] */ PrimitiveIndex nPrimIdx,
	                            /* [in] */ VertexHandle vertex1,
	                            /* [in] */ VertexHandle vertex2,
	                            /* [in] */ VertexHandle vertex3,
	                            /* [out,retval] */ FaceIndex* pFaceIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].AddFace(vertex1, vertex2, vertex3, pFaceIdx);
	}

	STDMETHOD(RemovePrimitiveFace)(/* [in] */ PrimitiveIndex nPrimIdx,
	                               /* [in] */ FaceIndex nFaceIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].RemoveFace(nFaceIdx);
	}

	STDMETHOD(GetVertex)(/* [in] */ VertexHandle Vertex,
	                     /* [in] */ enum VertexField Field,
	                     /* [out,retval] */ VARIANT* pVarVal)
	{
		HRESULT hr = E_FAIL;

		if (Vertex < m_vertices.GetElementCount())
		{
			if (Field == SR_VERTEX_POSITION)
			{
				vtPOINT3D pt;
				pt.x = m_vertices[Vertex].x;
				pt.y = m_vertices[Vertex].y;
				pt.z = m_vertices[Vertex].z;
				*pVarVal = pt;
				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(SetVertex)(/* [in] */ VertexHandle Vertex,
	                     /* [in] */ enum VertexField Field,
	                     /* [in] */ VARIANT varVal)
	{
		HRESULT hr = E_FAIL;

		if (Vertex < m_vertices.GetElementCount())
		{
			if (Field == SR_VERTEX_POSITION)
			{
				vtPOINT3D pt(varVal);
				m_vertices[Vertex].x = pt.x;
				m_vertices[Vertex].y = pt.y;
				m_vertices[Vertex].z = pt.z;
				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(AddVertex)(/* [in] */ VertexField Field,
	                     /* [in] */ VARIANT varVal,
	                     /* [out,retval] */ VertexHandle* pVertexOut)
	{
		HRESULT hr = E_FAIL;

		_VertexType v;
		memset(&v, 0, sizeof(_VertexType));

		if (Field == SR_VERTEX_POSITION)
		{
			vtPOINT3D pt(varVal);
			v.x = pt.x;
			v.y = pt.y;
			v.z = pt.z;
			*pVertexOut = m_vertices.PutElement(v);
			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(UpdateVertex)(/* [in] */ VertexField Field,
	                        /* [in] */ VARIANT varVal,
	                        /* [in] */ VertexHandle VertexIn,
	                        /* [out] */ VertexHandle* pVertexOut)
	{
		HRESULT hr = E_FAIL;

		if (VertexIn < m_vertices.GetElementCount())
		{
			_VertexType v = m_vertices[VertexIn];

			if (Field == SR_VERTEX_POSITION)
			{
				vtPOINT3D pt(varVal);
				v.x = pt.x;
				v.y = pt.y;
				v.z = pt.z;

				*pVertexOut = m_vertices.PutElement(v);

				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(RemoveUnusedVertices)()
	{
		HRESULT hr = E_FAIL;

		USHORT* pVertexRefCount = CountVertexReferences(m_vertices.m_nElementCount);

		if (pVertexRefCount != NULL)
		{
			long nNextUnusedSlot = 0;
			bool bDone = false;

			while (!bDone)
			{
				while (nNextUnusedSlot < m_vertices.m_nElementCount && pVertexRefCount[nNextUnusedSlot] > 0)
				{
					nNextUnusedSlot++;
				}

				long nNextOccupiedSlot = nNextUnusedSlot;

				while (nNextOccupiedSlot < m_vertices.m_nElementCount && pVertexRefCount[nNextOccupiedSlot] == 0)
				{
					nNextOccupiedSlot++;
				}

				long nShiftCount = nNextOccupiedSlot - nNextUnusedSlot;
				long nCopyCount = m_vertices.m_nElementCount - nNextOccupiedSlot;

				if (nShiftCount > 0 && nCopyCount > 0)
				{
					memmove(&m_vertices[nNextUnusedSlot], &m_vertices[nNextOccupiedSlot], nCopyCount * sizeof(_VertexType));

					if (pVertexRefCount != NULL)
					{
						memmove(&pVertexRefCount[nNextUnusedSlot], &pVertexRefCount[nNextOccupiedSlot], nCopyCount * sizeof(USHORT));
						memset(&pVertexRefCount[m_vertices.m_nElementCount - nShiftCount], 0, nShiftCount);
					}

					m_vertices.m_nElementCount = static_cast<VertexCount>(m_vertices.m_nElementCount - nShiftCount);

					// Loop through each primitive in the mesh and update vertices to
					// reflect the change in the vertex buffer.

					PrimitiveCount nPrimitives = m_primitives.size();

					for (PrimitiveIndex nPrimIdx = 0; nPrimIdx < nPrimitives; nPrimIdx++)
					{
						// Loop through each vertex in the current primitive

						VertexHandle vertex;
						VertexCount nVertexCount;
						nVertexCount = m_primitives[nPrimIdx].GetVertexCount();
						for (VertexIndex nVertIdx = 0; nVertIdx < nVertexCount; nVertIdx++)
						{
							// If the current vertex is in the block that moved, then decrement
							// the vertex by the number of slots removed.

							if (SUCCEEDED(m_primitives[nPrimIdx].GetVertex(nVertIdx, &vertex)))
							{
								if (vertex >= nNextOccupiedSlot)
								{
									vertex = static_cast<VertexHandle>(vertex - nShiftCount);
									m_primitives[nPrimIdx].SetVertex(nVertIdx, vertex);
								}
							}
						}
					}

					nNextUnusedSlot++;
				}
				else if (nShiftCount > 0)
				{
					m_vertices.m_nElementCount = static_cast<VertexCount>(m_vertices.m_nElementCount - nShiftCount);
					bDone = true;
				}
				else
				{
					bDone = true;
				}
			}

			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(UpdateVertexNormals)(/* [in] */ enum VertexNormalAlgorithm Algorithm)
	{
		if (Algorithm == SSCL_NORMAL_BLENDED)
		{
			return E_INVALIDARG;  // Not implemented yet
		}

		HRESULT hr = S_OK;

		if (!(_vertex_format & D3DFVF_NORMAL))
		{
			return S_FALSE;  // Mesh doesn't support normal vectors
		}

		_PrimitiveVector::iterator iPrimitive;

		for (iPrimitive = m_primitives.begin(); hr == S_OK && iPrimitive != m_primitives.end(); iPrimitive++)
		{
			FaceCount nFaces = iPrimitive->GetFaceCount();

			for (FaceIndex nFaceIdx = 0; hr == S_OK && nFaceIdx < nFaces; nFaceIdx++)
			{
				VertexHandle vert[3];
				D3DVECTOR normal;

				hr = iPrimitive->GetFace(nFaceIdx, &vert[0], &vert[1], &vert[2]);

				if (hr == S_OK)
				{
					CalcNormal(m_vertices[vert[0]], m_vertices[vert[1]], m_vertices[vert[2]], normal);

					for (int k = 0; hr == S_OK && k < 3; k++)
					{
						hr = UpdateVertex(SR_VERTEX_NORMAL,
								          vtVECTOR3D(normal),
								          vert[k], &vert[k]);
					}

					if (hr == S_OK)
					{
						hr = iPrimitive->SetFace(nFaceIdx, vert[0], vert[1], vert[2]);
					}
				}
			}
		}

		return hr;
	}

	STDMETHOD(PickFace)(/* [in] */ VARIANT varRayOrigin,
	                    /* [in] */ VARIANT varRayDirection,
	                    /* [out] */ PrimitiveIndex* pPrimIdx,
	                    /* [out] */ FaceIndex* pFaceIdx,
	                    /* [out] */ VALUE3D* pU,
	                    /* [out] */ VALUE3D* pV,
	                    /* [out] */ VALUE3D* pDist)
	{
		varRayOrigin;
		varRayDirection;
		pPrimIdx;
		pFaceIdx;
		pU;
		pV;
		pDist;
		return E_NOTIMPL;
	}

	STDMETHOD(get_VertexBufferId)(/* [out, retval] */ DeviceResourceId *pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}
		*pVal = m_idVertexBuffer;
		return S_OK;
	}

	STDMETHOD(put_VertexBufferId)(/* [in] */ DeviceResourceId newVal)
	{
		m_idVertexBuffer = newVal;
		return S_OK;
	}

	STDMETHOD(get_IndexBufferId)(/* [out, retval] */ DeviceResourceId *pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}
		*pVal = m_idIndexBuffer;
		return S_OK;
	}

	STDMETHOD(put_IndexBufferId)(/* [in] */ DeviceResourceId newVal)
	{
		m_idIndexBuffer = newVal;
		return S_OK;
	}

	// IDrawMeshDX8 interface

	STDMETHOD(Draw)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources, IContextProvider* pContextProvider)
	{
		HRESULT hr = E_FAIL;
		HRESULT hrStateBlock = E_FAIL;
		DWORD dwCurState;
		_PrimitiveVector::iterator iPrimitive;
		PrimitiveIndex nPrimIdx = 0;
		LPDIRECT3DVERTEXBUFFER8 pVB = NULL;
		LPDIRECT3DINDEXBUFFER8 pIB = NULL;

		HRESULT hrVB = pDevResources->GetVertexBuffer(m_idVertexBuffer, &pVB);
		HRESULT hrIB = pDevResources->GetIndexBuffer(m_idIndexBuffer, &pIB);

		hr = pDevice->SetVertexShader(_vertex_format);

		if (SUCCEEDED(hr))
		{
			if (hrVB != S_OK || hrIB != S_OK)
			{
				hrStateBlock = pDevice->CreateStateBlock(D3DSBT_ALL, &dwCurState);

				if (SUCCEEDED(hrStateBlock))
				{
					for (iPrimitive = m_primitives.begin(); hr == S_OK && iPrimitive != m_primitives.end(); iPrimitive++)
					{
						if (pContextProvider != NULL)
						{
							pContextProvider->SelectContext(pDevice, pDevResources, nPrimIdx);
						}

						hr = iPrimitive->Draw(pDevice, m_vertices, m_vertices.m_nElementCount, sizeof(_VertexType));

						if (hr == S_OK)
						{
							hr = pDevice->ApplyStateBlock(dwCurState);
						}

						nPrimIdx++;
					}

					pDevice->DeleteStateBlock(dwCurState);
				}
			}
			else
			{
				hr = pDevice->SetStreamSource(0, pVB, sizeof(_VertexType));

				if (SUCCEEDED(hr))
				{
					hr = pDevice->SetIndices(pIB, 0);
				}

				if (SUCCEEDED(hr))
				{
					hrStateBlock = pDevice->CreateStateBlock(D3DSBT_ALL, &dwCurState);

					if (SUCCEEDED(hrStateBlock))
					{
						for (iPrimitive = m_primitives.begin(); SUCCEEDED(hr) && iPrimitive != m_primitives.end(); iPrimitive++)
						{
							if (pContextProvider != NULL)
							{
								pContextProvider->SelectContext(pDevice, pDevResources, nPrimIdx);
							}

							hr = iPrimitive->DrawBuffered(pDevice, 0, m_vertices.m_nElementCount);

							if (SUCCEEDED(hr))
							{
								hr = pDevice->ApplyStateBlock(dwCurState);
							}

							nPrimIdx++;
						}

						pDevice->DeleteStateBlock(dwCurState);
					}
				}
			}
		}

		if (hrVB == S_OK && pVB != NULL)
		{
			pVB->Release();
		}

		if (hrIB == S_OK && pIB != NULL)
		{
			pIB->Release();
		}

		return hr;
	}

	STDMETHOD(BeginDraw)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources, BOOL* pBuffered)
	{
		BOOL bBuffered = FALSE;
		LPDIRECT3DVERTEXBUFFER8 pVB = NULL;
		LPDIRECT3DINDEXBUFFER8 pIB = NULL;

		HRESULT hr = E_FAIL;

		if (pDevice->SetVertexShader(_vertex_format) == D3D_OK)
		{
			hr = S_OK;

			HRESULT hrVB = pDevResources->GetVertexBuffer(m_idVertexBuffer, &pVB);
			HRESULT hrIB = pDevResources->GetIndexBuffer(m_idIndexBuffer, &pIB);

			if (hrVB == S_OK && hrIB == S_OK)
			{
				if (SUCCEEDED(pDevice->SetStreamSource(0, pVB, sizeof(_VertexType))))
				{
					if (SUCCEEDED(pDevice->SetIndices(pIB, 0)))
					{
						bBuffered = TRUE;
					}
				}
			}

			if (hrVB == S_OK && pVB != NULL)
			{
				pVB->Release();
			}

			if (hrIB == S_OK && pIB != NULL)
			{
				pIB->Release();
			}
		}

		if (pBuffered != NULL)
		{
			*pBuffered = bBuffered;
		}

		return hr;
	}

	STDMETHOD(DrawPrimitive)(LPDIRECT3DDEVICE8 pDevice,
	                         IResourceManagerDX8* /* pDevResources */,
	                         PrimitiveIndex nPrimIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].Draw(pDevice, m_vertices, m_vertices.m_nElementCount, sizeof(_VertexType));
	}

	STDMETHOD(DrawPrimitiveBuffered)(LPDIRECT3DDEVICE8 pDevice,
	                                 IResourceManagerDX8* /* pDevResources */,
	                                 PrimitiveIndex nPrimIdx)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		return m_primitives[nPrimIdx].DrawBuffered(pDevice, 0, m_vertices.m_nElementCount);
	}

	STDMETHOD(EndDraw)(LPDIRECT3DDEVICE8 /* pDevice */, IResourceManagerDX8* /* pDevResources */)
	{
		return S_OK;
	}

	// IMeshEx interface

	STDMETHOD(_GetVertexFormat)(DWORD* pVertexTypeDesc)
	{
		if (pVertexTypeDesc != NULL)
		{
			*pVertexTypeDesc = _vertex_format;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(_GetVertexSize)(UINT* pVertexSize)
	{
		if (pVertexSize != NULL)
		{
			*pVertexSize = sizeof(_VertexType);
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(_GetVertexCount)(long* pNumVertices)
	{
		if (pNumVertices != NULL)
		{
			*pNumVertices = m_vertices.m_nElementCount;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(_GetVertexBuffer)(LPVOID* ppVertexBuf)
	{
		if (ppVertexBuf == NULL)
		{
			return E_POINTER;
		}
		*ppVertexBuf = m_vertices;
		return S_OK;
	}

	STDMETHOD(_SetVertexBuffer)(LPVOID pVertexBuf, VertexCount nVertices)
	{
		m_vertices.AttachArray((_VertexType*) pVertexBuf, nVertices, nVertices);
		return S_OK;
	}

	STDMETHOD(_PutVertex)(LPVOID pVertex, DWORD dwVertexTypeDesc, VertexHandle* pVertexOut)
	{
		if (dwVertexTypeDesc == _vertex_format)
		{
			*pVertexOut = m_vertices.PutElement(*((_VertexType*)pVertex));
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(_GetVertices)(VertexHandle StartVertex,
	                        long nNumVertices,
	                        LPVOID pVertexOutBuffer,
	                        DWORD dwVertexTypeDesc)
	{
		VertexHandle EndVertex = StartVertex + nNumVertices - 1;

		if (StartVertex < 0 || StartVertex >= m_vertices.m_nElementCount ||
		    EndVertex < 0 || EndVertex >= m_vertices.m_nElementCount)
		{
			return E_INVALIDARG;
		}

		HRESULT hr = E_FAIL;

		if (dwVertexTypeDesc == _vertex_format)
		{
			memcpy(pVertexOutBuffer, &m_vertices.m_pElementArray[StartVertex],
			       sizeof(_VertexType) * nNumVertices);
			hr = S_OK;
		}
		else
		{
			// TODO: Provide conversion between differing
			// vertex formats.
		}

		return hr;
	}

	STDMETHOD(_SetVertices)(LPVOID pVertexInBuffer,
	                        DWORD dwVertexTypeDesc,
	                        VertexHandle nStartVertex,
	                        VertexCount nNumVertices)
	{
		if (nStartVertex < 0)
		{
			return E_INVALIDARG;
		}

		HRESULT hr = E_FAIL;

		if (dwVertexTypeDesc == _vertex_format)
		{
			VertexCount nNewCount = (m_vertices.m_nElementCount >= (nStartVertex + nNumVertices)) ?
			                        (m_vertices.m_nElementCount) :
			                        (nStartVertex + nNumVertices);

			// Make sure vertex array is large enough
			m_vertices.GrowArray(nNewCount);

			// Do a straight copy since incoming vertex format matches
			memcpy(&m_vertices.m_pElementArray[nStartVertex],
			       pVertexInBuffer,
			       sizeof(_VertexType) * nNumVertices);

			m_vertices.m_nElementCount = nNewCount;

			hr = S_OK;
		}
		else
		{
			// TODO: Provide conversion between differing
			// vertex formats.
		}

		return hr;
	}

	STDMETHOD(_GetPoints)(D3DVECTOR* pPoints, VertexCount nBufferSize, VertexCount* pActualSize)
	{
		HRESULT hr = E_FAIL;
		VertexCount nPoints = 0;

		if (pPoints != NULL && nBufferSize > 0)
		{
			for (VertexIndex i = 0; i < m_vertices.m_nElementCount && i < nBufferSize; i++)
			{
				pPoints[i].x = m_vertices[i].x;
				pPoints[i].y = m_vertices[i].y;
				pPoints[i].z = m_vertices[i].z;
				nPoints++;
			}

			if (nPoints > 0)
			{
				hr = S_OK;
			}
		}
		else
		{
			nPoints = m_vertices.m_nElementCount;
			hr = S_OK;
		}

		if (pActualSize != NULL)
		{
			*pActualSize = nPoints;
		}

		return hr;
	}

	STDMETHOD(_GetTriangleCount)(FaceCount* pNumTriangles)
	{
		if (pNumTriangles == NULL)
		{
			return E_POINTER;
		}

		*pNumTriangles = 0;
		_PrimitiveVector::iterator iPrimitive;

		for (iPrimitive = m_primitives.begin(); iPrimitive != m_primitives.end(); iPrimitive++)
		{
			*pNumTriangles += iPrimitive->GetFaceCount();
		}

		return S_OK;
	}

	STDMETHOD(_GetTriangleList)(D3DVECTOR* pTriangleList,
	                            FaceCount* pTrianglesCopied)
	{
		HRESULT hr = S_OK;
		_PrimitiveVector::iterator iPrimitive;
		VertexIndex vertIdx = 0;

		for (iPrimitive = m_primitives.begin(); hr == S_OK && iPrimitive != m_primitives.end(); iPrimitive++)
		{
			FaceCount nFaceCount = iPrimitive->GetFaceCount();
			VertexHandle v1, v2, v3;

			FaceIndex nFaceIdx = 0;

			for (nFaceIdx = 0; hr == S_OK && nFaceIdx < nFaceCount; nFaceIdx++)
			{
				hr = iPrimitive->GetFace(nFaceIdx, &v1, &v2, &v3);

				if (SUCCEEDED(hr))
				{
					pTriangleList[vertIdx].x = m_vertices[v1].x;
					pTriangleList[vertIdx].y = m_vertices[v1].y;
					pTriangleList[vertIdx].z = m_vertices[v1].z;
					vertIdx++;

					pTriangleList[vertIdx].x = m_vertices[v2].x;
					pTriangleList[vertIdx].y = m_vertices[v2].y;
					pTriangleList[vertIdx].z = m_vertices[v2].z;
					vertIdx++;

					pTriangleList[vertIdx].x = m_vertices[v3].x;
					pTriangleList[vertIdx].y = m_vertices[v3].y;
					pTriangleList[vertIdx].z = m_vertices[v3].z;
					vertIdx++;
				}
			}
		}

		if (pTrianglesCopied != NULL)
		{
			*pTrianglesCopied = vertIdx / 3;
		}

		return hr;
	}

	STDMETHOD(_AddVerticesToPrimitive)(PrimitiveIndex nPrimIdx,
	                                   LPVOID pVertices,
	                                   DWORD dwVertexTypeDesc,
	                                   VertexCount nNumVertices)
	{
		HRESULT hr = E_FAIL;

		if (dwVertexTypeDesc == _vertex_format)
		{
			// Incoming vertex format matches the vertex format of
			// this mesh. Just append the vertices and set the
			// range of vertices in the primitive.

			VertexIndex nOffset = m_vertices.AppendArray((_vertex_type*) pVertices,
				                                         nNumVertices);

			VertexCount nStartIdx = m_primitives[nPrimIdx].GetVertexCount();
			hr = m_primitives[nPrimIdx].SetVertexRange(nStartIdx, nNumVertices, nOffset);
		}
		else
		{
			// Incoming vertex format is different than the vertex format
			// of the mesh. Must loop through each vertex and convert.

			size_t nVertexSize = D3DXGetFVFVertexSize(dwVertexTypeDesc);
			_VertexType v;

			for (VertexIndex i = 0; SUCCEEDED(hr) && i < nNumVertices; i++)
			{
				memset(&v, 0, sizeof(_VertexType));
				CopyFlexVertex((LPVOID) &v, _vertex_format,
					           ((BYTE*)pVertices)+(i*nVertexSize),
					           dwVertexTypeDesc);
				VertexHandle nVertexId = m_vertices.PutElement(v);
				hr = m_primitives[nPrimIdx].AppendVertex(nVertexId);
			}
		}

		return hr;
	}

	STDMETHOD(_AttachVerticesToPrimitive)(PrimitiveIndex nPrimIdx,
	                                      WORD* pArray,
	                                      const VertexCount nNumVertices)
	{
		if (nPrimIdx < 0 || nPrimIdx >= (PrimitiveIndex) m_primitives.size())
		{
			return E_INVALIDARG;
		}

		m_primitives[nPrimIdx].AttachVertexArray(pArray, nNumVertices);

		return S_OK;
	}

	STDMETHOD(_GetVertexBuffer)(LPVOID* ppVertexBuffer, long* pVertexCount)
	{
		if (pVertexCount != NULL)
		{
			*pVertexCount = m_vertices.m_nElementCount;
		}
		if (ppVertexBuffer != NULL)
		{
			*ppVertexBuffer = m_vertices.m_pElementArray;
		}
		return S_OK;
	}

	STDMETHOD(_SetVertexBuffer)(LPVOID pVertices,
	                            DWORD dwVertexTypeDesc,
	                            VertexCount nNumVertices)
	{
		m_vertices.ClearArray();

		if (dwVertexTypeDesc == _vertex_format)
		{
			// Incoming vertex format matches the vertex format of
			// this mesh.
			m_vertices.AppendArray((_vertex_type*) pVertices, nNumVertices);
		}
		else
		{
			// Incoming vertex format is different than the vertex format
			// of the mesh. Must loop through each vertex and convert.

			size_t nVertexSize = D3DXGetFVFVertexSize(dwVertexTypeDesc);
			_VertexType v;

			for (VertexIndex i = 0; i < nNumVertices; i++)
			{
				memset(&v, 0, sizeof(_VertexType));
				CopyFlexVertex((LPVOID) &v, _vertex_format,
						       ((BYTE*)pVertices)+(i*nVertexSize),
						       dwVertexTypeDesc);
				m_vertices.AppendElement(v);
			}
		}

		return S_OK;
	}

	//////////////////////////////////////////////////////////////
	// IDeviceResource interface

	STDMETHOD(LoadDevice)(/* [in] */ IDevice* pDevice)
	{
		HRESULT hr = S_OK;
		LPDIRECT3DDEVICE8 pD3DDevice = NULL;


		if (m_vertices.m_nElementCount == 0)
		{
			return S_OK;
		}

		IResourceManagerDX8* pResMgr = NULL;

		if (SUCCEEDED(pDevice->QueryInterface(IID_IResourceManagerDX8, (LPVOID*) &pResMgr)))
		{
			hr = pResMgr->GetD3DDevice8(&pD3DDevice);

			if (hr == S_OK)
			{
				// Create and load vertex buffer
				LPDIRECT3DVERTEXBUFFER8 pVB = NULL;
				UINT nVBLength = m_vertices.m_nElementCount * sizeof(_VertexType);
				hr = pD3DDevice->CreateVertexBuffer(nVBLength,
				                                    D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
				                                    _vertex_format,
				                                    D3DPOOL_DEFAULT,
				                                    &pVB);

				if (hr == D3D_OK && pVB != NULL)
				{
					BYTE* pVBData = NULL;
					hr = pVB->Lock(0, nVBLength, &pVBData, D3DLOCK_DISCARD);

					if (hr == D3D_OK)
					{
						memcpy(pVBData, m_vertices, nVBLength);
						hr = pVB->Unlock();
					}

					if (hr == D3D_OK)
					{
						// Add the vertex buffer to the device
						if (m_idVertexBuffer == SSCL_RESOURCEID_NULL)
						{
							hr = pResMgr->GenerateResourceId(D3DRTYPE_VERTEXBUFFER, &m_idVertexBuffer);
						}
						else
						{
							hr = pResMgr->ReserveResourceId(D3DRTYPE_VERTEXBUFFER, m_idVertexBuffer);
						}

						if (hr == S_OK)
						{
							hr = pResMgr->SetVertexBuffer(m_idVertexBuffer, pVB);
						}
					}

					pVB->Release();
				}

				// Create index buffer
				LPDIRECT3DINDEXBUFFER8 pIB = NULL;
				if (hr == S_OK)
				{
					// Loop through each primitive and determine the
					// size of the index buffer
					UINT nIBSize = 0;
					_PrimitiveVector::iterator iPrimitive = m_primitives.begin();

					while (iPrimitive != m_primitives.end())
					{
						iPrimitive->GetIndexBufferSize(&nIBSize);
						iPrimitive++;
					}

					// Create and load index buffer
					hr = pD3DDevice->CreateIndexBuffer(nIBSize,
					                                   D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
					                                   D3DFMT_INDEX16,
					                                   D3DPOOL_DEFAULT,
					                                   &pIB);

					if (hr == D3D_OK && pIB != NULL)
					{
						BYTE* pIBData = NULL;
						hr = pIB->Lock(0, nIBSize, &pIBData, D3DLOCK_DISCARD);

						if (hr == D3D_OK)
						{
							iPrimitive = m_primitives.begin();
							UINT nStartIndex = 0;

							while (iPrimitive != m_primitives.end())
							{
								iPrimitive->LoadIndexBuffer(pIBData, &nStartIndex);
								iPrimitive++;
							}

							hr = pIB->Unlock();
						}

						if (hr == D3D_OK)
						{
							if (m_idIndexBuffer == SSCL_RESOURCEID_NULL)
							{
								hr = pResMgr->GenerateResourceId(D3DRTYPE_INDEXBUFFER, &m_idIndexBuffer);
							}
							else
							{
								hr = pResMgr->ReserveResourceId(D3DRTYPE_INDEXBUFFER, m_idIndexBuffer);
							}

							if (hr == S_OK)
							{
								hr = pResMgr->SetIndexBuffer(m_idIndexBuffer, pIB);
							}
						}

						pIB->Release();
					}
				}

				pD3DDevice->Release();
			}

			pResMgr->Release();
		}

		return hr;
	}

	STDMETHOD(UnloadDevice)(/* [in] */ IDevice* pDevice)
	{
		pDevice;
		return E_NOTIMPL;
	}

// Implementation
protected:
	USHORT* CountVertexReferences(const VertexCount nVertexCount)
	{
		USHORT* pVertexRefCount = NULL;

		if (nVertexCount > 0)
		{
			// Create and initialize the reference count array

			pVertexRefCount = new USHORT[nVertexCount];
			memset(pVertexRefCount, 0, sizeof(USHORT) * nVertexCount);

			// Have each primitive in the mesh increment the reference count for
			// each vertex it uses.

			PrimitiveCount nPrimitiveCount = (PrimitiveCount) m_primitives.size();

			for (PrimitiveIndex  nPrimIdx = 0; nPrimIdx < nPrimitiveCount; nPrimIdx++)
			{
				m_primitives[nPrimIdx].GetVertexUsage(pVertexRefCount);
			}
		}

		return pVertexRefCount;
	}
};

/////////////////////////////////////////////////////////////////////////
// CMesh

class ATL_NO_VTABLE CMesh : public CComObjectRootEx<CComSingleThreadModel>,
                            public CComCoClass<CMesh, &CLSID_Mesh>,
                            public IMeshImpl<VERTEX3D,D3DFVF_VERTEX>,
                            public IPersistXMLImpl<CMesh>
{
public:
	typedef IMeshImpl<VERTEX3D,D3DFVF_VERTEX> _MeshBase;

DECLARE_REGISTRY_RESOURCEID(IDR_MESH)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Mesh)

// Interface map
BEGIN_COM_MAP(CMesh)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(IDeviceResource)
	COM_INTERFACE_ENTRY(IMesh)
	COM_INTERFACE_ENTRY_IID(IID_IMeshEx, IMeshEx)
	COM_INTERFACE_ENTRY_IID(IID_IDrawMeshDX8, IDrawMeshDX8)
	COM_INTERFACE_ENTRY(IPersistXML)
END_COM_MAP()

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IMesh interface

	STDMETHOD(GetVertex)(/* [in] */ VertexHandle Vertex,
	                     /* [in] */ enum VertexField Field,
	                     /* [out,retval] */ VARIANT* pVal);

	STDMETHOD(SetVertex)(/* [in] */ VertexHandle Vertex,
	                     /* [in] */ enum VertexField Field,
	                     /* [in] */ VARIANT Val);

	STDMETHOD(AddVertex)(/* [in] */ VertexField Field,
	                     /* [in] */ VARIANT varVal,
	                     /* [out,retval] */ VertexHandle* pVertexOut);

	STDMETHOD(UpdateVertex)(/* [in] */ VertexField Field,
	                        /* [in] */ VARIANT varVal,
	                        /* [in] */ VertexHandle VertexIn,
	                        /* [out] */ VertexHandle* pVertexOut);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);
};

/////////////////////////////////////////////////////////////////////////
// CPrelitMesh

class ATL_NO_VTABLE CPrelitMesh : public CComObjectRootEx<CComSingleThreadModel>,
                                  public CComCoClass<CPrelitMesh, &CLSID_PrelitMesh>,
                                  public IMeshImpl<LVERTEX3D,D3DFVF_LVERTEX>
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_PRELITMESH)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CPrelitMesh)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(IDeviceResource)
	COM_INTERFACE_ENTRY(IMesh)
	COM_INTERFACE_ENTRY_IID(IID_IMeshEx, IMeshEx)
	COM_INTERFACE_ENTRY_IID(IID_IDrawMeshDX8, IDrawMeshDX8)
END_COM_MAP()

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IMesh interface

	STDMETHOD(GetVertex)(/* [in] */ VertexHandle Vertex,
	                     /* [in] */ enum VertexField Field,
	                     /* [out,retval] */ VARIANT* pVal);

	STDMETHOD(SetVertex)(/* [in] */ VertexHandle Vertex,
	                     /* [in] */ enum VertexField Field,
	                     /* [in] */ VARIANT Val);

	STDMETHOD(AddVertex)(/* [in] */ VertexField Field,
	                     /* [in] */ VARIANT varVal,
	                     /* [out,retval] */ VertexHandle* pVertexOut);

	STDMETHOD(UpdateVertex)(/* [in] */ VertexField Field,
	                        /* [in] */ VARIANT varVal,
	                        /* [in] */ VertexHandle VertexIn,
	                        /* [out] */ VertexHandle* pVertexOut);
};

};  // namespace ShapeShifter

#endif // #ifndef __MESH_H__
