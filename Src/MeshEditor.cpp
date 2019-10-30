//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: MeshEditor.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <math.h>

#include "MeshEditor.h"
#include "Primitive.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CMeshEditor

CMeshEditor::CMeshEditor() :
	m_pMesh(NULL),
	m_curPrimIdx(0)
{
}

CMeshEditor::~CMeshEditor()
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
	}
}

STDMETHODIMP CMeshEditor::BeginEdit(/* [in] */ IMesh* pMesh)
{
	HRESULT hr = E_FAIL;

	if (m_pMesh == NULL)
	{
		if (pMesh != NULL)
		{
			hr = pMesh->QueryInterface(IID_IMeshEx, (LPVOID*) &m_pMesh);
		}
	}

	return hr;
}

STDMETHODIMP CMeshEditor::SelectPrimitive(/* [in] */ PrimitiveIndex nPrimIdx)
{
	m_curPrimIdx = nPrimIdx;
	return S_OK;
}

STDMETHODIMP CMeshEditor::AppendVertex(/* [in] */ enum VertexField Field,
                                       /* [in] */ VARIANT varVal)
{
	HRESULT hr = E_FAIL;
	VertexHandle Vertex;

	if (m_pMesh != NULL)
	{
		hr = m_pMesh->AddVertex(Field, varVal, &Vertex);

		if (SUCCEEDED(hr))
		{
			hr = m_pMesh->AppendPrimitiveVertex(m_curPrimIdx, Vertex);
		}
	}

	return hr;
}

STDMETHODIMP CMeshEditor::UpdateVertex(/* [in] */ VertexIndex nVertIdx,
                                       /* [in] */ enum VertexField Field,
                                       /* [in] */ VARIANT varVal)
{
	HRESULT hr = E_FAIL;

	if (m_pMesh != NULL)
	{
		VertexHandle vertexId;
		VertexHandle newVertexId;

		hr = m_pMesh->GetPrimitiveVertex(m_curPrimIdx, nVertIdx, &vertexId);

		if (SUCCEEDED(hr))
		{
			hr = m_pMesh->UpdateVertex(Field, varVal, vertexId, &newVertexId);

			if (hr == S_OK)
			{
				if (newVertexId != vertexId)
				{
					// Vertex handle changed. Update the primitive.
					hr = m_pMesh->SetPrimitiveVertex(m_curPrimIdx, nVertIdx, newVertexId);
				}
			}
		}
	}

	return hr;
}

STDMETHODIMP CMeshEditor::RemoveVertex(/* [in] */ VertexIndex nVertIdx)
{
	HRESULT hr = E_FAIL;

	if (m_pMesh != NULL)
	{
		hr = m_pMesh->RemovePrimitiveVertex(m_curPrimIdx, nVertIdx);
	}

	return E_NOTIMPL;
}

STDMETHODIMP CMeshEditor::UpdateVertexPosition(/* [in] */ VertexIndex nVertIdx,
                                               /* [in] */ VALUE3D x,
                                               /* [in] */ VALUE3D y,
                                               /* [in] */ VALUE3D z)
{
	vtPOINT3D pt(x, y, z);
	return UpdateVertex(nVertIdx, SR_VERTEX_POSITION, pt);
}

STDMETHODIMP CMeshEditor::UpdateTextureCoordinate(/* [in] */ VertexIndex nVertIdx,
                                                  /* [in] */ VALUE3D tu,
                                                  /* [in] */ VALUE3D tv)
{
	vtTEXTURECOORDINATE3D texcoord;
	texcoord.tu = tu;
	texcoord.tv = tv;
	return UpdateVertex(nVertIdx, SR_VERTEX_TEXTURE_COORDINATE_1, texcoord);
}

STDMETHODIMP CMeshEditor::AppendFace(/* [in] */ VARIANT varTriangle)
{
	HRESULT hr = E_FAIL;

	vtTRIANGLE3D triangle(varTriangle);

	hr = AppendVertex(SR_VERTEX_POSITION, vtPOINT3D(triangle.x1, triangle.y1, triangle.z1));

	if (hr == S_OK)
	{
		hr = AppendVertex(SR_VERTEX_POSITION, vtPOINT3D(triangle.x2, triangle.y2, triangle.z2));
	}

	if (hr == S_OK)
	{
		hr = AppendVertex(SR_VERTEX_POSITION, vtPOINT3D(triangle.x3, triangle.y3, triangle.z3));
	}

	return hr;
}

