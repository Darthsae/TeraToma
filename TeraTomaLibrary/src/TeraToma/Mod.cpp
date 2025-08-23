#include <TeraToma/Mod.h>
#include <print>

std::string StringFromWString(std::wstring a_wstring) {
    size_t len = wcstombs(nullptr, a_wstring.c_str(), 0) + 1;
    char* buffer = new char[len];
    wcstombs(buffer, a_wstring.c_str(), len);
    std::string str(buffer);
    delete[] buffer;
    return str;
}

namespace TeraToma {
    Mod::Mod(std::string a_modName, std::wstring a_path) {
        //std::println("We are loading the mod.");
        name = a_modName;
        library = TeraTomaLoadLibrary(a_path);
        modLoadFnptr = (ModLoadFnptr)TeraTomaLoadFunction(library, "ModLoad");
        modInitializeFnptr = (ModInitializeFnptr)TeraTomaLoadFunction(library, "ModInitialize");
        modUninitializeFnptr = (ModUninitializeFnptr)TeraTomaLoadFunction(library, "ModUninitialize");
        modUnloadFnptr = (ModUnloadFnptr)TeraTomaLoadFunction(library, "ModUnload");
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
        TeraTomaCloseLibrary(library);
    }
}
