
// ftp_client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cftp_clientApp: 
// �йش����ʵ�֣������ ftp_client.cpp
//

class Cftp_clientApp : public CWinApp
{
public:
	Cftp_clientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cftp_clientApp theApp;