
// LightSpot.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLightSpotApp: 
// �йش����ʵ�֣������ LightSpot.cpp
//

class CLightSpotApp : public CWinApp
{
public:
	CLightSpotApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLightSpotApp theApp;