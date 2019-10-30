//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Controller.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Controller.h"
#include "ControllerUpdates.h"
#include "Behavior.h"
#include "NodeCollection.h"
#include "CommandDispatcher.H"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CBrowseController

CBrowseController::CBrowseController()
{
}

CBrowseController::~CBrowseController()
{
}

HRESULT CBrowseController::FinalConstruct()
{
	HRESULT hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pSelectionList);

	if (SUCCEEDED(hr))
	{
		IBehavior* pCameraStep = NULL;
		hr = CCameraStep::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pCameraStep);
		if (SUCCEEDED(hr))
		{
			hr = AddBehavior(pCameraStep);
			pCameraStep->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		IBehavior* pMouseSelect = NULL;
		hr = CMouseSelect::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pMouseSelect);
		if (SUCCEEDED(hr))
		{
			hr = AddBehavior(pMouseSelect);
			pMouseSelect->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pCollisions);
	}

	return hr;
}

STDMETHODIMP CBrowseController::Update(/* [in] */ long nUpdateFlags, /* [in] */ INodes* pNodesAffected)
{
	pNodesAffected;

	if (nUpdateFlags & ControllerUpdate::SELECTION)
	{
		Fire_OnSelectionChange(m_pSelectionList);
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Render();
		m_pViewport->RefreshScreen();
	}

	return S_OK;
}

STDMETHODIMP CBrowseController::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;

	_WndMessageBehaviors::iterator itMsgRcvr;

	for (itMsgRcvr = m_wndMessageBehaviors.begin(); itMsgRcvr != m_wndMessageBehaviors.end(); itMsgRcvr++)
	{
		hr = (*itMsgRcvr)->ProcessWindowMessage(uMsg, wParam, lParam);
	}

	return hr;
}

STDMETHODIMP CBrowseController::Animate(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame)
{
	HRESULT hr = S_OK;

	_AnimationBehaviors::iterator itAnimate;

	for (itAnimate = m_animationBehaviors.begin(); hr == S_OK && itAnimate != m_animationBehaviors.end(); itAnimate++)
	{
		hr = (*itAnimate)->Animate(nTime, bFirstFrame);
	}

	if (SUCCEEDED(hr))
	{
		Fire_OnAnimate(nTime, bFirstFrame);
		Update(ControllerUpdate::VIEWPORT, NULL);
	}

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// CBrowseEditController

CBrowseEditController::CBrowseEditController()
{
}

CBrowseEditController::~CBrowseEditController()
{
}

HRESULT CBrowseEditController::FinalConstruct()
{
	HRESULT hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pSelectionList);

	if (SUCCEEDED(hr))
	{
		IBehavior* pCameraStep = NULL;
		hr = CCameraStep::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pCameraStep);
		if (SUCCEEDED(hr))
		{
			hr = AddBehavior(pCameraStep);
			pCameraStep->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		IBehavior* pMouseSelect = NULL;
		hr = CMouseSelect::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pMouseSelect);
		if (SUCCEEDED(hr))
		{
			hr = AddBehavior(pMouseSelect);
			pMouseSelect->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		IBehavior* pInsertDelete = NULL;
		hr = CInsertDeleteNodes::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pInsertDelete);
		if (SUCCEEDED(hr))
		{
			hr = AddBehavior(pInsertDelete);
			pInsertDelete->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		IBehavior* pDragDrop = NULL;
		hr = CDragDrop::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pDragDrop);
		if (SUCCEEDED(hr))
		{
			hr = AddBehavior(pDragDrop);
			pDragDrop->Release();
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pCollisions);
	}

	return hr;
}

void CBrowseEditController::FinalRelease()
{
	if (m_pModel != NULL)
	{
		_ModelEventSink::DispEventUnadvise(m_pModel);
	}
}

STDMETHODIMP CBrowseEditController::Update(/* [in] */ long nUpdateFlags, /* [in] */ INodes* pNodesAffected)
{
	INode* pNodeClicked = NULL;

	if (nUpdateFlags == ControllerUpdate::SELECTION)
	{
		Fire_OnSelectionChange(m_pSelectionList);
	}

	if (nUpdateFlags == ControllerUpdate::NODE_DBLCLICK)
	{
		if (SUCCEEDED(pNodesAffected->get_Item(0, &pNodeClicked)))
		{
			Fire_OnNodeDblClick(pNodeClicked);
			pNodeClicked->Release();
		}
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Render();
		m_pViewport->RefreshScreen();
	}

	return S_OK;
}

