
// LRKompasView.cpp: реализация класса CLRKompasView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "LRKompas.h"
#endif

#include "LRKompasDoc.h"
#include "LRKompasView.h"
#include "MainFrm.h"
#include "MyTreeView.h"

#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants.h"
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants3D.h"
#import "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\lib\kAPI5.tlb"
using namespace Kompas6API5;
KompasObjectPtr pKompasApp5;
ksPartPtr pPart;
ksDocument3DPtr pDoc;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLRKompasView

IMPLEMENT_DYNCREATE(CLRKompasView, CView)

BEGIN_MESSAGE_MAP(CLRKompasView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, &CLRKompasView::OnButtonBuildClicked)
END_MESSAGE_MAP()

// Создание или уничтожение CLRKompasView

CLRKompasView::CLRKompasView() noexcept
{
	// TODO: добавьте код создания

}

CLRKompasView::~CLRKompasView()
{
}

BOOL CLRKompasView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CLRKompasView

void CLRKompasView::OnDraw(CDC* pDC)	//снял комментарии с pDCs
{
	CLRKompasDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных

	CDC dcTemp;
	CBitmap cb;
	if (selected_detal == 0) cb.LoadBitmap(IDB_BITMAP1);
	else if (selected_detal == 1) cb.LoadBitmap(IDB_BITMAP2);
	else if (selected_detal == 2) cb.LoadBitmap(IDB_BITMAP3);
	else if (selected_detal == 3) cb.LoadBitmap(IDB_BITMAP4);
	else if (selected_detal == 4) cb.LoadBitmap(IDB_BITMAP5);
	dcTemp.CreateCompatibleDC(pDC);
	dcTemp.SelectObject(cb);

	pDC->BitBlt(0, 0, 1000, 1000, &dcTemp, 0, 0, SRCCOPY);
	//pDC->TextOut(100, 500, selected_ispolnenie_text);
	CRect rect(100, 480, 500, 700);		// Задаем прямоугольник для вывода текста
	pDC->DrawText(selected_ispolnenie_text, rect, DT_WORDBREAK | DT_LEFT | DT_TOP);

	//CRect rect;
	//GetClientRect(&rect);
	//pButton->MoveWindow(rect.Width() - 150, rect.Height() - 100, rect.Width(), rect.Height());
}

void CLRKompasView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: добавьте специализированный код или вызов базового класса
	CButton* pButton = new CButton();
	pButton->Create(_T("Построить"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(600, 450, 750, 530), this, IDC_BUTTON_BUILD);
}


// Печать CLRKompasView

BOOL CLRKompasView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CLRKompasView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CLRKompasView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CLRKompasView

#ifdef _DEBUG
void CLRKompasView::AssertValid() const
{
	CView::AssertValid();
}

