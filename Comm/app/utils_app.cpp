#include "utils_app.h"
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
namespace utils
{
    namespace
    {

#define MAX_PATH_LONG 4096

        std::string ws2s(const std::wstring& ws)
        {
            std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
            setlocale(LC_ALL, "chs");
            const wchar_t* _Source = ws.c_str();
            size_t _Dsize = 2 * ws.size() + 1;
            char *_Dest = new char[_Dsize];
            memset(_Dest, 0, _Dsize);
            //wcstombs(_Dest, _Source, _Dsize);//_CRT_SECURE_NO_WARNINGS
            size_t i;
            wcstombs_s(&i, _Dest, _Dsize, _Source, _TRUNCATE);
            std::string result = _Dest;
            delete[]_Dest;
            setlocale(LC_ALL, curLocale.c_str());
            return result;
        }

        std::wstring s2ws(const std::string& s)
        {
            std::wstring wszStr;
            int nLength = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, NULL);
            wszStr.resize(nLength);
            LPWSTR lpwszStr = new wchar_t[nLength];
            MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength);
            wszStr = lpwszStr;
            delete[] lpwszStr;
            return wszStr;
        }
    }

    namespace app
    {
        //API 多线程有BUG，内部有waitforsigleobject
        //
        std::wstring GetInstancePathW()
        {
            static  std::wstring sInstancePath;
            if (!sInstancePath.empty())
            {
                return sInstancePath;
            }

            TCHAR tszModule[MAX_PATH_LONG] = { 0 };
            TCHAR tszLongModule[MAX_PATH_LONG] = { 0 };
            if (0 == ::GetModuleFileName(NULL, tszModule, MAX_PATH_LONG))
            {
                return sInstancePath;
            }
            if (0 == ::GetLongPathName(tszModule, tszLongModule, MAX_PATH_LONG))
            {
                PathRemoveFileSpec(tszModule);
                sInstancePath = tszModule;
                return sInstancePath;
            }
            wchar_t tmp_buff[MAX_PATH_LONG] = { 0 };
            //相对路径转绝对路径
            _wfullpath(tmp_buff, tszLongModule, MAX_PATH_LONG);
            //去掉应用名称
            PathRemoveFileSpec(tmp_buff);
            sInstancePath = tmp_buff;
            return sInstancePath;
        }
        std::string GetInstancePathA()
        {
            return ws2s(GetInstancePathW());
        }
        std::wstring GetExeName()
        {
            std::wstring module_path = GetModulePath();
            wchar_t* w_exe_name = PathFindFileName(module_path.c_str());
            std::wstring exe_name(w_exe_name);
            return exe_name;
        }
        std::wstring GetModulePath()
        {
			std::wstring module_path;
			wchar_t tszOriginModule[MAX_PATH_LONG] = { 0 };
			wchar_t tszLongModule[MAX_PATH_LONG] = { 0 };

			if (0 != ::GetModuleFileNameW(NULL, tszOriginModule, MAX_PATH_LONG))
			{
				if (0 != ::GetLongPathNameW(tszOriginModule, tszLongModule, MAX_PATH_LONG))
				{
					module_path = tszLongModule;
				}
				else
				{
					module_path = tszOriginModule;
				}
			}
			wchar_t tmp_buff[MAX_PATH_LONG] = { 0 };
			_wfullpath(tmp_buff, module_path.c_str(), MAX_PATH_LONG);
			module_path = tmp_buff;
			return module_path;
        }
    }//namespace app
}
