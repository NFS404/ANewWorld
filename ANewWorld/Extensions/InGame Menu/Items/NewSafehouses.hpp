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
#include "Helpers\Game Internals\Data\GameTypes.h"

namespace Extensions {
   namespace InGameMenu {
      namespace Hooks {
         namespace NewSafehouses {
            // Not a game struct!
            struct SafehouseCarPosition {
               Vector3 Position;
               USHORT  Rotation;
               float   TireRotation;
            };
            // Game struct
            struct FreeCameraProperties {
               /* padding */ unsigned char __unk[0x9C];
               /* +0x9C */ float VerticalAngle;
               /* +0xA0 */ float HorizontalAngle;
               /* +0xA4 */ float Zoom;
               /* padding */ unsigned char __unk2[0x80];
               /* +0x128 */ float Rotation;
               /* +0x12C */ float FOV;
               /* padding */ unsigned char __unk3[0x1C];
               /* +0x14C */ Vector3 Position;
               /* padding */ unsigned char __unk4[0x20];
               /* +0x178 */ float MinimumZoom;
               /* +0x17C */ float MaximumZoom;
               /* +0x180 */ float MinimumVerticalAngle;
               /* +0x184 */ float MaximumVerticalAngle;
               /* +0x188 */ float MinimumHorizontalAngle;
               /* +0x18C */ float MaximumHorizontalAngle;

               void operator=(const FreeCameraProperties& newProperties) {
                  this->VerticalAngle = newProperties.VerticalAngle;
                  this->HorizontalAngle = newProperties.HorizontalAngle;
                  this->Zoom = newProperties.Zoom;
                  this->Rotation = newProperties.Rotation;
                  this->FOV = newProperties.FOV;
                  this->Position = newProperties.Position;
                  memcpy_s(&MinimumZoom, sizeof(float) * 6, &newProperties.MinimumZoom, sizeof(float) * 6);
               }
            };

            SafehouseCarPosition newCarPosition      ={};
            FreeCameraProperties newCameraProperties ={};

