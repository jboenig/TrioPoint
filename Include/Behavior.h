//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Behavior.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IBehaviorImpl

template <class base_class>
class IBehaviorImpl : public base_class
{
// Constructors/destructor
public:
	IBehaviorImpl()
	{
		m_pController = NULL;
		m_bEnabled = true;
	}

// Attributes
public:
	IController* m_pController;   // Weak reference - not reference counted
	bool m_bEnabled;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IBehavior interface

	STDMETHOD(SetController)(/* [in] */ IController* pController)
	{
		// This is a weak reference so it is not AddRef'd
		m_pController = pController;
		return S_OK;
	}

	STDMETHOD(get_Enabled)(/* [out, retval] */ VARIANT_BOOL *pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = m_bEnabled ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(put_Enabled)(/* [int] */ VARIANT_BOOL newVal)
	{
		m_bEnabled = newVal ? true : false;
		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////
// CCameraFly

class ATL_NO_VTABLE CCameraFly : public CComObjectRootEx<CComSingleThreadModel>,
                                 public CComCoClass<CCameraFly, &CLSID_CameraFly>,
                                 public IDispatchImpl< IBehaviorImpl<ICameraFly> , &__uuidof(ICameraFly), &LIBID_ShapeShifter>,
                                 public IAnimate,
                                 public IInputDeviceUserDX8
{
// Embedded types
public:
	typedef IDispatchImpl< IBehaviorImpl<ICameraFly> , &__uuidof(ICameraFly), &LIBID_ShapeShifter> _BaseBehavior;

// Constructors/destructor
public:
	CCameraFly();
	virtual ~CCameraFly();

DECLARE_REGISTRY_RESOURCEID(IDR_CAMERA_FLY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CCameraFly)
	COM_INTERFACE_ENTRY(ICameraFly)
	COM_INTERFACE_ENTRY(IBehavior)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IAnimate)
	COM_INTERFACE_ENTRY_IID(IID_IInputDeviceUserDX8, IInputDeviceUserDX8)
END_COM_MAP()

// Attributes
protected:
	VALUE3D m_fVelocityDistance;
	DWORD m_dwVelocityTime;
	DWORD m_dwFirstMove;
	DWORD m_dwLastMove;
	DWORD m_dwRotateTime;
	float m_fRotateDistance;
	LPDIRECTINPUTDEVICE8 m_pKeyboard;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IBehavior interface

	STDMETHOD(get_Name)(/* [out, retval] */ BSTR *pVal);

	//////////////////////////////////////////////////////////////
	// ICameraFly interface

	STDMETHOD(get_RotateDistance)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_RotateDistance)(/*[in]*/ float newVal);
	STDMETHOD(get_RotateTime)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_RotateTime)(/*[in]*/ long newVal);

	//////////////////////////////////////////////////////////////
	// ICameraFly interface

	STDMETHOD(get_VelocityDistance)(/* [out, retval] */ VALUE3D *pVal);
	STDMETHOD(put_VelocityDistance)(/* [in] */ VALUE3D newVal);
	STDMETHOD(get_VelocityTime)(/* [out, retval] */ long *pVal);
	STDMETHOD(put_VelocityTime)(/* [in] */ long newVal);

	//////////////////////////////////////////////////////////////
	// IAnimate interface

	STDMETHOD(Animate)(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame);

	//////////////////////////////////////////////////////////////
	// IInputDeviceUserDX8 interface

	STDMETHOD(AcquireInputDevices)(IInputDeviceProviderDX8* pDevProvider);
	STDMETHOD(FreeInputDevices)();
};

/////////////////////////////////////////////////////////////////////////
// CCameraStep

class ATL_NO_VTABLE CCameraStep : public CComObjectRootEx<CComSingleThreadModel>,
                                  public CComCoClass<CCameraStep, &CLSID_CameraStep>,
                                  public IDispatchImpl< IBehaviorImpl<ICameraStep> , &__uuidof(ICameraStep), &LIBID_ShapeShifter>,
                                  public IWndMessageReceiver
{
// Constructors/destructor
public:
	CCameraStep();

DECLARE_REGISTRY_RESOURCEID(IDR_CAMERA_STEP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CCameraStep)
	COM_INTERFACE_ENTRY(ICameraStep)
	COM_INTERFACE_ENTRY(IBehavior)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
END_COM_MAP()

// Attributes
protected:
	VALUE3D m_fMoveIncrement;
	VALUE3D m_fRotateIncrement;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IBehavior interface

	STDMETHOD(get_Name)(/* [out, retval] */ BSTR *pVal);

	//////////////////////////////////////////////////////////////
	// ICameraStep interface

	STDMETHOD(get_MoveIncrement)(/* [out, retval] */ VALUE3D *pVal);
	STDMETHOD(put_MoveIncrement)(/* [in] */ VALUE3D newVal);
	STDMETHOD(get_RotateIncrement)(/* [out, retval] */ VALUE3D *pVal);
	STDMETHOD(put_RotateIncrement)(/* [in] */ VALUE3D newVal);

	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	bool OnKeyDown(const WPARAM vk);
	bool OnMouseWheel(const UINT nFlags, const short zDelta, const int xPos, const int yPos);
};

