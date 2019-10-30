//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Measurement.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MEASUREMENT_H__
#define __MEASUREMENT_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////////
// CMeasurement
//
class CMeasurement
{
public:
	CMeasurement(const UnitOfMeasure units = SR_INCHES, const double val = 0.0f);
	CMeasurement(const UnitOfMeasure units, const CMeasurement& src);
	CMeasurement(const CMeasurement& src);

	inline operator double() const
	{
		return m_val;
	}

	CMeasurement& operator=(const CMeasurement& src);

	CMeasurement& operator=(const double val)
	{
		m_val = val;
		return *this;
	}

	BOOL operator==(const CMeasurement& m) const;
	BOOL operator!=(const CMeasurement& m) const;
	BOOL operator<(const CMeasurement& m) const;
	BOOL operator>(const CMeasurement& m) const;
	BOOL operator<=(const CMeasurement& m) const;
	BOOL operator>=(const CMeasurement& m) const;

	virtual CMeasurement& Convert(const CMeasurement& src);
	virtual CMeasurement& Convert(const UnitOfMeasure units, const double val);

	inline void SetUnits(const UnitOfMeasure units)
	{
		m_units = units;
	}

	UnitOfMeasure GetUnits() const
	{
		return m_units;
	}

	static BOOL IsMetric(const UnitOfMeasure units);

protected:
	UnitOfMeasure m_units;
	double m_val;
};

};  // namespace ShapeShifter

#endif // #ifndef __MEASUREMENT_H__
