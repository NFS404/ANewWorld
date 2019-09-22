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

namespace Modding::Hooks::HookCreateFile {
   namespace TemporaryHelper {
      // https://stackoverflow.com/a/3152296
      template<typename TChar>
      class pred_Equals {
         const std::locale& _loc;

      public:
         pred_Equals(const std::locale& loc) : _loc(loc) {}

         bool operator() (TChar ch1, TChar ch2) {
            return std::toupper(ch1, _loc) == std::toupper(ch2, _loc);
         }
      };

      template<typename T>
      int findSubstring(const T& str1, const T& str2, const std::locale& loc = std::locale()) {
         typename T::const_iterator it = std::search(str1.begin(), str1.end(),
                                                     str2.begin(), str2.end(),
                                                     pred_Equals<typename T::value_type>(loc)
         );

         if (it != str1.end())
            return it - str1.begin();
         return -1;
      }
   }

   struct RedirectedFileEntry {
      size_t  lenData = 0;
      LPCVOID pData = nullptr;

      RedirectedFileEntry(size_t _lenData, LPCVOID _pData) : lenData(_lenData), pData(_pData) {}

      HANDLE getPipe() const {
         if (pData == nullptr || lenData == 0) {
            Log(LogLevel::Error, "pData or lenData is NULL.");
            return nullptr;
         }

         HANDLE hReadPipe, hWritePipe;
         CreatePipe(&hReadPipe, &hWritePipe, nullptr, lenData);
         if (!hReadPipe) {
            Log(LogLevel::Error, "RedirectedFileEntry->getPipe(): Failed to create pipe!");
            return nullptr;
         }

         static DWORD dwBytesWritten;
         WriteFile(hWritePipe, pData, lenData, &dwBytesWritten, nullptr);
         CloseHandle(hWritePipe);

         return hReadPipe;
      }
   };

   static std::map<std::wstring, std::shared_ptr<RedirectedFileEntry>> mFileRedirects ={};
   void addHook(const std::wstring& name, const std::shared_ptr<RedirectedFileEntry>& entry) {
      mFileRedirects[name] = entry;
   }

   HANDLE WINAPI hkCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
      if (!mFileRedirects.empty()) {
         std::wstring wstrFileName(lpFileName);

         for (auto& entry : mFileRedirects) {
            if (TemporaryHelper::findSubstring(wstrFileName, entry.first) != -1) {
               Log(LogLevel::Debug, Logger::FormatString("mFileRedirects[%ws]->getPipe()\n\t", entry.first.c_str()));
               if (HANDLE hFileData = entry.second->getPipe())
                  return hFileData;

               Log(LogLevel::Error, "hFileData was null! Returning original file...");
               break;
            }
            std::replace(wstrFileName.begin(), wstrFileName.end(), '/', '\\');
            if (TemporaryHelper::findSubstring(wstrFileName, entry.first) != -1) {
               Log(LogLevel::Debug, Logger::FormatString("mFileRedirects[%ws]->getPipe() + edited file name\n\t", entry.first.c_str()));
               if (HANDLE hFileData = entry.second->getPipe())
                  return hFileData;

               Log(LogLevel::Error, "hFileData was null! Returning original file...");
               break;
            }
         }
      }

      return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
   }

   void installHook() {
      Log(LogLevel::Info, "Installing hooks.");
      // redirect CreateFileW
      {
         DWORD addCreateFileW = Memory::makeAbsolute(0x71D0FC);
         Memory::openMemoryAccess(addCreateFileW, 4);
         *(DWORD*)addCreateFileW = (DWORD)&hkCreateFileW;
         Memory::restoreMemoryAccess();
      }
   }
}
