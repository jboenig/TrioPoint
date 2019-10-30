//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: WndMessageSource.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __WNDMESSAGESOURCE_H__
#define __WNDMESSAGESOURCE_H__

#pragma once

#include <vector>

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CWndMessageSource

class CWndMessageSource : public CComObjectRootEx<CComSingleThreadModel>,
                          public CComCoClass<CWndMessageSource, &CLSID_WndMessageSource>,
                          public IDispatchImpl<IWndMessageSource, &__uuidof(IWndMessageSource), &LIBID_ShapeShifter>
{
// Embedded types
public:
	typedef std::vector<IWndMessageReceiver*> _Receivers;

// Constructors/destructor
public:
	CWndMessageSource();
	virtual ~CWndMessageSource();

DECLARE_REGISTRY_RESOURCEID(IDR_WNDMESSAGESOURCE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CWndMessageSource)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IWndMessageSource)
END_COM_MAP()

// Attributes
protected:
	HWND m_hWnd;
	_Receivers m_receivers;

// Operations
public:
	STDMETHOD(Connect)(/* [in] */ long WindowHandle);
	STDMETHOD(Disconnect)();
	STDMETHOD(AddReceiver)(/* [in] */ IUnknown* pReceiverUnk);
	STDMETHOD(RemoveReceiver)(/* [in] */ IUnknown* pReceiverUnk);
	STDMETHOD(SendWindowMessage)(/* [in] */ UINT uMsg, /* [in] */ WPARAM wParam, /* [in] */ LPARAM lParam);
};

};  // namespace ShapeShifter

#endif // #ifndef __WNDMESSAGESOURCE_H__
