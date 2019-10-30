//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: AnimationLoop.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __ANIMATIONLOOP_H__
#define __ANIMATIONLOOP_H__

#pragma once

#include <list>

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CAnimationLoop

class ATL_NO_VTABLE CAnimationLoop : public CComObjectRootEx<CComSingleThreadModel>,
                                     public CComCoClass<CAnimationLoop, &CLSID_AnimationLoop>,
                                     public IDispatchImpl<IAnimationLoop, &__uuidof(IAnimationLoop), &LIBID_ShapeShifter>
{
// Embedded types
public:
	typedef std::list<IAnimate*> _AnimatorList;

// Constructors/destructor
public:
	CAnimationLoop();
	virtual ~CAnimationLoop();

DECLARE_REGISTRY_RESOURCEID(IDR_ANIMATION_LOOP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CAnimationLoop)
	COM_INTERFACE_ENTRY(IAnimationLoop)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	_AnimatorList m_animators;
	DWORD m_dwInterval;
	bool m_bRunning;
	DWORD m_dwLastTime;
	DWORD m_dwCurTime;
	DWORD m_dwThreadId;
	HANDLE m_hThread;
	bool m_bFirstFrame;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IAnimationLoop interface

	STDMETHOD(AddAnimator)(/* [in] */ IAnimate* pAnimator);
	STDMETHOD(RemoveAnimator)(/* [in] */ IAnimate* pAnimator);
	STDMETHOD(get_Interval)(/* [out, retval] */ long *pVal);
	STDMETHOD(put_Interval)(/* [in] */ long newVal);
	STDMETHOD(get_Running)(/* [out, retval] */ VARIANT_BOOL *pVal);
	STDMETHOD(Start)();
	STDMETHOD(FireAnimation)(/* [in] */ long nTime);
	STDMETHOD(Stop)();

// Implementation
protected:
	void FreeAllAnimators();
};

};  // namespace ShapeShifter

#endif // #ifndef __ANIMATIONLOOP_H__
