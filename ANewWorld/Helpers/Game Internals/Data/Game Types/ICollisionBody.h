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
#include "UMath.h"
#include "UCrc32.h"
#include "UTL.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct ICollisionBody : UTL::COM::IUnknown {
         virtual ~ICollisionBody();
         virtual UMath::Vector3* GetPosition()                                           = 0;
         virtual UMath::Vector3* GetLinearVelocity()                                     = 0;
         virtual UMath::Vector3* GetAngularVelocity()                                    = 0;
         virtual void            GetDimension(UMath::Vector3* pOutVector3)               = 0;
         virtual void            Damp(float amount)                                      = 0;
         virtual void            SetAnimating(bool isAnimating)                          = 0;
         virtual bool            IsModeling()                                            = 0;
         virtual void            DisableModeling()                                       = 0;
         virtual void            EnableModeling()                                        = 0;
         virtual bool            IsTriggering()                                          = 0;
         virtual void            DisableTriggering()                                     = 0;
         virtual void            EnableTriggering()                                      = 0;
         virtual bool            IsSleeping()                                            = 0;
         virtual void            SetCenterOfGravity(UMath::Vector3* pNewCenterOfGravity) = 0;
         virtual UMath::Vector3* GetCenterOfGravity()                                    = 0;
         virtual bool            HasHadCollision()                                       = 0;
         virtual bool            HasHadWorldCollision()                                  = 0;
         virtual bool            HasHadObjectCollision()                                 = 0;
         virtual void            EnableCollisionGeometries(UCrc32 crc, bool _switch)     = 0;
         virtual bool            DistributeMass()                                        = 0;
         virtual UMath::Vector3* GetWorldMomentScale()                                   = 0;
         virtual UMath::Vector3* GetGroundMomentScale()                                  = 0;
         virtual bool            IsAttachedToWorld()                                     = 0;
         virtual void            AttachedToWorld(bool, float)                            = 0;
         virtual bool            IsAnchored()                                            = 0;
         virtual void            SetAnchored(bool isAnchored)                            = 0;
         virtual void            SetInertiaTensor(UMath::Vector3* pNewInertiaTensor)     = 0;
         virtual UMath::Vector3* GetInertiaTensor()                                      = 0;
         virtual float           GetOrientToGround()                                     = 0;
         virtual bool            IsInGroundContact()                                     = 0;
         virtual int32_t         GetNumContactPoints()                                   = 0;
         virtual UMath::Vector4  GetGroundNormal()                                       = 0;
         virtual void            SetForce(UMath::Vector3* pNewForce)                     = 0;
         virtual void            SetTorque(UMath::Vector3* pNewTorque)                   = 0;
         virtual UMath::Vector3* GetForce()                                              = 0;
         virtual UMath::Vector3* GetTorque()                                             = 0;
         virtual float           GetGravity()                                            = 0;
         virtual UMath::Vector3* GetForwardVector()                                      = 0;
         virtual UMath::Vector3* GetRightVector()                                        = 0;
         virtual UMath::Vector3* GetUpVector()                                           = 0;
         virtual UMath::Matrix4* GetMatrix4()                                            = 0;
         virtual DWORD           SetWorldCollisionMask(DWORD mask)                       = 0;
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;