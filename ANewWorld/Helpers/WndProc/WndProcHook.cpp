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

#include "stdafx.h"
#include "WndProcHook.h"
#include "Helpers\LocalMirrorHook\LocalMirrorHook.hpp"

namespace Helpers {
   namespace WndProcHook {
      HWND                 windowHandle = nullptr;
      WNDPROC              origWndProc  = nullptr;
      std::vector<WNDPROC> extensions   = std::vector<WNDPROC>();

      void addExtension(LPVOID extensionAddress) {
         Log(LogLevel::Debug, Logger::FormatString("Adding WndProc extension from %p.", extensionAddress));
         extensions.push_back(reinterpret_cast<WNDPROC>(extensionAddress));
      }

      LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
         if (!extensions.empty()) {
            static LRESULT stickyRetVal = WndProcHook_IgnoreExtension;
            for (WNDPROC wndProcExtender : extensions) {
               static LRESULT retVal;
               if (retVal = wndProcExtender(hWnd, uMsg, wParam, lParam) != WndProcHook_IgnoreExtension)
                  stickyRetVal = retVal;
            }
            if (stickyRetVal != WndProcHook_IgnoreExtension) {
               LRESULT ret = stickyRetVal;
               stickyRetVal = WndProcHook_IgnoreExtension;
               return ret;
            }

            return CallWindowProcA(origWndProc, hWnd, uMsg, wParam, lParam);
         }
      }

      void Init() {
         Log(LogLevel::Debug, "Waiting for the window handle...");
         while (!windowHandle) {
            windowHandle = LocalMirrorHook::D3D9::GetWindowHandle();
            Sleep(1000);
         }
         Log(LogLevel::Info, "Installing WndProc hook.");
         origWndProc = (WNDPROC)SetWindowLongPtrA(windowHandle, GWL_WNDPROC, (LONG_PTR)&hkWndProc);
      }
   }
}