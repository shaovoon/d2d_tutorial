
// D2DSaveImageDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DSaveImageDlg dialog
class CD2DSaveImageDlg : public CDialogEx
{
// Construction
public:
	CD2DSaveImageDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1RenderTarget> m_Target;
	ComPtr<IWICBitmap> m_WicBitmap; // WIC for above RT
	CString m_PatternImageFile;
	ComPtr<IWICFormatConverter> m_BkgdBitmap;
	ComPtr<ID2D1Bitmap> m_D2DBkgdBitmap;
	CString m_Text;
	ComPtr<IDWriteTextFormat> m_TextFormat;
	ComPtr<ID2D1SolidColorBrush> m_FillBrush;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();

	void LoadImage(LPCTSTR file);
	void SaveAs(ComPtr<IWICBitmap>& bitmap, PCWSTR filename);
	void PaintAndSaveImage(PCWSTR filename);

	void Draw();
	void DrawImage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DSAVEIMAGE_DIALOG };
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
