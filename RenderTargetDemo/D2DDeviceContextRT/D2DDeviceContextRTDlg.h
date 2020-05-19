
// D2DDeviceContextRTDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DDeviceContextRTDlg dialog
class CD2DDeviceContextRTDlg : public CDialogEx
{
// Construction
public:
	CD2DDeviceContextRTDlg(CWnd* pParent = nullptr);	// standard constructor
private:
	ComPtr<ID2D1DCRenderTarget> m_Target;

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void Draw();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DDEVICECONTEXTRT_DIALOG };
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
