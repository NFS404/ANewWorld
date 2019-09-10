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
#include "Dynamics.h"
#include "UMath.h"

namespace GameInternals {
   namespace Data::GameTypes {
      namespace WCollisionMgr {
         struct WorldCollisionInfo
         {
            UMath::Vector4 fCollidePt;
            UMath::Vector4 fNormal;
            float fDist;
            bool fAnimated;
            bool fType;
            uint16_t fPad;
            void* fCInst;
            Dynamics::Collision::Geometry* pWorldGeom;
            Dynamics::Collision::Geometry* pUserGeom;
            UTL::COM::IUnknown* pUnk;
            unsigned char __PADDING__[8];
         };

         struct ICollisionHandler {
            virtual bool OnWCollide(WCollisionMgr::WorldCollisionInfo*, UMath::Vector3*, void*) = 0;
         };
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;