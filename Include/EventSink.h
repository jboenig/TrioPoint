//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: EventSink.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __EVENTSINK_H__
#define __EVENTSINK_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

class ATL_NO_VTABLE IDispatchEventSink
{
public:
	IDispatchEventSink() {m_dwEventCookie = 0xFEFEFEFE; m_iid = IID_NULL; }

	//this method needs a different name than QueryInterface
	STDMETHOD(_ES_QueryInterface)(REFIID riid, void ** ppvObject) = 0;

	virtual ULONG STDMETHODCALLTYPE _ES_AddRef(void) = 0;
	virtual ULONG STDMETHODCALLTYPE _ES_Release(void) = 0;

	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo) = 0;
	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo) = 0;
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid) = 0;
	STDMETHOD(_ES_Invoke)(DISPID dispidMember, REFIID,
		LCID lcid, WORD /*wFlags*/, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* /*pexcepinfo*/, UINT* /*puArgErr*/) = 0;

	IID m_iid; // used for dynamic case
	DWORD m_dwEventCookie;

	HRESULT DispEventAdvise(IUnknown* pUnk, const IID* piid)
	{
		ATLASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return AtlAdvise(pUnk, (IUnknown*)this, *piid, &m_dwEventCookie);
	}

	HRESULT DispEventUnadvise(IUnknown* pUnk, const IID* piid)
	{
		HRESULT hr = AtlUnadvise(pUnk, *piid, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}
};

template <UINT nID, const IID* piid>
class ATL_NO_VTABLE COMIdentity_IDispatchEventSink : public IDispatchEventSink
{
public:
};

