//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ModelReader3DS.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <tchar.h>

#include "ModelReader3DS.h"
#include "Model.h"
#include "MeshEditor.h"
#include "Primitive.h"
#include "Light.h"

namespace ShapeShifter {

const LPOLESTR RENDERING_CONTEXT_NODENAME   = L"RenderingContext";
const LPOLESTR MATERIALS_NODENAME           = L"Materials";
const LPOLESTR MESHES_NODENAME              = L"Meshes";

//==============================================================================
// TODO: Put the following 2 functions into separate module

#if 0
static HRESULT CopyFaces(IPrimitive* pDest, IPrimitive* pSrc, const FaceIndex* pFaceArray, const UINT nFaceCount)
{
	if (pFaceArray == NULL || pDest == NULL || pSrc == NULL)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	for (UINT i = 0; hr == S_OK && i < nFaceCount; i++)
	{
		VertexHandle vert[3];

		hr = pSrc->GetFace(pFaceArray[i], &vert[0], &vert[1], &vert[2]);

		if (hr == S_OK)
		{
			hr = pDest->AddFace(vert[0], vert[1], vert[2], NULL);
		}
	}

	return hr;
}

static HRESULT DeleteFaces(IPrimitive* pPrim, const FaceIndex* pFaceArray, const UINT nFaceCount)
{
	if (pFaceArray == NULL || pPrim == NULL)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	for (UINT i = 0; hr == S_OK && i < nFaceCount; i++)
	{
		hr = pPrim->RemoveFace(pFaceArray[i]);
	}

	return hr;
}
#endif

//==============================================================================

// 0xxx group

#define NULL_CHUNK						0x0000
#define M3D_VERSION						0x0002
#define M3D_KFVERSION					0x0005
#define COLOR_F							0x0010
#define COLOR_24						0x0011
#define LIN_COLOR_24					0x0012
#define LIN_COLOR_F						0x0013
#define INT_PERCENTAGE					0x0030
#define FLOAT_PERCENTAGE				0x0031
#define MASTER_SCALE					0x0100

// 1xxx group

#define BIT_MAP							0x1100
#define USE_BIT_MAP						0x1101
#define SOLID_BGND						0x1200
#define USE_SOLID_BGND					0x1201
#define V_GRADIENT						0x1300
#define USE_V_GRADIENT					0x1301
#define LO_SHADOW_BIAS					0x1400
#define HI_SHADOW_BIAS					0x1410
#define SHADOW_MAP_SIZE					0x1420
#define SHADOW_SAMPLES					0x1430
#define SHADOW_RANGE					0x1440
#define SHADOW_FILTER					0x1450
#define RAY_BIAS						0x1460
#define O_CONSTS						0x1500

// 2xxx group

#define AMBIENT_LIGHT					0x2100
#define FOG								0x2200
#define USE_FOG							0x2201
#define FOG_BGND						0x2210
#define DISTANCE_CUE					0x2300
#define USE_DISTANCE_CUE				0x2301
#define LAYER_FOG						0x2302
#define USE_LAYER_FOG					0x2303
#define DCUE_BGND						0x2310
#define SMAGIC							0x2D2D
#define LMAGIC							0x2D3D

// 3xxx group

#define DEFAULT_VIEW					0x3000
#define VIEW_TOP						0x3010
#define VIEW_BOTTOM						0x3020
#define VIEW_LEFT						0x3030
#define VIEW_RIGHT						0x3040
#define VIEW_FRONT						0x3050
#define VIEW_BACK						0x3060
#define VIEW_USER						0x3070
#define VIEW_CAMERA						0x3080
#define VIEW_WINDOW						0x3090
#define MDATA							0x3D3D
#define MESH_VERSION					0x3D3E
#define MLIBMAGIC						0x3DAA
#define PRJMAGIC						0x3DC2
#define MATMAGIC						0x3DFF

// 4xxx group

#define NAMED_OBJECT					0x4000
#define OBJ_HIDDEN						0x4010
#define OBJ_VIS_LOFTER					0x4011
#define OBJ_DOESNT_CAST					0x4012
#define OBJ_MATTE						0x4013
#define OBJ_FAST						0x4014
#define OBJ_PROCEDURAL					0x4015
#define OBJ_FROZEN						0x4016
#define OBJ_DONT_RCVSHADOW				0x4017
#define N_TRI_OBJECT					0x4100
#define POINT_ARRAY						0x4110
#define POINT_FLAG_ARRAY				0x4111
#define FACE_ARRAY						0x4120
#define MSH_MAT_GROUP					0x4130
#define OLD_MAT_GROUP					0x4131
#define TEX_VERTS						0x4140
#define SMOOTH_GROUP					0x4150
#define MESH_MATRIX						0x4160
#define MESH_COLOR						0x4165
#define MESH_TEXTURE_INFO				0x4170
#define PROC_NAME						0x4181
#define PROC_DATA						0x4182
#define MSH_BOXMAP						0x4190
#define N_D_L_OLD						0x4400
#define N_CAM_OLD						0x4500
#define N_DIRECT_LIGHT					0x4600
#define DL_SPOTLIGHT					0x4610
#define DL_OFF							0x4620
#define DL_ATTENUATE					0x4625
#define DL_RAYSHAD						0x4627
#define DL_SHADOWED						0x4630
#define DL_LOCAL_SHADOW					0x4640
#define DL_LOCAL_SHADOW2				0x4641
#define DL_SEE_CONE						0x4650
#define DL_SPOT_RECTANGULAR				0x4651
#define DL_SPOT_OVERSHOOT				0x4652
#define DL_SPOT_PROJECTOR				0x4653
#define DL_EXCLUDE						0x4654
#define DL_RANGE						0x4655
#define DL_SPOT_ROLL					0x4656
#define DL_SPOT_ASPECT					0x4657
#define DL_SPOT_ASPECT					0x4657
#define DL_RAY_BIAS						0x4658
#define DL_INNER_RANGE					0x4659
#define DL_OUTER_RANGE					0x465A
#define DL_MULTIPLIER					0x465B
#define N_AMBIENT_LIGHT					0x4680
#define M3DMAGIC						0x4D4D

// Axxx group

#define MAT_NAME						0xA000
#define MAT_AMBIENT						0xA010
#define MAT_DIFFUSE						0xA020
#define MAT_SPECULAR					0xA030
#define MAT_SHININESS					0xA040
#define MAT_SHIN2PCT					0xA041
#define MAT_TRANSPARENCY				0xA050
#define MAT_XPFALL						0xA052
#define MAT_REFBLUR						0xA053
#define MAT_SELF_ILPCT					0xA084
#define MAT_WIRESIZE					0xA087
#define MAT_XPFALLIN					0xA08A
#define MAT_PHONGSOFT					0xA08C
#define MAT_SHADING						0xA100
#define MAT_TEXMAP						0xA200
#define MAT_MAPNAME						0xA300
#define MAT_MAP_TILING					0xA351
#define MAT_MAP_TEXBLUR					0xA353
#define MAT_ENTRY						0xAFFF

// Bxxx group

#define	KFDATA							0xB000
#define OBJECT_NODE_TAG					0xB002
#define KFSEG							0xB008
#define KFHDR							0xB00A
#define KFCURTIME						0xB009
#define NODE_HDR						0xB010
#define PIVOT							0xB013
#define POS_TRACK_TAG					0xB020
#define ROT_TRACK_TAG					0xB021
#define SCL_TRACK_TAG					0xB022
#define FOV_TRACK_TAG					0xB023
#define ROLL_TRACK_TAG					0xB024
#define COL_TRACK_TAG  					0xB025
#define MORPH_TRACK_TAG					0xB026
#define HOT_TRACK_TAG					0xB027
#define FALL_TRACK_TAG					0xB028
#define HIDE_TRACK_TAG					0xB029
#define NODE_ID							0xB030

/////////////////////////////////////////////////////////////////////////
// CModelReader3DS::_MaterialGroup

CModelReader3DS::_MaterialGroup::_MaterialGroup() :
	m_pContext(NULL),
	m_nPrimIdx(0)
{
}

CModelReader3DS::_MaterialGroup::~_MaterialGroup()
{
	if (m_pContext != NULL)
	{
		m_pContext->Release();
	}
}

CModelReader3DS::_MaterialGroup::_MaterialGroup(const _MaterialGroup& src) :
	m_pContext(NULL)
{
	*this = src;
}

CModelReader3DS::_MaterialGroup& CModelReader3DS::_MaterialGroup::operator=(const CModelReader3DS::_MaterialGroup& src)
{
	m_nPrimIdx = src.m_nPrimIdx;
	SetContext(src.m_pContext);
	if (m_faces.GetElementCount() > 0)
	{
		m_faces.CopyArray(src.m_faces, src.m_faces.GetElementCount());
	}
	return *this;
}

void CModelReader3DS::_MaterialGroup::SetContext(IRenderingContext* pContext)
{
	if (pContext != NULL)
	{
		pContext->AddRef();
	}
	if (m_pContext != NULL)
	{
		m_pContext->Release();
	}
	m_pContext = pContext;
}

/////////////////////////////////////////////////////////////////////////
// CModelReader3DS::_MeshEntry

CModelReader3DS::_MeshEntry::_MeshEntry(IMeshEx* pMesh) :
	m_spMesh(pMesh)
{
}

CModelReader3DS::_MeshEntry::~_MeshEntry()
{
}

CModelReader3DS::_MeshEntry::_MeshEntry(const _MeshEntry& src)
{
	*this = src;
}

CModelReader3DS::_MeshEntry& CModelReader3DS::_MeshEntry::operator=(const CModelReader3DS::_MeshEntry& src)
{
	m_spMesh = src.m_spMesh;

	_MaterialList::iterator itMat;
	m_materials.clear();

	for (itMat = m_materials.begin(); itMat != m_materials.end(); itMat++)
	{
		m_materials.push_back(*itMat);
	}

	return *this;
}

/////////////////////////////////////////////////////////////////////////
// CModelReader3DS

CModelReader3DS::CModelReader3DS() :
	m_pFile(NULL),
	m_pModel(NULL),
	m_pCurShape(NULL),
	m_pCurMaterial(NULL),
	m_pCurTexture(NULL)
{
}

CModelReader3DS::~CModelReader3DS()
{
	if (m_pModel != NULL)
	{
		m_pModel->Release();
	}

	FreeShape();
	FreeMaterial();
	FreeTexture();
	Close();
}

HRESULT CModelReader3DS::FinalConstruct()
{
	return S_OK;
}

HRESULT CModelReader3DS::Open(/* [in] */ VARIANT varDataSource)
{
	HRESULT hr = E_FAIL;

	if (m_pFile == NULL)
	{
		if (varDataSource.vt == VT_BSTR)
		{
			_bstr_t strFullName = GetFullPath(_bstr_t(varDataSource), _bstr_t("Models"));

			m_pFile = _tfopen(strFullName, _T("rb"));

			if (m_pFile != NULL)
			{
				// Read the first chunk and verify the file type
				hr = ReadChunk(NULL);

				if (m_nCurChunkId != M3DMAGIC)
				{
					Close();
					hr = E_UNEXPECTED;
				}
			}
		}
	}

	return hr;
}

STDMETHODIMP CModelReader3DS::Read(/* [in] */ VARIANT varDataSource,
                                   /* [in] */ IModel* pModel)
{
	if (pModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = Open(varDataSource);

	if (hr == S_OK)
	{
		hr = pModel->QueryInterface(IID_IModel, (LPVOID*) &m_pModel);

		if (SUCCEEDED(hr))
		{
			InitModel(m_pModel);

			m_nCurChunkId = NULL_CHUNK;
			m_nPrevChunkId = NULL_CHUNK;
			m_nObjChunkId = NULL_CHUNK;

			HRESULT hrLoop = S_OK;

			while (hrLoop == S_OK)
			{
				hrLoop = ReadChunk(m_pModel);
			}

#if 0
			// Loop through each mesh and group faces in the same
			// material groups into separate primitives.
			_MeshMap::iterator iMesh = m_meshes.begin();
			while (SUCCEEDED(hr) && iMesh != m_meshes.end())
			{
				GroupFaces(iMesh->second);
				iMesh++;
			}
#endif

			INode* pRenderingContextNode = NULL;
			if (SUCCEEDED(m_pModel->GetChildByName(_bstr_t(RENDERING_CONTEXT_NODENAME), &pRenderingContextNode)))
			{
				IRenderingContextPtr spRenderingContext = pRenderingContextNode;

				if (spRenderingContext != NULL)
				{
					spRenderingContext->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				}
			}

			m_pModel->Release();
			m_pModel = NULL;
		}

		Close();
	}

	return hr;
}

HRESULT CModelReader3DS::Close()
{
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	return S_OK;
}

#if 0
HRESULT CModelReader3DS::ReadChunkHeader(unsigned short &nChunkId, int& nChunkLen)
{
	fread(&nChunkId, sizeof(nChunkId), 1, m_pFile);

	if (feof(m_pFile))
	{
		return S_FALSE;
	}

	fread(&nChunkLen, sizeof(nChunkLen), 1, m_pFile);

	if (feof(m_pFile))
	{
		return S_FALSE;
	}

	return S_OK;
}
#endif

HRESULT CModelReader3DS::ReadChunk(IModel* pModel)
{
	int nChunkLen;
	bool process_data_flag = false;
	VALUE3D red, green, blue;

	fread(&m_nCurChunkId, sizeof(m_nCurChunkId), 1, m_pFile);

	if (feof(m_pFile))
	{
		return S_FALSE;
	}

	fread(&nChunkLen, sizeof(nChunkLen), 1, m_pFile);
	int data_length = nChunkLen - 6;

	HRESULT hr = S_OK;

	switch (m_nCurChunkId)
	{
	case COLOR_F:
	case LIN_COLOR_F:
		{
			hr = ReadRGBColor(red, green, blue);

			if (SUCCEEDED(hr))
			{
				AssignColor(red, green, blue);
			}

			process_data_flag = true;
		}
		break;

	case COLOR_24:
	case LIN_COLOR_24:
		{
			hr = ReadTrueColor(red, green, blue);

			if (SUCCEEDED(hr))
			{
				AssignColor(red, green, blue);
			}

			process_data_flag = true;
		}
		break;

	case N_TRI_OBJECT:
		{
			SetObjectChunkId(N_TRI_OBJECT);

			// Create a mesh
			hr = CreateMesh(pModel);
			process_data_flag = true;
		}
		break;

	case POINT_ARRAY:
		{
			// Read list of vertices into buffer
			hr = ReadPointArray();
			if (hr == S_OK)
			{
				process_data_flag = true;
			}
		}
		break;

	case FACE_ARRAY:
		{
			hr = ReadFaceArray();
			if (hr == S_OK)
			{
				process_data_flag = true;
			}
		}
		break;

	case TEX_VERTS:
		{
			hr = ReadMappingData();
			if (hr == S_OK)
			{
				process_data_flag = true;
			}
		}
		break;

	case MSH_MAT_GROUP:
		{
			hr = ReadMaterial(pModel);
			if (hr == S_OK)
			{
				process_data_flag = true;
			}
		}
		break;

	case MAT_ENTRY:
		{
			process_data_flag = true;
		}
		break;

	case MAT_NAME:
		{
			SetObjectChunkId(MAT_NAME);

			hr = ReadString(m_strCurMaterialName);
			if (hr == S_OK)
			{
				hr = CreateMaterial(pModel);
				process_data_flag = true;
			}
		}
		break;

	case MAT_TEXMAP:
		{
			process_data_flag = true;
		}
		break;

	case MAT_MAPNAME:
		{
			hr = ReadString(m_strCurTextureName);
			if (hr == S_OK)
			{
				CreateTexture(pModel);
				process_data_flag = true;
			}
		}
		break;

	case NAMED_OBJECT:
		{
			SetObjectChunkId(NULL_CHUNK);  // New object - don't know what type yet

			hr = ReadString(m_strCurObjName);
			if (hr == S_OK)
			{
				process_data_flag = true;
			}
		}
		break;

	case M3DMAGIC:
		process_data_flag = true;
		break;

	case MDATA:
		process_data_flag = true;
		break;

	case KFDATA:
		process_data_flag = true;
		break;

	case OBJECT_NODE_TAG:
		m_kf_count++;
		process_data_flag = true;
		break;

	case KFSEG:
		fread(&m_kfstart, sizeof(unsigned long), 1, m_pFile);
		fread(&m_kfend,	sizeof(unsigned long), 1, m_pFile);
		process_data_flag = true;
		break;

	case KFHDR:
		process_data_flag = false;
		break;

	case KFCURTIME:
		fread(&m_kfcurframe, sizeof(long), 1, m_pFile);
		process_data_flag = true;
		break;

	case PIVOT:
		hr = ReadPivots();
		if (hr == S_OK)
		{
			process_data_flag = true;
		}
		break;

	case POS_TRACK_TAG:  
		hr = ReadPositionTrack();
		if (hr == S_OK)
		{
			process_data_flag = true;
		}
		break;

	case ROT_TRACK_TAG:  
		hr = ReadRotationTrack();
		if (hr == S_OK)
		{
			process_data_flag = true;
		}
		break;

	case SCL_TRACK_TAG:
		hr = ReadScaleTrack();
		if (hr == S_OK)
		{
			process_data_flag = true;
		}
		break;

	case FOV_TRACK_TAG:	
		process_data_flag = false;
		break;

	case ROLL_TRACK_TAG:
		process_data_flag = false;
		break;

	case COL_TRACK_TAG:
		process_data_flag = false;
		break;

	case MORPH_TRACK_TAG:
		process_data_flag = false;
		break;

	case HOT_TRACK_TAG:
		process_data_flag = false;
		break;

	case FALL_TRACK_TAG:
		process_data_flag = false;
		break;

	case HIDE_TRACK_TAG:
		process_data_flag = false;
		break;

	case NODE_HDR:
		{
			hr = ReadNodeHeader(pModel);
			if (hr == S_OK)
			{
				process_data_flag = true;
			}
		}
		break;

	case NODE_ID:
		{
			short node_id;
			ReadShort(node_id);
			process_data_flag = true;
		}
		break;

	case M3D_VERSION:
		{
			short version;
			Read3DSVersion(version);
			process_data_flag = true;
		}
		break;

	case MESH_VERSION:
		process_data_flag = false;
		break;

	case INT_PERCENTAGE:
		process_data_flag = false;
		break;

	case MASTER_SCALE:
		{
			float master_scale;
			ReadFloat(master_scale);
			process_data_flag = true;
		}
		break;		

	case MESH_TEXTURE_INFO:
		process_data_flag = false;
		break;	

	case POINT_FLAG_ARRAY:
		process_data_flag = false;
		break;

	case SMOOTH_GROUP:
		process_data_flag = false;
		break;

	case MESH_MATRIX:
		hr = ReadMatrix();
		if (hr == S_OK)
		{
			process_data_flag = true;
		}
		break;

	case MESH_COLOR:
		{
			process_data_flag = false;
		}
		break;

	case MAT_AMBIENT:
		{
			process_data_flag = true;
		}
		break;

	case MAT_DIFFUSE:
		{
			process_data_flag = true;
		}
		break;

	case MAT_SPECULAR:
		{
			process_data_flag = true;
		}
		break;

	case MAT_SHININESS:
		process_data_flag = false;
		break;

	case MAT_SHIN2PCT:
		process_data_flag = false;
		break;

	case MAT_MAP_TILING:
		process_data_flag = false;
		break;
		
	case MAT_MAP_TEXBLUR:
		process_data_flag = false;
		break;

	case MAT_TRANSPARENCY:
		process_data_flag = false;
		break;
		
	case MAT_XPFALL:
		process_data_flag = false;
		break;

	case MAT_REFBLUR:
		process_data_flag = false;
		break;
		
	case MAT_SHADING:
		process_data_flag = false;
		break;

	case MAT_SELF_ILPCT:
		process_data_flag = false;
		break;
		
	case MAT_WIRESIZE:
		process_data_flag = false;
		break;

	case MAT_XPFALLIN:
		process_data_flag = false;
		break;
		
	case MAT_PHONGSOFT:
		process_data_flag = false;
		break;

	case AMBIENT_LIGHT:
		process_data_flag = true;
		break;

	case N_DIRECT_LIGHT:
		{
			// Set new object ID
			SetObjectChunkId(N_DIRECT_LIGHT);

			// Initialize light info buffer
			ZeroMemory(&m_curLight, sizeof(D3DLIGHT8));
			m_curLight.Type = D3DLIGHT_POINT;
			m_curLight.Attenuation0 = 1.0f;
			m_curLight.Attenuation1 = 0.0f;
			m_curLight.Attenuation2 = 0.0f;
			m_curLight.Falloff = 0.0f;
			m_bCurLightOff = FALSE;
			m_curLightMultiplier = 1.0f;
			m_bCurLightAmbient = false;

			// Read light position
			hr = ReadXYZ(m_curLight.Position.x, m_curLight.Position.y, m_curLight.Position.z);

			process_data_flag = true;
		}
		break;

	case N_AMBIENT_LIGHT:
		{
			m_bCurLightAmbient = true;
			process_data_flag = true;
		}
		break;

	case DL_INNER_RANGE:
		{
#if 0
			hr = ReadFloat(m_curLight.Range);
#else
			FLOAT fInnerRange;
			hr = ReadFloat(fInnerRange);
#endif
			process_data_flag = true;
		}
		break;

	case DL_OUTER_RANGE:
		{
			hr = ReadFloat(m_curLight.Range);
			process_data_flag = true;
		}
		break;

	case DL_RANGE:
		{
			hr = ReadFloat(m_curLight.Range);
			process_data_flag = true;
		}
		break;

	case DL_MULTIPLIER:
		{
			hr = ReadFloat(m_curLightMultiplier);
			process_data_flag = true;
		}
		break;

	case DL_OFF:
		{
			m_bCurLightOff = TRUE;
			process_data_flag = true;
		}
		break;

	case DL_SPOTLIGHT:
		{
			m_curLight.Type = D3DLIGHT_SPOT;
			D3DXVECTOR3 lightDir;
			hr = ReadXYZ(lightDir.x, lightDir.y, lightDir.z);
			lightDir.x = lightDir.x - m_curLight.Position.x;
			lightDir.y = lightDir.y - m_curLight.Position.y;
			lightDir.z = lightDir.z - m_curLight.Position.z;
			D3DXVec3Normalize(&lightDir, &lightDir);
			m_curLight.Direction = lightDir;
			float fHotspotAngle;
			ReadFloat(fHotspotAngle);
			m_curLight.Theta = D3DXToRadian(fHotspotAngle);
			float fFalloffAngle;
			ReadFloat(fFalloffAngle);
			m_curLight.Phi = D3DXToRadian(fFalloffAngle);
			process_data_flag = true;
		}
		break;
	}

	if (FAILED(hr))
	{
		return hr;
	}

	if (!process_data_flag)
	{
		fseek(m_pFile, data_length, SEEK_CUR);
		data_length = 0;
	}

	m_nPrevChunkId = m_nCurChunkId;

	return hr;
}

HRESULT CModelReader3DS::ReadString(_bstr_t& strVal)
{
	bool bDone = false;

	char buf[2];
	memset(buf, 0, sizeof(char) * 2);

	strVal = L"";

	while (!bDone)
	{
		fread(&buf[0], 1, 1, m_pFile);

		if (feof(m_pFile) || buf[0] == 0)
		{
			bDone = true;
		}
		else
		{
			strVal += buf;
		}
	}

	return S_OK;
}

HRESULT CModelReader3DS::ReadFloat(float& fVal)
{
	fread(&fVal, sizeof(float), 1, m_pFile);
	return S_OK;
}

HRESULT CModelReader3DS::ReadShort(short& nVal)
{
	fread(&nVal, sizeof(short), 1, m_pFile);
	return S_OK;
}

HRESULT CModelReader3DS::ReadRGBColor(VALUE3D& red, VALUE3D& green, VALUE3D& blue)
{
	float buf;

	fread(&buf, sizeof(float), 1, m_pFile);
	red = buf;

	fread(&buf, sizeof(float), 1, m_pFile);
	green = buf;

	fread(&buf, sizeof(float), 1, m_pFile);
	blue = buf;

	return S_OK;
}

HRESULT CModelReader3DS::ReadTrueColor(VALUE3D& red, VALUE3D& green, VALUE3D& blue)
{
	unsigned char buf[3];

	for (int i = 0; i < 3; i++)
	{
		fread(&buf[i], sizeof(unsigned char), 1, m_pFile);
	}

	red = (VALUE3D) buf[0] / 255.0f;
	green = (VALUE3D) buf[1] / 255.0f;
	blue = (VALUE3D) buf[2] / 255.0f;

	return S_OK;
}

HRESULT CModelReader3DS::ReadXYZ(VALUE3D& x, VALUE3D& y, VALUE3D& z)
{
	float buf;

	fread(&buf, sizeof(float), 1, m_pFile);
	x = buf;

	fread(&buf, sizeof(float), 1, m_pFile);
	z = buf;

	fread(&buf, sizeof(float), 1, m_pFile);
	y = buf;

	return S_OK;
}

HRESULT CModelReader3DS::ReadPointArray()
{
	HRESULT hr = S_OK;

	if (m_curMesh == m_meshes.end())
	{
		return E_FAIL;
	}
	_MeshEntry& curMesh = (*m_curMesh).second;

	unsigned short nNumPoints = 0;

	fread(&nNumPoints, sizeof(nNumPoints), 1, m_pFile);

	if (feof(m_pFile))
	{
		return E_FAIL;
	}

	VERTEX3D* pPointArray = new VERTEX3D[nNumPoints];

	for (int i = 0; i < (int)nNumPoints; i++)
	{
		float pt[3];

		for (int j = 0; j < 3; j++)
		{
			fread(&pt[j], sizeof(float), 1, m_pFile);
		}

		memset(&pPointArray[i], 0, sizeof(VERTEX3D));
		pPointArray[i].x = pt[0];
		pPointArray[i].y = pt[2];
		pPointArray[i].z = pt[1];
	}

	curMesh.m_spMesh->_SetVertexBuffer(pPointArray, nNumPoints);

	return hr;
}

HRESULT CModelReader3DS::ReadFaceArray()
{
	unsigned short nNumFaces;
	unsigned short nVisibleFaces;
	WORD* pVertIdx = NULL;
	VertexCount nNumVertices = 0;
	VERTEX3D* vert[3];

	if (m_curMesh == m_meshes.end())
	{
		return E_FAIL;
	}
	_MeshEntry& curMesh = (*m_curMesh).second;

	fread(&nNumFaces, sizeof(nNumFaces), 1, m_pFile);

	if (feof(m_pFile))
	{
		return E_FAIL;
	}

	// Create triangle list primitive to hold faces
	PrimitiveIndex nPrimIdx;

	HRESULT hr = curMesh.m_spMesh->AppendPrimitive(SSCL_TRIANGLELIST, &nPrimIdx);

	if (hr == S_OK)
	{
		// Allocate array to hold vertex indices
		nNumVertices = nNumFaces * 3;
		pVertIdx = new WORD[nNumVertices];

		curMesh.m_spMesh->_AttachVerticesToPrimitive(nPrimIdx, pVertIdx, nNumVertices);

		// Get pointer to vertex buffer in the mesh in
		// order to calculate vertex normals
		VERTEX3D* pVertexBuf = NULL;
		curMesh.m_spMesh->_GetVertexBuffer((LPVOID*) &pVertexBuf);

		// Read each face and calculate the vertex normals
		for (int i = 0; i < (int)nNumFaces; i++)
		{
			// Read each vertex index for the current face
			int nFaceOffset = i * 3;

			fread(&pVertIdx[nFaceOffset], sizeof(unsigned short), 1, m_pFile);
			vert[0] = &pVertexBuf[pVertIdx[nFaceOffset]];

			fread(&pVertIdx[nFaceOffset+1], sizeof(unsigned short), 1, m_pFile);
			vert[1] = &pVertexBuf[pVertIdx[nFaceOffset+1]];

			fread(&pVertIdx[nFaceOffset+2], sizeof(unsigned short), 1, m_pFile);
			vert[2] = &pVertexBuf[pVertIdx[nFaceOffset+2]];

			// The next short is described as "visible faces". Not sure
			// what it is supposed to be used for though.
			fread(&nVisibleFaces, sizeof(nVisibleFaces), 1, m_pFile);

			// Calculate the normal vector
			D3DVECTOR normalVector;
			CalcNormal(*vert[2], *vert[1], *vert[0], normalVector);

			for (int k = 0; k < 3; k++)
			{
				if (vert[k]->nx == 0 && vert[k]->ny == 0 && vert[k]->nz == 0)
				{
					// Normal vector not assigned to the vertex yet, so
					// go ahead and use this entry in the buffer
					vert[k]->nx = normalVector.x;
					vert[k]->ny = normalVector.y;
					vert[k]->nz = normalVector.z;
				}
#if 1
				else if (vert[k]->nx != normalVector.x || vert[k]->ny != normalVector.y || vert[k]->nz != normalVector.z)
				{
					// Must add a new entry to the vertex buffer
					VertexHandle newVert;
					curMesh.m_spMesh->_PutVertex(vert[k], D3DFVF_VERTEX, &newVert);
					pVertIdx[nFaceOffset+k] = (WORD) newVert;
				}
#endif
			}
		}

	}

	return hr;
}

HRESULT CModelReader3DS::ReadMappingData()
{
	unsigned short num_mapping_coords;
	float tu, tv;

	if (m_curMesh == m_meshes.end())
	{
		return E_FAIL;
	}
	_MeshEntry& curMesh = (*m_curMesh).second;

	VERTEX3D* pVertexBuf = NULL;
	curMesh.m_spMesh->_GetVertexBuffer((LPVOID*) &pVertexBuf);

	fread(&num_mapping_coords, sizeof(num_mapping_coords), 1, m_pFile);

	for (int i = 0; i < num_mapping_coords; i++)
	{
		fread(&tu, sizeof(float), 1, m_pFile);
		fread(&tv, sizeof(float), 1, m_pFile);

		pVertexBuf[i].tu = tu;
		pVertexBuf[i].tv = 1.0f - tv;
	}

	return S_OK;
}

HRESULT CModelReader3DS::ReadMaterial(IModel* pModel)
{
	HRESULT hr = E_FAIL;
	unsigned short nNumFaces;
	short nFaceIdx = 0;
	_bstr_t strMaterialName;
	INode* pContextNode = NULL;
	IRenderingContext* pContext = NULL;
	HRESULT hrMatFound = E_FAIL;

	if (m_curMesh == m_meshes.end())
	{
		return E_FAIL;
	}
	_MeshEntry& curMesh = (*m_curMesh).second;

	// Add a new material entry to the material list of the current mesh
	curMesh.m_materials.push_back(_MaterialGroup());
	UINT nMatIdx = curMesh.m_materials.size() - 1;
	_MaterialGroup& matEntry = curMesh.m_materials[nMatIdx];

	// Read name of material
	hr = ReadString(strMaterialName);

	if (SUCCEEDED(hr))
	{
		_bstr_t strFullName(_bstr_t(MATERIALS_NODENAME) + _bstr_t(".") + strMaterialName);

		// Lookup the material context in the model
		hrMatFound = pModel->GetChildByName(strFullName, &pContextNode);

		if (hrMatFound == S_OK)
		{
			// Get IRenderingContext interface from node found
			hrMatFound = pContextNode->QueryInterface(IID_IRenderingContext, (LPVOID*) &pContext);

			if (SUCCEEDED(hrMatFound) && pContext != NULL)
			{
				// Assign material point to material list entry
				matEntry.SetContext(pContext);
				pContext->Release();
				pContext = NULL;
			}

			pContextNode->Release();
			pContextNode = NULL;
		}
	}

	// Read number of faces this material applies to
	fread(&nNumFaces, sizeof(nNumFaces), 1, m_pFile);

	// Read the face index for each face this material applies to
	for (int i = 0; i < nNumFaces; i++)
	{
		fread(&nFaceIdx, sizeof(short), 1, m_pFile);
		matEntry.m_faces.AppendElement(nFaceIdx);
	}

	return hr;
}

HRESULT CModelReader3DS::ReadMatrix()
{
	if (m_curMesh == m_meshes.end())
	{
		return E_FAIL;
	}
	_MeshEntry& curMesh = (*m_curMesh).second;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fread(&curMesh.m_matrix[i][j], sizeof(float), 1, m_pFile);
		}
	}

