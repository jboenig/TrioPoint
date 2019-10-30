//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Controller.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#pragma once

#include <math.h>
#include <vector>
#include "ShapeShifter.h"
#include "CommandDispatcher.h"
#include "ControllerStates.h"
#include "NodeCollection.h"
#include "ShapeShifterCP.h"
#include "InputDevice.h"
#include "EventSink.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IControllerImpl

template <class base_class>
class IControllerImpl : public base_class,
                        public IInputDeviceProviderDX8
{
// Embedded types
public:
	typedef std::vector<IBehavior*> _Behaviors;
	typedef std::vector<IAnimate*> _AnimationBehaviors;
	typedef std::vector<IWndMessageReceiver*> _WndMessageBehaviors;

// Constructors/destructor
public:
	IControllerImpl() :
		m_pModel(NULL),
		m_pViewport(NULL),
		m_pSelectionList(NULL)
	{
		m_nCurrentState = ControllerState::READY;
	}

	virtual ~IControllerImpl()
	{
		while (m_behaviors.size() > 0)
		{
			IBehavior* pBehavior = m_behaviors.back();
			pBehavior->SetController(NULL);
			pBehavior->Release();
			m_behaviors.pop_back();
		}

		while (m_animationBehaviors.size() > 0)
		{
			IAnimate* pAnimate = m_animationBehaviors.back();
			pAnimate->Release();
			m_animationBehaviors.pop_back();
		}

		while (m_wndMessageBehaviors.size() > 0)
		{
			IWndMessageReceiver* pMsgRcvr = m_wndMessageBehaviors.back();
			pMsgRcvr->Release();
			m_wndMessageBehaviors.pop_back();
		}

		if (m_pSelectionList != NULL)
		{
			m_pSelectionList->Release();
		}

		if (m_pModel != NULL)
		{
			m_pModel->Release();
		}

		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
		}
	}

