//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Command.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __COMMAND_H__
#define __COMMAND_H__

#pragma once

#include <vector>

#include "ShapeShifter.h"
#include "Node.h"
#include "NodeCollection.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// ICommandImpl

template <class base_class>
class ICommandImpl : public base_class
{
// Constructors/destructor
public:
	ICommandImpl()
	{
	}

// Operations
public:
	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal)
	{
		*pVal = NULL;
		return S_OK;
	}

	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget)
	{
		pCmdTarget;
		return E_FAIL;
	}

	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget)
	{
		pCmdTarget;
		return E_FAIL;
	}
};

/////////////////////////////////////////////////////////////////////////
// INodeCommandImpl

template <class base_class>
class INodeCommandImpl : public ICommandImpl<base_class>
{
// Constructors/destructor
public:
	INodeCommandImpl() :
		m_pNodes(NULL)
	{
	}

	virtual ~INodeCommandImpl()
	{
		if (m_pNodes != NULL)
		{
			m_pNodes->Release();
		}
	}

// Attributes
protected:
	INodes* m_pNodes;

// Operations
public:
	STDMETHOD(AddNode)(/* [in] */ INode* pNode)
	{
		if (m_pNodes != NULL)
		{
			return m_pNodes->Add(pNode);
		}

		return E_FAIL;
	}

