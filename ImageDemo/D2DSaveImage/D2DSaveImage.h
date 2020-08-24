
// D2DSaveImage.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CD2DSaveImageApp:
// See D2DSaveImage.cpp for the implementation of this class
//

class CD2DSaveImageApp : public CWinApp
{
public:
	CD2DSaveImageApp();
	~CD2DSaveImageApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CD2DSaveImageApp theApp;
