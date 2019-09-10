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
#include <stdint.h>
#include "UMath.h"

namespace GameInternals {
   namespace Data::GameTypes {
      namespace Dynamics {
         namespace Collision {
            struct Geometry {
               UMath::Vector4 mPosition;
               UMath::Vector4 mNormal[3];
               UMath::Vector4 mExtent[3];
               UMath::Vector4 mCollision_point;
               UMath::Vector4 mCollision_normal;
               float          mDimension[3];
               uint16_t       mShape;
               uint16_t       mPenetratesOther;
               UMath::Vector3 mDelta;
               float          mOverlap;
            };
         }

         struct IEntity {
            virtual UMath::Vector3* GetPosition()                               = 0;
            virtual void SetPosition(UMath::Vector3* newPosition)               = 0;
            virtual UMath::Vector3* GetAngularVelocity()                        = 0;
            virtual void SetAngularVelocity(UMath::Vector3* newAngularVelocity) = 0;
            virtual UMath::Vector3* GetLinearVelocity()                         = 0;
            virtual void SetLinearVelocity(UMath::Vector3* newLinearVelocity)   = 0;
            virtual UMath::Matrix4* GetRotation()                               = 0;
            virtual void SetRotation(UMath::Matrix4* newRotation)               = 0;
            virtual UMath::Vector4* GetOrientation()                            = 0;
            virtual void SetOrientation(UMath::Vector4* newOrientation)         = 0;
            virtual UMath::Vector3* GetPrincipalInertia()                       = 0;
            virtual float GetMass()                                             = 0;
            virtual UMath::Vector3* GetCenterOfGravity()                        = 0;
            virtual bool IsImmobile()                                           = 0;
         };
      }
   }
} namespace GameTypes = GameInternals::Data::GameTypes;