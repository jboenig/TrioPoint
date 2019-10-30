//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Behavior.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Behavior.h"
#include "ControllerStates.h"
#include "ControllerUpdates.h"
#include "Command.h"
#include "NodeCollection.h"
#include "Tracker.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CCameraFly

CCameraFly::CCameraFly() :
	m_dwVelocityTime(0),
	m_dwFirstMove(0),
	m_dwLastMove(0),
	m_fVelocityDistance(0.0f),
	m_dwRotateTime(0),
	m_fRotateDistance(0.0f),
	m_pKeyboard(NULL)
{
}

CCameraFly::~CCameraFly()
{
	FreeInputDevices();
}

STDMETHODIMP CCameraFly::get_Name(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"CameraFly");

	return S_OK;
}

STDMETHODIMP CCameraFly::get_VelocityDistance(/* [out, retval] */ VALUE3D *pVal)
{
	if (pVal != NULL)
	{
		*pVal = m_fVelocityDistance;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CCameraFly::put_VelocityDistance(/* [in] */ VALUE3D newVal)
{
	m_fVelocityDistance = newVal;
	return S_OK;
}

STDMETHODIMP CCameraFly::get_VelocityTime(/* [out, retval] */ long *pVal)
{
	if (pVal != NULL)
	{
		*pVal = static_cast<long>(m_dwVelocityTime);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CCameraFly::put_VelocityTime(/* [in] */ long newVal)
{
	m_dwVelocityTime = static_cast<DWORD>(newVal);
	return S_OK;
}

STDMETHODIMP CCameraFly::get_RotateTime(long *pVal)
{
	if (pVal != NULL)
	{
		*pVal = static_cast<long>(m_dwRotateTime);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CCameraFly::put_RotateTime(long newVal)
{
	m_dwRotateTime = static_cast<DWORD>(newVal);
	return S_OK;
}

STDMETHODIMP CCameraFly::get_RotateDistance(float *pVal)
{
	if (pVal != NULL)
	{
		*pVal = m_fRotateDistance;
		return S_OK;
	}	
	return E_FAIL;
}

STDMETHODIMP CCameraFly::put_RotateDistance(float newVal)
{
	m_fRotateDistance = newVal;
	return S_OK;
}

STDMETHODIMP CCameraFly::Animate(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame)
{
	HRESULT hr = S_OK;

	DWORD dwTime = static_cast<DWORD>(nTime);

	if (bFirstFrame)
	{
		m_dwFirstMove = dwTime;
		m_dwLastMove = dwTime;
		return S_OK;
	}

	DWORD dwMoveDuration;
	DWORD dwElapsed = dwTime - m_dwFirstMove;

	if (dwElapsed > m_dwVelocityTime)
	{
		// Restart the sequence
		m_dwFirstMove = (dwElapsed / m_dwVelocityTime) * m_dwVelocityTime;
		dwMoveDuration = dwTime - m_dwLastMove;
	}
	else
	{
		// Continue current sequence
		dwMoveDuration = dwTime - m_dwLastMove;
	}

	m_dwLastMove = dwTime;

	float fPctMove = ((float) dwMoveDuration / (float) m_dwVelocityTime);
	VALUE3D fMoveDist = m_fVelocityDistance * fPctMove;

	if (m_pController != NULL && m_bEnabled)
	{
		float dx = 0.0f;
		float dy = 0.0f;
		float dz = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;

		if (m_pKeyboard != NULL)
		{
			BYTE diks[256];   // DirectInput keyboard state buffer
			ZeroMemory(&diks, sizeof(diks));

			hr = m_pKeyboard->GetDeviceState(sizeof(diks), &diks);

			if (SUCCEEDED(hr))
			{
				goto CheckKeyboardInput;
			}

			hr = m_pKeyboard->Acquire();
			while (hr == DIERR_INPUTLOST)
			{
				hr = m_pKeyboard->Acquire();
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pKeyboard->GetDeviceState(sizeof(diks), &diks);
			}

			if (FAILED(hr))
			{
				goto SkipKeyboardInput;
			}

CheckKeyboardInput:

			if (diks[DIK_UP] & 0x80)
			{
				dy += 1;
			}

			if (diks[DIK_DOWN] & 0x80)
			{
				dy -= 1;
			}

			if (diks[DIK_LEFT] & 0x80)
			{
				dx -= 1;
			}

			if (diks[DIK_RIGHT] & 0x80)
			{
				dx += 1;
			}

			if (diks[DIK_W] & 0x80)
			{
				dz += 1;
			}

			if (diks[DIK_S] & 0x80)
			{
				dz -= 1;
			}

			if (diks[DIK_NUMPAD8] & 0x80)
			{
				rx -= 1;
			}

			if (diks[DIK_NUMPAD2] & 0x80)
			{
				rx += 1;
			}

			if (diks[DIK_NUMPAD4] & 0x80)
			{
				ry += 1;
			}

			if (diks[DIK_NUMPAD6] & 0x80)
			{
				ry -= 1;
			}

			if (diks[DIK_NUMPAD7] & 0x80)
			{
				rz -= 1;
			}

			if (diks[DIK_NUMPAD9] & 0x80)
			{
				rz += 1;
			}
		}

SkipKeyboardInput:

		m_pController->RotateCamera(rx/4.0f, ry/4.0f, rz/4.0f);
		m_pController->TranslateCamera(dx, dy, fMoveDist);
	}

	return hr;
}

STDMETHODIMP CCameraFly::AcquireInputDevices(IInputDeviceProviderDX8* pDevProvider)
{
	FreeInputDevices();

	HRESULT hr = pDevProvider->GetInputDevice(SSCL_KEYBOARD, &m_pKeyboard);

	if (SUCCEEDED(hr))
	{
		hr = m_pKeyboard->Acquire();
	}

	return hr;
}

STDMETHODIMP CCameraFly::FreeInputDevices()
{
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Release();
		m_pKeyboard = NULL;
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
// CCameraStep

CCameraStep::CCameraStep() :
	m_fMoveIncrement(2.0f),
	m_fRotateIncrement(0.1f)
{
}

STDMETHODIMP CCameraStep::get_Name(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"CameraStep");

	return S_OK;
}

STDMETHODIMP CCameraStep::get_MoveIncrement(/* [out, retval] */ VALUE3D *pVal)
{
	if (pVal != NULL)
	{
		*pVal = m_fMoveIncrement;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CCameraStep::put_MoveIncrement(/* [in] */ VALUE3D newVal)
{
	m_fMoveIncrement = newVal;
	return S_OK;
}

STDMETHODIMP CCameraStep::get_RotateIncrement(/* [out, retval] */ VALUE3D *pVal)
{
	if (pVal != NULL)
	{
		*pVal = m_fRotateIncrement;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CCameraStep::put_RotateIncrement(/* [in] */ VALUE3D newVal)
{
	m_fRotateIncrement = newVal;
	return S_OK;
}

STDMETHODIMP CCameraStep::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	lParam;

	HRESULT hr = S_FALSE;
	short zDelta = 0;

	if (m_bEnabled)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
			if (OnKeyDown(wParam))
			{
				m_pController->Update(ControllerUpdate::VIEWPORT, NULL);
				hr = S_OK;
			}
			break;

		case WM_MOUSEWHEEL:
			{
				zDelta = HIWORD(wParam);
				if (OnMouseWheel(LOWORD(wParam), zDelta, LOWORD(lParam), HIWORD(lParam)))
				{
					m_pController->Update(ControllerUpdate::VIEWPORT, NULL);
					hr = S_OK;
				}
			}
			break;
		}
	}

	return hr;
}

bool CCameraStep::OnKeyDown(const WPARAM vk)
{
	bool bHandled = false;
	HRESULT hr = E_FAIL;
	long nCurState;

	if (m_pController != NULL)
	{
		m_pController->get_CurrentState(&nCurState);

		if (nCurState == ControllerState::READY)
		{
			bHandled = true;

			switch(vk)
			{
			case VK_UP:
				hr = m_pController->MoveCamera(SSCL_CAMERA_UP, m_fMoveIncrement);
				break;

			case VK_DOWN:
				hr = m_pController->MoveCamera(SSCL_CAMERA_DOWN, m_fMoveIncrement);
				break;

			case VK_LEFT:
				hr = m_pController->MoveCamera(SSCL_CAMERA_LEFT, m_fMoveIncrement);
				break;

			case VK_RIGHT:
				hr = m_pController->MoveCamera(SSCL_CAMERA_RIGHT, m_fMoveIncrement);
				break;

			case 'W':
				hr = m_pController->MoveCamera(SSCL_CAMERA_FORWARD, m_fMoveIncrement);
				break;

			case 'S':
				hr = m_pController->MoveCamera(SSCL_CAMERA_BACKWARD, m_fMoveIncrement);
				break;

			case VK_NUMPAD8:
				hr = m_pController->MoveCamera(SSCL_CAMERA_PITCH_DOWN, m_fRotateIncrement);
				break;

			case VK_NUMPAD2:
				hr = m_pController->MoveCamera(SSCL_CAMERA_PITCH_UP, m_fRotateIncrement);
				break;

			case VK_NUMPAD4:
				hr = m_pController->MoveCamera(SSCL_CAMERA_YAW_LEFT, m_fRotateIncrement);
				break;

			case VK_NUMPAD6:
				hr = m_pController->MoveCamera(SSCL_CAMERA_YAW_RIGHT, m_fRotateIncrement);
				break;

			case VK_NUMPAD7:
				hr = m_pController->MoveCamera(SSCL_CAMERA_ROLL_LEFT, m_fRotateIncrement);
				break;

			case VK_NUMPAD9:
				hr = m_pController->MoveCamera(SSCL_CAMERA_ROLL_RIGHT, m_fRotateIncrement);
				break;

			case VK_NUMPAD5:
				hr = m_pController->MoveCamera(SSCL_CAMERA_LEVEL_XZ, 0.0f);
				break;

			default:
				bHandled = false;
			}
		}
	}

	return bHandled;
}

bool CCameraStep::OnMouseWheel(const UINT nFlags, const short zDelta, const int xPos, const int yPos)
{
	nFlags;
	xPos;
	yPos;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	bool bHandled = false;

	long nCurState;
	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::READY)
	{
		if (SUCCEEDED(m_pController->TranslateCamera(0.0f, 0.0f, (zDelta / 120) * m_fMoveIncrement)))
		{
			bHandled = true;
		}
	}

	return bHandled;
}

/////////////////////////////////////////////////////////////////////////
// CMouseSelect

CMouseSelect::CMouseSelect() :
	m_pSelectedNodes(NULL),
	m_pNodesHit(NULL),
	m_bMultiSelect(FALSE)
{
}

CMouseSelect::~CMouseSelect()
{
	if (m_pNodesHit != NULL)
	{
		m_pNodesHit->Release();
	}
}

HRESULT CMouseSelect::FinalConstruct()
{
	return CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pNodesHit);
}

STDMETHODIMP CMouseSelect::get_Name(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"MouseSelect");

	return S_OK;
}

STDMETHODIMP CMouseSelect::get_MultiSelect(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_bMultiSelect ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CMouseSelect::put_MultiSelect(/* [in] */ VARIANT_BOOL newVal)
{
	m_bMultiSelect = newVal ? true : false;
	return S_OK;
}

STDMETHODIMP CMouseSelect::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wParam;

	HRESULT hr = S_FALSE;

	if (m_bEnabled)
	{
		int xPos = 0;
		int yPos = 0;

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnLButtonDown(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_LBUTTONDBLCLK:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnLButtonDblClk(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
}

STDMETHODIMP CMouseSelect::SetController(/* [in] */ IController* pController)
{
	HRESULT hr = _BaseBehavior::SetController(pController);

	if (SUCCEEDED(hr))
	{
		if (m_pController != NULL)
		{
			hr = m_pController->get_SelectionList(&m_pSelectedNodes);

			if (SUCCEEDED(hr) && m_pSelectedNodes != NULL)
			{
				m_pSelectedNodes->Release();   // Weak reference - don't reference count
			}
		}
	}

	return hr;
}

bool CMouseSelect::OnLButtonDown(const WPARAM fwKeys, const int xPos, const int yPos)
{
	if (m_pController == NULL || m_pSelectedNodes == NULL)
	{
		return false;
	}

	bool bHandled = false;
	IViewport* pViewport = NULL;
	IModel* pModel = NULL;
	_variant_t varRayOrigin;
	_variant_t varRayDirection;
	long nNumHits = 0;
	long nCurState;

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::READY)
	{
		HRESULT hr = m_pController->get_Viewport(&pViewport);

		if (hr == S_OK)
		{
			hr = pViewport->ScreenToWorld(xPos, yPos, &varRayOrigin, &varRayDirection);
			pViewport->Release();
		}

		if (hr == S_OK)
		{
			hr = m_pController->get_Model(&pModel);

			if (hr == S_OK)
			{
				m_pNodesHit->Clear();

				hr = pModel->GetChildrenIntersectingRay(m_pNodesHit, varRayOrigin, varRayDirection, SSCL_TEST_BOUNDING_BOX);

				if (SUCCEEDED(hr))
				{
					m_pNodesHit->get_Count(&nNumHits);

					if (nNumHits == 0)
					{
						m_pSelectedNodes->Clear();
						m_pController->Update(ControllerUpdate::SELECTION, NULL);
					}
					else
					{
						INode* pNearestHit = GetNearestNodeHit();

						if (pNearestHit != NULL)
						{
							if ((fwKeys & MK_CONTROL) && m_bMultiSelect)
							{
								m_pSelectedNodes->Add(pNearestHit);
								m_pController->Update(ControllerUpdate::SELECTION, NULL);
							}
							else
							{
								if (m_pSelectedNodes->ContainsNode(pNearestHit) != S_OK)
								{
									m_pSelectedNodes->Clear();
									m_pSelectedNodes->Add(pNearestHit);
									m_pController->Update(ControllerUpdate::SELECTION, NULL);
								}
							}
							pNearestHit->Release();
						}
					}
				}

				pModel->Release();
			}
		}

		bHandled = true;
	}

	return bHandled;
}

bool CMouseSelect::OnLButtonDblClk(const WPARAM fwKeys, const int xPos, const int yPos)
{
	fwKeys;

	if (m_pController == NULL || m_pSelectedNodes == NULL)
	{
		return false;
	}

	bool bHandled = false;
	IViewport* pViewport = NULL;
	IModel* pModel = NULL;
	_variant_t varRayOrigin;
	_variant_t varRayDirection;
	long nNumHits = 0;
	long nCurState;

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::READY)
	{
		HRESULT hr = m_pController->get_Viewport(&pViewport);

		if (hr == S_OK)
		{
			hr = pViewport->ScreenToWorld(xPos, yPos, &varRayOrigin, &varRayDirection);

			if (hr == S_OK)
			{
				hr = m_pController->get_Model(&pModel);

				if (hr == S_OK)
				{
					m_pNodesHit->Clear();

					hr = pModel->GetChildrenIntersectingRay(m_pNodesHit, varRayOrigin, varRayDirection, SSCL_TEST_BOUNDING_BOX);

					if (hr == S_OK)
					{
						m_pNodesHit->get_Count(&nNumHits);
					}

					pModel->Release();
				}
			}

			if (hr == S_OK && nNumHits > 0)
			{
				ICamera* pCamera = NULL;
				if (SUCCEEDED(pViewport->get_Camera(&pCamera)))
				{
					D3DXVECTOR3 cameraLoc;
					if (SUCCEEDED(pCamera->GetLocation(&cameraLoc.x, &cameraLoc.y, &cameraLoc.z)))
					{
						m_pNodesHit->SortByDistance(vtPOINT3D(cameraLoc));
					}
					pCamera->Release();
				}

				m_pController->Update(ControllerUpdate::NODE_DBLCLICK, m_pNodesHit);
			}

			pViewport->Release();
		}

		bHandled = true;
	}

	return bHandled;
}

INode* CMouseSelect::GetNearestNodeHit()
{
	if (m_pController == NULL)
	{
		return NULL;
	}

	D3DXVECTOR3 cameraLoc;
	HRESULT hrCamLoc = E_FAIL;

	IViewport* pViewport = NULL;
	HRESULT hr = m_pController->get_Viewport(&pViewport);

	if (hr == S_OK)
	{
		ICamera* pCamera = NULL;
		if (SUCCEEDED(pViewport->get_Camera(&pCamera)))
		{
			hrCamLoc = pCamera->GetLocation(&cameraLoc.x, &cameraLoc.y, &cameraLoc.z);
			pCamera->Release();
		}
		pViewport->Release();
	}

	if (FAILED(hrCamLoc))
	{
		return NULL;
	}

	INode* pNearest = NULL;
	m_pNodesHit->GetNearestNode(vtPOINT3D(cameraLoc), StyleFlag::ALLOW_SELECT, &pNearest);
	return pNearest;
}

/////////////////////////////////////////////////////////////////////////
// CDragDrop

CDragDrop::CDragDrop() :
	m_pSelectedNodes(NULL),
	m_pMoveSet(NULL),
	m_pViewport(NULL)
{
}

CDragDrop::~CDragDrop()
{
	if (m_pMoveSet != NULL)
	{
		m_pMoveSet->Clear();
		m_pMoveSet->Release();
	}

	if (m_pViewport != NULL)
	{
		m_pViewport->Release();
	}
}

HRESULT CDragDrop::FinalConstruct()
{
	HRESULT hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pMoveSet);

	if (SUCCEEDED(hr))
	{
		hr = TrackMoveInit();
	}

	return hr;
}

STDMETHODIMP CDragDrop::get_MoveIncrement(/* [out, retval] */ VALUE3D *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_fMoveIncrement;

	return S_OK;
}

STDMETHODIMP CDragDrop::put_MoveIncrement(/* [in] */ VALUE3D newVal)
{
	m_fMoveIncrement = newVal;
	return S_OK;
}

STDMETHODIMP CDragDrop::BeginMove()
{
	if (m_pController == NULL)
	{
		return E_FAIL;  // Behavior not initialized
	}

	HRESULT hr = PrepareMoveSet();

	if (hr == S_OK)
	{
		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
			m_pViewport = NULL;
		}

		hr = m_pController->get_Viewport(&m_pViewport);

		if (SUCCEEDED(hr))
		{
			hr = BeginTrackingMove();


			if (SUCCEEDED(hr))
			{
				hr = m_pController->put_CurrentState(ControllerState::MOVING_NODES);
			}
		}
	}

	return hr;
}

STDMETHODIMP CDragDrop::EndMove()
{
	if (m_pController == NULL)
	{
		return E_FAIL;  // Behavior not initialized
	}

	HRESULT hr = E_FAIL;
	long nCurState;

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::MOVING_NODES)
	{
		hr = DoMoveCommand();

		RestoreCursor();

		m_pTrackerNodes->Clear();

		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
			m_pViewport = NULL;
		}

		m_pController->put_CurrentState(ControllerState::READY);
	}

	return hr;
}

STDMETHODIMP CDragDrop::CancelMove()
{
	if (m_pController == NULL)
	{
		return E_FAIL;  // Behavior not initialized
	}

	HRESULT hr = E_FAIL;
	long nCurState;

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::MOVING_NODES)
	{
		m_pController->Update(ControllerUpdate::VIEWPORT, NULL);

		RestoreCursor();

		m_pTrackerNodes->Clear();

		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
			m_pViewport = NULL;
		}

		hr = m_pController->put_CurrentState(ControllerState::READY);
	}

	return hr;
}

