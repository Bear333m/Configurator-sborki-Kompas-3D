#pragma once

#include "afxcview.h"
class CLRKompasDoc;
class CLRKompasView;

// Просмотр CMyTreeView

class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // защищенный конструктор, используемый при динамическом создании
	virtual ~CMyTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	HTREEITEM m_hRoot, m_hBranch1, m_hBranch2, m_hBranch3, m_hBranch4;
	void BuildTree();
	CLRKompasDoc* m_pDoc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	CLRKompasView* pView;
	CString** sizes = new CString*[5];
	int num_columns[5];		//количество столбцов
	CString text_ispolneniy[5];
	
};


