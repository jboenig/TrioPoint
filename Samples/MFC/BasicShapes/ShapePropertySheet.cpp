// ShapePropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "BasicShapes.h"
#include "ShapePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapePropertySheet

IMPLEMENT_DYNAMIC(CShapePropertySheet, CPropertySheet)

CShapePropertySheet::CShapePropertySheet(CWnd* pParentWnd)
	:CPropertySheet(IDS_SHAPE_PROPERTYSHEET, pParentWnd)
{
	AddPage(&m_pageGeneral);
}

CShapePropertySheet::~CShapePropertySheet()
{
}

void CShapePropertySheet::SetNode(IDispatch* pNode)
{
	m_pageGeneral.SetNode(pNode);
}

BEGIN_MESSAGE_MAP(CShapePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CShapePropertySheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapePropertySheet message handlers

BOOL CShapePropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	CenterWindow();
	return bResult;
}
