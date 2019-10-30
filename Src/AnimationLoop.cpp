//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: AnimationLoop.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <mmsystem.h>

#include "AnimationLoop.h"

namespace ShapeShifter {

CAnimationLoop::CAnimationLoop() :
	m_dwInterval(0),
	m_bRunning(FALSE),
	m_dwLastTime(0),
	m_dwCurTime(0),
	m_dwThreadId(0),
	m_hThread(NULL),
	m_bFirstFrame(TRUE)
{
}

CAnimationLoop::~CAnimationLoop()
{
	Stop();
	FreeAllAnimators();
}

STDMETHODIMP CAnimationLoop::AddAnimator(/* [in] */ IAnimate* pAnimator)
{
	if (pAnimator == NULL)
	{
		return E_POINTER;
	}

	pAnimator->AddRef();
	m_animators.push_back(pAnimator);

	return S_OK;
}

STDMETHODIMP CAnimationLoop::RemoveAnimator(/* [in] */ IAnimate* pAnimator)
{
	pAnimator;
	return E_NOTIMPL;
}

STDMETHODIMP CAnimationLoop::get_Interval(/* [out, retval] */ long *pVal)
{
	if (pVal != NULL)
	{
		*pVal = static_cast<long>(m_dwInterval);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CAnimationLoop::put_Interval(/* [in] */ long newVal)
{
	m_dwInterval = static_cast<DWORD>(newVal);
	return S_OK;
}

STDMETHODIMP CAnimationLoop::get_Running(/* [out, retval] */ VARIANT_BOOL *pVal)
{
	if (pVal == NULL)
	{
		return E_POINTER;
	}

	*pVal = m_bRunning ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

static DWORD WINAPI AnimationProc(LPVOID lpParameter)
{
	IStream* pStm = (IStream*) lpParameter;
	IAnimationLoop* pAnimationLoop = NULL;
	long nInterval = 0;
	DWORD dwCurTime = 0;
	DWORD dwLastTime = 0;
	VARIANT_BOOL bRunning = TRUE;

	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	if (SUCCEEDED(hr))
	{
		hr = ::CoGetInterfaceAndReleaseStream(pStm, IID_IAnimationLoop, (LPVOID*) &pAnimationLoop);
	}

	if (FAILED(hr))
	{
		return 0;
	}

	pAnimationLoop->get_Interval(&nInterval);
	DWORD dwInterval = static_cast<DWORD>(nInterval);

	while (bRunning)
	{
		dwCurTime = timeGetTime();

		if ((dwCurTime - dwLastTime) > dwInterval)
		{
			pAnimationLoop->FireAnimation((long) dwCurTime);
			dwLastTime = dwCurTime;
		}

		pAnimationLoop->get_Running(&bRunning);
	}

	pAnimationLoop->Release();

	return 0;
}

STDMETHODIMP CAnimationLoop::Start()
{
	if (m_animators.size() == 0)
	{
		return E_FAIL;
	}

	if (m_bRunning != NULL)
	{
		return E_FAIL;
	}

	CRITICAL_SECTION cs;
	::InitializeCriticalSection(&cs);
	::EnterCriticalSection(&cs);

	HRESULT hr = E_FAIL;
	IStream* pStm = NULL;
	IAnimationLoop* pAnimationLoop = NULL;

	hr = this->QueryInterface(IID_IAnimationLoop, (LPVOID*) &pAnimationLoop);

	if (SUCCEEDED(hr))
	{
		hr = ::CoMarshalInterThreadInterfaceInStream(IID_IAnimationLoop, pAnimationLoop, &pStm);

		if (SUCCEEDED(hr))
		{
			m_hThread = ::CreateThread(NULL, 0, AnimationProc, pStm, 0, &m_dwThreadId);

			if (m_hThread == NULL)
			{
				hr = E_FAIL;
			}
			else
			{
				::SetThreadPriority(m_hThread, THREAD_PRIORITY_IDLE);
				m_bRunning = TRUE;
				m_bFirstFrame = TRUE;
			}
		}

		pAnimationLoop->Release();
	}

	::LeaveCriticalSection(&cs);

	return hr;
}

STDMETHODIMP CAnimationLoop::FireAnimation(/* [in] */ long nTime)
{
	HRESULT hr = S_OK;

	_AnimatorList::iterator itAnimator = m_animators.begin();

	while (itAnimator != m_animators.end() && hr == S_OK)
	{
		hr = (*itAnimator)->Animate(nTime, m_bFirstFrame);
		m_bFirstFrame = FALSE;
		itAnimator++;
	}

	return hr;
}

STDMETHODIMP CAnimationLoop::Stop()
{
	CRITICAL_SECTION cs;
	::InitializeCriticalSection(&cs);
	::EnterCriticalSection(&cs);

	m_bRunning = FALSE;

	::LeaveCriticalSection(&cs);

#if 0
	::WaitForSingleObject(m_hThread, INFINITE);
#endif
	m_hThread = NULL;

	return S_OK;
}

void CAnimationLoop::FreeAllAnimators()
{
	_AnimatorList::iterator itAnimator = m_animators.begin();

	while (itAnimator != m_animators.end())
	{
		(*itAnimator)->Release();
		itAnimator++;
	}

	m_animators.clear();
}

};  // namespace ShapeShifter
