//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: MatrixStack.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MATRIXSTACK_H__
#define __MATRIXSTACK_H__

#pragma once

#include "ShapeShifter.h"
#include "Node.h"

namespace ShapeShifter {

namespace MatrixStack {

extern D3DXMATRIX& Push(const D3DXMATRIX& mat);
extern D3DXMATRIX& Peek();
extern D3DXMATRIX Pop();
extern void Clear();

};  // namespace MatrixStack

};  // namespace ShapeShifter

#endif // #ifndef __MATRIXSTACK_H__
