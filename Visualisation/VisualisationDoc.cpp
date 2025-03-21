
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
END_MESSAGE_MAP()


// CVisualisationDoc construction/destruction

CVisualisationDoc::CVisualisationDoc() noexcept
{
	// TODO: add one-time construction code here
	values[0] = 1;
	values[1] = 2;
	values[2] = 3;
	values[3] = 4;
	values[4] = 5;
	values[5] = 6;
	count = 6;
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
		ar << count;
		for (int i = 0; i<count; i++)
		{
			ar << values[i];
		}
	}
	else
	{
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			ar >> values[i];
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