	STDMETHOD(AddNodes)(/* [in] */ INodes* pNodeCollection)
	{
		HRESULT hr = E_FAIL;
		IUnknown* pEnumUnk = NULL;

		if (SUCCEEDED(pNodeCollection->get__NewNodeEnum(&pEnumUnk)) && pEnumUnk != NULL)
		{
			IEnumNodes* pEnumNodes;

			if (SUCCEEDED(pEnumUnk->QueryInterface(IID_IEnumNodes, (LPVOID*) &pEnumNodes)))
			{
				ULONG nFetched;
				INode* pCurNode;
				while (pEnumNodes->Next(1, &pCurNode, &nFetched) == S_OK && nFetched > 0)
				{
					m_pNodes->Add(pCurNode);
					pCurNode->Release();
				}

				pEnumNodes->Release();
				hr = S_OK;
			}

			pEnumUnk->Release();
		}

		return hr;
	}

// Implementation
protected:
	HRESULT CreateNodeCollection()
	{
		HRESULT hr = E_FAIL;

		if (m_pNodes == NULL)
		{
			hr = CNodeList::_CreatorClass::CreateInstance(NULL, IID_INodes, (LPVOID*) &m_pNodes);
		}

		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////
// ICreateCommandImpl

template <class base_class>
class ICreateCommandImpl : public ICommandImpl<base_class>
{
// Constructors/destructor
public:
	ICreateCommandImpl() :
		m_pNode(NULL)
	{
	}

	virtual ~ICreateCommandImpl()
	{
		if (m_pNode != NULL)
		{
			m_pNode->Release();
		}
	}

// Attributes
protected:
	_bstr_t m_strNodeName;
	_bstr_t m_strProgId;
	_bstr_t m_strParent;
	INode* m_pNode;

// Operations
public:
	STDMETHOD(SetNodeName)(/* [in] */ BSTR strNodeName)
	{
		m_strNodeName = strNodeName;
		return S_OK;
	}

	STDMETHOD(SetNodeType)(/* [in] */ BSTR strProgId)
	{
		m_strProgId = strProgId;
		return S_OK;
	}

	STDMETHOD(SetParent)(/* [in] */ BSTR strParent)
	{
		m_strParent = strParent;
		return S_OK;
	}

	STDMETHOD(get_Node)(/* [out,retval] */ INode** ppNode)
	{
		if (ppNode == NULL)
		{
			return E_POINTER;
		}

		*ppNode = m_pNode;

		if (*ppNode != NULL)
		{
			(*ppNode)->AddRef();
		}

		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////
// CCreateShapeCommand

class ATL_NO_VTABLE CCreateShapeCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                          public CComCoClass<CCreateShapeCommand, &CLSID_CreateShapeCommand>,
                                          public IDispatchImpl<ICreateCommandImpl<ICreateShapeCommand>, &__uuidof(ICreateShapeCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CCreateShapeCommand();
	virtual ~CCreateShapeCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_CREATE_SHAPE_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CCreateShapeCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(ICreateCommand)
	COM_INTERFACE_ENTRY(ICreateShapeCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	_bstr_t m_strMeshName;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);

	//////////////////////////////////////////////////////////////
	// ICreateShapeCommand interface

	STDMETHOD(SetMeshName)(/* [in] */ BSTR strMeshName);
};

/////////////////////////////////////////////////////////////////////////
// CInsertCommand

class ATL_NO_VTABLE CInsertCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                     public CComCoClass<CInsertCommand, &CLSID_InsertCommand>,
                                     public IDispatchImpl<INodeCommandImpl<IInsertCommand>, &__uuidof(IInsertCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CInsertCommand();
	virtual ~CInsertCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_INSERT_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CInsertCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(INodeCommand)
	COM_INTERFACE_ENTRY(IInsertCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	_bstr_t m_strParent;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);

	//////////////////////////////////////////////////////////////
	// IInsertCommand interface

	STDMETHOD(SetParent)(/* [in] */ BSTR strParent);
};

/////////////////////////////////////////////////////////////////////////
// CDeleteCommand

class ATL_NO_VTABLE CDeleteCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                     public CComCoClass<CDeleteCommand, &CLSID_DeleteCommand>,
                                     public IDispatchImpl<INodeCommandImpl<INodeCommand>, &__uuidof(INodeCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CDeleteCommand();
	virtual ~CDeleteCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_DELETE_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CDeleteCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(INodeCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	std::vector<NodeInsertPosition> m_posInfo;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);
};

/////////////////////////////////////////////////////////////////////////
// CMoveCommand

class ATL_NO_VTABLE CMoveCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                   public CComCoClass<CMoveCommand, &CLSID_MoveCommand>,
                                   public IDispatchImpl<INodeCommandImpl<IMoveCommand>, &__uuidof(IMoveCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CMoveCommand();
	virtual ~CMoveCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_MOVE_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CMoveCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(INodeCommand)
	COM_INTERFACE_ENTRY(IMoveCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	VALUE3D m_tx;
	VALUE3D m_ty;
	VALUE3D m_tz;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IMoveCommand interface

	STDMETHOD(SetDelta)(/* [in] */ VALUE3D tx, /* [in] */ VALUE3D ty, /* [in] */ VALUE3D tz);

	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);
};

/////////////////////////////////////////////////////////////////////////
// CRotateCommand

class ATL_NO_VTABLE CRotateCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                     public CComCoClass<CRotateCommand, &CLSID_RotateCommand>,
                                     public IDispatchImpl<INodeCommandImpl<IRotateCommand>, &__uuidof(IRotateCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CRotateCommand();
	virtual ~CRotateCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_ROTATE_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CRotateCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(INodeCommand)
	COM_INTERFACE_ENTRY(IRotateCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	VALUE3D m_fRadians;
	VALUE3D m_fAxisX;
	VALUE3D m_fAxisY;
	VALUE3D m_fAxisZ;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IRotateCommand interface

	STDMETHOD(SetAngle)(/* [in] */ VALUE3D fRadians);
	STDMETHOD(SetAxis)(/* [in] */ VALUE3D fAxisX,
	                   /* [in] */ VALUE3D fAxisY,
	                   /* [in] */ VALUE3D fAxisZ);

	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);
};

/////////////////////////////////////////////////////////////////////////
// CScaleCommand

class ATL_NO_VTABLE CScaleCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                    public CComCoClass<CScaleCommand, &CLSID_ScaleCommand>,
                                    public IDispatchImpl<INodeCommandImpl<IScaleCommand>, &__uuidof(IScaleCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CScaleCommand();
	virtual ~CScaleCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_SCALE_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CScaleCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(INodeCommand)
	COM_INTERFACE_ENTRY(IScaleCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	VALUE3D m_sx;
	VALUE3D m_sy;
	VALUE3D m_sz;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// IScaleCommand interface

	STDMETHOD(SetScale)(/* [in] */ VALUE3D sx, /* [in] */ VALUE3D sy, /* [in] */ VALUE3D sz);

	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);
};

/////////////////////////////////////////////////////////////////////////
// CGroupCommand

class ATL_NO_VTABLE CGroupCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                    public CComCoClass<CGroupCommand, &CLSID_GroupCommand>,
                                    public IDispatchImpl<INodeCommandImpl<IGroupCommand>, &__uuidof(IGroupCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CGroupCommand();
	virtual ~CGroupCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_GROUP_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CGroupCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(INodeCommand)
	COM_INTERFACE_ENTRY(IGroupCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	_bstr_t m_strParent;
	_bstr_t m_strNodeName;
	INode* m_pGroupNode;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);

	//////////////////////////////////////////////////////////////
	// IGroupCommand interface

	STDMETHOD(SetParent)(/* [in] */ BSTR strParent);
	STDMETHOD(SetNodeName)(/* [in] */ BSTR strNodeName);
};

/////////////////////////////////////////////////////////////////////////
// CUngroupCommand

class ATL_NO_VTABLE CUngroupCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                      public CComCoClass<CUngroupCommand, &CLSID_UngroupCommand>,
                                      public IDispatchImpl<ICommandImpl<IUngroupCommand>, &__uuidof(IUngroupCommand), &LIBID_ShapeShifter>
{
// Constructors/destructor
public:
	CUngroupCommand();
	virtual ~CUngroupCommand();

	HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_UNGROUP_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CUngroupCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(IUngroupCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	_bstr_t m_strNodeName;
	INode* m_pGroupNode;
	INodes* m_pNodes;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);

	//////////////////////////////////////////////////////////////
	// IGroupCommand interface

	STDMETHOD(SetNodeName)(/* [in] */ BSTR strNodeName);
};

/////////////////////////////////////////////////////////////////////////
// CMacroCommand

class ATL_NO_VTABLE CMacroCommand : public CComObjectRootEx<CComSingleThreadModel>,
                                    public CComCoClass<CMacroCommand, &CLSID_MacroCommand>,
                                    public IDispatchImpl<ICommandImpl<IMacroCommand>, &__uuidof(IMacroCommand), &LIBID_ShapeShifter>
{
// Embedded types
public:
	typedef std::vector<ICommandPtr> _CommandList;

// Constructors/destructor
public:
	CMacroCommand();
	virtual ~CMacroCommand();

DECLARE_REGISTRY_RESOURCEID(IDR_MACRO_COMMAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface map
BEGIN_COM_MAP(CMacroCommand)
	COM_INTERFACE_ENTRY(ICommand)
	COM_INTERFACE_ENTRY(IMacroCommand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Attributes
protected:
	_CommandList m_commands;

// Operations
public:
	//////////////////////////////////////////////////////////////
	// ICommand interface

	STDMETHOD(get_Description)(/* [out, retval] */ BSTR *pVal);
	STDMETHOD(Do)(/* [in] */ INode* pCmdTarget);
	STDMETHOD(Undo)(/* [in] */ INode* pCmdTarget);

	//////////////////////////////////////////////////////////////
	// IMacroCommand interface

	STDMETHOD(AddCommand)(/* [in] */ ICommand* pCommand);
	STDMETHOD(ClearCommands)();
	STDMETHOD(get_CommandCount)(/* [out, retval] */ long* pVal);
};

};  // namespace ShapeShifter

#endif // #ifndef __COMMAND_H__