STDMETHODIMP CMeshEditor::UpdateFaceNormal(/* [in] */ FaceIndex nFaceIdx,
                                           /* [in] */ VALUE3D NX,
                                           /* [in] */ VALUE3D NY,
                                           /* [in] */ VALUE3D NZ)
{
	HRESULT hr = E_FAIL;

	if (m_pMesh != NULL)
	{
		VertexHandle vert[3];
		VARIANT varNormal(vtVECTOR3D(NX,NY,NZ));

		// Get the vertices for the given face
		hr = m_pMesh->GetPrimitiveFace(m_curPrimIdx, nFaceIdx, &vert[0], &vert[1], &vert[2]);

		if (hr == S_OK)
		{
			// Loop through each vertex in the face
			for (VertexIndex i = 0; hr == S_OK && i < 3; i++)
			{
				// Update the normal vector of the current vertex
				hr = m_pMesh->UpdateVertex(SR_VERTEX_NORMAL, varNormal, vert[i], &vert[i]);
			}

			if (hr == S_OK)
			{
				hr = m_pMesh->SetPrimitiveFace(m_curPrimIdx, nFaceIdx, vert[0], vert[1], vert[2]);
			}
		}

		::VariantClear(&varNormal);
	}

	return hr;
}

STDMETHODIMP CMeshEditor::MakeCube(/* [in] */ VALUE3D cx, /* [in] */ VALUE3D cy, /* [in] */ VALUE3D cz)
{
	HRESULT hr = E_FAIL;

	if (m_pMesh != NULL)
	{
		PrimitiveIndex nPrimIdx;

		hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLELIST, &nPrimIdx);

		if (SUCCEEDED(hr))
		{
			SelectPrimitive(nPrimIdx);

			VALUE3D posX = cx / 2;
			VALUE3D negX = -posX;
			VALUE3D posY = cy / 2;
			VALUE3D negY = -posY;
			VALUE3D posZ = cz / 2;
			VALUE3D negZ = -posZ;

			AppendFace(vtTRIANGLE3D(POINT3D(negX, negY, negZ),
			                        POINT3D(negX, posY, negZ),
			                        POINT3D(posX, negY, negZ)));
			UpdateFaceNormal(0, 0.0f, 0.0f, -1.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(posX, negY, negZ),
			                        POINT3D(negX, posY, negZ),
			                        POINT3D(posX, posY, negZ)));
			UpdateFaceNormal(1, 0.0f, 0.0f, -1.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(posX, negY, negZ),
			                        POINT3D(posX, posY, negZ),
			                        POINT3D(posX, negY, posZ)));
			UpdateFaceNormal(2, 1.0f, 0.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(posX, negY, posZ),
			                        POINT3D(posX, posY, negZ),
			                        POINT3D(posX, posY, posZ)));
			UpdateFaceNormal(3, 1.0f, 0.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(posX, negY, posZ),
			                        POINT3D(posX, posY, posZ),
			                        POINT3D(negX, negY, posZ)));
			UpdateFaceNormal(4, 0.0f, 0.0f, 1.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(negX, negY, posZ),
			                        POINT3D(posX, posY, posZ),
			                        POINT3D(negX, posY, posZ)));
			UpdateFaceNormal(5, 0.0f, 0.0f, 1.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(negX, negY, posZ),
			                        POINT3D(negX, posY, posZ),
			                        POINT3D(negX, negY, negZ)));
			UpdateFaceNormal(6, -1.0f, 0.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(negX, negY, negZ),
			                        POINT3D(negX, posY, posZ),
			                        POINT3D(negX, posY, negZ)));
			UpdateFaceNormal(7, -1.0f, 0.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(negX, posY, negZ),
			                        POINT3D(negX, posY, posZ),
			                        POINT3D(posX, posY, negZ)));
			UpdateFaceNormal(8, 0.0f, 1.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(posX, posY, negZ),
			                        POINT3D(negX, posY, posZ),
			                        POINT3D(posX, posY, posZ)));
			UpdateFaceNormal(9, 0.0f, 1.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(negX, negY, negZ),
			                        POINT3D(posX, negY, negZ),
			                        POINT3D(negX, negY, posZ)));
			UpdateFaceNormal(10, 0.0f, -1.0f, 0.0f);

			AppendFace(vtTRIANGLE3D(POINT3D(posX, negY, negZ),
			                        POINT3D(posX, negY, posZ),
			                        POINT3D(negX, negY, posZ)));
			UpdateFaceNormal(11, 0.0f, -1.0f, 0.0f);

			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CMeshEditor::MakePyramid(/* [in] */ VALUE3D fBase,
                                      /* [in] */ VALUE3D fHeight)
{
	if (m_pMesh == NULL)
	{
		return E_FAIL;
	}

	if (fBase <= 0.0f)
	{
		return E_FAIL;
	}

	if (fHeight <= 0.0f)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;

	VALUE3D fMinY = -(fHeight / 2.0f);
	VALUE3D fMaxY = (fHeight / 2.0f);
	VALUE3D fMinX = -(fBase / 2.0f);
	VALUE3D fMaxX = (fBase / 2.0f);
	VALUE3D fMinZ = -(fBase / 2.0f);
	VALUE3D fMaxZ = (fBase / 2.0f);

	int nVertexCount = 18;
	VERTEX3D* pVertices = new VERTEX3D[nVertexCount];
	ZeroMemory(pVertices, sizeof(VERTEX3D) * nVertexCount);

	// Base

	pVertices[0].x = fMinX;
	pVertices[0].y = fMinY;
	pVertices[0].z = fMinZ;
	pVertices[1].x = fMaxX;
	pVertices[1].y = fMinY;
	pVertices[1].z = fMinZ;
	pVertices[2].x = fMinX;
	pVertices[2].y = fMinY;
	pVertices[2].z = fMaxZ;

	pVertices[3].x = fMinX;
	pVertices[3].y = fMinY;
	pVertices[3].z = fMaxZ;
	pVertices[4].x = fMaxX;
	pVertices[4].y = fMinY;
	pVertices[4].z = fMinZ;
	pVertices[5].x = fMaxX;
	pVertices[5].y = fMinY;
	pVertices[5].z = fMaxZ;

	pVertices[6].x = pVertices[9].x = pVertices[12].x = pVertices[15].x = 0.0f;
	pVertices[6].y = pVertices[9].y = pVertices[12].y = pVertices[15].y = fMaxY;
	pVertices[6].z = pVertices[9].z = pVertices[12].z = pVertices[15].z = 0.0f;

	pVertices[7].x = pVertices[17].x = fMaxX;
	pVertices[7].y = pVertices[17].y = fMinY;
	pVertices[7].z = pVertices[17].z = fMinZ;

	pVertices[8].x = pVertices[10].x = fMinX;
	pVertices[8].y = pVertices[10].y = fMinY;
	pVertices[8].z = pVertices[10].z = fMinZ;

	pVertices[11].x = pVertices[13].x = fMinX;
	pVertices[11].y = pVertices[13].y = fMinY;
	pVertices[11].z = pVertices[13].z = fMaxZ;

	pVertices[14].x = pVertices[16].x = fMaxX;
	pVertices[14].y = pVertices[16].y = fMinY;
	pVertices[14].z = pVertices[16].z = fMaxZ;

	D3DXVECTOR3 curNormal(0,0,0);

	for (int nCurVertex = 0; nCurVertex < nVertexCount; nCurVertex++)
	{
		if ((nCurVertex % 3) == 0 && (nCurVertex + 2) < nVertexCount)
		{
			CalcNormal(pVertices[nCurVertex],
			           pVertices[nCurVertex+1],
			           pVertices[nCurVertex+2],
			           curNormal);
		}
		pVertices[nCurVertex].nx = curNormal.x;
		pVertices[nCurVertex].ny = curNormal.y;
		pVertices[nCurVertex].nz = curNormal.z;
	}

	m_pMesh->_SetVertexBuffer(pVertices, nVertexCount);

	PrimitiveIndex nPrimIdx;
	hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLELIST, &nPrimIdx);

	if (SUCCEEDED(hr))
	{
		m_pMesh->SetPrimitiveVertexRange(nPrimIdx, 0, nVertexCount, 0);
	}

	return hr;
}