template <UINT nID, class T, const IID* pdiid>
class ATL_NO_VTABLE IDispatchEventSinkImpl : public COMIdentity_IDispatchEventSink<nID, pdiid>
{
public:
	STDMETHOD(_ES_QueryInterface)(REFIID riid, void ** ppvObject)
	{
		if (::InlineIsEqualGUID(riid, *pdiid) || 
			InlineIsEqualUnknown(riid) ||
			::InlineIsEqualGUID(riid, IID_IDispatch) ||
			::InlineIsEqualGUID(riid, m_iid))
		{
			if (ppvObject == NULL)
				return E_POINTER;
			*ppvObject = this;
			_ES_AddRef();
#ifdef _ATL_DEBUG_INTERFACES
			_Module.AddThunk((IUnknown**)ppvObject, _T("IDispEventImpl"), riid);
#endif // _ATL_DEBUG_INTERFACES
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}

	// These are here only to support use in non-COM objects	
	virtual ULONG STDMETHODCALLTYPE _ES_AddRef()
	{
		return 1;
	}
	virtual ULONG STDMETHODCALLTYPE _ES_Release()
	{
		return 1;
	}

	STDMETHOD(GetTypeInfoCount)(UINT* /* pctinfo */)
	{return E_NOTIMPL;}

	STDMETHOD(GetTypeInfo)(UINT /* itinfo */, LCID /* lcid */, ITypeInfo** /* pptinfo */)
	{return E_NOTIMPL;}

	STDMETHOD(GetIDsOfNames)(REFIID /* riid */, LPOLESTR* /* rgszNames */, UINT /* cNames */,
		LCID /* lcid */, DISPID* /* rgdispid */)
	{return E_NOTIMPL;}

	STDMETHOD(_ES_Invoke)(DISPID dispidMember, REFIID,
		LCID /* lcid */, WORD /*wFlags*/, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* /*pexcepinfo*/, UINT* /*puArgErr*/)
	{
		const _ATL_EVENT_ENTRY<T>* pMap = T::_GetSinkMap();
		const _ATL_EVENT_ENTRY<T>* pFound = NULL;

		while (pMap->piid != NULL)
		{
			if ((pMap->nControlID == nID) && (pMap->dispid == dispidMember) && 
				(pMap->piid == pdiid)) //comparing pointers here should be adequate
			{
				pFound = pMap;
				break;
			}
			pMap++;
		}
		if (pFound == NULL)
			return S_OK;
		
		if (pFound->pInfo != NULL)
		{
			InvokeFromFuncInfo(pFound->pfn, *(pFound->pInfo), pdispparams, pvarResult);
		}

		return S_OK;
	}

	//Helper for invoking the event
	HRESULT InvokeFromFuncInfo(void (__stdcall T::*pEvent)(), _ATL_FUNC_INFO& info, DISPPARAMS* pdispparams, VARIANT* pvarResult)
	{
		T* pT = static_cast<T*>(this);
		VARIANTARG** pVarArgs = info.nParams ? (VARIANTARG**)alloca(sizeof(VARIANTARG*)*info.nParams) : 0;
		for (int i=0; i<info.nParams; i++)
			pVarArgs[i] = &pdispparams->rgvarg[info.nParams - i - 1];

		CComStdCallThunk<T> thunk;
		thunk.Init(pEvent, pT);
		CComVariant tmpResult;
		if (pvarResult == NULL)
			pvarResult = &tmpResult;

		HRESULT hr = DispCallFunc(
			&thunk,
			0,
			info.cc,
			info.vtReturn,
			info.nParams,
			info.pVarTypes,
			pVarArgs,
			pvarResult);
		ATLASSERT(SUCCEEDED(hr));
		return hr;
	}

	//Helpers for sinking events on random IUnknown*
#if 0
	HRESULT DispEventAdvise(IUnknown* pUnk, const IID* piid)
	{
		ATLASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return AtlAdvise(pUnk, (IUnknown*)this, *piid, &m_dwEventCookie);
	}
	HRESULT DispEventUnadvise(IUnknown* pUnk, const IID* piid)
	{
		HRESULT hr = AtlUnadvise(pUnk, *piid, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}
#endif

	HRESULT DispEventAdvise(IUnknown* pUnk)
	{
		return IDispatchEventSink::DispEventAdvise(pUnk, pdiid);
	}
	HRESULT DispEventUnadvise(IUnknown* pUnk)
	{
		return IDispatchEventSink::DispEventUnadvise(pUnk, pdiid);
	}
};

#define SSCL_BEGIN_SINK_MAP(_class)\
	static const _ATL_EVENT_ENTRY< _class >* _GetSinkMap()\
	{\
		typedef _class _atl_event_classtype;\
		static const _ATL_EVENT_ENTRY< _class > map[] = {


#define SSCL_SINK_ENTRY_INFO(id, iid, dispid, fn, info) {id, &iid, (int)(static_cast<COMIdentity_IDispatchEventSink<id, &iid>*>((_atl_event_classtype*)8))-8, dispid, (void (__stdcall _atl_event_classtype::*)())fn, info},
#define SSCL_SINK_ENTRY_EX(id, iid, dispid, fn) SSCL_SINK_ENTRY_INFO(id, iid, dispid, fn, NULL)
#define SSCL_SINK_ENTRY(id, dispid, fn) SSCL_SINK_ENTRY_EX(id, IID_NULL, dispid, fn)
#define SSCL_END_SINK_MAP() {0, NULL, 0, 0, NULL, NULL} }; return map;}

/////////////////////////////////////////////////////////////////////////////
// CModelEvents

extern _ATL_FUNC_INFO OnAddNodeInfo;
extern _ATL_FUNC_INFO OnRemoveNodeInfo;
extern _ATL_FUNC_INFO OnMoveNodeInfo;

template <class T>
class ATL_NO_VTABLE CSinkModelEvents : public IDispatchEventSinkImpl<0, CSinkModelEvents<T>, &DIID__IModelEvents>
{
public:

	virtual void __stdcall OnAddNode(/* [in] */ INode* pNode)
	{
		pNode;
	}

	virtual void __stdcall OnRemoveNode(/* [in] */ INode* pNode)
	{
		pNode;
	}

	virtual void __stdcall OnMoveNode(/* [in] */ INode* pNode)
	{
		pNode;
	}

SSCL_BEGIN_SINK_MAP(CSinkModelEvents<T>)
	SSCL_SINK_ENTRY_INFO(0, DIID__IModelEvents, SSCLID_ON_ADD_NODE, OnAddNode, &OnAddNodeInfo)
	SSCL_SINK_ENTRY_INFO(0, DIID__IModelEvents, SSCLID_ON_REMOVE_NODE, OnRemoveNode, &OnRemoveNodeInfo)
	SSCL_SINK_ENTRY_INFO(0, DIID__IModelEvents, SSCLID_ON_MOVE_NODE, OnMoveNode, &OnMoveNodeInfo)
SSCL_END_SINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CControllerEvents

extern _ATL_FUNC_INFO OnSelectionChangeInfo;

template <class T>
class ATL_NO_VTABLE CSinkControllerEvents : public IDispatchEventSinkImpl<0, CSinkControllerEvents<T>, &DIID__IControllerEvents>
{
public:

	virtual void __stdcall OnSelectionChange(IDispatch* pSelectionList)
	{
		pSelectionList;
	}

SSCL_BEGIN_SINK_MAP(CSinkControllerEvents<T>)
	SSCL_SINK_ENTRY_INFO(0, DIID__IControllerEvents, SSCLID_ON_SELECTION_CHANGE, OnSelectionChange, &OnSelectionChangeInfo)
SSCL_END_SINK_MAP()
};

};  // namespace ShapeShifter

#endif // #ifndef __EVENTSINK_H__