STDMETHODIMP CDragDrop::SetController(/* [in] */ IController* pController)
{
	HRESULT hr = _BaseBehavior::SetController(pController);

	if (SUCCEEDED(hr))
	{
		if (m_pController != NULL)
		{
			hr = m_pController->get_SelectionList(&m_pSelectedNodes);

			if (SUCCEEDED(hr) && m_pSelectedNodes != NULL)
			{
				m_pSelectedNodes->Release();   // Weak reference - don't reference count
			}
		}
	}

	return hr;
}

STDMETHODIMP CDragDrop::get_Name(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"DragDrop");

	return S_OK;
}

STDMETHODIMP CDragDrop::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_FALSE;

	if (m_bEnabled)
	{
		int xPos = 0;
		int yPos = 0;
		short zDelta = 0;

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnLButtonDown(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_RBUTTONDOWN:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnRButtonDown(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_MOUSEMOVE:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnMouseMove(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_MOUSEWHEEL:
			{
				zDelta = HIWORD(wParam);
				if (OnMouseWheel(LOWORD(wParam), zDelta, LOWORD(lParam), HIWORD(lParam)))
				{
					hr = S_OK;
				}
			}
			break;

		case WM_KEYDOWN:
			if (OnKeyDown(wParam))
			{
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
}

bool CDragDrop::OnLButtonDown(const WPARAM fwKeys, const int xPos, const int yPos)
{
	fwKeys;
	xPos;
	yPos;

	bool bHandled = false;

	if (EndMove() == S_OK)
	{
		bHandled = true;
	}

	return bHandled;
}

bool CDragDrop::OnRButtonDown(const WPARAM fwKeys, const int xPos, const int yPos)
{
	fwKeys;
	xPos;
	yPos;

	bool bHandled = false;

	if (CancelMove() == S_OK)
	{
		bHandled = true;
	}

	return bHandled;
}

bool CDragDrop::OnMouseMove(const DWORD fwKeys, const int xPos, const int yPos)
{
	fwKeys;

	long nCurState;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::MOVING_NODES)
	{
		TrackMouseMove(xPos, yPos);
	}

	return true;
}

bool CDragDrop::OnKeyDown(const WPARAM vk)
{
	bool bHandled = false;
	long nCurState;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::MOVING_NODES)
	{
		switch (vk)
		{
		case 'S':
			MoveZ(1);
			bHandled = true;
			break;

		case 'W':
			MoveZ(-1);
			bHandled = true;
			break;

		case VK_UP:
			MoveY(1);
			bHandled = true;
			break;

		case VK_DOWN:
			MoveY(-1);
			bHandled = true;
			break;

		case VK_LEFT:
			MoveX(-1);
			bHandled = true;
			break;

		case VK_RIGHT:
			MoveX(1);
			bHandled = true;
			break;

		case VK_RETURN:
			if (EndMove() == S_OK)
			{
				bHandled = true;
			}
			break;

		case VK_ESCAPE:
			if (CancelMove() == S_OK)
			{
				bHandled = true;
			}
			break;
		}
	}

	return bHandled;
}

bool CDragDrop::OnMouseWheel(const UINT nFlags, const short zDelta, const int xPos, const int yPos)
{
	nFlags;
	xPos;
	yPos;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	long nCurState;
	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::MOVING_NODES)
	{
		MoveZ(zDelta / 120);
	}

	return true;
}

HRESULT CDragDrop::PrepareMoveSet()
{
	if (m_pController == NULL || m_pSelectedNodes == NULL ||
	    m_pMoveSet == NULL || m_pTrackerNodes == NULL)
	{
		return E_FAIL;
	}

	m_pMoveSet->Clear();
	m_pTrackerNodes->Clear();

	INodesAccessor<IVisual> curSelection(m_pSelectedNodes);
	IVisual* pCurVisual = NULL;

	HRESULT hr = S_FALSE;
	VARIANT_BOOL bEnabled;

	HRESULT hrLoop = curSelection.GetFirst(&pCurVisual);

	while (hrLoop == S_OK)
	{
		HRESULT hrStyles = pCurVisual->TestStyleFlags(StyleFlag::ALLOW_MOVE, &bEnabled);

		if (SUCCEEDED(hrStyles) && bEnabled)
		{
			m_pMoveSet->Add(pCurVisual);
			AddMoveTracker(pCurVisual);
			hr = S_OK;
		}

		pCurVisual->Release();
		pCurVisual = NULL;

		hrLoop = curSelection.GetNext(&pCurVisual);
	}

	return hr;
}

HRESULT CDragDrop::DoMoveCommand()
{
	HRESULT hr = E_FAIL;

	if (m_pController != NULL)
	{
		INode* pCurNode = NULL;
		INodesAccessor<INode> moveSet(m_pMoveSet);

		HRESULT hrLoop = moveSet.GetFirst(&pCurNode);

		while (hrLoop == S_OK)
		{
			IMoveCommand* pCmd = NULL;
			hr = CMoveCommand::_CreatorClass::CreateInstance(NULL, IID_IMoveCommand, (LPVOID*) &pCmd);

			if (SUCCEEDED(hr))
			{
				pCmd->SetDelta(m_moveVector.x, m_moveVector.y, m_moveVector.z);
				pCmd->AddNode(pCurNode);
				hr = m_pController->Do(pCmd);
				pCmd->Release();
			}

			pCurNode->Release();
			pCurNode = NULL;

			hrLoop = moveSet.GetNext(&pCurNode);
		}
	}

	return hr;
}

/////////////////////////////////////////////////////////////////////////
// CInsertDeleteNodes

CInsertDeleteNodes::CInsertDeleteNodes() :
	m_pSelectedNodes(NULL),
	m_pViewport(NULL),
	m_pInsertNode(NULL)
{
}

CInsertDeleteNodes::~CInsertDeleteNodes()
{
}

HRESULT CInsertDeleteNodes::FinalConstruct()
{
	return CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pTrackerNodes);
}

