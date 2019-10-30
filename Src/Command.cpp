//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Command.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Command.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CCreateShapeCommand

CCreateShapeCommand::CCreateShapeCommand()
{
	m_strProgId = L"ShapeShifter.Shape";
	m_strNodeName = L"Shape";
}

CCreateShapeCommand::~CCreateShapeCommand()
{
}

HRESULT CCreateShapeCommand::FinalConstruct()
{
	return S_OK;
}

STDMETHODIMP CCreateShapeCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"CreateShape");

	return S_OK;
}

STDMETHODIMP CCreateShapeCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	INode* pMeshNode = NULL;
	IMeshPtr spMesh;

	// Locate the mesh

	if (m_strMeshName.length() > 0)
	{
		if (spModel->GetChildByName(m_strMeshName, &pMeshNode) == S_OK)
		{
			spMesh = pMeshNode;
			pMeshNode->Release();
		}
		else
		{
			hr = E_FAIL;
		}
	}

	if (hr == S_OK)
	{
		if (m_pNode != NULL)
		{
			m_pNode->Release();
			m_pNode = NULL;
		}

		hr = spModel->CreateNode(m_strNodeName, m_strProgId, m_strParent, &m_pNode);

		if (SUCCEEDED(hr))
		{
			IShapePtr spShape(m_pNode);

			if (spShape != NULL)
			{
				spShape->putref_Mesh(spMesh);
			}
		}
	}

	return hr;
}

STDMETHODIMP CCreateShapeCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = E_FAIL;

	if (m_pNode != NULL)
	{
		hr = spModel->DeleteNode(m_pNode);
	}

	return hr;
}

STDMETHODIMP CCreateShapeCommand::SetMeshName(/* [in] */ BSTR strMeshName)
{
	m_strMeshName = strMeshName;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// CInsertCommand

CInsertCommand::CInsertCommand()
{
}

CInsertCommand::~CInsertCommand()
{
}

HRESULT CInsertCommand::FinalConstruct()
{
	return CreateNodeCollection();
}

STDMETHODIMP CInsertCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Insert");

	return S_OK;
}

STDMETHODIMP CInsertCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return spModel->AppendNodes(m_pNodes, m_strParent);
}

STDMETHODIMP CInsertCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return spModel->DeleteNodes(m_pNodes);
}

STDMETHODIMP CInsertCommand::SetParent(/* [in] */ BSTR strParent)
{
	m_strParent = strParent;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// CDeleteCommand

CDeleteCommand::CDeleteCommand()
{
}

CDeleteCommand::~CDeleteCommand()
{
}

HRESULT CDeleteCommand::FinalConstruct()
{
	return CreateNodeCollection();
}

STDMETHODIMP CDeleteCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Delete");

	return S_OK;
}

STDMETHODIMP CDeleteCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	// Save position info for nodes before deleting them

	m_posInfo.clear();

	INodesAccessor<INode> nodes(m_pNodes);
	INode* pNode = NULL;

	HRESULT hrLoop = nodes.GetFirst(&pNode);

	while (hr == S_OK && hrLoop == S_OK)
	{
		// Save position info for node before deleting it so that
		// it can be restored in the same place if necessary
		m_posInfo.push_back(NodeInsertPosition(pNode));

		hr = spModel->DeleteNode(pNode);

		hrLoop = nodes.GetNext(&pNode);
	}

	return hr;
}

STDMETHODIMP CDeleteCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	INodesAccessor<INode> nodes(m_pNodes);
	INode* pNode = NULL;
	UINT nNodeIdx = 0;

	HRESULT hrLoop = nodes.GetFirst(&pNode);

	while (hr == S_OK && hrLoop == S_OK)
	{
		if (nNodeIdx < m_posInfo.size())
		{
			NodeInsertPosition curPosInfo = m_posInfo[nNodeIdx];
			hr = spModel->InsertNode(pNode, curPosInfo.m_strParent, curPosInfo.m_nChildIdx);
		}
		hrLoop = nodes.GetNext(&pNode);
	}

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// CMoveCommand

