// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "../browser/client_app_browser.h"

#include "include/base/cef_logging.h"
#include "include/cef_cookie.h"
//#include "../browser/main_message_loop_external_pump.h"
#include "../common/client_switches.h"

namespace client {

ClientAppBrowser::ClientAppBrowser() {
  CreateDelegates(delegates_);
}

void ClientAppBrowser::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
  // Pass additional command-line flags to the browser process.
  //if (process_type.empty()) {
  //  //// Pass additional command-line flags when off-screen rendering is enabled.
  //  //if (command_line->HasSwitch(switches::kOffScreenRenderingEnabled) &&
  //  //    !command_line->HasSwitch(switches::kSharedTextureEnabled)) {
  //  //  // Use software rendering and compositing (disable GPU) for increased FPS
  //  //  // and decreased CPU usage. This will also disable WebGL so remove these
  //  //  // switches if you need that capability.
  //  //  // See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
  //  //  //if (!command_line->HasSwitch(switches::kEnableGPU)) {
  //  //    command_line->AppendSwitch("disable-gpu");
  //  //    command_line->AppendSwitch("disable-gpu-compositing");
  //  //  //}
  //  //}

  //  //if (command_line->HasSwitch(switches::kUseViews) &&
  //  //    !command_line->HasSwitch("top-chrome-md")) {
  //  //  // Use non-material mode on all platforms by default. Among other things
  //  //  // this causes menu buttons to show hover state. See usage of
  //  //  // MaterialDesignController::IsModeMaterial() in Chromium code.
  //  //  command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
  //  //}

  //  //if (!command_line->HasSwitch(switches::kCachePath) &&
  //  //    !command_line->HasSwitch("disable-gpu-shader-disk-cache")) {
  //  //  // Don't create a "GPUCache" directory when cache-path is unspecified.
  //  //  command_line->AppendSwitch("disable-gpu-shader-disk-cache");
  //  //}

	 // //command_line->AppendSwitch("disable-gpu");
	 // //command_line->AppendSwitch("disable-gpu-compositing");


  //  DelegateSet::iterator it = delegates_.begin();
  //  for (; it != delegates_.end(); ++it)
  //    (*it)->OnBeforeCommandLineProcessing(this, command_line);
  //}


	command_line->AppendSwitch("disable-web-security");
	//http https����
	//By default, an https page cannot run JavaScript, CSS or plugins from http URLs.
	//This provides an override to get the old insecure behavior.
	command_line->AppendSwitch("allow-running-insecure-content");
	//���ô���
	//Don't use a proxy server, always make direct connections.
	//Overrides any other proxy server flags that are passed.
	command_line->AppendSwitchWithValue("no-proxy-server", "1");
	//���ƽ�������
	command_line->AppendSwitch("process-per-site");
	command_line->AppendSwitchWithValue("renderer-process-limit", "10");
	//����ƴд���
	command_line->AppendSwitch("disable-spell-checking");
	//�����ļ�����
	//By default, file:// URIs cannot read other file:// URIs.
	//This is an override for developers who need the old behavior for testing.
	command_line->AppendSwitch("allow-file-access-from-files");
	command_line->AppendSwitch("allow-universal-access-from-files");
	command_line->AppendSwitch("allow-insecure-localhost");
	//����textarea����
	command_line->AppendSwitch("disable-text-area-resize");
	//����pdf��չ
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

	//ȥ��webgl gpu extensions��֧��,���Լ����ļ�:
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
	//ʹ48px���µ������Ϊsub-pixel��Ⱦ ����cpu����
	command_line->AppendSwitch("enable-direct-write");
	//command_line->AppendSwitch("disable-direct-write");
	//command_line->AppendSwitch("off-screen-rendering-enabled");
	//command_line->AppendSwitch("enable-begin-frame-scheduling");
	//command_line->AppendSwitch("disable-accelerated-compositing");
	//����ý����
	command_line->AppendSwitch("enable-media-stream");
	command_line->AppendSwitch("enable-usermedia-screen-capturing");
	//���ù�դ
	//Number of worker threads used to rasterize content.
	command_line->AppendSwitchWithValue("num-raster-threads", "2");
	//dpi����
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
	//npapi���֧��
	command_line->AppendSwitch("enable-npapi");
	//������Ƶ�Զ�����
	command_line->AppendSwitchWithValue("autoplay-policy", "no-user-gesture-required");
}

void ClientAppBrowser::OnContextInitialized() {
  if (!cookieable_schemes_.empty()) {
    // Register cookieable schemes with the global cookie manager.
    CefRefPtr<CefCookieManager> manager =
        CefCookieManager::GetGlobalManager(NULL);
    DCHECK(manager.get());
    manager->SetSupportedSchemes(cookieable_schemes_, true, NULL);
  }

  print_handler_ = CreatePrintHandler();

  DelegateSet::iterator it = delegates_.begin();
  for (; it != delegates_.end(); ++it)
    (*it)->OnContextInitialized(this);
}

void ClientAppBrowser::OnBeforeChildProcessLaunch(
    CefRefPtr<CefCommandLine> command_line) {
  //DelegateSet::iterator it = delegates_.begin();
  //for (; it != delegates_.end(); ++it)
  //  (*it)->OnBeforeChildProcessLaunch(this, command_line);
}

void ClientAppBrowser::OnRenderProcessThreadCreated(
    CefRefPtr<CefListValue> extra_info) {
  //DelegateSet::iterator it = delegates_.begin();
  //for (; it != delegates_.end(); ++it)
  //  (*it)->OnRenderProcessThreadCreated(this, extra_info);
}

void ClientAppBrowser::OnScheduleMessagePumpWork(int64 delay) {
  // Only used when `--external-message-pump` is passed via the command-line.
//  MainMessageLoopExternalPump* message_pump =
//      MainMessageLoopExternalPump::Get();
//  if (message_pump)
//    message_pump->OnScheduleMessagePumpWork(delay);
}

}  // namespace client
