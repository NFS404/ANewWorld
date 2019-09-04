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
#include <stdint.h>
#include "Helpers\Memory\Memory.h"

namespace GameInternals {
   namespace Data::GameTypes {
      namespace PPMalloc {
         enum class AllocType : uint32_t {
            CodeExec = 0x2,
            External = 0xA
         };
         static void* alloc(AllocType type, uint32_t size) {
            // name and args unknown
            return ((void*(__cdecl*)(uint32_t, int, AllocType, int, int, int))Memory::makeAbsolute(0x21C2A0))
               (size, NULL, type, 1, NULL, NULL);
         }
      }
   }
} namespace GameTypes = GameInternals::Data::GameTypes;