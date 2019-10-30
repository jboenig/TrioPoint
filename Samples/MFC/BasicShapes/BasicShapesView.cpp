// BasicShapesView.cpp : implementation of the CBasicShapesView class
//

#include "stdafx.h"
#include "BasicShapes.h"

#include "BasicShapesDoc.h"
#include "BasicShapesView.h"
#include "ShapePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_RANGE  6.0f

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesView

IMPLEMENT_DYNCREATE(CBasicShapesView, CView)

BEGIN_MESSAGE_MAP(CBasicShapesView, CView)
	//{{AFX_MSG_MAP(CBasicShapesView)
	ON_COMMAND(ID_SELECT_TOOL, OnSelectTool)
	ON_COMMAND(ID_INSERT_CUBE, OnInsertCube)
	ON_COMMAND(ID_INSERT_PYRAMID, OnInsertPyramid)
	ON_COMMAND(ID_INSERT_CYLINDER, OnInsertCylinder)
	ON_COMMAND(ID_INSERT_SPHERE, OnInsertSphere)
	ON_COMMAND(ID_INSERT_CONE, OnInsertCone)
	ON_COMMAND(ID_VIEW_AXES, OnViewAxes)
	ON_UPDATE_COMMAND_UI(ID_VIEW_AXES, OnUpdateViewAxes)
	ON_COMMAND(ID_MOVE_TOOL, OnMoveTool)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_COMMAND(ID_VIEW_BACK, OnViewBack)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_COMMAND(ID_VIEW_BOTTOM, OnViewBottom)
	ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CBasicShapesView, CView)
	INTERFACE_PART(CBasicShapesView, __uuidof(_IControllerEvents), Dispatch)
END_INTERFACE_MAP()

BEGIN_DISPATCH_MAP(CBasicShapesView, CView)
	//{{AFX_DISPATCH_MAP(CBasicShapesView)
	DISP_FUNCTION_ID(CBasicShapesView, "OnNodeDblClick", SSCLID_ON_NODE_DBLCLICK, OnNodeDblClick, VT_EMPTY, VTS_DISPATCH)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesView construction/destruction

CBasicShapesView::CBasicShapesView()
{
	EnableAutomation();
}

CBasicShapesView::~CBasicShapesView()
{
}

BOOL CBasicShapesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesView drawing

void CBasicShapesView::OnDraw(CDC* pDC)
{
	CBasicShapesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	/////////////////////////////////////////////////////////////////////////
	// BEGIN ShapeShifter
	m_spViewport->RefreshScreen();
	// END ShapeShifter
	/////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesView diagnostics

#ifdef _DEBUG
void CBasicShapesView::AssertValid() const
{
	CView::AssertValid();
}

void CBasicShapesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBasicShapesDoc* CBasicShapesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBasicShapesDoc)));
	return (CBasicShapesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesView message handlers

void CBasicShapesView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	/////////////////////////////////////////////////////////////////////////
	// BEGIN ShapeShifter

	// Create device
	IEnginePtr spEngine = ((CBasicShapesApp*)AfxGetApp())->m_spEngine;
	IDevicePtr spDevice = ((CBasicShapesApp*)AfxGetApp())->m_spDevice;

	// Create ShapeShifter viewport
	m_spViewport = spDevice->CreateViewport(_bstr_t("ShapeShifter.Viewport"));

	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	m_spViewport->SetBounds(&rc);
	FLOAT fAspect = ((FLOAT) rc.right - (FLOAT) rc.left) / ((FLOAT) rc.bottom - (FLOAT) rc.top);

	// Assign window handle to the viewport
	m_spViewport->ContainerWindow = (long) m_hWnd;

	// Set the camera position and direction
	m_spViewport->Camera->SetLocation(0.0f, 0.0f, -8.0f);
	m_spViewport->Camera->SetAimVector(0.0f, 0.0f, 1.0f);

	// Set the viewing volume
	m_spViewport->SetFrustum(1.57f, fAspect, 1.0f, 1000.0f);

	// Enable Z buffering
	m_spViewport->PutProperty(SSCLID_ZENABLE, _variant_t((LONG) TRUE));

	// Attach the model to the viewport
	m_spViewport->Model = GetDocument()->m_spModel;

	spDevice->Initialize((long) m_hWnd);

	GetDocument()->m_spModel->LoadDevice(spDevice);

	m_spViewport->Render();

	// Create a controller and attach the viewport to it
	m_spController.CreateInstance(__uuidof(BrowseEditController));
	m_spController->Viewport = m_spViewport;
	m_spMessageRcvr = m_spController;

	// Create a mesh editor
	m_spMeshEditor.CreateInstance(__uuidof(MeshEditor));

	BOOL bSuccess = AfxConnectionAdvise(m_spController, __uuidof(_IControllerEvents), GetIDispatch(TRUE), FALSE, &m_dwCookie);

	// END ShapeShifter
	/////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////
// BEGIN ShapeShifter
BOOL CBasicShapesView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_spMessageRcvr != NULL)
	{
		m_spMessageRcvr->ProcessWindowMessage(message, wParam, lParam);
	}
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
// END ShapeShifter
/////////////////////////////////////////////////////////////////////////

