//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: PropPages.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <math.h>
#include <atlwin.h>
#include "atlcontrols.h"

#include "PropPages.h"

#define MAX_FILENAME_LEN   512

using namespace ATLControls;

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// Data exchange buffers

const int g_cnvBufSize = 40;
static TCHAR g_cnvBuf[g_cnvBufSize+1];

const int g_txtBufSize = 512;
static TCHAR g_txtBuf[g_txtBufSize+1];

static void FilterConversionBuffer()
{
	UINT i, j;

	for (i = 0, j = 0; j < _tcslen(g_cnvBuf); j++)
	{
		if ((g_cnvBuf[j] >= '0' && g_cnvBuf[j] < '9') ||
		    (g_cnvBuf[j] == '.'))
		{
			g_cnvBuf[i] = g_cnvBuf[j];
			i++;
		}
	}

	g_cnvBuf[i] = '\0';
}

/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage

LRESULT CGeneralPropPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	InitializeControlsFromObject(DISPID_UNKNOWN);
	return 0;
}

LRESULT CGeneralPropPage::OnChangeFilename(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_FILENAME, g_txtBuf, g_txtBufSize) > 0)
	{
		m_strFilenameNew = g_txtBuf;
	}

    if (m_strFilenameOrig != m_strFilenameNew)
	{
        m_changeFlags |= FILENAME_CHANGED;
	}
    else
	{
        m_changeFlags &= ~FILENAME_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CGeneralPropPage::OnChangeInternetEnabled(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	CButton btnInternetEnabled = GetDlgItem(IDC_INTERNET_ENABLED);

	if (btnInternetEnabled.GetCheck() == 0)
	{
		m_bInternetEnabledNew = VARIANT_FALSE;
	}
	else
	{
		m_bInternetEnabledNew = VARIANT_TRUE;
	}

	if (m_bInternetEnabledOrig != m_bInternetEnabledNew)
	{
		m_changeFlags |= INTERNET_ENABLED_CHANGED;
	}
	else
	{
		m_changeFlags &= ~INTERNET_ENABLED_CHANGED;
	}

	SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CGeneralPropPage::OnBrowse(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	USES_CONVERSION;

	OPENFILENAME ofn;       // common dialog box structure

	if (_tcslen(OLE2T(m_strFilenameNew)) >= g_txtBufSize)
		return 0;

	_tcscpy(g_txtBuf, OLE2T(m_strFilenameNew));

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = g_txtBuf;
	ofn.nMaxFile = g_txtBufSize;
	ofn.lpstrFilter = _T("3D Studio\0*.3DS\0XML files\0*.xml\0All\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn))
	{
		SetDlgItemText(IDC_FILENAME, ofn.lpstrFile);
	}

	return 0;
}

STDMETHODIMP CGeneralPropPage::Apply(void)
{
    ATLTRACE(_T("CGeneralPropPage::Apply\n"));
    SetPropertiesFromControls(DISPID_UNKNOWN);
    m_bDirty = FALSE;
    m_changeFlags  = 0;
    return S_OK;
}

void CGeneralPropPage::InitializeControlsFromObject(DISPID dispid)
{
	USES_CONVERSION;

	CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[0];
	BSTR strBuf = NULL;

	if (SSCLID_MODEL_FILE == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_ModelFile(&strBuf);

		if (SUCCEEDED(hr))
		{
			m_strFilenameOrig = strBuf;
			m_strFilenameNew = m_strFilenameOrig;

			if (IsWindow())
			{
				SetDlgItemText(IDC_FILENAME, OLE2CA(m_strFilenameOrig));
			}
			::SysFreeString(strBuf);
		}
	}

	if (SSCLID_INTERNET_ENABLED == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_InternetEnabled(&m_bInternetEnabledOrig);

		if (SUCCEEDED(hr))
		{
			m_bInternetEnabledNew = m_bInternetEnabledOrig;

			if (IsWindow())
			{
				CButton btnInternetEnabled = GetDlgItem(IDC_INTERNET_ENABLED);

				if (!m_bInternetEnabledOrig)
				{
					btnInternetEnabled.SetCheck(0);
				}
				else
				{
					btnInternetEnabled.SetCheck(1);
				}
			}
		}
	}
}

void CGeneralPropPage::SetPropertiesFromControls(DISPID dispid)
{
	// In case QI failed in SetObjects
	if (!m_ppUnk || !m_ppUnk[0]) return;

	// For all objects in array...
	for (UINT i = 0; i < m_nObjects; i++)
	{
		// Get the appropriate interface...
		CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[i];

		// Update the Filename property, if requested and required
		if ((SSCLID_MODEL_FILE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & FILENAME_CHANGED)
		{
			pCtl->put_ModelFile(m_strFilenameNew);       // Update property
			m_strFilenameOrig = m_strFilenameNew;        // Prop page and property synced
			m_changeFlags &= ~FILENAME_CHANGED;          // Clear changed flag
		}

		// Update the InternetEnabled property, if requested and required
		if ((SSCLID_INTERNET_ENABLED == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & INTERNET_ENABLED_CHANGED)
		{
			pCtl->put_InternetEnabled(m_bInternetEnabledNew);       // Update property
			m_bInternetEnabledOrig = m_bInternetEnabledNew;        // Prop page and property synced
			m_changeFlags &= ~INTERNET_ENABLED_CHANGED;          // Clear changed flag
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CViewPropPage

const LPCTSTR FMT_FOV              = _T("%1.2f");
const LPCTSTR FMT_ASPECT           = _T("%2.2f");
const LPCTSTR FMT_NEARPLANE        = _T("%8.2f");
const LPCTSTR FMT_FARPLANE         = _T("%8.2f");
const LPCTSTR FMT_CAMERA_LOCATION  = _T("%8.2f");
const LPCTSTR FMT_CAMERA_AIM       = _T("%8.2f");
const LPCTSTR FMT_CAMERA_UP        = _T("%8.2f");

LRESULT CViewPropPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CComboBox viewportType = GetDlgItem(IDC_VIEWPORT_TYPE);
	viewportType.AddString(_T("Windowed"));
	viewportType.AddString(_T("Full Screen"));
	viewportType.SetCurSel(0);
	InitializeControlsFromObject(DISPID_UNKNOWN);
	return 0;
}

STDMETHODIMP CViewPropPage::Apply(void)
{
    ATLTRACE(_T("CGeneralPropPage::Apply\n"));
    SetPropertiesFromControls(DISPID_UNKNOWN);
    m_bDirty = FALSE;
    m_changeFlags  = 0;
    return S_OK;
}

LRESULT CViewPropPage::OnChangeViewportType(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	CComboBox viewportType = GetDlgItem(IDC_VIEWPORT_TYPE);

	if (viewportType.GetCurSel() == 0)
	{
		m_bWindowedNew = VARIANT_TRUE;
	}
	else
	{
		m_bWindowedNew = VARIANT_FALSE;
	}

	if (m_bWindowedOrig != m_bWindowedNew)
	{
		m_changeFlags |= VIEWPORT_TYPE_CHANGED;
	}
	else
	{
		m_changeFlags &= ~VIEWPORT_TYPE_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnChangeFOV(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_FOV, g_cnvBuf, g_cnvBufSize) > 0)
	{
		FilterConversionBuffer();
		m_FOVNew = (FLOAT) atof(g_cnvBuf);
	}

	if (m_FOVOrig != m_FOVNew)
	{
		m_changeFlags |= FOV_CHANGED;
	}
	else
	{
		m_changeFlags &= ~FOV_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusFOV(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_FOV, m_FOVNew);
	SetDlgItemText(IDC_FOV, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeAspect(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_ASPECT, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_AspectNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_AspectOrig != m_AspectNew)
	{
        m_changeFlags |= ASPECT_CHANGED;
	}
    else
	{
        m_changeFlags &= ~ASPECT_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusAspect(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_ASPECT, m_AspectNew);
	SetDlgItemText(IDC_ASPECT, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeAutoAspect(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	CButton btnAutoAspect = GetDlgItem(IDC_AUTO_ASPECT);

	if (btnAutoAspect.GetCheck() == 0)
	{
		m_bAutoAspectNew = VARIANT_FALSE;
	}
	else
	{
		m_bAutoAspectNew = VARIANT_TRUE;
	}

	if (m_bAutoAspectOrig != m_bAutoAspectNew)
	{
        m_changeFlags |= AUTO_ASPECT_CHANGED;
	}
    else
	{
        m_changeFlags &= ~AUTO_ASPECT_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	if (m_bAutoAspectNew)
	{
		::EnableWindow(GetDlgItem(IDC_ASPECT), FALSE);
	}
	else
	{
		::EnableWindow(GetDlgItem(IDC_ASPECT), TRUE);
	}

	return 0;
}

LRESULT CViewPropPage::OnChangeNearPlane(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_NEARPLANE, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_NearPlaneNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_NearPlaneOrig != m_NearPlaneNew)
	{
        m_changeFlags |= NEARPLANE_CHANGED;
	}
    else
	{
        m_changeFlags &= ~NEARPLANE_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusNearPlane(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_NEARPLANE, m_NearPlaneNew);
	SetDlgItemText(IDC_NEARPLANE, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeFarPlane(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_FARPLANE, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_FarPlaneNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_FarPlaneOrig != m_FarPlaneNew)
	{
        m_changeFlags |= FARPLANE_CHANGED;
	}
    else
	{
        m_changeFlags &= ~FARPLANE_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusFarPlane(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_FARPLANE, m_FarPlaneNew);
	SetDlgItemText(IDC_FARPLANE, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraLocX(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERALOC_X, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraLocXNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraLocXOrig != m_CameraLocXNew)
	{
        m_changeFlags |= CAMERALOCX_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERALOCX_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraLocX(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_LOCATION, m_CameraLocXNew);
	SetDlgItemText(IDC_CAMERALOC_X, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraLocY(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERALOC_Y, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraLocYNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraLocYOrig != m_CameraLocYNew)
	{
        m_changeFlags |= CAMERALOCY_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERALOCY_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraLocY(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_LOCATION, m_CameraLocYNew);
	SetDlgItemText(IDC_CAMERALOC_Y, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraLocZ(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERALOC_Z, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraLocZNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraLocZOrig != m_CameraLocZNew)
	{
        m_changeFlags |= CAMERALOCZ_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERALOCZ_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraLocZ(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_LOCATION, m_CameraLocZNew);
	SetDlgItemText(IDC_CAMERALOC_Z, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraAimX(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERAAIM_X, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraAimXNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraAimXOrig != m_CameraAimXNew)
	{
        m_changeFlags |= CAMERAAIMX_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERAAIMX_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraAimX(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_AIM, m_CameraAimXNew);
	SetDlgItemText(IDC_CAMERAAIM_X, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraAimY(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERAAIM_Y, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraAimYNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraAimYOrig != m_CameraAimYNew)
	{
        m_changeFlags |= CAMERAAIMY_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERAAIMY_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraAimY(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_AIM, m_CameraAimYNew);
	SetDlgItemText(IDC_CAMERAAIM_Y, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraAimZ(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERAAIM_Z, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraAimZNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraAimZOrig != m_CameraAimZNew)
	{
        m_changeFlags |= CAMERAAIMZ_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERAAIMZ_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraAimZ(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_AIM, m_CameraAimZNew);
	SetDlgItemText(IDC_CAMERAAIM_Z, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraUpX(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERAUP_X, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraUpXNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraUpXOrig != m_CameraUpXNew)
	{
        m_changeFlags |= CAMERAUPX_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERAUPX_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraUpX(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_UP, m_CameraUpXNew);
	SetDlgItemText(IDC_CAMERAUP_X, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraUpY(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
   if (GetDlgItemText(IDC_CAMERAUP_Y, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraUpYNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraUpYOrig != m_CameraUpYNew)
	{
        m_changeFlags |= CAMERAUPY_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERAUPY_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraUpY(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_UP, m_CameraUpYNew);
	SetDlgItemText(IDC_CAMERAUP_Y, g_cnvBuf);
	return 0;
}

LRESULT CViewPropPage::OnChangeCameraUpZ(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CAMERAUP_Z, g_cnvBuf, g_cnvBufSize) > 0)
	{
		m_CameraUpZNew = (FLOAT) atof(g_cnvBuf);
	}

    if (m_CameraUpZOrig != m_CameraUpZNew)
	{
        m_changeFlags |= CAMERAUPZ_CHANGED;
	}
    else
	{
        m_changeFlags &= ~CAMERAUPZ_CHANGED;
	}

    SetDirty (m_changeFlags != 0);

	return 0;
}

LRESULT CViewPropPage::OnKillFocusCameraUpZ(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_CAMERA_UP, m_CameraUpZNew);
	SetDlgItemText(IDC_CAMERAUP_Z, g_cnvBuf);
	return 0;
}

void CViewPropPage::InitializeControlsFromObject(DISPID dispid)
{
	USES_CONVERSION;

	CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[0];

	if (SSCLID_WINDOWED == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_Windowed(&m_bWindowedOrig);

		if (SUCCEEDED(hr))
		{
			m_bWindowedNew = m_bWindowedOrig;

			if (IsWindow())
			{
				CComboBox viewportType = GetDlgItem(IDC_VIEWPORT_TYPE);

				if (m_bWindowedOrig)
				{
					viewportType.SetCurSel(0);
				}
				else
				{
					viewportType.SetCurSel(1);
				}
			}
		}
	}

	if (SSCLID_FOV == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_FOV(&m_FOVOrig);

		if (SUCCEEDED(hr))
		{
			m_FOVNew = m_FOVOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_FOV, m_FOVOrig);
				SetDlgItemText(IDC_FOV, g_cnvBuf);
			}
		}
	}

	if (SSCLID_ASPECT == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_Aspect(&m_AspectOrig);

		if (SUCCEEDED(hr))
		{
			m_AspectNew = m_AspectOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_ASPECT, m_AspectOrig);
				SetDlgItemText(IDC_ASPECT, g_cnvBuf);
			}
		}
	}

	if (SSCLID_AUTO_ASPECT == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_AutoAspect(&m_bAutoAspectOrig);

		if (SUCCEEDED(hr))
		{
			m_bAutoAspectNew = m_bAutoAspectOrig;

			if (IsWindow())
			{
				CButton btnAutoAspect = GetDlgItem(IDC_AUTO_ASPECT);

				if (m_bAutoAspectOrig)
				{
					btnAutoAspect.SetCheck(1);
					::EnableWindow(GetDlgItem(IDC_ASPECT), FALSE);
				}
				else
				{
					btnAutoAspect.SetCheck(0);
					::EnableWindow(GetDlgItem(IDC_ASPECT), TRUE);
				}
			}
		}
	}

	if (SSCLID_NEARPLANE == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_NearPlane(&m_NearPlaneOrig);

		if (SUCCEEDED(hr))
		{
			m_NearPlaneNew = m_NearPlaneOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_NEARPLANE, m_NearPlaneOrig);
				SetDlgItemText(IDC_NEARPLANE, g_cnvBuf);
			}
		}
	}

	if (SSCLID_FARPLANE == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_FarPlane(&m_FarPlaneOrig);

		if (SUCCEEDED(hr))
		{
			m_FarPlaneNew = m_FarPlaneOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_FARPLANE, m_FarPlaneOrig);
				SetDlgItemText(IDC_FARPLANE, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_LOCATION_X == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraLocationX(&m_CameraLocXOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraLocXNew = m_CameraLocXOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_LOCATION, m_CameraLocXOrig);
				SetDlgItemText(IDC_CAMERALOC_X, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_LOCATION_Y == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraLocationY(&m_CameraLocYOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraLocYNew = m_CameraLocYOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_LOCATION, m_CameraLocYOrig);
				SetDlgItemText(IDC_CAMERALOC_Y, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_LOCATION_Z == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraLocationZ(&m_CameraLocZOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraLocZNew = m_CameraLocZOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_LOCATION, m_CameraLocZOrig);
				SetDlgItemText(IDC_CAMERALOC_Z, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_AIM_X == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraAimX(&m_CameraAimXOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraAimXNew = m_CameraAimXOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_AIM, m_CameraAimXOrig);
				SetDlgItemText(IDC_CAMERAAIM_X, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_AIM_Y == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraAimY(&m_CameraAimYOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraAimYNew = m_CameraAimYOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_AIM, m_CameraAimYOrig);
				SetDlgItemText(IDC_CAMERAAIM_Y, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_AIM_Z == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraAimZ(&m_CameraAimZOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraAimZNew = m_CameraAimZOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_AIM, m_CameraAimZOrig);
				SetDlgItemText(IDC_CAMERAAIM_Z, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_UP_X == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraUpX(&m_CameraUpXOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraUpXNew = m_CameraUpXOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_UP, m_CameraUpXOrig);
				SetDlgItemText(IDC_CAMERAUP_X, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_UP_Y == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraUpY(&m_CameraUpYOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraUpYNew = m_CameraUpYOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_UP, m_CameraUpYOrig);
				SetDlgItemText(IDC_CAMERAUP_Y, g_cnvBuf);
			}
		}
	}

	if (SSCLID_CAMERA_UP_Z == dispid || DISPID_UNKNOWN == dispid)
	{
		HRESULT hr = pCtl->get_CameraUpZ(&m_CameraUpZOrig);

		if (SUCCEEDED(hr))
		{
			m_CameraUpZNew = m_CameraUpZOrig;

			if (IsWindow())
			{
				_stprintf(g_cnvBuf, FMT_CAMERA_UP, m_CameraUpZOrig);
				SetDlgItemText(IDC_CAMERAUP_Z, g_cnvBuf);
			}
		}
	}
}

void CViewPropPage::SetPropertiesFromControls(DISPID dispid)
{
	// In case QI failed in SetObjects
	if (!m_ppUnk || !m_ppUnk[0]) return;

	// For all objects in array...
	for (UINT i = 0; i < m_nObjects; i++)
	{
		// Get the appropriate interface...
		CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[i];

		// Update the Windowed property, if requested and required
		if ((SSCLID_WINDOWED == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & VIEWPORT_TYPE_CHANGED)
		{
			pCtl->put_Windowed(m_bWindowedNew);       // Update property
			m_bWindowedOrig = m_bWindowedNew;         // Prop page and property synced
			m_changeFlags &= ~VIEWPORT_TYPE_CHANGED;  // Clear changed flag
		}

		// Update the FOV property, if requested and required
		if ((SSCLID_FOV == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & FOV_CHANGED)
		{
			pCtl->put_FOV(m_FOVNew);       // Update property
			m_FOVOrig = m_FOVNew;          // Prop page and property synced
			m_changeFlags &= ~FOV_CHANGED; // Clear changed flag
		}

		// Update the Aspect property, if requested and required
		if ((SSCLID_ASPECT == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & ASPECT_CHANGED)
		{
			pCtl->put_Aspect(m_AspectNew);       // Update property
			m_AspectOrig = m_AspectNew;          // Prop page and property synced
			m_changeFlags &= ~ASPECT_CHANGED; // Clear changed flag
		}

		// Update the AutoAspect property, if requested and required
		if ((SSCLID_AUTO_ASPECT == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & AUTO_ASPECT_CHANGED)
		{
			pCtl->put_AutoAspect(m_bAutoAspectNew);       // Update property
			m_bAutoAspectOrig = m_bAutoAspectNew;          // Prop page and property synced
			m_changeFlags &= ~AUTO_ASPECT_CHANGED; // Clear changed flag
		}

		// Update the NearPlane property, if requested and required
		if ((SSCLID_NEARPLANE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & NEARPLANE_CHANGED)
		{
			pCtl->put_NearPlane(m_NearPlaneNew);       // Update property
			m_NearPlaneOrig = m_NearPlaneNew;          // Prop page and property synced
			m_changeFlags &= ~NEARPLANE_CHANGED; // Clear changed flag
		}

		// Update the FarPlane property, if requested and required
		if ((SSCLID_FARPLANE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & FARPLANE_CHANGED)
		{
			pCtl->put_FarPlane(m_FarPlaneNew);       // Update property
			m_FarPlaneOrig = m_FarPlaneNew;          // Prop page and property synced
			m_changeFlags &= ~FARPLANE_CHANGED; // Clear changed flag
		}

		// Update the CameraLocationX property, if requested and required
		if ((SSCLID_CAMERA_LOCATION_X == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CAMERALOCX_CHANGED)
		{
			pCtl->put_CameraLocationX(m_CameraLocXNew);       // Update property
			m_CameraLocXOrig = m_CameraLocXNew;          // Prop page and property synced
			m_changeFlags &= ~CAMERALOCX_CHANGED; // Clear changed flag
		}

		// Update the CameraLocationY property, if requested and required
		if ((SSCLID_CAMERA_LOCATION_Y == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CAMERALOCY_CHANGED)
		{
			pCtl->put_CameraLocationY(m_CameraLocYNew);       // Update property
			m_CameraLocYOrig = m_CameraLocYNew;          // Prop page and property synced
			m_changeFlags &= ~CAMERALOCY_CHANGED; // Clear changed flag
		}

		// Update the CameraLocationZ property, if requested and required
		if ((SSCLID_CAMERA_LOCATION_Z == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CAMERALOCZ_CHANGED)
		{
			pCtl->put_CameraLocationZ(m_CameraLocZNew);       // Update property
			m_CameraLocZOrig = m_CameraLocZNew;          // Prop page and property synced
			m_changeFlags &= ~CAMERALOCZ_CHANGED; // Clear changed flag
		}

		// Update the CameraAimX property, if requested and required
		if ((SSCLID_CAMERA_AIM_X == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CAMERAAIMX_CHANGED)
		{
			pCtl->put_CameraAimX(m_CameraAimXNew);       // Update property
			m_CameraAimXOrig = m_CameraAimXNew;          // Prop page and property synced
			m_changeFlags &= ~CAMERAAIMX_CHANGED; // Clear changed flag
		}

		// Update the CameraAimY property, if requested and required
		if ((SSCLID_CAMERA_AIM_Y == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CAMERAAIMY_CHANGED)
		{
			pCtl->put_CameraAimY(m_CameraAimYNew);       // Update property
			m_CameraAimYOrig = m_CameraAimYNew;          // Prop page and property synced
			m_changeFlags &= ~CAMERAAIMY_CHANGED; // Clear changed flag
		}

		// Update the CameraAimZ property, if requested and required
		if ((SSCLID_CAMERA_AIM_Z == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CAMERAAIMZ_CHANGED)
		{
			pCtl->put_CameraAimZ(m_CameraAimZNew);       // Update property
			m_CameraAimZOrig = m_CameraAimZNew;          // Prop page and property synced
			m_changeFlags &= ~CAMERAAIMZ_CHANGED; // Clear changed flag
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTypesPropPage

LRESULT CTypesPropPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	InitializeControlsFromObject(DISPID_UNKNOWN);
	return 0;
}

STDMETHODIMP CTypesPropPage::Apply(void)
{
    ATLTRACE(_T("CTypesPropPage::Apply\n"));
    SetPropertiesFromControls(DISPID_UNKNOWN);
    m_bDirty = FALSE;
    m_changeFlags  = 0;
    return S_OK;
}

LRESULT CTypesPropPage::OnChangeModelType(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_MODELTYPE, g_txtBuf, g_txtBufSize) > 0)
	{
		m_strModelTypeNew = g_txtBuf;
	}

	if (m_strModelTypeOrig != m_strModelTypeNew)
	{
		m_changeFlags |= MODELTYPE_CHANGED;
	}
	else
	{
		m_changeFlags &= ~MODELTYPE_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

LRESULT CTypesPropPage::OnChangeViewportType(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_VIEWPORTTYPE, g_txtBuf, g_txtBufSize) > 0)
	{
		m_strViewportTypeNew = g_txtBuf;
	}

	if (m_strViewportTypeOrig != m_strViewportTypeNew)
	{
		m_changeFlags |= VIEWPORTTYPE_CHANGED;
	}
	else
	{
		m_changeFlags &= ~VIEWPORTTYPE_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

LRESULT CTypesPropPage::OnChangeControllerType(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_CONTROLLERTYPE, g_txtBuf, g_txtBufSize) > 0)
	{
		m_strControllerTypeNew = g_txtBuf;
	}

	if (m_strControllerTypeOrig != m_strControllerTypeNew)
	{
		m_changeFlags |= CONTROLLERTYPE_CHANGED;
	}
	else
	{
		m_changeFlags &= ~CONTROLLERTYPE_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

LRESULT CTypesPropPage::OnChangeModelReaderType(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_MODELREADERTYPE, g_txtBuf, g_txtBufSize) > 0)
	{
		m_strModelReaderTypeNew = g_txtBuf;
	}

	if (m_strModelReaderTypeOrig != m_strModelReaderTypeNew)
	{
		m_changeFlags |= MODELREADERTYPE_CHANGED;
	}
	else
	{
		m_changeFlags &= ~MODELREADERTYPE_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

void CTypesPropPage::InitializeControlsFromObject(DISPID dispid)
{
	HRESULT hr = S_OK;
	USES_CONVERSION;

	CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[0];
	BSTR strBuf = NULL;

	if (SSCLID_MODEL_TYPE == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_ModelType(&strBuf);

		if (SUCCEEDED(hr))
		{
			m_strModelTypeOrig = strBuf;
			m_strModelTypeNew = m_strModelTypeOrig;

			if (IsWindow())
			{
				SetDlgItemText(IDC_MODELTYPE, OLE2CA(m_strModelTypeOrig));
			}
			::SysFreeString(strBuf);
		}
	}

	if (SSCLID_VIEWPORT_TYPE == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_ViewportType(&strBuf);

		if (SUCCEEDED(hr))
		{
			m_strViewportTypeOrig = strBuf;
			m_strViewportTypeNew = m_strViewportTypeOrig;

			if (IsWindow())
			{
				SetDlgItemText(IDC_VIEWPORTTYPE, OLE2CA(m_strViewportTypeOrig));
			}
			::SysFreeString(strBuf);
		}
	}

	if (SSCLID_CONTROLLER_TYPE == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_ControllerType(&strBuf);

		if (SUCCEEDED(hr))
		{
			m_strControllerTypeOrig = strBuf;
			m_strControllerTypeNew = m_strControllerTypeOrig;

			if (IsWindow())
			{
				SetDlgItemText(IDC_CONTROLLERTYPE, OLE2CA(m_strControllerTypeOrig));
			}
			::SysFreeString(strBuf);
		}
	}

	if (SSCLID_MODEL_READER_TYPE == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_ModelReaderType(&strBuf);

		if (SUCCEEDED(hr))
		{
			m_strModelReaderTypeOrig = strBuf;
			m_strModelReaderTypeNew = m_strModelReaderTypeOrig;

			if (IsWindow())
			{
				SetDlgItemText(IDC_MODELREADERTYPE, OLE2CA(m_strModelReaderTypeOrig));
			}
			::SysFreeString(strBuf);
		}
	}
}

void CTypesPropPage::SetPropertiesFromControls(DISPID dispid)
{
	// In case QI failed in SetObjects
	if (!m_ppUnk || !m_ppUnk[0]) return;

	// For all objects in array...
	for (UINT i = 0; i < m_nObjects; i++)
	{
		// Get the appropriate interface...
		CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[i];

		// Update the ModelType property, if requested and required
		if ((SSCLID_MODEL_TYPE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & MODELTYPE_CHANGED)
		{
			pCtl->put_ModelType(m_strModelTypeNew);       // Update property
			m_strModelTypeOrig = m_strModelTypeNew;          // Prop page and property synced
			m_changeFlags &= ~MODELTYPE_CHANGED; // Clear changed flag
		}

		// Update the ViewportType property, if requested and required
		if ((SSCLID_VIEWPORT_TYPE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & VIEWPORTTYPE_CHANGED)
		{
			pCtl->put_ViewportType(m_strViewportTypeNew);       // Update property
			m_strViewportTypeOrig = m_strViewportTypeNew;          // Prop page and property synced
			m_changeFlags &= ~VIEWPORTTYPE_CHANGED; // Clear changed flag
		}

		// Update the ControllerType property, if requested and required
		if ((SSCLID_CONTROLLER_TYPE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & CONTROLLERTYPE_CHANGED)
		{
			pCtl->put_ControllerType(m_strControllerTypeNew);       // Update property
			m_strControllerTypeOrig = m_strControllerTypeNew;          // Prop page and property synced
			m_changeFlags &= ~CONTROLLERTYPE_CHANGED; // Clear changed flag
		}

		// Update the ModelReaderType property, if requested and required
		if ((SSCLID_MODEL_READER_TYPE == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & MODELREADERTYPE_CHANGED)
		{
			pCtl->put_ModelReaderType(m_strModelReaderTypeNew);       // Update property
			m_strModelReaderTypeOrig = m_strModelReaderTypeNew;          // Prop page and property synced
			m_changeFlags &= ~MODELREADERTYPE_CHANGED; // Clear changed flag
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBehaviorPropPage

const LPCTSTR FMT_ANIMATION_INTERVAL = _T("%d");

LRESULT CBehaviorPropPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	InitializeControlsFromObject(DISPID_UNKNOWN);
	return 0;
}

STDMETHODIMP CBehaviorPropPage::Apply(void)
{
    ATLTRACE(_T("CBehaviorPropPage::Apply\n"));
    SetPropertiesFromControls(DISPID_UNKNOWN);
    m_bDirty = FALSE;
    m_changeFlags  = 0;
    return S_OK;
}

LRESULT CBehaviorPropPage::OnChangeAnimationEnabled(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	CButton btnAnimationEnabled = GetDlgItem(IDC_ANIMATION_ENABLED);

    if (btnAnimationEnabled.GetCheck() == 0)
	{
		m_bAnimationEnabledNew = VARIANT_FALSE;
	}
	else
	{
		m_bAnimationEnabledNew = VARIANT_TRUE;
	}

	if (m_bAnimationEnabledOrig != m_bAnimationEnabledNew)
	{
		m_changeFlags |= ANIMATION_ENABLED_CHANGED;
	}
	else
	{
		m_changeFlags &= ~ANIMATION_ENABLED_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	if (m_bAnimationEnabledNew)
	{
		::EnableWindow(GetDlgItem(IDC_ANIMATION_INTERVAL), TRUE);
	}
	else
	{
		::EnableWindow(GetDlgItem(IDC_ANIMATION_INTERVAL), FALSE);
	}

	return 0;
}

LRESULT CBehaviorPropPage::OnChangeAnimationInterval(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
    if (GetDlgItemText(IDC_ANIMATION_INTERVAL, g_cnvBuf, g_cnvBufSize) > 0)
	{
		FilterConversionBuffer();
		m_nAnimationIntervalNew = atoi(g_cnvBuf);
	}

	if (m_nAnimationIntervalNew != m_nAnimationIntervalOrig)
	{
		m_changeFlags |= ANIMATION_INTERVAL_CHANGED;
	}
	else
	{
		m_changeFlags &= ~ANIMATION_INTERVAL_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

LRESULT CBehaviorPropPage::OnKillFocusAnimationInterval(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	_stprintf(g_cnvBuf, FMT_ANIMATION_INTERVAL, m_nAnimationIntervalNew);
	SetDlgItemText(IDC_ANIMATION_INTERVAL, g_cnvBuf);
	return 0;
}

LRESULT CBehaviorPropPage::OnChangeCollisionsEnabled(WORD /* wNotifyCode */, WORD /* wID */, HWND /* hWndCtl */, BOOL& /* bHandled */)
{
	CButton btnCollisionsEnabled = GetDlgItem(IDC_COLLISIONS_ENABLED);

    if (btnCollisionsEnabled.GetCheck() == 0)
	{
		m_bCollisionsEnabledNew = VARIANT_FALSE;
	}
	else
	{
		m_bCollisionsEnabledNew = VARIANT_TRUE;
	}

	if (m_bCollisionsEnabledOrig != m_bCollisionsEnabledNew)
	{
		m_changeFlags |= COLLISIONS_ENABLED_CHANGED;
	}
	else
	{
		m_changeFlags &= ~COLLISIONS_ENABLED_CHANGED;
	}

	SetDirty(m_changeFlags != 0);

	return 0;
}

void CBehaviorPropPage::InitializeControlsFromObject(DISPID dispid)
{
	HRESULT hr = S_OK;
	USES_CONVERSION;

	CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[0];

	if (SSCLID_ANIMATION_ENABLED == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_AnimationEnabled(&m_bAnimationEnabledOrig);

		if (SUCCEEDED(hr))
		{
			m_bAnimationEnabledNew = m_bAnimationEnabledOrig;

			if (IsWindow())
			{
				CButton btnAnimationEnabled = GetDlgItem(IDC_ANIMATION_ENABLED);

				if (!m_bAnimationEnabledOrig)
				{
					btnAnimationEnabled.SetCheck(0);
				}
				else
				{
					btnAnimationEnabled.SetCheck(1);
				}
			}
		}
	}

	if (SSCLID_ANIMATION_INTERVAL == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_AnimationInterval(&m_nAnimationIntervalOrig);

		if (SUCCEEDED(hr))
		{
			m_nAnimationIntervalNew = m_nAnimationIntervalOrig;

			if (IsWindow())
			{
				SetDlgItemInt(IDC_ANIMATION_INTERVAL, m_nAnimationIntervalOrig);

				if (m_bAnimationEnabledOrig)
				{
					::EnableWindow(GetDlgItem(IDC_ANIMATION_INTERVAL), TRUE);
				}
				else
				{
					::EnableWindow(GetDlgItem(IDC_ANIMATION_INTERVAL), FALSE);
				}
			}
		}
	}

	if (SSCLID_COLLISIONS_ENABLED == dispid || DISPID_UNKNOWN == dispid)
	{
		hr = pCtl->get_CollisionsEnabled(&m_bCollisionsEnabledOrig);

		if (SUCCEEDED(hr))
		{
			m_bCollisionsEnabledNew = m_bCollisionsEnabledOrig;

			if (IsWindow())
			{
				CButton btnCollisionsEnabled = GetDlgItem(IDC_COLLISIONS_ENABLED);

				if (!m_bCollisionsEnabledOrig)
				{
					btnCollisionsEnabled.SetCheck(0);
				}
				else
				{
					btnCollisionsEnabled.SetCheck(1);
				}
			}
		}
	}
}

void CBehaviorPropPage::SetPropertiesFromControls(DISPID dispid)
{
	// In case QI failed in SetObjects
	if (!m_ppUnk || !m_ppUnk[0]) return;

	// For all objects in array...
	for (UINT i = 0; i < m_nObjects; i++)
	{
		// Get the appropriate interface...
		CComQIPtr<IShapeShifterCtl> pCtl = m_ppUnk[i];

		// Update the AnimationEnabled property, if requested and required
		if ((SSCLID_ANIMATION_ENABLED == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & ANIMATION_ENABLED_CHANGED)
		{
			pCtl->put_AnimationEnabled(m_bAnimationEnabledNew);       // Update property
			m_bAnimationEnabledOrig = m_bAnimationEnabledNew;          // Prop page and property synced
			m_changeFlags &= ~ANIMATION_ENABLED_CHANGED; // Clear changed flag
		}

		// Update the AnimationInterval property, if requested and required
		if ((SSCLID_ANIMATION_INTERVAL == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & ANIMATION_INTERVAL_CHANGED)
		{
			pCtl->put_AnimationInterval(m_nAnimationIntervalNew);       // Update property
			m_nAnimationIntervalOrig = m_nAnimationIntervalNew;          // Prop page and property synced
			m_changeFlags &= ~ANIMATION_INTERVAL_CHANGED; // Clear changed flag
		}

		// Update the CollisionsEnabled property, if requested and required
		if ((SSCLID_COLLISIONS_ENABLED == dispid || DISPID_UNKNOWN == dispid) &&
			m_changeFlags & COLLISIONS_ENABLED_CHANGED)
		{
			pCtl->put_CollisionsEnabled(m_bCollisionsEnabledNew);       // Update property
			m_bCollisionsEnabledOrig = m_bCollisionsEnabledNew;          // Prop page and property synced
			m_changeFlags &= ~COLLISIONS_ENABLED_CHANGED; // Clear changed flag
		}
	}
}

};  // namespace ShapeShifter
