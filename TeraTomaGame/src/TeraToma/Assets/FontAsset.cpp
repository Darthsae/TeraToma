#include <TeraToma/Assets/FontAsset.h>
#include <iostream>

using namespace TeraToma::Assets;

FontAsset::FontAsset(std::string a_id, std::string a_name, float a_ptsize) {
    this->id = a_id;
    this->name = a_name;
    this->ptsize = a_ptsize;
}

void FontAsset::Load(std::string a_path) {
    font = TTF_OpenFont(a_path.c_str(), ptsize);
}

void FontAsset::Unload() {
    TTF_CloseFont(font);
}