/////////////////////////////////////////////////////////////////////////
// CMouseSelect

class ATL_NO_VTABLE CMouseSelect : public CComObjectRootEx<CComSingleThreadModel>,
                                   public CComCoClass<CMouseSelect, &CLSID_MouseSelect>,
                                   public IDispatchImpl< IBehaviorImpl<ISelectNodes> , &__uuidof(ISelectNodes), &LIBID_ShapeShifter>,
                                   public IWndMessageReceiver
{
// Embedded types
public:
	typedef IDispatchImpl< IBehaviorImpl<ISelectNodes> , &__uuidof(ISelectNodes), &LIBID_ShapeShifter> _BaseBehavior;

// Constructors/destructor
public:
	CMouseSelect();
	virtual ~CMouseSelect();

DECLARE_REGISTRY_RESOURCEID(IDR_MOUSE_SELECT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CMouseSelect)
	COM_INTERFACE_ENTRY(IBehavior)
	COM_INTERFACE_ENTRY(ISelectNodes)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
protected:
	INodes* m_pSelectedNodes;
	INodes* m_pNodesHit;
	bool m_bMultiSelect;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IBehavior interface

	STDMETHOD(SetController)(/* [in] */ IController* pController);
	STDMETHOD(get_Name)(/* [out, retval] */ BSTR *pVal);

	//////////////////////////////////////////////////////////////
	// ISelectNodes interface

	STDMETHOD(get_MultiSelect)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_MultiSelect)(/* [in] */ VARIANT_BOOL newVal);

	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	bool OnLButtonDown(const WPARAM fwKeys, const int xPos, const int yPos);
	bool OnLButtonDblClk(const WPARAM fwKeys, const int xPos, const int yPos);
	INode* GetNearestNodeHit();
};

/////////////////////////////////////////////////////////////////////////
// TrackMoveImpl

template <class T>
class TrackMoveImpl
{
// Constructors/destructor
public:
	TrackMoveImpl() :
		m_pTrackerNodes(NULL),
		m_fMoveIncrement(0.5f)
	{
	}

	virtual ~TrackMoveImpl()
	{
		if (m_pTrackerNodes != NULL)
		{
			m_pTrackerNodes->Clear();
			m_pTrackerNodes->Release();
		}
	}

// Attributes
protected:
	INodes* m_pTrackerNodes;
	POINT3D m_prevMousePt;
	VECTOR3D m_prevMouseDir;
	VALUE3D m_fMoveIncrement;
	D3DXVECTOR3 m_moveVector;
	D3DXVECTOR3 m_cameraLoc;
	D3DXVECTOR3 m_cameraAim;
	D3DXVECTOR3 m_cameraUp;
	D3DXVECTOR3 m_cameraRight;
	HCURSOR m_hPrevCursor;

// Operations
public:

