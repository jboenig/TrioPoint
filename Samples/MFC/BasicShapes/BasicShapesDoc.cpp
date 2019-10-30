// BasicShapesDoc.cpp : implementation of the CBasicShapesDoc class
//

#include "stdafx.h"
#include "BasicShapes.h"

#include "BasicShapesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesDoc

IMPLEMENT_DYNCREATE(CBasicShapesDoc, CDocument)

BEGIN_MESSAGE_MAP(CBasicShapesDoc, CDocument)
	//{{AFX_MSG_MAP(CBasicShapesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesDoc construction/destruction

CBasicShapesDoc::CBasicShapesDoc()
{
	// TODO: add one-time construction code here

}

CBasicShapesDoc::~CBasicShapesDoc()
{
}

BOOL CBasicShapesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	/////////////////////////////////////////////////////////////////////
	// BEGIN ShapeShifter

	m_spModel = NULL;
	m_spModel.CreateInstance(__uuidof(Model));
	m_spModel->SetAmbientLight(0.5f, 0.5f, 0.5f, 0.0f);

	IMaterialPtr pDefaultMaterial = m_spModel->CreateNode(_bstr_t("Default Material"), _bstr_t("ShapeShifter.Material"), _bstr_t());
	pDefaultMaterial->SetDiffuseColor(0.5f, 0.6f, 0.5f, 0.0f);
	pDefaultMaterial->SetAmbientColor(0.4f, 0.4f, 0.4f, 0.0f);
	pDefaultMaterial->SetSpecularColor(0.5f, 0.5f, 0.5f, 0.0f);

	ILightPtr pDirLight = m_spModel->CreateNode(_bstr_t("Light1"), _bstr_t("ShapeShifter.DirectionalLight"), _bstr_t());
	pDirLight->SetAmbientColor(0.5f, 0.5f, 0.5f);
	pDirLight->SetDiffuseColor(0.5f, 0.5f, 0.5f);
	pDirLight->SetSpecularColor(0.5f, 0.5f, 0.5f);
	pDirLight->SetPosition(20.0f, 20.0f, -20.0f);
	pDirLight->SetDirection(-1.0f, -1.0f, 1.0f);

	// END ShapeShifter
	/////////////////////////////////////////////////////////////////////

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBasicShapesDoc serialization

void CBasicShapesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesDoc diagnostics

#ifdef _DEBUG
void CBasicShapesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBasicShapesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesDoc commands

BOOL CBasicShapesDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	IModelWriterPtr spXMLWriter;
	spXMLWriter.CreateInstance(__uuidof(ModelWriterXML));
	spXMLWriter->Write(_variant_t(lpszPathName), m_spModel);
	spXMLWriter = NULL;

	return TRUE;
}

BOOL CBasicShapesDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	IModelReaderPtr spXMLReader;
	spXMLReader.CreateInstance(__uuidof(ModelReaderXML));
	m_spModel->Clear();
	spXMLReader->Read(_variant_t(lpszPathName), m_spModel);
	spXMLReader = NULL;

	return TRUE;
}
