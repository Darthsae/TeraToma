#include <TeraToma/UI/UIImageComponent.h>
#include <TeraToma/UI/UIElement.h>

namespace TeraToma::UI { 
    UIImageComponent::UIImageComponent(std::string_view a_texture, bool a_nineSliced) {
        texture = std::string(a_texture);
        nineSliced = a_nineSliced;
    }

    void UIImageComponent::Hookup(SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
        if (nineSliced) {
            a_element->onRender.emplace_back([this](SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
                SDL_RenderTexture9Grid(a_renderer, a_assets->textures.at(this->texture)->texture, NULL, 6, 6, 6, 6, 2, &a_element->displayArea.rect);
            });
        } else {
            a_element->onRender.emplace_back([this](SDL_Renderer* a_renderer, GameAPI* a_gameAPI, Assets::Assets* a_assets, UILayer* a_uiLayer, UIElement* a_element) {
                SDL_RenderTexture(a_renderer, a_assets->textures.at(this->texture)->texture, NULL, &a_element->displayArea.rect);
            });
        }
    }
}