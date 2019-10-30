//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: FaceArray.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "FaceArray.h"

namespace ShapeShifter {

CFaceArray::CFaceArray()
{
}

CFaceArray::~CFaceArray()
{
}

STDMETHODIMP CFaceArray::GetFaceCount(/* [out,retval] */ long* pNumFaces)
{
	if (pNumFaces != NULL)
	{
		*pNumFaces = m_faces.size();
		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP CFaceArray::GetFacePrimitiveIndex(/* [in] */ long nArrayIdx,
                                               /* [out,retval] */ PrimitiveIndex* pPrimIdx)
{
	HRESULT hr = E_FAIL;

	if (pPrimIdx != NULL)
	{
		if (nArrayIdx >= 0 && (size_t) nArrayIdx < m_faces.size())
		{
			*pPrimIdx = m_faces[nArrayIdx].m_nPrimIdx;
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CFaceArray::GetFaceIndex(/* [in] */ long nArrayIdx,
                                      /* [out,retval] */ FaceIndex* pFaceIdx)
{
	HRESULT hr = E_FAIL;

	if (pFaceIdx != NULL)
	{
		if (nArrayIdx >= 0 && (size_t) nArrayIdx < m_faces.size())
		{
			*pFaceIdx = m_faces[nArrayIdx].m_nFaceIdx;
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CFaceArray::AddFace(/* [in] */ PrimitiveIndex nPrimIdx,
                                 /* [in] */ FaceIndex nFaceIdx)
{
	m_faces.push_back(_Face(nPrimIdx, nFaceIdx));
	return S_OK;
}

STDMETHODIMP CFaceArray::Clear()
{
	m_faces.clear();
	return S_OK;
}

};  // namespace ShapeShifter