            void ChangeCarPositionOnEntry() {
               unsigned char* carPositionAddr = (unsigned char*)Memory::readPointer(0x89F7DC, false);
               *(float*)(carPositionAddr + 0x10) = newCarPosition.Position.x;
               *(float*)(carPositionAddr + 0x14) = newCarPosition.Position.y;
               *(float*)(carPositionAddr + 0x18) = newCarPosition.Position.z;
               *(USHORT*)(carPositionAddr + 0x20) = newCarPosition.Rotation;

               DWORD targetAddr;
               // Z Position
               targetAddr = Memory::makeAbsolute(0x7B5FA8);
               //Memory::openMemoryAccess(targetAddr, sizeof(float));
               *(float*)targetAddr = -newCarPosition.Position.z;
               //Memory::restoreMemoryAccess();
               // Z Shadow
               targetAddr = Memory::makeAbsolute(0x7C5154);
               //Memory::openMemoryAccess(targetAddr, sizeof(float));
               *(float*)targetAddr = newCarPosition.Position.z + 0.05f;
               //Memory::restoreMemoryAccess();
               // Tire Rotation
               targetAddr = Memory::makeAbsolute(0x910CCC);
               //Memory::openMemoryAccess(targetAddr, sizeof(float));
               *(float*)targetAddr = newCarPosition.TireRotation;
               //Memory::restoreMemoryAccess();
            }
            DWORD WINAPI OnSafehouseEntry(LPVOID) {
               ChangeCarPositionOnEntry();
               *(Hooks::NewSafehouses::FreeCameraProperties*)Memory::readPointer(0x9C0BE4, false) = newCameraProperties;
               return TRUE;
            }

#pragma region Camera hooks
            DWORD hkLoadCameraOnRedefineReturnAddress;
            void __declspec(naked) hkLoadCameraOnRedefine() {
               __asm {

                  // if you want to edit the first safehouse
                  //cmp[userLoggedIn], 1
                  //jne loginSkip

                  // to let free camera alone
                  //cmp[playerState], 2
                  //jne camNotInit

               loginSkip:
                  push eax
                     mov eax, [newCameraProperties.Rotation]
                     mov[ebp + 0xC], eax

                     mov eax, [newCameraProperties.FOV]
                     mov[ebp + 0x10], eax

                     camNotInit :
                  pop eax

                     fld dword ptr[ecx + 0xA0]
                     jmp[hkLoadCameraOnRedefineReturnAddress]
               }
            }
            void __declspec(naked) hkLoadCameraOnRedefineBoundaries() {
               __asm {
                  push ebp
                  mov ebp, esp

                  //cmp[userLoggedIn], 1
                  //jne loginSkip

                  // to let free camera alone
                  //cmp[playerState], 2
                  //jne camNotInit

                  movss xmm0, [newCameraProperties.MinimumZoom]
                  movss[ecx + 0x17C], xmm0
                  movss xmm0, [newCameraProperties.MaximumZoom]
                  movss[ecx + 0x180], xmm0
                  movss xmm0, [newCameraProperties.MinimumHorizontalAngle]
                  movss[ecx + 0x18C], xmm0
                  movss xmm0, [newCameraProperties.MaximumHorizontalAngle]
                  movss[ecx + 0x190], xmm0
                  movss xmm0, [newCameraProperties.MinimumVerticalAngle]
                  movss[ecx + 0x184], xmm0
                  movss xmm0, [newCameraProperties.MaximumVerticalAngle]
                  movss[ecx + 0x188], xmm0
                  pop ebp
                  ret 0x18
               }
            }
            DWORD hkLoadCameraOnEntryReturnAddress;
            void __declspec(naked) hkLoadCameraOnEntry() {
               __asm {
                  mov eax, [ebp + 0x8]

                  push ebx

                  mov ebx, [newCameraProperties.VerticalAngle]
                  mov[eax], ebx

                  mov ebx, [newCameraProperties.HorizontalAngle]
                  mov[eax + 0x4], ebx

                  mov ebx, [newCameraProperties.Zoom]
                  mov[eax + 0x8], ebx

                  pop ebx

                  fld dword ptr[eax]
                  jmp[hkLoadCameraOnEntryReturnAddress]
               }
            }
            DWORD hkLoadCameraOnMenuReturnAddress;
            void __declspec(naked) hkLoadCameraOnMenu() {
               __asm {
                  mov eax, [ebp + 0x24]
                  fld dword ptr[ebp + 0x20]
                  push eax

                  mov eax, [newCameraProperties.Rotation]
                  mov[ebp + 0xC], eax

                  mov eax, [newCameraProperties.FOV]
                  mov[ebp + 0x10], eax

                  mov eax, [newCameraProperties.VerticalAngle]
                  mov[ebp + 0x38], eax

                  mov eax, [newCameraProperties.HorizontalAngle]
                  mov[ebp + 0x3C], eax

                  mov eax, [newCameraProperties.Zoom]
                  mov[ebp + 0x40], eax

                  mov eax, [newCameraProperties.Position.x]
                  mov[ebp + 0x48], eax

                  mov eax, [newCameraProperties.Position.y]
                  mov[ebp + 0x4C], eax

                  mov eax, [newCameraProperties.Position.z]
                  mov[ebp + 0x50], eax

                  pop eax
                  jmp[hkLoadCameraOnMenuReturnAddress]
               }
            }
#pragma endregion
#pragma region Safehouse hooks
            DWORD hkSafehouseOnEntryInnerCall;
            DWORD hkSafehouseOnEntryReturnAddress;

