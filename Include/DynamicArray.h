//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: DynamicArray.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __DYNAMICARRAY_H__
#define __DYNAMICARRAY_H__

#pragma once

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// CDynamicArray

template <typename element_type, typename index_type, typename size_type, size_type nInitSize = 3, size_type nGrowBy = 3>
class CDynamicArray
{
// Embedded types
public:
	typedef element_type _ElementType;
	typedef index_type _IndexType;
	typedef size_type _SizeType;

// Constructors/destructor
public:
	CDynamicArray() :
		m_pElementArray(NULL),
		m_nElementCount(0),
		m_nArraySize(0)
	{
		SetArraySize(nInitSize);
	}

	virtual ~CDynamicArray()
	{
		ClearArray();
	}

// Attributes
public:
	_ElementType* m_pElementArray;
	_SizeType m_nElementCount;
	_SizeType m_nArraySize;

// Operations
public:
	inline _SizeType GetElementCount() const
	{
		return m_nElementCount;
	}

	inline _ElementType& operator[](const _IndexType i)
	{
		_ASSERTE(i < m_nArraySize);  // probably should throw exception instead

		if (i >= m_nElementCount)
		{
			m_nElementCount = i + 1;
		}

		return m_pElementArray[i];
	}

	inline _ElementType& operator[](const WORD i)
	{
		_ASSERTE(i < m_nArraySize);  // probably should throw exception instead

		if (i >= m_nElementCount)
		{
			m_nElementCount = i + 1;
		}

		return m_pElementArray[i];
	}

	inline operator _ElementType*() const
	{
		return m_pElementArray;
	}

	inline operator const _ElementType*() const
	{
		return m_pElementArray;
	}

	inline void ClearArray()
	{
		if (m_pElementArray != NULL)
		{
			delete[] m_pElementArray;
		}
		m_pElementArray = NULL;
		m_nElementCount = 0;
		m_nArraySize = 0;
	}

	void SetArraySize(const _SizeType nArraySize)
	{
		_ASSERTE(nArraySize >= 0);

		if (nArraySize == 0)
		{
			ClearArray();
		}
		else if (nArraySize > 0 && nArraySize != m_nArraySize)
		{
			_ElementType* pArray = new _ElementType[nArraySize];
			_SizeType nCopyCount = (m_nElementCount <= nArraySize) ? (m_nElementCount) : (nArraySize);
			_SizeType nZeroCount = nArraySize - nCopyCount;
			_ASSERTE(nZeroCount >= 0);

			if (m_pElementArray != NULL)
			{
				// Copy from old array to new array
				_ASSERTE(nCopyCount >= 0 && nCopyCount <= nArraySize);
				memcpy(pArray, m_pElementArray, nCopyCount * sizeof(_ElementType));
				delete[] m_pElementArray;
			}

			if (nZeroCount > 0)
			{
				memset(&pArray[nCopyCount], 0, nZeroCount * sizeof(_ElementType));
			}

			m_pElementArray = pArray;
			m_nArraySize = nArraySize;
		}
	}

	void GrowArray(const _SizeType nElements)
	{
		if (nElements > m_nArraySize)
		{
			SetArraySize(nElements + (nElements % nGrowBy));
		}
	}

	int PutElement(const _ElementType& element)
	{
		// If element is already in the array, just return
		// the index of the existing entry.
		for (_IndexType i = 0; i < m_nElementCount; i++)
		{
			int nCmpVal = memcmp(&m_pElementArray[i], &element, sizeof(_ElementType));

			if (nCmpVal == 0)
			{
				return i;
			}
		}

		// Add a new entry for the element.
		return AppendElement(element);
	}

	int AppendElement(const _ElementType& element)
	{
		int nIdx = m_nElementCount;
		GrowArray(m_nElementCount + 1);
		m_pElementArray[nIdx] = element;
		m_nElementCount++;
		return nIdx;
	}

	bool RemoveElements(const _IndexType nStartIdx, const _IndexType nEndIdx)
	{
		bool bSuccess = false;

		_ASSERTE(m_pElementArray != NULL);
		_ASSERTE(nStartIdx >= 0 && nStartIdx < m_nElementCount);
		_ASSERTE(nEndIdx >= nStartIdx && nEndIdx < m_nElementCount);

		if ((m_pElementArray != NULL) &&
		    (nStartIdx >= 0 && nStartIdx < m_nElementCount) &&
		    (nEndIdx >= nStartIdx && nEndIdx < m_nElementCount))
		{
			_SizeType nCopyCount = m_nElementCount - (nEndIdx + 1);
			if (nCopyCount > 0)
			{
				memcpy(&m_pElementArray[nStartIdx], &m_pElementArray[nEndIdx+1],
					   nCopyCount * sizeof(_ElementType));
			}
			m_nElementCount -= (nEndIdx - nStartIdx + 1);
			bSuccess = true;
		}

		return bSuccess;
	}

	bool FindElement(const _ElementType& element, _IndexType& elementIdx) const
	{
		for (_IndexType i = 0; i < m_nElementCount; i++)
		{
			int nCmpVal = memcmp(&m_pElementArray[i], &element, sizeof(_ElementType));

			if (nCmpVal == 0)
			{
				elementIdx = i;
				return true;
			}
		}
		return false;
	}

	void AttachArray(_ElementType* pElementArray, const _SizeType nElementCount, const _SizeType nArraySize)
	{
		ClearArray();
		m_pElementArray = pElementArray;
		m_nElementCount = nElementCount;
		m_nArraySize = nArraySize;
	}

	_ElementType* DetachArray(_SizeType* pElementCount, _SizeType* pArraySize)
	{
		_ElementType* pArray = m_pElementArray;

		if (pElementCount != NULL)
		{
			*pElementCount = m_nElementCount;
		}

		if (pArraySize != NULL)
		{
			*pArraySize = m_nArraySize;
		}

		m_pElementArray = NULL;
		m_nElementCount = 0;
		m_nArraySize = 0;

		return pArray;
	}

	void CopyArray(_ElementType* pElementArray, const _SizeType nElementCount)
	{
		_ASSERTE(pElementArray != NULL);
		_ASSERTE(nElementCount > 0);
		ClearArray();
		m_pElementArray = new _ElementType[nElementCount];
		memcpy(m_pElementArray, pElementArray, nElementCount * sizeof(_ElementType));
		m_nArraySize = nElementCount;
		m_nElementCount = nElementCount;
	}

	_IndexType AppendArray(_ElementType* pElementArray, const _SizeType nElementCount)
	{
		_IndexType nStart = m_nElementCount;
		_ASSERTE(pElementArray != NULL);
		_ASSERTE(nElementCount > 0);

		GrowArray(m_nElementCount + nElementCount);

		memcpy(&m_pElementArray[nStart], pElementArray, nElementCount * sizeof(_ElementType));
		m_nElementCount += nElementCount;

		return nStart;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __DYNAMICARRAY_H__

