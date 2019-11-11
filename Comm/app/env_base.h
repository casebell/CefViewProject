#pragma once
#include <string>

class BaseEnv;
namespace env
{
    void Load();
	void Clear();

    template<typename T>
    void Set(const std::string&, const T&)
    {
		throw std::logic_error("env::Set not support type.");
    }
	template<>
	void Set(const std::string&, const std::string&);
	template<>
	void Set(const std::string&, const bool&);
	template<>
	void Set(const std::string&, const int&);
	template<>
	void Set(const std::string&, const long&);
	template<>
	void Set(const std::string&, const double&);

	template<typename T>
	const T Get(const std::string&)
	{
		throw std::logic_error("env::Get not support type.");
	}
	template<>
    const std::string Get(const std::string&);
	template<>
	const bool Get(const std::string&);
	template<>
	const int Get(const std::string&);
	template<>
	const long Get(const std::string&);
	template<>
	const double Get(const std::string&);

    const std::string InstancePath();
    const std::string ModulePath();
	const std::string ExeName();
}