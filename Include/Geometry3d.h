//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Geometry3D.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __GEOMETRY3D_H__
#define __GEOMETRY3D_H__

#pragma once

#if !defined(__midl)
#include "ShapeShifterUtil.h"
#include "VtRecord.h"
#endif

// Macros for defining IDL attributes
#if defined(__midl)
#define SSCL_IDL_STRUCT(guid)    [public,uuid(guid)]
#define SSCL_IDL_PUBLIC_TYPEDEF  [public]
#else
#define SSCL_IDL_STRUCT(guid)
#define SSCL_IDL_PUBLIC_TYPEDEF
#endif

#if !defined(__midl)
namespace ShapeShifter {
#endif

#if !defined(__midl)

template <typename vertex_type>
void CalcNormal(const vertex_type& v1, const vertex_type& v2, const vertex_type& v3, D3DVECTOR& normal)
{
	D3DVECTOR v1_to_v2;
	D3DVECTOR v2_to_v3;

	// Calculate two vectors from the three points
	v1_to_v2.x = v1.x - v2.x;
	v1_to_v2.y = v1.y - v2.y;
	v1_to_v2.z = v1.z - v2.z;

	v2_to_v3.x = v2.x - v3.x;
	v2_to_v3.y = v2.y - v3.y;
	v2_to_v3.z = v2.z - v3.z;

	// Take the cross product of the two vectors to get the
	// normal vector
	normal.x = (v1_to_v2.y * v2_to_v3.z) - (v1_to_v2.z * v2_to_v3.y);
	normal.y = (v1_to_v2.z * v2_to_v3.x) - (v1_to_v2.x * v2_to_v3.z);
	normal.z = (v1_to_v2.x * v2_to_v3.y) - (v1_to_v2.y * v2_to_v3.x);

	NormalizeVector(normal);
}

#endif

/////////////////////////////////////////////////////////////////////////
// VECTOR3D

SSCL_IDL_STRUCT(003BB316-70B4-44eb-9A76-51F3FB6AF72D)
typedef struct VECTOR3D
{
	VALUE3D x;
	VALUE3D y;
	VALUE3D z;

#if !defined(__midl)

	VECTOR3D()
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
	}

