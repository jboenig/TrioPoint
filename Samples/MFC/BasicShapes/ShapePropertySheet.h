#if !defined(AFX_SHAPEPROPERTYSHEET_H__9EF09AF1_CC5F_45B2_9059_3DE88427FD5B__INCLUDED_)
#define AFX_SHAPEPROPERTYSHEET_H__9EF09AF1_CC5F_45B2_9059_3DE88427FD5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShapePropertySheet.h : header file
//

#include "ShapeGeneralPage.h"

/////////////////////////////////////////////////////////////////////////////
// CShapePropertySheet

class CShapePropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CShapePropertySheet)

// Construction
public:
	CShapePropertySheet(CWnd* pParentWnd = NULL);

// Attributes
public:
	CShapeGeneralPage m_pageGeneral;

// Operations
public:
	void SetNode(IDispatch* pNode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapePropertySheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShapePropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShapePropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPEPROPERTYSHEET_H__9EF09AF1_CC5F_45B2_9059_3DE88427FD5B__INCLUDED_)
