
// D2DHwndRTDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "D2DHwndRT.h"
#include "D2DHwndRTDlg.h"
#include "afxdialogex.h"
#include "../Common/FactorySingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CD2DHwndRTDlg dialog

CD2DHwndRTDlg::CD2DHwndRTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_D2DHWNDRT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD2DHwndRTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CD2DHwndRTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CD2DHwndRTDlg message handlers

BOOL CD2DHwndRTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2DHwndRTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		if (!m_Target)
		{
			CRect rc;
			GetClientRect(rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
			);

			HR(FactorySingleton::GetGraphicsFactory()->CreateHwndRenderTarget(
				RenderTargetProperties(),
				HwndRenderTargetProperties(GetSafeHwnd(), size),
				m_Target.ReleaseAndGetAddressOf()));

			CreateDeviceResources();
		}

		if (!(D2D1_WINDOW_STATE_OCCLUDED & m_Target->CheckWindowState()))
		{
			m_Target->BeginDraw();

			Draw();

			if (D2DERR_RECREATE_TARGET == m_Target->EndDraw())
			{
				m_Target.Reset();
				Invalidate();
			}
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2DHwndRTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD2DHwndRTDlg::CreateDeviceResources()
{
}

void CD2DHwndRTDlg::CreateDeviceIndependentResources()
{
}

void CD2DHwndRTDlg::Draw()
{
	m_Target->Clear(ColorF(0.26f, 0.56f, 0.87f));
}

