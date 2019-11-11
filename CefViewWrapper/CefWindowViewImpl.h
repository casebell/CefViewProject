#pragma once
#include "CefViewWrapper.h"
#include "StandardWindow.h"
#include "simple_handler.h"

class SimpleHandler;

class CefWindowViewImpl : public CefWindowView
	, public StandardWindow::Delegate
	,public SimpleHandler::Delegate
{
public:
	CefWindowViewImpl(HWND parent);
	~CefWindowViewImpl();

	virtual void Show() override;

	virtual void Hide() override;

	virtual void LoadUrl(const char * url) override;

	virtual void SetGeometry(int x, int y, int w, int h) override;

	virtual void GoBack() override;

	virtual void GoForward() override;

	virtual HWND GetBrowserHolderWindow() override;

	HWND GetBrowserWindow();



	virtual void OnPaint(HDC dc) override;

	virtual void OnResize(int width, int height) override;


	virtual void OnBrowserCreated() override;

	virtual void Resize(int w, int h) override;

	virtual void AdjustSize();

	virtual void OnBrowserDestroyed() override;


	virtual void Close(bool force) override;

	virtual bool OnClosed() override;



	virtual void OnDestoried() override;

private:
	StandardWindow standard_wind_;
	bool is_closing_ = false;
	CefRefPtr <SimpleHandler> hander_ = nullptr;
	int close_thread_id_ = 0;
};