STDMETHODIMP CInsertDeleteNodes::get_Name(/* [out, retval] */ BSTR *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = ::SysAllocString(L"InsertDeleteNodes");

	return S_OK;
}

STDMETHODIMP CInsertDeleteNodes::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_FALSE;

	if (m_bEnabled)
	{
		int xPos = 0;
		int yPos = 0;
		short zDelta = 0;

		switch (uMsg)
		{
		case WM_LBUTTONUP:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnLButtonUp(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_RBUTTONUP:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnRButtonUp(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_MOUSEMOVE:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			if (OnMouseMove(wParam, xPos, yPos))
			{
				hr = S_OK;
			}
			break;

		case WM_MOUSEWHEEL:
			{
				zDelta = HIWORD(wParam);
				if (OnMouseWheel(LOWORD(wParam), zDelta, LOWORD(lParam), HIWORD(lParam)))
				{
					hr = S_OK;
				}
			}
			break;

		case WM_KEYDOWN:
			if (OnKeyDown(wParam))
			{
				hr = S_OK;
			}
			break;
		}
	}

	return hr;
}

STDMETHODIMP CInsertDeleteNodes::InsertNode(/* [in] */ IVisual* pVisualNode, /* [in] */ BSTR strParent)
{
	if (pVisualNode == NULL)
	{
		return E_POINTER;
	}

	if (m_pController == NULL)
	{
		return E_FAIL;  // Behavior not initialized
	}

	HRESULT hr = E_FAIL;

	long nCurState;
	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::READY)
	{
		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
			m_pViewport = NULL;
		}

		// Cache a pointer to the viewport to avoid repetitive calls
		hr = m_pController->get_Viewport(&m_pViewport);

		if (SUCCEEDED(hr))
		{
			// Make sure previous insert node is released
			if (m_pInsertNode != NULL)
			{
				m_pInsertNode->Release();
				m_pInsertNode = NULL;
			}

			m_pInsertNode = pVisualNode;
			m_pInsertNode->AddRef();

			// Get current camera location and orientation and cache it
			GetCameraInfo();

			// Set default location for insert
			D3DXVECTOR3 initPos = m_cameraLoc + (m_cameraAim * 6);
			VARIANT varOrigin;
			m_pInsertNode->GetOrigin(&varOrigin);
			vtPOINT3D ptOrg(varOrigin);
			D3DXVECTOR3 initMove = initPos - (D3DXVECTOR3) ptOrg;
			::VariantClear(&varOrigin);
			m_pInsertNode->Translate(initMove.x, initMove.y, initMove.z);

			// Add tracker for node being inserted
			m_pTrackerNodes->Clear();
			AddMoveTracker(m_pInsertNode);

			BeginTrackingMove();

			m_strInsertParent = strParent;

			m_pController->put_CurrentState(ControllerState::ADDING_NODES);

			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CInsertDeleteNodes::EndInsert()
{
	if (m_pController == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = S_FALSE;

	long nCurState;
	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::ADDING_NODES)
	{
		hr = DoInsertCommand();

		if (m_pInsertNode != NULL)
		{
			m_pInsertNode->Release();
			m_pInsertNode = NULL;
		}

		m_pTrackerNodes->Clear();

		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
			m_pViewport = NULL;
		}

		RestoreCursor();
		m_pController->put_CurrentState(ControllerState::READY);
	}

	return hr;
}

STDMETHODIMP CInsertDeleteNodes::CancelInsert()
{
	if (m_pController == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr = S_FALSE;

	long nCurState;
	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::ADDING_NODES)
	{
		m_pController->Update(ControllerUpdate::VIEWPORT, NULL);

		if (m_pInsertNode != NULL)
		{
			m_pInsertNode->Release();
			m_pInsertNode = NULL;
		}

		m_pTrackerNodes->Clear();

		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
			m_pViewport = NULL;
		}

		RestoreCursor();
		m_pController->put_CurrentState(ControllerState::READY);

		hr = S_OK;
	}

	return hr;
}

