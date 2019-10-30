//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ShapeShifterCtl.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __SHAPESHIFTERCTL_H__
#define __SHAPESHIFTERCTL_H__

#pragma once

#include <atlctl.h>

#include "ShapeShifter.h"
#include "ShapeShifterCP.h"
#include "EventSink.h"
#include "LicenseMgr.h"

namespace ShapeShifter {

const int PROPCAT_Types      = 1;
const int PROPCAT_Device     = 2;
const int PROPCAT_Viewport   = 3;

/////////////////////////////////////////////////////////////////////////////
// CShapeShifterCtl

class ATL_NO_VTABLE CShapeShifterCtl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CShapeShifterCtl, IShapeShifterCtl, &IID_IShapeShifterCtl, &LIBID_ShapeShifter>,
	public CComControl<CShapeShifterCtl>,
	public IPersistStreamInitImpl<CShapeShifterCtl>,
	public IPersistPropertyBagImpl<CShapeShifterCtl>,
	public IOleControlImpl<CShapeShifterCtl>,
	public IOleObjectImpl<CShapeShifterCtl>,
	public IOleInPlaceActiveObjectImpl<CShapeShifterCtl>,
	public IViewObjectExImpl<CShapeShifterCtl>,
	public IOleInPlaceObjectWindowlessImpl<CShapeShifterCtl>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CShapeShifterCtl>,
	public IPersistStorageImpl<CShapeShifterCtl>,
	public ISpecifyPropertyPagesImpl<CShapeShifterCtl>,
	public IQuickActivateImpl<CShapeShifterCtl>,
	public IDataObjectImpl<CShapeShifterCtl>,
	public IProvideClassInfo2Impl<&CLSID_ShapeShifterCtl, &DIID__IShapeShifterCtlEvents, &LIBID_ShapeShifter>,
	public IPropertyNotifySinkCP<CShapeShifterCtl>,
	public CComCoClass<CShapeShifterCtl, &CLSID_ShapeShifterCtl>,
	public CProxy_IShapeShifterCtlEvents< CShapeShifterCtl >,
	public CSinkControllerEvents<CShapeShifterCtl>,
	public CSinkModelEvents<CShapeShifterCtl>,
	public IObjectSafetyImpl<CShapeShifterCtl, INTERFACESAFE_FOR_UNTRUSTED_CALLER |
	                                           INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public ICategorizeProperties
{
// Embedded types
public:
	typedef CSinkControllerEvents<CShapeShifterCtl> _ControllerEventSink;
	typedef CSinkModelEvents<CShapeShifterCtl> _ModelEventSink;

// Constructors/destructor
public:
	CShapeShifterCtl();
	~CShapeShifterCtl();

	HRESULT FinalConstruct();

DECLARE_CLASSFACTORY2(CLicenseManager)

DECLARE_REGISTRY_RESOURCEID(IDR_SHAPESHIFTER_CTL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CShapeShifterCtl)
	COM_INTERFACE_ENTRY(IShapeShifterCtl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY(IPersistPropertyBag)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(ICategorizeProperties)
END_COM_MAP()

BEGIN_PROP_MAP(CShapeShifterCtl)
	PROP_PAGE(CLSID_GeneralPropPage)
	PROP_PAGE(CLSID_ViewPropPage)
	PROP_PAGE(CLSID_TypesPropPage)
	PROP_PAGE(CLSID_BehaviorPropPage)
	PROP_PAGE(CLSID_StockColorPage)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	PROP_ENTRY("ModelFile", SSCLID_MODEL_FILE, CLSID_NULL)
	PROP_ENTRY("ModelType", SSCLID_MODEL_TYPE, CLSID_NULL)
	PROP_ENTRY("ViewportType", SSCLID_VIEWPORT_TYPE, CLSID_NULL)
	PROP_ENTRY("ControllerType", SSCLID_CONTROLLER_TYPE, CLSID_NULL)
	PROP_ENTRY("ModelReaderType", SSCLID_MODEL_READER_TYPE, CLSID_NULL)
	PROP_ENTRY("Windowed", SSCLID_WINDOWED, CLSID_NULL)
	PROP_ENTRY("FOV", SSCLID_FOV, CLSID_NULL)
	PROP_ENTRY("AutoAspect", SSCLID_AUTO_ASPECT, CLSID_NULL)
	PROP_ENTRY("Aspect", SSCLID_ASPECT, CLSID_NULL)
	PROP_ENTRY("NearPlane", SSCLID_NEARPLANE, CLSID_NULL)
	PROP_ENTRY("FarPlane", SSCLID_FARPLANE, CLSID_NULL)
	PROP_ENTRY("CameraLocationX", SSCLID_CAMERA_LOCATION_X, CLSID_NULL)
	PROP_ENTRY("CameraLocationY", SSCLID_CAMERA_LOCATION_Y, CLSID_NULL)
	PROP_ENTRY("CameraLocationZ", SSCLID_CAMERA_LOCATION_Z, CLSID_NULL)
	PROP_ENTRY("CameraAimX", SSCLID_CAMERA_AIM_X, CLSID_NULL)
	PROP_ENTRY("CameraAimY", SSCLID_CAMERA_AIM_Y, CLSID_NULL)
	PROP_ENTRY("CameraAimZ", SSCLID_CAMERA_AIM_Z, CLSID_NULL)
	PROP_ENTRY("CameraUpX", SSCLID_CAMERA_UP_X, CLSID_NULL)
	PROP_ENTRY("CameraUpY", SSCLID_CAMERA_UP_Y, CLSID_NULL)
	PROP_ENTRY("CameraUpZ", SSCLID_CAMERA_UP_Z, CLSID_NULL)	
	PROP_ENTRY("AnimationInterval", SSCLID_ANIMATION_INTERVAL, CLSID_NULL)
	PROP_ENTRY("AnimationEnabled", SSCLID_ANIMATION_ENABLED, CLSID_NULL)
	PROP_ENTRY("CollisionsEnabled", SSCLID_COLLISIONS_ENABLED, CLSID_NULL)
	PROP_ENTRY("InternetEnabled", SSCLID_INTERNET_ENABLED, CLSID_NULL)
	PROP_ENTRY("BackColor", DISPID_BACKCOLOR, CLSID_NULL)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CShapeShifterCtl)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__IShapeShifterCtlEvents)
END_CONNECTION_POINT_MAP()

// Message handlers
public:

BEGIN_MSG_MAP(CShapeShifterCtl)
	CHAIN_MSG_MAP(CComControl<CShapeShifterCtl>)
	DEFAULT_REFLECTION_HANDLER()
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)

	// Forward messages to the controller, if it implements IWndMessageReceiver
	if (m_pWndMsgRcvr != NULL)
	{
		m_pWndMsgRcvr->ProcessWindowMessage(uMsg, wParam, lParam);
	}

