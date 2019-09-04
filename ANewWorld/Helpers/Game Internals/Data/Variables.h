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
#include "GameTypes.h"
#include "Helpers\Memory\Memory.h"

namespace GameInternals {
   namespace Data {
      namespace Variables {
         static BYTE*                                     gameWindowActive = (BYTE*)(0x9064FC);
         static int16_t*                                  nosFOVWidening   = (int16_t*)(0x8C5E04);
         static GameTypes::__array<GameTypes::ISimable*>* arrSimables      = (GameTypes::__array<GameTypes::ISimable*>*)(0x00CC6E14);

         template <typename T>
         inline T getVariable(T* pVariable) {
            return *(T*)Memory::makeAbsolute((DWORD)pVariable);
         }
         template <typename T>
         inline void setVariable(LPVOID pVariable, const T& newValue) {
            auto* pVar = (T*)Memory::makeAbsolute((DWORD)pVariable);
            Memory::openMemoryAccess((DWORD)pVar, sizeof(T));
            *(T*)pVar = newValue;
            Memory::restoreMemoryAccess();
         }
      }
   }
} namespace GameVariables = GameInternals::Data::Variables;