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
   enum class DI8Device : uint32_t {
      Keyboard = 0,
      Mouse    = 1
   };
   enum class DI8Extension : uint32_t {
      GetDeviceState = 0
   };

   namespace DI8 {
      inline std::map<DI8Device, std::vector<GetDeviceState_t>> mGetDeviceStateExtensions;

   #pragma region function hooks
      inline GetDeviceState_t origGetDeviceStateA_Keyboard;
      inline GetDeviceState_t origGetDeviceStateA_Mouse;
      static HRESULT WINAPI hkGetDeviceStateA_Keyboard(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         auto retOrig = origGetDeviceStateA_Keyboard(hInstance, cbData, lpvData);

         for (GetDeviceState_t keyboardExtension : mGetDeviceStateExtensions[DI8Device::Keyboard]) {
            if (keyboardExtension)
               keyboardExtension(hInstance, cbData, lpvData);
         }
         return retOrig;
      }
      static HRESULT WINAPI hkGetDeviceStateA_Mouse(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         auto retOrig = origGetDeviceStateA_Mouse(hInstance, cbData, lpvData);

         for (GetDeviceState_t mouseExtension : mGetDeviceStateExtensions[DI8Device::Mouse]) {
            if (mouseExtension)
               mouseExtension(hInstance, cbData, lpvData);
         }
         return retOrig;
      }

      inline GetDeviceState_t origGetDeviceStateW_Keyboard;
      inline GetDeviceState_t origGetDeviceStateW_Mouse;
      static HRESULT WINAPI hkGetDeviceStateW_Keyboard(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         auto retOrig = origGetDeviceStateW_Keyboard(hInstance, cbData, lpvData);

         for (GetDeviceState_t keyboardExtension : mGetDeviceStateExtensions[DI8Device::Keyboard]) {
            if (keyboardExtension)
               keyboardExtension(hInstance, cbData, lpvData);
         }
         return retOrig;
      }
      static HRESULT WINAPI hkGetDeviceStateW_Mouse(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         auto retOrig = origGetDeviceStateW_Mouse(hInstance, cbData, lpvData);

         for (GetDeviceState_t mouseExtension : mGetDeviceStateExtensions[DI8Device::Mouse]) {
            if (mouseExtension)
               mouseExtension(hInstance, cbData, lpvData);
         }
         return retOrig;
      }
   #pragma endregion

   #pragma region helpers
      static HRESULT AddExtension(DI8Device deviceType, DI8Extension extensionType, LPVOID extensionAddress) {
         Log(LogLevel::Debug, Logger::FormatString("Adding DI8 extension (type %u) for device type-%u from %p.", extensionType, deviceType, extensionAddress));
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
            static bool keyboardCaptured = false;
            static bool mouseCaptured    = false;

            if (deviceType == DI8DEVTYPE_KEYBOARD && !keyboardCaptured) {
               Log(LogLevel::Debug, "KeyboardA detected.");
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStateA_Keyboard = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceStateA_Keyboard;
               Memory::restoreMemoryAccess();
               Log(LogLevel::Debug, "KeyboardA captured.");
               keyboardCaptured = true;
            }
            if (deviceType == DI8DEVTYPE_MOUSE && !mouseCaptured) {
               Log(LogLevel::Debug, "MouseA detected.");
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStateA_Mouse = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceStateA_Mouse;
               Memory::restoreMemoryAccess();
               Log(LogLevel::Debug, "MouseA captured.");
               mouseCaptured = true;
            }
            if (keyboardCaptured && mouseCaptured)
               return DIENUM_STOP;
         }

         return DIENUM_CONTINUE;
      }
      static BOOL CALLBACK enumCallbackW(LPCDIDEVICEINSTANCEW lpddi, LPVOID var) {
         BYTE deviceType = LOBYTE(lpddi->dwDevType);
         if (deviceType == DI8DEVTYPE_KEYBOARD || deviceType == DI8DEVTYPE_MOUSE) {
            DWORD* inputTable;
            LPDIRECTINPUT8W lpdi8 = (LPDIRECTINPUT8W)var;
            LPDIRECTINPUTDEVICE8W device;
            static bool keyboardCaptured = false;
            static bool mouseCaptured    = false;

            if (deviceType == DI8DEVTYPE_KEYBOARD && !keyboardCaptured) {
               Log(LogLevel::Debug, "KeyboardW detected.");

               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStateW_Keyboard = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceStateW_Keyboard;
               Memory::restoreMemoryAccess();
               Log(LogLevel::Debug, "KeyboardW captured.");
               keyboardCaptured = true;
            }
            if (deviceType == DI8DEVTYPE_MOUSE && !mouseCaptured) {
               Log(LogLevel::Debug, "MouseW detected.");
               lpdi8->CreateDevice(lpddi->guidInstance, &device, NULL);
               inputTable = *(PDWORD*)device;
               origGetDeviceStateW_Mouse = (GetDeviceState_t)(DWORD)inputTable[9];

               Memory::openMemoryAccess(inputTable[9], 4);
               inputTable[9] = (DWORD)hkGetDeviceStateW_Mouse;
               Memory::restoreMemoryAccess();
               Log(LogLevel::Debug, "MouseW captured.");
               mouseCaptured = true;
            }
            if (keyboardCaptured && mouseCaptured)
               return DIENUM_STOP;
         }

         return DIENUM_CONTINUE;
      }

      static void __stdcall hkDirectInput8ACreate() {
         LPDIRECTINPUT8A lpdi8;
         PDWORD var;
         __asm {
            push eax
            push ebx
            mov eax, [ecx+0xB4]
            mov[lpdi8], eax
            mov[var], ecx
         }
         lpdi8->EnumDevices(DI8DEVCLASS_ALL, (LPDIENUMDEVICESCALLBACKA)Memory::makeAbsolute(0x597D24), var, DIEDFL_ATTACHEDONLY);
         lpdi8->EnumDevices(DI8DEVCLASS_KEYBOARD, &enumCallbackA, lpdi8, DIEDFL_ATTACHEDONLY);
         lpdi8->EnumDevices(DI8DEVCLASS_POINTER, &enumCallbackA, lpdi8, DIEDFL_ATTACHEDONLY);
         __asm {
            pop ebx
            pop eax
         }
      }

      inline DWORD origCallW = 0x3C4E30;
      static void __stdcall hkDirectInput8WCreate() {
         __asm {
            call[origCallW]
            pushad
         }
         (*(LPDIRECTINPUT8W*)Memory::makeAbsolute(0x9C2C34))->EnumDevices(DI8DEVCLASS_KEYBOARD, &enumCallbackW, (*(LPDIRECTINPUT8W*)Memory::makeAbsolute(0x9C2C34)), DIEDFL_ATTACHEDONLY);
         (*(LPDIRECTINPUT8W*)Memory::makeAbsolute(0x9C2C34))->EnumDevices(DI8DEVCLASS_POINTER, &enumCallbackW, (*(LPDIRECTINPUT8W*)Memory::makeAbsolute(0x9C2C34)), DIEDFL_ATTACHEDONLY);
         __asm popad;
      }

      static void Init() {
         Log(LogLevel::Debug, "Setting up DI8 hook.");
         mGetDeviceStateExtensions[DI8Device::Keyboard] = std::vector<GetDeviceState_t>();
         mGetDeviceStateExtensions[DI8Device::Mouse]    = std::vector<GetDeviceState_t>();

         origCallW += Memory::baseAddress;
         Memory::writeCall(0x597ECE, (DWORD)&hkDirectInput8ACreate, false);
         Memory::writeCall(0x3C6387, (DWORD)&hkDirectInput8WCreate, false);
         Log(LogLevel::Debug, "Hooks installed successfully.");
      }
   }
}