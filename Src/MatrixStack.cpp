//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: MatrixStack.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include <stack>
using namespace std;

#include "MatrixStack.h"

namespace ShapeShifter {

namespace MatrixStack {

typedef stack<D3DXMATRIX> _MatrixStack;
static _MatrixStack g_worldTransforms;

D3DXMATRIX& Push(const D3DXMATRIX& mat)
{
	D3DXMATRIX matTop;

	if (g_worldTransforms.empty())
	{
		g_worldTransforms.push(mat);
	}
	else
	{
		matTop = g_worldTransforms.top();
		matTop = matTop * mat;
		g_worldTransforms.push(matTop);
	}

	return g_worldTransforms.top();
}

D3DXMATRIX& Peek()
{
	static D3DXMATRIX matIdentity;

	if (g_worldTransforms.empty())
	{
		D3DXMatrixIdentity(&matIdentity);
		return matIdentity;
	}

	return g_worldTransforms.top();
}

D3DXMATRIX Pop()
{
	D3DXMATRIX matTop;

	if (g_worldTransforms.empty())
	{
		D3DXMatrixIdentity(&matTop);
	}
	else
	{
		matTop = g_worldTransforms.top();
		g_worldTransforms.pop();
	}

	return matTop;
}

void Clear()
{
	while (!g_worldTransforms.empty())
	{
		g_worldTransforms.pop();
	}
}

};  // namespace MatrixStack

};  // namespace ShapeShifter
