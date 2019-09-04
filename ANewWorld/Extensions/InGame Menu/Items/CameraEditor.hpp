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

#pragma once
#include "stdafx.h"
#include "Extensions\Extensions.h"
using GameInternals::CameraInfo;
#include <MMSystem.h> // joy*

namespace Extensions
{
   namespace InGameMenu
   {
      class CameraEditor : public _BaseInGameMenuItem {
         struct CameraEditorData {
            CameraInfo defaultCameraInfo          ={ 0 };
            bool       showAdvancedOptions        = false;
            bool       hasLoadedSettings          = false;
            bool       hasLoadedDefaultCameraInfo = false;

            bool  joyViewEnabled       = false;
            float joyViewData[2]       ={ 0.0f, 0.0f };
            bool  hasLoadedJoyViewData = false;

            void enableForceLookBack(const bool& shouldForce) {
               if (shouldForce)
                  Memory::writeRaw(0x6B6890, true, 4, 0x83, 0xF0, 0x01, 0xC3); // xor eax,1; ret;
               else
                  Memory::writeRaw(0x6B6890, true, 4, 0xD9, 0x81, 0xDC, 0x02); // fld [ecx+0x2DC0];
            }

            bool  speedFOVEnabled = false;
            float speedFOVScale   = 20.0f;

            int16_t nitrousFOVWidening = 1638; // Game default value, 0x666
            float   nitrousFOVScale    = 5.0f; // nitrousFOVWidening / 327.6

            float* getNitrousFOVWideningAsPScale() {
               nitrousFOVScale = nitrousFOVWidening / 327.6f;
               return &nitrousFOVScale;
            }
            void setNitrousFOVScaleToWidening(const bool& writeToMemory = true) {
               nitrousFOVWidening = (int16_t)(nitrousFOVScale * 327.6f);
               if (writeToMemory)
                  GameVariables::setVariable(GameVariables::nosFOVWidening, nitrousFOVWidening);
            }

            ~CameraEditorData() {
               delete defaultCameraInfo.Height.pXValues;
               delete defaultCameraInfo.Height.pYValues;
               delete defaultCameraInfo.Height.pY2Values;
               delete defaultCameraInfo.FOV.pXValues;
               delete defaultCameraInfo.FOV.pYValues;
               delete defaultCameraInfo.FOV.pY2Values;
               delete defaultCameraInfo.Follow.pXValues;
               delete defaultCameraInfo.Follow.pYValues;
               delete defaultCameraInfo.Follow.pY2Values;
            }
         };
         std::map<CameraInfo*, CameraEditorData> cache ={};
         CameraEditorData*     pActiveCameraEditorData = nullptr;

         int         oldCameraIndex     = 0;
         int*        pActiveCameraIndex = nullptr;
         CameraInfo* pActiveCameraInfo  = nullptr;

         void drawAdvanced(const char* title, GameInternals::_internalCameraNodeDetails* pCameraNodeDetails) {
            if (ImGui::BeginChild(title, ImVec2(250.0f, 150.0f))) {
               ImGui::Text(title);
               ImGui::ProperLabels::SliderFloat("GraphScale", &pCameraNodeDetails->GraphScale, -100.0f, 100.0f);
               ImGui::ProperLabels::SliderFloat("MaxX", &pCameraNodeDetails->MaxX, -100.0f, 100.0f);
               ImGui::ProperLabels::SliderFloat("MaxY", &pCameraNodeDetails->MaxY, -100.0f, 100.0f);
               ImGui::ProperLabels::SliderFloat("MinX", &pCameraNodeDetails->MinX, -100.0f, 100.0f);
               ImGui::ProperLabels::SliderFloat("MinY", &pCameraNodeDetails->MinY, -100.0f, 100.0f);
               ImGui::Text("pXValues"); ImGui::SameLine(); ImGui::PushID(pCameraNodeDetails->pXValues);
               ImGui::SliderFloat3("", *pCameraNodeDetails->pXValues, -100.0f, 100.0f); ImGui::PopID();
               ImGui::Text("pYValues"); ImGui::SameLine(); ImGui::PushID(pCameraNodeDetails->pYValues);
               ImGui::SliderFloat3("", *pCameraNodeDetails->pYValues, -100.0f, 100.0f); ImGui::PopID();
               ImGui::Text("pY2Values"); ImGui::SameLine(); ImGui::PushID(pCameraNodeDetails->pY2Values);
               ImGui::SliderFloat3("", *pCameraNodeDetails->pY2Values, -100.0f, 100.0f); ImGui::PopID();
            } ImGui::EndChild();
         }
         void resetData(const bool& resetJoyView, const bool& resetSpeedFOV) {
            if (!pActiveCameraInfo)
               return;

            CameraEditorData* pCameraEditorData = pActiveCameraEditorData;
            if (!pCameraEditorData) {
               auto iter = cache.find(pActiveCameraInfo);
               if (iter != cache.end()) {
                  pCameraEditorData = &iter->second;
               }
            }
            if (pCameraEditorData) {
               /*if (resetJoyView) {
                  pActiveCameraInfo->Angle[0] = pCameraEditorData->defaultCameraInfo.Angle[0];
                  pActiveCameraInfo->Follow.MinY = pCameraEditorData->defaultCameraInfo.Follow.MinY;
                  pActiveCameraEditorData->enableForceLookBack(false);
                  pActiveCameraEditorData->hasLoadedJoyViewData = false;
               }*/
               if (resetSpeedFOV) {
                  pActiveCameraInfo->FOV.MinY = pCameraEditorData->defaultCameraInfo.FOV.MinY;
                  pActiveCameraInfo->FOV.MaxY = pCameraEditorData->defaultCameraInfo.FOV.MaxY;
               }
            }
         }
         public:
         const virtual void loadData() override {
            hasLoadedData = true;
         }

