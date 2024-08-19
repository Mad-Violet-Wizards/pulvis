#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <iostream>

template <class C>
class Singleton
{

public:
	template <typename... Args>
	static void Init(Args&&... args)
	{
		static_assert(!std::is_copy_constructible_v<C>);
		static_assert(!std::is_move_constructible_v<C>);
		static_assert(!std::is_copy_assignable_v<C>);
		static_assert(!std::is_move_assignable_v<C>);

		if (s_Instance)
		{
			std::cout << "[Singleton] " << std::type_index(typeid(s_Instance)).name() << "already initialized.\n";
		}

		s_Instance = new C(std::forward<Args>(args)...);
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
		return *s_Instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	static inline C* s_Instance{ nullptr };

	Singleton() = default;

	~Singleton() noexcept = default;
};