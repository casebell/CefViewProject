#include "stdafx.h"
#include "StandardWindow.h"

#define STTANDARD_CLASS_NAME _T("StandardWnd")

StandardWindow::StandardWindow()
{
}


StandardWindow::~StandardWindow()
{
}

LRESULT CALLBACK StandardWindow::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	StandardWindow *  target_obj = (StandardWindow *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (message)                  /* handle the messages */
	{
	case WM_NCCREATE:
	{
		StandardWindow* remote = (StandardWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)remote);
		remote->hwnd_ = hwnd;
	}
	break;
	case WM_NCDESTROY:
	{
		target_obj->hwnd_ = NULL;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)NULL);
	}
	break;
	case WM_CLOSE:
	{
		if (target_obj->delegate_)
		{
			if (!target_obj->delegate_->OnClosed())
			{
				return true;
			}
		}
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		if (target_obj->delegate_)
		{
			target_obj->delegate_->OnDestoried();
		}
	}
	break;
	case WM_SIZE:
	{
		if (target_obj->delegate_)
		{
			target_obj->delegate_->OnResize(LOWORD(lParam), HIWORD(lParam));
		}
	}
	break;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

HWND StandardWindow::CreateBaseWindow(HWND parent_hwnd)
{
	WNDCLASSEX wincl;        /* Data structure for the window class */

   /* The Window structure */
	wincl.hInstance = GetModuleHandle(NULL);
	wincl.lpszClassName = STTANDARD_CLASS_NAME;//+-69+
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	/* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	/* Register the window class, and if it fails quit the program */
	RegisterClassEx(&wincl);

	hwnd_ = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,             /* Extended possibilites for variation */
		STTANDARD_CLASS_NAME,         /* class name */
		STTANDARD_CLASS_NAME,       /* Title Text */
		WS_POPUP, /* default window */
		0,       /* Windows decides the position */
		0,       /* where the window ends up on the screen */
		1,                 /* The programs width */
		1,                 /* and height in pixels */
		parent_hwnd,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		GetModuleHandle(NULL),       /* Program Instance handler */
		this                 /* No Window Creation data */
	);
	return hwnd_;
}

void StandardWindow::SetGeometry(int x, int y, int w, int h)
{
	MoveWindow(hwnd_, x, y, w, h, true);
}

void StandardWindow::Show()
{
	ShowWindow(hwnd_, SW_SHOW);
}

void StandardWindow::Hide()
{
	ShowWindow(hwnd_, SW_HIDE);
}

HWND StandardWindow::GetHwnd()
{
	return hwnd_;
}
