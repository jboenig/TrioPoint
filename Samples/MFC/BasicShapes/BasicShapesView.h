// BasicShapesView.h : interface of the CBasicShapesView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICSHAPESVIEW_H__6CB6380D_4148_4040_89FD_FEB06A018AB0__INCLUDED_)
#define AFX_BASICSHAPESVIEW_H__6CB6380D_4148_4040_89FD_FEB06A018AB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBasicShapesView : public CView
{
protected: // create from serialization only
	CBasicShapesView();
	DECLARE_DYNCREATE(CBasicShapesView)

// Attributes
public:
	CBasicShapesDoc* GetDocument();

/////////////////////////////////////////////////////////////////////////////
// BEGIN ShapeShifter
	IViewportPtr m_spViewport;
	IControllerPtr m_spController;
	IWndMessageReceiverPtr m_spMessageRcvr;
	IMeshEditorPtr m_spMeshEditor;
	DWORD m_dwCookie;
// END ShapeShifter
/////////////////////////////////////////////////////////////////////////////

// Operations
public:
/////////////////////////////////////////////////////////////////////////////
// BEGIN ShapeShifter
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
// END ShapeShifter
/////////////////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicShapesView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBasicShapesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBasicShapesView)
	afx_msg void OnSelectTool();
	afx_msg void OnInsertCube();
	afx_msg void OnInsertPyramid();
	afx_msg void OnInsertCylinder();
	afx_msg void OnInsertSphere();
	afx_msg void OnInsertCone();
	afx_msg void OnViewAxes();
	afx_msg void OnUpdateViewAxes(CCmdUI* pCmdUI);
	afx_msg void OnMoveTool();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnViewFront();
	afx_msg void OnViewBack();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewBottom();
	afx_msg void OnEditRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(PoohBear)
	afx_msg void OnNodeDblClick(IDispatch* pNode);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

#ifndef _DEBUG  // debug version in BasicShapesView.cpp
inline CBasicShapesDoc* CBasicShapesView::GetDocument()
   { return (CBasicShapesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICSHAPESVIEW_H__6CB6380D_4148_4040_89FD_FEB06A018AB0__INCLUDED_)
