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
#include "DI8Types.h"
#include "..\VTableHook.hpp"
#include <memory>
using std::unique_ptr;
using std::make_unique;

namespace LocalMirrorHook {
   enum class DI8Device {
      Keyboard,
      Mouse
   };
   enum class DI8Extension {
      GetDeviceState
   };

   namespace DI8 {
      static std::map<DI8Device, std::vector<GetDeviceState_t>> mGetDeviceStateExtensions;

   #pragma region function hooks
      static std::map<DWORD, GetDeviceState_t> origGetDeviceStates_Keyboard;
      static std::map<DWORD, GetDeviceState_t> origGetDeviceStates_Mouse;

      static HRESULT WINAPI hkGetDeviceState_Keyboard(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         auto retOrig = origGetDeviceStates_Keyboard[(DWORD)hInstance](hInstance, cbData, lpvData);

         for (GetDeviceState_t keyboardExtension : mGetDeviceStateExtensions[DI8Device::Keyboard]) {
            if (keyboardExtension)
               keyboardExtension(hInstance, cbData, lpvData);
         }
         return retOrig;
      }
      static HRESULT WINAPI hkGetDeviceState_Mouse(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         auto retOrig = origGetDeviceStates_Mouse[(DWORD)hInstance](hInstance, cbData, lpvData);

         for (GetDeviceState_t mouseExtension : mGetDeviceStateExtensions[DI8Device::Mouse]) {
            if (mouseExtension)
               mouseExtension(hInstance, cbData, lpvData);
         }
         return retOrig;
      }
   #pragma endregion

   #pragma region helpers
      static HRESULT AddExtension(DI8Device deviceType, DI8Extension extensionType, LPVOID extensionAddress) {
         switch (extensionType) {
            case DI8Extension::GetDeviceState:
            {
               switch (deviceType) {
                  case DI8Device::Keyboard:
                  case DI8Device::Mouse:
                     mGetDeviceStateExtensions[deviceType].push_back(reinterpret_cast<GetDeviceState_t>(extensionAddress));
                     return TRUE;
               }
            }
         }
         return FALSE;
      }
   #pragma endregion

      static BOOL CALLBACK enumCallbackA(LPCDIDEVICEINSTANCEA lpddi, LPVOID var) {
         BYTE deviceType = LOBYTE(lpddi->dwDevType);
         if (deviceType == DI8DEVTYPE_KEYBOARD || deviceType == DI8DEVTYPE_MOUSE) {
            DWORD* inputTable;
            LPDIRECTINPUT8A lpdi8 = (LPDIRECTINPUT8A)var;
            LPDIRECTINPUTDEVICE8A device;

            if (deviceType == DI8DEVTYPE_KEYBOARD) {
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStates_Keyboard[(DWORD)lpdi8] = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceState_Keyboard;
               Memory::restoreMemoryAccess();
            } else {
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStates_Mouse[(DWORD)lpdi8] = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceState_Mouse;
               Memory::restoreMemoryAccess();
            }
         }

         return DIENUM_CONTINUE;
      }
      static BOOL CALLBACK enumCallbackW(LPCDIDEVICEINSTANCEW lpddi, LPVOID var) {
         BYTE deviceType = LOBYTE(lpddi->dwDevType);
         if (deviceType == DI8DEVTYPE_KEYBOARD || deviceType == DI8DEVTYPE_MOUSE) {
            DWORD* inputTable;
            LPDIRECTINPUT8W lpdi8 = (LPDIRECTINPUT8W)var;
            LPDIRECTINPUTDEVICE8W device;

            if (deviceType == DI8DEVTYPE_KEYBOARD) {
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStates_Keyboard[(DWORD)lpdi8] = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceState_Keyboard;
               Memory::restoreMemoryAccess();
            } else {
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStates_Mouse[(DWORD)lpdi8] = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceState_Mouse;
               Memory::restoreMemoryAccess();
            }
         }

         return DIENUM_CONTINUE;
      }

      static DWORD origCall = 0x597E1A;
      static void __declspec(naked) hkDirectInput8ACreate() {
         __asm {
            enter 0, 0
            pushad
         }
         LPDIRECTINPUT8A lpdi8A;
         __asm {
            push eax
            mov eax, [ecx+0xB4]
            mov eax, [eax]
            mov[lpdi8A], eax
            pop eax
         }
         lpdi8A->EnumDevices(DI8DEVCLASS_ALL, &enumCallbackA, NULL, DIEDFL_ATTACHEDONLY);
         __asm {
            popad
            leave
            jmp[origCall]
         }
      }
      static void Init() {
         mGetDeviceStateExtensions[DI8Device::Keyboard] = std::vector<GetDeviceState_t>();
         mGetDeviceStateExtensions[DI8Device::Mouse]    = std::vector<GetDeviceState_t>();

         (*(LPDIRECTINPUT8W*)Memory::makeAbsolute(0x9C2C34))->EnumDevices(DI8DEVCLASS_ALL, &enumCallbackW, NULL, DIEDFL_ATTACHEDONLY);

         origCall += Memory::baseAddress;
         Memory::writeJMP(0x597ECE, (DWORD)&hkDirectInput8ACreate, false);
      }
   }
}