	HRESULT TrackMoveInit()
	{
		return CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pTrackerNodes);
	}

	HRESULT AddMoveTracker(INode* pNode)
	{
		IVisual* pTracker = NULL;
		HRESULT hr = CTracker::_CreatorClass::CreateInstance(NULL, IID_IVisual, (LPVOID*) &pTracker);
		if (SUCCEEDED(hr))
		{
			m_pTrackerNodes->Add(pTracker);
			pTracker->AppendChild(pNode);
			pTracker->Release();
		}
		return hr;
	}

	HRESULT GetCameraInfo()
	{
		HRESULT hr = E_FAIL;

		if (static_cast<T*>(this)->m_pViewport != NULL)
		{
			// Cache camera position and orientation for convenience and speed
			ICamera* pCamera = NULL;
			if (static_cast<T*>(this)->m_pViewport->get_Camera(&pCamera) == S_OK)
			{
				pCamera->GetLocation(&m_cameraLoc.x, &m_cameraLoc.y, &m_cameraLoc.z);
				pCamera->GetAimVector(&m_cameraAim.x, &m_cameraAim.y, &m_cameraAim.z);
				pCamera->GetUpVector(&m_cameraUp.x, &m_cameraUp.y, &m_cameraUp.z);
				D3DXVec3Cross(&m_cameraRight, &m_cameraUp, &m_cameraAim);
				D3DXVec3Normalize(&m_cameraRight, &m_cameraRight);
				pCamera->Release();
				hr = S_OK;
			}
		}

		return hr;
	}

	HRESULT BeginTrackingMove()
	{
		HRESULT hr = E_FAIL;

		// Initialize move vector
		m_moveVector.x = m_moveVector.y = m_moveVector.z = 0.0f;

		if (static_cast<T*>(this)->m_pViewport != NULL)
		{
			// Cache camera position and orientation for convenience and speed
			GetCameraInfo();

			// Get screen coordinates of tracker object and initialize the cursor
			POINT ptTracker;
			GetTrackerPos(ptTracker);
			::SetCursorPos(ptTracker.x, ptTracker.y);
			SetMoveCursor(ptTracker);

			// Convert screen coordinates to a ray in world coordinates
			_variant_t varRayOrigin;
			_variant_t varRayDirection;

			hr = static_cast<T*>(this)->m_pViewport->ScreenToWorld(ptTracker.x, ptTracker.y, &varRayOrigin, &varRayDirection);

			if (hr == S_OK)
			{
				// Save ray as starting point for drag operation
				m_prevMousePt = vtPOINT3D(varRayOrigin);
				m_prevMouseDir = vtVECTOR3D(varRayDirection);
			}
		}

		return hr;
	}

	HRESULT TrackMouseMove(const int xPos, const int yPos)
	{
		if (static_cast<T*>(this)->m_pController == NULL)
		{
			return E_FAIL;  // Behavior not initialized
		}

		HRESULT hr = E_FAIL;

		_variant_t varRayOrigin;
		_variant_t varRayDirection;

		POINT3D curPt;
		VECTOR3D curDir;

		// Move on XY axes
		if (static_cast<T*>(this)->m_pViewport != NULL)
		{
			hr = static_cast<T*>(this)->m_pViewport->ScreenToWorld(xPos, yPos, &varRayOrigin, &varRayDirection);

			if (hr == S_OK)
			{
				curPt = vtPOINT3D(varRayOrigin);
				curDir = vtVECTOR3D(varRayDirection);

				INode* pCurNode = NULL;
				INodesAccessor<INode> trackerNodes(m_pTrackerNodes);

				HRESULT hrLoop = trackerNodes.GetFirst(&pCurNode);

#if 0
				while (hrLoop == S_OK)
#else
				if (hrLoop == S_OK)
#endif
				{
					// Get plane intersecting node that is parallel to the
					// near and far plane of the frustum
					PLANE3D viewingPlane;
					GetViewingPlane(pCurNode, viewingPlane);

					// Get intersection of starting ray and viewing plane of component
					D3DXVECTOR3 ptStart;
					D3DXVECTOR3 startPtNear = m_prevMousePt;
					D3DXVECTOR3 startPtFar = startPtNear + ((D3DXVECTOR3) m_prevMouseDir * 50);
					viewingPlane.GetLineIntersection(ptStart, startPtNear, startPtFar);

					// Get intersection of starting ray and viewing plane of component
					D3DXVECTOR3 ptEnd;
					D3DXVECTOR3 endPtNear = curPt;
					D3DXVECTOR3 endPtFar = endPtNear + ((D3DXVECTOR3) curDir * 50);
					viewingPlane.GetLineIntersection(ptEnd, endPtNear, endPtFar);

					D3DXVECTOR3 delta = ptEnd - ptStart;
					D3DXVec3Add(&m_moveVector, &m_moveVector, &delta);

					pCurNode->Release();
					pCurNode = NULL;

#if 0
					hrLoop = trackerNodes.GetNext(&pCurNode);
#endif
				}

				m_prevMousePt = curPt;
				m_prevMouseDir = curDir;

				hr = TrackMove();
			}
		}

		return hr;
	}

	HRESULT MoveX(const int nSteps = 1)
	{
		D3DXVECTOR3 delta = m_cameraRight * (m_fMoveIncrement * nSteps);
		D3DXVec3Add(&m_moveVector, &m_moveVector, &delta);
		return TrackMove();
	}

	HRESULT MoveY(const int nSteps = 1)
	{
		D3DXVECTOR3 delta = m_cameraUp * (m_fMoveIncrement * nSteps);
		D3DXVec3Add(&m_moveVector, &m_moveVector, &delta);
		return TrackMove();
	}

	HRESULT MoveZ(const int nSteps = 1)
	{
		D3DXVECTOR3 delta = m_cameraAim * (m_fMoveIncrement * nSteps);
		D3DXVec3Add(&m_moveVector, &m_moveVector, &delta);
		return TrackMove();
	}

	HRESULT TrackMove()
	{
		HRESULT hr = S_OK;

		if (static_cast<T*>(this)->m_pViewport != NULL)
		{
			INode* pCurNode = NULL;
			INodesAccessor<INode> trackerNodes(m_pTrackerNodes);

			HRESULT hrLoop = trackerNodes.GetFirst(&pCurNode);

			while (hrLoop == S_OK)
			{
				IVisual* pCurVisual = NULL;
				if (SUCCEEDED(pCurNode->QueryInterface(IID_IVisual, (LPVOID*) &pCurVisual)))
				{
					pCurVisual->Translate(m_moveVector.x, m_moveVector.y, m_moveVector.z);
					pCurVisual->Release();
				}

				pCurNode->Release();
				pCurNode = NULL;

				hrLoop = trackerNodes.GetNext(&pCurNode);
			}

			static_cast<T*>(this)->m_pViewport->DrawTracking(m_pTrackerNodes);
		}

		return hr;
	}

	HRESULT EndTrackingMove()
	{
		RestoreCursor();
		return S_OK;
	}

	void GetTrackerPos(POINT& ptTracker)
	{
		VARIANT varOrigin;
		int screenX, screenY;
		INodesAccessor<IVisual> trackerNodes(m_pTrackerNodes);

		if (static_cast<T*>(this)->m_pViewport != NULL)
		{
			IVisual* pTracker = trackerNodes.GetNodeAt(0);
			if (pTracker != NULL)
			{
				pTracker->GetOrigin(&varOrigin);
				static_cast<T*>(this)->m_pViewport->WorldToScreen(varOrigin, &screenX, &screenY);

				ptTracker.x = screenX;
				ptTracker.y = screenY;

				pTracker->Release();
			}
		}
	}

	void SetMoveCursor(const POINT& ptTracker)
	{
		POINT ptCursor = ptTracker;
		IViewport* pViewport = NULL;
		HWND hWnd = NULL;

		if (static_cast<T*>(this)->m_pController != NULL)
		{
			if (static_cast<T*>(this)->m_pController->get_Viewport(&pViewport) == S_OK)
			{
				pViewport->get_ContainerWindow((long*)&hWnd);
				::SetCapture(hWnd);

				::ClientToScreen(hWnd, &ptCursor);

				HCURSOR hCur = ::LoadCursor(NULL, IDC_SIZEALL);
				m_hPrevCursor = ::SetCursor(hCur);

				::SetCursorPos(ptCursor.x, ptCursor.y);

				pViewport->Release();
			}
		}
	}

	void RestoreCursor()
	{
		::SetCursor(m_hPrevCursor);
		::ReleaseCapture();
	}

	void GetViewingPlane(INode* pNode, PLANE3D& plane)
	{
		// This function returns a plane that intersects the given node and
		// is parallel to the near and far planes of the frustum.

		IVisual* pVisual = NULL;
		if (SUCCEEDED(pNode->QueryInterface(IID_IVisual, (LPVOID*) &pVisual)))
		{
			_variant_t varOriginPt;
			pVisual->GetOrigin(&varOriginPt);
			POINT3D ptOrigin = vtPOINT3D(varOriginPt);

			D3DXVECTOR3 aimVector;
			ICamera* pCamera = NULL;
			if (SUCCEEDED(static_cast<T*>(this)->m_pViewport->get_Camera(&pCamera)))
			{
				pCamera->GetAimVector(&aimVector.x, &aimVector.y, &aimVector.z);
				pCamera->Release();
			}

			D3DXVECTOR3 ptPlane = (D3DXVECTOR3) ptOrigin;
			plane.FromPointNormal(ptPlane, aimVector);

			pVisual->Release();
		}
	}
};

