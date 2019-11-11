#include "env_base.h"
#include <Windows.h>
#include <shlwapi.h>
#include <map>
#include <mutex>
#pragma comment(lib,"shlwapi.lib")
#define MAX_PATH_LONG 4096
namespace
{
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


class BaseEnv
{
public:
    BaseEnv() {}
    ~BaseEnv() {}
    static void Load()
    {
        std::wstring intance_path;
        std::wstring module_path;
        std::wstring exe_name;
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

		PathRemoveFileSpecW(tmp_buff);
		intance_path = tmp_buff;
		wchar_t* w_exe_name= PathFindFileName(module_path.c_str());
        exe_name = w_exe_name;
        Set("instance_path", ws2s(intance_path));
        Set("module_path", ws2s(module_path));
        Set("exe_name", ws2s(exe_name));
    }

    static void Clear()
    {
        env_map_.clear();
    }

    static void Set(const std::string& key, const std::string& val)
    {
        std::lock_guard<std::mutex> locker(map_mutex_);
        env_map_.insert_or_assign(key, val);
    }
    static std::string Get(const std::string& key)
    {
        std::lock_guard<std::mutex> locker(map_mutex_);
        return env_map_[key];
    }
private:
    static std::map<std::string, std::string> env_map_;
    static std::mutex map_mutex_;
};

std::map<std::string, std::string> BaseEnv::env_map_;
std::mutex BaseEnv::map_mutex_;

namespace env
{
    void Load()
    {
        BaseEnv::Load();
    }

    void Clear()
    {
        BaseEnv::Clear();
    }

    template<>
    void Set(const std::string& key, const std::string& val)
    {
        BaseEnv::Set(key, val);
    }
    template<>
    void Set(const std::string& key, const bool& val)
    {
        std::string val_str = val ? "true" : "false";
        Set(key, val_str);
    }
    template<>
    void Set(const std::string& key, const int& val)
    {
        std::string val_str = std::to_string(val);
        Set(key, val_str);
    }
    template<>
    void Set(const std::string& key, const long& val)
    {
        std::string val_str = std::to_string(val);
        Set(key, val_str);
    }
    template<>
    void Set(const std::string& key, const double& val)
    {
        std::string val_str = std::to_string(val);
        Set(key, val_str);
    }


    template<>
    const std::string Get(const std::string& key)
    {
        return BaseEnv::Get(key);
    }
    template<>
    const bool Get(const std::string& key)
    {
        std::string val_str = Get<std::string>(key);
        bool ret = "true" == val_str || "1" == val_str;
        return ret;
    }
    template<>
    const int Get(const std::string& key)
    {
        std::string val_str = Get<std::string>(key);
        int ret = 0;
        if (!val_str.empty())
        {
            try
            {
                ret = std::stoi(val_str);
            }
            catch (...)
            {

            }
        }
        return ret;
    }

    template<>
    const long Get(const std::string& key)
    {
        std::string val_str = Get<std::string>(key);
        long ret = 0l;
        if (!val_str.empty())
        {
            try
            {
                ret = std::stol(val_str);
            }
            catch (...)
            {

            }
        }
        return ret;
    }

    template<>
    const double Get(const std::string& key)
    {
        std::string val_str = Get<std::string>(key);
        double ret = .0;
        if (!val_str.empty())
        {
            try
            {
                ret = std::stod(val_str);
            }
            catch (...)
            {

            }
        }
        return ret;
    }


    const std::string InstancePath()
    {
        return Get<std::string>("instance_path");
    }
    const std::string ModulePath()
    {
        return Get<std::string>("module_path");
    }
    const std::string ExeName()
    {
        return Get<std::string>("exe_name");
    }
}