	return S_OK;
}

HRESULT CModelReader3DS::ReadNodeHeader(IModel* pModel)
{
	HRESULT hr = E_FAIL;

	short flags1;
	short flags2;
	short hierarchy;

	hr = ReadString(m_strCurObjName);

	if (hr == S_OK)
	{
		hr = ReadShort(flags1);
	}

	if (hr == S_OK)
	{
		ReadShort(flags2);
	}

	if (hr == S_OK)
	{
		ReadShort(hierarchy);
	}

	if (hr == S_OK)
	{
		hr = CreateShape(pModel);
	}

	return S_OK;
}

HRESULT CModelReader3DS::Read3DSVersion(short &version)
{
	fread(&version, sizeof(short), 1, m_pFile);
	fread(&version, sizeof(short), 1, m_pFile);
	return S_OK;
}

HRESULT CModelReader3DS::ReadPivots()
{
	float x,y,z;
	
	fread(&x, sizeof(float), 1, m_pFile);
	fread(&y,  sizeof(float), 1, m_pFile);
	fread(&z,  sizeof(float), 1, m_pFile);

#if 0
	m_pData->oblist[m_pData->kf_count].pivot.x = x;
	m_pData->oblist[m_pData->kf_count].pivot.y = y;
	m_pData->oblist[m_pData->kf_count].pivot.z = z;
#endif

	return S_OK;
}