STDMETHODIMP CInsertDeleteNodes::SetController(/* [in] */ IController* pController)
{
	HRESULT hr = _BaseBehavior::SetController(pController);

	if (SUCCEEDED(hr))
	{
		if (m_pController != NULL)
		{
			hr = m_pController->get_SelectionList(&m_pSelectedNodes);

			if (SUCCEEDED(hr) && m_pSelectedNodes != NULL)
			{
				m_pSelectedNodes->Release();   // Weak reference - don't reference count
			}
		}
	}

	return hr;
}

bool CInsertDeleteNodes::OnLButtonUp(const WPARAM fwKeys, const int xPos, const int yPos)
{
	fwKeys;
	xPos;
	yPos;

	bool bHandled = false;

	if (EndInsert() == S_OK)
	{
		bHandled = true;
	}

	return bHandled;
}

bool CInsertDeleteNodes::OnRButtonUp(const WPARAM fwKeys, const int xPos, const int yPos)
{
	fwKeys;
	xPos;
	yPos;

	if (m_pController == NULL)
	{
		return false;
	}

	bool bHandled = false;

	if (CancelInsert() == S_OK)
	{
		bHandled = true;
	}

	return bHandled;
}

bool CInsertDeleteNodes::OnMouseMove(const DWORD fwKeys, const int xPos, const int yPos)
{
	fwKeys;

	long nCurState;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::ADDING_NODES)
	{
		TrackMouseMove(xPos, yPos);
	}

	return true;
}

