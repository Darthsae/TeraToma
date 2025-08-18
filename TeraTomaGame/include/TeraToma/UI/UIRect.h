#pragma once
#ifndef UI_RECT_H
#define UI_RECT_H
#include <SDL3/SDL.h>

namespace TeraToma::UI {
    /// @brief 
    enum struct UIAnchor {
        TOP_LEFT,      ///< @brief a
        TOP_CENTER,    ///< @brief a
        TOP_RIGHT,     ///< @brief a
        MIDDLE_LEFT,   ///< @brief a
        MIDDLE_CENTER, ///< @brief a
        MIDDLE_RIGHT,  ///< @brief a
        BOTTOM_LEFT,   ///< @brief a
        BOTTOM_CENTER, ///< @brief a
        BOTTOM_RIGHT   ///< @brief a
    };

    /// @brief 
    struct UIRect {
    public:
        /// @brief 
        SDL_FRect rect;

        /// @brief 
        /// @param 
        void SetTopLeft(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetTopCenter(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetTopRight(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetMiddleLeft(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetMiddleCenter(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetMiddleRight(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetBottomLeft(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetBottomCenter(SDL_FPoint);
        /// @brief 
        /// @param 
        void SetBottomRight(SDL_FPoint);

        /// @brief 
        /// @return 
        SDL_FPoint GetTopLeft(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetTopCenter(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetTopRight(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetMiddleLeft(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetMiddleCenter(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetMiddleRight(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetBottomLeft(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetBottomCenter(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetBottomRight(void);

        /// @brief 
        /// @param 
        void SetX(float);
        /// @brief 
        /// @param 
        void SetY(float);
        /// @brief 
        /// @param 
        void SetCenterX(float);
        /// @brief 
        /// @param 
        void SetCenterY(float);
        /// @brief 
        /// @param 
        void SetRightX(float);
        /// @brief 
        /// @param 
        void SetBottomY(float);
        /// @brief 
        /// @param 
        void SetWidth(float);
        /// @brief 
        /// @param 
        void SetHeight(float);
        /// @brief 
        /// @param 
        void SetSize(SDL_FPoint);

        /// @brief 
        /// @param 
        void SetHalfWidth(float);
        /// @brief 
        /// @param 
        void SetHalfHeight(float);
        /// @brief 
        /// @param 
        void SetHalfSize(SDL_FPoint);

        /// @brief 
        /// @return 
        float GetX(void);
        /// @brief 
        /// @return 
        float GetY(void);
        /// @brief 
        /// @return 
        float GetCenterX(void);
        /// @brief 
        /// @return 
        float GetCenterY(void);
        /// @brief 
        /// @return 
        float GetRightX(void);
        /// @brief 
        /// @return 
        float GetBottomY(void);
        /// @brief 
        /// @return 
        float GetWidth(void);
        /// @brief 
        /// @return 
        float GetHeight(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetSize(void);
        
        /// @brief 
        /// @return 
        float GetHalfWidth(void);
        /// @brief 
        /// @return 
        float GetHalfHeight(void);
        /// @brief 
        /// @return 
        SDL_FPoint GetHalfSize(void);
    };
    
}
#endif