HRESULT CModelReader3DS::ReadRotationTrack()
{
	short framenum;
	long lunknown; 
	float rotation_rad;
	float axis_x;
	float axis_y;
	float axis_z;
	short pt_flags;
	short unknown;
	short rotkeys;

	fread(&pt_flags, sizeof(short), 1, m_pFile);

	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);

	fread(&rotkeys,	 sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);

	for (int i = 0; i < rotkeys; i++)
	{
		fread(&framenum,  sizeof(short), 1, m_pFile);
		fread(&lunknown,  sizeof(long), 1, m_pFile);
		fread(&rotation_rad,  sizeof(float), 1, m_pFile);
		fread(&axis_x,  sizeof(float), 1, m_pFile);
		fread(&axis_y,  sizeof(float), 1, m_pFile);
		fread(&axis_z,  sizeof(float), 1, m_pFile);

#if 0
		if (m_pCurShape != NULL && framenum == 0)
		{
			m_pCurShape->Rotate(rotation_rad, vtVECTOR3D(axis_x, axis_y, axis_z));
		}
#endif
	}

	return S_OK;
}

HRESULT CModelReader3DS::ReadPositionTrack()
{
	short framenum;
	long  lunknown;
	float pos_x;
	float pos_y;
	float pos_z;
	short pt_flags;
	short unknown;
	short poskeys;
	
	fread(&pt_flags, sizeof(short), 1, m_pFile);

	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);

	fread(&poskeys,	 sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);

	for (int i = 0; i < poskeys; i++)
	{
		fread(&framenum,  sizeof(short), 1, m_pFile);
		fread(&lunknown,  sizeof(long), 1, m_pFile);
		fread(&pos_x,  sizeof(float), 1, m_pFile);
		fread(&pos_y,  sizeof(float), 1, m_pFile);
		fread(&pos_z,  sizeof(float), 1, m_pFile);

#if 0
		if (m_pCurShape != NULL && framenum == 0)
		{
			m_pCurShape->Translate(pos_x, pos_y, pos_z);
		}
#endif
	}

	return S_OK;
}
	
