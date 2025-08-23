#pragma once
#ifndef MOD_H
#define MOD_H
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#define TeraTomaLibraryHandle HINSTANCE
#define TeraTomaLoadLibrary(a_libraryName) LoadLibraryW(a_libraryName.c_str())
#define TeraTomaLoadFunction(a_libraryHandle, a_functionName) GetProcAddress(a_libraryHandle, a_functionName)
#define TeraTomaCloseLibrary(a_libraryHandle) FreeLibrary(a_libraryHandle)
#define TeraTomaCallPrefix __stdcall
#endif
#if defined(__linux__) || defined(__unix__) || defined(linux)
#include <dlfcn.h>
#define TeraTomaLibraryHandle void*
#define TeraTomaLoadLibrary(a_libraryName) dlopen(a_libraryName.c_str(), RTLD_LAZY)
#define TeraTomaLoadFunction(a_libraryHandle, a_functionName) dlsym(a_libraryHandle, a_functionName)
#define TeraTomaCloseLibrary(a_libraryHandle) dlclose(a_libraryHandle)
#define TeraTomaCallPrefix __cdecl
#endif

namespace TeraToma {
    class Mod;
}

#include <TeraToma/GameAPI.h>

namespace TeraToma {
    typedef void (TeraTomaCallPrefix *ModLoadFnptr)(GameAPI* const);
    typedef void (TeraTomaCallPrefix *ModInitializeFnptr)(GameAPI* const);
    typedef void (TeraTomaCallPrefix *ModUninitializeFnptr)(GameAPI* const);
    typedef void (TeraTomaCallPrefix *ModUnloadFnptr)(GameAPI* const);

    /// @brief 
    class Mod {
    public:
        /// @brief 
        /// @param  
        /// @param  
        Mod(std::string, std::wstring);

        /// @brief 
        std::string name;
        /// @brief 
        TeraTomaLibraryHandle library;
        /// @brief 
        bool enabled = true;

        /// @brief 
        ModLoadFnptr modLoadFnptr = nullptr;
        /// @brief 
        ModInitializeFnptr modInitializeFnptr = nullptr;
        /// @brief 
        ModUninitializeFnptr modUninitializeFnptr = nullptr;
        /// @brief 
        ModUnloadFnptr modUnloadFnptr = nullptr;

        /// @brief 
        /// @param  
        void Load(GameAPI* const);
        /// @brief 
        /// @param  
        void Initialize(GameAPI* const);
        /// @brief 
        /// @param  
        void Uninitialize(GameAPI* const);
        /// @brief 
        /// @param  
        void Unload(GameAPI* const);

        /// @brief 
        ~Mod();
    };
}
#endif