bool CInsertDeleteNodes::OnMouseWheel(const UINT nFlags, const short zDelta, const int xPos, const int yPos)
{
	nFlags;
	xPos;
	yPos;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	long nCurState;
	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::ADDING_NODES)
	{
		MoveZ(zDelta / 120);
	}

	return true;
}

bool CInsertDeleteNodes::OnKeyDown(const WPARAM vk)
{
	bool bHandled = false;
	long nCurState;

	if (m_pController == NULL)
	{
		return false;  // Behavior not initialized
	}

	m_pController->get_CurrentState(&nCurState);

	if (nCurState == ControllerState::ADDING_NODES)
	{
		switch (vk)
		{
		case 'S':
			MoveZ(1);
			bHandled = true;
			break;

		case 'W':
			MoveZ(-1);
			bHandled = true;
			break;

		case VK_UP:
			MoveY(1);
			bHandled = true;
			break;

		case VK_DOWN:
			MoveY(-1);
			bHandled = true;
			break;

		case VK_LEFT:
			MoveX(-1);
			bHandled = true;
			break;

		case VK_RIGHT:
			MoveX(1);
			bHandled = true;
			break;

		case VK_RETURN:
			if (EndInsert() == S_OK)
			{
				bHandled = true;
			}
			break;

		case VK_ESCAPE:
			if (CancelInsert() == S_OK)
			{
				bHandled = true;
			}
			break;
		}
	}
	else if (nCurState == ControllerState::READY)
	{
		if (vk == VK_DELETE)
		{
			if (SUCCEEDED(DoDeleteCommand()))
			{
				bHandled = true;
			}
		}
	}

	return bHandled;
}

