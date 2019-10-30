//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReader3DS.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MODELREADER3DS_H__
#define __MODELREADER3DS_H__

#pragma once

#include <vector>
#include <map>

#include "ShapeShifter.h"
#include "DynamicArray.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CModelReader3DS

class ATL_NO_VTABLE CModelReader3DS : public CComObjectRootEx<CComSingleThreadModel>,
                                      public CComCoClass<CModelReader3DS, &CLSID_ModelReader3DS>,
                                      public IDispatchImpl<IModelReader, &__uuidof(IModelReader), &LIBID_ShapeShifter>
{
// Embedded types
public:
	typedef CDynamicArray<FaceIndex, long, long, 12, 12> _FaceArray;
	typedef std::map< _bstr_t, _com_ptr_t<ITexture> > _TextureMap;

	struct _MaterialGroup
	{
		IRenderingContext* m_pContext;
		_FaceArray m_faces;
		PrimitiveIndex m_nPrimIdx;

		_MaterialGroup();
		~_MaterialGroup();
		_MaterialGroup(const _MaterialGroup& src);
		_MaterialGroup& operator=(const _MaterialGroup& src);
		void SetContext(IRenderingContext* pContext);
	};

	typedef std::vector<_MaterialGroup> _MaterialList;

	struct _MeshEntry
	{
		IMeshExPtr m_spMesh;
		_MaterialList m_materials;
		float m_matrix[4][3];

		_MeshEntry(IMeshEx* pMesh = NULL);
		~_MeshEntry();
		_MeshEntry(const _MeshEntry& src);
		_MeshEntry& operator=(const _MeshEntry& src);
		void SetMesh(IMeshEx* pMesh);
	};

	typedef std::map<_bstr_t, _MeshEntry> _MeshMap;

// Constructors/destructor
public:
	CModelReader3DS();
	virtual ~CModelReader3DS();

DECLARE_REGISTRY_RESOURCEID(IDR_MODEL_READER_3DS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CModelReader3DS)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IModelReader)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	FILE* m_pFile;
	IModel* m_pModel;
	unsigned short m_nCurChunkId;
	unsigned short m_nPrevChunkId;
	unsigned short m_nObjChunkId;
	_bstr_t m_strCurObjName;
	IShape* m_pCurShape;
	_MeshMap::iterator m_curMesh;
	_bstr_t m_strCurMaterialName;
	_bstr_t m_strCurTextureName;
	IMaterial* m_pCurMaterial;
	D3DLIGHT8 m_curLight;
	FLOAT m_curLightMultiplier;
	BOOL m_bCurLightOff;
	BOOL m_bCurLightAmbient;
	ITexture* m_pCurTexture;
	unsigned long m_kfstart;
	unsigned long m_kfend;
	unsigned long m_kfcurframe;
	int m_kf_count;
	_MeshMap m_meshes;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IModelReader interface

	STDMETHOD(Read)(/* [in] */ VARIANT varDataSource,
	                /* [in] */ IModel* pModel);

// Implementation
protected:
	HRESULT Open(VARIANT varDataSource);
	HRESULT Close();
	HRESULT ReadChunk(IModel* pModel);
	HRESULT ReadString(_bstr_t& strVal);
	HRESULT ReadFloat(float& fVal);
	HRESULT ReadShort(short& nVal);
	HRESULT ReadRGBColor(VALUE3D& red, VALUE3D& green, VALUE3D& blue);
	HRESULT ReadTrueColor(VALUE3D& red, VALUE3D& green, VALUE3D& blue);
	HRESULT ReadXYZ(VALUE3D& x, VALUE3D& y, VALUE3D& z);
	HRESULT ReadPointArray();
	HRESULT ReadFaceArray();
	HRESULT ReadMappingData();
	HRESULT ReadMaterial(IModel* pModel);
	HRESULT ReadSpotLight();
	HRESULT ReadMatrix();
	HRESULT ReadNodeHeader(IModel* pModel);
	HRESULT Read3DSVersion(short &version);
	HRESULT ReadPivots();
	HRESULT ReadRotationTrack();
	HRESULT ReadPositionTrack();
	HRESULT ReadScaleTrack();

	HRESULT SetObjectChunkId(const unsigned short nChunkId);

	HRESULT AssignColor(const VALUE3D& red, const VALUE3D& green, const VALUE3D& blue);

	HRESULT InitModel(IModel* pModel);
	HRESULT CreateShape(IModel* pModel);
	void FreeShape();
	HRESULT CreateMesh(IModel* pModel);
	HRESULT CreateMaterial(IModel* pModel);
	void FreeMaterial();
	HRESULT CreateTexture(IModel* pModel);
	void FreeTexture();

#if 0
	HRESULT GroupFaces(_MeshEntry& meshEntry);
#endif
};

};  // namespace ShapeShifter

#endif // #ifndef __MODELREADER3DS_H__
