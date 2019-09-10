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
#include "_enums.h"
#include "UMath.h"
#include "UTL.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct ISimable;
      struct IRigidBody : UTL::COM::IUnknown {
         virtual ~IRigidBody();
         virtual ISimable*       GetOwner()                                                                = 0;
         virtual bool            IsSimple()                                                                = 0;
         virtual uint32_t        GetIndex()                                                                = 0;
         virtual SimableType     GetSimableType()                                                          = 0;
         virtual float           GetRadius()                                                               = 0;
         virtual float           GetMass()                                                                 = 0;
         virtual float           GetOOMass()                                                               = 0;
         virtual UMath::Vector3* GetPosition()                                                             = 0;
         virtual UMath::Vector3* GetLinearVelocity()                                                       = 0;
         virtual UMath::Vector3* GetAngularVelocity()                                                      = 0;
         virtual float           GetSpeed()                                                                = 0;
         virtual float           GetSpeedXZ()                                                              = 0;
         virtual void            GetForwardVector(UMath::Vector3 *)                                        = 0;
         virtual void            GetRightVector(UMath::Vector3 *)                                          = 0;
         virtual void            GetUpVector(UMath::Vector3 *)                                             = 0;
         virtual void            GetMatrix4(UMath::Matrix4 *)                                              = 0;
         virtual UMath::Vector4* GetOrientation()                                                          = 0;
         virtual void            GetDimension(UMath::Vector3 *)                                            = 0;
         virtual void            _dupGetDimension(UMath::Vector3 *)                                        = 0;
         virtual uint32_t        GetTriggerFlags()                                                         = 0;
         virtual void*           GetWCollider()                                                            = 0;
         virtual void            GetPointVelocity(UMath::Vector3* pInVector3, UMath::Vector3* pOutVector3) = 0;
         virtual void            SetPosition(UMath::Vector3*)                                              = 0;
         virtual void            SetLinearVelocity(UMath::Vector3*)                                        = 0;
         virtual void            SetAngularVelocity(UMath::Vector3*)                                       = 0;
         virtual void            SetRadius(float)                                                          = 0;
         virtual void            SetMass(float)                                                            = 0;
         virtual void            SetOrientation(UMath::Matrix4*)                                           = 0;
         virtual void            SetOrientation(UMath::Vector4*)                                           = 0;
         virtual void            ModifyXPos(float)                                                         = 0;
         virtual void            ModifyYPos(float)                                                         = 0;
         virtual void            ModifyZPos(float)                                                         = 0;
         virtual void            Resolve(UMath::Vector3*, UMath::Vector3*)                                 = 0;
         virtual void            ResolveForce(UMath::Vector3*, UMath::Vector3*)                            = 0;
         virtual void            ResolveForce(UMath::Vector3*)                                             = 0;
         virtual void            ResolveTorque(UMath::Vector3*, UMath::Vector3*)                           = 0;
         virtual void            ResolveTorque(UMath::Vector3*)                                            = 0;
         virtual void            PlaceObject(UMath::Matrix4*, UMath::Vector3*)                             = 0;
         virtual void            Accelerate(UMath::Vector3*, float)                                        = 0;
         virtual bool            IsTumbling()                                                              = 0;
         virtual void            ConvertLocalToWorld(UMath::Vector3*, bool)                                = 0;
         virtual void            ConvertWorldToLocal(UMath::Vector3*, bool)                                = 0;
         virtual void            Debug()                                                                   = 0;
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;