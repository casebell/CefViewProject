// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "simple_handler.h"

namespace {

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
class SimpleWindowDelegate : public CefWindowDelegate {
 public:
  explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

  void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
    // Add the browser view and show the window.
    window->AddChildView(browser_view_);
    window->Show();

    // Give keyboard focus to the browser view.
    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
    browser_view_ = NULL;
  }

  bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
    // Allow the window to close if the browser says it's OK.
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser)
      return browser->GetHost()->TryCloseBrowser();
    return true;
  }

  CefSize GetPreferredSize(CefRefPtr<CefView> view) OVERRIDE {
    return CefSize(800, 600);
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;

  IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
  DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
};

}  // namespace

SimpleApp::SimpleApp() {}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();

//#if defined(OS_WIN) || defined(OS_LINUX)
//  // Create the browser using the Views framework if "--use-views" is specified
//  // via the command-line. Otherwise, create the browser using the native
//  // platform framework. The Views framework is currently only supported on
//  // Windows and Linux.
//  const bool use_views = command_line->HasSwitch("use-views");
//#else
//  const bool use_views = false;
//#endif
//
//  // SimpleHandler implements browser-level callbacks.
//  CefRefPtr<SimpleHandler> handler(new SimpleHandler(use_views));
//
//  // Specify CEF browser settings here.
//  CefBrowserSettings browser_settings;
//
//  std::string url;
//
//  // Check if a "--url=" value was provided via the command-line. If so, use
//  // that instead of the default URL.
//  url = command_line->GetSwitchValue("url");
//  if (url.empty())
//    url = "http://www.google.com";
//
//  if (use_views) {
//    // Create the BrowserView.
//    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
//        handler, url, browser_settings, NULL, NULL, NULL);
//
//    // Create the Window. It will show itself after creation.
//    CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
//  } else {
//    // Information used when creating the native window.
//    CefWindowInfo window_info;
//
//#if defined(OS_WIN)
//    // On Windows we need to specify certain flags that will be passed to
//    // CreateWindowEx().
//    window_info.SetAsPopup(NULL, "cefsimple");
//#endif
//
//    // Create the first browser window.
//    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
//                                  NULL, NULL);
//  }
}

void SimpleApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitch("disable-web-security");
	//http https互访
	//By default, an https page cannot run JavaScript, CSS or plugins from http URLs.
	//This provides an override to get the old insecure behavior.
	command_line->AppendSwitch("allow-running-insecure-content");
	//禁用代理
	//Don't use a proxy server, always make direct connections.
	//Overrides any other proxy server flags that are passed.
	command_line->AppendSwitchWithValue("no-proxy-server", "1");
	//限制进程数量
	command_line->AppendSwitch("process-per-site");
	command_line->AppendSwitchWithValue("renderer-process-limit", "10");
	//禁用拼写检查
	command_line->AppendSwitch("disable-spell-checking");
	//允许文件访问
	//By default, file:// URIs cannot read other file:// URIs.
	//This is an override for developers who need the old behavior for testing.
	command_line->AppendSwitch("allow-file-access-from-files");
	command_line->AppendSwitch("allow-universal-access-from-files");
	command_line->AppendSwitch("allow-insecure-localhost");
	//禁用textarea缩放
	command_line->AppendSwitch("disable-text-area-resize");
	//禁用pdf扩展
	command_line->AppendSwitch("disable-pdf-extension");
	//Disable extensions.
	command_line->AppendSwitch("disable-extensions");
	//Force logging to be disabled.
	//Logging is enabled by default in debug builds.
	command_line->AppendSwitch("disable-logging");
	//https://bitbucket.org/chromiumembedded/cef/issues/1708/add-support-for-plugin-placeholders-and
	//--plugin-policy=allow: Default mode. All plugins are allowed to play.
	//--plugin-policy=block: All plugins will be blocked (right-click on the plugin to play).
	//--plugin-policy=detect: Detect and play important plugin content. Other plugins will be blocked (right-click on the plugin to play).
	command_line->AppendSwitchWithValue("plugin-policy", "allow");

	command_line->AppendSwitch("disable-javascript-close-windows");

	//去掉webgl gpu extensions的支持,可以减少文件:
	//webgl(d3dcompiler_43.dll,d3dcompiler_47.dll,libEGL.dll,libGLESv2.dll)
	//extensions(cef_extensions.pak)
	//Disables GPU hardware acceleration.
	//If software renderer is not in place, then the GPU process won't launch.
	//command_line->AppendSwitch("disable-gpu");
	//Prevent the compositor from using its GPU implementation.
	//command_line->AppendSwitch("disable-gpu-compositing");
	//Stop the GPU from synchronizing on the vsync before presenting.
	//We can select from the options below: beginframe: Next frame can start without any delay on cc::scheduler in renderer compositors.
	//gpu: Disable display and browser vsync.
	//default: Set both flags.
	//command_line->AppendSwitch("disable-gpu-vsync");
	//Disable experimental WebGL support.
	//command_line->AppendSwitch("disable-webgl");
	//Disable Pepper3D.
	//command_line->AppendSwitch("disable-pepper-3d");
	//使48px以下的字体均为sub-pixel渲染 减少cpu开销
	command_line->AppendSwitch("enable-direct-write");
	//command_line->AppendSwitch("disable-direct-write");
	//command_line->AppendSwitch("off-screen-rendering-enabled");
	//command_line->AppendSwitch("enable-begin-frame-scheduling");
	//command_line->AppendSwitch("disable-accelerated-compositing");
	//允许媒体流
	command_line->AppendSwitch("enable-media-stream");
	command_line->AppendSwitch("enable-usermedia-screen-capturing");
	//设置光栅
	//Number of worker threads used to rasterize content.
	command_line->AppendSwitchWithValue("num-raster-threads", "2");
	//dpi禁用
	//Device scale factor passed to certain processes like renderers, etc.
	command_line->AppendSwitchWithValue("device-scale-factor", "1");
	command_line->AppendSwitchWithValue("force-device-scale-factor", "1");


	//Enables compositor-accelerated touch-screen pinch gestures.
	command_line->AppendSwitch("enable-pinch");

	//Enables sending the next BeginMainFrame before the previous commit activates.
	//command_line->AppendSwitch("enable-main-frame-before-activation");
	//flash
	//command_line->AppendSwitch("enable-system-flash");
	//command_line->AppendSwitchWithValue("ppapi-flash-path", "plugins\\pepflashplayer.dll");
	//command_line->AppendSwitchWithValue("ppapi-flash-version", "29.0.0.171");
	//npapi插件支持
	command_line->AppendSwitch("enable-npapi");
	//允许视频自动播放
	command_line->AppendSwitchWithValue("autoplay-policy", "no-user-gesture-required");
}
