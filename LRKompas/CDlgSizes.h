#pragma once
#include "afxdialogex.h"

#include <vector>

using namespace std;


// Диалоговое окно CDlgSizes

class CDlgSizes : public CDialog
{
	DECLARE_DYNAMIC(CDlgSizes)

public:
	CDlgSizes(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CDlgSizes();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SIZES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;						//что бы в ListCtrl появились столбцы надо зайти в его свойства и в параметре Вид выбрать Отчет
	CComboBox m_ComboBox;						//что бы в ComboBox элементы не сортировало по символам, надо в свойствах в параметре Сортировка поставить False
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDlgSizes();
	int selected_ispolnenie_id;					//индекс выбранного исполнения
	void ClearTable();
	vector<vector<int>> arr;
};