void CBasicShapesView::OnSelectTool() 
{
	// TODO: Add your command handler code here
	
}

void CBasicShapesView::OnInsertCube() 
{
	IModelPtr spModel = GetDocument()->m_spModel;

	IShapePtr spShape;

	spShape.CreateInstance(__uuidof(CompositeShape));

	m_spMeshEditor->BeginEdit(spShape->Mesh);
	m_spMeshEditor->MakeCube(3, 3, 3);
	m_spMeshEditor->EndEdit();

	spShape->put_Name(_bstr_t("Cube"));  // Model will generate a unique name

	IMaterialPtr pDefaultMaterial = spModel->GetChildByName(_bstr_t("Default Material"));
	IRenderingContextPtr spContext = spShape->GetContext(0);
	spContext->AppendChild(pDefaultMaterial);

	IInsertDeleteNodesPtr spIns = m_spController->GetBehavior("InsertDeleteNodes");
	spIns->InsertNode(spShape, _bstr_t(""));
}

void CBasicShapesView::OnInsertPyramid() 
{
	IModelPtr spModel = GetDocument()->m_spModel;

	IShapePtr spShape;

	spShape.CreateInstance(__uuidof(CompositeShape));

	m_spMeshEditor->BeginEdit(spShape->Mesh);
	m_spMeshEditor->MakePyramid(3, 3);
	m_spMeshEditor->EndEdit();

	spShape->put_Name(_bstr_t("Pyramid"));  // Model will generate a unique name

	IMaterialPtr pDefaultMaterial = spModel->GetChildByName(_bstr_t("Default Material"));
	IRenderingContextPtr spContext = spShape->GetContext(0);
	spContext->AppendChild(pDefaultMaterial);

	IInsertDeleteNodesPtr spIns = m_spController->GetBehavior("InsertDeleteNodes");
	spIns->InsertNode(spShape, _bstr_t(""));
}

void CBasicShapesView::OnInsertCylinder() 
{
	IModelPtr spModel = GetDocument()->m_spModel;

	IShapePtr spShape;

	spShape.CreateInstance(__uuidof(CompositeShape));

	m_spMeshEditor->BeginEdit(spShape->Mesh);
	m_spMeshEditor->MakeCylinder(1.5, 4, 32);
	m_spMeshEditor->EndEdit();

	spShape->put_Name(_bstr_t("Cylinder"));  // Model will generate a unique name

	IMaterialPtr pDefaultMaterial = spModel->GetChildByName(_bstr_t("Default Material"));
	IRenderingContextPtr spContext = spShape->GetContext(0);
	spContext->AppendChild(pDefaultMaterial);
	spContext = spShape->GetContext(1);
	spContext->AppendChild(pDefaultMaterial);
	spContext = spShape->GetContext(2);
	spContext->AppendChild(pDefaultMaterial);

	IInsertDeleteNodesPtr spIns = m_spController->GetBehavior("InsertDeleteNodes");
	spIns->InsertNode(spShape, _bstr_t(""));
}

void CBasicShapesView::OnInsertSphere() 
{
	IModelPtr spModel = GetDocument()->m_spModel;

	IShapePtr spShape;

	spShape.CreateInstance(__uuidof(CompositeShape));

	m_spMeshEditor->BeginEdit(spShape->Mesh);
	m_spMeshEditor->MakeSphere(1.5, 32, 32);
	m_spMeshEditor->EndEdit();

	spShape->put_Name(_bstr_t("Sphere"));  // Model will generate a unique name

	IMaterialPtr pDefaultMaterial = spModel->GetChildByName(_bstr_t("Default Material"));
	IRenderingContextPtr spContext = spShape->GetContext(0);
	spContext->AppendChild(pDefaultMaterial);

	IInsertDeleteNodesPtr spIns = m_spController->GetBehavior("InsertDeleteNodes");
	spIns->InsertNode(spShape, _bstr_t(""));
}

