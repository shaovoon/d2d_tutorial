
// D2DAffineTransformDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DAffineTransformDlg dialog
class CD2DAffineTransformDlg : public CDialogEx
{
// Construction
public:
	CD2DAffineTransformDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1HwndRenderTarget> m_Target;
	ComPtr<ID2D1SolidColorBrush> m_FillBrush;
	CString m_Text;
	ComPtr<IDWriteTextFormat> m_TextFormat;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void Draw();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DAFFINETRANSFORM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
