#pragma once
//����CEF�Ķര��Ӧ�÷�װ
//���нӿ��ַ���Ӧʹ��utf-8��Ϊ����


#ifdef LIB_CEF_STATIC
	#define LIB_CEF_WRAPPER
#else 
	#ifdef LIB_EXPORT
		#define LIB_CEF_WRAPPER __declspec(dllexport)
	#else 
		#define LIB_CEF_WRAPPER __declspec(dllimport)
	#endif //LIB_EXPORT
#endif //LIB_CEF_STATIC

class CefWindowView
{
public:
	class Delegate
	{
	public:
		virtual void onClosed() {};

		virtual void onLoadStatus(CefWindowView*) {};
	};
	CefWindowView(HWND parent) {}
	virtual ~CefWindowView() {}

	virtual void Show() = 0;

	virtual void Hide() = 0;

	virtual void LoadUrl(const char * url)=0;

	virtual void SetGeometry(int x, int y, int w, int h) = 0;

	virtual void GoBack() = 0;

	virtual void GoForward() = 0;

	virtual HWND GetBrowserHolderWindow() = 0;

	virtual void Resize(int w ,int h) = 0;
	
	virtual void Close(bool force) = 0;
};


using  WebResItem = struct
{
	//type : 0 = file or  1=directory
	int type = 0;
	//����ͷ��·��
	char url_path[2048] = { 0 };
	//����·��
	char local_path[2048] = { 0 };
	//����
	char password[256] = { 0 };
	//˳��
	int order = 0;
	//Ψһ��ʶ
	char identifier[128] = { 0 };
};




class CefViewWrapper
{
public:
	CefViewWrapper() {};
	virtual ~CefViewWrapper() {};

	virtual void SetGlobalRegisterJS(const char * js_data/**/,int len) = 0;

	virtual void AddWebResItem(const WebResItem& res) = 0;

	virtual void SetCachedDirector(const wchar_t * directory,int len)=0;

	virtual void SetWindowBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a)=0;

	virtual bool  Init() = 0 ;

	virtual void UnInit() = 0;

	virtual CefWindowView * CreateCefView( HWND  parent ) = 0;

	virtual void DeleteCefView(CefWindowView *)  =0;
};

extern "C" LIB_CEF_WRAPPER CefViewWrapper* CreateInstance();
extern "C" LIB_CEF_WRAPPER void DeleteInstance(CefViewWrapper*);
