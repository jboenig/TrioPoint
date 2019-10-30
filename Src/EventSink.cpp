//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: EventSink.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include "EventSink.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CModelEvents

_ATL_FUNC_INFO OnAddNodeInfo =
	{CC_STDCALL, VT_EMPTY, 1, { VT_DISPATCH } };

_ATL_FUNC_INFO OnRemoveNodeInfo =
	{CC_STDCALL, VT_EMPTY, 1, { VT_DISPATCH } };

_ATL_FUNC_INFO OnMoveNodeInfo =
	{CC_STDCALL, VT_EMPTY, 1, { VT_DISPATCH } };

/////////////////////////////////////////////////////////////////////////////
// CControllerEvents

_ATL_FUNC_INFO OnSelectionChangeInfo =
	{CC_STDCALL, VT_EMPTY, 1, { VT_DISPATCH } };

};  // namespace ShapeShifter
