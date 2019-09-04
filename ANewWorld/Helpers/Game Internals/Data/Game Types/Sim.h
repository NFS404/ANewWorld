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
#include "Attrib.h"
#include "HSIMPROFILE__.h"
#include "HSIMSERVICE__.h"
#include "HSIMTASK__.h"
#include "UTL.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct Sim {
#pragma region Forward declarations
         struct IEntity;
         struct IServiceable;
         struct ITaskable;
         struct Object;
         struct Packet;
#pragma endregion
         struct IEntity {};
         struct IServiceable {
            virtual bool OnService(HSIMSERVICE__*, Packet*) = 0;
         };
         struct ITaskable {
            virtual bool OnTask(HSIMTASK__*, float) = 0;
            unsigned char __unk[0x1C];
         };

         struct Packet {
            virtual UCrc32   ConnectionClass();
            virtual uint32_t Compress(Packet*);
            virtual uint32_t Decompress(Packet*);
            virtual uint32_t Type();
            virtual uint32_t Size();
         };
         struct Object : UTL::COM::Object, IServiceable, ITaskable {
            virtual bool OnService(HSIMSERVICE__*, Packet*) override; // return 0;
            virtual ~Object();
         };
      };
      union SimSurface {
         unsigned char kNull[0x10];
         Attrib::Class* mClass;
         Attrib::Collection* mUnknown;
         Attrib::Collection* mNullSpec;
         Attrib::Collection* mOverride;
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;