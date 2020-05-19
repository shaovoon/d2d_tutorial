
// D2DDeviceContextRT.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CD2DDeviceContextRTApp:
// See D2DDeviceContextRT.cpp for the implementation of this class
//

class CD2DDeviceContextRTApp : public CWinApp
{
public:
	CD2DDeviceContextRTApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CD2DDeviceContextRTApp theApp;