HRESULT CModelReader3DS::ReadScaleTrack()
{
	short framenum;
	long lunknown;		
	float scale_x;
	float scale_y;
	float scale_z;
	short pt_flags;
	short unknown;
	short sclkeys;

	fread(&pt_flags, sizeof(short), 1, m_pFile);

	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);

	fread(&sclkeys,	 sizeof(short), 1, m_pFile);
	fread(&unknown,  sizeof(short), 1, m_pFile);

	for (int i = 0; i < sclkeys; i++)
	{
		fread(&framenum,  sizeof(short), 1, m_pFile);
		fread(&lunknown,  sizeof(long), 1, m_pFile);
		
		fread(&scale_x,  sizeof(float), 1, m_pFile);
		fread(&scale_y,  sizeof(float), 1, m_pFile);
		fread(&scale_z,  sizeof(float), 1, m_pFile);

#if 0
		if (m_pCurShape != NULL && framenum == 0)
		{
			m_pCurShape->Scale(scale_x, scale_y, scale_z);
		}
#endif
	}

	return S_OK;
}

HRESULT CModelReader3DS::SetObjectChunkId(const unsigned short nChunkId)
{
	HRESULT hr = S_OK;

	if (m_nObjChunkId == N_DIRECT_LIGHT)
	{
		// Create current light before going to next object
		if (m_curLight.Type == D3DLIGHT_POINT)
		{
			ILight* pLight = NULL;
			hr = CPointLight::_CreatorClass::CreateInstance(NULL, IID_ILight, (LPVOID*) &pLight);
			if (SUCCEEDED(hr))
			{
				pLight->put_Name(m_strCurObjName);
				pLight->SetPosition(m_curLight.Position.x, m_curLight.Position.y, m_curLight.Position.z);
				pLight->SetDiffuseColor(m_curLight.Diffuse.r, m_curLight.Diffuse.g, m_curLight.Diffuse.b);
				pLight->SetAmbientColor(m_curLight.Ambient.r, m_curLight.Ambient.g, m_curLight.Ambient.b);
				pLight->SetSpecularColor(m_curLight.Specular.r, m_curLight.Specular.g, m_curLight.Specular.b);
				pLight->put_ConstantAttenuation(m_curLight.Attenuation0);
				pLight->put_LinearAttenuation(m_curLight.Attenuation1);
				pLight->put_QuadraticAttenuation(m_curLight.Attenuation2);
				pLight->put_Range(m_curLight.Range);

				if (m_bCurLightOff)
				{
					pLight->put_Enabled(FALSE);
				}
				else
				{
					pLight->put_Enabled(TRUE);
				}

				hr = m_pModel->AppendChild(pLight);

				pLight->Release();
			}
		}
		else if (m_curLight.Type == D3DLIGHT_SPOT)
		{
			ISpotLight* pLight = NULL;
			hr = CSpotLight::_CreatorClass::CreateInstance(NULL, IID_ISpotLight, (LPVOID*) &pLight);
			if (SUCCEEDED(hr))
			{
				pLight->put_Name(m_strCurObjName);

				pLight->SetPosition(m_curLight.Position.x, m_curLight.Position.y, m_curLight.Position.z);
				pLight->SetDirection(m_curLight.Direction.x, m_curLight.Direction.y, m_curLight.Direction.z);
				pLight->SetDiffuseColor(m_curLight.Diffuse.r, m_curLight.Diffuse.g, m_curLight.Diffuse.b);
				pLight->SetAmbientColor(m_curLight.Ambient.r, m_curLight.Ambient.g, m_curLight.Ambient.b);
				pLight->SetSpecularColor(m_curLight.Specular.r, m_curLight.Specular.g, m_curLight.Specular.b);
				pLight->put_ConstantAttenuation(m_curLight.Attenuation0);
				pLight->put_LinearAttenuation(m_curLight.Attenuation1);
				pLight->put_QuadraticAttenuation(m_curLight.Attenuation2);
				pLight->put_InnerSpotlightConeAngle(m_curLight.Theta);
				pLight->put_OuterSpotlightConeAngle(m_curLight.Phi);
#if 1
				pLight->put_Range(m_curLight.Range);
#else
				pLight->put_Range(1000.0f); // force range to be longer
#endif

				if (m_bCurLightOff)
				{
					pLight->put_Enabled(FALSE);
				}
				else
				{
					pLight->put_Enabled(TRUE);
				}

				hr = m_pModel->AppendChild(pLight);

				pLight->Release();
			}
		}
	}

	m_nObjChunkId = nChunkId;

	return hr;
}

