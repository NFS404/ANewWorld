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
#include <d3d9.h>
#include <dinput.h>

namespace LocalMirrorHook {
   namespace D3D9 {
      enum class D3D9Extension {
         BeginScene,
         EndScene,
         BeforeReset,
         AfterReset
      };

      HRESULT AddExtension(D3D9Extension extensionType, LPVOID extensionAddress);
      HWND GetWindowHandle();
      LPDIRECT3DDEVICE9 GetD3D9Device();
      bool IsReady();      
      void Init();
   }
   namespace DI8 {
      enum class DI8Device {
         Keyboard,
         Mouse
      };
      enum class DI8Extension {
         GetDeviceState
      };
      
      HRESULT AddExtension(DI8Device deviceType, DI8Extension extensionType, LPVOID extensionAddress);
      LPDIRECTINPUT8A GetDirectInput8A();
      LPDIRECTINPUTDEVICE8A GetDirectInputDevice8A(DI8Device deviceType);
      bool IsReady();
   }
   
   bool Prepare();
   bool IsReady();
}