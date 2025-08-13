#include <TeraToma/UI/UITextComponent.h>
#include <TeraToma/UI/UIElement.h>
#include <print>

namespace TeraToma::UI {
    UITextComponent::UITextComponent(std::string a_text) {
        text = a_text;
    }

    void UITextComponent::Hookup(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
        std::println("Woah: \"{}\"", this->text.c_str());
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(a_assets->fonts.at("NotoSansMono-Regular").font, this->text.c_str(), this->text.size(), {255, 255, 255, SDL_ALPHA_OPAQUE}, (int)a_element->displayArea.GetWidth());
        textTexture = SDL_CreateTextureFromSurface(a_renderer, surface);
        SDL_DestroySurface(surface);

        a_element->onRecalculate.emplace_back([this](SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
            SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(a_assets->fonts.at("NotoSansMono-Regular").font, this->text.c_str(), this->text.size(), {255, 255, 255, SDL_ALPHA_OPAQUE}, (int)a_element->displayArea.GetWidth());
            textTexture = SDL_CreateTextureFromSurface(a_renderer, surface);
            SDL_DestroySurface(surface);
        });
        
        a_element->onRender.emplace_back([this](SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
            UIRect proper = {{a_element->displayArea.GetX(), a_element->displayArea.GetY(), (float)textTexture->w, (float)textTexture->h}};
            proper.SetMiddleCenter(a_element->displayArea.GetMiddleCenter());
            SDL_RenderTexture(a_renderer, textTexture, NULL, &proper.rect);
        });
    }

    void UITextComponent::SetText(SDL_Renderer* a_renderer, Assets::Assets* a_assets, UIElement* a_element, std::string a_text) {
        SDL_DestroyTexture(textTexture);
        text = a_text;
        std::println("Woah: \"{}\"", this->text.c_str());
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(a_assets->fonts.at("NotoSansMono-Regular").font, this->text.c_str(), this->text.size(), {255, 255, 255, SDL_ALPHA_OPAQUE}, (int)a_element->displayArea.GetWidth());
        textTexture = SDL_CreateTextureFromSurface(a_renderer, surface);
        SDL_DestroySurface(surface);
    }
}