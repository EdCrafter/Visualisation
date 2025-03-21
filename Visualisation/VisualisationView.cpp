
// VisualisationView.cpp : implementation of the CVisualisationView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Visualisation.h"
#endif

#include "VisualisationDoc.h"
#include "VisualisationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisualisationView

IMPLEMENT_DYNCREATE(CVisualisationView, CView)

BEGIN_MESSAGE_MAP(CVisualisationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CVisualisationView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CVisualisationView construction/destruction

CVisualisationView::CVisualisationView() noexcept
{
	// TODO: add construction code here
	font.CreatePointFont(200, L"Arial");
}

CVisualisationView::~CVisualisationView()
{
	font.DeleteObject();
}

BOOL CVisualisationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszClass = AfxRegisterWndClass(CS_ACTIVE, NULL, (HBRUSH)(0), NULL);
	//= AfxRegisterWndClass(CS_DBLCLKS, NULL, (HBRUSH)(0), NULL);

	

	return CView::PreCreateWindow(cs);
}

// CVisualisationView drawing

void CVisualisationView::OnDraw(CDC* pDC)
{
	CVisualisationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDoc->count)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	memDC.FillSolidRect(rect, RGB(255, 255, 255));

	CFont* pNewFont = pDC->SelectObject(&font);

	switch (pDoc->visualisationType)
	{
	case CVisualisationDoc::VisualisationType::Bar:
		createBar(pDoc, rect, &memDC);
		break;
	case CVisualisationDoc::VisualisationType::Line:
		createLine(pDoc, rect, &memDC);
		break;
	case CVisualisationDoc::VisualisationType::Pie:
		createPie(pDoc, rect, &memDC);
		break;
	default:
		createBar(pDoc, rect, &memDC); 
		break;
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	pDC->SelectObject(pNewFont);

	memDC.SelectObject(pOldBmp);
}


// CVisualisationView printing


void CVisualisationView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CVisualisationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVisualisationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVisualisationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CVisualisationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVisualisationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVisualisationView diagnostics

#ifdef _DEBUG
void CVisualisationView::AssertValid() const
{
	CView::AssertValid();
}

void CVisualisationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CVisualisationView::createBar(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc)
{
	double max = pDoc->values[0];
	double min = pDoc->values[0];
	double sum = 0;
	double indentX = 100;
	double indentY = 100;
	for (int i = 0; i<pDoc->count; i++)
	{
		sum += pDoc->values[i];
		if (pDoc->values[i] > max)
		{
			max = pDoc->values[i];
		}
		if (pDoc->values[i] < min)
		{
			min = pDoc->values[i];
		}
	}
	double stepX = (rect.Width() - 2*indentX ) / pDoc->count;
	double stepY = (rect.Height() - 2 * indentY) / (max - min);








	pDc->TextOutW(indentX, indentY, L"Bar");
}

void CVisualisationView::createLine(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc)
{
}

void CVisualisationView::createPie(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc)
{
}

CVisualisationDoc* CVisualisationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisualisationDoc)));
	return (CVisualisationDoc*)m_pDocument;
}
#endif //_DEBUG


// CVisualisationView message handlers


BOOL CVisualisationView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CView::OnEraseBkgnd(pDC);
}
