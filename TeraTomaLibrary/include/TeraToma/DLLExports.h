#pragma once
#ifndef DLL_EXPORTS_H
#define DLL_EXPORTS_H
#if defined(_WIN32) || defined(_WIN64)
    #ifdef DLL_EXPORTS
        #define MOD_API __declspec(dllexport)
    #else
        #define MOD_API __declspec(dllimport)
    #endif
#endif
#if defined(__linux__) || defined(__unix__) || defined(linux)
    #define MOD_API
#endif
#if defined(__APPLE__)
    #define MOD_API
#endif
#endif