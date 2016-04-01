/*
 * =====================================================================================
 *
 *       Filename: sdldevice.hpp
 *        Created: 03/07/2016 23:57:04
 *  Last Modified: 04/01/2016 00:18:54
 *
 *    Description: copy from flare-engine:
 *				   SDLHardwareRenderDevice.h/cpp
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once
#include "colorfunc.hpp"
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class SDLDevice final
{
    public:
        SDLDevice();
       ~SDLDevice();

    public:
       SDL_Texture *CreateTexture(const uint8_t *, size_t);

    public:
       void SetWindowIcon();
       void DrawTexture(SDL_Texture *, int, int);
       void DrawTexture(SDL_Texture *, int, int, int, int, int, int);

    public:
       void Present()
       {
           SDL_RenderPresent(m_Renderer);
       }

       void SetWindowTitle(const char *szUTF8Title)
       {
           SDL_SetWindowTitle(m_Window, (szUTF8Title) ? szUTF8Title : "");
       }

       void SetGamma(double fGamma)
       {
           Uint16 pRawRamp[256];
           SDL_CalculateGammaRamp((float)(std::min(std::max(fGamma, 0.0), 1.0)), pRawRamp);
           SDL_SetWindowGammaRamp(m_Window, pRawRamp, pRawRamp, pRawRamp);
       }

       void ClearScreen()
       {
           SetColor(0, 0, 0, 0);
           SDL_RenderClear(m_Renderer);
       }

       void DrawLine(int nX, int nY, int nW, int nH)
       {
           SDL_RenderDrawLine(m_Renderer, nX, nY, nX + nW, nY + nH);
       }

       void SetColor(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nA)
       {
           SDL_SetRenderDrawColor(m_Renderer, nR, nG, nB, nA);
       }

       void PushColor(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nA)
       {
           uint32_t nRGBA = Color2U32RGBA(MakeColor(nR, nG, nB, nA));
           if(nRGBA != m_ColorStack.back()){
               SetColor(nR, nG, nB, nA);
           }
           m_ColorStack.push_back(nRGBA);
       }

       void PopColor()
       {
           uint32_t nOldRGBA = m_ColorStack.back();
           m_ColorStack.pop_back();

           if(m_ColorStack.back() != nOldRGBA){
               SDL_Color stColor = U32RGBA2Color(m_ColorStack.back());
               SetColor(stColor.r, stColor.g, stColor.b, stColor.a);
           }
       }

       void FillRectangle(int nX, int nY, int nW, int nH)
       {
           // TODO
           // boundary check??
           SDL_Rect stRect;
           stRect.x = nX;
           stRect.y = nY;
           stRect.w = nW;
           stRect.h = nH;

           SDL_RenderFillRect(m_Renderer, &stRect);
       }

       SDL_Color Color()
       {
           return U32RGBA2Color(m_ColorStack.back());
       }

       void DrawPixel(int nX, int nY)
       {
           SDL_RenderDrawPoint(m_Renderer, nX, nY);
       }

       void DrawRectangle(int nX, int nY, int nW, int nH)
       {
           DrawLine(nX     , nY     , nX + nW, nY     );
           DrawLine(nX     , nY     , nX     , nY + nH);
           DrawLine(nX + nW, nY     , nX + nW, nY + nH);
           DrawLine(nX     , nY + nH, nX + nW, nY + nH);
       }

       SDL_Texture *CreateTextureFromSurface(SDL_Surface * pSurface)
       {
           if(pSurface){
               return SDL_CreateTextureFromSurface(m_Renderer, pSurface);
           }
           return nullptr;
       }

       int WindowW(bool bRealWindowSizeInPixel)
       {
           int nW, nH;
           if(bRealWindowSizeInPixel){
               SDL_GetWindowSize(m_Window, &nW, &nH);
               return nW;
           }else{
               return m_WindowW;
           }
       }

       int WindowH(bool bRealWindowSizeInPixel)
       {
           int nW, nH;
           if(bRealWindowSizeInPixel){
               SDL_GetWindowSize(m_Window, &nW, &nH);
               return nH;
           }else{
               return m_WindowH;
           }
       }

    public:

       TTF_Font *CreateTTF(const uint8_t *, size_t, uint8_t);

    private:
       // for graphics hardware
       SDL_Window   *m_Window;
       SDL_Renderer *m_Renderer;

    private:
       std::vector<uint32_t> m_ColorStack;

    private:
       int m_WindowW;
       int m_WindowH;

    private:
       // for sound
};
