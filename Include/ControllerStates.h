//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: ControllerStates.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __CONTROLLERSTATES_H__
#define __CONTROLLERSTATES_H__

#pragma once

namespace ShapeShifter {

	namespace ControllerState {

		const long READY = 0;
		const long MOVING_NODES = 1;
		const long ADDING_NODES = 2;

	};  // namespace ControllerState

};  // namespace ShapeShifter

#endif // #ifndef __CONTROLLERSTATES_H__