// Attributes
protected:
	long m_nCurrentState;
	IModel* m_pModel;
	IViewport* m_pViewport;
	INodes* m_pSelectionList;
	_Behaviors m_behaviors;
	_AnimationBehaviors m_animationBehaviors;
	_WndMessageBehaviors m_wndMessageBehaviors;
	CInputDevicesDX8 m_inputDevices;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IController interface

	STDMETHOD(get_Viewport)(/* [out, retval] */ IViewport** ppVal)
	{
		if (ppVal != NULL)
		{
			*ppVal = m_pViewport;

			if (*ppVal != NULL)
			{
				(*ppVal)->AddRef();
			}

			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(putref_Viewport)(/* [in] */ IViewport* pViewport)
	{
		// Addref the new viewport
		if (pViewport != NULL)
		{
			pViewport->AddRef();
		}

		// Release the old viewport
		if (m_pViewport != NULL)
		{
			m_pViewport->Release();
		}

		// Assign new viewport
		m_pViewport = pViewport;

		// Release old model
		SetModel(NULL);

		// Assign new model
		if (m_pViewport != NULL)
		{
			IModel* pModel = NULL;
			if (m_pViewport->get_Model(&pModel) == S_OK)
			{
				SetModel(pModel);
				pModel->Release();
			}
		}

		if (m_pViewport != NULL)
		{
			// Attach the controller's selection list to the viewport
			// so that the viewport can highlight selected items
			m_pViewport->SetSelectionList(m_pSelectionList);
		}

		return S_OK;
	}

	STDMETHOD(get_Model)(/* [out, retval] */ IModel** ppVal)
	{
		HRESULT hr = E_FAIL;

		if (ppVal != NULL)
		{
			if (m_pViewport != NULL)
			{
				hr = m_pViewport->get_Model(ppVal);
			}
		}

		return hr;
	}

	STDMETHOD(get_SelectionList)(/* [out, retval] */ INodes** ppVal)
	{
		if (ppVal == NULL)
		{
			return E_POINTER;
		}

		*ppVal = m_pSelectionList;

		if (*ppVal != NULL)
		{
			(*ppVal)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(get_CurrentState)(/* [out, retval] */ long* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = static_cast<long>(m_nCurrentState);

		return S_OK;
	}

	STDMETHOD(put_CurrentState)(/* [in] */ long newVal)
	{
		m_nCurrentState = static_cast<WORD>(newVal);

#if 0
		HCURSOR hCur = NULL;

		switch (m_nCurrentState)
		{
		case ControllerState::READY:
			hCur = LoadCursor(NULL, IDC_ARROW);
			SetCursor(hCur);
			break;

		case ControllerState::ADDING_NODES:
			hCur = LoadCursor(NULL, IDC_CROSS);
			SetCursor(hCur);
			break;
		}
#endif

		return S_OK;
	}

	STDMETHOD(GetBehavior)(/* [in] */ BSTR strName, /* [out,retval] */ IBehavior** ppBehavior)
	{
		*ppBehavior = NULL;

		// strName may be a match or this may just be querying for the existence
		// of a behvaior
		HRESULT hr = S_FALSE;

		// Look through m_behaviors
		_Behaviors::iterator itBehaviors;

		for (itBehaviors = m_behaviors.begin(); itBehaviors != m_behaviors.end(); itBehaviors++)
		{
			BSTR bstrBehaviorName;
			hr = (*itBehaviors)->get_Name(&bstrBehaviorName);
			if(SUCCEEDED(hr))
			{
				bool bMatch = (wcsicmp(strName, bstrBehaviorName) == 0);
				::SysFreeString(bstrBehaviorName);
				if(bMatch)
				{
					*ppBehavior = *itBehaviors;
					(*ppBehavior)->AddRef();
					return S_OK;
				}
			}
		}

		// If we failed along the way, return the failure code
		// otherwise return a failed match
		if(SUCCEEDED(hr))
		{
			return S_FALSE;
		}
		else
		{
			return hr;
		}
	}

	STDMETHOD(AttachInputDevice)(/* [in] */ IInputDevice* pInputDev)
	{
		if (pInputDev == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_OK;
		IUnknown* pDIDeviceUnk = NULL;
		LPDIRECTINPUTDEVICE8 pDIDevice = NULL;
		enum InputDeviceCategory devCat;

		// Get device type
		hr = pInputDev->get_DeviceCategory(&devCat);

		if (SUCCEEDED(hr))
		{
			// Get IUnknown pointer to DirectInput device
			hr = pInputDev->GetDIDevice(&pDIDeviceUnk);

			if (SUCCEEDED(hr))
			{
				// Query IUnknown pointer for IDirectInputDevice8 interface
				hr = pDIDeviceUnk->QueryInterface(IID_IDirectInputDevice8, (LPVOID*) &pDIDevice);
				if (SUCCEEDED(hr))
				{
					// Add pointer to DirectInput device to the device array
					m_inputDevices.SetDevice(devCat, pDIDevice);
					// Notify all device users that a change has occurred
					UpdateInputDeviceUsers();
					pDIDevice->Release();
				}
				pDIDeviceUnk->Release();
			}
		}

		return hr;
	}

	STDMETHOD(DetachInputDevice)(/* [in] */ enum InputDeviceCategory devCat)
	{
		devCat;
		return E_NOTIMPL;
	}

	STDMETHOD(MoveCamera)(/* [in] */ enum CameraMovement moveType,
	                      /* [in] */ VALUE3D fAmount)
	{
		HRESULT hr = E_FAIL;
		ICamera* pCamera = NULL;

		if (m_pViewport != NULL)
		{
			hr = m_pViewport->get_Camera(&pCamera);

			if (hr == S_OK)
			{
				hr = pCamera->Move(moveType, fAmount);
				pCamera->Release();
			}
		}

		return hr;
	}

	STDMETHOD(TranslateCamera)(/* [in] */ VALUE3D dx, /* [in] */ VALUE3D dy, /* [in] */ VALUE3D dz)
	{
		HRESULT hr = E_FAIL;
		ICamera* pCamera = NULL;

		if (m_pViewport != NULL)
		{
			hr = m_pViewport->get_Camera(&pCamera);

			if (hr == S_OK)
			{
				hr = pCamera->Translate(dx, dy, dz);
				pCamera->Release();
			}
		}

		return hr;
	}

	STDMETHOD(RotateCamera)(/* [in] */ VALUE3D fXRadians,
	                        /* [in] */ VALUE3D fYRadians,
	                        /* [in] */ VALUE3D fZRadians)
	{
		HRESULT hr = E_FAIL;
		ICamera* pCamera = NULL;

		if (m_pViewport != NULL)
		{
			hr = m_pViewport->get_Camera(&pCamera);

			if (hr == S_OK)
			{
				hr = pCamera->Rotate(fXRadians, fYRadians, fZRadians);
				pCamera->Release();
			}
		}

		return hr;
	}

	STDMETHOD(Update)(/* [in] */ long nUpdateFlags, /* [in] */ INodes* pNodesAffected)
	{
		// Default behavior is to simply refresh the viewport. See
		// derived classes for more specific behavior.

		nUpdateFlags;
		pNodesAffected;

		if (m_pViewport != NULL)
		{
			m_pViewport->Render();
			m_pViewport->RefreshScreen();
		}

		return S_OK;
	}

	//////////////////////////////////////////////////////////////
	// IInputDeviceProviderDX8 interface

	STDMETHOD(GetInputDevice)(enum InputDeviceCategory devCat,
	                          LPDIRECTINPUTDEVICE8* ppDIDevice)
	{
		if (ppDIDevice == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		*ppDIDevice = m_inputDevices[devCat];

		if (*ppDIDevice != NULL)
		{
			(*ppDIDevice)->AddRef();
			hr = S_OK;
		}

		return hr;
	}

	void UpdateInputDeviceUsers()
	{
		_Behaviors::iterator itBehavior;
		IInputDeviceUserDX8* pDevUser = NULL;

		for (itBehavior = m_behaviors.begin(); itBehavior != m_behaviors.end(); itBehavior++)
		{
			if (SUCCEEDED((*itBehavior)->QueryInterface(IID_IInputDeviceUserDX8, (LPVOID*) &pDevUser)))
			{
				pDevUser->AcquireInputDevices(static_cast<IInputDeviceProviderDX8*>(this));
				pDevUser->Release();
			}
		}
	}

// Implementation
protected:
	inline IModel* GetCommandTarget()
	{
		return m_pModel;
	}

	virtual void SetModel(IModel* pModel)
	{
		if (pModel != NULL)
		{
			pModel->AddRef();
		}

		if (m_pModel != NULL)
		{
			m_pModel->Release();
		}

		m_pModel = pModel;
	}

	STDMETHOD(AddBehavior)(IBehavior* pBehavior)
	{
		if (pBehavior == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = pBehavior->SetController(this);

		if (SUCCEEDED(hr))
		{
			pBehavior->AddRef();
			m_behaviors.push_back(pBehavior);

			IAnimate* pAnimate;
			if (SUCCEEDED(pBehavior->QueryInterface(IID_IAnimate, (LPVOID*) &pAnimate)))
			{
				m_animationBehaviors.push_back(pAnimate);
			}

			IWndMessageReceiver* pWndMessageReceiver;
			if (SUCCEEDED(pBehavior->QueryInterface(IID_IWndMessageReceiver, (LPVOID*) &pWndMessageReceiver)))
			{
				m_wndMessageBehaviors.push_back(pWndMessageReceiver);
			}
		}

		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////
// IFirstPersonControllerImpl

class IFirstPersonControllerImpl : public IControllerImpl<IFirstPersonController>
{
// Constructors/destructor
public:
	IFirstPersonControllerImpl() :
		m_pCollisions(NULL),
		m_bCollisionsEnabled(TRUE),
		m_collisionStrategy(SSCL_TEST_BOUNDING_BOX)
	{
		m_boundingSphere.radius = 2;
	}

	virtual ~IFirstPersonControllerImpl()
	{
		if (m_pCollisions != NULL)
		{
			m_pCollisions->Clear();
			m_pCollisions->Release();
		}
	}

// Attributes
protected:
	bool m_bCollisionsEnabled;
	INodes* m_pCollisions;
	enum HitTestStrategy m_collisionStrategy;
	D3DXVECTOR3 m_prevCameraUp;
	D3DXVECTOR3 m_prevCameraAim;
	D3DXVECTOR3 m_prevCameraLoc;
	SPHERE3D m_boundingSphere;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IController interface

	STDMETHOD(MoveCamera)(/* [in] */ enum CameraMovement moveType,
	                      /* [in] */ VALUE3D fAmount)
	{
		HRESULT hr = E_FAIL;
		ICamera* pCamera = NULL;

		if (m_pViewport != NULL)
		{
			hr = m_pViewport->get_Camera(&pCamera);

			if (hr == S_OK)
			{
				// Save position and orientation of the camera
				SaveCamera(pCamera);

				hr = pCamera->Move(moveType, fAmount);

				if (m_bCollisionsEnabled)
				{
					UpdateBoundingSphere(pCamera);

					if (DetectCollisions())
					{
						// Restore camera to previous position and orientation
						RestoreCamera(pCamera);
					}
				}

				pCamera->Release();
			}

		}

		return hr;
	}

	STDMETHOD(TranslateCamera)(/* [in] */ VALUE3D dx,
	                           /* [in] */ VALUE3D dy,
	                           /* [in] */ VALUE3D dz)
	{
		HRESULT hr = E_FAIL;
		ICamera* pCamera = NULL;

		if (m_pViewport != NULL)
		{
			hr = m_pViewport->get_Camera(&pCamera);

			if (hr == S_OK)
			{
				// Save position and orientation of the camera
				SaveCamera(pCamera);

				hr = pCamera->Translate(dx, dy, dz);

				if (m_bCollisionsEnabled)
				{
					UpdateBoundingSphere(pCamera);

					if (DetectCollisions())
					{
						// Restore camera to previous position and orientation
						RestoreCamera(pCamera);
					}
				}

				pCamera->Release();
			}
		}

		return hr;
	}

	STDMETHOD(RotateCamera)(/* [in] */ VALUE3D fXRadians,
	                        /* [in] */ VALUE3D fYRadians,
	                        /* [in] */ VALUE3D fZRadians)
	{
		HRESULT hr = E_FAIL;
		ICamera* pCamera = NULL;

		if (m_pViewport != NULL)
		{
			hr = m_pViewport->get_Camera(&pCamera);

			if (hr == S_OK)
			{
				// Save position and orientation of the camera
				SaveCamera(pCamera);

				hr = pCamera->Rotate(fXRadians, fYRadians, fZRadians);

				if (m_bCollisionsEnabled)
				{
					UpdateBoundingSphere(pCamera);

					if (DetectCollisions())
					{
						// Restore camera to previous position and orientation
						RestoreCamera(pCamera);
					}
				}

				pCamera->Release();
			}
		}

		return hr;
	}

	//////////////////////////////////////////////////////////////
	// IFirstPersonController interface

	STDMETHOD(get_CollisionsEnabled)(/* [out, retval] */ VARIANT_BOOL* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = m_bCollisionsEnabled ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(put_CollisionsEnabled)(/* [in] */ VARIANT_BOOL newVal)
	{
		m_bCollisionsEnabled = newVal ? true : false;
		return S_OK;
	}

// Implementation
protected:
	inline void SaveCamera(ICamera* pCamera)
	{
		pCamera->GetLocation(&m_prevCameraLoc.x, &m_prevCameraLoc.y, &m_prevCameraLoc.z);
		pCamera->GetAimVector(&m_prevCameraAim.x, &m_prevCameraAim.y, &m_prevCameraAim.z);
		pCamera->GetUpVector(&m_prevCameraUp.x, &m_prevCameraUp.y, &m_prevCameraUp.z);
	}

	inline void RestoreCamera(ICamera* pCamera)
	{
		pCamera->SetLocation(m_prevCameraLoc.x, m_prevCameraLoc.y, m_prevCameraLoc.z);
		pCamera->SetAimVector(m_prevCameraAim.x, m_prevCameraAim.y, m_prevCameraAim.z);
		pCamera->SetUpVector(m_prevCameraUp.x, m_prevCameraUp.y, m_prevCameraUp.z);
	}

	virtual bool DetectCollisions()
	{
		bool bCollision = false;

		m_pCollisions->Clear();

		HRESULT hr = m_pModel->GetChildrenIntersectingSphere(m_pCollisions, vtSPHERE3D(m_boundingSphere), m_collisionStrategy);

		if (hr == S_OK)
		{
			long nCollisionCount = 0;
			m_pCollisions->get_Count(&nCollisionCount);
			bCollision = (nCollisionCount != 0);
		}

		return bCollision;
	}

	inline void UpdateBoundingSphere(ICamera* pCamera)
	{
		pCamera->GetLocation(&m_boundingSphere.x,
		                     &m_boundingSphere.y,
		                     &m_boundingSphere.z);
	}
};

/////////////////////////////////////////////////////////////////////////
// CBrowseController

class ATL_NO_VTABLE CBrowseController : public CComObjectRootEx<CComSingleThreadModel>,
                                        public CComCoClass<CBrowseController, &CLSID_BrowseController>,
                                        public IConnectionPointContainerImpl<CBrowseController>,
                                        public CProxy__IControllerEvents<CBrowseController>,
                                        public IDispatchImpl< ICommandDispatcherImpl<IFirstPersonControllerImpl>, &__uuidof(IFirstPersonController), &LIBID_ShapeShifter>,
                                        public IAnimate,
                                        public IWndMessageReceiver
{
// Constructors/destructor
public:
	CBrowseController();
	virtual ~CBrowseController();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_BROWSE_CTLR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface map
BEGIN_COM_MAP(CBrowseController)
	COM_INTERFACE_ENTRY(IFirstPersonController)
	COM_INTERFACE_ENTRY(IController)
	COM_INTERFACE_ENTRY(ICommandDispatcher)
	COM_INTERFACE_ENTRY(IAnimate)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
	COM_INTERFACE_ENTRY_IID(IID_IInputDeviceProviderDX8, IInputDeviceProviderDX8)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// Connection point map
BEGIN_CONNECTION_POINT_MAP(CBrowseController)
	CONNECTION_POINT_ENTRY(DIID__IControllerEvents)
END_CONNECTION_POINT_MAP()

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IController interface

	STDMETHOD(Update)(/* [in] */ long nUpdateFlags, /* [in] */ INodes* pNodesAffected);

	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//////////////////////////////////////////////////////////////
	// IAnimate interface

	STDMETHOD(Animate)(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame);
};

/////////////////////////////////////////////////////////////////////////
// CBrowseEditController

class ATL_NO_VTABLE CBrowseEditController : public CComObjectRootEx<CComSingleThreadModel>,
                                            public CComCoClass<CBrowseEditController, &CLSID_BrowseEditController>,
                                            public IConnectionPointContainerImpl<CBrowseEditController>,
                                            public CProxy__IControllerEvents<CBrowseEditController>,
                                            public IDispatchImpl< ICommandDispatcherImpl<IFirstPersonControllerImpl>, &__uuidof(IFirstPersonController), &LIBID_ShapeShifter>,
                                            public CSinkModelEvents<CBrowseEditController>,
                                            public IAnimate,
                                            public IWndMessageReceiver
{
// Embedded types
public:
	typedef IDispatchImpl< ICommandDispatcherImpl<IFirstPersonControllerImpl>, &__uuidof(IFirstPersonController), &LIBID_ShapeShifter> _ControllerBase;
	typedef CSinkModelEvents<CBrowseEditController> _ModelEventSink;

// Constructors/destructor
public:
	CBrowseEditController();
	virtual ~CBrowseEditController();

	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_BROWSE_EDIT_CTLR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface map
BEGIN_COM_MAP(CBrowseEditController)
	COM_INTERFACE_ENTRY(IFirstPersonController)
	COM_INTERFACE_ENTRY(IController)
	COM_INTERFACE_ENTRY(ICommandDispatcher)
	COM_INTERFACE_ENTRY(IAnimate)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
	COM_INTERFACE_ENTRY_IID(IID_IInputDeviceProviderDX8, IInputDeviceProviderDX8)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// Connection point map
BEGIN_CONNECTION_POINT_MAP(CBrowseEditController)
	CONNECTION_POINT_ENTRY(DIID__IControllerEvents)
END_CONNECTION_POINT_MAP()

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IController interface

	STDMETHOD(Update)(/* [in] */ long nUpdateFlags, /* [in] */ INodes* pNodesAffected);

	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//////////////////////////////////////////////////////////////
	// IAnimate interface

	STDMETHOD(Animate)(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame);

// Implementation
protected:
	virtual void SetModel(IModel* pModel);

// Model event handlers
public:
	virtual void __stdcall OnAddNode(/* [in] */ INode* pNode);
	virtual void __stdcall OnRemoveNode(/* [in] */ INode* pNode);
	virtual void __stdcall OnMoveNode(/* [in] */ INode* pNode);
};

/////////////////////////////////////////////////////////////////////////
// CFlightController

class ATL_NO_VTABLE CFlightController : public CComObjectRootEx<CComSingleThreadModel>,
                                        public CComCoClass<CFlightController, &CLSID_FlightController>,
                                        public IConnectionPointContainerImpl<CFlightController>,
                                        public CProxy__IControllerEvents<CFlightController>,
                                        public IDispatchImpl< ICommandDispatcherImpl<IFirstPersonControllerImpl>, &__uuidof(IFirstPersonController), &LIBID_ShapeShifter>,
                                        public IAnimate,
                                        public IWndMessageReceiver
{
// Constructors/destructor
public:
	CFlightController();
	virtual ~CFlightController();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_FLIGHT_CTLR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface map
BEGIN_COM_MAP(CFlightController)
	COM_INTERFACE_ENTRY(IFirstPersonController)
	COM_INTERFACE_ENTRY(IController)
	COM_INTERFACE_ENTRY(ICommandDispatcher)
	COM_INTERFACE_ENTRY(IAnimate)
	COM_INTERFACE_ENTRY(IWndMessageReceiver)
	COM_INTERFACE_ENTRY_IID(IID_IInputDeviceProviderDX8, IInputDeviceProviderDX8)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

// Connection point map
BEGIN_CONNECTION_POINT_MAP(CFlightController)
	CONNECTION_POINT_ENTRY(DIID__IControllerEvents)
END_CONNECTION_POINT_MAP()

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IWndMessageReceiver interface

	STDMETHOD(ProcessWindowMessage)(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//////////////////////////////////////////////////////////////
	// IAnimate interface

	STDMETHOD(Animate)(/* [in] */ long nTime, /* [in] */ VARIANT_BOOL bFirstFrame);
};

};  // namespace ShapeShifter

#endif // #ifndef __CONTROLLER_H__

