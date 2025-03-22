
// VisualisationView.h : interface of the CVisualisationView class
//

#pragma once


class CVisualisationView : public CView
{
protected: // create from serialization only
	CVisualisationView() noexcept;
	DECLARE_DYNCREATE(CVisualisationView)

// Attributes
public:
	CVisualisationDoc* GetDocument() const;

// Operations
public:
	CFont font;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CVisualisationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void createBar(CVisualisationDoc* pDoc,CRect& rect, CDC* pDc);
	void createLine(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc);
	void createPie(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc);
protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in VisualisationView.cpp
inline CVisualisationDoc* CVisualisationView::GetDocument() const
   { return reinterpret_cast<CVisualisationDoc*>(m_pDocument); }
#endif

