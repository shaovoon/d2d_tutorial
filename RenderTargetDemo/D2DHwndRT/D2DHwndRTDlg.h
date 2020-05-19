
// D2DHwndRTDlg.h : header file
//

#pragma once

using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DHwndRTDlg dialog
class CD2DHwndRTDlg : public CDialogEx
{
// Construction
public:
	CD2DHwndRTDlg(CWnd* pParent = nullptr);	// standard constructor
	ComPtr<ID2D1HwndRenderTarget> m_Target;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void Draw();


public:
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DHWNDRT_DIALOG };
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