HRESULT CModelReader3DS::AssignColor(const VALUE3D& red, const VALUE3D& green, const VALUE3D& blue)
{
	if (m_pCurMaterial == NULL)
	{
		return E_FAIL;
	}

	if (m_nPrevChunkId == AMBIENT_LIGHT)
	{
		m_pModel->SetAmbientLight(red, green, blue, 1.0f);
	}
	else if (m_nObjChunkId == MAT_NAME)
	{
		switch (m_nPrevChunkId)
		{
		case MAT_AMBIENT:
			m_pCurMaterial->SetAmbientColor(red, green, blue, 1.0f);
			break;

		case MAT_DIFFUSE:
			m_pCurMaterial->SetDiffuseColor(red, green, blue, 1.0f);
			break;

		case MAT_SPECULAR:
			m_pCurMaterial->SetSpecularColor(red, green, blue, 1.0f);
			break;

		default:
			return E_FAIL;
		}
	}
	else if (m_nObjChunkId == N_DIRECT_LIGHT)
	{
		if (m_bCurLightAmbient)
		{
			m_curLight.Ambient.r = red;
			m_curLight.Ambient.g = green;
			m_curLight.Ambient.b = blue;
		}
		else
		{
			m_curLight.Diffuse.r = red;
			m_curLight.Diffuse.g = green;
			m_curLight.Diffuse.b = blue;

			m_curLight.Specular.r = red;
			m_curLight.Specular.g = green;
			m_curLight.Specular.b = blue;
		}
	}

	return S_OK;
}

