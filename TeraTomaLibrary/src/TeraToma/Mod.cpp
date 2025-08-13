#include <TeraToma/Mod.h>
#include <print>

namespace TeraToma {
    Mod::Mod(std::string a_modName, std::wstring a_path) {
        //std::println("We are loading the mod.");
        name = a_modName;
        library = LoadLibraryW(a_path.c_str());
        modLoadFnptr = (ModLoadFnptr)GetProcAddress(library, "ModLoad");
        modInitializeFnptr = (ModInitializeFnptr)GetProcAddress(library, "ModInitialize");
        modUninitializeFnptr = (ModUninitializeFnptr)GetProcAddress(library, "ModUninitialize");
        modUnloadFnptr = (ModUnloadFnptr)GetProcAddress(library, "ModUnload");
        //std::println("Lib {} Load {} Init {} Uninit {} Unload {}", library != NULL, modLoadFnptr != NULL, modInitializeFnptr != NULL, modUninitializeFnptr != NULL, modUnloadFnptr != NULL);
    }

    void Mod::Load(GameAPI* const a_gameAPI) {
        if (modLoadFnptr != NULL) {
            modLoadFnptr(a_gameAPI);
        }
    }

    void Mod::Initialize(GameAPI* const a_gameAPI) {
        if (modInitializeFnptr != NULL) {
            modInitializeFnptr(a_gameAPI);
        }
    }

    void Mod::Uninitialize(GameAPI* const a_gameAPI) {
        if (modUninitializeFnptr != NULL) {
            modUninitializeFnptr(a_gameAPI);
        }
    }

    void Mod::Unload(GameAPI* const a_gameAPI) {
        if (modUnloadFnptr != NULL) {
            modUnloadFnptr(a_gameAPI);
        }
    }

    Mod::~Mod() {
        FreeLibrary(library);
    }
}