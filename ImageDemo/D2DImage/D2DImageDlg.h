
// D2DImageDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DImageDlg dialog
class CD2DImageDlg : public CDialogEx
{
// Construction
public:
	CD2DImageDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1HwndRenderTarget> m_Target;
	CString m_ImageFile;
	CString m_PatternImageFile;
	ComPtr<IWICFormatConverter> m_BkgdBitmap;
	ComPtr<ID2D1Bitmap> m_D2DBkgdBitmap;
	CString m_Text;
	ComPtr<IDWriteTextFormat> m_TextFormat;
	ComPtr<ID2D1BitmapBrush> m_FillBrush;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();

	void LoadImage(LPCTSTR file);
	void Draw();
	void DrawImage();
	void DrawImageText();

private:

public:
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DIMAGE_DIALOG };
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
