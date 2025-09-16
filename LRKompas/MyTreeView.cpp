// MyTreeView.cpp: файл реализации
//

#include "pch.h"
#include "LRKompas.h"
#include "MyTreeView.h"

#include "MainFrm.h"
#include "LRKompasDoc.h"
#include "LRKompasView.h"
#include "CDlgSizes.h"

#include <vector>


// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{

}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// Диагностика CMyTreeView

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// Обработчики сообщений CMyTreeView

void CMyTreeView::BuildTree()
{
	CTreeCtrl& tree = GetTreeCtrl();
	tree.DeleteAllItems();

	m_hRoot = tree.InsertItem(L"Сальник пластмассовый типа СКПП", -1, -1, NULL, TVI_FIRST);

	m_hBranch1 = tree.InsertItem(L"Гдездо сальника типа СКПП", -1, -1, m_hRoot, TVI_FIRST);
	m_hBranch2 = tree.InsertItem(L"Гайка нажимная исполнения 3", -1, -1, m_hRoot, TVI_FIRST);
	m_hBranch3 = tree.InsertItem(L"Гайка привертная", -1, -1, m_hRoot, TVI_FIRST);
	m_hBranch4 = tree.InsertItem(L"Шайбы", -1, -1, m_hRoot, TVI_FIRST);

	tree.Expand(m_hRoot, TVE_EXPAND);
}



int CMyTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;	// | TVS_CHECKBOXES;	//мое

	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Добавьте специализированный код создания

	sizes[0] = new CString[12];
	sizes[1] = new CString[5];
	sizes[2] = new CString[5];
	sizes[3] = new CString[3];
	sizes[4] = new CString[3];

	num_columns[0] = 12;		//количество столбцов
	num_columns[1] = 5;
	num_columns[2] = 5;
	num_columns[3] = 3;
	num_columns[4] = 3;

	for (int i = 0; i < 5; i++) text_ispolneniy[i] = L"Исполнение для данной детали не выбрано";

	return 0;
}


void CMyTreeView::OnLButtonDown(UINT nFlags, CPoint point)					//при двойном нажатии тоже срабатывает
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	CTreeView::OnLButtonDown(nFlags, point);

	CTreeCtrl& tree = GetTreeCtrl();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CLRKompasDoc* pDoc = (CLRKompasDoc*)pMainFrame->GetActiveDocument();
	pView = pDoc->m_pView;
	
	CRect rc;
	tree.GetItemRect(m_hRoot, &rc, false);
	if (rc.PtInRect(point)) pView->ChangeView(0, text_ispolneniy[0]);		//меняем содержимое главного вида
	tree.GetItemRect(m_hBranch1, &rc, false);
	if (rc.PtInRect(point)) pView->ChangeView(1, text_ispolneniy[1]);
	tree.GetItemRect(m_hBranch2, &rc, false);
	if (rc.PtInRect(point)) pView->ChangeView(2, text_ispolneniy[2]);
	tree.GetItemRect(m_hBranch3, &rc, false);
	if (rc.PtInRect(point)) pView->ChangeView(3, text_ispolneniy[3]);
	tree.GetItemRect(m_hBranch4, &rc, false);
	if (rc.PtInRect(point)) pView->ChangeView(4, text_ispolneniy[4]);

}


void CMyTreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//заполнение диалога находится в классе диалога
	
	//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//dlg.tree_view = (CMyTreeView*) pMainFrame->GetActiveView();		//dlg.tree = &GetTreeCtrl();

	CDlgSizes dlg;
	if (dlg.DoModal() != IDOK)	//нажата кнопка отличная от ОК (например ОТМЕНА), значит дальнейшие действия не принимаем
		return;


	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CLRKompasDoc* pDoc = (CLRKompasDoc*)pMainFrame->GetActiveDocument();
	pView = pDoc->m_pView;

	int id_detal = pView->selected_detal;		//индекс выбранной детали
	int id_row = dlg.selected_ispolnenie_id;	//индекс выбранного исполнения
	for (int i = 0; i < num_columns[id_detal]; i++) sizes[id_detal][i].Format(L"%i", dlg.arr[id_row][i]);		// = m_ListCtrl.GetItemText(id_row, i);		//строки детали, столбцы значения размеров

	//text_ispolneniy[id_detal] = L"Текущее исполнение: " + sizes[id_detal][0];
	//for (int i = 1; i < num_columns[id_detal]; i++) text_ispolneniy[id_detal] += L", " + sizes[id_detal][i];	//формируем строку с выбранным исполнением
	
	vector<int> text_buffer;
	for (int i = 0; i < num_columns[id_detal]; i++) text_buffer.push_back(_ttoi(sizes[id_detal][i]));
	if (id_detal == 0) text_ispolneniy[id_detal].Format(L"Текущее исполнение: d = %i, M = %i, D = %i, L = %i, d шайбы = %i, D шайбы = %i, s шайбы = %i, l гнезда = %i, L гнезда = %i, s гайки нажимной = %i, l гайки нажимной = %i, h гайки привертной = %i", text_buffer[0], text_buffer[1], text_buffer[2], text_buffer[3], text_buffer[4], text_buffer[5], text_buffer[6], text_buffer[7], text_buffer[8], text_buffer[9], text_buffer[10], text_buffer[11]);
	if (id_detal == 1) text_ispolneniy[id_detal].Format(L"Текущее исполнение: d = %i, M = %i, D = %i, L = %i, l = %i", text_buffer[0], text_buffer[1], text_buffer[2], text_buffer[3], text_buffer[4]);
	if (id_detal == 2) text_ispolneniy[id_detal].Format(L"Текущее исполнение: d = %i, M = %i, D = %i, s = %i, l = %i", text_buffer[0], text_buffer[1], text_buffer[2], text_buffer[3], text_buffer[4]);
	if (id_detal == 3) text_ispolneniy[id_detal].Format(L"Текущее исполнение: M = %i, D = %i, h = %i", text_buffer[0], text_buffer[1], text_buffer[2]);
	if (id_detal == 4) text_ispolneniy[id_detal].Format(L"Текущее исполнение: d = %i, D = %i, s = %i", text_buffer[0], text_buffer[1], text_buffer[2]);
	text_buffer.clear();
	text_buffer.shrink_to_fit();

	pView->ChangeView(id_detal, text_ispolneniy[id_detal]);	//обновляем измененные размеры

	CTreeView::OnLButtonDblClk(nFlags, point);
}
		

/*
		CTreeCtrl& tree = GetTreeCtrl();
	CRect rc;
	tree.GetItemRect(m_hRoot, &rc, false);
	if (rc.PtInRect(point)) {
		//CString d, M, D, L, shaiba_d, shaiba_D;
		for (int i = 0; i < 6; i++) sizes0[i] = dlg.m_ListCtrl.GetItemText(id, i);		//получаем значение из ячейки таблицы
	}
	tree.GetItemRect(m_hBranch1, &rc, false);
	if (rc.PtInRect(point)) {
		//double d, M, D, l, L;
		for (int i = 0; i < 5; i++) sizes1[i] = dlg.m_ListCtrl.GetItemText(id, i);	//sizes1[5] это массив дерева, если понадобится его значения можно будет получить позже
	}
	tree.GetItemRect(m_hBranch2, &rc, false);
	if (rc.PtInRect(point)) {
		//double d, M, D, D1, s, l;
		for (int i = 0; i < 6; i++) sizes2[i] = dlg.m_ListCtrl.GetItemText(id, i);
	}
	tree.GetItemRect(m_hBranch3, &rc, false);
	if (rc.PtInRect(point)) {
		//double M, D, h;
		for (int i = 0; i < 3; i++) sizes3[i] = dlg.m_ListCtrl.GetItemText(id, i);
	}
	tree.GetItemRect(m_hBranch4, &rc, false);
	if (rc.PtInRect(point)) {
		//double d, D, s;
		for (int i = 0; i < 3; i++) sizes4[i] = dlg.m_ListCtrl.GetItemText(id, i);
	}
		*/


		/*d = dlg.m_ListCtrl.GetItemText(id, 0);
		M = dlg.m_ListCtrl.GetItemText(id, 1);
		D = dlg.m_ListCtrl.GetItemText(id, 2);
		L = dlg.m_ListCtrl.GetItemText(id, 3);
		shaiba_d = dlg.m_ListCtrl.GetItemText(id, 4);
		shaiba_D = dlg.m_ListCtrl.GetItemText(id, 5);*/