CMoveCommand::CMoveCommand()
{
	m_tx = m_ty = m_tz = 0.0f;
}

CMoveCommand::~CMoveCommand()
{
}

HRESULT CMoveCommand::FinalConstruct()
{
	return CreateNodeCollection();
}

STDMETHODIMP CMoveCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Move");

	return S_OK;
}

STDMETHODIMP CMoveCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return spModel->MoveNodes(m_pNodes, m_tx, m_ty, m_tz);
}

STDMETHODIMP CMoveCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return spModel->MoveNodes(m_pNodes, -m_tx, -m_ty, -m_tz);
}

STDMETHODIMP CMoveCommand::SetDelta(/* [in] */ VALUE3D tx, /* [in] */ VALUE3D ty, /* [in] */ VALUE3D tz)
{
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// CRotateCommand

CRotateCommand::CRotateCommand()
{
	m_fRadians = 0.0f;
	m_fAxisX = 0.0f;
	m_fAxisY = 0.0f;
	m_fAxisZ = 0.0f;
}

CRotateCommand::~CRotateCommand()
{
}

HRESULT CRotateCommand::FinalConstruct()
{
	return CreateNodeCollection();
}

STDMETHODIMP CRotateCommand::SetAngle(/* [in] */ VALUE3D fRadians)
{
	m_fRadians = fRadians;
	return S_OK;
}

STDMETHODIMP CRotateCommand::SetAxis(/* [in] */ VALUE3D fAxisX,
                                     /* [in] */ VALUE3D fAxisY,
                                     /* [in] */ VALUE3D fAxisZ)
{
	m_fAxisX = fAxisX;
	m_fAxisY = fAxisY;
	m_fAxisZ = fAxisZ;
	return S_OK;
}

STDMETHODIMP CRotateCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Rotate");

	return S_OK;
}

STDMETHODIMP CRotateCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return spModel->RotateNodes(m_pNodes, m_fRadians, m_fAxisX, m_fAxisY, m_fAxisZ);
}

STDMETHODIMP CRotateCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////
// CScaleCommand

CScaleCommand::CScaleCommand()
{
	m_sx = 1.0f;
	m_sy = 1.0f;
	m_sz = 1.0f;
}

CScaleCommand::~CScaleCommand()
{
}

HRESULT CScaleCommand::FinalConstruct()
{
	return CreateNodeCollection();
}

STDMETHODIMP CScaleCommand::SetScale(/* [in] */ VALUE3D sx, /* [in] */ VALUE3D sy, /* [in] */ VALUE3D sz)
{
	m_sx = sx;
	m_sy = sy;
	m_sz = sz;
	return S_OK;
}

STDMETHODIMP CScaleCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Scale");

	return S_OK;
}

STDMETHODIMP CScaleCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return spModel->ScaleNodes(m_pNodes, m_sx, m_sy, m_sz);
}

STDMETHODIMP CScaleCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////
// CGroupCommand

CGroupCommand::CGroupCommand() :
	m_pGroupNode(NULL)
{
	m_strNodeName = L"Group";
}

CGroupCommand::~CGroupCommand()
{
	if (m_pGroupNode != NULL)
	{
		m_pGroupNode->Release();
	}
}

HRESULT CGroupCommand::FinalConstruct()
{
	return CreateNodeCollection();
}

STDMETHODIMP CGroupCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Group");

	return S_OK;
}

STDMETHODIMP CGroupCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = spModel->CreateNode(m_strNodeName,
	                                 _bstr_t("ShapeShifter.Group"),
	                                 m_strParent,
	                                 &m_pGroupNode);

	if (SUCCEEDED(hr))
	{
		hr = spModel->GroupNodes(m_pGroupNode, m_pNodes);
	}

	return hr;
}

