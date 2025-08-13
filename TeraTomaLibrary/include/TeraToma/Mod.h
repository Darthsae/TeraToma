#pragma once
#ifndef MOD_H
#define MOD_H
#include <string>
#include <vector>
#include <Windows.h>

namespace TeraToma {
    class Mod;
}

#include <TeraToma/GameAPI.h>

namespace TeraToma {
    typedef void (__stdcall *ModLoadFnptr)(GameAPI* const);
    typedef void (__stdcall *ModInitializeFnptr)(GameAPI* const);
    typedef void (__stdcall *ModUninitializeFnptr)(GameAPI* const);
    typedef void (__stdcall *ModUnloadFnptr)(GameAPI* const);

    class Mod {
    public:
        Mod(std::string, std::wstring);

        std::string name;
        HINSTANCE library;
        bool enabled = true;

        ModLoadFnptr modLoadFnptr = nullptr;
        ModInitializeFnptr modInitializeFnptr = nullptr;
        ModUninitializeFnptr modUninitializeFnptr = nullptr;
        ModUnloadFnptr modUnloadFnptr = nullptr;

        void Load(GameAPI* const);
        void Initialize(GameAPI* const);
        void Uninitialize(GameAPI* const);
        void Unload(GameAPI* const);

        ~Mod();
    };
}
#endif