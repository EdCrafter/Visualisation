
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

	long colors[16]{
		RGB(255, 0, 0), // red
		RGB(0, 255, 0), // green
		RGB(0, 0, 255), // blue
		RGB(255, 255, 0), // yellow
		RGB(255, 0, 255), // magenta
		RGB(0, 255, 255), // cyan
		RGB(128, 128, 128), // gray
		RGB(128, 0, 0), // dark red
		RGB(0, 128, 0), // dark green
		RGB(0, 0, 128), // dark blue
		RGB(128, 128, 0), // dark yellow
		RGB(128, 0, 128), // dark magenta
		RGB(0, 128, 128), // dark cyan
		RGB(192, 192, 192), // light gray
		RGB(255, 255, 255), // white
		RGB(255, 165, 0) // orange
	};
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