STDMETHODIMP CMeshEditor::MakeSphere(/* [in] */ VALUE3D radius, /* [in] */ short LatitudeCount, /* [in] */ short LongitudeCount)
{
	HRESULT hr = E_FAIL;

	if (m_pMesh == NULL)
	{
		return E_FAIL;
	}

	if (radius <= 0.0f)
	{
		return E_FAIL;
	}

	if (LatitudeCount < 2 || (LatitudeCount % 2) != 0)
	{
		return E_FAIL;
	}

	if (LongitudeCount < 3)
	{
		return E_FAIL;
	}

	// The sphere is divided into a series of quadrilaterals which are
	// subsequently divided into triangles. The quads array is treated
	// as a two-dimensional array in which rows represent latitude and
	// columns represent longitude. Each quad represents an area bounded
	// two lines of latitude and two lines of longitude. The rows of quads
	// at the north and south pole of the sphere are actually triangles
	// instead of quadrilaterals since they are attached to the pole.
	// IMPORTANT: Caution must be used when accessing the rows of quads
	// at each pole because one point in each quad is left uninitialized!
	// At the north pole, the topRight vertex is left uninitialized. At
	// the south pole, the bottomRight vertex is left uninitialized.

	int nNumQuadRows = LatitudeCount;
	int nNumQuadCols = LongitudeCount;
	int nNumTriangles = (LongitudeCount * 2) + ((LatitudeCount-2) * LongitudeCount * 2);
	int nVertexCount = nNumTriangles * 3;

	QUAD3D* quads;
	quads = new QUAD3D[nNumQuadRows * nNumQuadCols];

	struct AngleCalcs
	{
		double cosTheta;
		double sinTheta;
	};
	AngleCalcs* longitudeCalcs = new AngleCalcs[LongitudeCount];

	VALUE3D tuSpacing = 1.0f / LongitudeCount;
	VALUE3D tvSpacing = 1.0f / LatitudeCount;

	// The north pole and south pole of the sphere are on the Y axis.
	POINT3D northPole;
	POINT3D southPole;
	northPole.x = 0.0; northPole.y = radius; northPole.z = 0.0;
	southPole.x = 0.0; southPole.y = -radius; southPole.z = 0.0;

	// deltaY is the distance between lines of latitude in the sphere.
	VALUE3D deltaY = (radius * 2.0f) / LatitudeCount;

	// deltaTheta is the angle between lines of longitude.
	VALUE3D deltaTheta = 2.0f * g_PI / LongitudeCount;

	int i = 0;
	int j = 0;
	int lat1 = 0;
	int lat2 = 0;
	VALUE3D theta = g_2_PI;
	POINT3D p1;
	POINT3D p2;

	// Assign north and south pole coordinates to quads that
	// touch the poles (i.e. the first and last row of quads).
	// Quads along the north pole have their top left and top
	// right points filled in. Quads along the south pole have
	// their bottom left and bottom right points filled in.
	// Note that quads along the poles are actually just
	// triangles. The nullValue constant is used to indicate
	// that one of the points in the quad is not used.

	int offsetLastQuadRow = (LatitudeCount-1) * LongitudeCount;

	for (j = 0; j < LongitudeCount; j++)
	{
		quads[j].topLeft = northPole;
		quads[offsetLastQuadRow + j].bottomLeft = southPole;

		// Calculate and save the cos and sin for each angle
		theta -= deltaTheta;
		longitudeCalcs[j].cosTheta = cos(theta);
		longitudeCalcs[j].sinTheta = sin(theta);
	}

	p1.x = p2.x = radius;
	p1.y = p2.y = p1.z = p2.z = 0.0f;
	lat2 = (LatitudeCount / 2) * LongitudeCount;
	lat1 = lat2 - LongitudeCount;
	theta = g_2_PI;

	// Calculate points around equator. Fills in the bottom left and bottom right
	// points in the first row of quads above the equator and the top left and top
	// right points in the first row of quads below the equator.

	for (j = 0; j < LongitudeCount; ++j)
	{
		quads[lat1 + j].bottomRight = quads[lat2 + j].topRight = p1;

		if (j > 0)
		{
			quads[lat1 + (j-1)].bottomLeft = quads[lat2 + (j-1)].topLeft = p1;
		}
		theta -= deltaTheta;
		p1.x = static_cast<VALUE3D>(radius * longitudeCalcs[j].cosTheta);
		p1.z = static_cast<VALUE3D>(radius * longitudeCalcs[j].sinTheta);
	}
	quads[lat2 + (LongitudeCount-1)].topLeft = quads[lat1].bottomRight;
	quads[lat1 + (LongitudeCount-1)].bottomLeft = quads[lat1].bottomRight;

	// Loop through remaining latitudes and calculate their points. This algorithm
	// simultaneously loops through the northern and southern hemisphere.

	VALUE3D curX = radius;

	while (lat1 > 0)
	{
		// lat1 is northern hemisphere
		lat1 -= LongitudeCount;
		// lat2 is southern hemisphere
		lat2 += LongitudeCount;

		// Increment Y coordinate of both points
		p1.y += deltaY;
		p2.y -= deltaY;

		// Calculate distance along X axis from current Y axis position to
		// the surface of the sphere. The pythagorean theorem is used to
		// solve for X. The radius of the sphere is the hypotenuse and the
		// distance along the Y axis is one side. The value of X is the
		// side of the triangle that is solved for.
		curX = static_cast<VALUE3D>(sqrt((radius * radius) - (p1.y * p1.y)));

		p1.x = p2.x = curX;
		p1.z = p2.z = 0.0;

		theta = g_2_PI;

		for (j = 0; j < LongitudeCount; ++j)
		{
			quads[lat1 + j].bottomRight = quads[(lat1+LongitudeCount) + j].topRight = p1;
			quads[lat2 + j].topRight = quads[(lat2-LongitudeCount) + j].bottomRight = p2;
			if (j > 0)
			{
				quads[(lat1+LongitudeCount)+(j-1)].topLeft = p1;
				quads[(lat2-LongitudeCount)+(j-1)].bottomLeft = p2;
				quads[lat1+(j-1)].bottomLeft = p1;
				quads[lat2+(j-1)].topLeft = p2;
			}
			theta -= deltaTheta;
			p1.x = p2.x = static_cast<VALUE3D>(curX * longitudeCalcs[j].cosTheta);
			p1.z = p2.z = static_cast<VALUE3D>(curX * longitudeCalcs[j].sinTheta);
		}
		quads[(lat1+LongitudeCount)+(LongitudeCount-1)].topLeft = quads[lat1].bottomRight;
		quads[lat1+(LongitudeCount-1)].bottomLeft = quads[lat1].bottomRight;
		quads[lat2+(LongitudeCount-1)].topLeft = quads[lat2].topRight;
		quads[(lat2-LongitudeCount)+(LongitudeCount-1)].bottomLeft = quads[lat2].topRight;
	}

	// Now that all of the quadrilaterals that make up the sphere have been
	// computed, create a triangle list and populate it by dividing up each
	// quad into two triangles. The row of quads at the north and south pole
	// are an exception - they are triangles instead of quads.

	VERTEX3D* vertices = new VERTEX3D[nVertexCount];

	D3DXVECTOR3 faceNormal;
	UINT nCurLat = 0;
	UINT curQuad;
	UINT nCurVertex = 0;

	// Loop through triangles at north pole

	for (j = 0; j < LongitudeCount; j++)
	{
		curQuad = nCurLat + j;

		// Use bottom left triangle in quad

		CalcNormal(quads[curQuad].bottomLeft,
		           quads[curQuad].topLeft,
		           quads[curQuad].bottomRight,
		           faceNormal);

		vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].bottomLeft.x,
		                                             quads[curQuad].bottomLeft.y,
		                                             quads[curQuad].bottomLeft.z),
		                                             faceNormal,
		                                             1.0f - (j+1)*tuSpacing, tvSpacing);

		vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].topLeft.x,
		                                             quads[curQuad].topLeft.y,
		                                             quads[curQuad].topLeft.z),
		                                             faceNormal,
		                                             1.0f - (j+1)*tuSpacing, 0);

		vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].bottomRight.x,
		                                             quads[curQuad].bottomRight.y,
		                                             quads[curQuad].bottomRight.z),
		                                             faceNormal,
		                                             1.0f - j*tuSpacing, tvSpacing);
	}

	// Loop through each row of quadrilaterals between the two poles.

	for (i = 1; i < (LatitudeCount - 1); i++)
	{
		nCurLat = i * LongitudeCount;

		for (j = 0; j < LongitudeCount; j++)
		{
			curQuad = nCurLat + j;

			// Divide the quadrilateral into 2 triangles

			CalcNormal(quads[curQuad].bottomLeft,
			           quads[curQuad].topLeft,
			           quads[curQuad].bottomRight,
			           faceNormal);

			vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].bottomLeft.x,
			                                             quads[curQuad].bottomLeft.y,
			                                             quads[curQuad].bottomLeft.z),
			                                             faceNormal,
			                                             1.0f - (j+1)*tuSpacing, (i+1)*tvSpacing);

			vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].topLeft.x,
			                                             quads[curQuad].topLeft.y,
			                                             quads[curQuad].topLeft.z),
			                                             faceNormal,
			                                             1.0f - (j+1)*tuSpacing, i*tvSpacing);

			vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].bottomRight.x,
			                                             quads[curQuad].bottomRight.y,
			                                             quads[curQuad].bottomRight.z),
			                                             faceNormal,
			                                             1.0f - j*tuSpacing, (i+1)*tvSpacing);

			CalcNormal(quads[curQuad].bottomRight,
			           quads[curQuad].topLeft,
			           quads[curQuad].topRight,
			           faceNormal);

			vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].bottomRight.x,
			                                             quads[curQuad].bottomRight.y,
			                                             quads[curQuad].bottomRight.z),
			                                             faceNormal,
			                                             1.0f - j*tuSpacing, (i+1)*tvSpacing);

			vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].topLeft.x,
			                                             quads[curQuad].topLeft.y,
			                                             quads[curQuad].topLeft.z),
			                                             faceNormal,
			                                             1.0f - (j+1)*tuSpacing, i*tvSpacing);

			vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].topRight.x,
			                                             quads[curQuad].topRight.y,
			                                             quads[curQuad].topRight.z),
			                                             faceNormal,
			                                             1.0f - j*tuSpacing, i*tvSpacing);
		}
	}

	// Loop through triangles at south pole

	nCurLat = (LatitudeCount - 1) * LongitudeCount;

	for (j = 0; j < LongitudeCount; j++)
	{
		curQuad = nCurLat + j;

		CalcNormal(quads[curQuad].bottomLeft,
		           quads[curQuad].topLeft,
		           quads[curQuad].topRight,
		           faceNormal);

		vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].bottomLeft.x,
		                                             quads[curQuad].bottomLeft.y,
		                                             quads[curQuad].bottomLeft.z),
		                                             faceNormal,
		                                             1.0f - (j+1)*tuSpacing, 1.0f);

		vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].topLeft.x,
		                                             quads[curQuad].topLeft.y,
		                                             quads[curQuad].topLeft.z),
		                                             faceNormal,
		                                             1.0f - (j+1)*tuSpacing, 1.0f - tvSpacing);

		vertices[nCurVertex++] = VERTEX3D(D3DXVECTOR3(quads[curQuad].topRight.x,
		                                             quads[curQuad].topRight.y,
		                                             quads[curQuad].topRight.z),
		                                             faceNormal,
		                                             1.0f - j*tuSpacing, 1.0f - tvSpacing);
	}

	PrimitiveIndex nPrimIdx;
	hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLELIST, &nPrimIdx);

	if (SUCCEEDED(hr))
	{
		SelectPrimitive(nPrimIdx);
		m_pMesh->_AddVerticesToPrimitive(0, vertices, D3DFVF_VERTEX, nCurVertex);
	}

	delete[] vertices;
	delete[] longitudeCalcs;
	delete[] quads;

	return hr;
}

