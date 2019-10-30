//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: RenderingContext.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __PROPPAGES_H__
#define __PROPPAGES_H__

#pragma once

#include <atlctl.h>

#include "ShapeShifter.h"
#include "..\resource.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage

class ATL_NO_VTABLE CGeneralPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGeneralPropPage, &CLSID_GeneralPropPage>,
	public IPropertyPageImpl<CGeneralPropPage>,
	public CDialogImpl<CGeneralPropPage>
{
public:
	CGeneralPropPage() 
	{
		m_dwTitleID = IDS_TITLEGeneralPropPage;
		m_dwHelpFileID = IDS_HELPFILEGeneralPropPage;
		m_dwDocStringID = IDS_DOCSTRINGGeneralPropPage;
	}

	enum {IDD = IDD_GENERAL_PROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_GENERAL_PROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGeneralPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CGeneralPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CGeneralPropPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_FILENAME, EN_CHANGE, OnChangeFilename)
	COMMAND_HANDLER(IDC_INTERNET_ENABLED, BN_CLICKED, OnChangeInternetEnabled)
	COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnBrowse)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChangeFilename(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeInternetEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Apply)(void);

// Attributes
public:
    enum
	{
		FILENAME_CHANGED = 1,
		INTERNET_ENABLED_CHANGED = 2
	};

	short m_changeFlags;

	CComBSTR m_strFilenameOrig;
	CComBSTR m_strFilenameNew;
	VARIANT_BOOL m_bInternetEnabledOrig;
	VARIANT_BOOL m_bInternetEnabledNew;

protected:
	void InitializeControlsFromObject(DISPID dispid);
    void SetPropertiesFromControls(DISPID dispid);
};

/////////////////////////////////////////////////////////////////////////////
// CViewPropPage