/////////////////////////////////////////////////////////////////////////
// CDragDrop

class ATL_NO_VTABLE CDragDrop : public CComObjectRootEx<CComSingleThreadModel>,
                                public CComCoClass<CDragDrop, &CLSID_DragDrop>,
                                public IDispatchImpl< IBehaviorImpl<IMoveNodes> , &__uuidof(IMoveNodes), &LIBID_ShapeShifter>,
                                public TrackMoveImpl<CDragDrop>,
                                public IWndMessageReceiver
{
// Embedded types
public:
	typedef IDispatchImpl< IBehaviorImpl<IMoveNodes> , &__uuidof(IMoveNodes), &LIBID_ShapeShifter> _BaseBehavior;

// Constructors/destructor
public:
	CDragDrop();
	virtual ~CDragDrop();

DECLARE_REGISTRY_RESOURCEID(IDR_DRAG_DROP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CDragDrop)
	COM_INTERFACE_ENTRY(IMoveNodes)
	COM_INTERFACE_ENTRY(IBehavior)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
public:
	INodes* m_pSelectedNodes;
	INodes* m_pMoveSet;
	IViewport* m_pViewport;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IMoveNodes interface

	STDMETHOD(get_MoveIncrement)(/* [out, retval] */ VALUE3D *pVal);
	STDMETHOD(put_MoveIncrement)(/* [in] */ VALUE3D newVal);
	STDMETHOD(BeginMove)();
	STDMETHOD(EndMove)();
	STDMETHOD(CancelMove)();

	//////////////////////////////////////////////////////////////
	// IBehavior interface

	STDMETHOD(SetController)(/* [in] */ IController* pController);
	STDMETHOD(get_Name)(/* [out, retval] */ BSTR *pVal);

	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	bool OnLButtonDown(const WPARAM fwKeys, const int xPos, const int yPos);
	bool OnRButtonDown(const WPARAM fwKeys, const int xPos, const int yPos);
	bool OnMouseMove(const DWORD fwKeys, const int xPos, const int yPos);
	bool OnKeyDown(const WPARAM vk);
	bool OnMouseWheel(const UINT nFlags, const short zDelta, const int xPos, const int yPos);

	HRESULT PrepareMoveSet();
	HRESULT DoMoveCommand();
};

