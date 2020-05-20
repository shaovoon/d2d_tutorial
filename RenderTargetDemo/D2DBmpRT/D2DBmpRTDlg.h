
// D2DBmpRTDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DBmpRTDlg dialog
class CD2DBmpRTDlg : public CDialogEx
{
// Construction
public:
	CD2DBmpRTDlg(CWnd* pParent = nullptr);	// standard constructor
private:
	ComPtr<ID2D1DCRenderTarget> m_Target;
	ComPtr<ID2D1BitmapRenderTarget> m_BmpTarget;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void Draw();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DBMPRT_DIALOG };
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