	VECTOR3D(const VECTOR3D& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	VECTOR3D(const D3DVECTOR& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	VECTOR3D(const VALUE3D xIn, const VALUE3D yIn, const VALUE3D zIn)
	{
		x = xIn;
		y = yIn;
		z = zIn;
	}

	VECTOR3D& operator=(const VECTOR3D& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

	VECTOR3D& operator=(const D3DVECTOR& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

	operator D3DXVECTOR3() const
	{
		D3DXVECTOR3 v;
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}

#endif  // #if !defined(__midl)

} VECTOR3D;

/////////////////////////////////////////////////////////////////////////
// POINT3D

typedef SSCL_IDL_PUBLIC_TYPEDEF VECTOR3D POINT3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtVECTOR3D

class vtVECTOR3D : public CVtRecord<VECTOR3D>
{
// Constructors/destructor
public:
	vtVECTOR3D()
	{
		m_spRecordInfo = NULL;
		x = 0.0f; y = 0.0f; z = 0.0f;
	}

	vtVECTOR3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtVECTOR3D(const VECTOR3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

	vtVECTOR3D(const D3DVECTOR& v)
	{
		m_spRecordInfo = NULL;
		*this = v;
	}

	vtVECTOR3D(const VALUE3D fx, const VALUE3D fy, const VALUE3D fz)
	{
		m_spRecordInfo = NULL;
		x = fx;
		y = fy;
		z = fz;
	}

// Operations
public:

	vtVECTOR3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		// Copy first point

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtVECTOR3D& operator=(const VECTOR3D& val)
	{
		x = val.x;
		y = val.y;
		z = val.z;
		return *this;
	}

	vtVECTOR3D& operator=(const D3DVECTOR& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtVECTOR3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = x;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x", &varVal);

				varVal.fltVal = y;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y", &varVal);

				varVal.fltVal = z;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtVECTOR3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}

	operator D3DXVECTOR3() const
	{
		D3DXVECTOR3 v;
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}
};

typedef vtVECTOR3D vtPOINT3D;

#endif  // #if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// LINE3D

SSCL_IDL_STRUCT(505500BF-BB2C-463c-B1A5-1D9EB92C4866)
typedef struct LINE3D
{
	VALUE3D x1;
	VALUE3D y1;
	VALUE3D z1;
	VALUE3D x2;
	VALUE3D y2;
	VALUE3D z2;

#if !defined(__midl)

	LINE3D()
	{
	}

	LINE3D(const LINE3D& src)
	{
		x1 = src.x1;
		y1 = src.y1;
		z1 = src.z1;
		x2 = src.x2;
		y2 = src.y2;
		z2 = src.z2;
	}

	LINE3D(const D3DVECTOR& point1, const D3DVECTOR& point2)
	{
		x1 = point1.x;
		y1 = point1.y;
		z1 = point1.z;
		x2 = point2.x;
		y2 = point2.y;
		z2 = point2.z;
	}

	LINE3D& operator=(const LINE3D& src)
	{
		x1 = src.x1;
		y1 = src.y1;
		z1 = src.z1;
		x2 = src.x2;
		y2 = src.y2;
		z2 = src.z2;
		return *this;
	}

#endif  // #if !defined(__midl)

} LINE3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtLINE3D

class vtLINE3D : public CVtRecord<LINE3D>
{
// Constructors/destructor
public:
	vtLINE3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtLINE3D(const LINE3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

	vtLINE3D(const D3DVECTOR& point1, const D3DVECTOR& point2)
	{
		m_spRecordInfo = NULL;
		x1 = point1.x;
		y1 = point1.y;
		z1 = point1.z;
		x2 = point2.x;
		y2 = point2.y;
		z2 = point2.z;
	}

// Operations
public:

	vtLINE3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		// Copy first point

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x1", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x1 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y1", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y1 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z1", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z1 = *(varVal.pfltVal);

		// Copy second point

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x2", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x2 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y2", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y2 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z2", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z2 = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtLINE3D& operator=(const LINE3D& val)
	{
		x1 = val.x1;
		y1 = val.y1;
		z1 = val.z1;
		x2 = val.x2;
		y2 = val.y2;
		z2 = val.z2;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtLINE3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = x1;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x1", &varVal);

				varVal.fltVal = y1;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y1", &varVal);

				varVal.fltVal = z1;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z1", &varVal);

				varVal.fltVal = x2;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x2", &varVal);

				varVal.fltVal = y2;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y2", &varVal);

				varVal.fltVal = z2;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z2", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtLINE3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}
};

#endif  // #if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// TRIANGLE3D

SSCL_IDL_STRUCT(1FBA3290-C357-441c-A371-893CEFCDC582)
typedef struct TRIANGLE3D
{
	VALUE3D x1;
	VALUE3D y1;
	VALUE3D z1;
	VALUE3D x2;
	VALUE3D y2;
	VALUE3D z2;
	VALUE3D x3;
	VALUE3D y3;
	VALUE3D z3;
} TRIANGLE3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtTRIANGLE3D

class vtTRIANGLE3D : public CVtRecord<TRIANGLE3D>
{
// Constructors/destructor
public:
	vtTRIANGLE3D()
	{
		m_spRecordInfo = NULL;
	}

	vtTRIANGLE3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtTRIANGLE3D(const TRIANGLE3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

	vtTRIANGLE3D::vtTRIANGLE3D(const POINT3D& ptIn1,
	                           const POINT3D& ptIn2,
	                           const POINT3D& ptIn3)
	{
		m_spRecordInfo = NULL;
		x1 = ptIn1.x;
		y1 = ptIn1.y;
		z1 = ptIn1.z;
		x2 = ptIn2.x;
		y2 = ptIn2.y;
		z2 = ptIn2.z;
		x3 = ptIn3.x;
		y3 = ptIn3.y;
		z3 = ptIn3.z;
	}

// Operations
public:

	vtTRIANGLE3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		// Copy first point

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x1", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x1 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y1", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y1 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z1", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z1 = *(varVal.pfltVal);

		// Copy second point

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x2", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x2 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y2", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y2 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z2", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z2 = *(varVal.pfltVal);

		// Copy third point

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x3", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x3 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y3", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y3 = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z3", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z3 = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtTRIANGLE3D& operator=(const TRIANGLE3D& val)
	{
		x1 = val.x1;
		y1 = val.y1;
		z1 = val.z1;
		x2 = val.x2;
		y2 = val.y2;
		z2 = val.z2;
		x3 = val.x3;
		y3 = val.y3;
		z3 = val.z3;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtTRIANGLE3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = x1;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x1", &varVal);

				varVal.fltVal = y1;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y1", &varVal);

				varVal.fltVal = z1;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z1", &varVal);

				varVal.fltVal = x2;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x2", &varVal);

				varVal.fltVal = y2;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y2", &varVal);

				varVal.fltVal = z2;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z2", &varVal);

				varVal.fltVal = x3;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x3", &varVal);

				varVal.fltVal = y3;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y3", &varVal);

				varVal.fltVal = z3;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z3", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtTRIANGLE3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}
};

#endif  // #if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// QUAD3D

SSCL_IDL_STRUCT(3C1920CC-89FC-49b2-B4AB-EF2D0E1E0976)
typedef struct QUAD3D
{
	POINT3D topLeft;
	POINT3D topRight;
	POINT3D bottomLeft;
	POINT3D bottomRight;
} QUAD3D;

/////////////////////////////////////////////////////////////////////////
// BOX3D

SSCL_IDL_STRUCT(C4B804E3-ABB4-401b-8FA1-46452B76B2F4)
typedef struct BOX3D
{
	VALUE3D left;
	VALUE3D right;
	VALUE3D top;
	VALUE3D bottom;
	VALUE3D front;
	VALUE3D back;

#if !defined(__midl)

	BOX3D()
	{
		left = 0.0f;
		right = 0.0f;
		top = 0.0f;
		bottom = 0.0f;
		front = 0.0f;
		back = 0.0f;
	}

	BOX3D(VALUE3D leftIn, VALUE3D rightIn,
	      VALUE3D topIn, VALUE3D bottomIn,
	      VALUE3D frontIn, VALUE3D backIn)
	{
		left = leftIn;
		right = rightIn;
		top = topIn;
		bottom = bottomIn;
		front = frontIn;
		back = backIn;
	}

	BOX3D(const BOX3D& src)
	{
		*this = src;
	}

	BOX3D& operator=(const BOX3D& src)
	{
		left = src.left;
		right = src.right;
		top = src.top;
		bottom = src.bottom;
		front = src.front;
		back = src.back;
		return *this;
	}

	bool IsEmpty() const
	{
		D3DXVECTOR3 boxSize;
		GetSize(boxSize);
		return ((boxSize.x <= EPSILON) &&
		        (boxSize.y <= EPSILON) &&
		        (boxSize.z <= EPSILON));
	}

	BOX3D Union(const BOX3D& src)
	{
		BOX3D resBox;

		D3DXVECTOR3 boxSize;
		GetSize(boxSize);

		if (boxSize.x  <= EPSILON)
		{
			resBox.left = src.left;
			resBox.right = src.right;
		}
		else
		{
			if (src.left < left)
			{
				resBox.left = src.left;
			}
			else
			{
				resBox.left = left;
			}

			if (src.right > right)
			{
				resBox.right = src.right;
			}
			else
			{
				resBox.right = right;
			}
		}

		if (boxSize.y <= EPSILON)
		{
			resBox.top = src.top;
			resBox.bottom = src.bottom;
		}
		else
		{
			if (src.top > top)
			{
				resBox.top = src.top;
			}
			else
			{
				resBox.top = top;
			}

			if (src.bottom < bottom)
			{
				resBox.bottom = src.bottom;
			}
			else
			{
				resBox.bottom = bottom;
			}
		}

		if (boxSize.z <= EPSILON)
		{
			resBox.back = src.back;
			resBox.front = src.front;
		}
		else
		{
			if (src.back > back)
			{
				resBox.back = src.back;
			}
			else
			{
				resBox.back = back;
			}

			if (src.front < front)
			{
				resBox.front = src.front;
			}
			else
			{
				resBox.front = front;
			}
		}

		return resBox;
	}

	void Normalize()
	{
		D3DXVECTOR3 minPoint;
		D3DXVECTOR3 maxPoint;
		GetMinPoint(minPoint);
		GetMaxPoint(maxPoint);
		SetMinMax(minPoint, maxPoint);
	}

	bool ContainsPoint(const D3DXVECTOR3& pt) const
	{
		D3DXVECTOR3 minPt;
		D3DXVECTOR3 maxPt;
		GetMinPoint(minPt);
		GetMaxPoint(maxPt);

		return ((pt.x >= minPt.x) && (pt.x <= maxPt.x) &&
				(pt.y >= minPt.y) && (pt.y <= maxPt.y) &&
				(pt.z >= minPt.z) && (pt.z <= maxPt.z));
	}

	bool HitTestRay(const POINT3D& ptRayOrigin, const VECTOR3D& vRayDir) const
	{
		D3DXVECTOR3 minPoint;
		GetMinPoint(minPoint);
		D3DXVECTOR3 maxPoint;
		GetMaxPoint(maxPoint);
		D3DXVECTOR3 rayOriginVect = ptRayOrigin;
		D3DXVECTOR3 rayDirVect = vRayDir;
		if (D3DXBoxBoundProbe(&minPoint, &maxPoint, &rayOriginVect, &rayDirVect))
			return true;
		return false;
	}

	bool HitTestBox(const BOX3D& src) const
	{
		BOX3D boxIntersect = Intersect(src);
		return boxIntersect.IsEmpty();
	}

	BOX3D Intersect(const BOX3D& src) const
	{
		BOX3D resBox;

		if ((right - src.left) > 0 && (src.right - left) > 0)
		{
			if (left > src.left)
			{
				resBox.left = left;
			}
			else
			{
				resBox.left = src.left;
			}

			if (right < src.right)
			{
				resBox.right = right;
			}
			else
			{
				resBox.right = src.right;
			}
		}

		if ((top - src.bottom) > 0 && (src.top - bottom) > 0)
		{
			if (bottom > src.bottom)
			{
				resBox.bottom = bottom;
			}
			else
			{
				resBox.bottom = src.bottom;
			}

			if (top < src.top)
			{
				resBox.top = top;
			}
			else
			{
				resBox.top = src.top;
			}
		}

		if ((back - src.front) > 0 && (src.back - front) > 0)
		{
			if (front > src.front)
			{
				resBox.front = front;
			}
			else
			{
				resBox.front = src.front;
			}

			if (back < src.back)
			{
				resBox.back = back;
			}
			else
			{
				resBox.back = src.back;
			}
		}

		return resBox;
	}

	void CalculateBounds(D3DXVECTOR3* pPoints,
	                     VertexCount nNumPoints)
	{
		D3DXVECTOR3 minPoint;
		D3DXVECTOR3 maxPoint;

		minPoint.x = FLT_MAX;
		minPoint.y = FLT_MAX;
		minPoint.z = FLT_MAX;
		maxPoint.x = -FLT_MAX;
		maxPoint.y = -FLT_MAX;
		maxPoint.z = -FLT_MAX;

		for (VertexIndex i = 0; i < nNumPoints; i++)
		{
			if (pPoints[i].x < minPoint.x)
			{
				minPoint.x = pPoints[i].x;
			}
			if (pPoints[i].x > maxPoint.x)
			{
				maxPoint.x = pPoints[i].x;
			}

			if (pPoints[i].y < minPoint.y)
			{
				minPoint.y = pPoints[i].y;
			}
			if (pPoints[i].y > maxPoint.y)
			{
				maxPoint.y = pPoints[i].y;
			}

			if (pPoints[i].z < minPoint.z)
			{
				minPoint.z = pPoints[i].z;
			}
			if (pPoints[i].z > maxPoint.z)
			{
				maxPoint.z = pPoints[i].z;
			}
		}

		left = minPoint.x;
		top = maxPoint.y;
		front = minPoint.z;
		right = maxPoint.x;
		bottom = minPoint.y;
		back = maxPoint.z;
	}

	template <typename vector_type>
	void GetSize(vector_type& sz) const
	{
		sz.x = right - left;
		sz.y = top - bottom;
		sz.z = back - front;
	}

	template <typename vertex_type>
	void SetMinMax(const vertex_type& minPoint, const vertex_type& maxPoint)
	{
		left = minPoint.x;
		top = maxPoint.y;
		front = minPoint.z;
		right = maxPoint.x;
		bottom = minPoint.y;
		back = maxPoint.z;
	}

	template <typename vertex_type>
	void GetCenter(vertex_type& center) const
	{
		center.x = (left + right) / 2;
		center.y = (top + bottom) / 2;
		center.z = (front + back) / 2;
	}

	template <typename vertex_type>
	void GetMinPoint(vertex_type& minPoint) const
	{
		D3DVECTOR boxSize;
		GetSize(boxSize);
		float fOffsetX = (boxSize.x < 0.0f) ? (boxSize.x) : (0.0f);
		float fOffsetY = (boxSize.y < 0.0f) ? (boxSize.y) : (0.0f);
		float fOffsetZ = (boxSize.z < 0.0f) ? (boxSize.z) : (0.0f);
		minPoint.x = left + fOffsetX;
		minPoint.y = bottom + fOffsetY;
		minPoint.z = front + fOffsetZ;
	}

	template <typename vertex_type>
	void GetMaxPoint(vertex_type& maxPoint) const
	{
		D3DVECTOR boxSize;
		GetSize(boxSize);
		float fOffsetX = (boxSize.x > 0.0f) ? (boxSize.x) : (0.0f);
		float fOffsetY = (boxSize.y > 0.0f) ? (boxSize.y) : (0.0f);
		float fOffsetZ = (boxSize.z > 0.0f) ? (boxSize.z) : (0.0f);
		maxPoint.x = left + fOffsetX;
		maxPoint.y = bottom + fOffsetY;
		maxPoint.z = front + fOffsetZ;
	}

	template <typename vertex_type>
	void GetPoints(vertex_type* pVertexArray) const
	{
		vertex_type minPoint;
		vertex_type maxPoint;
		GetMinPoint(minPoint);
		GetMaxPoint(maxPoint);
		pVertexArray[0].x = pVertexArray[1].x = pVertexArray[6].x = pVertexArray[7].x = minPoint.x;
		pVertexArray[2].x = pVertexArray[3].x = pVertexArray[4].x = pVertexArray[5].x = maxPoint.x;
		pVertexArray[0].y = pVertexArray[2].y = pVertexArray[4].y = pVertexArray[6].y = minPoint.y;
		pVertexArray[1].y = pVertexArray[3].y = pVertexArray[5].y = pVertexArray[7].y = maxPoint.y;
		pVertexArray[0].z = pVertexArray[1].z = pVertexArray[2].z = pVertexArray[3].z = minPoint.z;
		pVertexArray[4].z = pVertexArray[5].z = pVertexArray[6].z = pVertexArray[7].z = maxPoint.z;
	}

#endif  // #if !defined(__midl)

} BOX3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtBOX3D

class vtBOX3D : public CVtRecord<BOX3D>
{
// Constructors/destructor
public:
	vtBOX3D()
	{
		m_spRecordInfo = NULL;
	}

	vtBOX3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtBOX3D(const BOX3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

// Operations
public:

	vtBOX3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"left", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		left = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"right", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		right = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"top", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		top = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"bottom", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		bottom = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"front", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		front = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"back", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		back = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtBOX3D& operator=(const BOX3D& val)
	{
		left = val.left;
		right = val.right;
		top = val.top;
		bottom = val.bottom;
		front = val.front;
		back = val.back;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtBOX3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = left;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"left", &varVal);

				varVal.fltVal = right;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"right", &varVal);

				varVal.fltVal = top;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"top", &varVal);

				varVal.fltVal = bottom;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"bottom", &varVal);

				varVal.fltVal = front;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"front", &varVal);

				varVal.fltVal = back;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"back", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtBOX3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}
};

#endif  // #if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// SPHERE3D

SSCL_IDL_STRUCT(652A0023-07A2-4aa8-B703-B18FE4C7F224)
typedef struct SPHERE3D
{
	VALUE3D x;
	VALUE3D y;
	VALUE3D z;
	VALUE3D radius;

#if !defined(__midl)
	SPHERE3D()
	{
		x = y = z = 0.0f;
		radius = 0.0f;
	}

	SPHERE3D(const SPHERE3D& src)
	{
		*this = src;
	}

	SPHERE3D& operator=(const SPHERE3D& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		radius = src.radius;
		return *this;
	}

	D3DXVECTOR3 Origin() const
	{
		return D3DXVECTOR3(x, y, z);
	}

	void CalculateBounds(D3DXVECTOR3* pPoints,
	                     VertexCount nNumPoints)
	{
		D3DXVECTOR3 originVect;

		HRESULT hr = D3DXComputeBoundingSphere(pPoints,
		                                       (DWORD) nNumPoints,
		                                       D3DFVF_XYZ,
		                                       &originVect,
		                                       &radius);

		if (hr == D3D_OK)
		{
			x = originVect.x;
			y = originVect.y;
			z = originVect.z;
		}
	}

	SPHERE3D Union(const SPHERE3D& src)
	{
		SPHERE3D resSphere;

		if (IsEqualFloat(radius, 0.0f))
		{
			// This sphere is empty. Assign source sphere
			// to this sphere.
			resSphere.x = src.x;
			resSphere.y = src.y;
			resSphere.z = src.z;
			resSphere.radius = src.radius;
		}
		else if (src.radius > 0.0f)
		{
			D3DXVECTOR3 originVect(x, y, z);
			D3DXVECTOR3 srcOriginVect(src.x, src.y, src.z);

			D3DXVECTOR3 v1(originVect - srcOriginVect);
			D3DXVec3Normalize(&v1, &v1);
			v1 = v1 * radius;
			D3DXVECTOR3 pt1;
			D3DXVec3Add(&pt1, &originVect, &v1);

			D3DXVECTOR3 v2(srcOriginVect - originVect);
			D3DXVec3Normalize(&v2, &v2);
			v2 = v2 * src.radius;
			D3DXVECTOR3 pt2;
			D3DXVec3Add(&pt2, &srcOriginVect, &v2);

			D3DXVECTOR3 unionVect(pt2 - pt1);
			D3DXVECTOR3 radiusVect = unionVect / 2;
			resSphere.radius = D3DXVec3Length(&radiusVect);
			D3DXVECTOR3 resOrigin;
			D3DXVec3Add(&resOrigin, &pt1, &radiusVect);
			resSphere.x = resOrigin.x;
			resSphere.y = resOrigin.y;
			resSphere.z = resOrigin.z;
		}

		return resSphere;
	}

	bool ContainsPoint(const D3DXVECTOR3& pt) const
	{
		D3DXVECTOR3 v = D3DXVECTOR3(x, y, z) - pt;

		if (D3DXVec3LengthSq(&v) > (radius * radius))
		{
			return false;
		}

		return true;
	}

	bool HitTestRay(POINT3D& ptRayOrigin, VECTOR3D& vRayDir) const
	{
		D3DXVECTOR3 originVect(x, y, z);
		D3DXVECTOR3 rayOriginVect = ptRayOrigin;
		D3DXVECTOR3 rayDirVect = vRayDir;
		if (D3DXSphereBoundProbe(&originVect, radius, &rayOriginVect, &rayDirVect))
			return true;
		return false;
	}

	bool HitTestLineSeg(POINT3D& pt1, POINT3D& pt2) const
	{
		D3DXVECTOR3 originVect(x, y, z);
		D3DXVECTOR3 ray1Origin = pt1;
		D3DXVECTOR3 ray2Origin = pt2;
		D3DXVECTOR3 ray1Dir = ray2Origin - ray1Origin;
		D3DXVECTOR3 ray2Dir = ray1Origin - ray2Origin;
		return (D3DXSphereBoundProbe(&originVect, radius, &ray1Origin, &ray1Dir) &&
		        D3DXSphereBoundProbe(&originVect, radius, &ray2Origin, &ray2Dir));
	}

	bool HitTestSphere(const SPHERE3D& sphere2) const
	{
		D3DXVECTOR3 v = D3DXVECTOR3(sphere2.x, sphere2.y, sphere2.z) - D3DXVECTOR3(x, y, z);
		float centDist = radius + sphere2.radius;
		return (D3DXVec3LengthSq(&v) <= centDist * centDist);
	}

	bool HitTestBox(const BOX3D& box) const
	{
		VALUE3D minDist = 0.0f;
		D3DXVECTOR3 boxMin;
		D3DXVECTOR3 boxMax;
		VALUE3D xDist = 0.0f;
		VALUE3D yDist = 0.0f;
		VALUE3D zDist = 0.0f;

		box.GetMinPoint(boxMin);
		box.GetMaxPoint(boxMax);

		if (x < boxMin.x)
		{
			xDist = x - boxMin.x;
			minDist += (xDist * xDist);
		}
		else if (x > boxMax.x)
		{
			xDist = x - boxMax.x;
			minDist += (xDist * xDist);
		}

		if (y < boxMin.y)
		{
			yDist = y - boxMin.y;
			minDist += (yDist * yDist);
		}
		else if (y > boxMax.y)
		{
			yDist = y - boxMax.y;
			minDist += (yDist * yDist);
		}

		if (z < boxMin.z)
		{
			zDist = z - boxMin.z;
			minDist += (zDist * zDist);
		}
		else if (z > boxMax.z)
		{
			zDist = z - boxMax.z;
			minDist += (zDist * zDist);
		}

		return (minDist <= (radius * radius));
	}

	VALUE3D IntersectDist(const SPHERE3D& sphere2) const
	{
		D3DXVECTOR3 v = D3DXVECTOR3(sphere2.x, sphere2.y, sphere2.z) - D3DXVECTOR3(x, y, z);
		return (D3DXVec3Length(&v) - (radius + sphere2.radius));
	}

	template <typename vertex_type>
	bool HitTestTriangleList(vertex_type* pTriangleList, const UINT nNumTriangles) const
	{
		bool bHit = false;

		for (UINT nTriangleIdx = 0; !bHit && nTriangleIdx < nNumTriangles; nTriangleIdx++)
		{
			UINT nVert1 = (nTriangleIdx * 3);
			UINT nVert2 = nVert1 + 1;
			UINT nVert3 = nVert1 + 2;

			POINT3D pt1;
			pt1.x = pTriangleList[nVert1].x;
			pt1.y = pTriangleList[nVert1].y;
			pt1.z = pTriangleList[nVert1].z;

			POINT3D pt2;
			pt2.x = pTriangleList[nVert2].x;
			pt2.y = pTriangleList[nVert2].y;
			pt2.z = pTriangleList[nVert2].z;

			POINT3D pt3;
			pt3.x = pTriangleList[nVert3].x;
			pt3.y = pTriangleList[nVert3].y;
			pt3.z = pTriangleList[nVert3].z;

			bHit = HitTestLineSeg(pt1, pt2) ||
			       HitTestLineSeg(pt2, pt3) ||
			       HitTestLineSeg(pt1, pt3);
		}

		return bHit;
	}

#endif  // #if !defined(__midl)

} SPHERE3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtSPHERE3D

class vtSPHERE3D : public CVtRecord<SPHERE3D>
{
// Constructors/destructor
public:
	vtSPHERE3D()
	{
		m_spRecordInfo = NULL;
	}

	vtSPHERE3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtSPHERE3D(const SPHERE3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

// Operations
public:

	vtSPHERE3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"x", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		x = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"y", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		y = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"z", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		z = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"radius", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		radius = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtSPHERE3D& operator=(const SPHERE3D& val)
	{
		x = val.x;
		y = val.y;
		z = val.z;
		radius = val.radius;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtSPHERE3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = x;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"x", &varVal);

				varVal.fltVal = y;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"y", &varVal);

				varVal.fltVal = z;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"z", &varVal);

				varVal.fltVal = radius;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"radius", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtSPHERE3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}
};

#endif  // #if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// PLANE3D

enum RelativePositionToPlane
{
	COINCIDENT,
	IN_BACK_OF,
	IN_FRONT_OF,
	SPANNING
};

SSCL_IDL_STRUCT(F868136B-133A-42ea-A170-951603DAE9B0)
typedef struct PLANE3D
{
	VALUE3D a;
	VALUE3D b;
	VALUE3D c;
	VALUE3D d;

#if !defined(__midl)

	PLANE3D()
	{
		a = 0.0f;
		b = 0.0f;
		c = 0.0f;
		d = 0.0f;
	}

	PLANE3D(const D3DXPLANE& src)
	{
		*this = src;
	}

	PLANE3D& operator=(const D3DXPLANE& src)
	{
		a = src.a;
		b = src.b;
		c = src.c;
		d = src.d;
		return *this;
	}

	operator D3DXPLANE() const
	{
		return D3DXPLANE(a,b,c,d);
	}

	void FromPointNormal(const D3DXVECTOR3& v, const D3DXVECTOR3& n)
	{
		D3DXPLANE p;
		D3DXVECTOR3 vBuf(v);
		D3DXVECTOR3 nBuf(n);
		D3DXPlaneFromPointNormal(&p, &vBuf, &nBuf);
		*this = p;
	}

	template <typename vertex_type>
	void FromPoints(const vertex_type& pt1, const vertex_type& pt2, const vertex_type& pt3)
	{
		// Cross product between the two vectors made up by the
		// three points generates the normal for the plane
		D3DXVECTOR3 normal;
		normal.x = pt1.y * pt2.z - pt1.z * pt2.y;
		normal.y = pt1.z * pt2.x - pt1.x * pt2.z;
		normal.z = pt1.x * pt2.y - pt1.y * pt2.x;
		D3DXVec3Normalize(&normal, &normal);

		a = normal.x;
		b = normal.y;
		c = normal.z;

		// d is the distance from the plane to the origin
		// along the normal vector. It is calculated by taking
		// the dot product of the normal vector and any of the
		// three points and then making it negative

		d = -(normal.x * pt1.x + normal.y * pt1.y + normal.z * pt1.z);
	}

	D3DXVECTOR3 NormalVector() const
	{
		D3DXPLANE plane(a, b, c, d);
		D3DXPlaneNormalize(&plane, &plane);
		return D3DXVECTOR3(a, b, c);
	}

	bool TestLineIntersection(const D3DXVECTOR3& pt1, const D3DXVECTOR3& pt2) const
	{
		D3DXVECTOR3 ptIntersect;
		D3DXPLANE p = *this;
		D3DXVECTOR3 pt1Buf(pt1);
		D3DXVECTOR3 pt2Buf(pt2);

		if (D3DXPlaneIntersectLine(&ptIntersect, &p, &pt1Buf, &pt2Buf) != NULL)
		{
			return true;
		}

		return false;
	}

	bool GetLineIntersection(D3DXVECTOR3& ptIntersect, const D3DXVECTOR3& pt1, const D3DXVECTOR3& pt2) const
	{
		D3DXPLANE p = *this;
		D3DXVECTOR3 pt1Buf(pt1);
		D3DXVECTOR3 pt2Buf(pt2);

		if (D3DXPlaneIntersectLine(&ptIntersect, &p, &pt1Buf, &pt2Buf) != NULL)
		{
			return true;
		}

		return false;
	}

	template <typename vertex_type>
	RelativePositionToPlane TestPoints(vertex_type* pPtArray, long nNumPoints) const
	{
		RelativePositionToPlane relPos = COINCIDENT;
		RelativePositionToPlane curRelPos;
		FLOAT fPtVal;

		for (long nPtIdx = 0; nPtIdx < nNumPoints && relPos != SPANNING; nPtIdx++)
		{
			fPtVal = (a * pPtArray[nPtIdx].x) + (b * pPtArray[nPtIdx].y) + (c * pPtArray[nPtIdx].z) + d;

			if (fPtVal < 0.0f)
			{
				curRelPos = IN_BACK_OF;
			}
			else if (fPtVal > 0.0f)
			{
				curRelPos = IN_FRONT_OF;
			}
			else
			{
				curRelPos = COINCIDENT;
			}

			if (relPos != curRelPos)
			{
				if (relPos == COINCIDENT)
				{
					relPos = curRelPos;
				}
				else
				{
					relPos = SPANNING;
				}
			}
		}

		return relPos;
	}

	RelativePositionToPlane TestSphere(const SPHERE3D& sphere) const
	{
		D3DXVECTOR3 normalVector = NormalVector();
		D3DXVECTOR3 radiusVector1 = normalVector * sphere.radius;
		D3DXVECTOR3 radiusVector2 = normalVector * (-sphere.radius);
		D3DXVECTOR3 pts[3];

		D3DXVECTOR3 sphereOrigin = sphere.Origin();

		pts[0] = sphereOrigin + radiusVector1;
		pts[1] = sphereOrigin;
		pts[2] = sphereOrigin + radiusVector2;

		return TestPoints(pts, 3);
	}

	RelativePositionToPlane TestBox(const BOX3D& box) const
	{
		D3DXVECTOR3 boxPts[8];
		box.GetPoints(boxPts);
		return TestPoints(boxPts, 8);
	}

#endif  // #if !defined(__midl)

} PLANE3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtPLANE3D

class vtPLANE3D : public CVtRecord<PLANE3D>
{
// Constructors/destructor
public:
	vtPLANE3D()
	{
		m_spRecordInfo = NULL;
	}

	vtPLANE3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtPLANE3D(const PLANE3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

// Operations
public:

	vtPLANE3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"a", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		a = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"b", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		b = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"c", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		c = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"d", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		d = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtPLANE3D& operator=(const PLANE3D& val)
	{
		a = val.a;
		b = val.b;
		c = val.c;
		d = val.d;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtPLANE3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = a;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"a", &varVal);

				varVal.fltVal = b;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"b", &varVal);

				varVal.fltVal = c;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"c", &varVal);

				varVal.fltVal = d;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"d", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtPLANE3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}
};

#endif  // #if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// TEXTURECOORDINATE3D

SSCL_IDL_STRUCT(2D2F3B0F-9195-4bf1-9D52-B8D2F559D6AB)
typedef struct TEXTURECOORDINATE3D
{
	VALUE3D tu;
	VALUE3D tv;
} TEXTURECOORDINATE3D;

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// vtTEXTURECOORDINATE3D

class vtTEXTURECOORDINATE3D : public CVtRecord<TEXTURECOORDINATE3D>
{
// Constructors/destructor
public:
	vtTEXTURECOORDINATE3D()
	{
		m_spRecordInfo = NULL;
	}

