/*
   MIT License

   Copyright (c) 2019 Berkay Yigit <berkay2578@gmail.com>
       Copyright holder detail: Nickname(s) used by the copyright holder: 'berkay2578', 'berkayylmao'.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#pragma once
#include "stdafx.h"
#include "Extensions\Extensions.h"
#include "d3dx9.h"

namespace Extensions {
   namespace InGameMenu {
      namespace Hooks::NewLoadingScreens {
         inline bool isShowingLoadingScreen = false;
         static void __declspec(naked) hkLoadingScreen_LoadScreen() {
            __asm {
               mov[Extensions::InGameMenu::hasUserSeenFirstLoadingScreen], 1
               mov[isShowingLoadingScreen], 1
               mov edx, [ecx]
               mov edx, [edx+0x5C]
               ret
            }
         }
         static void __declspec(naked) hkLoadingScreen_UnloadScreen() {
            __asm {
               mov[isShowingLoadingScreen], 0
               pop edi
               pop esi
               mov esp, ebp
               pop ebp
               retn 0x10
            }
         }
      }

      struct NewLoadingScreens : _BaseInGameMenuItem {
         struct LoadingScreenTextures {
            struct BackgroundsByRatios {
               LPDIRECT3DTEXTURE9
                  _3by2, _4by3, _5by4,
                  _16by9, _16by10,
                  _21by9, _32by9,
                  _683by358;
            };

            BackgroundsByRatios backgrounds;
            LPDIRECT3DTEXTURE9  loadingSpiral;

            void createTextures() {
               auto pDevice = LocalMirrorHook::D3D9::GetD3D9Device();
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_CIRCLE.png", &loadingSpiral);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_3by2.png", &backgrounds._3by2);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_4by3.png", &backgrounds._4by3);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_5by4.png", &backgrounds._5by4);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_16by9.png", &backgrounds._16by9);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_16by10.png", &backgrounds._16by10);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_21by9.png", &backgrounds._21by9);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_32by9.png", &backgrounds._32by9);
               D3DXCreateTextureFromFile(pDevice, "ANewWorld\\UI_LOADING_BACKGROUND_683by358.png", &backgrounds._683by358);
            }
            void releaseTextures() {
               loadingSpiral->Release();
               backgrounds._3by2->Release();
               backgrounds._4by3->Release();
               backgrounds._5by4->Release();
               backgrounds._16by9->Release();
               backgrounds._16by10->Release();
               backgrounds._21by9->Release();
               backgrounds._32by9->Release();
               backgrounds._683by358->Release();
            }
         };
         LoadingScreenTextures textures ={};

         const virtual void loadData() override {
            Memory::writeCall(0x897A6, (DWORD)&Hooks::NewLoadingScreens::hkLoadingScreen_LoadScreen, false);
            Memory::writeJMP(0x896FE, (DWORD)&Hooks::NewLoadingScreens::hkLoadingScreen_UnloadScreen, false);

            auto pDevice = LocalMirrorHook::D3D9::GetD3D9Device();
            textures.createTextures();

            hasLoadedData = true;
         }

         const virtual void onFrame() override {
            static float collectiveDeltaTime = 0.0f;

            if (Hooks::NewLoadingScreens::isShowingLoadingScreen) {
               ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
               ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
               ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
               ImGui::SetNextWindowSize(pImGuiIO->DisplaySize * 2.0f, ImGuiCond_Appearing);
               if (ImGui::Begin("###NewLoadingScreen", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground)) {
                  // Background
                  {
                     static LPDIRECT3DTEXTURE9* pBackgroundTexture = nullptr;
                     double aspectRatio = (double)pImGuiIO->DisplaySize.x / (double)pImGuiIO->DisplaySize.y;
                     if (aspectRatio <= 1.30) {
                        pBackgroundTexture = &textures.backgrounds._5by4;
                     } else if (aspectRatio <= 1.38) {
                        pBackgroundTexture = &textures.backgrounds._4by3;
                     } else if (aspectRatio <= 1.55) {
                        pBackgroundTexture = &textures.backgrounds._3by2;
                     } else if (aspectRatio <= 1.65) {
                        pBackgroundTexture = &textures.backgrounds._16by10;
                     } else if (aspectRatio <= 1.83) {
                        pBackgroundTexture = &textures.backgrounds._16by9;
                     } else if (aspectRatio <= 2.00) {
                        // very irregular aspect ratio, use an almost widescreen aspect ratio instead
                        pBackgroundTexture = &textures.backgrounds._683by358;
                     } else if (aspectRatio <= 2.38) {
                        pBackgroundTexture = &textures.backgrounds._21by9;
                     } else {
                        pBackgroundTexture = &textures.backgrounds._32by9;
                     }
                     ImGui::Image(*pBackgroundTexture, pImGuiIO->DisplaySize);
                  }
                  // Loading spiral
                  {
                     ImGui::SetCursorPos(pImGuiIO->DisplaySize - ImVec2(96.0f, 96.0f));
                     ImGui::ImRotateStart();
                     ImGui::Image(textures.loadingSpiral, ImVec2(64.0f, 64.0f));
                     ImGui::ImRotateEnd(0.0174532925f * collectiveDeltaTime);
                  }
               }
               ImGui::End();
               ImGui::PopStyleVar(2);

               collectiveDeltaTime += pImGuiIO->DeltaTime * 60.0f;
            } else {
               collectiveDeltaTime = 0.0f;
            }
         }
         const virtual void beforeReset() override {
            // - no need cause of D3DPOOL_MANAGED
         }
         const virtual void afterReset() override {
            // - no need cause of D3DPOOL_MANAGED
         }

         const virtual bool displayMenuItem(const ImVec2& buttonSize) override {
            return false;
         }
         const virtual bool displayMenu() override {
            return false;
         }
      };
   }
}
