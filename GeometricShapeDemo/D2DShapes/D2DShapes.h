
// D2DShapes.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CD2DShapesApp:
// See D2DShapes.cpp for the implementation of this class
//

class CD2DShapesApp : public CWinApp
{
public:
	CD2DShapesApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CD2DShapesApp theApp;
