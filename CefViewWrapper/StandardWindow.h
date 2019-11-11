#pragma once
#include "CommDef.h"

//#define SUPPORT_DELEGATE \
//public:\
//	class Delegate;\
//	void SetDelegate(Delegate* dele){ delegate_= dele;} \
//	Delegate* GetDelegate(){ };\
//private:\
//	Delegate* delegate_ = nullptr;

class StandardWindow
{
	CLASS_SUPPORT_DELEGATE
public:
	DELEGATE_FUNC_BEGIN
		virtual void OnPaint(HDC dc) {}
		virtual void OnResize(int width,int height) {}
		virtual bool OnClosed() { return true; }
		virtual void OnDestoried() {}
	DELEGATE_FUNC_END

	StandardWindow();
	virtual ~StandardWindow();
	
	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetDelegate();

	HWND CreateBaseWindow(HWND parent);

	void SetGeometry(int x ,int y ,int w ,int h);

	void Show();

	void Hide();

	HWND GetHwnd();

private:
	HWND hwnd_ = NULL;
};

