#pragma once

#if defined (_WIN32) || defined(_WIN64)
    #ifdef PULVIS_EXPORTS
        #define PULVIS_API __declspec(dllexport)
    #else
        #define PULVIS_API __declspec(dllimport)
    #endif
#elif defined (__APPLE__)
    #ifdef PULVIS_EXPORTS
        #define PULVIS_API __attribute__((visibility("default")))
    #else
        #define PULVIS_API
    #endif
#endif