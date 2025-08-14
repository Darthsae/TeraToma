#pragma once
#ifndef UI_RECT_H
#define UI_RECT_H
#include <SDL3/SDL.h>

namespace TeraToma::UI {
    enum struct UIAnchor {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        MIDDLE_CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    };

    struct UIRect {
    public:
        SDL_FRect rect;

        void SetTopLeft(SDL_FPoint);
        void SetTopCenter(SDL_FPoint);
        void SetTopRight(SDL_FPoint);
        void SetMiddleLeft(SDL_FPoint);
        void SetMiddleCenter(SDL_FPoint);
        void SetMiddleRight(SDL_FPoint);
        void SetBottomLeft(SDL_FPoint);
        void SetBottomCenter(SDL_FPoint);
        void SetBottomRight(SDL_FPoint);

        SDL_FPoint GetTopLeft(void);
        SDL_FPoint GetTopCenter(void);
        SDL_FPoint GetTopRight(void);
        SDL_FPoint GetMiddleLeft(void);
        SDL_FPoint GetMiddleCenter(void);
        SDL_FPoint GetMiddleRight(void);
        SDL_FPoint GetBottomLeft(void);
        SDL_FPoint GetBottomCenter(void);
        SDL_FPoint GetBottomRight(void);

        void SetX(float);
        void SetY(float);
        void SetCenterX(float);
        void SetCenterY(float);
        void SetRightX(float);
        void SetBottomY(float);
        void SetWidth(float);
        void SetHeight(float);
        void SetSize(SDL_FPoint);

        void SetHalfWidth(float);
        void SetHalfHeight(float);
        void SetHalfSize(SDL_FPoint);

        float GetX(void);
        float GetY(void);
        float GetCenterX(void);
        float GetCenterY(void);
        float GetRightX(void);
        float GetBottomY(void);
        float GetWidth(void);
        float GetHeight(void);
        SDL_FPoint GetSize(void);
        
        float GetHalfWidth(void);
        float GetHalfHeight(void);
        SDL_FPoint GetHalfSize(void);
    };
    
}
#endif