         const virtual void onFrame() override {
            pActiveCameraIndex = CameraInternals::getActiveCameraIndexAsPointer();
            if (pActiveCameraIndex) {
               if (oldCameraIndex != *pActiveCameraIndex
                   || ((oldCameraIndex == *pActiveCameraIndex) && (!pActiveCameraInfo))) {
                  if (CameraInternals::getActiveCameraInfo(pActiveCameraInfo)) {
                     pActiveCameraEditorData = &cache[pActiveCameraInfo];

                     if (!pActiveCameraEditorData->hasLoadedSettings) {
                        std::string activeCameraName = pActiveCameraInfo->CollectionName;
                        auto iter = Settings::settingsType.cameraPresets.find(activeCameraName);
                        if (iter != Settings::settingsType.cameraPresets.end()) {
                           auto* pActiveCameraPreset = &iter->second;
                           pActiveCameraPreset->InfoPreset.copyDataToGameInternalsCompliantPointer(pActiveCameraInfo);
                           pActiveCameraEditorData->joyViewEnabled     = pActiveCameraPreset->JoyViewEnabled;
                           pActiveCameraEditorData->speedFOVEnabled    = pActiveCameraPreset->SpeedFOVEnabled;
                           pActiveCameraEditorData->speedFOVScale      = pActiveCameraPreset->SpeedFOVScale;
                           pActiveCameraEditorData->nitrousFOVWidening = pActiveCameraPreset->NitrousFOVWidening;

                           pActiveCameraEditorData->hasLoadedSettings = true;
                        }
                     }
                     pActiveCameraEditorData->defaultCameraInfo = *pActiveCameraInfo;
                     GameVariables::setVariable(GameVariables::nosFOVWidening, pActiveCameraEditorData->nitrousFOVWidening);
                  }
               } oldCameraIndex = *pActiveCameraIndex;

               if (pActiveCameraInfo)
                  pActiveCameraEditorData = &cache[pActiveCameraInfo];
               if (pActiveCameraInfo && pActiveCameraEditorData) {
                  /*
                  if (pActiveCameraEditorData->joyViewEnabled) {
                     if (!pActiveCameraEditorData->hasLoadedJoyViewData) {
                        pActiveCameraEditorData->joyViewData[0]       = 5.0f - pActiveCameraInfo->Angle[0];
                        pActiveCameraEditorData->joyViewData[1]       = -35.0f;
                        pActiveCameraEditorData->hasLoadedJoyViewData = true;
                     }

                     static JOYINFOEX jiEx = { 0 };
                     jiEx.dwFlags = JOY_RETURNU | JOY_RETURNR | JOY_RETURNBUTTONS;
                     jiEx.dwSize  = sizeof(JOYINFOEX);

                     if (joyGetPosEx(JOYSTICKID1, &jiEx) == JOYERR_NOERROR)
                     {
                        static int32_t edtX, edtY;
                        edtX = jiEx.dwUpos - INT16_MAX;
                        edtY = jiEx.dwRpos - INT16_MAX;

                        static float valX, valY;
                        valX = valY = 0.0f;
                        if (edtX >> 4)
                           valX = (float)edtX / INT16_MAX;
                        if (edtY >> 4)
                           valY = (float)edtY / INT16_MAX;

                        auto& modifiers = pActiveCameraEditorData->joyViewData;
                        static float mod0, mod1;
                        mod0 = mod1 = 0.0f;
                        if (valX >= 0.05f || valX <= -0.05f) {
                           mod0 = modifiers[0] * valX;
                           mod1 = modifiers[1] * abs(valX);
                        }
                        if (valY >= 0.05f) {
                           mod1 -= modifiers[1] * (valY * -1.8f);
                        }
                        auto& bkCameraInfo = pActiveCameraEditorData->defaultCameraInfo;
                        pActiveCameraInfo->Angle[0]    = bkCameraInfo.Angle[0] + mod0;
                        pActiveCameraInfo->Follow.MinY = bkCameraInfo.Follow.MinY + mod1;

                        if (jiEx.dwButtons & JOY_BUTTON10) // xbox 360 right stick down
                           pActiveCameraEditorData->enableForceLookBack(true);
                        else
                           pActiveCameraEditorData->enableForceLookBack(false);
                     }
                  }
                  */
                  if (pActiveCameraEditorData->speedFOVEnabled) {
                     static float playerCarSpeed = 0.0f;
                     playerCarSpeed = PlayerCarInternals::getSpeed(GameInternals::SpeedUnit::KMH);

                     pActiveCameraInfo->FOV.MaxY =
                        pActiveCameraEditorData->defaultCameraInfo.FOV.MaxY + pActiveCameraEditorData->speedFOVScale * (playerCarSpeed / 360.0f);
                  }
               }
            }
         }

