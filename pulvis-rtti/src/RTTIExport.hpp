#pragma once

//////////////////////////////////////////////////////////////////////////
// pulvis-rtti is built as a shared library (DLL) so that its global type
// registries (CRTTIClassStorage, CRTTIEnumStorage) live in a single
// instance shared across every consumer (the engine and any future
// editor/game/plugin DLLs).
//
// PULVIS_RTTI_API resolves to dllexport while building the rtti DLL
// (PULVIS_RTTI_EXPORTS is defined only on that project) and to dllimport
// when consumed elsewhere. It is a no-op on non-Windows platforms.
//////////////////////////////////////////////////////////////////////////
#if defined(WINDOWS_OS)
	#if defined(PULVIS_RTTI_EXPORTS)
		#define PULVIS_RTTI_API __declspec(dllexport)
	#else
		#define PULVIS_RTTI_API __declspec(dllimport)
	#endif
#else
	#define PULVIS_RTTI_API
#endif
