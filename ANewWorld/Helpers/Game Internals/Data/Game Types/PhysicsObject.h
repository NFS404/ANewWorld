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
#include "IAttachable.h"
#include "IBody.h"
#include "ISimable.h"
#include "Sim.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct PhysicsObject : Sim::Object, ISimable, IBody, IAttachable {
         virtual ~PhysicsObject();
         virtual void sub_704DE0();
         virtual void OnTaskSimulate(float) = 0;
#pragma region Sim::ITaskable overrides
         virtual bool OnTask(HSIMTASK__*, float) override;
#pragma endregion
#pragma region ISimable overrides
         virtual SimableType              GetSimableType()                                         override;
         virtual void                     Kill()                                                   override;
         virtual void                     Attach(UTL::COM::IUnknown* p)                            override;
         virtual void                     Detach(UTL::COM::IUnknown* p)                            override;
         virtual LPVOID                   GetAttachments()                                         override;
         virtual void                     AttachEntity(Sim::IEntity* pEntity)                      override;
         virtual void                     DetachEntity()                                           override;
         virtual IPlayer*                 GetPlayer()                                              override;
         virtual bool                     IsPlayer()                                               override;
         virtual bool                     IsOwnedByPlayer()                                        override;
         virtual Sim::IEntity*            GetEntity()                                              override;
         virtual void                     DebugObject()                                            override;
         virtual HSIMABLE__*              GetOwnerHandle()                                         override;
         virtual ISimable*                GetOwner()                                               override;
         virtual bool                     IsOwnedBy(ISimable* pISimable)                           override;
         virtual void                     SetOwnerObject(ISimable* pISimable)                      override;
         virtual Attrib::Instance*        GetAttributes()                                          override;
         virtual WWorldPos*               GetWPos()                                                override; // const
         virtual WWorldPos*               _dupGetWPos()                                            override;
         virtual IRigidBody*              GetRigidBody()                                           override; // const
         virtual IRigidBody*              _dupGetRigidBody()                                       override;
         virtual bool                     IsRigidBodySimple()                                      override;
         virtual bool                     IsRigidBodyComplex()                                     override;
         virtual UMath::Vector3*          GetPosition()                                            override;
         virtual void                     GetTransform(UMath::Matrix4* pOutTransform)              override;
         virtual void                     GetLinearVelocity(UMath::Vector3* pOutLinearVelocity)    override;
         virtual void                     GetAngularVelocity(UMath::Vector3* pOutAngularVelocity)  override;
         virtual uint32_t                 GetWorldID()                                             override;
         virtual EventSequencer::IEngine* GetEventSequencer()                                      override;
         virtual void                     ProcessStimulus(uint32_t)                                override;
         virtual IModel*                  GetModel()                                               override; // const
         virtual IModel*                  _dupGetModel()                                           override;
         virtual void                     SetCausality(HCAUSE__* p, float)                         override;
         virtual HCAUSE__*                GetCausality()                                           override;
         virtual float                    GetCausalityTime()                                       override;
#pragma endregion
#pragma region IBody overrides
         //virtual void     GetTransform(UMath::Matrix4* pOutTransform)             override;
         //virtual void     GetLinearVelocity(UMath::Vector3* pOutLinearVelocity)   override;
         //virtual void     GetAngularVelocity(UMath::Vector3* pOutAngularVelocity) override;
         virtual void     GetDimension(UMath::Vector3* pOutDimension)             override;
         //virtual uint32_t GetWorldID()                                            override;
#pragma endregion
#pragma region IAttachable overrides
         //virtual void  Attach(UTL::COM::IUnknown* p)                  = 0;
         //virtual void  Detach(UTL::COM::IUnknown* p)                  = 0;
         virtual bool  IsAttached(UTL::COM::IUnknown* p) override;
         virtual void  OnAttached(IAttachable* p)        override;
         virtual void  OnDetached(IAttachable* p)        override;
         //virtual void* GetAttachments()                               = 0;
#pragma endregion
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;