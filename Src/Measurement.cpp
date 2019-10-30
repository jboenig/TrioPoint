//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Measurement.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"

#include "Measurement.h"

namespace ShapeShifter {

const int NUM_UNITS = 9;

const double g_fDistanceConversionTable[NUM_UNITS][NUM_UNITS] =
{
//     In                    Ft                    Yds             Miles           Mm                    Cm                    M                     Km                     Points
	{ {1.0f},               {12.0f},              {36.0f},        {6.336e4f},     {3.937007874016e-2f}, {3.937007874016e-1f}, {3.937007874016e+1f}, {3.937007874016e+4f},  {1.388888888889e-2f} },  // In
	{ {8.33333e-2f},        {1.0f},               {3.0f},         {5.280e3f},     {3.280839895013e-3f}, {3.280839895013e-2f}, {3.280839895013e+0f}, {3.280839895013e+3f},  {1.157407407407e-3f} },  // Ft
	{ {2.77777e-2f},        {3.333333e-1f},       {1.0f},         {1760.0f},      {1.093613298338e-3f}, {1.093613298338e-2f}, {1.093613298338e+0f}, {1.093613298338e+3f},  {3.858024691358e-4f} },  // Yds
	{ {1.578282828283e-5f}, {1.893939393939e-4f}, {5.68181e-4f},  {1.0f},         {6.213711922373e-7f}, {6.213711922373e-6f}, {6.213711922373e-4f}, {6.213711922373e-1f},  {2.192059483726e-7f} },  // Miles
	{ {2.54e1f},            {3.048e2f},           {9.144e2f},     {1.609344e6f},  {1.0f},               {1e1f},               {1e3f},               {1e6f},                {3.527777777778e-1f} },  // Mm
	{ {2.54f},              {3.048e1f},           {9.144e1f},     {1.609344e5f},  {1e-1f},              {1.0f},               {1e2f},               {1e5f},                {3.527777777778e2f}  },  // Cm
	{ {2.54e-2f},           {3.048e-1f},          {9.144e-1f},    {1.609344e3f},  {1e-3f},              {1e-2f},              {1.0f},               {1e3f},                {3.527777777778e-4f} },  // M
	{ {2.54e-5f},           {3.048e-4f},          {9.144e-4f},    {1.609344f},    {1e-6f},              {1e-5f},              {1e-3f},              {1.0f},                {3.527777777778e-7f} },  // Km
	{ {72.0f},              {864.0f},             {2592.0f},      {4.561920e6f},  {2.834645669291f},    {28.34645669291f},    {2.834645669291e3f},  {2.834645669291e6f},   {1.0f} }                 // Points
};

CMeasurement::CMeasurement(const UnitOfMeasure units, const double val)
{
	m_units = units;
	m_val = val;
}

CMeasurement::CMeasurement(const UnitOfMeasure units, const CMeasurement& src)
{
	m_units = units;
	Convert(src);
}

CMeasurement::CMeasurement(const CMeasurement& src)
{
	m_units = src.GetUnits();
	m_val = (double) src;
}

CMeasurement& CMeasurement::operator=(const CMeasurement& src)
{
	m_units = src.GetUnits();
	m_val = (double) src;
	return *this;
}

BOOL CMeasurement::operator==(const CMeasurement& m) const
{
	double val = (double) m * g_fDistanceConversionTable[m_units][m.GetUnits()];
	return (m_val == val);
}

BOOL CMeasurement::operator!=(const CMeasurement& m) const
{
	double val = (double) m * g_fDistanceConversionTable[m_units][m.GetUnits()];
	return (m_val != val);
}

BOOL CMeasurement::operator<(const CMeasurement& m) const
{
	double val = (double) m * g_fDistanceConversionTable[m_units][m.GetUnits()];
	return (m_val < val);
}

BOOL CMeasurement::operator>(const CMeasurement& m) const
{
	double val = (double) m * g_fDistanceConversionTable[m_units][m.GetUnits()];
	return (m_val > val);
}

BOOL CMeasurement::operator<=(const CMeasurement& m) const
{
	double val = (double) m * g_fDistanceConversionTable[m_units][m.GetUnits()];
	return (m_val <= val);
}

BOOL CMeasurement::operator>=(const CMeasurement& m) const
{
	double val = (double) m * g_fDistanceConversionTable[m_units][m.GetUnits()];
	return (m_val >= val);
}

CMeasurement& CMeasurement::Convert(const CMeasurement& src)
{
	m_val = (double) src * g_fDistanceConversionTable[m_units][src.GetUnits()];
	return *this;
}

CMeasurement& CMeasurement::Convert(const UnitOfMeasure units, const double val)
{
	if (m_units > 0 && units > 0)
	{
		m_val = val * g_fDistanceConversionTable[m_units][units];
	}
	return *this;
}

BOOL CMeasurement::IsMetric(const UnitOfMeasure units)
{
	switch (units)
	{
	case SR_MILLIMETERS:
	case SR_CENTIMETERS:
	case SR_METERS:
	case SR_KILOMETERS:
		return TRUE;
	}
	return FALSE;
}

};  // namespace ShapeShifter