            void __declspec(naked) hkSafehouseOnEntry() {
               __asm call hkSafehouseOnEntryInnerCall;
               CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)& OnSafehouseEntry, NULL, 0, NULL);
               __asm jmp hkSafehouseOnEntryReturnAddress;
            }
#pragma endregion
         }
      }
      struct NewSafehouses : _BaseInGameMenuItem {
      private:
         bool hasInit = false;
      public:
         const virtual void loadData() override {
            hasLoadedData = true;
         }

         const virtual void onFrame() override {}

         const virtual bool displayMenuItem(const ImVec2& buttonSize) override {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.08f, 0.59f, 0.26f, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08f, 0.59f, 0.26f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.08f, 0.53f, 0.06f, 1.00f));
            bool ret = ImGui::Button("Test new safehouses", buttonSize);
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            return ret;
         }

         const virtual bool displayMenu() override {
            if (!hasInit) {
               if (hasInit = ImGui::Button("Init code")) {
                  // Car
                  {
                     Memory::writeRaw(0x256EF4 + 1, false, 1, 0x1);

                     Hooks::NewSafehouses::newCarPosition.Position = Vector3(4686.174f, 289.403f, 40.280f);
                     Hooks::NewSafehouses::newCarPosition.Rotation = (USHORT)0;
                     Hooks::NewSafehouses::newCarPosition.TireRotation = -15.652f;

#pragma region DEBUGGING 
                     // Z Position
                     DWORD targetAddr = Memory::makeAbsolute(0x7B5FA8);
                     Memory::openMemoryAccess(targetAddr, sizeof(float));
                     // Z Shadow
                     targetAddr = Memory::makeAbsolute(0x7C5154);
                     Memory::openMemoryAccess(targetAddr, sizeof(float));
                     targetAddr = Memory::makeAbsolute(0x910CCC);
                     Memory::openMemoryAccess(targetAddr, sizeof(float));
#pragma endregion
                  }
                  // Camera
                  {
                     Hooks::NewSafehouses::newCameraProperties.Position = Vector3(4686.174f, 288.0f, 41.5f);
                     Hooks::NewSafehouses::newCameraProperties.Zoom = 11.0f;
                     Hooks::NewSafehouses::newCameraProperties.FOV = 21.217f;
                     Hooks::NewSafehouses::newCameraProperties.Rotation = 0.0f;
                     Hooks::NewSafehouses::newCameraProperties.VerticalAngle = 89.0f;
                     Hooks::NewSafehouses::newCameraProperties.HorizontalAngle = 73.0f;
                     Hooks::NewSafehouses::newCameraProperties.MinimumZoom = 11.0f;
                     Hooks::NewSafehouses::newCameraProperties.MaximumZoom = 11.01f;
                     Hooks::NewSafehouses::newCameraProperties.MinimumVerticalAngle = 89.0f;
                     Hooks::NewSafehouses::newCameraProperties.MaximumVerticalAngle = 89.01f;
                     Hooks::NewSafehouses::newCameraProperties.MinimumHorizontalAngle = 73.0f;
                     Hooks::NewSafehouses::newCameraProperties.MaximumHorizontalAngle = 73.01f;

                     Hooks::NewSafehouses::hkLoadCameraOnRedefineReturnAddress = Memory::makeAbsolute(0x3D17C3);
                     Hooks::NewSafehouses::hkLoadCameraOnEntryReturnAddress = Memory::makeAbsolute(0x3CD698);
                     Hooks::NewSafehouses::hkLoadCameraOnMenuReturnAddress = Memory::makeAbsolute(0x3D1AD9);

                     // Variables
                     Memory::writeNop(0x3DCCB8, 8, false); // VerticalAngle
                     Memory::writeNop(0x3DCCDC, 8, false); // HorizontalAngle
                     Memory::writeNop(0x3DCD00, 8, false); // Zoom

                     // Boundaries
                     Memory::writeJMP(0x3D17BD, (DWORD)& Hooks::NewSafehouses::hkLoadCameraOnRedefine, false);
                     Memory::writeCall(0x253C93, (DWORD)& Hooks::NewSafehouses::hkLoadCameraOnRedefineBoundaries, false);
                     Memory::writeCall(0x255974, (DWORD)& Hooks::NewSafehouses::hkLoadCameraOnRedefineBoundaries, false);
                     Memory::writeJMP(0x3CD693, (DWORD)& Hooks::NewSafehouses::hkLoadCameraOnEntry, false);
                     Memory::writeJMP(0x3D1AD3, (DWORD)& Hooks::NewSafehouses::hkLoadCameraOnMenu, false);
                  }
                  // Safehouse
                  {
                     Hooks::NewSafehouses::hkSafehouseOnEntryInnerCall = Memory::makeAbsolute(0x2C4460);
                     Hooks::NewSafehouses::hkSafehouseOnEntryReturnAddress = Memory::makeAbsolute(0x253CA7);

                     Memory::writeJMP(0x253CA2, (DWORD)Hooks::NewSafehouses::hkSafehouseOnEntry, false);
                  }
               }
            }
            else {
               ImGui::Text("Code initialized, hooks are running.");
               ImGui::Separator();
               ImGui::Text("Debug options");
               if (ImGui::BeginChild("##CarInfo", ImVec2(0.0f, 150.0f), true)) {
                  ImGui::Text("Car Controls");
                  unsigned char* pCarInfo = (unsigned char*)Memory::readPointer(0x89F7DC, false);

                  Vector3* pos = (Vector3*)(pCarInfo + 0x10);
                  USHORT* rot = (USHORT*)(pCarInfo + 0x20);
                  static float* posZReal = (float*)Memory::makeAbsolute(0x7B5FA8);
                  static float* shadowZ = (float*)Memory::makeAbsolute(0x7C5154);
                  static float* tireRot = (float*)Memory::makeAbsolute(0x910CCC);

                  if (ImGui::InputFloat3("Position", *pos)) {
                     *posZReal = -pos->z;
                     *shadowZ = pos->z - 0.015f;

                     memcpy_s(Hooks::NewSafehouses::newCarPosition.Position, sizeof(Vector3), pos, sizeof(Vector3));
                  }
                  ImGui::InputFloat("Z Shadow", shadowZ, 0.01f, 0.1f);
                  ImGui::SliderFloat("Tire Rotation", tireRot, -180.0f, 180.0f);
                  if (ImGui::SliderInt("Rotation", (int*)rot, -36000, 36000)) {
                     Hooks::NewSafehouses::newCarPosition.Rotation = *rot;
                  }
               } ImGui::EndChild();
               if (ImGui::BeginChild("##CameraInfo", ImVec2(0.0f, 250.0f), true)) {
                  ImGui::Text("Camera Controls");
                  auto* pActiveCamera = (Hooks::NewSafehouses::FreeCameraProperties*)Memory::readPointer(0x9C0BE4, false);

                  if (ImGui::InputFloat3("Position", pActiveCamera->Position) ||
                     ImGui::SliderFloat2("Angles", &pActiveCamera->VerticalAngle, -360.0f, 360.0f) ||
                     ImGui::SliderFloat("Rotation", &pActiveCamera->Rotation, -360.0f, 3360.0f) ||
                     ImGui::SliderFloat("FOV", &pActiveCamera->FOV, 1.0f, 280.0f) ||
                     ImGui::SliderFloat("Zoom", &pActiveCamera->Zoom, 1.0f, 180.0f)) {
                     Hooks::NewSafehouses::newCameraProperties = *pActiveCamera;
                  }

                  ImGui::Text("Boundaries");
                  if (ImGui::SliderFloat2("min/max VAngle", &pActiveCamera->MinimumVerticalAngle, -360.0f, 360.0f) ||
                     ImGui::SliderFloat2("min/max HAngle", &pActiveCamera->MinimumHorizontalAngle, -360.0f, 360.0f) ||
                     ImGui::SliderFloat2("min/max Zoom", &pActiveCamera->MinimumZoom, 0.0f, 180.0f)) {
                     Hooks::NewSafehouses::newCameraProperties = *pActiveCamera;
                  }
               } ImGui::EndChild();
            }

            return true;
         }
      };
   }
}