STDMETHODIMP CMeshEditor::MakeCylinder(/* [in] */ VALUE3D fRadius,
                                       /* [in] */ VALUE3D fHeight,
                                       /* [in] */ short nNumSlices)
{
	if (fRadius <= 0.0f || fHeight <= 0.0f || nNumSlices <= 0)
	{
		return E_INVALIDARG;
	}

	if (m_pMesh == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;

	FLOAT deltaTheta = 2.0f * g_PI / (FLOAT) nNumSlices;
	FLOAT yTop = fHeight / 2.0f;
	FLOAT yBottom = -yTop;

	int nEndsVertexCount = 3 + (nNumSlices - 1);
	int nMiddleVertexCount = (nNumSlices * 2) * 3;
	int nVertexCount = (nEndsVertexCount * 2) + nMiddleVertexCount;

	VERTEX3D* pVertices = new VERTEX3D[nVertexCount];
	ZeroMemory(pVertices, sizeof(VERTEX3D) * nVertexCount);
	VERTEX3D* pTopVertices = pVertices;
	VERTEX3D* pMiddleVertices = &pVertices[nEndsVertexCount];
	VERTEX3D* pBottomVertices = &pMiddleVertices[nMiddleVertexCount];

	pTopVertices[0].x = 0.0f;
	pTopVertices[0].y = yTop;
	pTopVertices[0].z = 0.0f;
	pTopVertices[1].x = fRadius;
	pTopVertices[1].y = yTop;
	pTopVertices[1].z = 0.0f;

	pBottomVertices[0].x = 0.0f;
	pBottomVertices[0].y = yBottom;
	pBottomVertices[0].z = 0.0f;
	pBottomVertices[1].x = fRadius;
	pBottomVertices[1].y = yBottom;
	pBottomVertices[1].z = 0.0f;

	pMiddleVertices[nMiddleVertexCount - 4] = pTopVertices[1];
	pMiddleVertices[nMiddleVertexCount - 2] = pTopVertices[1];
	pMiddleVertices[0] = pTopVertices[1];
	pMiddleVertices[nMiddleVertexCount - 3] = pBottomVertices[1];
	pMiddleVertices[nMiddleVertexCount - 1] = pBottomVertices[1];
	pMiddleVertices[1] = pBottomVertices[1];

	FLOAT thetaTop = g_2_PI;
	FLOAT thetaBottom = 0;

	int nCurSlice;
	int nCurVertex;
	int nCurMiddleTopVertex = 2;
	int nCurMiddleBottomVertex = nMiddleVertexCount - 3;
	int nCurEndVertex = 2;

	for (nCurVertex = 2; nCurVertex < nEndsVertexCount; nCurVertex++)
	{
		nCurSlice = nCurVertex - 2;

		thetaTop -= deltaTheta;
		thetaBottom += deltaTheta;

		pTopVertices[nCurEndVertex].x = static_cast<VALUE3D>(fRadius * cos(thetaTop));
		pTopVertices[nCurEndVertex].y = yTop;
		pTopVertices[nCurEndVertex].z = static_cast<VALUE3D>(fRadius * sin(thetaTop));

		pBottomVertices[nCurEndVertex].x = static_cast<VALUE3D>(fRadius * cos(thetaBottom));
		pBottomVertices[nCurEndVertex].y = yBottom;
		pBottomVertices[nCurEndVertex].z = static_cast<VALUE3D>(fRadius * sin(thetaBottom));

		if (nCurMiddleTopVertex < nMiddleVertexCount)
		{
			pMiddleVertices[nCurMiddleTopVertex] = pTopVertices[nCurEndVertex];
			nCurMiddleTopVertex += 2;
		}

		if (nCurMiddleTopVertex < nMiddleVertexCount)
		{
			pMiddleVertices[nCurMiddleTopVertex] = pTopVertices[nCurEndVertex];
			nCurMiddleTopVertex += 2;
		}

		if (nCurMiddleTopVertex < nMiddleVertexCount)
		{
			pMiddleVertices[nCurMiddleTopVertex] = pTopVertices[nCurEndVertex];
			nCurMiddleTopVertex += 2;
		}

		if (nCurMiddleBottomVertex >= 0)
		{
			pMiddleVertices[nCurMiddleBottomVertex] = pBottomVertices[nCurEndVertex];
			nCurMiddleBottomVertex -= 2;
		}

		if (nCurMiddleBottomVertex >= 0)
		{
			pMiddleVertices[nCurMiddleBottomVertex] = pBottomVertices[nCurEndVertex];
			nCurMiddleBottomVertex -= 2;
		}

		if (nCurMiddleBottomVertex >= 0)
		{
			pMiddleVertices[nCurMiddleBottomVertex] = pBottomVertices[nCurEndVertex];
			nCurMiddleBottomVertex -= 2;
		}

		nCurEndVertex++;
	}

	// Set vertex normals for ends

	for (nCurVertex = 0; nCurVertex < nEndsVertexCount; nCurVertex++)
	{
		pTopVertices[nCurVertex].nx = 0.0f;
		pTopVertices[nCurVertex].ny = 1.0f;
		pTopVertices[nCurVertex].nz = 0.0f;
		pBottomVertices[nCurVertex].nx = 0.0f;
		pBottomVertices[nCurVertex].ny = -1.0f;
		pBottomVertices[nCurVertex].nz = 0.0f;
	}

	D3DXVECTOR3 sliceNormal;

	for (nCurSlice = 0; nCurSlice < nNumSlices; nCurSlice++)
	{
		nCurVertex = nCurSlice * 6;

		CalcNormal(pMiddleVertices[nCurVertex],
			       pMiddleVertices[nCurVertex + 1],
			       pMiddleVertices[nCurVertex + 2],
			       sliceNormal);

		for (int i = 0; i < 6; i++)
		{
			pMiddleVertices[nCurVertex + i].nx = sliceNormal.x;
			pMiddleVertices[nCurVertex + i].ny = sliceNormal.y;
			pMiddleVertices[nCurVertex + i].nz = sliceNormal.z;
		}
	}

	m_pMesh->_SetVertexBuffer(pVertices, nVertexCount);

	PrimitiveIndex nTopPrim;
	PrimitiveIndex nMiddlePrim;
	PrimitiveIndex nBottomPrim;

	hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLEFAN, &nTopPrim);

	if (SUCCEEDED(hr))
	{
		m_pMesh->SetPrimitiveVertexRange(nTopPrim, 0, nEndsVertexCount, 0);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLESTRIP, &nMiddlePrim);

		if (SUCCEEDED(hr))
		{
			m_pMesh->SetPrimitiveVertexRange(nMiddlePrim, 0, nMiddleVertexCount, nEndsVertexCount);
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLEFAN, &nBottomPrim);

		if (SUCCEEDED(hr))
		{
			m_pMesh->SetPrimitiveVertexRange(nBottomPrim, 0, nEndsVertexCount, nEndsVertexCount + nMiddleVertexCount);
		}
	}

	return hr;
}