void CLRKompasView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLRKompasDoc* CLRKompasView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLRKompasDoc)));
	return (CLRKompasDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CLRKompasView
void CLRKompasView::ChangeView(int num, CString str) {		//меняем выбранную таблицу, текст выбранного исполнения, перезагружаем главный вид
	selected_detal = num;
	selected_ispolnenie_text = str;
	Invalidate();
}



void CLRKompasView::OnButtonBuildClicked()
{
	if (selected_ispolnenie_text == L"Исполнение не выбрано") {
		AfxMessageBox(_T("Выберите исполнение"));
		return;
	}

	BeginWaitCursor();

	CComPtr<IUnknown> pKompasAppUnk = nullptr;
	if (!pKompasApp5)
	{
		// Получаем CLSID для Компас
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Kompas.Application.5", &InvAppClsid);
		if (FAILED(hRes)) {
			pKompasApp5 = nullptr;
			return;
		}

		// Проверяем есть ли запущенный экземпляр Компас
		//если есть получаем IUnknown
		hRes = ::GetActiveObject(InvAppClsid, NULL, &pKompasAppUnk);
		if (FAILED(hRes)) {
			// Приходится запускать Компас самим так как работающего нет
			// Также получаем IUnknown для только что запущенного приложения Компас
			TRACE(L"Could not get hold of an active Inventor, will start a new session\n");
			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pKompasAppUnk);
			if (FAILED(hRes)) {
				pKompasApp5 = nullptr;
				return;
			}
		}

		// Получаем интерфейс приложения Компас
		hRes = pKompasAppUnk->QueryInterface(__uuidof(KompasObject), (void**)&pKompasApp5);
		if (FAILED(hRes)) {
			return;
		}
	}

	CMainFrame* pFrame = (CMainFrame * ) AfxGetMainWnd();
	CLRKompasDoc* m_pDoc = (CLRKompasDoc*) pFrame->GetActiveDocument();
	CMyTreeView* pTree = m_pDoc->m_pTreeView;

	pKompasApp5->Visible = true;
	if (selected_detal == 0 || selected_detal == 1) {	//гнездо сальника
		pDoc = pKompasApp5->Document3D();
		pDoc->Create(false, true);
		pPart = pDoc->GetPart(pTop_Part);

		int d, M, D, l, L;
		if (selected_detal == 1) {
			d = _wtoi(pTree->sizes[1][0]);
			M = _wtoi(pTree->sizes[1][1]);
			D = _wtoi(pTree->sizes[1][2]);
			l = _wtoi(pTree->sizes[1][3]);
			L = _wtoi(pTree->sizes[1][4]);
		}
		else {
			d = _wtoi(pTree->sizes[0][0]);
			M = _wtoi(pTree->sizes[0][1]);
			D = _wtoi(pTree->sizes[0][2]);
			l = _wtoi(pTree->sizes[0][7]);
			L = _wtoi(pTree->sizes[0][8]);
		}

		ksEntityPtr pSketchСylinderBig = pPart->NewEntity(o3d_sketch);
		ksSketchDefinitionPtr pSketchDef = pSketchСylinderBig->GetDefinition();
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchСylinderBig->Create();
		ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

		p2DDoc->ksCircle(0, 0, d / 2.f, 1);
		p2DDoc->ksCircle(0, 0, D / 2.f, 1);
		pSketchDef->EndEdit();

		ksEntityPtr pExtrudeСylinderBig = pPart->NewEntity(o3d_bossExtrusion);		//приклеивание выдавливанимем
		ksBossExtrusionDefinitionPtr pExDef = pExtrudeСylinderBig->GetDefinition();
		pExDef->directionType = dtNormal;
		pExDef->SetSketch(pSketchСylinderBig);
		pExDef->SetSideParam(true, etBlind, L - l, 0, false);
		pExtrudeСylinderBig->Create();

		ksEntityPtr pSketchСylinderSmall = pPart->NewEntity(o3d_sketch);
		pSketchDef = pSketchСylinderSmall->GetDefinition();					//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchСylinderSmall->Create();
		p2DDoc = pSketchDef->BeginEdit();									//переменная была объявлена ранее

		p2DDoc->ksCircle(0, 0, d / 2.f, 1);
		p2DDoc->ksCircle(0, 0, M / 2.f, 1);
		pSketchDef->EndEdit();

		ksEntityPtr pExtrudeСylinderSmall = pPart->NewEntity(o3d_bossExtrusion);		//приклеивание выдавливанимем
		pExDef = pExtrudeСylinderSmall->GetDefinition();								//переменная была объявлена ранее
		pExDef->directionType = dtNormal;
		pExDef->SetSketch(pSketchСylinderSmall);
		pExDef->SetSideParam(true, etBlind, L, 0, false);
		pExtrudeСylinderSmall->Create();

		ksEntityPtr pSketchHoleBig = pPart->NewEntity(o3d_sketch);
		pSketchDef = pSketchHoleBig->GetDefinition();					//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchHoleBig->Create();
		p2DDoc = pSketchDef->BeginEdit();								//переменная была объявлена ранее

		p2DDoc->ksCircle(0, 0, M / 2.f, 1);
		pSketchDef->EndEdit();

		ksEntityPtr pCutHoleBig = pPart->NewEntity(o3d_cutExtrusion);
		ksCutExtrusionDefinitionPtr pExDefCut = pCutHoleBig->GetDefinition();		//создаём объект вырезания
		pExDefCut->directionType = dtReverse;		//сделали обратное направление
		pExDefCut->SetSketch(pSketchHoleBig);
		pExDefCut->SetSideParam(false, etBlind, 4 * (L - l) / 5.f, 0, false);		//тут тоже сделали обратное направление
		pCutHoleBig->Create();

		ksEntityPtr filletEntity = pPart->NewEntity(o3d_fillet);			//скругление
		ksFilletDefinitionPtr filletDef = filletEntity->GetDefinition();

		filletDef->radius = 1.f;
		ksEntityCollectionPtr EdgeCollection = filletDef->array();	//создаём массив рёбер
		EdgeCollection->Clear();
		ksEntityCollectionPtr edges = pPart->EntityCollection(o3d_edge);
		edges->SelectByPoint(-(L - l), 0, D / 2.f);
		EdgeCollection->Add(edges->GetByIndex(0));
		filletEntity->Create();

		ksEntityPtr pSketchCutouts = pPart->NewEntity(o3d_sketch);
		pSketchDef = pSketchCutouts->GetDefinition();		//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchCutouts->Create();
		p2DDoc = pSketchDef->BeginEdit();		//переменная была объявлена ранее

		p2DDoc->ksLineSeg(D / 10.f, D / 2.f, -D / 10.f, D / 2.f, 1);						//верх
		p2DDoc->ksLineSeg(D / 10.f, D / 2.f, D / 10.f, 0, 1);
		p2DDoc->ksLineSeg(-D / 10.f, D / 2.f, -D / 10.f, 0, 1);
		p2DDoc->ksLineSeg(D / 10.f, 0, -D / 10.f, 0, 1);	//низ
		pSketchDef->EndEdit();

		ksEntityPtr pCutCutouts = pPart->NewEntity(o3d_cutExtrusion);
		pExDefCut = pCutCutouts->GetDefinition();		//переменная была объявлена ранее
		pExDefCut->directionType = dtReverse;			//сделали обратное направление
		pExDefCut->SetSketch(pSketchCutouts);
		pExDefCut->SetSideParam(false, etBlind, L / 15.f, 0, false);		//тут тоже сделали обратное направление
		pCutCutouts->Create();

		ksEntityPtr EntityPlaneXOY = pPart->GetDefaultEntity(o3d_planeXOY);  //в языке Delphi наверно сначала имя переменной, а потом тип 
		ksEntityPtr EntityPlaneXOZ = pPart->GetDefaultEntity(o3d_planeXOZ);  //дописал к Part p, получилось pPart 

		ksEntityPtr Axis2Planes = pPart->NewEntity(o3d_axis2Planes);
		ksAxis2PlanesDefinitionPtr ksAxis2PlanesDefinition = Axis2Planes->GetDefinition();
		ksAxis2PlanesDefinition->SetPlane(1, EntityPlaneXOY);
		ksAxis2PlanesDefinition->SetPlane(2, EntityPlaneXOZ);
		Axis2Planes->Create();

		ksEntityPtr pCirCopy = pPart->NewEntity(o3d_circularCopy);				//круговой массив
		ksCircularCopyDefinitionPtr CirCopDef = pCirCopy->GetDefinition();
		CirCopDef->SetAxis(Axis2Planes);
		CirCopDef->count2 = 4;
		CirCopDef->factor2 = FALSE;
		CirCopDef->step2 = 90;
		ksEntityCollectionPtr CirColl = CirCopDef->GetOperationArray();   //создал массив к которому привязал массив для зеркалирования 
		CirColl->Clear();
		CirColl->Add(pCutCutouts);
		pCirCopy->Create();

		pDoc->SaveAs("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 1.m3d");
		if (selected_detal == 0) pDoc->close();
	}
	if (selected_detal == 0 || selected_detal == 2) {	//гайка нажимная
		pDoc = pKompasApp5->Document3D();
		pDoc->Create(false, true);
		pPart = pDoc->GetPart(pTop_Part);

		int d, M, D, s, l;
		if (selected_detal == 2) {
			d = _wtoi(pTree->sizes[2][0]);
			M = _wtoi(pTree->sizes[2][1]);
			D = _wtoi(pTree->sizes[2][2]);
			s = _wtoi(pTree->sizes[2][3]);
			l = _wtoi(pTree->sizes[2][4]);
		}
		else {
			d = _wtoi(pTree->sizes[0][0]);
			M = _wtoi(pTree->sizes[0][1]);
			D = _wtoi(pTree->sizes[0][2]);
			s = _wtoi(pTree->sizes[0][9]);
			l = _wtoi(pTree->sizes[0][10]);
		}

		ksEntityPtr pSketchСylinderBig = pPart->NewEntity(o3d_sketch);
		ksSketchDefinitionPtr pSketchDef = pSketchСylinderBig->GetDefinition();
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchСylinderBig->Create();
		ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

		p2DDoc->ksCircle(0, 0, d / 2.f, 1);
		p2DDoc->ksCircle(0, 0, D / 2.f, 1);
		pSketchDef->EndEdit();

		ksEntityPtr pExtrudeСylinderBig = pPart->NewEntity(o3d_bossExtrusion);		//приклеивание выдавливанимем
		ksBossExtrusionDefinitionPtr pExDef = pExtrudeСylinderBig->GetDefinition();
		pExDef->directionType = dtNormal;
		pExDef->SetSketch(pSketchСylinderBig);
		pExDef->SetSideParam(true, etBlind, l / 3.f, 0, false);
		pExtrudeСylinderBig->Create();

		ksEntityPtr pSketchСylinderSmall = pPart->NewEntity(o3d_sketch);
		pSketchDef = pSketchСylinderSmall->GetDefinition();					//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchСylinderSmall->Create();
		p2DDoc = pSketchDef->BeginEdit();									//переменная была объявлена ранее

		p2DDoc->ksCircle(0, 0, d / 2.f, 1);
		p2DDoc->ksCircle(0, 0, M / 2.f, 1);
		pSketchDef->EndEdit();

		ksEntityPtr pExtrudeСylinderSmall = pPart->NewEntity(o3d_bossExtrusion);		//приклеивание выдавливанимем
		pExDef = pExtrudeСylinderSmall->GetDefinition();								//переменная была объявлена ранее
		pExDef->directionType = dtNormal;
		pExDef->SetSketch(pSketchСylinderSmall);
		pExDef->SetSideParam(true, etBlind, l, 0, false);
		pExtrudeСylinderSmall->Create();

		ksEntityPtr chamferEntity = pPart->NewEntity(o3d_chamfer);			//фаска
		ksChamferDefinitionPtr chamferDef = chamferEntity->GetDefinition();

		chamferDef->SetChamferParam(TRUE, 2, 2);
		ksEntityCollectionPtr EdgeCollection = chamferDef->array();	//создаём массив рёбер
		EdgeCollection->Clear();
		ksEntityCollectionPtr edges = pPart->EntityCollection(o3d_edge);
		edges->SelectByPoint(0, 0, d / 2.f);
		EdgeCollection->Add(edges->GetByIndex(0));
		chamferEntity->Create();

		ksEntityPtr pSketchHexagon = pPart->NewEntity(o3d_sketch);
		pSketchDef = pSketchHexagon->GetDefinition();					//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchHexagon->Create();
		p2DDoc = pSketchDef->BeginEdit();								//переменная была объявлена ранее

		p2DDoc->ksLineSeg(D, s / 2.f, -D, s / 2.f, 1);		//низ
		p2DDoc->ksLineSeg(D, s / 2.f, D, D, 1);
		p2DDoc->ksLineSeg(-D, s / 2.f, -D, D, 1);
		p2DDoc->ksLineSeg(D, D, -D, D, 1);					//верх
		pSketchDef->EndEdit();

		ksEntityPtr pCutouts = pPart->NewEntity(o3d_cutExtrusion);
		ksCutExtrusionDefinitionPtr pExDefCut = pCutouts->GetDefinition();		//создаём объект вырезания
		pExDefCut->directionType = dtReverse;		//сделали обратное направление
		pExDefCut->SetSketch(pSketchHexagon);
		pExDefCut->SetSideParam(false, etBlind, l / 4.f, 0, false);		//тут тоже сделали обратное направление
		pCutouts->Create();

		ksEntityPtr EntityPlaneXOY = pPart->GetDefaultEntity(o3d_planeXOY);  //в языке Delphi наверно сначала имя переменной, а потом тип 
		ksEntityPtr EntityPlaneXOZ = pPart->GetDefaultEntity(o3d_planeXOZ);  //дописал к Part p, получилось pPart 

		ksEntityPtr Axis2Planes = pPart->NewEntity(o3d_axis2Planes);
		ksAxis2PlanesDefinitionPtr ksAxis2PlanesDefinition = Axis2Planes->GetDefinition();
		ksAxis2PlanesDefinition->SetPlane(1, EntityPlaneXOY);
		ksAxis2PlanesDefinition->SetPlane(2, EntityPlaneXOZ);
		Axis2Planes->Create();

		ksEntityPtr pCirCopy = pPart->NewEntity(o3d_circularCopy);				//круговой массив
		ksCircularCopyDefinitionPtr CirCopDef = pCirCopy->GetDefinition();
		CirCopDef->SetAxis(Axis2Planes);
		CirCopDef->count2 = 6;
		CirCopDef->factor2 = FALSE;
		CirCopDef->step2 = 60;
		ksEntityCollectionPtr CirColl = CirCopDef->GetOperationArray();   //создал массив к которому привязал массив для зеркалирования 
		CirColl->Clear();
		CirColl->Add(pCutouts);
		pCirCopy->Create();

		pDoc->SaveAs("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 2.m3d");
		if (selected_detal == 0) pDoc->close();
	}
	if (selected_detal == 0 || selected_detal == 3) {	//гайка привертная
		pDoc = pKompasApp5->Document3D();
		pDoc->Create(false, true);
		pPart = pDoc->GetPart(pTop_Part);

		int M, D, h;
		if (selected_detal == 3) {
			M = _wtoi(pTree->sizes[3][0]);
			D = _wtoi(pTree->sizes[3][1]);
			h = _wtoi(pTree->sizes[3][2]);
		}
		else {
			M = _wtoi(pTree->sizes[0][1]);
			D = _wtoi(pTree->sizes[0][2]);
			h = _wtoi(pTree->sizes[0][11]);
		}

		ksEntityPtr pSketchNutPrivert = pPart->NewEntity(o3d_sketch);
		ksSketchDefinitionPtr pSketchDef = pSketchNutPrivert->GetDefinition();
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchNutPrivert->Create();
		ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

		p2DDoc->ksCircle(0, 0, M / 2.f, 1);
		p2DDoc->ksCircle(0, 0, D / 2.f, 1);
		pSketchDef->EndEdit();

		ksEntityPtr pExtrudeNutPrivert = pPart->NewEntity(o3d_bossExtrusion);		//приклеивание выдавливанимем
		ksBossExtrusionDefinitionPtr pExDef = pExtrudeNutPrivert->GetDefinition();
		pExDef->directionType = dtReverse;					//сделали обратное направление
		pExDef->SetSketch(pSketchNutPrivert);
		pExDef->SetSideParam(false, etBlind, h, 0, false);	//тут тоже сделали обратное направление
		pExtrudeNutPrivert->Create();

		ksEntityPtr chamferEntity = pPart->NewEntity(o3d_chamfer);			//фаска
		ksChamferDefinitionPtr chamferDef = chamferEntity->GetDefinition();

		chamferDef->SetChamferParam(TRUE, 2, 2);
		ksEntityCollectionPtr EdgeCollection = chamferDef->array();	//создаём массив рёбер
		EdgeCollection->Clear();
		ksEntityCollectionPtr edges = pPart->EntityCollection(o3d_edge);
		edges->SelectByPoint(-h, 0, M / 2.f);
		EdgeCollection->Add(edges->GetByIndex(0));

		chamferEntity->Create();

		ksEntityPtr pSketchCutouts = pPart->NewEntity(o3d_sketch);
		pSketchDef = pSketchCutouts->GetDefinition();		//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchCutouts->Create();
		p2DDoc = pSketchDef->BeginEdit();		//переменная была объявлена ранее

		p2DDoc->ksLineSeg(D / 8.f, D / 2.f, - D / 8.f, D / 2.f, 1);						//верх
		p2DDoc->ksLineSeg(D / 8.f, D / 2.f, D / 8.f, ((M + (D - M) / 2.f) / 2.f), 1);
		p2DDoc->ksLineSeg(-D / 8.f, D / 2.f, -D / 8.f, ((M + (D - M) / 2.f) / 2.f), 1);
		p2DDoc->ksLineSeg(D / 8.f, ((M + (D - M) / 2.f) / 2.f), -D / 8.f, ((M + (D - M) / 2.f) / 2.f), 1);	//низ
		pSketchDef->EndEdit();

		ksEntityPtr pCutCutouts = pPart->NewEntity(o3d_cutExtrusion);
		ksCutExtrusionDefinitionPtr pExDefCut = pCutCutouts->GetDefinition();		//создаём объект вырезания
		pExDefCut->directionType = dtNormal;
		pExDefCut->SetSketch(pSketchCutouts);
		pExDefCut->SetSideParam(true, etBlind, h / 2.f, 0, false);
		pCutCutouts->Create();

		ksEntityPtr EntityPlaneXOY = pPart->GetDefaultEntity(o3d_planeXOY);  //в языке Delphi наверно сначала имя переменной, а потом тип 
		ksEntityPtr EntityPlaneXOZ = pPart->GetDefaultEntity(o3d_planeXOZ);  //дописал к Part p, получилось pPart 

		ksEntityPtr Axis2Planes = pPart->NewEntity(o3d_axis2Planes);
		ksAxis2PlanesDefinitionPtr ksAxis2PlanesDefinition = Axis2Planes->GetDefinition();
		ksAxis2PlanesDefinition->SetPlane(1, EntityPlaneXOY);
		ksAxis2PlanesDefinition->SetPlane(2, EntityPlaneXOZ);
		Axis2Planes->Create();

		ksEntityPtr pCirCopy = pPart->NewEntity(o3d_circularCopy);				//круговой массив
		ksCircularCopyDefinitionPtr CirCopDef = pCirCopy->GetDefinition();
		CirCopDef->SetAxis(Axis2Planes);
		CirCopDef->count2 = 4;
		CirCopDef->factor2 = FALSE;
		CirCopDef->step2 = 90;
		ksEntityCollectionPtr CirColl = CirCopDef->GetOperationArray();   //создал массив к которому привязал массив для зеркалирования 
		CirColl->Clear();
		CirColl->Add(pCutCutouts);
		pCirCopy->Create();

		pDoc->SaveAs("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 3.m3d");
		if (selected_detal == 0) pDoc->close();
	}
	if (selected_detal == 0 || selected_detal == 4) {	//шайба
		pDoc = pKompasApp5->Document3D();
		pDoc->Create(false, true);
		pPart = pDoc->GetPart(pTop_Part);
		
		int d, D, s;
		if (selected_detal == 4) {
			d = _wtoi(pTree->sizes[4][0]);
			D = _wtoi(pTree->sizes[4][1]);
			s = _wtoi(pTree->sizes[4][2]);
		}
		else {
			d = _wtoi(pTree->sizes[0][4]);
			D = _wtoi(pTree->sizes[0][5]);
			s = _wtoi(pTree->sizes[0][6]);
		}

		ksEntityPtr pSketchWasher = pPart->NewEntity(o3d_sketch);
		ksSketchDefinitionPtr pSketchDef = pSketchWasher->GetDefinition();		//переменная была объявлена ранее
		pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeYOZ));
		pSketchWasher->Create();
		ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

		p2DDoc->ksCircle(0, 0, d / 2.f, 1);
		p2DDoc->ksCircle(0, 0, D / 2.f, 1);

		pSketchDef->EndEdit();

		ksEntityPtr pExtrudeWasher = pPart->NewEntity(o3d_bossExtrusion);		//приклеивание выдавливанимем
		ksBossExtrusionDefinitionPtr pExDef = pExtrudeWasher->GetDefinition();
		pExDef->directionType = dtNormal;
		pExDef->SetSketch(pSketchWasher);
		pExDef->SetSideParam(true, etBlind, s, 0, false);
		pExtrudeWasher->Create();

		pDoc->SaveAs("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 4.m3d");
		if (selected_detal == 0) pDoc->close();
	}
	if (selected_detal == 0) {
		pDoc = pKompasApp5->Document3D();
		pDoc->Create(false, false);
		pPart = pDoc->GetPart(pTop_Part);

		int d = _wtoi(pTree->sizes[0][0]);
		int M = _wtoi(pTree->sizes[0][1]);
		int D = _wtoi(pTree->sizes[0][2]);
		int L = _wtoi(pTree->sizes[0][3]);
		int d_shaibi = _wtoi(pTree->sizes[0][4]);
		int D_shaibi = _wtoi(pTree->sizes[0][5]);
		int s_shaibi = _wtoi(pTree->sizes[0][6]);
		int l_salnika = _wtoi(pTree->sizes[0][7]);
		int L_salnika = _wtoi(pTree->sizes[0][8]);
		int s_nut_nazhim = _wtoi(pTree->sizes[0][9]);
		int l_nut_nazhim = _wtoi(pTree->sizes[0][10]);
		int h_nut_privert = _wtoi(pTree->sizes[0][11]);

		ksPartPtr pGnezdo, pNutNazhim, pNutPrivert, pShaiba1, pShaiba2;
		pDoc->SetPartFromFile("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 1.m3d", pPart, true);
		pDoc->SetPartFromFile("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 2.m3d", pPart, true);
		pDoc->SetPartFromFile("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 3.m3d", pPart, true);
		pDoc->SetPartFromFile("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 4.m3d", pPart, true);
		pDoc->SetPartFromFile("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Detail 4.m3d", pPart, true);

		pGnezdo = pDoc->GetPart(0);
		pNutNazhim = pDoc->GetPart(1);
		pNutPrivert = pDoc->GetPart(2);
		pShaiba1 = pDoc->GetPart(3);
		pShaiba2 = pDoc->GetPart(4);

		ksEntityCollectionPtr pGezdoColl, pNutNazhimColl, pNutPrivertColl, pShaibaColl1, pShaibaColl2;
		ksEntityPtr pGnezdoForConcentric, pShaiba1Concentric, pShaiba1Coincidence, pNutNazhimForConcentric, pNutNazhimForCoincidenceShaiba, pShaiba2Concentric, pShaiba2Coincidence, pGnezdoForCoincidence, pNutNazhimForCoincidenceGnezdo, pNutPrivertForConcentric, pNutPrivertForCoincidence;

		pGezdoColl = pGnezdo->EntityCollection(o3d_edge);
		pGezdoColl->SelectByPoint(-4 * (L_salnika - l_salnika) / 5.f, 0, M / 2.f);		//-L_salnika
		pGnezdoForConcentric = pGezdoColl->First();

		pShaibaColl1 = pShaiba1->EntityCollection(o3d_edge);
		pShaibaColl1->SelectByPoint(-s_shaibi, 0, D_shaibi / 2.f);
		pShaiba1Concentric = pShaibaColl1->First();
		pShaibaColl1 = pShaiba1->EntityCollection(o3d_face);
		pShaibaColl1->SelectByPoint(-s_shaibi, 0, D_shaibi / 2.f - 1);
		pShaiba1Coincidence = pShaibaColl1->First();

		pDoc->AddMateConstraint(mc_Concentric, pGnezdoForConcentric, pShaiba1Concentric, 1, 1, 0);
		pDoc->AddMateConstraint(mc_Coincidence, pGnezdoForConcentric, pShaiba1Coincidence, 1, 1, 0);


		pNutNazhimColl = pNutNazhim->EntityCollection(o3d_edge);
		pNutNazhimColl->SelectByPoint(-l_nut_nazhim, 0, M / 2.f);
		pNutNazhimForConcentric = pNutNazhimColl->First();
		pNutNazhimColl = pNutNazhim->EntityCollection(o3d_face);
		pNutNazhimColl->SelectByPoint(-l_nut_nazhim, 0, M / 2.f - 1);
		pNutNazhimForCoincidenceShaiba = pNutNazhimColl->First();

		pShaibaColl2 = pShaiba2->EntityCollection(o3d_edge);
		pShaibaColl2->SelectByPoint(0, 0, D_shaibi / 2.f);
		pShaiba2Concentric = pShaibaColl2->First();
		pShaibaColl2 = pShaiba2->EntityCollection(o3d_face);
		pShaibaColl2->SelectByPoint(0, 0, D_shaibi / 2.f - 1);
		pShaiba2Coincidence = pShaibaColl2->First();

		pDoc->AddMateConstraint(mc_Concentric, pNutNazhimForConcentric, pShaiba2Concentric, 1, 1, 0);
		pDoc->AddMateConstraint(mc_Coincidence, pNutNazhimForCoincidenceShaiba, pShaiba2Coincidence, -1, 1, 0);

		pGezdoColl = pGnezdo->EntityCollection(o3d_face);
		pGezdoColl->SelectByPoint(-L_salnika, 0, M / 2.f - 1);
		pGnezdoForCoincidence = pGezdoColl->First();
		pNutNazhimColl = pNutNazhim->EntityCollection(o3d_face);
		pNutNazhimColl->SelectByPoint(0, 0, d / 2.f + 3);
		pNutNazhimForCoincidenceGnezdo = pNutNazhimColl->First();

		pDoc->AddMateConstraint(mc_Concentric, pNutNazhimForConcentric, pGnezdoForConcentric, 1, 1, 0);
		pDoc->AddMateConstraint(mc_Distance, pNutNazhimForCoincidenceGnezdo, pGnezdoForCoincidence, -1, 1, -L);

		pNutPrivertColl = pNutPrivert->EntityCollection(o3d_edge);
		pNutPrivertColl->SelectByPoint(0, 0, D / 2.f);
		pNutPrivertForConcentric = pNutPrivertColl->First();
		pNutPrivertColl = pNutPrivert->EntityCollection(o3d_face);
		pNutPrivertColl->SelectByPoint(0, 0, M / 2.f + 1);
		pNutPrivertForCoincidence = pNutPrivertColl->First();

		pDoc->AddMateConstraint(mc_Concentric, pNutPrivertForConcentric, pGnezdoForConcentric, 1, 1, 0);
		pDoc->AddMateConstraint(mc_Distance, pNutPrivertForCoincidence, pGnezdoForCoincidence, 1, 1, L / 20.f);

		pDoc->SaveAs("D:\\Projects\\C++\\3 семестр\\курсовая Моя\\сборка\\Assembly.a3d");
	}
}

