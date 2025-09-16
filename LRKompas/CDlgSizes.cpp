// CDlgSizes.cpp: файл реализации
//

#include "pch.h"
#include "LRKompas.h"
#include "afxdialogex.h"
#include "CDlgSizes.h"

#include "MainFrm.h"
#include "LRKompasDoc.h"
#include "LRKompasView.h"
//#include <vector>

//using namespace std;


// Диалоговое окно CDlgSizes

IMPLEMENT_DYNAMIC(CDlgSizes, CDialog)

CDlgSizes::CDlgSizes(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_SIZES, pParent)
{

}

CDlgSizes::~CDlgSizes()
{
}

void CDlgSizes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLG_SIZES, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_DLG_SIZES, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CDlgSizes, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DLG_SIZES, &CDlgSizes::OnSelchangeComboDlgSizes)
END_MESSAGE_MAP()


// Обработчики сообщений CDlgSizes


BOOL CDlgSizes::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CLRKompasDoc* pDoc = (CLRKompasDoc*)pMainFrame->GetActiveDocument();
	CLRKompasView* pView = pDoc->m_pView;

	int selected_detal = pView->selected_detal;
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Очистка двумерного вектора
	for (auto& row : arr) {
		row.clear();		 // Очищаем каждый внутренний вектор
	}
	arr.clear();			 // Очищаем основной вектор

	switch (selected_detal)
	{
	case 0:
	{
		m_ListCtrl.InsertColumn(0, L"d", LVCFMT_LEFT, 100);				//4-ый аргумент это ширина столбца
		m_ListCtrl.InsertColumn(1, L"M", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(2, L"D", LVCFMT_LEFT, 100);				//также используем его для D нажимной гайки
		m_ListCtrl.InsertColumn(3, L"L", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(4, L"d шайбы", LVCFMT_LEFT, 100);		//сначала шайба, потому что так указано в доках
		m_ListCtrl.InsertColumn(5, L"D шайбы", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(6, L"s шайбы", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(7, L"l гнезда", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(8, L"L гнезда", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(9, L"s гайки нажимной", LVCFMT_LEFT, 150);
		m_ListCtrl.InsertColumn(10, L"l гайки нажимной", LVCFMT_LEFT, 150);
		m_ListCtrl.InsertColumn(11, L"h гайки привертной", LVCFMT_LEFT, 150);
		arr.push_back({ 10, 18, 32, 92, 10, 14, 2, 36, 80, 22, 32, 10 });
		arr.push_back({ 45, 64, 88, 133, 40, 58, 3, 42, 108, 70, 62, 15 });
		arr.push_back({ 80, 100, 125, 151, 72, 93, 3, 45, 117, 105, 71, 18 });
		break;
	}
	case 1:		//гнездо
	{
		m_ListCtrl.InsertColumn(0, L"d", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(1, L"M", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(2, L"D", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(3, L"l", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(4, L"L", LVCFMT_LEFT, 100);
		arr.push_back({ 10, 18, 26, 36, 73 });
		arr.push_back({ 45, 64, 82, 42, 108 });
		arr.push_back({ 80, 100, 125, 45, 117 });
		break;
	}
	case 2:		//гайка нажимная
	{
		m_ListCtrl.InsertColumn(0, L"d", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(1, L"M", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(2, L"D", LVCFMT_LEFT, 100);		//взял размеры D1
		m_ListCtrl.InsertColumn(3, L"s", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(4, L"l", LVCFMT_LEFT, 100);
		arr.push_back({ 10, 18, 27, 22, 32 });
		arr.push_back({ 45, 64, 82, 70, 62 });
		arr.push_back({ 80, 100, 123, 105, 71 });
		break;
	}
	case 3:		//гнездо привертная
	{
		m_ListCtrl.InsertColumn(0, L"M", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(1, L"D", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(2, L"h", LVCFMT_LEFT, 100);
		arr.push_back({ 18, 32, 10 });
		arr.push_back({ 48, 68, 12 });
		arr.push_back({ 100, 125, 18 });
		break;
	}
	case 4:		//шайбы
	{
		m_ListCtrl.InsertColumn(0, L"d", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(1, L"D", LVCFMT_LEFT, 100);
		m_ListCtrl.InsertColumn(2, L"s", LVCFMT_LEFT, 100);
		//int arr[3][3] = {
		arr.push_back({ 10, 14, 2 });
		arr.push_back({ 32, 48, 3 });
		arr.push_back({ 70, 84, 3 });
		break;
	}
	default:
	{
	//	return;
	}
	}
	
	
	for (int i = 0; i < arr.size(); i++) {
		CString str;
		str.Format(L"%i", arr[i][0]);						//конвертируем int из вектора в CString
		m_ComboBox.AddString(str);							//добавляем элементы из первого столбца в Комбобокс
		int rowId = m_ListCtrl.InsertItem(i, str, -1);		//индекс строки из массива (которая i)
		for (int j = 1; j < arr[0].size(); j++)
		{
			str.Format(L"%i", arr[i][j]);				//конвертируем int из вектора в CString
			m_ListCtrl.SetItemText(rowId, j, str);		//первый аргумент это индекс строки, а второй индекс строки
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CDlgSizes::ClearTable()
{
	m_ListCtrl.DeleteAllItems();

	CHeaderCtrl* pTableHeader = m_ListCtrl.GetHeaderCtrl();
	if (pTableHeader != NULL) {
		int columnsCount = pTableHeader->GetItemCount();
		for (int i = 0; i < columnsCount; i++)
		{
			m_ListCtrl.DeleteColumn(0);
		}
	}
}


void CDlgSizes::OnSelchangeComboDlgSizes()
{
	// TODO: добавьте свой код обработчика уведомлений
	selected_ispolnenie_id = m_ComboBox.GetCurSel();		//получаем индекс выбранного исполнения
}
