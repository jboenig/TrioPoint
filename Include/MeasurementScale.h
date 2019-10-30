//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: MeasurementScale.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __MEASUREMENTSCALE_H__
#define __MEASUREMENTSCALE_H__

#pragma once

#include "ShapeShifter.h"
#include "Measurement.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CMeasurementScale

class ATL_NO_VTABLE CMeasurementScale : public CComObjectRootEx<CComSingleThreadModel>,
                                        public CComCoClass<CMeasurementScale, &CLSID_MeasurementScale>,
                                        public IDispatchImpl<IMeasurementScale, &__uuidof(IMeasurementScale), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CMeasurementScale();

DECLARE_REGISTRY_RESOURCEID(IDR_MEASUREMENT_SCALE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CMeasurementScale)
	COM_INTERFACE_ENTRY(IMeasurementScale)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	VALUE3D m_logicalUnits;
	CMeasurement m_measurement;

// Operations
public:
	/////////////////////////////////////////////////////////////////////
	// IMeasurementScale

	STDMETHOD(get_LogicalUnits)(/* [out, retval] */ VALUE3D *pVal);
	STDMETHOD(put_LogicalUnits)(/* [in] */ VALUE3D newVal);

	STDMETHOD(get_UnitOfMeasure)(/* [out, retval] */ UnitOfMeasure *pVal);
	STDMETHOD(put_UnitOfMeasure)(/* [in] */ UnitOfMeasure newVal);

	STDMETHOD(get_Measurement)(/* [out, retval] */ double *pVal);
	STDMETHOD(put_Measurement)(/* [in] */ double newVal);

	STDMETHOD(ConvertMeasurement)(/* [in] */ UnitOfMeasure Units,
	                              /* [in] */ double InputVal,
	                              /* [out,retval] */ VALUE3D* pVal);

	STDMETHOD(ConvertLogicalUnits)(/* [in] */ VALUE3D LogicalUnits,
	                               /* [in] */ UnitOfMeasure Units,
	                               /* [out,retval] */ double* pVal);
};

};  // namespace ShapeShifter

#endif // #ifndef __MEASUREMENTSCALE_H__