STDMETHODIMP CMeshEditor::MakeTube(/* [in] */ VALUE3D fInnerRadius,
                                   /* [in] */ VALUE3D fOuterRadius,
                                   /* [in] */ VALUE3D fHeight,
                                   /* [in] */ short nNumSlices)
{
	fInnerRadius;
	fOuterRadius;
	fHeight;
	nNumSlices;
	return E_NOTIMPL;
}

STDMETHODIMP CMeshEditor::MakeCone(/* [in] */ VALUE3D fBaseRadius,
                                   /* [in] */ VALUE3D fHeight,
                                   /* [in] */ short nNumSlices)
{
	if (fBaseRadius <= 0.0f || fHeight <= 0.0f || nNumSlices <= 0)
	{
		return E_INVALIDARG;
	}

	if (m_pMesh == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = E_FAIL;

	FLOAT deltaTheta = 2.0f * g_PI / (FLOAT) nNumSlices;
	FLOAT yTop = fHeight / 2.0f;
	FLOAT yBottom = -yTop;
	VERTEX3D topPoint;
	topPoint.x = 0.0f;
	topPoint.y = yTop;
	topPoint.z = 0.0f;

	int nBaseVertexCount = 3 + (nNumSlices - 1);
	int nBodyVertexCount = nNumSlices * 3;
	int nVertexCount = nBaseVertexCount + nBodyVertexCount;

	VERTEX3D* pVertices = new VERTEX3D[nVertexCount];
	ZeroMemory(pVertices, sizeof(VERTEX3D) * nVertexCount);
	VERTEX3D* pBaseVertices = pVertices;
	VERTEX3D* pBodyVertices = &pVertices[nBaseVertexCount];

	pBaseVertices[0].x = 0.0f;
	pBaseVertices[0].y = yBottom;
	pBaseVertices[0].z = 0.0f;
	pBaseVertices[1].x = fBaseRadius;
	pBaseVertices[1].y = yBottom;
	pBaseVertices[1].z = 0.0f;

	pBodyVertices[nBodyVertexCount - 2] = topPoint;
	pBodyVertices[nBodyVertexCount - 1] = pBaseVertices[1];
	pBodyVertices[0] = pBaseVertices[1];
	pBodyVertices[1] = topPoint;

	FLOAT thetaBase = 0;
	int nCurSlice;
	int nCurVertex;
	int nCurBodyVertex = 2;

	for (nCurVertex = 2; nCurVertex < nBaseVertexCount; nCurVertex++)
	{
		thetaBase += deltaTheta;

		pBaseVertices[nCurVertex].x = static_cast<VALUE3D>(fBaseRadius * cos(thetaBase));
		pBaseVertices[nCurVertex].y = yBottom;
		pBaseVertices[nCurVertex].z = static_cast<VALUE3D>(fBaseRadius * sin(thetaBase));

		if (nCurBodyVertex < nBodyVertexCount)
		{
			pBodyVertices[nCurBodyVertex] = pBaseVertices[nCurVertex];
			nCurBodyVertex++;
		}

		if (nCurBodyVertex < nBodyVertexCount)
		{
			pBodyVertices[nCurBodyVertex] = pBaseVertices[nCurVertex];
			nCurBodyVertex++;
		}

		if (nCurBodyVertex < nBodyVertexCount)
		{
			pBodyVertices[nCurBodyVertex] = topPoint;
			nCurBodyVertex++;
		}
	}

	// Set vertex normals for base

	for (nCurVertex = 0; nCurVertex < nBaseVertexCount; nCurVertex++)
	{
		pBaseVertices[nCurVertex].nx = 0.0f;
		pBaseVertices[nCurVertex].ny = -1.0f;
		pBaseVertices[nCurVertex].nz = 0.0f;
	}

	D3DXVECTOR3 sliceNormal;

	for (nCurSlice = 0; nCurSlice < nNumSlices; nCurSlice++)
	{
		nCurVertex = nCurSlice * 3;

		CalcNormal(pBodyVertices[nCurVertex],
			       pBodyVertices[nCurVertex + 1],
			       pBodyVertices[nCurVertex + 2],
			       sliceNormal);

		for (int i = 0; i < 3; i++)
		{
			pBodyVertices[nCurVertex + i].nx = sliceNormal.x;
			pBodyVertices[nCurVertex + i].ny = sliceNormal.y;
			pBodyVertices[nCurVertex + i].nz = sliceNormal.z;
		}
	}

	m_pMesh->_SetVertexBuffer(pVertices, nVertexCount);

	PrimitiveIndex nBasePrim;
	PrimitiveIndex nBodyPrim;

	hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLEFAN, &nBasePrim);

	if (SUCCEEDED(hr))
	{
		m_pMesh->SetPrimitiveVertexRange(nBasePrim, 0, nBaseVertexCount, 0);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pMesh->AppendPrimitive(SSCL_TRIANGLELIST, &nBodyPrim);

		if (SUCCEEDED(hr))
		{
			m_pMesh->SetPrimitiveVertexRange(nBodyPrim, 0, nBodyVertexCount, nBaseVertexCount);
		}
	}

	return hr;
}

STDMETHODIMP CMeshEditor::EndEdit()
{
	HRESULT hr = E_FAIL;

	if (m_pMesh != NULL)
	{
		hr = m_pMesh->RemoveUnusedVertices();
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	return hr;
}

};  // namespace ShapeShifter
