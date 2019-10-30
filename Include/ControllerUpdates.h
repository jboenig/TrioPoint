//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ControllerUpdates.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __CONTROLLERUPDATES_H__
#define __CONTROLLERUPDATES_H__

#pragma once

namespace ShapeShifter {

	namespace ControllerUpdate {

		const ULONG VIEWPORT       = 0x00000001;
		const ULONG SELECTION      = 0x00000002;
		const ULONG NODE_CLICK     = 0x00000004;
		const ULONG NODE_DBLCLICK  = 0x00000008;

	};  // namespace ControllerUpdate

};  // namespace ShapeShifter

#endif // #ifndef __CONTROLLERUPDATES_H__