END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	HRESULT OnDraw(ATL_DRAWINFO& di);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_IShapeShifterCtl,
		};
		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (::InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IOleObject
	STDMETHOD(DoVerb)(LONG iVerb, LPMSG pMsg, IOleClientSite* pActiveSite,
	                  LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid);
	STDMETHOD(Update)(void);
	STDMETHOD(IsUpToDate)(void);
	STDMETHOD(GetMiscStatus)(DWORD dwAspect, DWORD* pdwStatus);

// IOleControl
	STDMETHOD(GetControlInfo)(LPCONTROLINFO pCI);

// IOleInPlaceActiveObject
	STDMETHOD(TranslateAccelerator)(LPMSG pMsg);

// ICategorizeProperties
	STDMETHOD(MapPropertyToCategory)(/* [in] */ DISPID dispid, /* [out] */ PROPCAT* ppropcat);
	STDMETHOD(GetCategoryName)(/* [in] */ PROPCAT propcat, /* [in] */ LCID lcid, /* [out] */ BSTR* pbstrName);

// Attributes
public:
	// Pointer to the ShapeShifter engine
	IEngine* m_pEngine;

	// Pointer to the rendering device used
	IDevice* m_pDevice;

	// Flag to indicate if device has been initialized
	bool m_bDeviceInitialized;

	// Pointer to the model
	IModel* m_pModel;

	// Pointer to the viewport
	IViewport* m_pViewport;

	// Pointer to the controller
	IController* m_pController;

	// Pointer to window message receiver of the controller. Used to forward
	// window messages from the control window to the controller
	IWndMessageReceiver* m_pWndMsgRcvr;

	// Pointer to the animation loop. Animation loop is only started if the
	// m_bAnimationEnabled flag is set to true.
	IAnimationLoop* m_pAnimationLoop;

	// Indicates if the animation loop will be started during run-time
	bool m_bAnimationEnabled;

	// GUID of model class to create
	CLSID m_clsidModel;

	// GUID of viewport class to create
	CLSID m_clsidViewport;

	// GUID of controller class to create
	CLSID m_clsidController;

	// GUID of model reader class to create
	CLSID m_clsidModelReader;

	// Name of file that contains model (may be empty)
	_bstr_t m_strModelFile;

	// Stock property members
	OLE_COLOR m_clrBackColor;

	// Indicates if aspect ratio will be automatically calculated
	bool m_bAutoAspect;

// IShapeShifter
public:
	STDMETHOD(get_DriverMode)(/* [out,retval] */ enum DriverMode *pMode);
	STDMETHOD(put_DriverMode)(/* [in] */ enum DriverMode mode);
	STDMETHOD(get_ModelType)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ModelType)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ViewportType)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ViewportType)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ControllerType)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ControllerType)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Engine)(/*[out, retval]*/ IEngine** pVal);
	STDMETHOD(get_Device)(/* [out, retval] */ IDevice** pVal);
	STDMETHOD(get_Controller)(/*[out, retval]*/ IController** pVal);
	STDMETHOD(get_Viewport)(/*[out, retval]*/ IViewport** pVal);
	STDMETHOD(get_Model)(/*[out, retval]*/ IModel** pVal);
	STDMETHOD(get_ModelFile)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(put_ModelFile)(/* [in] */ BSTR newVal);
	STDMETHOD(get_ModelReaderType)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ModelReaderType)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_BackColor)(/* [in] */ OLE_COLOR clr);
	STDMETHOD(get_Windowed)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_Windowed)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(get_FOV)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_FOV)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_Aspect)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_Aspect)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_AutoAspect)(/* [out, retval] */ VARIANT_BOOL* pVal);
	STDMETHOD(put_AutoAspect)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(get_NearPlane)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_NearPlane)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_FarPlane)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_FarPlane)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraLocationX)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraLocationX)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraLocationY)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraLocationY)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraLocationZ)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraLocationZ)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraAimX)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraAimX)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraAimY)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraAimY)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraAimZ)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraAimZ)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraUpX)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraUpX)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraUpY)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraUpY)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_CameraUpZ)(/* [out, retval] */ FLOAT* pVal);
	STDMETHOD(put_CameraUpZ)(/* [in] */ FLOAT newVal);
	STDMETHOD(get_AnimationInterval)(/* [out, retval] */ long *pVal);
	STDMETHOD(put_AnimationInterval)(/* [in] */ long newVal);
	STDMETHOD(get_AnimationEnabled)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_AnimationEnabled)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(get_CollisionsEnabled)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(put_CollisionsEnabled)(/* [in] */ VARIANT_BOOL newVal);
	STDMETHOD(get_InternetEnabled)(/* [out, retval] */ VARIANT_BOOL* pVal);
	STDMETHOD(put_InternetEnabled)(/* [in] */ VARIANT_BOOL newVal);

	STDMETHOD(LoadFile)();
	STDMETHOD(ReloadDevice)();
    STDMETHOD(Refresh)();
	STDMETHOD(AboutBox)();

// Implementation
protected:
	HRESULT CreateDevice();
	HRESULT CreateModel(const CLSID& clsid);
	HRESULT CreateViewport(const CLSID& clsid);
	HRESULT CreateController(const CLSID& clsid);
	HRESULT CreateAnimationLoop();
	HRESULT DefaultModelReaderType();

	bool IsRuntime() const;
	HRESULT RuntimeInit();

// Event handlers
public:
	// Controller events
	virtual void __stdcall OnSelectionChange(IDispatch* pSelectionList);
};

};  // namespace ShapeShifter

#endif // #ifndef __SHAPESHIFTERCTL_H__