HRESULT CInsertDeleteNodes::DoInsertCommand()
{
	HRESULT hr = E_FAIL;

	if (m_pController != NULL && m_pInsertNode != NULL)
	{
		IInsertCommand* pCmd = NULL;
		hr = CInsertCommand::_CreatorClass::CreateInstance(NULL, IID_IInsertCommand, (LPVOID*) &pCmd);

		if (SUCCEEDED(hr))
		{
			m_pInsertNode->Translate(m_moveVector.x, m_moveVector.y, m_moveVector.z);
			pCmd->AddNode(m_pInsertNode);
			hr = m_pController->Do(pCmd);
			pCmd->Release();
		}
	}

	return hr;
}

HRESULT CInsertDeleteNodes::DoDeleteCommand()
{
	HRESULT hr = E_FAIL;

	if (m_pController != NULL && m_pSelectedNodes != NULL)
	{
		INodeCommand* pCmd = NULL;
		hr = CDeleteCommand::_CreatorClass::CreateInstance(NULL, IID_INodeCommand, (LPVOID*) &pCmd);

		if (SUCCEEDED(hr))
		{
			pCmd->AddNodes(m_pSelectedNodes);
			hr = m_pController->Do(pCmd);
			pCmd->Release();
		}
	}

	return hr;
}

};  // namespace ShapeShifter