STDMETHODIMP CGroupCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_FALSE;

	if (m_pGroupNode != NULL)
	{
		hr = spModel->UngroupNodes(m_pGroupNode, m_pNodes);

		if (SUCCEEDED(hr))
		{
			m_pGroupNode->Release();
			m_pGroupNode = NULL;
		}
	}

	return hr;
}

STDMETHODIMP CGroupCommand::SetParent(/* [in] */ BSTR strParent)
{
	m_strParent = strParent;
	return S_OK;
}

STDMETHODIMP CGroupCommand::SetNodeName(/* [in] */ BSTR strNodeName)
{
	m_strNodeName = strNodeName;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// CUngroupCommand

CUngroupCommand::CUngroupCommand() :
	m_pGroupNode(NULL),
	m_pNodes(NULL)
{
	m_strNodeName = L"Group";
}

CUngroupCommand::~CUngroupCommand()
{
	if (m_pNodes != NULL)
	{
		m_pNodes->Release();
	}

	if (m_pGroupNode != NULL)
	{
		m_pGroupNode->Release();
	}
}

HRESULT CUngroupCommand::FinalConstruct()
{
	return CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pNodes);
}

STDMETHODIMP CUngroupCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Ungroup");

	return S_OK;
}

STDMETHODIMP CUngroupCommand::Do(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	if (m_pGroupNode != NULL)
	{
		m_pGroupNode->Release();
		m_pGroupNode = NULL;
	}

	hr = spModel->GetChildByName(m_strNodeName, &m_pGroupNode);

	if (hr == S_OK)
	{
		hr = spModel->UngroupNodes(m_pGroupNode, m_pNodes);
	}

	return hr;
}

STDMETHODIMP CUngroupCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	if (pCmdTarget == NULL)
	{
		return E_POINTER;
	}

	IModelPtr spModel(pCmdTarget);

	if (spModel == NULL)
	{
		return E_POINTER;
	}

	if (m_pGroupNode == NULL)
	{
		return E_FAIL;
	}

	return spModel->GroupNodes(m_pGroupNode, m_pNodes);
}

STDMETHODIMP CUngroupCommand::SetNodeName(/* [in] */ BSTR strNodeName)
{
	m_strNodeName = strNodeName;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// CMacroCommand

CMacroCommand::CMacroCommand()
{
}

CMacroCommand::~CMacroCommand()
{
}

STDMETHODIMP CMacroCommand::get_Description(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"Macro");

	return S_OK;
}

STDMETHODIMP CMacroCommand::Do(/* [in] */ INode* pCmdTarget)
{
	HRESULT hr = S_OK;
	_CommandList::iterator itCmd;

	for (itCmd = m_commands.begin(); SUCCEEDED(hr) && itCmd != m_commands.end(); itCmd++)
	{
		hr = (*itCmd)->Do(pCmdTarget);
	}

	if (SUCCEEDED(hr))
	{
		hr = S_OK;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

STDMETHODIMP CMacroCommand::Undo(/* [in] */ INode* pCmdTarget)
{
	HRESULT hr = S_OK;
	_CommandList::reverse_iterator itCmd;

	for (itCmd = m_commands.rbegin(); SUCCEEDED(hr) && itCmd != m_commands.rend(); itCmd++)
	{
		hr = (*itCmd)->Undo(pCmdTarget);
	}

	if (SUCCEEDED(hr))
	{
		hr = S_OK;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

STDMETHODIMP CMacroCommand::AddCommand(/* [in] */ ICommand* pCommand)
{
	m_commands.push_back(pCommand);
	return S_OK;
}

STDMETHODIMP CMacroCommand::ClearCommands()
{
	m_commands.clear();
	return S_OK;
}

STDMETHODIMP CMacroCommand::get_CommandCount(/* [out, retval] */ long* pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = (long) m_commands.size();

	return S_OK;
}

};  // namespace ShapeShifter
