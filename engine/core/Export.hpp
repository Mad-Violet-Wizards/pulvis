#pragma once

#ifdef PULVIS_EXPORTS
#define PULVIS_API __declspec(dllexport)
#else
#define PULVIS_API __declspec(dllimport)
#endif