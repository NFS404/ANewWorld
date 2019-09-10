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
#include <WinDef.h>
#include "Helpers\Memory\Memory.h"
#include "GameTypes.h"

namespace GameInternals {
   namespace Data::GameFunctions {
      namespace Server {
         static LPCSTR* GetBaseUrl() {
            return ((LPCSTR*(__cdecl*)())Memory::makeAbsolute(0x28BBD0))();
         }
         static LPCSTR* GetBaseRawUrl() {
            return ((LPCSTR*(__cdecl*)())Memory::makeAbsolute(0x28BC20))();
         }

         static HRESULT SendDeleteRequest(LPCSTR restUri, const GameTypes::Server::Callback* pCallback = nullptr) {
            LPVOID pInstance = *(LPVOID**)Memory::makeAbsolute(0x8ECE58);
            return ((HRESULT(__thiscall*)(LPVOID, LPCSTR, LPCVOID))Memory::makeAbsolute(0x13F780))(pInstance, restUri, pCallback);
         }
         static HRESULT SendGetRequest(LPCSTR restUri, const GameTypes::Server::Callback* pCallback = nullptr) {
            LPVOID pInstance = *(LPVOID**)Memory::makeAbsolute(0x8ECE58);
            return ((HRESULT(__thiscall*)(LPVOID, LPCSTR, LPCVOID))Memory::makeAbsolute(0x13F6A0))(pInstance, restUri, pCallback);
         }
         static HRESULT SendPostRequest(LPCSTR restUri, LPCSTR* body, const GameTypes::Server::Callback* pCallback = nullptr) {
            LPVOID pInstance = *(LPVOID**)Memory::makeAbsolute(0x8ECE58);
            return ((HRESULT(__thiscall*)(LPVOID, LPCSTR, LPCSTR*, LPCVOID))Memory::makeAbsolute(0x13F860))(pInstance, restUri, body, pCallback);
         }
         static HRESULT SendPutRequest(LPCSTR restUri, LPCSTR* body, const GameTypes::Server::Callback* pCallback = nullptr) {
            LPVOID pInstance = *(LPVOID**)Memory::makeAbsolute(0x8ECE58);
            return ((HRESULT(__thiscall*)(LPVOID, LPCSTR, LPCSTR*, LPCVOID))Memory::makeAbsolute(0x13F950))(pInstance, restUri, body, pCallback);
         }

         static LPCSTR* UrlEncode(LPCSTR source) {
            return ((LPCSTR*(__cdecl*)(LPCSTR))Memory::makeAbsolute(0x28BC70))(source);
         }
      }
   }
} namespace GameFunctions = GameInternals::Data::GameFunctions;