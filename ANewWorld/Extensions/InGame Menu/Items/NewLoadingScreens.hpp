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
         inline bool isLoadingEvent         = false;
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
               mov[isLoadingEvent], 0
               pop edi
               pop esi
               mov esp, ebp
               pop ebp
               retn 0x10
            }
         }

         inline uint32_t eventId = 0;
         inline bool     needUpdateEventInfo = false;
         static void __declspec(naked) hkLaunchSinglePlayerRace() {
            __asm {
               add esp, 0x8
               push eax
               mov eax, [ebp+0x10]
               mov[eventId], eax
               mov[isLoadingEvent], 1
               mov[needUpdateEventInfo], 1
               pop eax
               pop ebp
               retn
            }
         }
      }

      struct InternalEventDefinition {
         enum class EventMode : uint32_t {
            kEventMode_Unknown = 0,
            kEventMode_TollBooth = 1,
            kEventMode_Canyon = 2,
            kEventMode_Challenge = 3,
            kEventMode_Circuit = 4,
            kEventMode_Drift = 5,
            kEventMode_PursuitKnockOut = 6,
            kEventMode_PursuitTag = 7,
            kEventMode_SpeedTrap = 8,
            kEventMode_Sprint = 9,
            kEventMode_CanyonDrift = 10,
            kEventMode_Pursuit = 12,
            kEventMode_LapKnockOut = 15,
            kEventMode_Drag = 19,
            kEventMode_CoopPursuit = 21,
            kEventMode_MeetingPlace = 22,
            kEventMode_TeamPursuit = 23,
            kEventMode_TeamEscape = 24,
            kEventMode_TreasureHunt = 25
         };
         struct Name {
            const char* pName;
            const char* pNameEnd;
            const char* pUnk;
            DWORD _unk;
         };

         DWORD _vfptr;
         uint32_t EventId;
         EventMode EventModeId;
         bool IsLocked;
         Name CourseName;
         Name GameModeName;
         Name TrackLength;
         unsigned char _unk[0xC4];
         //void* Laps;
         //void* CourseMap;
         //void* ZoneHeat;
         //void* Condition;
         //void* GameModeIcon;
         //void* CarClassIcon;
         //void* CarClassLockedIcon;
         //void* EventDetails;
         //void* EventCategory;
         //void* MaxPlayers;
         //void* SinglePlayer;
         //void* MultiPlayer;
         //void* PrivateMultiPlayer;
         //void* MinLevel;
         //void* MinHeatLevel;
         //void* TimeLimit;
         //void* BustLimit;
         //void* PlayerHeat;
         //void* LockedReason;
         //void* Traffic;
      };

      struct NewLoadingScreens : _BaseInGameMenuItem {
         struct LoadingScreenTextures {
            static inline std::string dirUIAssets = "ANewWorld\\UI\\LOADING\\";
            struct BackgroundsByRatios {
               LPDIRECT3DTEXTURE9
                  _3by2, _4by3, _5by4,
                  _16by9, _16by10,
                  _21by9, _32by9,
                  _683by358;
            };
            struct LoadingTitleBackgrounds {
               LPDIRECT3DTEXTURE9 circuit, sprint, drag, meet, teamescape, pursuit;
            };
            struct LoadingCircleTypes {
               LPDIRECT3DTEXTURE9 default, meet, pursuit;
            };

            BackgroundsByRatios     backgrounds;
            LoadingTitleBackgrounds loadingTitleBackgrounds;
            LoadingCircleTypes      loadingCircles;

            void createTextures() {
               auto pDevice = LocalMirrorHook::D3D9::GetD3D9Device();
               // Backgrounds
               {
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_3by2.png").c_str(), &backgrounds._3by2);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_4by3.png").c_str(), &backgrounds._4by3);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_5by4.png").c_str(), &backgrounds._5by4);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_16by9.png").c_str(), &backgrounds._16by9);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_16by10.png").c_str(), &backgrounds._16by10);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_21by9.png").c_str(), &backgrounds._21by9);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_32by9.png").c_str(), &backgrounds._32by9);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "BACKGROUND\\UI_LOADING_BACKGROUND_683by358.png").c_str(), &backgrounds._683by358);
               }
               // Loading title backgrounds
               {
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "DEFAULT\\TITLE\\UI_LOADING_TITLE_BACKGROUND_CIRCUIT.png").c_str(), &loadingTitleBackgrounds.circuit);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "DEFAULT\\TITLE\\UI_LOADING_TITLE_BACKGROUND_SPRINT.png").c_str(), &loadingTitleBackgrounds.sprint);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "DEFAULT\\TITLE\\UI_LOADING_TITLE_BACKGROUND_DRAG.png").c_str(), &loadingTitleBackgrounds.drag);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "MEET\\TITLE\\UI_LOADING_TITLE_BACKGROUND.png").c_str(), &loadingTitleBackgrounds.meet);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "PURSUIT\\TITLE\\UI_LOADING_TITLE_BACKGROUND_ESCAPE.png").c_str(), &loadingTitleBackgrounds.teamescape);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "PURSUIT\\TITLE\\UI_LOADING_TITLE_BACKGROUND_PURSUIT.png").c_str(), &loadingTitleBackgrounds.pursuit);
               }
               // Loading circles
               {
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "DEFAULT\\UI_LOADING_CIRCLE.png").c_str(), &loadingCircles.default);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "MEET\\UI_LOADING_CIRCLE.png").c_str(), &loadingCircles.meet);
                  D3DXCreateTextureFromFile(pDevice, (dirUIAssets + "PURSUIT\\UI_LOADING_CIRCLE.png").c_str(), &loadingCircles.pursuit);
               }

            }
         };
         LoadingScreenTextures textures ={};

         const virtual void loadData() override {
            Memory::writeCall(0x897A6, (DWORD)&Hooks::NewLoadingScreens::hkLoadingScreen_LoadScreen, false);
            Memory::writeJMP(0x896FE, (DWORD)&Hooks::NewLoadingScreens::hkLoadingScreen_UnloadScreen, false);
            Memory::writeJMP(0xBA3F6, (DWORD)&Hooks::NewLoadingScreens::hkLaunchSinglePlayerRace, false);

            textures.createTextures();
            hasLoadedData = true;
         }

         const virtual void onFrame() override {
            static float collectiveDeltaTime = 0.0f;

            if (Hooks::NewLoadingScreens::isShowingLoadingScreen) {
               static InternalEventDefinition eventInfo ={};
               if (Hooks::NewLoadingScreens::needUpdateEventInfo) {
                  ((void(__thiscall*)(void* _void))0x4ACA80)(&eventInfo);
                  ((void(__thiscall*)(DWORD _instance, uint32_t eventId, void* _void))0x6A73D0)(0x00D11A04, Hooks::NewLoadingScreens::eventId, &eventInfo);
                  Hooks::NewLoadingScreens::needUpdateEventInfo = false;
               }

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

                  if (Hooks::NewLoadingScreens::isLoadingEvent) {
                     // Loading title
                     {
                        static ImVec2 titleBGPos, titleBGSize;
                        // Title background
                        {
                           static LPDIRECT3DTEXTURE9* pBackgroundTexture = nullptr;
                           switch (eventInfo.EventModeId) {
                              case InternalEventDefinition::EventMode::kEventMode_Circuit:
                                 pBackgroundTexture = &textures.loadingTitleBackgrounds.circuit;
                                 break;
                              case InternalEventDefinition::EventMode::kEventMode_Sprint:
                                 pBackgroundTexture = &textures.loadingTitleBackgrounds.sprint;
                                 break;
                              case InternalEventDefinition::EventMode::kEventMode_Drag:
                                 pBackgroundTexture = &textures.loadingTitleBackgrounds.drag;
                                 break;
                              case InternalEventDefinition::EventMode::kEventMode_MeetingPlace:
                                 pBackgroundTexture = &textures.loadingTitleBackgrounds.meet;
                                 break;
                              case InternalEventDefinition::EventMode::kEventMode_Pursuit:
                                 pBackgroundTexture = &textures.loadingTitleBackgrounds.pursuit;
                                 break;
                              case InternalEventDefinition::EventMode::kEventMode_TeamEscape:
                                 pBackgroundTexture = &textures.loadingTitleBackgrounds.teamescape;
                                 break;
                           }

                           titleBGPos = ImVec2(0.0f, 30.0f + (pImGuiIO->DisplaySize.y / 1000));
                           ImGui::SetCursorPos(titleBGPos);

                           float newWidth  = pImGuiIO->DisplaySize.x * 0.70f;
                           float newHeight = newWidth / (2082.0f / 282.0f);
                           titleBGSize = ImVec2(newWidth, newHeight);

                           ImGui::Image(*pBackgroundTexture, titleBGSize);
                        }
                        // Title
                        {
                           // Event name
                           ImGui::SetWindowFontScale(titleBGSize.y / 80.0f);
                           ImGui::PushFont(pImGuiIO->Fonts->Fonts[3]);
                           {
                              ImGui::SetCursorPos(ImVec2(titleBGSize.x / 12.0f, titleBGPos.y + (titleBGSize.y * 0.1f)));
                              ImGui::TextColored(ImVec4(0.78f, 0.83f, 1.0f, 1.0f), eventInfo.CourseName.pName);
                           } ImGui::PopFont();

                           // Event mode
                           ImGui::SetWindowFontScale((titleBGSize.y * 0.5f) / 80.0f);
                           ImGui::PushFont(pImGuiIO->Fonts->Fonts[3]);
                           {
                              ImGui::SetCursorPos(ImVec2(titleBGSize.x / 11.0f, titleBGPos.y + (titleBGSize.y * 0.55f)));
                              ImGui::TextColored(ImVec4(0.78f, 0.83f, 1.0f, 1.0f), eventInfo.GameModeName.pName);
                           } ImGui::PopFont();

                           ImGui::SetWindowFontScale(1.0f);
                        }
                     }
                  }

                  // Loading circle
                  {
                     static LPDIRECT3DTEXTURE9* pTexture = nullptr;
                     if (Hooks::NewLoadingScreens::isLoadingEvent) {
                        switch (eventInfo.EventModeId) {
                           case InternalEventDefinition::EventMode::kEventMode_Circuit:
                           case InternalEventDefinition::EventMode::kEventMode_Sprint:
                           case InternalEventDefinition::EventMode::kEventMode_Drag:
                              pTexture = &textures.loadingCircles.default;
                              break;
                           case InternalEventDefinition::EventMode::kEventMode_MeetingPlace:
                              pTexture = &textures.loadingCircles.meet;
                              break;
                           case InternalEventDefinition::EventMode::kEventMode_Pursuit:
                           case InternalEventDefinition::EventMode::kEventMode_TeamEscape:
                              pTexture = &textures.loadingCircles.pursuit;
                              break;
                        }
                     } else {
                        pTexture = &textures.loadingCircles.default;
                     }

                     static ImVec2 textureSize;
                     float textureSqSize = pImGuiIO->DisplaySize.y / 12.0f;
                     textureSize = ImVec2(textureSqSize, textureSqSize);

                     ImGui::SetCursorPos(pImGuiIO->DisplaySize - (textureSize * 1.5f));
                     ImGui::ImRotateStart();
                     ImGui::Image(*pTexture, textureSize);
                     ImGui::ImRotateEnd(0.0174532925f * 2.0f * collectiveDeltaTime);
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
