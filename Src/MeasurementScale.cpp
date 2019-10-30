//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: MeasurementScale.cpp
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#include "stdafx.h"
#include <float.h>
#include "MeasurementScale.h"

namespace ShapeShifter {

CMeasurementScale::CMeasurementScale() :
	m_logicalUnits(1.0f),
	m_measurement(SR_INCHES, 1.0f)
{
}

STDMETHODIMP CMeasurementScale::get_LogicalUnits(/* [out, retval] */ VALUE3D *pVal)
{
	if (pVal != NULL)
	{
		*pVal = m_logicalUnits;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CMeasurementScale::put_LogicalUnits(/* [in] */ VALUE3D newVal)
{
	m_logicalUnits = newVal;
	return S_OK;
}

STDMETHODIMP CMeasurementScale::get_UnitOfMeasure(/* [out, retval] */ UnitOfMeasure *pVal)
{
	if (*pVal != NULL)
	{
		*pVal = m_measurement.GetUnits();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CMeasurementScale::put_UnitOfMeasure(/* [in] */ UnitOfMeasure newVal)
{
	m_measurement.SetUnits(newVal);
	return S_OK;
}

STDMETHODIMP CMeasurementScale::get_Measurement(/* [out, retval] */ double *pVal)
{
	if (pVal != NULL)
	{
		*pVal = m_measurement;
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CMeasurementScale::put_Measurement(/* [in] */ double newVal)
{
	m_measurement = newVal;
	return S_OK;
}

STDMETHODIMP CMeasurementScale::ConvertMeasurement(/* [in] */ UnitOfMeasure Units,
                                                   /* [in] */ double InputVal,
                                                   /* [out,retval] */ VALUE3D* pVal)
{
	double res = 0.0f;

	CMeasurement measure(m_measurement.GetUnits());
	measure.Convert(Units, InputVal);
	res = (m_logicalUnits * InputVal) / m_measurement;

	if (pVal != NULL)
	{
		// Make sure result will fit in a float

		if ((res >= FLT_MIN && res <= FLT_MAX) || res == 0)
		{
			*pVal = static_cast<VALUE3D>(res);
			return S_OK;
		}
	}

	return E_FAIL;
}

STDMETHODIMP CMeasurementScale::ConvertLogicalUnits(/* [in] */ VALUE3D LogicalUnits,
                                                    /* [in] */ UnitOfMeasure Units,
                                                    /* [out,retval] */ double* pVal)
{
	CMeasurement res(Units);
	double val = (m_measurement * LogicalUnits) / m_logicalUnits;
	res.Convert(m_measurement.GetUnits(), val);

	if (pVal != NULL)
	{
		*pVal = val;
		return S_OK;
	}

	return E_FAIL;
}

};  // namespace ShapeShifter

