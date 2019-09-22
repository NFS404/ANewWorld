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
#define __ANEWWORLD_INCLUDED__

#ifndef ANEWWORLD_API
#define ANEWWORLD_API
#endif

#include <Windows.h>
#include <stdexcept> // runtime_error
#include <exception> // throw_with_nested

namespace ANewWorld {
   /// <summary>
   /// Used internally.
   /// </summary>
   /// <returns>The handle of ANewWorld's live module, named "ANewWorld.WorldMod".</returns>
   ANEWWORLD_API HMODULE GetANewWorldHandle() {
      HMODULE hANW = nullptr;
      if (FAILED(hANW = GetModuleHandle(TEXT("ANewWorld.WorldMod"))))
         std::throw_with_nested(std::runtime_error("\"ANewWorld.WorldMod\" was not found in the current process! Call GetLastError to get more information."));
      return hANW;
   }

   /// <summary>
   /// Used internally.
   /// </summary>
   /// <param name="hANewWorld">Handle to ANewWorld's live module. Should not be NULL.</param>
   /// <param name="szFunctionName">Null-terminated function signature.</param>
   /// <returns>The address of the target function.</returns>
   ANEWWORLD_API FARPROC GetANewWorldFunction(HMODULE hANewWorld, const char* szFunctionName) {
      if (!hANewWorld)
         std::throw_with_nested(std::runtime_error("hANewWorld is NULL."));

      FARPROC pFunction = nullptr;
      if (FAILED(pFunction = GetProcAddress(hANewWorld, szFunctionName)))
         std::throw_with_nested(std::runtime_error("Error while retrieving ANewWorld function! Call GetLastError to get more information."));
      return pFunction;
   }
}
#endif