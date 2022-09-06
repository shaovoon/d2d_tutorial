// EndCreditDlg.cpp : implementation file
//

#include "pch.h"
#include "Resource.h"
#include "EndCreditDlg.h"
#include "afxdialogex.h"
#include "FactorySingleton.h"
#include "ComException.h"

// EndCreditDlg dialog

IMPLEMENT_DYNAMIC(EndCreditDlg, CDialogEx)

EndCreditDlg::EndCreditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_END_CREDIT, pParent)
	, m_pFont(nullptr)
{

}

EndCreditDlg::~EndCreditDlg()
{
	if (m_pFont)
		delete m_pFont;

	m_pFont = nullptr;
}

void EndCreditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_END_CREDIT, m_edtEndCredit);
	DDX_Control(pDX, IDC_STATIC_TEXT_DISPLAY, m_staticTextDisplay);
	DDX_Control(pDX, IDC_STATIC_END_CREDIT_IMAGE, m_staticEndCreditImage);
}


BEGIN_MESSAGE_MAP(EndCreditDlg, CDialogEx)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDT_END_CREDIT, &EndCreditDlg::OnChangeEdtEndCredit)
	ON_BN_CLICKED(IDC_BTN_END_CREDIT_OK, &EndCreditDlg::OnBnClickedBtnEndCreditOk)
	ON_BN_CLICKED(IDC_BTN_END_CREDIT_CANCEL, &EndCreditDlg::OnBnClickedBtnEndCreditCancel)
	ON_BN_CLICKED(IDC_BTN_REMOVE_END_CREDIT, &EndCreditDlg::OnBnClickedBtnRemoveEndCredit)
END_MESSAGE_MAP()


// EndCreditDlg message handlers


BOOL EndCreditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect(0, 0, 160, 200);
	CRuntimeClass* pClass = RUNTIME_CLASS(CFontScrollView);
	m_pFontScrollView = (CFontScrollView*)pClass->CreateObject();
	m_pFontScrollView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 36001);
	m_pFontScrollView->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_pFontScrollView->OnInitialUpdate();

	UINT dpi = GetDpiForWindow(GetSafeHwnd());
	
	m_pFontScrollView->SetWindowPos(
		NULL,
		10 * dpi / 96, 140 * dpi / 96, 210 * dpi / 96, 140 * dpi / 96,
		SWP_SHOWWINDOW);

	m_edtEndCredit.SetWindowTextW(L"Hello World");

	LPCTSTR fontName = L"Arial";
	//CreateNewFont(fontName);

	m_pFontScrollView->SetUpdateDrawFunc(std::bind(&EndCreditDlg::CallDraw, this));

	if (m_EndCreditInfo.mFontFamily.empty() == false)
		m_pFontScrollView->SetFontFamily(m_EndCreditInfo.mFontFamily.c_str());
	else
		m_pFontScrollView->SetFontFamily(fontName);

	m_pFontScrollView->ScrollToFont();

	m_staticEndCreditImage.SetImage(m_ImageFile);

	if (m_EndCreditInfo.mText.empty() == false)
	{
		CString temp;
		for (auto ch : m_EndCreditInfo.mText)
		{
			if (ch == '\n')
				temp += '\r';

			temp += ch;
		}
		m_edtEndCredit.SetWindowTextW(temp);
	}
	else
		m_edtEndCredit.SetWindowTextW(L"STARRING\r\nJohn Smith\r\nJane Doe");

	CallDraw();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void EndCreditDlg::SetImage(const CString& file)
{
	m_ImageFile = file;
}

void EndCreditDlg::CallDraw()
{
	//Invalidate();
	CString text;
	m_edtEndCredit.GetWindowTextW(text);

	m_staticTextDisplay.SetInfo(m_pFontScrollView->GetFontFamily(), text, true, 1.0f, 26.0f);
	m_staticTextDisplay.Invalidate();
	//CreateNewFont(m_pFontScrollView->GetFontFamily());
}

void EndCreditDlg::CreateNewFont(LPCTSTR fontName)
{
	if (m_pFont)
		delete m_pFont;

	m_pFont = nullptr;

	m_pFont = new CFont();

	CClientDC dc(this);
	int nFontSize = 20;
	int nHeight = -((dc.GetDeviceCaps(LOGPIXELSY) * nFontSize) / 72);

	m_pFont->CreateFont(nHeight, 0, 0, 0, FW_NORMAL, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, fontName);

	m_edtEndCredit.SetFont(m_pFont);
}


// Direct2D version
void EndCreditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	m_staticTextDisplay.Invalidate();
	m_staticEndCreditImage.Invalidate();
}



/*
// GDI+ version
void EndCreditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	using namespace Gdiplus;
	Graphics graphics(dc.GetSafeHdc());

	CRect rect;
	GetClientRect(rect);

	SolidBrush brushWhite(Color::White);
	graphics.FillRectangle(&brushWhite, Gdiplus::Rect(0, 0, rect.Width(), rect.Height()));

	//graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	//graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	FontFamily family(m_pFontScrollView->GetFontFamily());
	int nFontSize = 20;
	Gdiplus::Font font(&family, 20);

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	SolidBrush brushBlack(Color::Black);

	CString text;
	m_edtEndCredit.GetWindowTextW(text);
	Gdiplus::RectF rectFInput(0, 220, rect.Width(), rect.Height()-220);

	graphics.DrawString(text, text.GetLength(), &font, rectFInput, &format, &brushBlack);
}
*/

void EndCreditDlg::OnChangeEdtEndCredit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CallDraw();
}



void EndCreditDlg::OnBnClickedBtnEndCreditOk()
{
	// TODO: Add your control notification handler code here
	CString text;
	m_edtEndCredit.GetWindowTextW(text);

	if (text != L"")
	{
		m_EndCreditInfo.mFontFamily = m_pFontScrollView->GetFontFamily();

		m_EndCreditInfo.mText = L"";
		for (int i = 0; i < text.GetLength(); ++i)
		{
			auto ch = text[i];
			if (ch != L'\r')
				m_EndCreditInfo.mText += ch;
		}
	}
	else
		m_EndCreditInfo.mFontFamily = L"";

	EndDialog(IDOK);
}


void EndCreditDlg::OnBnClickedBtnEndCreditCancel()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
}


void EndCreditDlg::OnBnClickedBtnRemoveEndCredit()
{
	// TODO: Add your control notification handler code here
	m_EndCreditInfo.mFontFamily = L"";
	EndDialog(IDOK);
}
