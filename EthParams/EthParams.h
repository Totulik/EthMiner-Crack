
// EthParams.h: main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"	



class CEthParamsApp : public CWinApp
{
public:
	CEthParamsApp();


public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CEthParamsApp theApp;
