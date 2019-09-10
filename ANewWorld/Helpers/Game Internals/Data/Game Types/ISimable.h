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
#include "_enums.h"
#include "Attrib.h"
#include "EventSequencer.h"
#include "HCAUSE__.h"
#include "HSIMABLE__.h"
#include "IModel.h"
#include "IPlayer.h"
#include "IRigidBody.h"
#include "Sim.h"
#include "UTL.h"
#include "WWorldPos.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct ISimable : UTL::COM::IUnknown {
         virtual ~ISimable();
         virtual SimableType              GetSimableType()                                         = 0;
         virtual void                     Kill()                                                   = 0;
         virtual void                     Attach(UTL::COM::IUnknown* p)                            = 0;
         virtual void                     Detach(UTL::COM::IUnknown* p)                            = 0;
         virtual LPVOID                   GetAttachments()                                         = 0;
         virtual void                     AttachEntity(Sim::IEntity* pEntity)                      = 0;
         virtual void                     DetachEntity()                                           = 0;
         virtual IPlayer*                 GetPlayer()                                              = 0;
         virtual bool                     IsPlayer()                                               = 0;
         virtual bool                     IsOwnedByPlayer()                                        = 0;
         virtual Sim::IEntity*            GetEntity()                                              = 0;
         virtual void                     DebugObject()                                            = 0;
         virtual HSIMABLE__*              GetOwnerHandle()                                         = 0;
         virtual ISimable*                GetOwner()                                               = 0;
         virtual bool                     IsOwnedBy(ISimable* pISimable)                           = 0;
         virtual void                     SetOwnerObject(ISimable* pISimable)                      = 0;
         virtual Attrib::Instance*        GetAttributes()                                          = 0;
         virtual WWorldPos*               GetWPos()                                                = 0; // const
         virtual WWorldPos*               _dupGetWPos()                                            = 0;
         virtual IRigidBody*              GetRigidBody()                                           = 0; // const
         virtual IRigidBody*              _dupGetRigidBody()                                       = 0;
         virtual bool                     IsRigidBodySimple()                                      = 0;
         virtual bool                     IsRigidBodyComplex()                                     = 0;
         virtual UMath::Vector3*          GetPosition()                                            = 0;
         virtual void                     GetTransform(UMath::Matrix4* pOutTransform)              = 0;
         virtual void                     GetLinearVelocity(UMath::Vector3* pOutLinearVelocity)    = 0;
         virtual void                     GetAngularVelocity(UMath::Vector3* pOutAngularVelocity)  = 0;
         virtual uint32_t                 GetWorldID()                                             = 0;
         virtual EventSequencer::IEngine* GetEventSequencer()                                      = 0;
         virtual void                     ProcessStimulus(uint32_t)                                = 0;
         virtual IModel*                  GetModel()                                               = 0; // const
         virtual IModel*                  _dupGetModel()                                           = 0;
         virtual void                     SetCausality(HCAUSE__* p, float)                         = 0;
         virtual HCAUSE__*                GetCausality()                                           = 0;
         virtual float                    GetCausalityTime()                                       = 0;
         unsigned char __unk[4]; // mHandle, UInstanceable
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;