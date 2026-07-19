#pragma once

//////////////////////////////////////////////////////////////////////////
#if defined(WINDOWS_OS)
	#define PULVIS_DLL_EXPORT __declspec(dllexport)
	#define PULVIS_DLL_IMPORT __declspec(dllimport)
#elif defined(MAC_OS)
	#define PULVIS_DLL_EXPORT __attribute__((visibility("default")))
	#define PULVIS_DLL_IMPORT __attribute__((visibility("default")))
#endif

//////////////////////////////////////////////////////////////////////////
#define PULVIS_SYMBOL_EXPORT extern "C" PULVIS_DLL_EXPORT

//////////////////////////////////////////////////////////////////////////
#if defined(PULVIS_RTTI_EXPORTS)
	#define PULVIS_DLL_API PULVIS_DLL_EXPORT
#else
	#define PULVIS_DLL_API PULVIS_DLL_IMPORT
#endif