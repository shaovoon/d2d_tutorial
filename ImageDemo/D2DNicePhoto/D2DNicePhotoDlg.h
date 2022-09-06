
// D2DNicePhotoDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DNicePhotoDlg dialog
class CD2DNicePhotoDlg : public CDialogEx
{
// Construction
public:
	CD2DNicePhotoDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1HwndRenderTarget> m_Target;
	ComPtr<ID2D1BitmapRenderTarget> m_BmpTarget;
	CString m_ImageFile;
	CString m_PatternImageFile;
	ComPtr<IWICFormatConverter> m_BkgdBitmap;
	ComPtr<ID2D1Bitmap> m_D2DBkgdBitmap;
	ComPtr<IWICFormatConverter> m_PhotoBitmap;
	ComPtr<ID2D1Bitmap> m_D2DPhotoBitmap;
	CString m_Text;
	ComPtr<IDWriteTextFormat> m_TextFormat;
	ComPtr<ID2D1BitmapBrush> m_FillBrush;
	ComPtr<ID2D1SolidColorBrush> m_WhiteBrush;
	ComPtr<ID2D1SolidColorBrush> m_GreyBrush;
	ComPtr<ID2D1RadialGradientBrush> m_RadialBrushTopLeft;
	ComPtr<ID2D1RadialGradientBrush> m_RadialBrushBottomLeft;
	ComPtr<ID2D1RadialGradientBrush> m_RadialBrushBottomRight;
	ComPtr<ID2D1RadialGradientBrush> m_RadialBrushTopRight;
	int m_Radius;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void CreateRadialGradientBrush();

	void LoadImage(LPCTSTR file, ID2D1RenderTarget* rt, ComPtr<IWICFormatConverter>& bmp, ComPtr<ID2D1Bitmap>& d2dBmp);
	void Draw();
	void DrawImage();
	void DrawPhoto();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DNICEPHOTO_DIALOG };
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
