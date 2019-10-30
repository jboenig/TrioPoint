// ShapeGeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "BasicShapes.h"
#include "ShapeGeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapeGeneralPage property page

IMPLEMENT_DYNCREATE(CShapeGeneralPage, CPropertyPage)

CShapeGeneralPage::CShapeGeneralPage() : CPropertyPage(CShapeGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CShapeGeneralPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CShapeGeneralPage::~CShapeGeneralPage()
{
}

void CShapeGeneralPage::SetNode(IDispatch* pNode)
{
	m_spShape = pNode;
}

void CShapeGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapeGeneralPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapeGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CShapeGeneralPage)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapeGeneralPage message handlers

BOOL CShapeGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (m_spShape != NULL)
	{
		long nNumPrimitives = m_spShape->Mesh->GetPrimitiveCount();
		CString buf;
		buf.Format("%d", nNumPrimitives);
		SetDlgItemText(IDC_NUM_PRIMITIVES, buf);

		long nNumFaces = 0;
		long nNumVertices = 0;

		for (long nPrimIdx = 0; nPrimIdx < nNumPrimitives; nPrimIdx++)
		{
			long nCurPrimFaceCount = m_spShape->Mesh->GetPrimitiveFaceCount(nPrimIdx);
			nNumFaces += nCurPrimFaceCount;
			long nCurVertexCount = m_spShape->Mesh->GetPrimitiveVertexCount(nPrimIdx);
			nNumVertices += nCurVertexCount;
		}
		buf.Format("%d", nNumFaces);
		SetDlgItemText(IDC_NUM_FACES, buf);

		buf.Format("%d", nNumVertices);
		SetDlgItemText(IDC_NUM_VERTICES, buf);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
