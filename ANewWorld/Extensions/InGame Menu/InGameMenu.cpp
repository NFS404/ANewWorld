/*
   MIT License

   Copyright (c) 2019 Berkay Yigit <berkay2578@gmail.com>
   Nickname(s) used by the copyright holder: 'berkay2578', 'berkayylmao'.

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

#include "stdafx.h"
#include "InGameMenu.h"
#include "Extensions\Extensions.h"
// dear imgui
#include "Helpers\imgui\extra_fonts\RobotoMedium.hpp"
#include "Helpers\imgui\extra_fonts\CooperHewitt_Roman.hpp"
#include "Helpers\imgui\extra_fonts\CooperHewitt_Bold.hpp"
#include "Helpers\imgui\extra_fonts\Aramis_Book_Italic.hpp"
#include "Helpers\imgui\dx9\imgui_impl_dx9.h"
#include "Helpers\imgui\dx9\imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Extensions {
   ImGuiIO* pImGuiIO = nullptr;
   namespace InGameMenu {
      _BaseInGameMenuItem* activeItem = nullptr;
      std::vector<_BaseInGameMenuItem*> items      ={};

      bool isImguiInitialized  = false;
      bool isMainWindowVisible = true;
      bool isFirstTimeUser     = false;
      bool hasShownFirstWindow = false;

      RECT clientRect ={ 0 };

      void showUserGuide() {
         if (isFirstTimeUser) {
            if (ImGui::Begin("User Guide", &isFirstTimeUser)) {
               ImGui::BulletText("Press Insert to show/hide ANewWorld.");
               ImGui::BulletText("Double-click on title bar to collapse windows.");
               ImGui::BulletText("Click and drag on lower right corner to resize windows.");
               ImGui::BulletText("Click and drag on any empty space to move windows.");
               ImGui::BulletText("Mouse Wheel to scroll.");
               ImGui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
               ImGui::BulletText("CTRL+Click on a slider to input text.");
               ImGui::BulletText(
                  "While editing text:\n"
                  "- Hold SHIFT+Left/Right or use mouse to select text\n"
                  "- CTRL+Left/Right to word jump\n"
                  "- CTRL+A or double-click to select all\n"
                  "- CTRL+X, CTRL+C, CTRL+V clipboard\n"
                  "- CTRL+Z, CTRL+Y to undo/redo\n"
                  "- ESCAPE to cancel\n"
                  "- You can apply arithmetic operators +,*,/ on numerical values.\n");
               ImGui::BulletText("Click on the button at the top-right of this window to close it.");
            }
            ImGui::End();

            if (Settings::settingsType.isFirstTimeUser) {
               Settings::settingsType.isFirstTimeUser = false;
               Settings::saveSettings();
            }
         }
      }

      void WINAPI beginScene(LPDIRECT3DDEVICE9 pDevice) {
         if (!isImguiInitialized) {
            Extensions::pImGuiIO = &ImGui::CreateContext()->IO;
            ImGui_ImplWin32_Init(Helpers::WndProcHook::windowHandle);
            ImGui_ImplDX9_Init(pDevice);

            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium::RobotoMedium_compressed_data, RobotoMedium::RobotoMedium_compressed_size, 21.0f);
            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(CooperHewitt_Roman_compressed_data, CooperHewitt_Roman_compressed_size, 64.0f);
            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(CooperHewitt_Bold_compressed_data, CooperHewitt_Bold_compressed_size, 64.0f);
            pImGuiIO->Fonts->Build();
            pImGuiIO->FontDefault = NULL;
            pImGuiIO->IniFilename = NULL;

            ImGui::SetColorEditOptions(ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions
                                       | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::LoadStyle();
            isImguiInitialized = true;
         }

         ImGui_ImplDX9_NewFrame();
         ImGui_ImplWin32_NewFrame();
         ImGui::NewFrame();

         static float scaling = 1.0f;
         GetClientRect(Helpers::WndProcHook::windowHandle, &clientRect);
         scaling = std::max(0.6f, (float)clientRect.right / 2160.0f); // optimized for 4k
         for (int32_t i = 0; i < pImGuiIO->Fonts->Fonts.Size; i++) {
            pImGuiIO->Fonts->Fonts[i]->Scale = scaling;
         }

         for (auto item : items) {
            if (item->hasLoadedData) {
               item->onFrame();
            }
         }
      }
      void WINAPI endScene(LPDIRECT3DDEVICE9 pDevice) {
         if (isImguiInitialized) {
            if (!hasShownFirstWindow) {
               static uint32_t secondsToShow = 3;
               static uint32_t framesShown   = 0;
               uint32_t framesToShow         = pImGuiIO->Framerate * secondsToShow;

               ImGui::SetNextWindowBgAlpha(0.0f);
               ImGui::SetNextWindowSize(pImGuiIO->DisplaySize, ImGuiCond_Always);
               ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
               if (ImGui::Begin("##ANewWorld_FirstMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs)) {
                  static ImVec2 _TitlePlacementVec  ={ 0.0f, 0.0f };
                  if (_TitlePlacementVec.y == 0.0f) {
                     ImGui::PushFont(pImGuiIO->Fonts->Fonts[1]);
                     {
                        _TitlePlacementVec.x = (pImGuiIO->DisplaySize.x / 2.0f) - ((ImGui::CalcTextSize("A").x / 2.0f) + (ImGui::CalcTextSize("World").x / 2.0f));
                     } ImGui::PopFont();
                     ImGui::PushFont(pImGuiIO->Fonts->Fonts[2]);
                     {
                        auto& textSize = ImGui::CalcTextSize("New");
                        _TitlePlacementVec.x -= textSize.x / 2.0f;
                        _TitlePlacementVec.y = (pImGuiIO->DisplaySize.y / 2);

                     } ImGui::PopFont();
                  }
                  ImGui::SetCursorPos(_TitlePlacementVec);
                  // ANewWorld title 
                  {
                     static float _TitlePlacementDiff;
                     ImGui::PushFont(pImGuiIO->Fonts->Fonts[1]);
                     {
                        ImGui::Text("A");
                        _TitlePlacementDiff = ImGui::CalcTextSize("A").x;
                     } ImGui::PopFont();
                     ImGui::PushFont(pImGuiIO->Fonts->Fonts[2]);
                     {
                        ImGui::SameLine(_TitlePlacementVec.x + _TitlePlacementDiff);
                        ImGui::TextColored(ImVec4(0.0f, 0.565f, 1.0f, 1.0f), "New");
                        _TitlePlacementDiff += ImGui::CalcTextSize("New").x;
                     } ImGui::PopFont();
                     ImGui::PushFont(pImGuiIO->Fonts->Fonts[1]);
                     {
                        ImGui::SameLine(_TitlePlacementVec.x + _TitlePlacementDiff);
                        ImGui::Text("World");
                        _TitlePlacementDiff += ImGui::CalcTextSize("World").x;
                     } ImGui::PopFont();
                     {
                        ImGui::SameLine(_TitlePlacementVec.x + _TitlePlacementDiff + 3.0f);
                        ImGui::SetCursorPosY(_TitlePlacementVec.y - 6.0f);
                        ImGui::Text("v1.0");
                     }
                  }
               }

               framesShown++;
               if (framesShown >= framesToShow)
                  hasShownFirstWindow = true;
               ImGui::End();
            } else {
               if (isMainWindowVisible) {
                  showUserGuide();

                  ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f), ImGuiCond_Once);
                  if (ImGui::Begin("##ANewWorld_Main", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) {
                     // Title
                     static float buttonWidth;
                     {
                        static float _TitlePlacementDiff;
                        ImGui::PushFont(pImGuiIO->Fonts->Fonts[1]);
                        {
                           ImGui::Text("A");
                           _TitlePlacementDiff = ImGui::GetStyle().WindowPadding.x + ImGui::CalcTextSize("A").x;
                        } ImGui::PopFont();
                        ImGui::PushFont(pImGuiIO->Fonts->Fonts[2]);
                        {
                           ImGui::SameLine(_TitlePlacementDiff);
                           ImGui::TextColored(ImVec4(0.0f, 0.565f, 1.0f, 1.0f), "New");
                           _TitlePlacementDiff += ImGui::CalcTextSize("New").x;
                        } ImGui::PopFont();
                        ImGui::PushFont(pImGuiIO->Fonts->Fonts[1]);
                        {
                           ImGui::SameLine(_TitlePlacementDiff);
                           ImGui::Text("World");
                           _TitlePlacementDiff += ImGui::CalcTextSize("World").x;
                        } ImGui::PopFont(); ImGui::SameLine(_TitlePlacementDiff + 3.0f);
                        buttonWidth = ImGui::GetCursorPos().x + ImGui::CalcTextSize("v1.0").x;
                        ImGui::SetCursorPosY(ImGui::GetStyle().WindowPadding.y - 6.0f);
                        ImGui::Text("v1.0");
                     }

                     if (!activeItem) {
                        for (auto item : items) {
                           if (item->hasLoadedData && item->displayMenuItem(ImVec2(buttonWidth, 0.0f)))
                              activeItem = item;
                        }
                     } else {
                        if (ImGui::Button("< Back"))
                           activeItem = nullptr;
                        else {
                           if (!activeItem->displayMenu())
                              activeItem = nullptr;
                        }
                     }
                  }
                  ImGui::End();
               }
            }
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
         }
      }

      void WINAPI beforeReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
         if (isImguiInitialized)
            ImGui_ImplDX9_InvalidateDeviceObjects();
      }
      void WINAPI afterReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
         if (!pDevice || pDevice->TestCooperativeLevel() != D3D_OK) {
            if (isImguiInitialized) {
               ImGui_ImplDX9_Shutdown();
               ImGui::DestroyContext();
            }
            isImguiInitialized = false;
            return;
         }

         ImGui_ImplDX9_CreateDeviceObjects();
      }

      LRESULT CALLBACK wndProcExtension(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
         if (isImguiInitialized) {
            if (uMsg == WM_QUIT) {
               ImGui_ImplDX9_Shutdown();
               ImGui::DestroyContext();
               return FALSE;
            }

            if (isMainWindowVisible)
               ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

            if (!pImGuiIO->WantCaptureKeyboard || !pImGuiIO->WantTextInput) {
               if (uMsg == WM_KEYUP) {
                  switch (wParam) {
                     case VK_INSERT:
                        isMainWindowVisible = !isMainWindowVisible;
                        return FALSE;
                  }
               }
            }
         }
         return WndProcHook_IgnoreExtension;
      }

      void Init() {
         isFirstTimeUser = Settings::settingsType.isFirstTimeUser;

         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9::D3D9Extension::BeginScene, &beginScene);
         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9::D3D9Extension::EndScene, &endScene);
         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9::D3D9Extension::BeforeReset, &beforeReset);
         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9::D3D9Extension::AfterReset, &afterReset);

         Helpers::WndProcHook::addExtension(&wndProcExtension);
         while (!isImguiInitialized) Sleep(1000);
      }
   }
}