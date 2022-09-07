
// TextDemoDlg.h : header file
//

#pragma once
#include "FontScrollView.h"
#include "TextDisplayStatic.h"

using namespace D2D1;
using namespace Microsoft::WRL;


// CTextDemoDlg dialog
class CTextDemoDlg : public CDialogEx
{
// Construction
public:
	CTextDemoDlg(CWnd* pParent = nullptr);	// standard constructor
	void CallDraw();
private:
	void CreateNewFont(LPCTSTR fontName);
	CFontScrollView* m_pFontScrollView;
	CFont* m_pFont;
	CEdit m_edtEndCredit;
	TextDisplayStatic m_staticTextDisplay;

public:
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTDEMO_DIALOG };
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
	afx_msg void OnChangeEdtEndCredit();
	afx_msg void OnBnClickedBtnEndCreditOk();
	afx_msg void OnBnClickedBtnEndCreditCancel();
public:
	CButton m_chkItalic;
	afx_msg void OnBnClickedChkItalic();
	CButton m_chkBold;
	afx_msg void OnBnClickedChkBold();
};
