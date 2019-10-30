#if !defined(AFX_SHAPEGENERALPAGE_H__11B8B9BB_ECD8_474B_A273_7D8E187C0B29__INCLUDED_)
#define AFX_SHAPEGENERALPAGE_H__11B8B9BB_ECD8_474B_A273_7D8E187C0B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShapeGeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShapeGeneralPage dialog

class CShapeGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CShapeGeneralPage)

// Construction
public:
	CShapeGeneralPage();
	~CShapeGeneralPage();

// Dialog Data
	//{{AFX_DATA(CShapeGeneralPage)
	enum { IDD = IDD_SHAPE_GENERAL };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CShapeGeneralPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	IShapePtr m_spShape;

// Operations
public:
	void SetNode(IDispatch* pNode);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CShapeGeneralPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPEGENERALPAGE_H__11B8B9BB_ECD8_474B_A273_7D8E187C0B29__INCLUDED_)
