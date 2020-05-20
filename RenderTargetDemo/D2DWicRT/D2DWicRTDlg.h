
// D2DWicRTDlg.h : header file
//

#pragma once
using namespace D2D1;
using namespace Microsoft::WRL;

// CD2DWicRTDlg dialog
class CD2DWicRTDlg : public CDialogEx
{
// Construction
public:
	CD2DWicRTDlg(CWnd* pParent = nullptr);	// standard constructor
private:
	ComPtr<ID2D1RenderTarget> m_Target;
	ComPtr<IWICBitmap> m_WicBitmap; // WIC for above RT

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void Draw();

	void PaintAndSaveImage(PCWSTR filename);

	void SaveAs(ComPtr<IWICBitmap>& bitmap, PCWSTR filename);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DWICRT_DIALOG };
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
