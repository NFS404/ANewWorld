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
#include "Helpers\ServerTalk\GameServerTalk.hpp"
#include "Helpers\ServerTalk\Object\ListOfHashes.hpp"

#include "NewSafehouses.hpp"

#pragma warning(push)
#pragma warning(disable : 4018)
namespace Extensions {
   namespace InGameMenu {
      static void __cdecl __redirectvprintf(char* frmt, va_list pf) {
         vprintf(frmt, pf);
         printf("\n");
      }
      struct Debug : _BaseInGameMenuItem {
         void playerdebug() {
            static int32_t    iActivePVehicle  = -1;
            static PVehicle* pActivePVehicle  = nullptr;
            static RigidBody* pActiveRigidBody = nullptr;
            static bool dbgISimable, dbgIVehicle, dbgRigidBody;
            static char szActivePVehicleName[256] ={ 0 };
            static char szIteratorItemName[256]   ={ 0 };

            if (iActivePVehicle < 0)
               ImGui::TextColored(ImVec4(1, 0, 0, 1), "NO ACTIVE PVEHICLE POINTER!");
            else {
               ISimable* pISimable = GameVariables::arrSimables->elements[iActivePVehicle];
               if (!pISimable || GameVariables::arrSimables->count < (iActivePVehicle + 1)) {
                  iActivePVehicle = -1;
                  pActivePVehicle = nullptr;
                  pActiveRigidBody = nullptr;
                  ZeroMemory(szActivePVehicleName, sizeof(szActivePVehicleName));
               } else {
                  PVehicle* pPVehicle = (PVehicle*)((DWORD)pISimable - 0x24);
                  if (pActivePVehicle != pPVehicle) {
                     pActivePVehicle  = pPVehicle;
                     pActiveRigidBody = (RigidBody*)pActivePVehicle->GetRigidBody();
                     sprintf_s(szActivePVehicleName, sizeof(szActivePVehicleName),
                               "[i: %d - addr: %p] %s", iActivePVehicle, pActivePVehicle, pActivePVehicle->GetVehicleName());
                  }
               }
               ImGui::TextColored(ImVec4(0, 1, 0, 1), "Active PVehicle pointer:   %p", pActivePVehicle);
               ImGui::TextColored(ImVec4(0.5, 0.5, 0, 1), "FECustomizationRecord: %p", (char*)pActivePVehicle + 0xF4);
               ImGui::TextColored(ImVec4(0.5, 0.5, 0, 1), "VehicleCustomizations: %p", pActivePVehicle->GetCustomizations());
            }
            if (!pActiveRigidBody)
               ImGui::TextColored(ImVec4(1, 0, 0, 1), "NO ACTIVE RIGIDBODY POINTER!");
            else
               ImGui::TextColored(ImVec4(0, 1, 0, 1), "Active RigidBody pointer: %p", pActiveRigidBody);

            ImGui::Text("PVehicle instances (%d): ", GameVariables::arrSimables->count); ImGui::SameLine();
            if (ImGui::BeginCombo("##ISimableArrayCombo", szActivePVehicleName, ImGuiComboFlags_HeightLarge)) {
               for (int32_t i = 0; i < GameVariables::arrSimables->count; i++) {
                  ISimable* pIteratorISimable = GameVariables::arrSimables->elements[i];
                  PVehicle* pIteratorPVehicle = (PVehicle*)((DWORD)pIteratorISimable - 0x24);

                  sprintf_s(szIteratorItemName, sizeof(szActivePVehicleName),
                            "[i: %d - addr: %p] %s", i, pIteratorPVehicle, pIteratorPVehicle->GetVehicleName());
                  if (ImGui::Selectable(szIteratorItemName, pActivePVehicle == pIteratorPVehicle)) {
                     iActivePVehicle  = i;
                     pActivePVehicle  = pIteratorPVehicle;
                     pActiveRigidBody = (RigidBody*)pIteratorPVehicle->GetRigidBody();
                     strcpy_s(szActivePVehicleName, sizeof(szActivePVehicleName), szIteratorItemName);
                  }
               }
               ImGui::EndCombo();
            }
            if (pActivePVehicle) {
               ImGui::Text("x: %.3f", pActivePVehicle->GetPosition()->x);
               ImGui::Text("y: %.3f", pActivePVehicle->GetPosition()->y);
               ImGui::Text("z: %.3f", pActivePVehicle->GetPosition()->z);

               if (ImGui::Button("do 3")) {
                  DWORD targetAddr = Memory::makeAbsolute(0x8FC730);
                  Memory::openMemoryAccess(targetAddr, 4);
                  *(uint16_t*)targetAddr = (uint16_t)3;
                  Memory::restoreMemoryAccess();
               } ImGui::SameLine();
               if (ImGui::Button("do 6")) {
                  DWORD targetAddr = Memory::makeAbsolute(0x8FC730);
                  Memory::openMemoryAccess(targetAddr, 4);
                  *(uint16_t*)targetAddr = (uint16_t)6;
                  Memory::restoreMemoryAccess();
               }
               if (ImGui::Button("set free camera to active pvehicle")) {
                  auto* pActiveCamera = (Hooks::NewSafehouses::FreeCameraProperties*)Memory::readPointer(0x9C0BE4, false);
                  pActiveCamera->Position ={ pActivePVehicle->GetPosition()->x, -pActivePVehicle->GetPosition()->y, pActivePVehicle->GetPosition()->z };
               }
               if (ImGui::Button("spawn a crop vehicle near active pvehicle")) {
                  UMath::Vector3 aa{
                     pActivePVehicle->GetPosition()->x,
                     pActivePVehicle->GetPosition()->y,
                     pActivePVehicle->GetPosition()->z + 10.0f
                  };
                  UMath::Vector3 b;
                  UMath::ExtractZAxis(pActivePVehicle->GetRigidBody()->GetOrientation(), &b);
                  auto* xx = PVehicle::Create(DriverClass::DRIVER_COP, Attrib::StringToKey("copmidsize"), nullptr, &b, &aa);
                  pActivePVehicle = xx;
                  pActiveRigidBody = (RigidBody*)xx->GetRigidBody();
                  iActivePVehicle = GameVariables::arrSimables->count - 1;
               }
               if (ImGui::Button("delet")) {
                  auto* xxxxx = pActivePVehicle->GetPlayer();
                  if (xxxxx)
                     pActivePVehicle->Detach(xxxxx);
                  pActivePVehicle->Kill();
                  pActivePVehicle = nullptr;
                  pActiveRigidBody = nullptr;
                  iActivePVehicle = -1;
               }
               if (ImGui::Button("delet4real")) {
                  pActivePVehicle->~PVehicle();
               }
               if (ImGui::Button("spawn car with new customization on active pvehicle")) {
                  static char __space[0x800];
                  __asm {
                     push edx
                     mov edx, 0x672150
                     call edx
                     test eax, eax
                     call edx
                     mov ecx, eax

                     push ebp
                     mov ebp, esp
                     sub esp, 0x00000894
                     // -
                     mov edx, 0x680240
                     // -
                     call edx
                     mov edx, [eax]
                     mov edx, [edx+0x20]
                     lea ecx, [ebp-0x000007FC]
                     push ecx
                     mov ecx, eax
                     call edx
                     mov eax, [ebp-0x000007FC]
                     push eax
                     lea ecx, [ebp-0x00000894]
                     // -
                     mov edx, 0x41B100
                     // -
                     call edx
                     lea ecx, [ebp-0x000007EC]
                     // -
                     mov edx, 0x652AC0
                     // - 
                     call edx
                     lea ecx, [ebp-0x00000894]
                     push ecx
                     lea ecx, [ebp-0x000007EC]
                     // -
                     mov edx, 0x7317A0
                     // - 
                     call edx
                     lea ecx, [ebp-0x000007EC]
                     push 0x800
                     push ecx
                     lea ecx, [__space]
                     push ecx
                     // -
                     mov edx, 0x97C986
                     // -
                     call edx
                     add esp, 0xC

                     // end
                     lea ecx, [ebp-0x00000894]
                     // -
                     mov edx, 0x4105D0
                     // - 
                     call edx
                     lea ecx, [ebp-0x000007FC]
                     // -
                     mov edx, 0x41D0D0
                     // - 
                     call edx
                     add esp, 0x00000894
                     mov esp, ebp
                     pop ebp
                     pop edx
                  }

                  UMath::Vector3 aa{
                     pActivePVehicle->GetPosition()->x + 10.0f,
                     pActivePVehicle->GetPosition()->y + 10.0f,
                     pActivePVehicle->GetPosition()->z
                  };
                  UMath::Vector3 b;
                  UMath::ExtractZAxis(pActivePVehicle->GetRigidBody()->GetOrientation(), &b);
                  auto* xx = PVehicle::Create(DriverClass::DRIVER_REMOTE, pActivePVehicle->GetVehicleKey(), (FECustomizationRecord*)__space, &b, &aa);
                  xx->ForceStopOn(ForceStopType::kEventInstantStop);
               }
            }
         }

