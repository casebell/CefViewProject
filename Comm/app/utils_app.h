#pragma once
#include <string>
namespace utils {
	namespace app {
		std::wstring GetInstancePathW();
		std::string GetInstancePathA();
		std::wstring GetExeName();
		std::wstring GetModulePath();
	}//namespace app
}