HRESULT CModelReader3DS::InitModel(IModel* pModel)
{
	HRESULT hr = E_FAIL;

	INode* pNode = NULL;

	hr = pModel->CreateNode(_bstr_t(MATERIALS_NODENAME),
	                        _bstr_t("ShapeShifter.Namespace"),
							NULL,
							&pNode);

	if (hr == S_OK)
	{
		pNode->Release();

		hr = pModel->CreateNode(_bstr_t(MESHES_NODENAME),
		                        _bstr_t("ShapeShifter.Namespace"),
		                        NULL,
		                        &pNode);
	}

	if (hr == S_OK)
	{
		pNode->Release();

		hr = pModel->CreateNode(_bstr_t(RENDERING_CONTEXT_NODENAME),
		                        _bstr_t("ShapeShifter.RenderingContext"),
		                        NULL,
		                        &pNode);
	}

	if (hr == S_OK)
	{
		pNode->Release();
	}

	return hr;
}

HRESULT CModelReader3DS::CreateShape(IModel* pModel)
{
	INode* pShapeNode = NULL;

	// Create new shape node in the model
	HRESULT hr = pModel->CreateNode(m_strCurObjName,
		                            _bstr_t("ShapeShifter.CompositeShape"),
		                            NULL,
		                            &pShapeNode);

	if (SUCCEEDED(hr))
	{
		// Query the new node for IShape
		FreeShape();
		hr = pShapeNode->QueryInterface(IID_IShape, (LPVOID*) &m_pCurShape);
		pShapeNode->Release();
	}

	if (SUCCEEDED(hr))
	{
		// Lookup the mesh with the matching object name
		_MeshMap::iterator itMesh = m_meshes.find(m_strCurObjName);

		if (itMesh != m_meshes.end())
		{
			_MeshEntry& curMesh = (*itMesh).second;

			// Assign the corresponding mesh to the new shape
			m_pCurShape->putref_Mesh(curMesh.m_spMesh);

			// Assign materials and textures to the new shape.
			_MaterialList::iterator itMat;
			for (itMat = curMesh.m_materials.begin(); itMat != curMesh.m_materials.end(); itMat++)
			{
				m_pCurShape->SetContext((*itMat).m_nPrimIdx, (*itMat).m_pContext); 
			}
		}
	}

	return hr;
}

