#include "stdafx.h"
#include "CefViewWrapperImpl.h"
#include "include/cef_app.h"
#include "CefWindowViewImpl.h"
#include "app/utils_app.h"
#include "simple_app.h"

CefViewWrapperImpl::CefViewWrapperImpl()
{
}


CefViewWrapperImpl::~CefViewWrapperImpl()
{
}

CefWindowView * CefViewWrapperImpl::CreateCefView(HWND parent_hwnd)
{
	return new CefWindowViewImpl(parent_hwnd);

}

void CefViewWrapperImpl::DeleteCefView(CefWindowView *view)
{
	if (view)
		delete view;
}

bool CefViewWrapperImpl::Init()
{
	// Parse command-line arguments.
	CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
	command_line->InitFromString(::GetCommandLineW());
	//
	CefRefPtr<CefApp> app = new SimpleApp();
	CefMainArgs main_args(::GetModuleHandle(NULL));
	void* sandbox_info = NULL;
	CefExecuteProcess(main_args, app, sandbox_info);

	//初始化CEFSettings
	settings.no_sandbox = true;
	settings.multi_threaded_message_loop = true;
	std::wstring sub_process = utils::app::GetInstancePathW() +_T("\\")+ HELPER_LAUNCH_APP_NAME;
	CefString(&settings.browser_subprocess_path) = CefString(sub_process);

	//CEF初始化
	return CefInitialize(main_args, settings, app, sandbox_info);
}

void CefViewWrapperImpl::UnInit()
{
	CefShutdown();
}

void CefViewWrapperImpl::SetGlobalRegisterJS(const char * js_data, int len)
{
	
}

void CefViewWrapperImpl::AddWebResItem(const WebResItem& res)
{
	
}

void CefViewWrapperImpl::SetCachedDirector(const wchar_t * directory, int len)
{
	CefString(&settings.cache_path) = CefString(std::wstring(directory,len));
}

void CefViewWrapperImpl::SetWindowBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	settings.background_color = CefColorSetARGB(a, r, g, b);
}

//void CefViewWrapperImpl::CloseAllWindowViews()
//{
//	
//}
