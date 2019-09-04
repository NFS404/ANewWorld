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
#include <WinBase.h>
#include <WinDef.h>
#include "_memory.h"
#include "Helpers\Memory\Memory.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct Server {
         typedef void(__cdecl* fnSuccessHandler_t)(DWORD _unk, LPCSTR strResponseContent);

         struct Callback {
            DWORD               base                = 0xB257D4;
            /* ------------------------------------------ */  unsigned char _empty1[0x0C]  ={ 0x00 };
            bool                hasFailedHandler    = false; // or maybe type of the handler
            fnSuccessHandler_t* fnSuccessHandler    = nullptr;
            DWORD               addrFailedHandler   = NULL;
            /* ------------------------------------------ */  unsigned char _empty2[0x04] ={ 0x00 };

            Callback() = default;
            static Callback* Create(fnSuccessHandler_t* pFuncSuccessHandler) {
               Callback* pCallback          = (Callback*)PPMalloc::alloc(PPMalloc::AllocType::External, 0x20);
               pCallback->base              = Memory::makeAbsolute(0x7257D4);
               pCallback->hasFailedHandler  = false;
               pCallback->fnSuccessHandler  = pFuncSuccessHandler;
               pCallback->addrFailedHandler = NULL;
               ZeroMemory(pCallback->_empty1, sizeof(pCallback->_empty1));
               ZeroMemory(pCallback->_empty2, sizeof(pCallback->_empty2));

               return pCallback;
            }
         };
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;