void CModelReader3DS::FreeShape()
{
	if (m_pCurShape != NULL)
	{
		m_pCurShape->Release();
		m_pCurShape = NULL;
	}
}

HRESULT CModelReader3DS::CreateMesh(IModel* pModel)
{
	INode* pMeshNode = NULL;
	IMeshEx* pMesh = NULL;

	// Create a new mesh in the Meshes namespace using the current object name
	HRESULT hr = pModel->CreateNode(m_strCurObjName,
		                            _bstr_t("ShapeShifter.Mesh"),
		                            _bstr_t(MESHES_NODENAME),
		                            &pMeshNode);

	if (SUCCEEDED(hr))
	{
		hr = pMeshNode->QueryInterface(IID_IMeshEx, (LPVOID*) &pMesh);

		if (SUCCEEDED(hr))
		{
			// Add an entry to the mesh table for the new mesh
			m_meshes[m_strCurObjName] = _MeshEntry(pMesh);
			m_curMesh = m_meshes.find(m_strCurObjName);
		}

		pMeshNode->Release();
	}

	return hr;
}

HRESULT CModelReader3DS::CreateMaterial(IModel* pModel)
{
	INode* pContextNode = NULL;
	INode* pMaterialNode = NULL;

	// Create a rendering context to contain the material and
	// any textures that are found

	HRESULT hr = pModel->CreateNode(m_strCurMaterialName,
		                            _bstr_t("ShapeShifter.RenderingContext"),
		                            _bstr_t(MATERIALS_NODENAME),
		                            &pContextNode);

	if (SUCCEEDED(hr))
	{
		_bstr_t strContextName(_bstr_t(MATERIALS_NODENAME) + _bstr_t(".") + m_strCurMaterialName);

		// Create a material as a child of the context

		hr = pModel->CreateNode(_bstr_t("Material"),
		                        _bstr_t("ShapeShifter.Material"),
		                        strContextName,
		                        &pMaterialNode);

		if (SUCCEEDED(hr))
		{
			// Set pointer to current material so values read
			// in subsequent chunks can assigned to it

			FreeMaterial();
			hr = pMaterialNode->QueryInterface(IID_IMaterial, (LPVOID*) &m_pCurMaterial);
			pMaterialNode->Release();
		}

		pContextNode->Release();
	}

	return hr;
}

