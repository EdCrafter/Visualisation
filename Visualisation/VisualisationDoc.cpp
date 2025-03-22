
// VisualisationDoc.cpp : implementation of the CVisualisationDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Visualisation.h"
#endif

#include "VisualisationDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVisualisationDoc

IMPLEMENT_DYNCREATE(CVisualisationDoc, CDocument)

BEGIN_MESSAGE_MAP(CVisualisationDoc, CDocument)
	ON_COMMAND(ID_ADDITIONAL_LOGARITHMIC, &CVisualisationDoc::OnAdditionalLogarithmic)
	ON_COMMAND(ID_ADDITIONAL_PERCENT, &CVisualisationDoc::OnAdditionalPercent)
	ON_COMMAND(ID_ADDITIONAL_DECIMALSCALE, &CVisualisationDoc::OnAdditionalDecimalscale)
	ON_COMMAND(ID_ADDITIONAL_SHOW, &CVisualisationDoc::OnAdditionalShow)
	ON_COMMAND(ID_ADDITIONAL_2D, &CVisualisationDoc::OnAdditional2d)
	ON_COMMAND(ID_2D_BAR, &CVisualisationDoc::On2dBar)
	ON_COMMAND(ID_2D_LINE, &CVisualisationDoc::On2dLine)
	ON_COMMAND(ID_2D_PIE, &CVisualisationDoc::On2dPie)
END_MESSAGE_MAP()


// CVisualisationDoc construction/destruction

CVisualisationDoc::CVisualisationDoc() noexcept
{
	// TODO: add one-time construction code here
	values[0] = 1;
	values[1] = -2;
	values[2] = 3;
	values[3] = 2;
	values[4] = -5;
	values[5] = -6;
	values[6] = 7;
	values[7] = 8;
	values[8] = 9;
	values[9] = 10;
	values[10] = 11;
	values[11] = 12;
	values[12] = 13;
	values[13] = 14;
	values[14] = 15;
	values[15] = 16;

	count = 16;

	sum = 0;
	for (int i = 0; i < count; i++)
	{
		sum += fabs(values[i]);
	}
}

CVisualisationDoc::~CVisualisationDoc()
{
}

BOOL CVisualisationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CVisualisationDoc serialization

void CVisualisationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CString str;
		str.Format(L"%d;", count);
		for (int i = 0; i < count; i++)
		{
			CString num;
			num.Format(L"%.2f;", values[i]);
			str += num;
		}
		str += L"\n";
		ar.WriteString(str);
	}
	else
	{
		sum = 0;
		CString line;
		if (!ar.ReadString(line))
		{
			AfxMessageBox(L"Файл пошкоджений або порожній.");
			count = 0;
			return;
		}

		int pos = 0;
		CString token = line.Tokenize(L";", pos);

		if (token.IsEmpty() || _stscanf_s(token, L"%d", &count) != 1 || count <= 0 || count > 1000)
		{
			AfxMessageBox(L"Файл пошкоджений або містить неправильну кількість значень.");
			count = 0;
			return;
		}

		for (int i = 0; i < count; i++)
		{
			if (pos < 0)
			{
				AfxMessageBox(L"Файл розірваний або містить недостатньо значень.");
				count = i;
				break;
			}
			token = line.Tokenize(L";", pos);
			double val = 0;
			if (token.IsEmpty() || _stscanf_s(token, L"%lf", &val) != 1)
			{
				AfxMessageBox(L"У файлі знайдено некоректні числа.");
				val = 0;
			}
			values[i] = val;
			sum += fabs(val);
		}
	}
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CVisualisationDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CVisualisationDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CVisualisationDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CVisualisationDoc diagnostics

#ifdef _DEBUG
void CVisualisationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVisualisationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVisualisationDoc commands


void CVisualisationDoc::OnAdditionalLogarithmic()
{
	for (int i = 0;!isLog && i < count; i++)
	{
		if (!values[i]) continue;
		values[i] = log10(fabs(values[i]));
	}
	isLog = true;
	UpdateAllViews(NULL);
}


void CVisualisationDoc::OnAdditionalPercent()
{
	if (isPercent)
	{
		for (int i = 0; i < count; i++)
		{
			values[i] = values[i]* sum/100;
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			values[i] = values[i] / sum * 100;
		}
	}
	isPercent = !isPercent;
	UpdateAllViews(NULL);
}

void CVisualisationDoc::OnAdditionalDecimalscale()
{
	for (int i = 0; isLog && i < count; i++)
	{
		values[i] = pow(10, values[i]);
	}
	isLog = false;
	UpdateAllViews(NULL);
}


void CVisualisationDoc::OnAdditionalShow()
{
	isNotValues = !isNotValues;
	UpdateAllViews(NULL);
}


void CVisualisationDoc::OnAdditional2d()
{
	is3D = !is3D;
	UpdateAllViews(NULL);
}


void CVisualisationDoc::On2dBar()
{
	visualisationType = VisualisationType::Bar;
	UpdateAllViews(NULL);
}


void CVisualisationDoc::On2dLine()
{
	visualisationType = VisualisationType::Line;
	UpdateAllViews(NULL);
}


void CVisualisationDoc::On2dPie()
{
	visualisationType = VisualisationType::Pie;
	UpdateAllViews(NULL);
}
