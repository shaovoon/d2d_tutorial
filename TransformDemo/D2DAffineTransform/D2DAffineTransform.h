
// D2DAffineTransform.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CD2DAffineTransformApp:
// See D2DAffineTransform.cpp for the implementation of this class
//

class CD2DAffineTransformApp : public CWinApp
{
public:
	CD2DAffineTransformApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CD2DAffineTransformApp theApp;
