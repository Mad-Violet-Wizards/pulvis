#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <iostream>

template <class C>
class Singleton
{

public:

	~Singleton()
	{
		Destroy();
	}

	static void Destroy()
	{
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	static C& GetInstance() noexcept 
	{
		if (!s_Instance)
		{
			s_Instance = new C();
		}

		return *s_Instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	static inline C* s_Instance{ nullptr };

	Singleton() noexcept = default;
};