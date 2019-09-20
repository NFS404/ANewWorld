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

#include "stdafx.h"
#include "InGameMenu.h"
#include "Extensions\Extensions.h"
// dear imgui
#include "Helpers\imgui\extra_fonts\RobotoMedium.hpp"
#include "Helpers\imgui\extra_fonts\CooperHewitt_Roman.hpp"
#include "Helpers\imgui\extra_fonts\CooperHewitt_Italic.hpp"
#include "Helpers\imgui\extra_fonts\CooperHewitt_Bold.hpp"
#include "Helpers\imgui\extra_fonts\Reg-DB-I.hpp"
#include "Helpers\imgui\dx9\imgui_impl_dx9.h"
#include "Helpers\imgui\dx9\imgui_impl_win32.h"
#include "Helpers\imgui\imgui_freetype.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Extensions {
   ImGuiIO* pImGuiIO = nullptr;
   namespace InGameMenu {
      _BaseInGameMenuItem* activeItem = nullptr;
      std::vector<_BaseInGameMenuItem*> items ={};

      bool isImguiInitialized            = false;
      bool isMainWindowVisible           = true;
      bool isFirstTimeUser               = false;
      bool hasUserSeenFirstLoadingScreen = false;

      void showUserGuide() {
         if (isFirstTimeUser) {
            if (ImGui::Begin("User Guide", &isFirstTimeUser)) {
               ImGui::BulletText("Press Insert to show/hide ANewWorld.");
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

            if (Settings::instance.isFirstTimeUser) {
               Settings::instance.isFirstTimeUser = false;
               Settings::saveSettings();
            }
         }
      }

      void WINAPI beginScene(LPDIRECT3DDEVICE9 pDevice) {
         if (!isImguiInitialized) {
            Log(LogLevel::Debug, "Setting up imgui");
            pImGuiIO = &ImGui::CreateContext()->IO;
            ImGui_ImplWin32_Init(Helpers::WndProcHook::windowHandle);
            ImGui_ImplDX9_Init(pDevice);

            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium::RobotoMedium_compressed_data, RobotoMedium::RobotoMedium_compressed_size, 24.0f);
            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(CooperHewitt_Roman_compressed_data, CooperHewitt_Roman_compressed_size, 64.0f);
            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(CooperHewitt_Bold_compressed_data, CooperHewitt_Bold_compressed_size, 64.0f);
            pImGuiIO->Fonts->AddFontFromMemoryCompressedTTF(Reg_DB_I_compressed_data, Reg_DB_I_compressed_size, 80.0f);
            ImGuiFreeType::BuildFontAtlas(pImGuiIO->Fonts, ImGuiFreeType::LightHinting);
            pImGuiIO->FontDefault = NULL;
            pImGuiIO->IniFilename = NULL;

            ImGui::SetColorEditOptions(ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions
                                       | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::LoadStyle();
            isImguiInitialized = true;
            Log(LogLevel::Debug, "Initialized imgui successfully.");
         }

         ImGui_ImplDX9_NewFrame();
         ImGui_ImplWin32_NewFrame();
         ImGui::NewFrame();

         static float scaling = 1.0f;
         scaling = std::max(0.6f, pImGuiIO->DisplaySize.y / 2160.0f); // optimized for 4k
         for (int32_t i = 0; i < pImGuiIO->Fonts->Fonts.Size; i++) {
            pImGuiIO->Fonts->Fonts[i]->Scale = scaling;
         }
      }
      void WINAPI endScene(LPDIRECT3DDEVICE9 pDevice) {
         for (auto item : items) {
            if (item->hasLoadedData) {
               item->onFrame();
            }
         }

         if (isImguiInitialized) {
            if (!hasUserSeenFirstLoadingScreen) {
               ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
               ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Appearing);
               if (ImGui::Begin("###ANewWorld_FirstMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize)) {
                  // ANewWorld title 
                  {
                     static float _TitlePlacementDiff;
                     ImGui::PushFont(pImGuiIO->Fonts->Fonts[1]);
                     {
                        ImGui::Text("A");
                        _TitlePlacementDiff = 10.0f + ImGui::CalcTextSize("A").x;
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
                     } ImGui::PopFont();
                     {
                        ImGui::SameLine(_TitlePlacementDiff + 3.0f);
                        ImGui::SetCursorPosY(5.0f);
                        ImGui::Text("v1.0");
                     }
                  }
               }
               ImGui::Text("https://github.com/berkayylmao/ANewWorld");
               ImGui::Text("Features:\n"
                           " - Anticheat\n"
                           " - File mods\n"
                           " - Scripting API\n"
                           " - New loading screens\n"
                           " - New safehouses\n"
                           " - Various gameplay mods\n"
                           "Find me on Discord! berkayylmao#7392."
               );
               ImGui::End();
               ImGui::PopStyleVar();
            } else {
               if (isMainWindowVisible) {
                  showUserGuide();

                  ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Once);
                  if (ImGui::Begin("###ANewWorld_Main", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) {
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
                        ImGui::SetCursorPosY(ImGui::GetStyle().WindowPadding.y - 5.0f);
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
         for (auto item : items)
            item->beforeReset();
         if (isImguiInitialized)
            ImGui_ImplDX9_InvalidateDeviceObjects();
      }
      void WINAPI afterReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
         for (auto item : items)
            item->afterReset();

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
         isFirstTimeUser = Settings::instance.isFirstTimeUser;

         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9Extension::BeginScene, &beginScene);
         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9Extension::EndScene, &endScene);
         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9Extension::BeforeReset, &beforeReset);
         LocalMirrorHook::D3D9::AddExtension(LocalMirrorHook::D3D9Extension::AfterReset, &afterReset);

         Helpers::WndProcHook::addExtension(&wndProcExtension);
         while (!isImguiInitialized) Sleep(1000);
      }
   }
}