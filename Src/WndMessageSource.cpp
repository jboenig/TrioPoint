//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: WndMessageSource.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "WndMessageSource.h"

namespace ShapeShifter {

const LPCTSTR SHAPESHIFTER_MSGSOURCE = _T("ShapeShifter_MsgSource");

struct MsgSourceEntry
{
	IWndMessageSource* pMsgSource;
	WNDPROC lpfnOldWndProc;
};

LONG FAR PASCAL SubClassFunc(HWND hWnd, WORD Message, WORD wParam, LONG lParam)
{
	LONG lResult = 0;

	HANDLE hMsgSource = ::GetProp(hWnd, SHAPESHIFTER_MSGSOURCE);

	if (hMsgSource != NULL)
	{
		MsgSourceEntry* pMsgSourceEntry = (MsgSourceEntry*) ::GlobalLock(hMsgSource);
		if (pMsgSourceEntry != NULL)
		{
			if (Message == WM_DESTROY)
			{
				pMsgSourceEntry->pMsgSource->Disconnect();
			}
			else
			{
				pMsgSourceEntry->pMsgSource->SendWindowMessage(Message, wParam, lParam);
			}

			lResult = CallWindowProc(pMsgSourceEntry->lpfnOldWndProc, hWnd, Message, wParam, lParam);

			::GlobalUnlock(hMsgSource);
		}
	}

	return lResult;
}

CWndMessageSource::CWndMessageSource() :
	m_hWnd(NULL)
{
}

CWndMessageSource::~CWndMessageSource()
{
	while (m_receivers.size() > 0)
	{
		IWndMessageReceiver* pReceiver = m_receivers.back();
		pReceiver->Release();
		m_receivers.pop_back();
	}

	Disconnect();
}

STDMETHODIMP CWndMessageSource::Connect(/* [in] */ long WindowHandle)
{
	HRESULT hr = E_FAIL;
	HWND hWnd = reinterpret_cast<HWND>(WindowHandle);

	if (!IsWindow(hWnd))
	{
		return E_FAIL;
	}

	m_hWnd = hWnd;

	WNDPROC lpfnOldWndProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD) SubClassFunc);

	HANDLE hMsgSource = ::GlobalAlloc(GHND, sizeof(MsgSourceEntry));

	if (hMsgSource != NULL)
	{
		MsgSourceEntry* pMsgSourceEntry = (MsgSourceEntry*) ::GlobalLock(hMsgSource);
		if (pMsgSourceEntry != NULL)
		{
			pMsgSourceEntry->pMsgSource = static_cast<IWndMessageSource*>(this);
			pMsgSourceEntry->lpfnOldWndProc = lpfnOldWndProc;

			if (::SetProp(m_hWnd, SHAPESHIFTER_MSGSOURCE, hMsgSource))
			{
				hr = S_OK;
			}

			::GlobalUnlock(hMsgSource);
		}
	}

	return hr;
}

STDMETHODIMP CWndMessageSource::Disconnect()
{
	HRESULT hr = E_FAIL;

	if (m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		HANDLE hMsgSource = ::GetProp(m_hWnd, SHAPESHIFTER_MSGSOURCE);

		if (hMsgSource != NULL)
		{
			MsgSourceEntry* pMsgSourceEntry = (MsgSourceEntry*) ::GlobalLock(hMsgSource);
			if (pMsgSourceEntry != NULL)
			{
				SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD) pMsgSourceEntry->lpfnOldWndProc);
				::GlobalUnlock(hMsgSource);
			}

			::RemoveProp(m_hWnd, SHAPESHIFTER_MSGSOURCE);

			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CWndMessageSource::AddReceiver(/* [in] */ IUnknown* pReceiverUnk)
{
	HRESULT hr = E_FAIL;

	if (pReceiverUnk != NULL)
	{
		IWndMessageReceiver* pReceiver;
		hr = pReceiverUnk->QueryInterface(IID_IWndMessageReceiver, (LPVOID*) &pReceiver);
		if (SUCCEEDED(hr))
		{
			m_receivers.push_back(pReceiver);
			hr = S_OK;
		}
	}

	return hr;
}

STDMETHODIMP CWndMessageSource::RemoveReceiver(/* [in] */ IUnknown* pReceiverUnk)
{
	pReceiverUnk;
	return E_FAIL;
}

STDMETHODIMP CWndMessageSource::SendWindowMessage(/* [in] */ UINT uMsg, /* [in] */ WPARAM wParam, /* [in] */ LPARAM lParam)
{
	_Receivers::iterator itReceiver;

	for (itReceiver = m_receivers.begin(); itReceiver != m_receivers.end(); itReceiver++)
	{
		(*itReceiver)->ProcessWindowMessage(uMsg, wParam, lParam);
	}

	return S_OK;
}

};  // namespace ShapeShifter
