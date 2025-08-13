#pragma once
#ifndef DLL_EXPORTS_H
#define DLL_EXPORTS_H
#ifdef DLL_EXPORTS
    #define MOD_API __declspec(dllexport)
#else
    #define MOD_API __declspec(dllimport)
#endif
#endif