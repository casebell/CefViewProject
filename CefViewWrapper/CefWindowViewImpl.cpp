#include "stdafx.h"
#include "CefWindowViewImpl.h"
#include "include/internal/cef_win.h"
#include "include/cef_browser.h"
#include "simple_handler.h"
#include "include/cef_request_context.h"

////共享的请求会话
//CefRefPtr<CefRequestContext> shared_request_context_;
//
//CefRefPtr<CefRequestContext> GetSharedRequestContext()
//{
//	if (!shared_request_context_.get())
//	{
//		shared_request_context_ =
//			//	CefRequestContext::CreateContext(CefRequestContext::GetGlobalContext(),
//			CefRequestContext::GetGlobalContext();//);
//	}
//	return shared_request_context_;
//}

#define CEF_BLOCK_QUIT_MSG WM_USER + 1

CefWindowViewImpl::CefWindowViewImpl(HWND parent)
	:CefWindowView(parent)
{
	standard_wind_.SetDelegate(this);
	standard_wind_.CreateBaseWindow(parent);
	SetWindowLong(standard_wind_.GetHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	// Set window info
	CefWindowInfo window_info;
	RECT rc = { 0,0,1,1 };
	window_info.SetAsChild(standard_wind_.GetHwnd(),rc);

	CefBrowserSettings browserSettings;
	browserSettings.plugins = STATE_ENABLED;	// disable all plugins

	/*QCefSetting::BackgroundColor bc = QCefSetting::GetBackgroundColor();*/
	//M:背景色
	//browserSettings.background_color = CefColorSetARGB(bc.a, bc.r, bc.g, bc.b);
	//
	hander_ = new SimpleHandler(false);
	hander_->SetDelegate(this);

	// Create the main browser window.
	if (!CefBrowserHost::CreateBrowser(
		window_info,				// window info
		hander_,			// handler
		_T("https://www.baidu.com"),			// url
		browserSettings,			// settings
		NULL,
		NULL))
	{
		//QLOG(QStringLiteral("Failed to create browser."));
	}

}


CefWindowViewImpl::~CefWindowViewImpl()
{
	if (hander_)
	{
		hander_->CloseAllBrowsers(true);

		close_thread_id_ = GetCurrentThreadId();
		MSG msg;
		// 主消息循环:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (CEF_BLOCK_QUIT_MSG == msg.message)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
}

void CefWindowViewImpl::Show()
{
	standard_wind_.Show();
}

void CefWindowViewImpl::Hide()
{
	standard_wind_.Hide();
}

void CefWindowViewImpl::LoadUrl(const char * url)
{
	
}

void CefWindowViewImpl::SetGeometry(int x, int y, int w, int h)
{
	standard_wind_.SetGeometry(x, y, w, h);
}

void CefWindowViewImpl::GoBack()
{

}

void CefWindowViewImpl::GoForward()
{
	
}

HWND CefWindowViewImpl::GetBrowserHolderWindow()
{
	return standard_wind_.GetHwnd();
}

HWND CefWindowViewImpl::GetBrowserWindow()
{
	if (!hander_ || !hander_->GetMainBrowser())
		return false;
	return hander_->GetMainBrowser()->GetHost()->GetWindowHandle();
}

void CefWindowViewImpl::OnPaint(HDC dc)
{
	
}

void CefWindowViewImpl::OnResize(int width, int height)
{
	HWND hwnd = GetBrowserWindow();
	MoveWindow(hwnd, 0, 0, width, height, false);
}


void CefWindowViewImpl::OnBrowserCreated()
{
	AdjustSize();
}

void CefWindowViewImpl::Resize(int w, int h)
{
	HWND hwnd = GetBrowserHolderWindow();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetGeometry(rect.left, rect.top, w, h);
}

void CefWindowViewImpl::AdjustSize()
{
	HWND hwnd = GetBrowserHolderWindow();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetGeometry(rect.left, rect.top, rect.right-rect.left -1,rect.bottom-rect.top-1);
	SetGeometry(rect.left, rect.top, rect.right - rect.left , rect.bottom - rect.top );
}

void CefWindowViewImpl::OnBrowserDestroyed()
{
	DCHECK(hander_);
	hander_ = nullptr;
	PostThreadMessage(close_thread_id_, CEF_BLOCK_QUIT_MSG, 0, 0);
}

void CefWindowViewImpl::Close(bool force)
{
	CloseWindow(GetBrowserHolderWindow());
}

bool CefWindowViewImpl::OnClosed()
{
	if (!hander_)
		return false;

	if (hander_->IsClosing())
	{
		return true;
	}
	else
	{
		hander_->CloseAllBrowsers(FALSE);
		return false;
	}
}

void CefWindowViewImpl::OnDestoried()
{
	
}
