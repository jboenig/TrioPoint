//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Vertex3d.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __VERTEX3D_H__
#define __VERTEX3D_H__

#pragma once

#include "Geometry3d.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// LVERTEX3D

typedef struct _LVERTEX3D
{
    VALUE3D     x;             /* Homogeneous coordinates */
    VALUE3D     y;
    VALUE3D     z;
    D3DCOLOR    color;         /* Vertex color */
    D3DCOLOR    specular;      /* Specular component of vertex */
    VALUE3D     tu;            /* Texture coordinates */
    VALUE3D     tv;

    _LVERTEX3D() { }
    _LVERTEX3D(const VECTOR3D& v,
                D3DCOLOR _color, D3DCOLOR _specular,
                float _tu, float _tv)
        { x = v.x; y = v.y; z = v.z;
          color = _color; specular = _specular;
          tu = _tu; tv = _tv;
        }
} LVERTEX3D, *LPLVERTEX3D;

#define D3DFVF_LVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | \
                         D3DFVF_SPECULAR | D3DFVF_TEX1 )

/////////////////////////////////////////////////////////////////////////
// VERTEX3D

typedef struct _VERTEX3D
{
    VALUE3D     x;             /* Homogeneous coordinates */
    VALUE3D     y;
    VALUE3D     z;
    VALUE3D     nx;            /* Normal */
    VALUE3D     ny;
    VALUE3D     nz;
    VALUE3D     tu;            /* Texture coordinates */
    VALUE3D     tv;

    _VERTEX3D() { }
    _VERTEX3D(const VECTOR3D& v, const VECTOR3D& n, float _tu, float _tv)
        { x = v.x; y = v.y; z = v.z;
          nx = n.x; ny = n.y; nz = n.z;
          tu = _tu; tv = _tv;
        }
} VERTEX3D, *LPVERTEX3D;

#define D3DFVF_VERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

};  // namespace ShapeShifter

#endif // #ifndef __VERTEX3D_H__
