//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Model.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MODEL_H__
#define __MODEL_H__

#pragma once

#include <vector>
#include "ShapeShifter.h"
#include "ShapeShifterCP.h"
#include "Node.h"
#include "NodeCollection.h"
#include "Visual.h"
#include "Property.h"
#include "PersistXML.h"
#include "MeasurementScale.h"
#include "BSPTree.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IModelImpl

template <class base_class>
class IModelImpl : public base_class
{
// Constructors/destructor
public:
	IModelImpl() :
		m_bounds(-1000, 1000, 1000, -1000, -1000, 1000),
		m_pMeasurementScale(NULL),
		m_bCompiled(false),
		m_pBSPTree(NULL),
		m_bBSPTreeEnabled(false),
		m_nEventLocks(0)
	{
		m_nStyles = StyleFlag::NONE;
		m_ambientLight.r = 1.0f;
		m_ambientLight.g = 1.0f;
		m_ambientLight.b = 1.0f;
		m_ambientLight.a = 1.0f;
	}

	virtual ~IModelImpl()
	{
		if (m_pBSPTree != NULL)
		{
			m_pBSPTree->Release();
		}

		if (m_pMeasurementScale != NULL)
		{
			m_pMeasurementScale->Release();
		}
	}

// Embedded types
public:
	typedef std::vector<IContextObjectPtr> _ContextObjectVector;
	typedef std::vector<IRenderDX8Ptr> _RenderVector;

// Attributes
protected:
	BOX3D m_bounds;
	_ContextObjectVector m_contextNodes;
	_RenderVector m_renderNodes;
	D3DCOLORVALUE m_ambientLight;
	IMeasurementScale* m_pMeasurementScale;
	bool m_bCompiled;
	IBSPTree* m_pBSPTree;
	bool m_bBSPTreeEnabled;
	UINT m_nEventLocks;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IVisual interface

