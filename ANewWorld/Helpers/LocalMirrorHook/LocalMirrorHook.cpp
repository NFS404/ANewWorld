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
#include "LocalMirrorHook.h"
#include "D3D9Types.h"
#include "DI8Types.h"

#include "VTableHook.hpp"
#include <memory>
using std::unique_ptr;
using std::make_unique;

namespace LocalMirrorHook {
   namespace D3D9 {
      DWORD             d3dDeviceAddress               = NULL;
      LPDIRECT3DDEVICE9 d3dDevice                      = nullptr;
      HWND              d3dWindow                      = nullptr;

      std::vector<BeginScene_t> vBeginSceneExtensions  ={};
      std::vector<EndScene_t>   vEndSceneExtensions    ={};
      std::vector<Reset_t>      vBeforeResetExtensions ={};
      std::vector<Reset_t>      vAfterResetExtensions  ={};

      bool isExtenderReady                             = false;

   #pragma region function hooks
      unique_ptr<VTableHook> d3dDeviceHook       = nullptr;
      BeginScene_t           origBeginScene      = nullptr;
      EndScene_t             origEndScene        = nullptr;
      Reset_t                origReset           = nullptr;
      BeginStateBlock_t      origBeginStateBlock = nullptr;

      HRESULT WINAPI hkBeginScene(LPDIRECT3DDEVICE9 pDevice) {
         if (pDevice->TestCooperativeLevel() == D3D_OK) {
            if (!vBeginSceneExtensions.empty()) {
               for (BeginScene_t beginSceneExtension : vBeginSceneExtensions) {
                  if (beginSceneExtension)
                     beginSceneExtension(pDevice);
               }
            }
         }
         return origBeginScene(pDevice);
      }
      HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
         if (pDevice->TestCooperativeLevel() == D3D_OK) {
            if (!vEndSceneExtensions.empty()) {
               for (EndScene_t endSceneExtension : vEndSceneExtensions) {
                  if (endSceneExtension)
                     endSceneExtension(pDevice);
               }
            }
         }
         return origEndScene(pDevice);
      }
      HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
         if (!vBeforeResetExtensions.empty()) {
            for (Reset_t beforeResetExtension : vBeforeResetExtensions) {
               if (beforeResetExtension)
                  beforeResetExtension(pDevice, pPresentationParameters);
            }
         }

         auto retOrigReset = origReset(pDevice, pPresentationParameters);

         if (!vAfterResetExtensions.empty()) {
            for (Reset_t afterResetExtension : vAfterResetExtensions) {
               if (afterResetExtension)
                  afterResetExtension(pDevice, pPresentationParameters);
            }
         }

