
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
	font.CreatePointFont(100, L"Arial");
	srand(time(0));
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

	CFont* pNewFont = memDC.SelectObject(&font);

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

	memDC.SelectObject(pNewFont);

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
	
	double indentX = 150;
	double indentY = 50;
	for (int i = 0; i<pDoc->count; i++)
	{
		if (pDoc->values[i] > max)
		{
			max = pDoc->values[i];
		}
		if (pDoc->values[i] < min)
		{
			min = pDoc->values[i];
		}
	}
	double diff = max-min;

	double stepX = (rect.Width() - 2*indentX ) / (2*pDoc->count+1);
	int countDesh = 10;
	double stepY = (rect.Height() - 2 * indentY) / (countDesh+1);

	double koef = (rect.Height() - 2* indentY - 2* stepY)/ diff;

	double zeroAxis = indentY +stepY + max * koef;
	CRect r;
	r.left = indentX + stepX;
	r.right = r.left + stepX;
	double height = 0;
	int R, G, B;
	for (int i = 0; i < pDoc->count; i++)
	{
		height = fabs(pDoc->values[i]+(!pDoc->values[i])*0.01) * koef;
		
		if (pDoc->values[i] >= 0)
		{
			r.top = zeroAxis - height;
			r.bottom = zeroAxis;
		}
		else
		{
			r.top = zeroAxis;
			r.bottom = zeroAxis + height;
		}
		do
		{
			R = rand() % 256;
			G = rand() % 256;
			B = rand() % 256;
		} while (0.299 * R + 0.587 * G + 0.114 * B < 85 || 0.299 * R + 0.587 * G + 0.114 * B > 240);

		CBrush brush(RGB(R, G, B));
		pDc->SetBkColor(RGB(R, G, B));
		pDc->SelectObject(&brush);
		pDc->Rectangle(&r);

		if (pDoc->is3D) {
			int depth = stepX / 4;
			CPoint rightSide[4] = {
				{ r.right, r.top },
				{ r.right + depth, r.top - depth },
				{ r.right + depth, r.bottom - depth },
				{ r.right, r.bottom }
			};
			pDc->Polygon(rightSide, 4);
			CPoint topSide[4] = {
				{ r.left, r.top },
				{ r.left + depth, r.top - depth },
				{ r.right + depth, r.top - depth },
				{ r.right, r.top }
			};
			pDc->Polygon(topSide, 4);
		}

		if (!pDoc->isNotValues) {
			CString str;
			str.Format(L"%0.2f", pDoc->values[i]);
			CSize rStr = pDc->GetTextExtent(str);
			if (rStr.cx * 1.2 < stepX && rStr.cy * 1.2 < height)
			{
				pDc->DrawText(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (rStr.cx * 1.2 < stepX * 2 && rStr.cy * 1.2 < rect.Height()/3)
			{
				CRect rText = r;
				rText.top -= 5;
				rText.bottom = r.top - 25;
				rText.left -= stepX / 2;
				rText.right += stepX / 2;
				if (pDoc->is3D) {
					rText.top -= stepX / 4;
					rText.bottom -= stepX / 4;
					rText.left += stepX / 4;
					rText.right += stepX / 4;
				}
				pDc->SetBkColor(RGB(255, 255, 255));
				pDc->DrawText(str, &rText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}

		brush.DeleteObject();
		
		r.left += 2 * stepX;
		r.right += 2 * stepX;
	}

	pDc->SetBkColor(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
	pDc->SelectObject(&pen);

	pDc->MoveTo(indentX, rect.Height() - indentY);
	pDc->LineTo(indentX, indentY);
	pDc->LineTo(indentX + 13, indentY + 40);
	pDc->MoveTo(indentX, indentY);
	pDc->LineTo(indentX - 13, indentY + 40);

	pDc->MoveTo(indentX, zeroAxis);
	pDc->LineTo(rect.Width() - indentX, zeroAxis);
	pDc->LineTo(rect.Width() - indentX - 40, zeroAxis - 13);
	pDc->MoveTo(rect.Width() - indentX, zeroAxis);
	pDc->LineTo(rect.Width() - indentX - 40, zeroAxis + 13);

	double step = diff / (countDesh-1);
	for (int i = 0; i < countDesh; i++)
	{
		pDc->MoveTo(indentX - 17, rect.Height() - indentY - stepY - i * stepY );
		pDc->LineTo(indentX + 17, rect.Height() - indentY - stepY - i * stepY );

		CString str;
		str.Format(L"%.2f", min + i * step);
		CSize rStr = pDc->GetTextExtent(str);
		pDc->TextOutW(indentX-10-rStr.cx, rect.Height() - indentY - stepY - i * stepY - 40, str);
	}

	pen.DeleteObject();
	
	pDc->TextOutW(indentX + 10, zeroAxis + 10, L"0");
	pDc->TextOutW(rect.Width() - indentX-20, zeroAxis + 20, L"X");
	if (pDoc->isLog) {
		pDc->TextOutW(indentX - 80, indentY, L"lg(Y)");
	}
	else {
		pDc->TextOutW(indentX - 40, indentY, L"Y");
	}
	if (pDoc->isPercent) {
		pDc->TextOutW(indentX + 20, indentY, L"%");
	}
	
}

void CVisualisationView::createLine(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc)
{
	double max = pDoc->values[0];
	double min = pDoc->values[0];

	double indentX = 150;
	double indentY = 50;
	for (int i = 0; i < pDoc->count; i++)
	{
		if (pDoc->values[i] > max)
		{
			max = pDoc->values[i];
		}
		if (pDoc->values[i] < min)
		{
			min = pDoc->values[i];
		}
	}
	double diff = max - min;

	double stepX = (rect.Width() - 2 * indentX) / (2 * pDoc->count + 1);
	int countDesh = 10;
	double stepY = (rect.Height() - 2 * indentY) / (countDesh + 1);

	double koef = (rect.Height() - 2 * indentY - 2 * stepY) / diff;

	double zeroAxis = indentY + stepY + max * koef;
	CRect r;
	r.left = indentX + stepX;
	r.right = r.left + stepX;
	pDc->MoveTo(indentX, zeroAxis);
	CPen pen(PS_SOLID, 5, RGB(0, 200, 200));
	CPen pen2(PS_SOLID, 2, RGB(0, 0, 0));
	CBrush brush(RGB(230, 0, 0));
	pDc->SelectObject(&brush);
	pDc->SelectObject(&pen);
	double height = 0;
	for (int i = 0; i < pDoc->count; i++)
	{
		height = pDoc->values[i] * koef;
		pDc->LineTo(r.left, zeroAxis - height);
		r.left += 2 * stepX;
		r.right += 2 * stepX;
	}
	r.left = indentX + stepX;
	r.right = r.left + stepX;
	for (int i = 0; i < pDoc->count; i++)
	{
		height = pDoc->values[i] * koef;
		pDc->FillRect(&CRect(r.left - 5, zeroAxis - height - 5, r.left + 5, zeroAxis - height + 5), &brush);
		r.bottom = zeroAxis - height - 5;
		r.top = zeroAxis - height - 25;

		if (!pDoc->isNotValues) {
			pDc->SelectObject(&pen2);
			CString str;
			str.Format(L"%0.2f", pDoc->values[i]);
			CSize rStr = pDc->GetTextExtent(str);
			pDc->DrawText(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		r.left += 2 * stepX;
		r.right += 2 * stepX;
	}
	brush.DeleteObject();
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	pDc->SelectObject(&pen);

	pDc->MoveTo(indentX, rect.Height() - indentY);
	pDc->LineTo(indentX, indentY);
	pDc->LineTo(indentX + 13, indentY + 40);
	pDc->MoveTo(indentX, indentY);
	pDc->LineTo(indentX - 13, indentY + 40);

	pDc->MoveTo(indentX, zeroAxis);
	pDc->LineTo(rect.Width() - indentX, zeroAxis);
	pDc->LineTo(rect.Width() - indentX - 40, zeroAxis - 13);
	pDc->MoveTo(rect.Width() - indentX, zeroAxis);
	pDc->LineTo(rect.Width() - indentX - 40, zeroAxis + 13);

	double step = diff / (countDesh - 1);
	for (int i = 0; i < countDesh; i++)
	{
		pDc->MoveTo(indentX - 17, rect.Height() - indentY - stepY - i * stepY);
		pDc->LineTo(indentX + 17, rect.Height() - indentY - stepY - i * stepY);

		CString str;
		str.Format(L"%.2f", min + i * step);
		CSize rStr = pDc->GetTextExtent(str);
		pDc->TextOutW(indentX - 10 - rStr.cx, rect.Height() - indentY - stepY - i * stepY - 40, str);
	}

	pen.DeleteObject();

	pDc->TextOutW(indentX + 10, zeroAxis + 10, L"0");
	pDc->TextOutW(rect.Width() - indentX - 20, zeroAxis + 20, L"X");
	if (pDoc->isLog) {
		pDc->TextOutW(indentX - 80, indentY, L"lg(Y)");
	}
	else {
		pDc->TextOutW(indentX - 40, indentY, L"Y");
	}
	if (pDoc->isPercent) {
		pDc->TextOutW(indentX + 20, indentY, L"%");
	}

}

void CVisualisationView::createPie(CVisualisationDoc* pDoc, CRect& rect, CDC* pDc)
{
	double r1 = rect.Width() / 2.0;
	double r2 = rect.Height() / 2.0;
	double r = min(r1, r2)-40;

	double centerX = rect.left + r1;
	double centerY = rect.top + r2;

	CRect diagramRect((int)(centerX - r), (int)(centerY - r), (int)(centerX + r), (int)(centerY + r));

	double sum = 0;
	for (int i = 0; i < pDoc->count; i++)
	{
		sum += fabs(pDoc->values[i]);
	}

	double startAngle = 0;
	double angle = 0;
	double angle1 = 0;
	double angle2 = 0;
	CPoint point1, point2;
	CBrush brush;
	CBrush* pOldBrush;
	int R, G, B;

	for (int i = 0; i < pDoc->count; i++)
	{
		if (pDoc->values[i] == 0) continue;

		do
		{
			R = rand() % 256;
			G = rand() % 256;
			B = rand() % 256;
		} while (0.299 * R + 0.587 * G + 0.114 * B < 85 || 0.299 * R + 0.587 * G + 0.114 * B > 240);

		brush.CreateSolidBrush(RGB(R, G, B));
		pOldBrush = pDc->SelectObject(&brush);

		angle = fabs(pDoc->values[i]) / sum * 360.0;
		angle1 = startAngle * 3.1415 / 180.0;
		angle2 = (startAngle + angle) * 3.1415 / 180.0;

		point1.x = (int)(centerX + r * cos(angle1));
		point1.y = (int)(centerY - r * sin(angle1));
		point2.x = (int)(centerX + r * cos(angle2));
		point2.y = (int)(centerY - r * sin(angle2));

		pDc->Pie(diagramRect, point1, point2);

		brush.DeleteObject();
		brush.CreateSolidBrush(RGB(R / 2, G / 2, B / 2));
		if (pDoc->is3D) {
			pDc->Pie(diagramRect, point1, point2);
		}

		if (!pDoc->isNotValues) {
			double midAngle = (startAngle + angle / 2.0) * 3.1415 / 180.0;
			double textRadius = r * 0.85;
			int textX = (int)(centerX + textRadius * cos(midAngle));
			int textY = (int)(centerY - textRadius * sin(midAngle));
			CString str;
			str.Format(L"%0.2f", pDoc->values[i]);
			CSize textSize = pDc->GetTextExtent(str);
			CRect textRect;
			textRect.left = textX - textSize.cx / 2;
			textRect.top = textY - textSize.cy / 2;
			textRect.right = textRect.left + textSize.cx;
			textRect.bottom = textRect.top + textSize.cy;
			pDc->SetBkMode(TRANSPARENT);
			pDc->DrawText(str, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}


		pDc->SelectObject(pOldBrush);
		startAngle += angle;
		brush.DeleteObject();
	}
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
