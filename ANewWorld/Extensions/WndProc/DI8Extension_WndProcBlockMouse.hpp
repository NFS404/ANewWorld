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
// Helpers
#include "Helpers\WndProc\WndProcHook.h"

namespace Extensions {
   namespace DI8 {
      namespace WndProcBlockMouse {
         static LRESULT CALLBACK wndProcExtension(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            switch (uMsg) {
               case WM_MOUSEMOVE: case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
               case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
               case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
               case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
                  if (!GameVariables::getVariable(GameVariables::gameWindowActive) || pImGuiIO->WantCaptureMouse)
                     return FALSE;
                  break;
            }
            return WndProcHook_IgnoreExtension;
         }

         static void Init() {
            Helpers::WndProcHook::addExtension(&wndProcExtension);
         }
      }
   }
}