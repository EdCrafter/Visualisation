
// VisualisationDoc.h : interface of the CVisualisationDoc class
//


#pragma once


class CVisualisationDoc : public CDocument
{
protected: // create from serialization only
	CVisualisationDoc() noexcept;
	DECLARE_DYNCREATE(CVisualisationDoc)

// Attributes
public:
	double values[500]{};
	int count = 0;
	double sum = 0;
	enum class VisualisationType
	{
		Bar,
		Line,
		Pie
	};
	VisualisationType visualisationType = VisualisationType::Bar;

	bool is3D = false;
	bool isLog = false;
	bool isPercent = false;
	bool isNotValues = false;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CVisualisationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnAdditionalLogarithmic();
	afx_msg void OnAdditionalPercent();
	afx_msg void OnAdditionalDecimalscale();
	afx_msg void OnAdditionalShow();
	afx_msg void OnAdditional2d();
	afx_msg void On2dBar();
	afx_msg void On2dLine();
	afx_msg void On2dPie();
};
