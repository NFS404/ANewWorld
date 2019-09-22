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
#include "D3D9Types.h"
#include "..\VTableHook.hpp"
#include <memory>
using std::unique_ptr;
using std::make_unique;

namespace LocalMirrorHook {
   enum class D3D9Extension : uint32_t {
      BeginScene  = 0,
      EndScene    = 1,
      BeforeReset = 2,
      AfterReset  = 3
   };

   namespace D3D9 {
      inline DWORD             d3dDeviceAddress               = NULL;
      inline LPDIRECT3DDEVICE9 d3dDevice                      = nullptr;
      inline HWND              d3dWindow                      = nullptr;

      inline std::vector<BeginScene_t> vBeginSceneExtensions  ={};
      inline std::vector<EndScene_t>   vEndSceneExtensions    ={};
      inline std::vector<Reset_t>      vBeforeResetExtensions ={};
      inline std::vector<Reset_t>      vAfterResetExtensions  ={};

      inline bool isExtenderReady                             = false;

   #pragma region function hooks
      static unique_ptr<VTableHook> d3dDeviceHook       = nullptr;
      static BeginScene_t           origBeginScene      = nullptr;
      static EndScene_t             origEndScene        = nullptr;
      static Reset_t                origReset           = nullptr;
      static BeginStateBlock_t      origBeginStateBlock = nullptr;

      static HRESULT WINAPI hkBeginScene(LPDIRECT3DDEVICE9 pDevice) {
         if (pDevice->TestCooperativeLevel() == D3D_OK) {
            for (BeginScene_t beginSceneExtension : vBeginSceneExtensions) {
               if (beginSceneExtension)
                  beginSceneExtension(pDevice);
            }
         }
         return origBeginScene(pDevice);
      }
      static HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
         if (pDevice->TestCooperativeLevel() == D3D_OK) {
            for (EndScene_t endSceneExtension : vEndSceneExtensions) {
               if (endSceneExtension)
                  endSceneExtension(pDevice);
            }
         }
         return origEndScene(pDevice);
      }
      static HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
         for (Reset_t beforeResetExtension : vBeforeResetExtensions) {
            if (beforeResetExtension)
               beforeResetExtension(pDevice, pPresentationParameters);
         }

         auto retOrigReset = origReset(pDevice, pPresentationParameters);
         for (Reset_t afterResetExtension : vAfterResetExtensions) {
            if (afterResetExtension)
               afterResetExtension(pDevice, pPresentationParameters);
         }

         return retOrigReset;
      }
      static HRESULT WINAPI hkBeginStateBlock(LPDIRECT3DDEVICE9 pDevice) {
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
      static HRESULT AddExtension(D3D9Extension extensionType, LPVOID extensionAddress) {
         Log(LogLevel::Debug, Logger::FormatString("Adding D3D9 extension (type %u) from %p.", extensionType, extensionAddress));
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
      static LPDIRECT3DDEVICE9 GetD3D9Device() {
         if (!isExtenderReady)
            return nullptr;

         return d3dDevice;
      }
      static HWND GetWindowHandle() {
         if (!isExtenderReady)
            return nullptr;

         return d3dWindow;
      }
      static bool IsReady() {
         return isExtenderReady;
      }
   #pragma endregion

      static void Init() {
         Log(LogLevel::Info, "Setting up D3D9 hook.");
         d3dDeviceAddress = Memory::makeAbsolute(0x9064A8);

         DWORD pD3DDevice = NULL;
         while (!pD3DDevice) {
            pD3DDevice = *(DWORD*)d3dDeviceAddress;
            Sleep(1000);
         }
         Log(LogLevel::Debug, Logger::FormatString("Found the D3D9 device at %p.", pD3DDevice));
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
}