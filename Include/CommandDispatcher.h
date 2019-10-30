//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: CommandDispatcher.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __COMMANDDISPATCHER_H__
#define __COMMANDDISPATCHER_H__

#pragma once

#include <deque>

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// ICommandDispatcherImpl

template <class base_class>
class ICommandDispatcherImpl : public base_class
{
// Constructors/destructor
public:
	ICommandDispatcherImpl() :
		m_nMaxHistory(100)
	{
	}

// Attributes
protected:
	long m_nMaxHistory;
	std::deque<ICommandPtr> m_undoStack;
	std::deque<ICommandPtr> m_redoStack;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommandDispatcher interface

	STDMETHOD(get_MaxHistory)(/* [out, retval] */ long* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = m_nMaxHistory;

		return S_OK;
	}

	STDMETHOD(put_MaxHistory)(/* [in] */ long newVal)
	{
		m_nMaxHistory = newVal;
		return S_OK;
	}

	STDMETHOD(get_UndoCount)(/* [out, retval] */ long* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = m_undoStack.size();

		return S_OK;
	}

	STDMETHOD(get_RedoCount)(/* [out, retval] */ long* pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		*pVal = m_redoStack.size();

		return S_OK;
	}

	STDMETHOD(Do)(/* [in] */ ICommand* pCommand)
	{
		if (pCommand == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = pCommand->Do(GetCommandTarget());

		if (SUCCEEDED(hr))
		{
			m_undoStack.push_front(pCommand);
		}

		return hr;
	}

	STDMETHOD(Undo)(/* [in] */ long nNumCommands)
	{
		if (nNumCommands <= 0)
		{
			return E_POINTER;
		}

		HRESULT hr = S_OK;

		while (SUCCEEDED(hr) && nNumCommands > 0 && !m_undoStack.empty())
		{
			ICommandPtr spCommand = m_undoStack.front();
			m_undoStack.pop_front();
			hr = spCommand->Undo(GetCommandTarget());

			if (SUCCEEDED(hr))
			{
				m_redoStack.push_front(spCommand);
			}

			nNumCommands--;
		}

		ClearExpiredCommands();

		if (SUCCEEDED(hr))
		{
			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(Redo)(/* [in] */ long nNumCommands)
	{
		if (nNumCommands <= 0)
		{
			return E_INVALIDARG;
		}

		HRESULT hr = S_OK;

		while (SUCCEEDED(hr) && nNumCommands > 0 && !m_redoStack.empty())
		{
			ICommandPtr spCommand = m_redoStack.front();
			m_redoStack.pop_front();
			hr = spCommand->Do(GetCommandTarget());

			if (SUCCEEDED(hr))
			{
				m_undoStack.push_front(spCommand);
			}

			nNumCommands--;
		}

		ClearExpiredCommands();

		if (SUCCEEDED(hr))
		{
			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(PeekUndo)(/* [in] */ long nStackOffset, /* [out,retval] */ ICommand** ppCommand)
	{
		if (ppCommand == NULL)
		{
			return E_POINTER;
		}

		if (nStackOffset < 0 || nStackOffset >= (long) m_undoStack.size())
		{
			return E_INVALIDARG;
		}

		*ppCommand = m_undoStack[nStackOffset];

		if (*ppCommand != NULL)
		{
			(*ppCommand)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(PeekRedo)(/* [in] */ long nStackOffset, /* [out,retval] */ ICommand** ppCommand)
	{
		if (ppCommand == NULL)
		{
			return E_POINTER;
		}

		if (nStackOffset < 0 || nStackOffset >= (long) m_redoStack.size())
		{
			return E_INVALIDARG;
		}

		*ppCommand = m_redoStack[nStackOffset];

		if (*ppCommand != NULL)
		{
			(*ppCommand)->AddRef();
		}

		return S_OK;
	}

	STDMETHOD(ClearHistory)()
	{
		m_undoStack.clear();
		m_redoStack.clear();
		return S_OK;
	}

// Implementation
protected:
	void ClearExpiredCommands()
	{
		while ((long) m_undoStack.size() > m_nMaxHistory)
		{
			m_undoStack.pop_back();
		}

		while ((long) m_redoStack.size() > m_nMaxHistory)
		{
			m_redoStack.pop_back();
		}
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __COMMANDDISPATCHER_H__