void CModelReader3DS::FreeMaterial()
{
	if (m_pCurMaterial != NULL)
	{
		m_pCurMaterial->Release();
		m_pCurMaterial = NULL;
	}
}

HRESULT CModelReader3DS::CreateTexture(IModel* pModel)
{
	INode* pTextureNode = NULL;

	_bstr_t strContextName;
	strContextName = L"Materials.";
	strContextName += m_strCurMaterialName;

	HRESULT hr = pModel->CreateNode(_bstr_t("Texture"),
		                            _bstr_t("ShapeShifter.Texture"),
		                            strContextName,
		                            &pTextureNode);

	if (SUCCEEDED(hr))
	{
		FreeTexture();
		hr = pTextureNode->QueryInterface(IID_ITexture, (LPVOID*) &m_pCurTexture);
		pTextureNode->Release();

		if (SUCCEEDED(hr) && m_pCurTexture != NULL)
		{
			hr = m_pCurTexture->put_FileName(m_strCurTextureName);

			if (hr != S_OK)
			{
				m_pCurTexture->Release();
				m_pCurTexture = NULL;
			}

////////////////////////// TODO: temporary hack /////////////////////////////
#if 1
			if (m_pCurMaterial != NULL)
			{
				m_pCurMaterial->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
			}
#endif
////////////////////////// TODO: temporary hack /////////////////////////////
		}
	}

	return hr;
}

void CModelReader3DS::FreeTexture()
{
	if (m_pCurTexture != NULL)
	{
		m_pCurTexture->Release();
		m_pCurTexture = NULL;
	}
}

#if 0
HRESULT CModelReader3DS::GroupFaces(_MeshEntry& meshEntry)
{
	HRESULT hr = E_FAIL;

	IMesh* pMesh = meshEntry.m_spMesh;

	if (pMesh == NULL)
	{
		return E_UNEXPECTED;
	}

	IPrimitive* pSrcPrimitive = NULL;

	if (FAILED(pMesh->GetPrimitive(0, &pSrcPrimitive)))
	{
		return E_UNEXPECTED;
	}

	if (meshEntry.m_materials.size() <= 1)
	{
		return S_OK;
	}

	pMesh->RemovePrimitive(0);

	// Create a separate primitive for each material group in the mesh

	_FaceArray facesMoved;

	_MaterialList::iterator iMat = meshEntry.m_materials.begin();

	while (iMat != meshEntry.m_materials.end())
	{
		// Create a new primitive for the material group
		IPrimitive* pNewPrim = NULL;
		hr = CTriangleList::_CreatorClass::CreateInstance(NULL, IID_IPrimitive, (LPVOID*) &pNewPrim);

		if (SUCCEEDED(hr))
		{
			// Copy the faces belonging to the material group into the new primitive
			hr = CopyFaces(pNewPrim, pSrcPrimitive, iMat->m_faces, iMat->m_faces.GetElementCount());

			if (hr == S_OK)
			{
				facesMoved.AppendArray(iMat->m_faces, iMat->m_faces.GetElementCount());

				// Append the new primitive to the mesh
				hr = pMesh->AppendPrimitive(pNewPrim, NULL);
			}

			pNewPrim->Release();
		}

		iMat++;
	}

	DeleteFaces(pSrcPrimitive, facesMoved, facesMoved.GetElementCount());

	VertexCount nVerticesRemaining;
	pSrcPrimitive->GetVertexCount(&nVerticesRemaining);

	if (nVerticesRemaining > 0)
	{
		pMesh->AppendPrimitive(pSrcPrimitive, NULL);
	}

	pSrcPrimitive->Release();

	return S_OK;
}
#endif

};  // namespace ShapeShifter
