//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: VtRecord.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __VTRECORD_H__
#define __VTRECORD_H__

#pragma once

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CVtRecord

template <typename T>
class CVtRecord : public T
{
// Exceptions
public:
	struct ETypeMismatch {};
	struct ECopyError {};

// Constructors/destructor
public:
	CVtRecord()
	{
		m_spRecordInfo = NULL;
	}

	CVtRecord(const VARIANT& var)
    {
		m_spRecordInfo = NULL;
		*this = var;
    }

// Attributes
public:
	static CComTypeInfoHolder _tih;
	IRecordInfoPtr m_spRecordInfo;

// Operations
public:

	IRecordInfoPtr GetRecordInfo()
	{
		if (m_spRecordInfo == NULL)
		{
			ITypeInfo* pTypeInfo = NULL;
			HRESULT hr = _tih.GetTI(0, &pTypeInfo);
			if (hr == S_OK)
			{
				hr = ::GetRecordInfoFromTypeInfo(pTypeInfo, &m_spRecordInfo);
				pTypeInfo->Release();
			}
		}

		return m_spRecordInfo;
	}

	CVtRecord<T>& operator=(const VARIANT& var)
	{
		if (!TypeMatches(var))
		{
			throw new ETypeMismatch();
		}

		if (FAILED(var.pRecInfo->RecordCopy(var.pvRecord, this)))
		{
			throw new ECopyError();
		}

		return *this;
	}

	bool TypeMatches(const VARIANT& var) const
	{
		GUID guidSrc;

		if (var.vt != VT_RECORD)
		{
			return false;
		}

		if (var.pRecInfo == NULL)
		{
			return false;
		}

		if (FAILED(var.pRecInfo->GetGuid(&guidSrc)))
		{
			return false;
		}

		if (!::InlineIsEqualGUID(guidSrc, __uuidof(T)))
		{
			return false;
		}

		return true;
	}

	operator VARIANT() const
	{
		_variant_t var;
		var.vt = VT_RECORD;
		var.pvRecord = NULL;
		var.pRecInfo = const_cast< CVtRecord<T>* >(this)->GetRecordInfo();

		if (var.pRecInfo != NULL)
		{
			var.pRecInfo->AddRef();
			var.pRecInfo->RecordCreateCopy(const_cast< CVtRecord<T>* >(this), &var.pvRecord);
		}

		return var.Detach();
	}
};

template <typename T>
	CComTypeInfoHolder CVtRecord<T>::_tih =
		{&__uuidof(T), &CComModule::m_libid, 1, 0, NULL, 0, NULL, 0};

};  // namespace ShapeShifter

#endif // #ifndef __VTRECORD_H__
