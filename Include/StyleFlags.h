//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: StyleFlags.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __STYLEFLAGS_H__
#define __STYLEFLAGS_H__

#pragma once

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// Visual object style flags

namespace StyleFlag {

	const ULONG NONE               = 0x00000000;
	const ULONG ALLOW_SELECT       = 0x00000001;
	const ULONG ALLOW_MOVE         = 0x00000002;
	const ULONG ALLOW_ROTATE       = 0x00000004;
	const ULONG ALLOW_SCALE        = 0x00000008;
	const ULONG ALLOW_DELETE       = 0x00000010;
	const ULONG SOLID              = 0x00000020;
	const ULONG ALLOW_BSPTREE      = 0x00000040;
	const ULONG ALL                = 0xFFFFFFFF;

};

};  // namespace ShapeShifter

#endif // #ifndef __STYLEFLAGS_H__