/////////////////////////////////////////////////////////////////////////
// CInsertDeleteNodes

class ATL_NO_VTABLE CInsertDeleteNodes : public CComObjectRootEx<CComSingleThreadModel>,
                                         public CComCoClass<CInsertDeleteNodes, &CLSID_InsertDeleteNodes>,
                                         public IDispatchImpl< IBehaviorImpl<IInsertDeleteNodes> , &__uuidof(IInsertDeleteNodes), &LIBID_ShapeShifter>,
                                         public TrackMoveImpl<CInsertDeleteNodes>,
                                         public IWndMessageReceiver
{
// Embedded types
public:
	typedef IDispatchImpl< IBehaviorImpl<IInsertDeleteNodes> , &__uuidof(IInsertDeleteNodes), &LIBID_ShapeShifter> _BaseBehavior;

// Constructors/destructor
public:
	CInsertDeleteNodes();
	virtual ~CInsertDeleteNodes();

DECLARE_REGISTRY_RESOURCEID(IDR_INSERTDELETE_NODES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CInsertDeleteNodes)
	COM_INTERFACE_ENTRY(IInsertDeleteNodes)
	COM_INTERFACE_ENTRY(IBehavior)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
END_COM_MAP()

	HRESULT FinalConstruct();

// Attributes
public:
	INodes* m_pSelectedNodes;
	IViewport* m_pViewport;
	IVisual* m_pInsertNode;
	_bstr_t m_strInsertParent;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IInsertDeleteNodes interface

	STDMETHOD(InsertNode)(/* [in] */ IVisual* pVisualNode, /* [in] */ BSTR strParent);
	STDMETHOD(EndInsert)();
	STDMETHOD(CancelInsert)();

	//////////////////////////////////////////////////////////////
	// IBehavior interface

	STDMETHOD(SetController)(/* [in] */ IController* pController);
	STDMETHOD(get_Name)(/* [out, retval] */ BSTR *pVal);

	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	bool OnLButtonDown(const WPARAM fwKeys, const int xPos, const int yPos);
	bool OnLButtonUp(const WPARAM fwKeys, const int xPos, const int yPos);
	bool OnRButtonUp(const WPARAM fwKeys, const int xPos, const int yPos);
	bool OnMouseMove(const DWORD fwKeys, const int xPos, const int yPos);
	bool OnMouseWheel(const UINT nFlags, const short zDelta, const int xPos, const int yPos);
	bool OnKeyDown(const WPARAM vk);

	HRESULT DoInsertCommand();
	HRESULT DoDeleteCommand();
};

};  // namespace ShapeShifter

#endif // #ifndef __BEHAVIOR_H__