	vtTEXTURECOORDINATE3D(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

	vtTEXTURECOORDINATE3D(const TEXTURECOORDINATE3D& val)
    {
		m_spRecordInfo = NULL;
		*this = val;
    }

// Operations
public:

	vtTEXTURECOORDINATE3D& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

#if 0
		// This block of code performs the copy one field at a time instead of
		// doing a block copy on the whole structure. It is probably slower but
		// also possibly safer. It's been left in the code in the event that
		// structure alignment or padding causes problems with the block copy.
		VARIANT varVal;
		varVal.vt = VT_R4;
		VALUE3D fVal;

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"tu", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		tu = *(varVal.pfltVal);

		if (FAILED(var.pRecInfo->GetFieldNoCopy(var.pvRecord, L"tv", &varVal, (LPVOID*) &fVal)))
		{
			throw new ECopyError();
		}
		tv = *(varVal.pfltVal);
#else
		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}
#endif

		return *this;
	}

	vtTEXTURECOORDINATE3D& operator=(const TEXTURECOORDINATE3D& val)
	{
		tu = val.tu;
		tv = val.tv;
		return *this;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< vtTEXTURECOORDINATE3D* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
#if 0
			// This block of code performs the copy one field at a time instead of
			// doing a block copy on the whole structure. It is probably slower but
			// also possibly safer. It's been left in the code in the event that
			// structure alignment or padding causes problems with the block copy.
			var.pvRecord = var.pRecInfo->RecordCreate();

			if (var.pvRecord != NULL)
			{
				VARIANT varVal;
				varVal.vt = VT_R4;

				varVal.fltVal = tu;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"tu", &varVal);

				varVal.fltVal = tv;
				var.pRecInfo->PutField(INVOKE_PROPERTYPUT, var.pvRecord, L"tv", &varVal);
			}
#else
			var.pRecInfo->RecordCreateCopy(const_cast< vtTEXTURECOORDINATE3D* >(this), &var.pvRecord);
#endif
		}

		return var.Detach();
	}
};

