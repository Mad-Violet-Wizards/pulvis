#include "DynamicLibrary.hpp"

#include <string>

#if defined(WINDOWS_OS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(MAC_OS)
	#include <dlfcn.h>
#endif

namespace pulvis::core
{
	CDynamicLibrary::CDynamicLibrary()
		: m_Handle(nullptr)
	{
	}

	CDynamicLibrary::CDynamicLibrary(std::string_view _path)
	{
		const std::string path(_path);

#if defined(WINDOWS_OS)
		m_Handle = static_cast<void*>(LoadLibraryA(path.c_str()));
#elif defined(MAC_OS)
		m_Handle = ::dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
	}

	CDynamicLibrary::~CDynamicLibrary()
	{
		Unload();
	}

	CDynamicLibrary::CDynamicLibrary(CDynamicLibrary&& _other) noexcept
		: m_Handle(_other.m_Handle)
	{
		_other.m_Handle = nullptr;
	}

	CDynamicLibrary& CDynamicLibrary::operator=(CDynamicLibrary&& _other) noexcept
	{
		if (this != &_other)
		{
			Unload();
			m_Handle = _other.m_Handle;
			_other.m_Handle = nullptr;
		}
		return *this;
	}

	bool CDynamicLibrary::IsLoaded() const noexcept
	{
		return m_Handle != nullptr;
	}

	void* CDynamicLibrary::GetSymbol(std::string_view _symbol_name) const noexcept
	{
		if (!IsLoaded())
		{
			return nullptr;
		}

		const std::string symbol_name(_symbol_name);

#if defined(WINDOWS_OS)
		return reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(m_Handle), symbol_name.c_str()));
#elif defined(MAC_OS)
		return ::dlsym(m_Handle, symbol_name.c_str());
#endif
		return nullptr;
	}

	void CDynamicLibrary::Unload() noexcept
	{
		if (!IsLoaded())
		{
			return;
		}

#if defined(WINDOWS_OS)
		::FreeLibrary(static_cast<HMODULE>(m_Handle));
#elif defined(MAC_OS)
		::dlclose(m_Handle);
#endif

		m_Handle = nullptr;
	}
}