void CBasicShapesView::OnInsertCone() 
{
	IModelPtr spModel = GetDocument()->m_spModel;

	IShapePtr spShape;

	spShape.CreateInstance(__uuidof(CompositeShape));

	m_spMeshEditor->BeginEdit(spShape->Mesh);
	m_spMeshEditor->MakeCone(1.5, 4, 32);
	m_spMeshEditor->EndEdit();

	spShape->put_Name(_bstr_t("Cone"));  // Model will generate a unique name

	IMaterialPtr pDefaultMaterial = spModel->GetChildByName(_bstr_t("Default Material"));
	IRenderingContextPtr spContext = spShape->GetContext(0);
	spContext->AppendChild(pDefaultMaterial);
	spContext = spShape->GetContext(1);
	spContext->AppendChild(pDefaultMaterial);
	spContext = spShape->GetContext(2);
	spContext->AppendChild(pDefaultMaterial);

	IInsertDeleteNodesPtr spIns = m_spController->GetBehavior("InsertDeleteNodes");
	spIns->InsertNode(spShape, _bstr_t(""));
}

void CBasicShapesView::OnViewAxes() 
{
	IVisualPtr spAxisGuide = m_spViewport->GetChildByName(_bstr_t("AxisGuide"));

	if (spAxisGuide != NULL)
	{
		if (spAxisGuide->Visible)
		{
			spAxisGuide->Visible = FALSE;
		}
		else
		{
			spAxisGuide->Visible = TRUE;
		}

		m_spViewport->Render();
		m_spViewport->RefreshScreen();
	}
}

void CBasicShapesView::OnUpdateViewAxes(CCmdUI* pCmdUI) 
{
	IVisualPtr spAxisGuide = m_spViewport->GetChildByName(_bstr_t("AxisGuide"));
	if (spAxisGuide != NULL)
	{
		pCmdUI->SetCheck(spAxisGuide->Visible);
	}
}

void CBasicShapesView::OnMoveTool() 
{
	IMoveNodesPtr spMove = m_spController->GetBehavior("DragDrop");
	spMove->BeginMove();	
}

void CBasicShapesView::OnNodeDblClick(IDispatch* pNode)
{
	CShapePropertySheet* pPropSheet = new CShapePropertySheet;

	pPropSheet->SetNode(pNode);

	if (pPropSheet->DoModal() == IDOK)
	{
	}

	delete pPropSheet;
}

void CBasicShapesView::OnEditUndo() 
{
	try
	{
		m_spController->Undo(1);
	}
	catch (_com_error&) {}
}

void CBasicShapesView::OnEditRedo() 
{
	try
	{
		m_spController->Redo(1);
	}
	catch (_com_error&) {}
}

void CBasicShapesView::OnViewFront() 
{
	m_spController->MoveCamera(SSCL_CAMERA_GOTO_ZAXIS, -DEFAULT_RANGE);
	m_spController->Update(ControllerUpdate::VIEWPORT, NULL);
}

void CBasicShapesView::OnViewBack() 
{
	m_spController->MoveCamera(SSCL_CAMERA_GOTO_ZAXIS, DEFAULT_RANGE);
	m_spController->Update(ControllerUpdate::VIEWPORT, NULL);
}

void CBasicShapesView::OnViewLeft() 
{
	m_spController->MoveCamera(SSCL_CAMERA_GOTO_XAXIS, -DEFAULT_RANGE);
	m_spController->Update(ControllerUpdate::VIEWPORT, NULL);
}

void CBasicShapesView::OnViewRight() 
{
	m_spController->MoveCamera(SSCL_CAMERA_GOTO_XAXIS, DEFAULT_RANGE);
	m_spController->Update(ControllerUpdate::VIEWPORT, NULL);
}

void CBasicShapesView::OnViewTop() 
{
	m_spController->MoveCamera(SSCL_CAMERA_GOTO_YAXIS, DEFAULT_RANGE);
	m_spController->Update(ControllerUpdate::VIEWPORT, NULL);
}

void CBasicShapesView::OnViewBottom() 
{
	m_spController->MoveCamera(SSCL_CAMERA_GOTO_YAXIS, -DEFAULT_RANGE);
	m_spController->Update(ControllerUpdate::VIEWPORT, NULL);
}

void CBasicShapesView::OnEditRotate() 
{
#if 0
	INodes* pSelectedNodes = NULL;
	if (SUCCEEDED(m_spController->get_SelectionList(&pSelectedNodes)))
	{
		IRotateCommandPtr spRotateCmd;
		spRotateCmd.CreateInstance(__uuidof(RotateCommand));
		spRotateCmd->AddNodes(pSelectedNodes);
		spRotateCmd->SetAngle(1.57f);
		spRotateCmd->SetAxis(1.0f, 0.0f, 0.0f);
		pSelectedNodes->Release();
		m_spController->Do(spRotateCmd);
		m_spViewport->Render();
		m_spViewport->RefreshScreen();
	}
#endif
}