class ATL_NO_VTABLE CViewPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CViewPropPage, &CLSID_ViewPropPage>,
	public IPropertyPageImpl<CViewPropPage>,
	public CDialogImpl<CViewPropPage>
{
public:
	CViewPropPage() 
	{
		m_dwTitleID = IDS_TITLEViewPropPage;
		m_dwHelpFileID = IDS_HELPFILEViewPropPage;
		m_dwDocStringID = IDS_DOCSTRINGViewPropPage;
	}

	enum {IDD = IDD_VIEW_PROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_VIEW_PROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CViewPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CViewPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CViewPropPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_VIEWPORT_TYPE, CBN_SELCHANGE, OnChangeViewportType)
	COMMAND_HANDLER(IDC_FOV, EN_CHANGE, OnChangeFOV)
	COMMAND_HANDLER(IDC_FOV, EN_KILLFOCUS, OnKillFocusFOV)
	COMMAND_HANDLER(IDC_ASPECT, EN_CHANGE, OnChangeAspect)
	COMMAND_HANDLER(IDC_ASPECT, EN_KILLFOCUS, OnKillFocusAspect)
	COMMAND_HANDLER(IDC_AUTO_ASPECT, BN_CLICKED, OnChangeAutoAspect)
	COMMAND_HANDLER(IDC_NEARPLANE, EN_CHANGE, OnChangeNearPlane)
	COMMAND_HANDLER(IDC_NEARPLANE, EN_KILLFOCUS, OnKillFocusNearPlane)
	COMMAND_HANDLER(IDC_FARPLANE, EN_CHANGE, OnChangeFarPlane)
	COMMAND_HANDLER(IDC_FARPLANE, EN_KILLFOCUS, OnKillFocusFarPlane)
	COMMAND_HANDLER(IDC_CAMERALOC_X, EN_CHANGE, OnChangeCameraLocX)
	COMMAND_HANDLER(IDC_CAMERALOC_X, EN_KILLFOCUS, OnKillFocusCameraLocX)
	COMMAND_HANDLER(IDC_CAMERALOC_Y, EN_CHANGE, OnChangeCameraLocY)
	COMMAND_HANDLER(IDC_CAMERALOC_Y, EN_KILLFOCUS, OnKillFocusCameraLocY)
	COMMAND_HANDLER(IDC_CAMERALOC_Z, EN_CHANGE, OnChangeCameraLocZ)
	COMMAND_HANDLER(IDC_CAMERALOC_Z, EN_KILLFOCUS, OnKillFocusCameraLocZ)
	COMMAND_HANDLER(IDC_CAMERAAIM_X, EN_CHANGE, OnChangeCameraAimX)
	COMMAND_HANDLER(IDC_CAMERAAIM_X, EN_KILLFOCUS, OnKillFocusCameraAimX)
	COMMAND_HANDLER(IDC_CAMERAAIM_Y, EN_CHANGE, OnChangeCameraAimY)
	COMMAND_HANDLER(IDC_CAMERAAIM_Y, EN_KILLFOCUS, OnKillFocusCameraAimY)
	COMMAND_HANDLER(IDC_CAMERAAIM_Z, EN_CHANGE, OnChangeCameraAimZ)
	COMMAND_HANDLER(IDC_CAMERAAIM_Z, EN_KILLFOCUS, OnKillFocusCameraAimZ)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChangeViewportType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeFOV(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusFOV(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeAspect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusAspect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeAutoAspect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeNearPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusNearPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeFarPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusFarPlane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraLocX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraLocX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraLocY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraLocY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraLocZ(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraLocZ(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraAimX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraAimX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraAimY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraAimY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraAimZ(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraAimZ(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraUpX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraUpX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraUpY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraUpY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCameraUpZ(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusCameraUpZ(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Apply)(void);

// Attributes
public:
    enum
	{
		VIEWPORT_TYPE_CHANGED = 1,
		FOV_CHANGED = 2,
		ASPECT_CHANGED = 4,
		AUTO_ASPECT_CHANGED = 8,
		NEARPLANE_CHANGED = 16,
		FARPLANE_CHANGED = 32,
		CAMERALOCX_CHANGED = 64,
		CAMERALOCY_CHANGED = 128,
		CAMERALOCZ_CHANGED = 256,
		CAMERAAIMX_CHANGED = 512,
		CAMERAAIMY_CHANGED = 1024,
		CAMERAAIMZ_CHANGED = 2048,
		CAMERAUPX_CHANGED = 4096,
		CAMERAUPY_CHANGED = 8192,
		CAMERAUPZ_CHANGED = 16384
	};

	short m_changeFlags;

	VARIANT_BOOL m_bWindowedOrig;
	VARIANT_BOOL m_bWindowedNew;
	FLOAT m_FOVOrig;
	FLOAT m_FOVNew;
	FLOAT m_AspectOrig;
	FLOAT m_AspectNew;
	VARIANT_BOOL m_bAutoAspectOrig;
	VARIANT_BOOL m_bAutoAspectNew;
	FLOAT m_NearPlaneOrig;
	FLOAT m_NearPlaneNew;
	FLOAT m_FarPlaneOrig;
	FLOAT m_FarPlaneNew;
	FLOAT m_CameraLocXOrig;
	FLOAT m_CameraLocXNew;
	FLOAT m_CameraLocYOrig;
	FLOAT m_CameraLocYNew;
	FLOAT m_CameraLocZOrig;
	FLOAT m_CameraLocZNew;
	FLOAT m_CameraAimXOrig;
	FLOAT m_CameraAimXNew;
	FLOAT m_CameraAimYOrig;
	FLOAT m_CameraAimYNew;
	FLOAT m_CameraAimZOrig;
	FLOAT m_CameraAimZNew;
	FLOAT m_CameraUpXOrig;
	FLOAT m_CameraUpXNew;
	FLOAT m_CameraUpYOrig;
	FLOAT m_CameraUpYNew;
	FLOAT m_CameraUpZOrig;
	FLOAT m_CameraUpZNew;

protected:
	void InitializeControlsFromObject(DISPID dispid);
    void SetPropertiesFromControls(DISPID dispid);
};

/////////////////////////////////////////////////////////////////////////////
// CTypesPropPage

class ATL_NO_VTABLE CTypesPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTypesPropPage, &CLSID_TypesPropPage>,
	public IPropertyPageImpl<CTypesPropPage>,
	public CDialogImpl<CTypesPropPage>
{
public:
	CTypesPropPage() 
	{
		m_dwTitleID = IDS_TITLETypesPropPage;
		m_dwHelpFileID = IDS_HELPFILETypesPropPage;
		m_dwDocStringID = IDS_DOCSTRINGTypesPropPage;
	}

	enum {IDD = IDD_TYPES_PROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_TYPES_PROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTypesPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTypesPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTypesPropPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_MODELTYPE, EN_CHANGE, OnChangeModelType)
	COMMAND_HANDLER(IDC_VIEWPORTTYPE, EN_CHANGE, OnChangeViewportType)
	COMMAND_HANDLER(IDC_CONTROLLERTYPE, EN_CHANGE, OnChangeControllerType)
	COMMAND_HANDLER(IDC_MODELREADERTYPE, EN_CHANGE, OnChangeModelReaderType)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChangeModelType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeViewportType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeControllerType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeModelReaderType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Apply)(void);

// Attributes
public:
    enum
	{
		MODELTYPE_CHANGED = 1,
		VIEWPORTTYPE_CHANGED = 2,
		CONTROLLERTYPE_CHANGED = 4,
		MODELREADERTYPE_CHANGED = 8
	};

	short m_changeFlags;

	CComBSTR m_strModelTypeOrig;
	CComBSTR m_strModelTypeNew;
	CComBSTR m_strViewportTypeOrig;
	CComBSTR m_strViewportTypeNew;
	CComBSTR m_strControllerTypeOrig;
	CComBSTR m_strControllerTypeNew;
	CComBSTR m_strModelReaderTypeOrig;
	CComBSTR m_strModelReaderTypeNew;

protected:
	void InitializeControlsFromObject(DISPID dispid);
    void SetPropertiesFromControls(DISPID dispid);
};

/////////////////////////////////////////////////////////////////////////////
// CBehaviorPropPage

class ATL_NO_VTABLE CBehaviorPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBehaviorPropPage, &CLSID_BehaviorPropPage>,
	public IPropertyPageImpl<CBehaviorPropPage>,
	public CDialogImpl<CBehaviorPropPage>
{
public:
	CBehaviorPropPage() 
	{
		m_dwTitleID = IDS_TITLEBehaviorPropPage;
		m_dwHelpFileID = IDS_HELPFILEBehaviorPropPage;
		m_dwDocStringID = IDS_DOCSTRINGBehaviorPropPage;
	}

	enum {IDD = IDD_BEHAVIOR_PROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_BEHAVIOR_PROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBehaviorPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CBehaviorPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CBehaviorPropPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_ANIMATION_ENABLED, BN_CLICKED, OnChangeAnimationEnabled)
	COMMAND_HANDLER(IDC_ANIMATION_INTERVAL, EN_CHANGE, OnChangeAnimationInterval)
	COMMAND_HANDLER(IDC_ANIMATION_INTERVAL, EN_KILLFOCUS, OnKillFocusAnimationInterval)
	COMMAND_HANDLER(IDC_COLLISIONS_ENABLED, BN_CLICKED, OnChangeCollisionsEnabled)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChangeAnimationEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeAnimationInterval(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnKillFocusAnimationInterval(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeCollisionsEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	STDMETHOD(Apply)(void);

// Attributes
public:
    enum
	{
		ANIMATION_ENABLED_CHANGED = 1,
		ANIMATION_INTERVAL_CHANGED = 2,
		COLLISIONS_ENABLED_CHANGED = 4
	};

	short m_changeFlags;

	VARIANT_BOOL m_bAnimationEnabledOrig;
	VARIANT_BOOL m_bAnimationEnabledNew;
	long m_nAnimationIntervalOrig;
	long m_nAnimationIntervalNew;
	VARIANT_BOOL m_bCollisionsEnabledOrig;
	VARIANT_BOOL m_bCollisionsEnabledNew;

protected:
	void InitializeControlsFromObject(DISPID dispid);
    void SetPropertiesFromControls(DISPID dispid);
};

};  // namespace ShapeShifter

#endif //__PROPPAGES_H__
