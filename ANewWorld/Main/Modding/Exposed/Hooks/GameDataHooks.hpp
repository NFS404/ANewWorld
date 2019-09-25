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
#ifndef __ANEWWORLD_INCLUDED__
#include "..\ANewWorld.hpp"
#endif

namespace ANewWorld::Modding {
   namespace GameDataHooks {
      typedef bool(__stdcall* fnInstallFileHookFromMemoryA)(LPCSTR fileName, LPCVOID newFileContent, size_t newFileContentSize);
      typedef bool(__stdcall* fnInstallFileHookFromMemoryW)(LPCWSTR fileName, LPCVOID newFileContent, size_t newFileContentSize);

      /// <summary>
      /// Redirect game request file to a pointer in the memory.
      /// </summary>
      /// <param name="fileName">Target file name to redirect. Can be a relative path, case-insensitive.</param>
      /// <param name="newFileContent">Pointer to the new file data. The content is copied in the call, the memory can be freed by the caller afterwards.</param>
      /// <param name="newFileContentSize">Length of <paramref name="newFileContent"/></param>
      /// <returns>Whether installing the hook was successful.</returns>
      ANEWWORLD_API bool InstallFileHookFromMemoryA(LPCSTR fileName, LPCVOID newFileContent, size_t newFileContentSize) {
         try {
            if (HMODULE hANW = GetANewWorldHandle()) {
               if (FARPROC pFn = GetANewWorldFunction(hANW, "Modding::Hooks::HookCreateFile::InstallFileHookFromMemoryA")) {
                  return reinterpret_cast<fnInstallFileHookFromMemoryA>(pFn)(fileName, newFileContent, newFileContentSize);
               }
            }
         } catch (...) {
            std::rethrow_exception(std::current_exception());
         }
      }

      /// <summary>
      /// Redirect game request file to a pointer in the memory.
      /// </summary>
      /// <param name="fileName">Target file name to redirect. Can be a relative path, case-insensitive.</param>
      /// <param name="newFileContent">Pointer to the new file data. The content is copied in the call, the memory can be freed by the caller afterwards.</param>
      /// <param name="newFileContentSize">Length of <paramref name="newFileContent"/></param>
      /// <returns>Whether installing the hook was successful.</returns>
      ANEWWORLD_API bool InstallFileHookFromMemoryW(LPCWSTR fileName, LPCVOID newFileContent, size_t newFileContentSize) {
         try {
            if (HMODULE hANW = GetANewWorldHandle()) {
               if (FARPROC pFn = GetANewWorldFunction(hANW, "Modding::Hooks::HookCreateFile::InstallFileHookFromMemoryW")) {
                  return reinterpret_cast<fnInstallFileHookFromMemoryW>(pFn)(fileName, newFileContent, newFileContentSize);
               }
            }
         } catch (...) {
            std::rethrow_exception(std::current_exception());
         }
      }
   }
}
