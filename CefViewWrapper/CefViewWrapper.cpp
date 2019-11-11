// CefViewWrapper.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CefViewWrapper.h"

#include "CefViewWrapperImpl.h"


CefViewWrapper* CreateInstance()
{
	return  new CefViewWrapperImpl();
}

void DeleteInstance(CefViewWrapper* wrapper)
{
	if (wrapper)
		delete wrapper;
}

