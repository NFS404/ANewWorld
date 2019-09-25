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
#include <random>

namespace AntiCheat::EditWindowProperties {
   namespace TemporaryHelper {
      std::string generateRandomString(size_t length) {
         auto& chrs = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
         std::string str;
         str.reserve(length);
         {
            std::mt19937 rg{ std::random_device {}() };
            std::uniform_int_distribution<size_t> pick(0, sizeof(chrs) - 2);

            while (length--)
               str += chrs[pick(rg)];
         }
         return str;
      }

      void memOverrideStringA(DWORD strAddr, const char* newStr) {
         const size_t size = strlen(newStr);
         Memory::openMemoryAccess(strAddr, size * sizeof(char));
         strncpy((char*)strAddr, newStr, size);
         Memory::restoreMemoryAccess();
      }
      void memOverrideStringW(DWORD strAddr, const wchar_t* newStr) {
         const size_t size = wcslen(newStr);
         Memory::openMemoryAccess(strAddr, size * sizeof(wchar_t));
         wcsncpy((wchar_t*)strAddr, newStr, size);
         Memory::restoreMemoryAccess();
      }
   }

   void WINAPI editWindowProperties() {
      Log(LogLevel::Info, "Applying anti-cheat measures.");
      {
         Log(LogLevel::Debug, "Replacing class name with a random string");
         auto cstr = TemporaryHelper::generateRandomString(9);
         auto wstr = std::wstring(cstr.begin(), cstr.end());
         TemporaryHelper::memOverrideStringA(Memory::makeAbsolute(0x7B58CC), cstr.c_str());
         TemporaryHelper::memOverrideStringW(Memory::makeAbsolute(0x7B58A4), wstr.c_str());
      }
      {
         Log(LogLevel::Debug, "Appending ANewWorld string to window name.");
         wchar_t** pWWindowName = (wchar_t**)Memory::makeAbsolute(0x884E24);
         wchar_t* wExtraStr = new wchar_t[18]{ L" - ANewWorld v1.0" };
         wchar_t* wFinalName = new wchar_t[wcslen(*pWWindowName) + wcslen(wExtraStr)];
         wcscpy(wFinalName, *pWWindowName); wcscat(wFinalName, wExtraStr);
         delete[] wExtraStr;

         Memory::openMemoryAccess((DWORD)pWWindowName, sizeof(DWORD));
         *pWWindowName = wFinalName;
         Memory::restoreMemoryAccess();
      }
   }
}