         const virtual bool displayMenuItem(const ImVec2& buttonSize) override {
            return ImGui::Button("Camera Editor", buttonSize);
         }
         const virtual bool displayMenu() override {
            ImGui::PushItemWidth(-1.0f);
            if (pActiveCameraIndex) {
               ImGui::TextWrapped("Camera Index"); ImGui::SameLine();
               if (ImGui::SliderInt("##CameraIndex", pActiveCameraIndex, 0, 6))
                  oldCameraIndex = -1;

               if (pActiveCameraInfo) {
                  ImGui::TextWrapped("Camera Name: %s", pActiveCameraInfo->CollectionName);
                  /*if (ImGui::Checkbox("Controller View", &pActiveCameraEditorData->joyViewEnabled)) {
                     if (pActiveCameraEditorData->joyViewEnabled) {
                        if (!pActiveCameraEditorData->hasLoadedDefaultCameraInfo) {
                           pActiveCameraEditorData->defaultCameraInfo          = *pActiveCameraInfo;
                           pActiveCameraEditorData->hasLoadedDefaultCameraInfo = true;
                        } else {
                           memcpy_s(pActiveCameraEditorData->defaultCameraInfo.Angle, sizeof(float) * 2, pActiveCameraInfo->Angle, sizeof(float) * 2);
                           pActiveCameraEditorData->defaultCameraInfo.Follow.MinY = pActiveCameraInfo->Follow.MinY;
                        }

                        joySetCapture(Helpers::WndProcHook::windowHandle, JOYSTICKID1, 1, FALSE);
                     } else {
                        joyReleaseCapture(JOYSTICKID1);
                        resetData(true, false);
                     }
                  } ImGui::SameLine(); ImGui::VerticalSeparator(); ImGui::SameLine(); */
                  if (ImGui::Checkbox("Speed FOV", &pActiveCameraEditorData->speedFOVEnabled)) {
                     if (pActiveCameraEditorData->speedFOVEnabled) {
                        if (!pActiveCameraEditorData->hasLoadedDefaultCameraInfo) {
                           pActiveCameraEditorData->defaultCameraInfo          = *pActiveCameraInfo;
                           pActiveCameraEditorData->hasLoadedDefaultCameraInfo = true;
                        } else {
                           pActiveCameraEditorData->defaultCameraInfo.FOV.MinY = pActiveCameraInfo->FOV.MinY;
                           pActiveCameraEditorData->defaultCameraInfo.FOV.MaxY = pActiveCameraInfo->FOV.MaxY;
                        }
                     } else
                        resetData(false, true);
                  }
                  if (pActiveCameraEditorData->speedFOVEnabled)
                     ImGui::SliderFloat("##SpeedFOVScale", &pActiveCameraEditorData->speedFOVScale, -50.0f, 50.0f, "Speed FOV Scale: %.1f");

                  if (ImGui::SliderFloat("##NitrousFOVScale", pActiveCameraEditorData->getNitrousFOVWideningAsPScale(), -30.0f, 30.0f, "NOS FOV Scale: %.1f"))
                     pActiveCameraEditorData->setNitrousFOVScaleToWidening();

                  ImGui::Checkbox("Advanced options", &pActiveCameraEditorData->showAdvancedOptions);
                  ImGui::SameLine(); ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
                  if (ImGui::Button("Save Preset")) {
                     auto* pCameraPreset = &Settings::settingsType.cameraPresets[std::string(pActiveCameraInfo->CollectionName)];
                     pCameraPreset->InfoPreset.setTo(pActiveCameraInfo);
                     pCameraPreset->JoyViewEnabled     = pActiveCameraEditorData->joyViewEnabled;
                     pCameraPreset->SpeedFOVEnabled    = pActiveCameraEditorData->speedFOVEnabled;
                     pCameraPreset->SpeedFOVScale      = pActiveCameraEditorData->speedFOVScale;
                     pCameraPreset->NitrousFOVWidening = GameVariables::getVariable(GameVariables::nosFOVWidening);

                     Settings::saveSettings();
                  }

                  ImGui::TextWrapped("Stiffness");
                  ImGui::SliderFloat("##CameraStiffness", &pActiveCameraInfo->Stiffness[0], 0.0f, 1.0f);
                  if (pActiveCameraEditorData->showAdvancedOptions) {
                     ImGui::ProperLabels::SliderFloat("STIFFNESS[1]", &pActiveCameraInfo->Stiffness[1], 0.0f, 1.0f);
                  }

                  if (!pActiveCameraEditorData->joyViewEnabled) {
                     /*
                     ImGui::TextWrapped("Horizontal angle");
                     ImGui::SliderFloat("##CameraHorizontalAngle", &pActiveCameraInfo->Angle[0], -45.0f, 45.0f, "%.3f deg");
                     if (pActiveCameraEditorData->showAdvancedOptions) {
                        ImGui::Text("ANGLE[1]"); ImGui::SameLine();
                        ImGui::SliderFloat("##CameraHorizontalAngle_1", &pActiveCameraInfo->Angle[1], -45.0f, 45.0f, "%.3f deg");
                     }
                     */
                     ImGui::TextWrapped("Distance");
                     RockportEdControls::MultiSliderWithThreshold("##CameraDistance", &pActiveCameraInfo->Follow.MaxY, 0.0f, &pActiveCameraInfo->Follow.MinY, -100.0f, 100.0f);
                     if (pActiveCameraEditorData->showAdvancedOptions)
                        drawAdvanced("FOLLOW", &pActiveCameraInfo->Follow);
                  } else {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                     ImGui::TextWrapped("Cannot change horizontal angle or distance while 'Controller View' is enabled!");
                     ImGui::PopStyleColor();
                  }

                  if (!pActiveCameraEditorData->speedFOVEnabled) {
                     ImGui::TextWrapped("FOV");
                     RockportEdControls::MultiSliderWithThreshold("##CameraFOV", &pActiveCameraInfo->FOV.MaxY, 0.0f, &pActiveCameraInfo->FOV.MinY, 25.0f, 135.0f);
                     if (pActiveCameraEditorData->showAdvancedOptions)
                        drawAdvanced("FOV", &pActiveCameraInfo->FOV);
                  } else {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                     ImGui::TextWrapped("Cannot change FOV while 'Speed FOV' is enabled!");
                     ImGui::PopStyleColor();
                  }

                  ImGui::TextWrapped("Height");
                  RockportEdControls::MultiSliderWithThreshold("##CameraHeight", &pActiveCameraInfo->Height.MaxY, 0.0f, &pActiveCameraInfo->Height.MinY, 0.0f, 100.0f);
                  if (pActiveCameraEditorData->showAdvancedOptions)
                     drawAdvanced("HEIGHT", &pActiveCameraInfo->Height);

                  ImGui::TextWrapped("Vertical angle");
                  RockportEdControls::MultiSliderWithThreshold("##CameraAngle", &pActiveCameraInfo->Angle[0], 0.0f, &pActiveCameraInfo->Angle[1], -89.0f, 89.0f);
                  if (pActiveCameraEditorData->showAdvancedOptions) {
                     ImGui::ProperLabels::SliderFloat("ANGLE[1]", &pActiveCameraInfo->Angle[1], -89.0f, 89.0f, "%.3f deg");
                  }

                  ImGui::Checkbox("Selectable", &pActiveCameraInfo->Selectable[0]);
                  if (pActiveCameraEditorData->showAdvancedOptions) {
                     ImGui::Checkbox("SELECTABLE[1]", &pActiveCameraInfo->Selectable[1]);
                  }
               } else {
                  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                  ImGui::TextWrapped("No active CameraInfo was found.");
                  ImGui::PopStyleColor();
                  resetData(true, true);
               }
            } else {
               ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
               ImGui::TextWrapped("CameraAI not yet initialized by the game.");
               ImGui::PopStyleColor();
               resetData(true, true);
            }
            ImGui::PopItemWidth();
            return true;
         }
      };
   }
}