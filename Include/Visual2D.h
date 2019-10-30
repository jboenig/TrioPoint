//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Visual2D.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __VISUAL2D_H__
#define __VISUAL2D_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// IVisual2DImpl

template <class base_class>
class IVisual2DImpl : public base_class
{
// Constructors/destructor
public:
	IVisual2DImpl()
	{
		m_rcBounds.left = m_rcBounds.top = 0;
		m_rcBounds.right = m_rcBounds.bottom = 0;
	}

// Attributes
protected:
	RECT m_rcBounds;

// Operations
public:
	STDMETHOD(get_Left)(/*[out, retval]*/ long *pVal)
	{
		*pVal = m_rcBounds.left;
		return S_OK;
	}

	STDMETHOD(put_Left)(/*[in]*/ long newVal)
	{
		m_rcBounds.left = newVal;
		return S_OK;
	}

	STDMETHOD(get_Top)(/*[out, retval]*/ long *pVal)
	{
		if (pVal != NULL)
		{
			*pVal = m_rcBounds.top;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(put_Top)(/*[in]*/ long newVal)
	{
		m_rcBounds.top = newVal;
		return S_OK;
	}

	STDMETHOD(get_Width)(/*[out, retval]*/ long *pVal)
	{
		if (pVal != NULL)
		{
			*pVal = m_rcBounds.right - m_rcBounds.left;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(put_Width)(/*[in]*/ long newVal)
	{
		m_rcBounds.right = m_rcBounds.left + newVal;
		return S_OK;
	}

	STDMETHOD(get_Height)(/*[out, retval]*/ long *pVal)
	{
		if (pVal != NULL)
		{
			*pVal = m_rcBounds.bottom - m_rcBounds.top;
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(put_Height)(/*[in]*/ long newVal)
	{
		m_rcBounds.bottom = m_rcBounds.top + newVal;
		return S_OK;
	}

	STDMETHOD(SetBounds)(/* [in] */ LPRECT pBounds)
	{
		m_rcBounds = *pBounds;
		return S_OK;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __VISUAL2D_H__
