
// D2DShapesDlg.h : header file
//

#pragma once

using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DShapesDlg dialog
class CD2DShapesDlg : public CDialogEx
{
// Construction
public:
	CD2DShapesDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1HwndRenderTarget> m_Target;
	ComPtr<ID2D1SolidColorBrush> m_StrokeBrush;
	ComPtr<ID2D1SolidColorBrush> m_FillBrush;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void Draw();

	void DrawLine();

	void DrawRectangle();
	void DrawRoundedRectangle();
	void DrawCircle();
	ComPtr<ID2D1PathGeometry> GenTriangleGeometry();
	void DrawTriangle();


	ComPtr<ID2D1StrokeStyle> CreateStrokeStyle();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DSHAPES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