	STDMETHOD(get_Left)(/*[out, retval]*/ VALUE3D *pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(put_Left)(/*[in]*/ VALUE3D newVal)
	{
		newVal;
		return S_OK;
	}

	STDMETHOD(get_Right)(/*[out, retval]*/ VALUE3D *pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(put_Right)(/*[in]*/ VALUE3D newVal)
	{
		newVal;
		return S_OK;
	}

	STDMETHOD(get_Top)(/*[out, retval]*/ VALUE3D *pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(put_Top)(/*[in]*/ VALUE3D newVal)
	{
		newVal;
		return S_OK;
	}

	STDMETHOD(get_Bottom)(/*[out, retval]*/ VALUE3D *pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(put_Bottom)(/*[in]*/ VALUE3D newVal)
	{
		newVal;
		return S_OK;
	}

	STDMETHOD(get_Front)(/*[out, retval]*/ VALUE3D *pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(put_Front)(/*[in]*/ VALUE3D newVal)
	{
		newVal;
		return S_OK;
	}

	STDMETHOD(get_Back)(/*[out, retval]*/ VALUE3D *pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(put_Back)(/*[in]*/ VALUE3D newVal)
	{
		newVal;
		return S_OK;
	}

	STDMETHOD(get_OriginX)(/* [out,retval] */ VALUE3D* pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(get_OriginY)(/* [out,retval] */ VALUE3D* pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(get_OriginZ)(/* [out,retval] */ VALUE3D* pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(get_Radius)(/* [out,retval] */ VALUE3D* pVal)
	{
		*pVal = 0.0f;
		return S_OK;
	}

	STDMETHOD(GetOrigin)(/* [out,retval] */ VARIANT* pVarPoint)
	{
		pVarPoint;
		return E_NOTIMPL;
	}

	STDMETHOD(GetBoundingBox)(/* [out,retval] */ VARIANT* pVarBox)
	{
		if (pVarBox == NULL)
		{
			return E_POINTER;
		}
		*pVarBox = vtBOX3D(m_bounds);
		return S_OK;
	}

	STDMETHOD(SetBoundingBox)(/* [in] */ VARIANT varBox)
	{
		m_bounds = vtBOX3D(varBox);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////
	// IModel interface

	STDMETHOD(CreateNode)(/* [in] */ BSTR strName,
	                      /* [in] */ BSTR strProgID,
	                      /* [in] */ BSTR strParent,
	                      /* [out,retval] */ INode** ppObj)
	{
		HRESULT hr = E_FAIL;
		CLSID clsid;
		ICompositeNode* pParent = NULL;

		ATLTRACE2(atlTraceUser, 2, "\nCreating node \"%s\" in parent \"%s\"", (LPCTSTR) _bstr_t(strName), (LPCTSTR) _bstr_t(strParent));

		// If parent node name given then find it and use it as parent
		// instead of the model
		if (strParent != NULL && ::SysStringLen(strParent) > 0)
		{
			INode* pMatchingNode = NULL;
			if (GetChildByName(strParent, &pMatchingNode) == S_OK)
			{
				hr = pMatchingNode->QueryInterface(IID_ICompositeNode, (LPVOID*) &pParent);
				pMatchingNode->Release();
			}
		}
		else
		{
			hr = static_cast<ICompositeNode*>(this)->QueryInterface(IID_ICompositeNode, (LPVOID*) &pParent);
		}

		if (SUCCEEDED(hr))
		{
			// Translate prog ID into a GUID
			hr = CLSIDFromProgID(strProgID, &clsid);

			if (SUCCEEDED(hr))
			{
				// Create the object
				hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_INode, (LPVOID*) ppObj);

				if (SUCCEEDED(hr))
				{
					// Assign the object a name

					bool bGenerateName = false;

					if (strName != NULL && ::SysStringLen(strName) > 0)
					{
						(*ppObj)->put_Name(strName);

						if (DuplicateName(*ppObj))
						{
							bGenerateName = true;
						}
					}
					else
					{
						bGenerateName = true;
					}

					if (bGenerateName)
					{
						// No name given. Generate a unique name for the object.
						BSTR strNewName = MakeUniqueName(*ppObj);

						if (strNewName != NULL)
						{
							(*ppObj)->put_Name(strNewName);
							::SysFreeString(strNewName);
							ATLTRACE2(atlTraceUser, 2, "\n\tNode name \"%s\" generated automatically", (LPCTSTR) _bstr_t(strNewName));
						}
					}

					// Append the object to the parent node
					hr = pParent->AppendChild(*ppObj);
#ifdef _DEBUG
					if (SUCCEEDED(hr))
					{
						ATLTRACE2(atlTraceUser, 2, "\n\tNode added successfully");
					}
					else
					{
						ATLTRACE2(atlTraceUser, 2, "\n\tUnable to add new node");
					}
#endif
				}
			}
		}

		if (pParent != NULL)
		{
			pParent->Release();
		}

		return hr;
	}

	STDMETHOD(InsertNode)(/* [in] */ INode* pNode,
	                      /* [in] */ BSTR strParent,
	                      /* [in] */ long nChildIndex)
	{
		HRESULT hr = S_OK;
		ICompositeNode* pParent = NULL;
		BSTR strNodeName = NULL;

		// If parent node name given then find it and use it as parent
		// instead of the model
		if (strParent != NULL && ::SysStringLen(strParent) > 0)
		{
			INode* pMatchingNode = NULL;
			if (GetChildByName(strParent, &pMatchingNode) == S_OK)
			{
				hr = pMatchingNode->QueryInterface(IID_ICompositeNode, (LPVOID*) &pParent);
				pMatchingNode->Release();
			}
		}
		else
		{
			hr = static_cast<ICompositeNode*>(this)->QueryInterface(IID_ICompositeNode, (LPVOID*) &pParent);
		}

		if (SUCCEEDED(hr))
		{
			pNode->get_Name(&strNodeName);

			ATLTRACE2(atlTraceUser, 2, "\nCreating node \"%s\" in parent \"%s\"", (LPCTSTR) _bstr_t(strNodeName), (LPCTSTR) _bstr_t(strParent));

			if (strNodeName == NULL || ::SysStringLen(strNodeName) == 0 || DuplicateName(pNode))
			{
				// No name given or name is duplicate. Generate a unique name for the object.
				BSTR strNewName = MakeUniqueName(pNode);

				if (strNewName != NULL)
				{
					pNode->put_Name(strNewName);
					::SysFreeString(strNewName);
					ATLTRACE2(atlTraceUser, 2, "\n\tNode name \"%s\" generated automatically", (LPCTSTR) _bstr_t(strNewName));
				}
			}

			if (strNodeName != NULL)
			{
				::SysFreeString(strNodeName);
			}

			// Add the object to the parent node
			if (nChildIndex < 0)
			{
				hr = pParent->AppendChild(pNode);
			}
			else
			{
				hr = pParent->InsertChild(pNode, nChildIndex);
			}

#ifdef _DEBUG
			if (SUCCEEDED(hr))
			{
				ATLTRACE2(atlTraceUser, 2, "\n\tNode added successfully");
			}
			else
			{
				ATLTRACE2(atlTraceUser, 2, "\n\tUnable to add new node");
			}
#endif
		}

		if (pParent != NULL)
		{
			pParent->Release();
		}

		return hr;
	}

	STDMETHOD(AppendNodes)(/* [in] */ INodes* pNodeCollection, /* [in] */ BSTR strParent)
	{
		HRESULT hr = S_OK;

		INodesAccessor<INode> insertSet(pNodeCollection);
		INode* pCurNode = NULL;
		ICompositeNode* pParent = NULL;
		BSTR strNodeName = NULL;

		HRESULT hrLoop = insertSet.GetFirst(&pCurNode);

		while (hrLoop == S_OK && SUCCEEDED(hr))
		{
			// If parent node name given then find it and use it as parent
			// instead of the model
			if (strParent != NULL && ::SysStringLen(strParent) > 0)
			{
				INode* pMatchingNode = NULL;
				if (GetChildByName(strParent, &pMatchingNode) == S_OK)
				{
					hr = pMatchingNode->QueryInterface(IID_ICompositeNode, (LPVOID*) &pParent);
					pMatchingNode->Release();
				}
			}
			else
			{
				hr = static_cast<ICompositeNode*>(this)->QueryInterface(IID_ICompositeNode, (LPVOID*) &pParent);
			}

			if (SUCCEEDED(hr))
			{
				pCurNode->get_Name(&strNodeName);

				ATLTRACE2(atlTraceUser, 2, "\nCreating node \"%s\" in parent \"%s\"", (LPCTSTR) _bstr_t(strNodeName), (LPCTSTR) _bstr_t(strParent));

				if (strNodeName == NULL || ::SysStringLen(strNodeName) == 0 || DuplicateName(pCurNode))
				{
					// No name given or name is duplicate. Generate a unique name for the object.
					BSTR strNewName = MakeUniqueName(pCurNode);

					if (strNewName != NULL)
					{
						pCurNode->put_Name(strNewName);
						::SysFreeString(strNewName);
						ATLTRACE2(atlTraceUser, 2, "\n\tNode name \"%s\" generated automatically", (LPCTSTR) _bstr_t(strNewName));
					}
				}

				if (strNodeName != NULL)
				{
					::SysFreeString(strNodeName);
				}

				// Append the object to the parent node
				hr = pParent->AppendChild(pCurNode);

#ifdef _DEBUG
				if (SUCCEEDED(hr))
				{
					ATLTRACE2(atlTraceUser, 2, "\n\tNode added successfully");
				}
				else
				{
					ATLTRACE2(atlTraceUser, 2, "\n\tUnable to add new node");
				}
#endif
			}

			if (pParent != NULL)
			{
				pParent->Release();
			}

			pCurNode->Release();
			pCurNode = NULL;

			hrLoop = insertSet.GetNext(&pCurNode);
		}

		return hr;
	}

	STDMETHOD(DeleteNode)(/* [in] */ INode* pNode)
	{
		HRESULT hr = E_FAIL;

		INode* pCurParent = NULL;
		ICompositeNode* pCurCompositeParent = NULL;
		IVisualPtr spVisual(pNode);
		bool bCanDelete = true;

		if (spVisual != NULL)
		{
			VARIANT_BOOL bEnabled;
			HRESULT hrStyles = spVisual->TestStyleFlags(StyleFlag::ALLOW_DELETE, &bEnabled);

			if (SUCCEEDED(hrStyles) && !bEnabled)
			{
				bCanDelete = false;
			}
		}

		if (bCanDelete)
		{
			if (pNode->get_Parent(&pCurParent) == S_OK)
			{
				if (SUCCEEDED(pCurParent->QueryInterface(IID_ICompositeNode, (LPVOID*) &pCurCompositeParent)))
				{
					hr = pCurCompositeParent->RemoveChild(pNode);
					pCurCompositeParent->Release();
				}
				pCurParent->Release();
			}
		}

		return hr;
	}

	STDMETHOD(DeleteNodes)(/* [in] */ INodes* pNodeCollection)
	{
		HRESULT hr = E_FAIL;

		INodesAccessor<INode> deleteSet(pNodeCollection);
		INode* pCurNode = NULL;
		INode* pCurParent = NULL;
		ICompositeNode* pCurCompositeParent = NULL;

		HRESULT hrLoop = deleteSet.GetFirst(&pCurNode);

		while (hrLoop == S_OK)
		{
			bool bCanDelete = true;
			IVisualPtr spCurVisual(pCurNode);

			if (spCurVisual != NULL)
			{
				VARIANT_BOOL bEnabled;
				HRESULT hrStyles = spCurVisual->TestStyleFlags(StyleFlag::ALLOW_DELETE, &bEnabled);

				if (SUCCEEDED(hrStyles) && !bEnabled)
				{
					bCanDelete = false;
				}
			}

			if (bCanDelete)
			{
				if (pCurNode->get_Parent(&pCurParent) == S_OK)
				{
					if (SUCCEEDED(pCurParent->QueryInterface(IID_ICompositeNode, (LPVOID*) &pCurCompositeParent)))
					{
						hr = pCurCompositeParent->RemoveChild(pCurNode);
						pCurCompositeParent->Release();
					}
					pCurParent->Release();
				}
			}

			pCurNode->Release();
			pCurNode = NULL;

			hrLoop = deleteSet.GetNext(&pCurNode);
		}

		return hr;
	}

	STDMETHOD(MoveNodes)(/* [in] */ INodes* pNodeCollection,
	                     /* [in] */ VALUE3D tx,
	                     /* [in] */ VALUE3D ty,
	                     /* [in] */ VALUE3D tz)
	{
		HRESULT hr = S_OK;

		INodesAccessor<IVisual> moveSet(pNodeCollection);
		IVisual* pVisual = NULL;

		HRESULT hrLoop = moveSet.GetFirst(&pVisual);

		while (hrLoop == S_OK)
		{
			VARIANT_BOOL bEnabled;
			HRESULT hrStyles = pVisual->TestStyleFlags(StyleFlag::ALLOW_MOVE, &bEnabled);

			if (SUCCEEDED(hrStyles) && bEnabled)
			{
				hr = pVisual->Translate(tx, ty, tz);
			}

			hrLoop = moveSet.GetNext(&pVisual);
		}

		return hr;
	}

	STDMETHOD(ScaleNodes)(/* [in] */ INodes* pNodeCollection,
	                      /* [in] */ VALUE3D sx,
	                      /* [in] */ VALUE3D sy,
	                      /* [in] */ VALUE3D sz)
	{
		HRESULT hr = S_OK;

		INodesAccessor<IVisual> scaleSet(pNodeCollection);
		IVisual* pVisual = NULL;

		HRESULT hrLoop = scaleSet.GetFirst(&pVisual);

		while (hrLoop == S_OK)
		{
			VARIANT_BOOL bEnabled;
			HRESULT hrStyles = pVisual->TestStyleFlags(StyleFlag::ALLOW_SCALE, &bEnabled);

			if (SUCCEEDED(hrStyles) && bEnabled)
			{
				hr = pVisual->Scale(sx, sy, sz);
			}

			hrLoop = scaleSet.GetNext(&pVisual);
		}

		return hr;
	}

	STDMETHOD(RotateNodes)(/* [in] */ INodes* pNodeCollection,
	                       /* [in] */ VALUE3D fRadians,
	                       /* [in] */ VALUE3D fAxisX,
	                       /* [in] */ VALUE3D fAxisY,
	                       /* [in] */ VALUE3D fAxisZ)
	{
		HRESULT hr = S_OK;

		INodesAccessor<IVisual> rotateSet(pNodeCollection);
		IVisual* pVisual = NULL;

		HRESULT hrLoop = rotateSet.GetFirst(&pVisual);

		while (hrLoop == S_OK)
		{
			VARIANT_BOOL bEnabled;
			HRESULT hrStyles = pVisual->TestStyleFlags(StyleFlag::ALLOW_ROTATE, &bEnabled);

			if (SUCCEEDED(hrStyles) && bEnabled)
			{
				hr = pVisual->Rotate(fRadians, fAxisX, fAxisY, fAxisZ);
			}

			hrLoop = rotateSet.GetNext(&pVisual);
		}

		return hr;
	}

	STDMETHOD(GroupNodes)(/* [in] */ INode* pGroup,
	                      /* [in] */ INodes* pNodeCollection)
	{
		ICompositeNodePtr spGroupComposite(pGroup);

		if (pGroup == NULL || pNodeCollection == NULL || spGroupComposite == NULL)
		{
			return E_POINTER;
		}

		// Detach nodes in list from their parents

		INodesAccessor<INode> nodes(pNodeCollection);
		INode* pCurNode = NULL;
		INode* pCurParent = NULL;

		HRESULT hr = S_OK;
		HRESULT hrLoop = nodes.GetFirst(&pCurNode);

		while (hrLoop == S_OK && hr == S_OK)
		{
			if (pCurNode->get_Parent(&pCurParent) == S_OK)
			{
				ICompositeNodePtr spCurParentComposite(pCurParent);

				if (spCurParentComposite != NULL)
				{
					spCurParentComposite->RemoveChild(pCurNode);
				}

				pCurNode->putref_Parent(NULL);

				hr = spGroupComposite->AppendChild(pCurNode);

				pCurParent->Release();
			}

			pCurNode->Release();
			pCurNode = NULL;

			hrLoop = nodes.GetNext(&pCurNode);
		}

		return hr;
	}

	STDMETHOD(UngroupNodes)(/* [in] */ INode* pGroup,
	                        /* [in] */ INodes* pNodeCollection)
	{
		ICompositeNodePtr spGroupComposite(pGroup);

		if (pGroup == NULL || spGroupComposite == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = E_FAIL;

		long nChildCount;
		INode* pCurChild = NULL;

		if (FAILED(spGroupComposite->GetChildCount(&nChildCount)))
		{
			return E_FAIL;
		}

		if (pNodeCollection != NULL)
		{
			pNodeCollection->Clear();
		}

		INode* pGroupParent = NULL;

		if (pGroup->get_Parent(&pGroupParent) == S_OK)
		{
			ICompositeNodePtr spParentComposite(pGroupParent);

			if (spParentComposite != NULL)
			{
				for (long nChildIdx = 0; nChildIdx < nChildCount; nChildIdx++)
				{
					if (spGroupComposite->GetChild(nChildIdx, &pCurChild) == S_OK)
					{
						spGroupComposite->RemoveChild(pCurChild);
						pCurChild->putref_Parent(NULL);
						spParentComposite->AppendChild(pCurChild);

						if (pNodeCollection != NULL)
						{
							pNodeCollection->Add(pCurChild);
						}

						pCurChild->Release();
						pCurChild = NULL;
					}
				}
			}

			pGroupParent->Release();

			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(putref_MeasurementScale)(/* [in] */ IMeasurementScale* pNewVal)
	{
		if (pNewVal != NULL)
		{
			pNewVal->AddRef();
		}

		if (m_pMeasurementScale != NULL)
		{
			m_pMeasurementScale->Release();
		}

		m_pMeasurementScale = pNewVal;

		return S_OK;
	}

	STDMETHOD(get_MeasurementScale)(/* [out,retval] */ IMeasurementScale** ppVal)
	{
		HRESULT hr = S_OK;

		if (ppVal == NULL)
		{
			hr = E_POINTER;
		}
		else
		{
			if (m_pMeasurementScale == NULL)
			{
				// Create a measurement scale object if one doesn't exist
				hr = CMeasurementScale::_CreatorClass::CreateInstance(NULL, IID_IMeasurementScale, (LPVOID*) &m_pMeasurementScale);
			}

			if (SUCCEEDED(hr) && m_pMeasurementScale != NULL)
			{
				*ppVal = m_pMeasurementScale;
				(*ppVal)->AddRef();
			}
		}

		return hr;
	}

	STDMETHOD(Clear)()
	{
		return RemoveAllChildren();
	}

	STDMETHOD(SetAmbientLight)(/* [in] */ VALUE3D red,
	                           /* [in] */ VALUE3D green,
	                           /* [in] */ VALUE3D blue,
	                           /* [in] */ VALUE3D alpha)
	{
		m_ambientLight.r = red;
		m_ambientLight.g = green;
		m_ambientLight.b = blue;
		m_ambientLight.a = alpha;
		return S_OK;
	}
	
	STDMETHOD(GetAmbientLight)(/* [in] */ VALUE3D* pRed,
	                           /* [in] */ VALUE3D* pGreen,
	                           /* [in] */ VALUE3D* pBlue,
	                           /* [in] */ VALUE3D* pAlpha)
	{
		*pRed = m_ambientLight.r;
		*pGreen = m_ambientLight.g;
		*pBlue = m_ambientLight.b;
		*pAlpha = m_ambientLight.a;
		return S_OK;
	}

	STDMETHOD(TranslateObjects)(INodes* pCollection, FLOAT tx, FLOAT ty, FLOAT tz)
	{
		HRESULT hr = E_FAIL;

		IUnknown* pEnumUnk;
		IEnumNodes* pEnum = NULL;

		pCollection->get__NewEnum(&pEnumUnk);

		if (pEnumUnk != NULL)
		{
			hr = pEnumUnk->QueryInterface(IID_IEnumNodes, (LPVOID*) &pEnum);
			pEnumUnk->Release();
		}

		if (SUCCEEDED(hr) && pEnum != NULL)
		{
			INode* pNode;
			ULONG nFetched;

			while ((pEnum->Next(1, &pNode, &nFetched) == S_OK) && nFetched > 0)
			{
				IVisual* pVisual;
				if (SUCCEEDED(pNode->QueryInterface(IID_IVisual, (LPVOID*) &pVisual)))
				{
					pVisual->Translate(tx, ty, tz);
					pVisual->Release();
				}
			}

			pEnum->Release();
		}

		return hr;
	}

	STDMETHOD(get_BSPTree)(/* [out, retval] */ IBSPTree** ppBSPTree)
	{
		if (ppBSPTree == NULL)
		{
			return E_POINTER;
		}

		*ppBSPTree = m_pBSPTree;

		if (*ppBSPTree != NULL)
		{
			(*ppBSPTree)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(get_BSPTreeEnabled)(/* [out, retval] */ VARIANT_BOOL* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}
		*pVal = m_bBSPTreeEnabled ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}

	STDMETHOD(put_BSPTreeEnabled)(/* [in] */ VARIANT_BOOL newVal)
	{
		m_bBSPTreeEnabled = newVal ? true : false;
		return S_OK;
	}

	STDMETHOD(LoadBSPTree)()
	{
		HRESULT hr = E_FAIL;

		if (m_pBSPTree != NULL)
		{
			m_pBSPTree->RemoveAllObjects();
			hr = m_pBSPTree->AddObject(static_cast<INode*>(this), VARIANT_TRUE);
		}

		return hr;
	}

	STDMETHOD(LockEvents)(/* [in] */ VARIANT_BOOL bFlag)
	{
		if (bFlag)
		{
			m_nEventLocks++;
		}
		else
		{
			m_nEventLocks--;
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////
	// ICompositeNode interface

	STDMETHOD(OnChildUpdate)(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode)
	{
		pNode;

		if ((nNotifyCode & ChildNotification::ADD) ||
		    (nNotifyCode & ChildNotification::REMOVE) ||
		    (nNotifyCode & ChildNotification::REMOVE_ALL))
		{
			ClearPipeline();
		}

		return S_OK;
	}

// Implementation
protected:
	virtual void CompilePipeline()
	{
		_NodeVector::iterator itChild;
		IRenderDX8* pRender = NULL;
		IContextObject* pContextObj = NULL;
		ILight* pLight = NULL;
		long nCurLightIdx = 0;

		m_renderNodes.clear();
		m_contextNodes.clear();

		for (itChild = m_children.begin(); itChild != m_children.end(); itChild++)
		{
			if (SUCCEEDED(((INode*)*itChild)->QueryInterface(IID_IRenderDX8, (LPVOID*) &pRender)))
			{
				m_renderNodes.push_back(pRender);
				pRender->Release();
			}

			if (SUCCEEDED(((INode*)*itChild)->QueryInterface(IID_ILight, (LPVOID*) &pLight)))
			{
				pLight->put_LightIndex(nCurLightIdx++);
				pLight->Release();
			}

			if (SUCCEEDED(((INode*)*itChild)->QueryInterface(IID_IContextObject, (LPVOID*) &pContextObj)))
			{
				m_contextNodes.push_back(pContextObj);
				pContextObj->Release();
			}
		}

		m_bCompiled = true;
	}

	virtual void ClearPipeline()
	{
		m_renderNodes.clear();
		m_contextNodes.clear();
		m_bCompiled = false;
	}
};

/////////////////////////////////////////////////////////////////////////
// CModel

class ATL_NO_VTABLE CModel : public CComObjectRootEx<CComSingleThreadModel>,
                             public CComCoClass<CModel, &CLSID_Model>,
                             public IConnectionPointContainerImpl<CModel>,
                             public CProxy__IModelEvents<CModel>,
                             public ISupportErrorInfo,
                             public IDispatchPropConImpl<IModelImpl< ICompositeVisualImpl< IModel, INodePtr > >, &IID_IModel, &LIBID_ShapeShifter>,
							 public IRenderDX8,
							 public IPersistXMLImpl<CModel>
{
// Embedded types
public:
	typedef IDispatchPropConImpl<IModelImpl< ICompositeVisualImpl< IModel, INodePtr > >, &IID_IModel, &LIBID_ShapeShifter> _ModelBase;

// Constructors/destructor
public:
	CModel();
	virtual ~CModel();

DECLARE_REGISTRY_RESOURCEID(IDR_MODEL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DEFINE_XML_TAG(tag_Model)

// Interface map
BEGIN_COM_MAP(CModel)
	COM_INTERFACE_ENTRY_IID(IID_IRenderDX8, IRenderDX8)
	COM_INTERFACE_ENTRY(IModel)
	COM_INTERFACE_ENTRY(IVisual)
	COM_INTERFACE_ENTRY(INode)
	COM_INTERFACE_ENTRY(ICompositeNode)
	COM_INTERFACE_ENTRY(IPropertyContainer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IPersistXML)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// Connection point map
BEGIN_CONNECTION_POINT_MAP(CModel)
	CONNECTION_POINT_ENTRY(DIID__IModelEvents)
END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct();

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICompositeNode interface

	STDMETHOD(OnChildUpdate)(/* [in] */ INode* pNode, /* [in] */ long nHint);

	//////////////////////////////////////////////////////////////
	// IVisual interface

	STDMETHOD(SetBoundingBox)(/* [in] */ VARIANT varBox);

	STDMETHOD(GetChildrenIntersectingRay)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varRayOrigin,
	                                      /* [in] */ VARIANT varRayDirection,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingSphere)(/* [in] */ INodes* pNodeCollection,
	                                         /* [in] */ VARIANT varSphere,
	                                         /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingBox)(/* [in] */ INodes* pNodeCollection,
	                                      /* [in] */ VARIANT varBox,
	                                      /* [in] */ enum HitTestStrategy strategy);

	STDMETHOD(GetChildrenIntersectingPlane)(/* [in] */ INodes* pNodeCollection,
	                                        /* [in] */ VARIANT varPlane,
	                                        /* [in] */ enum HitTestStrategy strategy);

	//////////////////////////////////////////////////////////////
	// IModel interface

	STDMETHOD(InsertNode)(/* [in] */ INode* pNode,
	                      /* [in] */ BSTR strParent,
	                      /* [in] */ long nChildIndex);
	STDMETHOD(AppendNodes)(/* [in] */ INodes* pNodeCollection, /* [in] */ BSTR strParent);
	STDMETHOD(DeleteNode)(/* [in] */ INode* pNode);
	STDMETHOD(DeleteNodes)(/* [in] */ INodes* pNodeCollection);
	STDMETHOD(MoveNodes)(/* [in] */ INodes* pNodeCollection,
	                     /* [in] */ VALUE3D tx,
	                     /* [in] */ VALUE3D ty,
	                     /* [in] */ VALUE3D tz);

	//////////////////////////////////////////////////////////////
	// IRenderDX8 interface

	STDMETHOD(Render)(LPDIRECT3DDEVICE8 pDevice, IResourceManagerDX8* pDevResources);

	//////////////////////////////////////////////////////////////
	// IPersistXML interface

	STDMETHOD(GetClassID)(CLSID *pClassID);
	STDMETHOD(LoadXML)(/* [in] */ IXMLArchive* pArchive);
	STDMETHOD(SaveXML)(/* [in] */ IXMLArchive* pArchive);

	//////////////////////////////////////////////////////////////
	// ISupportsErrorInfo interface

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// Implementation
protected:
	IUnknown* __stdcall GetChildUnk(const long nIdx);
	bool __stdcall PutChildUnk(IUnknown* pUnk);
};

};  // namespace ShapeShifter

#endif // #ifndef __MODEL_H__
