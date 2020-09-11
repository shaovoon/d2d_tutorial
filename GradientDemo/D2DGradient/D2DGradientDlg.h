
// D2DGradientDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DGradientDlg dialog
class CD2DGradientDlg : public CDialogEx
{
// Construction
public:
	CD2DGradientDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1HwndRenderTarget> m_Target;

	void CreateLinearGradientBrush();
	void CreateRadialGradientBrush();

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();

	void DrawLinearGradientRect();
	void DrawLinearGradientText();
	void DrawRadialGradientRect();
	void DrawRadialGradientText();

	void Draw();

private:
	ComPtr<ID2D1LinearGradientBrush> m_LinearBrush;
	ComPtr<ID2D1RadialGradientBrush> m_RadialBrush;
	CString m_Text;
	ComPtr<IDWriteTextFormat> m_TextFormat;

public:
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DGRADIENT_DIALOG };
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