         const virtual void loadData() override {
            hasLoadedData = true;

            DWORD addrsToRedirect[]{
               // MiniGarage
               //0x250A59, //DisableCarRendering
               //0x257227, //DisableCarRendering
               //0x259753, //DisableCarRendering
               //0x2599C1, //DisableCarRendering
               0x253DA3,
               0x253D68,
               0x254189,
               0x254168,
               0x2540F3,
               0x259A9B,
               0x259C41,
               0x259C81,
               // GarageCarLoader
               0x256295,
               0x256233,
               0x2573B0,
               0x257336,
               0x257310,
               // CarLoader
               0x33358E
            };
            for (int i = 0; i < (sizeof(addrsToRedirect) / sizeof(DWORD)); i++) {
               Memory::writeCall(addrsToRedirect[i], (DWORD)&__redirectvprintf, false);
            }
         }

         const virtual void onFrame() override {}
         const virtual void beforeReset() override {}
         const virtual void afterReset() override {}

         const virtual bool displayMenuItem(const ImVec2& buttonSize) override {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.98f, 0.59f, 0.26f, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.98f, 0.59f, 0.26f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.98f, 0.53f, 0.06f, 1.00f));
            bool ret = ImGui::Button("Debug", buttonSize);
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            return ret;
         }

         const virtual bool displayMenu() override {
            if (ImGui::Begin("GenericDebug")) {
               ImGui::Text("Debug stuff");
               if (ImGui::Button("Dump exhausted memory (kills the game)"))
                  Memory::writeRaw(0xA006C0, false, 1, 0x00);
               if (ImGui::Button("Dump corrupted memory (kills the game)"))
                  Memory::writeRaw(0xA006C4, false, 1, 0x00);
               if (ImGui::Button("Dump something to console (kills the game)"))
                  Memory::writeRaw(0xA006C8, false, 1, 0x00);
               if (ImGui::Button("Dump something2 to console (kills the game)"))
                  Memory::writeRaw(0xA006CC, false, 1, 0x00);
            } ImGui::End();

            if (ImGui::Begin("ServerDebug")) {
               ImGui::Text("Server stuff");

               if (ImGui::Button("test server call")) {
                  ServerTalkObjects::ListOfHashes lHashes;
                  lHashes.addFileToHashMap("nfsw.exe");
                  lHashes.addFileToHashMap("gameplay.dll");
                  lHashes.addFileToHashMap("gameplay.native.dll");
                  lHashes.addFileToHashMap((const char*)Memory::makeAbsolute(0x3DB7CA)); // modded-in game string: "ANewWorld.WorldMod"
                  lHashes.addFileToHashMap(*(const char**)Memory::makeAbsolute(0x8C45A4)); // "GLOBAL\\attributes.bin"
                  lHashes.addFileToHashMap(*(const char**)Memory::makeAbsolute(0x8C45A8)); // "GLOBAL\\commerce.bin"
                  lHashes.addFileToHashMap(*(const char**)Memory::makeAbsolute(0x8C4518)); // "GLOBAL\\GlobalB.lzc"
                  lHashes.addFileToHashMap(*(const char**)Memory::makeAbsolute(0x8C451C)); // "CARS\\GlobalC.lzc"
                  lHashes.addFileToHashMap("Settings\\SystemSettings.dat");

                  //GameServerTalk::GetInstance()->SendPostRequest("Reporting/Test", lHashes);
               }
            } ImGui::End();

            if (ImGui::Begin("try pursuit")) {

            } ImGui::End();

            //static PVehicle *b;
            //if (ImGui::Button("get cust")) {
            //   ISimable *ppveh = GameVariables::arrSimables->elements[0];
            //   auto* pp = ppveh->GetPosition();
            //   UMath::Vector3 a(1.0f, 1.0f, 1.0f);
            //   b = PVehicle::Create(DriverClass::Racer, Attrib::StringToKey("silvias15"), *(FECustomizationRecord**)((char*)ppveh - 0x24 + 0xF4), pp, &a);
            //   printf("PVEH: %8X\n", b);
            //}
            static char      szCarName[256];
            static PVehicle* pLastCreated = nullptr;
            ImGui::InputText("##carname", szCarName, sizeof(szCarName), ImGuiInputTextFlags_CharsNoBlank);
            if (ImGui::Button("Spawn on player")) {
               for (size_t i = 0; i < sizeof(szCarName) - 1; i++)
                  szCarName[i] = tolower(szCarName[i]);
               DWORD hash = Attrib::StringToKey(szCarName);
               if (!hash)
                  strcpy(szCarName, "null");
               else {
                  int i_max = GameVariables::arrSimables->count;
                  for (int i = 0; i < i_max; i++) {
                     if (GameVariables::arrSimables->elements[i]->IsOwnedByPlayer()) {
                        PVehicle* pIteratorPVehicle = (PVehicle*)((DWORD)GameVariables::arrSimables->elements[i] - 0x24);
                        UMath::Vector3 _rot;
                        UMath::ExtractZAxis(pIteratorPVehicle->GetRigidBody()->GetOrientation(), &_rot);
                        pLastCreated = PVehicle::Create(DriverClass::DRIVER_REMOTE, hash, nullptr, &_rot, pIteratorPVehicle->GetPosition());
                     }
                  }
               }
            } ImGui::SameLine(); ImGui::Text("Last created PVehicle: %p", pLastCreated);

            static bool pVehicleDebug = false;
            //ImGui::Checkbox("debug PVehicle", &pVehicleDebug);
            if (pVehicleDebug)
               playerdebug();




            return true;
         }
      };
   }
}
#pragma warning(pop)