STDMETHODIMP CBrowseEditController::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;

	_WndMessageBehaviors::iterator itMsgRcvr;

	for (itMsgRcvr = m_wndMessageBehaviors.begin(); itMsgRcvr != m_wndMessageBehaviors.end(); itMsgRcvr++)
	{
		hr = (*itMsgRcvr)->ProcessWindowMessage(uMsg, wParam, lParam);
	}

	return hr;
}

STDMETHODIMP CBrowseEditController::Animate(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame)
{
	HRESULT hr = S_OK;

	_AnimationBehaviors::iterator itAnimate;

	for (itAnimate = m_animationBehaviors.begin(); hr == S_OK && itAnimate != m_animationBehaviors.end(); itAnimate++)
	{
		hr = (*itAnimate)->Animate(nTime, bFirstFrame);
	}

	if (SUCCEEDED(hr))
	{
		Fire_OnAnimate(nTime, bFirstFrame);
		Update(ControllerUpdate::VIEWPORT, NULL);
	}

	return hr;
}

void CBrowseEditController::SetModel(IModel* pModel)
{
	if (m_pModel != NULL)
	{
		_ModelEventSink::DispEventUnadvise(m_pModel);
	}

	_ControllerBase::SetModel(pModel);

	if (m_pModel != NULL)
	{
		_ModelEventSink::DispEventAdvise(m_pModel);
	}
}

void CBrowseEditController::OnAddNode(/* [in] */ INode* pNode)
{
	if (m_pSelectionList != NULL && pNode != NULL)
	{
		m_pSelectionList->Clear();
		m_pSelectionList->Add(pNode);
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Render();
		m_pViewport->RefreshScreen();
	}
}

void CBrowseEditController::OnRemoveNode(/* [in] */ INode* pNode)
{
	pNode;

	if (m_pSelectionList != NULL)
	{
		m_pSelectionList->Clear();
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Render();
		m_pViewport->RefreshScreen();
	}
}

void CBrowseEditController::OnMoveNode(/* [in] */ INode* pNode)
{
	pNode;

	if (m_pViewport != NULL)
	{
		m_pViewport->Render();
		m_pViewport->RefreshScreen();
	}
}

/////////////////////////////////////////////////////////////////////////
// CFlightController

CFlightController::CFlightController()
{
}

CFlightController::~CFlightController()
{
}

HRESULT CFlightController::FinalConstruct()
{
	HRESULT hr = E_FAIL;

	IBehavior* pCameraFly = NULL;
	hr = CCameraFly::_CreatorClass::CreateInstance(NULL, IID_IBehavior, (LPVOID*) &pCameraFly);
	if (SUCCEEDED(hr))
	{
		hr = AddBehavior(pCameraFly);
		pCameraFly->Release();
	}

	if (SUCCEEDED(hr))
	{
		hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pCollisions);
	}

	return hr;
}

STDMETHODIMP CFlightController::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;

	_WndMessageBehaviors::iterator itMsgRcvr;

	for (itMsgRcvr = m_wndMessageBehaviors.begin(); itMsgRcvr != m_wndMessageBehaviors.end(); itMsgRcvr++)
	{
		hr = (*itMsgRcvr)->ProcessWindowMessage(uMsg, wParam, lParam);
	}

	return hr;
}

STDMETHODIMP CFlightController::Animate(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame)
{
	HRESULT hr = S_OK;

	_AnimationBehaviors::iterator itAnimate;

	for (itAnimate = m_animationBehaviors.begin(); hr == S_OK && itAnimate != m_animationBehaviors.end(); itAnimate++)
	{
		hr = (*itAnimate)->Animate(nTime, bFirstFrame);
	}

	if (SUCCEEDED(hr))
	{
		Fire_OnAnimate(nTime, bFirstFrame);
		Update(ControllerUpdate::VIEWPORT, NULL);
	}

	return hr;
}

};  // namespace ShapeShifter
