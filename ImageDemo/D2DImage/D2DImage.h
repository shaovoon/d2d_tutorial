
// D2DImage.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CD2DImageApp:
// See D2DImage.cpp for the implementation of this class
//

class CD2DImageApp : public CWinApp
{
public:
	CD2DImageApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CD2DImageApp theApp;
