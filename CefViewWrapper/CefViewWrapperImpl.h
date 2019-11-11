#pragma once
#include "CefViewWrapper.h"
#include "include/internal/cef_types_wrappers.h"

class CefViewWrapperImpl:public CefViewWrapper
{
public:
	CefViewWrapperImpl();
	~CefViewWrapperImpl();

	virtual CefWindowView * CreateCefView(HWND parent_hwnd) override;

	virtual void DeleteCefView(CefWindowView *) override;
	
	virtual void SetGlobalRegisterJS(const char * js_data, int len) override;

	virtual void AddWebResItem(const WebResItem& res) override;

	virtual void SetCachedDirector(const wchar_t * directory, int len) override;

	virtual void SetWindowBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;

	//virtual void CloseAllWindowViews() override;

	virtual bool Init() override;

	virtual void UnInit() override;

private:
	CefSettings settings;
};

