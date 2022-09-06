#pragma once
#include "FontScrollView.h"
#include "TextDisplayStatic.h"
#include "EndCreditImageStatic.h"
#include "FileFormat.h"

using namespace D2D1;
using namespace Microsoft::WRL;

// EndCreditDlg dialog

class EndCreditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EndCreditDlg)

public:
	EndCreditDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EndCreditDlg();
	void CallDraw();

	void SetImage(const CString& file);

	void SetEndCreditInfo(const Rendering::EndCreditInfo& info)
	{
		m_EndCreditInfo = info;
	}

	Rendering::EndCreditInfo& GetEndCreditInfo()
	{
		return m_EndCreditInfo;
	}

private:
	void CreateNewFont(LPCTSTR fontName);
	CFontScrollView* m_pFontScrollView;
	CFont* m_pFont;
	CString m_ImageFile;
	Rendering::EndCreditInfo m_EndCreditInfo;

public:

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_END_CREDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_edtEndCredit;
	TextDisplayStatic m_staticTextDisplay;
	EndCreditImageStatic m_staticEndCreditImage;
	afx_msg void OnPaint();
	afx_msg void OnChangeEdtEndCredit();
	afx_msg void OnBnClickedBtnEndCreditOk();
	afx_msg void OnBnClickedBtnEndCreditCancel();
	afx_msg void OnBnClickedBtnRemoveEndCredit();
};
