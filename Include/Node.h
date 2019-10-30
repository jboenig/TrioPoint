#ifndef __NODE_H__
#define __NODE_H__

#pragma once

#include <vector>

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// INodeImpl

template <class base_class>
class INodeImpl : public base_class
{
// Constructors/destructor
public:
	INodeImpl()
	{
		m_pParent = NULL;
		m_name = NULL;
	}

	virtual ~INodeImpl()
	{
		if (m_name != NULL)
		{
			::SysFreeString(m_name);
		}
	}

// Attributes
protected:
	INode* m_pParent;   // Weak reference - not reference counted
	BSTR m_name;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// INodeImpl interface

	STDMETHOD(get_Parent)(/* [out, retval] */ INode** ppVal)
	{
		if (ppVal == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		*ppVal = m_pParent;

		if (*ppVal != NULL)
		{
			(*ppVal)->AddRef();
			hr = S_OK;
		}

		return hr;
	}

	STDMETHOD(putref_Parent)(/* [in] */ INode* pParent)
	{
		// AddRef omitted to avoid circular reference (weak reference)
		m_pParent = pParent;
		return S_OK;
	}

	STDMETHOD(get_Root)(/* [out, retval] */ INode** ppVal)
	{
		if (ppVal == NULL)
		{
			return E_POINTER;
		}

		if (m_pParent == NULL)
		{
			*ppVal = static_cast<INode*>(this);
			(*ppVal)->AddRef();
			return S_OK;
		}

		return m_pParent->get_Root(ppVal);
	}

	STDMETHOD(get_Name)(/* [out, retval] */ BSTR *pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		if (m_name != NULL)
		{
			*pVal = ::SysAllocString(m_name);
			if (::SysStringLen(*pVal) > 0)
			{
				return S_OK;
			}
			else
			{
				return S_FALSE;
			}
		}
		else
		{
			*pVal = NULL;
			return S_FALSE;
		}

		return E_FAIL;
	}

	STDMETHOD(put_Name)(/* [in] */ BSTR newVal)
	{
		if (m_name != NULL)
		{
			::SysFreeString(m_name);
			m_name = NULL;
		}
		if (newVal != NULL)
		{
			m_name = ::SysAllocString(newVal);
		}
		return S_OK;	
	}

	STDMETHOD(get_FullName)(/* [out, retval] */ BSTR *pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		if (m_name != NULL)
		{
			_bstr_t fullName;

			if (m_pParent != NULL)
			{
				BSTR strBaseName = NULL;

				if (m_pParent->get_FullName(&strBaseName) == S_OK)
				{
					fullName += strBaseName;
					fullName += L".";
				}
			}

			fullName += m_name;

			if (fullName.length() > 0)
			{
				*pVal = fullName.copy();
				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(get_RelativePath)(/* [out, retval] */ BSTR *pVal)
	{
		if (pVal == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		if (m_name != NULL)
		{
			_bstr_t fullName;

			if (m_pParent != NULL)
			{
				BSTR strBaseName = NULL;

				if (m_pParent->get_RelativePath(&strBaseName) == S_OK)
				{
					fullName += strBaseName;
					fullName += L".";
				}

				fullName += m_name;
			}

			if (fullName.length() > 0)
			{
				*pVal = fullName.copy();
				hr = S_OK;
			}
		}

		return hr;
	}

	STDMETHOD(LoadDevice)(/* [in] */ IDevice* pDevice)
	{
		pDevice;
		return S_OK;
	}

	STDMETHOD(UnloadDevice)(/* [in] */ IDevice* pDevice)
	{
		pDevice;
		return S_OK;
	}

// Implementation
protected:
	HRESULT NotifyParent(long nNotifyCode)
	{
		ICompositeNodePtr spParentComposite(m_pParent);

		if (spParentComposite != NULL)
		{
			return spParentComposite->OnChildUpdate(static_cast<INode*>(this), nNotifyCode);
		}

		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////
// ICompositeNodeImpl

template <class base_class, class child_entry>
class ICompositeNodeImpl : public INodeImpl<base_class>
{
// Embedded types
public:
	typedef std::vector<child_entry> _NodeVector;

// Constructors/destructor
public:
	ICompositeNodeImpl()
	{
	}

	virtual ~ICompositeNodeImpl()
	{
	}

// Attributes
protected:
	_NodeVector m_children;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICompositeNode interface

	STDMETHOD(GetChildCount)(/* [out,retval] */ long* pNumChildren)
	{
		if (pNumChildren != NULL)
		{
			*pNumChildren = m_children.size();
			return S_OK;
		}
		return E_FAIL;
	}

	STDMETHOD(GetChildIndex)(/* [in] */ BSTR strName, /* [out,retval] */ long* pChildIndex)
	{
		HRESULT hr = S_FALSE;
		_NodeVector::iterator itNode;
		_bstr_t name(strName);
		long nCurIdx = 0;

		for (itNode = m_children.begin(); itNode != m_children.end() && hr == S_FALSE; itNode++)
		{
			BSTR curName = NULL;

			if (SUCCEEDED((*itNode)->get_Name(&curName)))
			{
				if (name == _bstr_t(curName))
				{
					hr = S_OK;

					if (pChildIndex != NULL)
					{
						*pChildIndex = nCurIdx;
					}
				}

				::SysFreeString(curName);
			}

			nCurIdx++;
		}

		return hr;
	}

	STDMETHOD(GetChild)(/* [in] */ long nChildIndex, /* [out,retval] */ INode** ppChild)
	{
		HRESULT hr = E_FAIL;

		if (static_cast<UINT>(nChildIndex) < m_children.size())
		{
			if (ppChild != NULL)
			{
				*ppChild = m_children[nChildIndex];

				if (*ppChild != NULL)
				{
					(*ppChild)->AddRef();
					hr = S_OK;
				}
			}
		}

		return hr;
	}

	STDMETHOD(GetChildByName)(/* [in] */ BSTR bName, /* [out,retval] */ INode** ppNode)
	{
		// Check parameters for validity
		if (bName == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = S_FALSE;

		INode* pMatchingChild = NULL;
		_bstr_t baseName;
		_bstr_t relName;

		if (ParseNodeName(_bstr_t(bName), baseName, relName))
		{
			// Compound node name. Recurse to base node and pass the
			// remaining relative name to it.

			pMatchingChild = LocateChild(baseName);

			if (pMatchingChild != NULL)
			{
				ICompositeNodePtr spCompositeChild(pMatchingChild);

				if (spCompositeChild != NULL)
				{
					hr = spCompositeChild->GetChildByName(relName, ppNode);
				}

				pMatchingChild->Release();
			}
		}
		else
		{
			// Node name cannot be parsed any further. Return the
			// node matching the simple name.

			pMatchingChild = LocateChild(relName);

			if (pMatchingChild != NULL)
			{
				if (ppNode != NULL)
				{
					*ppNode = pMatchingChild;
					(*ppNode)->AddRef();
				}
				pMatchingChild->Release();
				hr = S_OK;
			}
			else
			{
				// Child node with the given name does not exist
				if (ppNode != NULL)
				{
					*ppNode = NULL;
				}
				hr = S_FALSE;
			}
		}

		return hr;
	}

	STDMETHOD(AppendChild)(/* [in] */ INode* pChild)
	{
		if (pChild == NULL)
		{
			return E_POINTER;
		}

		if (DuplicateName(pChild))
		{
			return E_FAIL;
		}

		m_children.push_back(pChild);

		if (CanOwnChildren())
		{
			INode* pParent = NULL;
			if (pChild->get_Parent(&pParent) == S_OK)
			{
				// This child already has a parent
				pParent->Release();
			}
			else
			{
				// Make this node the parent of the new child
				pChild->putref_Parent(this);
			}
		}

		OnChildUpdate(pChild, ChildNotification::ADD);

		return S_OK;
	}

	STDMETHOD(InsertChild)(/* [in] */ INode* pChild, /* [in] */ long nChildIndex)
	{
		if (pChild == NULL)
		{
			return E_POINTER;
		}

		if (nChildIndex < 0 || nChildIndex >= (long) m_children.size())
		{
			m_children.push_back(pChild);
		}
		else
		{
			_NodeVector::iterator itNode = m_children.begin() + nChildIndex;

			if (itNode == m_children.end())
			{
				return E_FAIL;
			}
			m_children.insert(itNode, pChild);
		}

		if (CanOwnChildren())
		{
			INode* pParent = NULL;
			if (pChild->get_Parent(&pParent) == S_OK)
			{
				// This child already has a parent
				pParent->Release();
			}
			else
			{
				// Make this node the parent of the new child
				pChild->putref_Parent(this);
			}
		}

		OnChildUpdate(pChild, ChildNotification::ADD);

		return S_OK;
	}

	STDMETHOD(RemoveChild)(/* [in] */ INode* pChild)
	{
		if (pChild == NULL)
		{
			return E_POINTER;
		}

		HRESULT hr = E_FAIL;
		IUnknown* pChildUnk = NULL;
		hr = pChild->QueryInterface(IID_IUnknown, (LPVOID*) &pChildUnk);

		if (SUCCEEDED(hr))
		{
			hr = S_FALSE;

			_NodeVector::iterator itNode = m_children.begin();

			while (hr != S_OK && itNode != m_children.end())
			{
				IUnknown* pCurUnk = NULL;

				if (SUCCEEDED((*itNode)->QueryInterface(IID_IUnknown, (LPVOID*) &pCurUnk)))
				{
					if (pCurUnk == pChildUnk)
					{
						m_children.erase(itNode);
						hr = S_OK;
					}

					pCurUnk->Release();
				}

				itNode++;
			}

			pChildUnk->Release();

			if (hr == S_OK)
			{
				OnChildUpdate(pChild, ChildNotification::REMOVE);
			}
		}

		return hr;
	}

	STDMETHOD(RemoveAllChildren)()
	{
		m_children.clear();
		OnChildUpdate(NULL, ChildNotification::REMOVE_ALL);
		return S_OK;
	}

	STDMETHOD(OnChildUpdate)(/* [in] */ INode* pNode, /* [in] */ long nNotifyCode)
	{
		// Forward notification to parent, if one exists

		ICompositeNodePtr spParentComposite(m_pParent);

		if (spParentComposite != NULL)
		{
			return spParentComposite->OnChildUpdate(pNode, nNotifyCode);
		}

		return S_OK;
	}

	STDMETHOD(LoadDevice)(/* [in] */ IDevice* pDevice)
	{
		HRESULT hr = S_OK;

		_NodeVector::iterator it;

		for (it = m_children.begin(); SUCCEEDED(hr) && it != m_children.end(); it++)
		{
			if (IsParent(*it))
			{
				hr = (*it)->LoadDevice(pDevice);
			}
		}

		return hr;
	}

	STDMETHOD(UnloadDevice)(/* [in] */ IDevice* pDevice)
	{
		HRESULT hr = S_OK;

		_NodeVector::iterator it;

		for (it = m_children.begin(); SUCCEEDED(hr) && it != m_children.end(); it++)
		{
			if (IsParent(*it))
			{
				hr = (*it)->UnloadDevice(pDevice);
			}
		}

		return hr;
	}

	bool DuplicateName(INode* pNamedObj)
	{
		bool bIsDup = false;
		BSTR strName = NULL;

		if (SUCCEEDED(pNamedObj->get_Name(&strName)))
		{
			if (GetChildByName(strName, NULL) == S_OK)
			{
				bIsDup = true;
			}
			::SysFreeString(strName);
		}

		return bIsDup;
	}

	BSTR MakeUniqueName(INode* pNamedObj)
	{
		BSTR strCurName;
		_bstr_t strBaseName;
		_bstr_t strNewName;
		bool bIsDup = true;
		INode* pCurObj = NULL;
		static int nSuffix = 1;

		if (pNamedObj->get_Name(&strCurName) == S_OK)
		{
			strBaseName = strCurName;
			::SysFreeString(strCurName);
		}
		else
		{
			strBaseName = L"Object";
		}

		strNewName = strBaseName;

		while (bIsDup)
		{
			if (GetChildByName(strNewName, &pCurObj) != S_OK)
			{
				bIsDup = false;
			}
			else
			{
				TCHAR szSuffix[20];
				_itot(nSuffix++, szSuffix, 10);
				strNewName = strBaseName + _bstr_t(szSuffix);
			}
		}

		return strNewName.copy();
	}

// Implementation
protected:
	virtual bool CanOwnChildren() const
	{
		return true;
	}

	bool IsParent(INode* pNode)
	{
		bool bIsParent = false;
		INode* pParent = NULL;

		if (pNode != NULL && pNode->get_Parent(&pParent) == S_OK)
		{
			bIsParent = (pParent == static_cast<INode*>(this));
			pParent->Release();
		}

		return bIsParent;
	}

	virtual INode* LocateChild(BSTR strName)
	{
		INode* pMatchingChild = NULL;
		_NodeVector::iterator itNode;
		_bstr_t name(strName);

		for (itNode = m_children.begin(); itNode != m_children.end() && pMatchingChild == NULL; itNode++)
		{
			BSTR curName = NULL;

			if (SUCCEEDED((*itNode)->get_Name(&curName)))
			{
				if (name == _bstr_t(curName))
				{
					// Matching name found
					pMatchingChild = (*itNode);
					pMatchingChild->AddRef();
				}

				::SysFreeString(curName);
			}
		}

		return pMatchingChild;
	}
};

/////////////////////////////////////////////////////////////////////////
// NodeInsertPosition

struct NodeInsertPosition
{
	_bstr_t m_strParent;
	long m_nChildIdx;

	NodeInsertPosition(INode* pNode) :
		m_nChildIdx(-1)
	{
		m_strParent = L"";
		INode* pParent = NULL;
		if (pNode->get_Parent(&pParent) == S_OK)
		{
			ICompositeNodePtr spParentComposite(pParent);

			// Get the name of the parent
			BSTR strParentName = NULL;
			if (pParent->get_RelativePath(&strParentName) == S_OK)
			{
				m_strParent = strParentName;
				::SysFreeString(strParentName);
			}

			// Get the index value of the node
			BSTR strNodeName = NULL;
			if (pNode->get_Name(&strNodeName) == S_OK)
			{
				if (spParentComposite != NULL)
				{
					spParentComposite->GetChildIndex(strNodeName, &m_nChildIdx);
				}
				::SysFreeString(strNodeName);
			}

			pParent->Release();
		}
	}

	NodeInsertPosition(const NodeInsertPosition& src)
	{
		m_strParent = src.m_strParent;
		m_nChildIdx = src.m_nChildIdx;
	}

	NodeInsertPosition& operator=(const NodeInsertPosition& src)
	{
		m_strParent = src.m_strParent;
		m_nChildIdx = src.m_nChildIdx;
		return *this;
	}
};

};  // namespace ShapeShifter

#endif // #ifndef __NODE_H__