#endif  // #if !defined(__midl)

#if !defined(__midl)

/////////////////////////////////////////////////////////////////////////
// Vector functions

template <typename vector_type>
VALUE3D VectorMagnitude(const vector_type& v)
{
	return static_cast<VALUE3D>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

template <typename vector_type>
void NormalizeVector(vector_type& v)
{
	VALUE3D len = static_cast<VALUE3D>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	if (len == 0.0f)
		return;
	v.x = v.x / len;
	v.y = v.y / len;
	v.z = v.z / len;
}

template <typename vector_type>
void AssignD3DVECTOR(D3DVECTOR& d3dv, const vector_type& v)
{
	d3dv.x = v.x;
	d3dv.y = v.y;
	d3dv.z = v.z;
}

template <typename vector_type>
inline D3DXVECTOR3 D3DXVECTOR3_cast(const vector_type& v)
{
	D3DXVECTOR3 d3dv;
	d3dv.x = v.x;
	d3dv.y = v.y;
	d3dv.z = v.z;
	return d3dv;
}

template <typename vector_type>
VALUE3D VectorDotProduct(const vector_type& v1, const vector_type& v2)
{
   return v1.x*v2.x + v1.y * v2.y + v1.z*v2.z;
}

template <typename vector_type>
void VectorCrossProduct(const vector_type& v1, const vector_type& v2, vector_type& result)
{
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
}

#endif  // if !defined(__midl)

#if !defined(__midl)
};  // namespace ShapeShifter
#endif

#endif // #ifndef __GEOMETRY3D_H__