         return retOrigReset;
      }
      HRESULT WINAPI hkBeginStateBlock(LPDIRECT3DDEVICE9 pDevice) {
         d3dDeviceHook->UnhookAll();
         auto retBeginStateBlock = origBeginStateBlock(pDevice);

         origReset                = d3dDeviceHook->Hook(16, hkReset);
         origBeginScene           = d3dDeviceHook->Hook(41, hkBeginScene);
         origEndScene             = d3dDeviceHook->Hook(42, hkEndScene);
         origBeginStateBlock      = d3dDeviceHook->Hook(60, hkBeginStateBlock);
         return retBeginStateBlock;
      }
   #pragma endregion

   #pragma region helpers
      HRESULT AddExtension(D3D9Extension extensionType, LPVOID extensionAddress) {
         switch (extensionType) {
            case D3D9Extension::BeginScene:
               vBeginSceneExtensions.push_back(reinterpret_cast<BeginScene_t>(extensionAddress));
               break;
            case D3D9Extension::EndScene:
               vEndSceneExtensions.push_back(reinterpret_cast<EndScene_t>(extensionAddress));
               break;
            case D3D9Extension::BeforeReset:
               vBeforeResetExtensions.push_back(reinterpret_cast<Reset_t>(extensionAddress));
               break;
            case D3D9Extension::AfterReset:
               vAfterResetExtensions.push_back(reinterpret_cast<Reset_t>(extensionAddress));
               break;
            default:
               return FALSE;
         }
         return TRUE;
      }
      LPDIRECT3DDEVICE9 GetD3D9Device() {
         if (!isExtenderReady)
            return nullptr;

         return d3dDevice;
      }
      HWND GetWindowHandle() {
         if (!isExtenderReady)
            return nullptr;

         return d3dWindow;
      }
      bool IsReady() {
         return isExtenderReady;
      }
   #pragma endregion

      void Init() {
         DWORD pD3DDevice = NULL;
         while (!pD3DDevice) {
            pD3DDevice = *(DWORD*)d3dDeviceAddress;
            Sleep(1000);
         }
         d3dDevice = (LPDIRECT3DDEVICE9)pD3DDevice;

         D3DDEVICE_CREATION_PARAMETERS cParams;
         d3dDevice->GetCreationParameters(&cParams);
         d3dWindow = cParams.hFocusWindow;

         d3dDeviceHook            = make_unique<VTableHook>((PDWORD*)d3dDevice);
         origReset                = d3dDeviceHook->Hook(16, hkReset);
         origBeginScene           = d3dDeviceHook->Hook(41, hkBeginScene);
         origEndScene             = d3dDeviceHook->Hook(42, hkEndScene);
         origBeginStateBlock      = d3dDeviceHook->Hook(60, hkBeginStateBlock);

         isExtenderReady = true;
      }
   }
   namespace DI8 {
      DWORD* pDInput8Address                   = nullptr;

      LPDIRECTINPUT8A       di8Instance                       = nullptr;
      LPDIRECTINPUTDEVICE8A device_Keyboard                   = nullptr;
      LPDIRECTINPUTDEVICE8A device_Mouse                      = nullptr;

      std::map<DI8Device, std::vector<GetDeviceState_t>> mGetDeviceStateExtensions;

      bool                  isExtenderReady                   = false;

   #pragma region function hooks
      GetDeviceState_t      origGetDeviceState_Keyboard       = nullptr;
      GetDeviceState_t      origGetDeviceState_Mouse          = nullptr;

      HRESULT WINAPI hkGetDeviceState_Keyboard(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         HRESULT retOrigGetDeviceState = origGetDeviceState_Keyboard(hInstance, cbData, lpvData);

         if (!mGetDeviceStateExtensions[DI8Device::Keyboard].empty()) {
            for (GetDeviceState_t keyboardExtension : mGetDeviceStateExtensions[DI8Device::Keyboard]) {
               if (keyboardExtension)
                  keyboardExtension(hInstance, cbData, lpvData);
            }
         }
         return retOrigGetDeviceState;
      }
      HRESULT WINAPI hkGetDeviceState_Mouse(HINSTANCE hInstance, DWORD cbData, LPVOID lpvData) {
         HRESULT retOrigGetDeviceState = origGetDeviceState_Mouse(hInstance, cbData, lpvData);

         if (!mGetDeviceStateExtensions[DI8Device::Mouse].empty()) {
            for (GetDeviceState_t mouseExtension : mGetDeviceStateExtensions[DI8Device::Mouse]) {
               if (mouseExtension)
                  mouseExtension(hInstance, cbData, lpvData);
            }
         }
         return retOrigGetDeviceState;
      }
   #pragma endregion

   #pragma region helpers
      HRESULT AddExtension(DI8Device deviceType, DI8Extension extensionType, LPVOID extensionAddress) {
         if (!isExtenderReady)
            return FALSE;

         switch (extensionType) {
            case DI8Extension::GetDeviceState:
            {
               switch (deviceType) {
                  case DI8Device::Keyboard:
                  case DI8Device::Mouse:
                     mGetDeviceStateExtensions[deviceType].push_back(reinterpret_cast<GetDeviceState_t>(extensionAddress));
                     break;
                  default:
                     return FALSE;
               }
               break;
            }
            default:
               return FALSE;
         }
         return TRUE;
      }
      LPDIRECTINPUT8A GetDirectInput8A() {
         if (!isExtenderReady)
            return nullptr;

         return di8Instance;
      }
      LPDIRECTINPUTDEVICE8A GetDirectInputDevice8A(DI8Device deviceType) {
         if (!isExtenderReady)
            return nullptr;

         switch (deviceType) {
            case DI8Device::Keyboard:
               return device_Keyboard;
            case DI8Device::Mouse:
               return device_Mouse;
         }
         return nullptr;
      }
      bool IsReady() {
         return isExtenderReady;
      }
   #pragma endregion

      BOOL CALLBACK enumCallback(LPCDIDEVICEINSTANCEA lpddi, LPVOID) {
         BYTE deviceType = LOBYTE(lpddi->dwDevType);
         if (deviceType == DI8DEVTYPE_KEYBOARD || deviceType == DI8DEVTYPE_MOUSE) {
            DWORD* inputTable;

            if (deviceType == DI8DEVTYPE_KEYBOARD) {
               di8Instance->CreateDevice(lpddi->guidInstance, &device_Keyboard, NULL);
               inputTable = *(PDWORD*)device_Keyboard;
            } else {
               di8Instance->CreateDevice(lpddi->guidInstance, &device_Mouse, NULL);
               inputTable = *(PDWORD*)device_Mouse;
            }

            Memory::openMemoryAccess(inputTable[9], 4);

            if (deviceType == DI8DEVTYPE_KEYBOARD) {
               origGetDeviceState_Keyboard = (GetDeviceState_t)(DWORD)inputTable[9];
               inputTable[9]               = (DWORD)hkGetDeviceState_Keyboard;
            } else {
               origGetDeviceState_Mouse    = (GetDeviceState_t)(DWORD)inputTable[9];
               inputTable[9]               = (DWORD)hkGetDeviceState_Mouse;
            }

            Memory::restoreMemoryAccess();
         }

         if (device_Keyboard && device_Mouse) {
            isExtenderReady = true;
            return DIENUM_STOP;
         }
         return DIENUM_CONTINUE;
      }

      void __stdcall hkInit(LPDIRECTINPUT8A addr) {
         di8Instance = addr;
         di8Instance->EnumDevices(DI8DEVCLASS_ALL, &enumCallback, NULL, DIEDFL_ATTACHEDONLY);
      }
      DWORD jmpBack = 0x597EC8;
      void __declspec(naked) hkGetDInput8Address() {
         __asm {
            call DirectInput8Create
            test eax, eax
            jne L_SkipInit
            mov eax, [esi + 0xB4]
            mov[pDInput8Address], eax
            push eax
            call hkInit
            L_SkipInit :
            jmp[jmpBack]
         }
      }
      void Init() {
         mGetDeviceStateExtensions[DI8Device::Keyboard] = std::vector<GetDeviceState_t>();
         mGetDeviceStateExtensions[DI8Device::Mouse]    = std::vector<GetDeviceState_t>();

         jmpBack += Memory::baseAddress;
         Memory::writeJMP(0x597EC3, (DWORD)&hkGetDInput8Address, false);
      }
   }

   bool isInit = false;
   bool Init() {
      D3D9::Init();
      DI8::Init();

      isInit = true;
      return true;
   }

   bool IsReady() {
      return isInit;
   }
   bool Prepare() {
      if (!isInit && !D3D9::isExtenderReady && !DI8::isExtenderReady) {
         D3D9::d3dDeviceAddress = Memory::makeAbsolute(0x9064A8);
         return Init();
      }
      return false;
   }
}