// BasicShapesDoc.h : interface of the CBasicShapesDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICSHAPESDOC_H__E69BAC28_19F0_4415_AFF6_8CA87A423681__INCLUDED_)
#define AFX_BASICSHAPESDOC_H__E69BAC28_19F0_4415_AFF6_8CA87A423681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBasicShapesDoc : public CDocument
{
protected: // create from serialization only
	CBasicShapesDoc();
	DECLARE_DYNCREATE(CBasicShapesDoc)

// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
// BEGIN ShapeShifter
	IModelPtr m_spModel;
// END ShapeShifter
/////////////////////////////////////////////////////////////////////////////

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicShapesDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBasicShapesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBasicShapesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICSHAPESDOC_H__E69BAC28_19F0_4415_AFF6_8CA87A423681__INCLUDED_)
