
// LRKompasView.h: интерфейс класса CLRKompasView
//

#pragma once


class CLRKompasView : public CView
{
protected: // создать только из сериализации
	CLRKompasView() noexcept;
	DECLARE_DYNCREATE(CLRKompasView)

// Атрибуты
public:
	CLRKompasDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CLRKompasView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

public:
	void ChangeView(int num, CString str);
	int selected_detal = 0;
	CString selected_ispolnenie_text = L"Исполнение не выбрано";	//текст с характеристиками выбранного исполнения для текстового поля на главном виде
	virtual void OnInitialUpdate();
	afx_msg void OnButtonBuildClicked();
};

#ifndef _DEBUG  // версия отладки в LRKompasView.cpp
inline CLRKompasDoc* CLRKompasView::GetDocument() const
   { return reinterpret_cast<CLRKompasDoc*>(m_pDocument); }
#endif

