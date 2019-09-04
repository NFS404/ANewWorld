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
#include "IAttributeable.h"
#include "IExplodeable.h"
#include "IVehicle.h"
#include "PhysicsObject.h"
#include "VehicleParams.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct __off_BC3D1C {
         virtual ~__off_BC3D1C();
         virtual LPVOID __GetAttributes();
         DWORD _unk;
      };
      struct __off_BC3CF0 {
         virtual void __sub_703C30();
         virtual void __sub_703990();
         virtual void __sub_77CA30();
         virtual void __sub_7789E0();
         virtual void __sub_778A10();
         virtual void __sub_778A90();
         virtual void __sub_7775E0();
         virtual void __sub_777680();
         virtual void __sub_777620();
         virtual void __sub_778B30();
         virtual void __sub_6F77F0();
         unsigned char __unk[0x78];
      };
      // sizeof(PVehicle) should be 0x1FC
      struct PVehicle : PhysicsObject, __off_BC3D1C, __off_BC3CF0, IVehicle, EventSequencer::IContext, IExplodeable, IAttributeable {
         virtual ~PVehicle();
         virtual void OnDebugDraw();
#pragma region PhysicsObject overrides
#pragma region Sim::ITaskable overrides
         virtual bool OnTask(HSIMTASK__*, float) override;
#pragma endregion
#pragma region ISimable overrides
         virtual void            Kill()         override;
         virtual void            DebugObject()  override;
         virtual UMath::Vector3* GetPosition()  override;
         virtual IModel*         GetModel()     override; // const
         virtual IModel*         _dupGetModel() override;
#pragma endregion
#pragma region IVehicle overrides
         virtual ISimable*              GetSimable()                                                           override;
         virtual ISimable*              _dupGetSimable()                                                       override;
         //virtual UMath::Vector3*        GetPosition()                                                          override;
         virtual void                   SetBehaviorOverride(UCrc32, UCrc32)                                    override;
         virtual void                   RemoveBehaviorOverride(UCrc32)                                         override;
         virtual void                   CommitBehaviorOverrides()                                              override;
         virtual void                   __unkFunc1()                                                           override;
         virtual void                   ReleaseBehaviorAudio()                                                 override;
         virtual void                   SetStaging(bool isStaging)                                             override;
         virtual bool                   IsStaging()                                                            override;
         virtual void                  __unkFunc2()                                                            override;
         virtual void                   SetDriverStyle(DriverStyle newDriverStyle)                             override;
         virtual DriverStyle            GetDriverStyle()                                                       override;
         virtual void                   SetPhysicsMode(PhysicsMode newPhysicsMode)                             override;
         virtual PhysicsMode            GetPhysicsMode()                                                       override;
         virtual void                   __unkFunc3()                                                           override;
         virtual void                   __unkFunc4()                                                           override;
         virtual void                   SetDraftZoneModifier(float newDraftZoneMultiplier)                     override;
         virtual float                  GetDraftZoneModifier()                                                 override;
         virtual uint32_t               GetModelType()                                                         override;
         virtual bool                   IsSpooled()                                                            override;
         virtual bool                   IsLowRez()                                                             override;
         virtual void                   __unkFunc5()                                                           override;
         virtual UCrc32*                GetVehicleClass()                                                      override;
         virtual void*                  GetVehicleAttributes()                                                 override;
         virtual const char*            GetVehicleName()                                                       override;
         virtual DWORD                  GetVehicleKey()                                                        override;
         virtual DWORD                  GetStockVehicleKey()                                                   override;
         virtual void                   SetDriverClass(DriverClass newDriverClass)                             override;
         virtual DriverClass            GetDriverClass()                                                       override;
         virtual bool                   IsLoading()                                                            override;
         virtual float                  GetOffscreenTime()                                                     override;
         virtual float                  GetOnScreenTime()                                                      override;
         virtual bool                   SetVehicleOnGround(UMath::Vector3*, UMath::Vector3*)                   override;
         virtual void                   ForceStopOn(ForceStopType type)                                        override;
         virtual void                   ForceStopOff(ForceStopType type)                                       override;
         virtual ForceStopType          GetForceStop()                                                         override;
         virtual void                   __unkFunc6()                                                           override;
         virtual void                   __unkFunc7()                                                           override;
         virtual bool                   InShock()                                                              override;
         virtual bool                   IsDestroyed()                                                          override;
         virtual void                   Activate()                                                             override;
         virtual void                   Deactivate()                                                           override;
         virtual bool                   IsActive()                                                             override;
         virtual float                  GetSpeedometer()                                                       override;
         virtual float                  GetSpeed()                                                             override;
         virtual void                   SetSpeed(float newSpeed)                                               override;
         virtual float                  GetAbsoluteSpeed()                                                     override;
         virtual bool                   IsGlareOn(VehicleFX::ID fxId)                                          override;
         virtual void                   GlareOn(VehicleFX::ID fxId)                                            override;
         virtual void                   GlareOff(VehicleFX::ID fxId)                                           override;
         virtual bool                   IsCollidingWithSoftBarrier()                                           override;
         virtual IVehicleAI*            GetAIVehiclePtr()                                                      override;
         virtual float                  GetSlipAngle()                                                         override;
         virtual UMath::Vector3*        GetLocalVelocity()                                                     override;
         virtual void                   __unkFunc8()                                                           override;
         virtual void                   __unkFunc9()                                                           override;
         virtual void                   __unkFunc10()                                                          override;
         virtual void                   __unkFunc11()                                                          override;
         virtual void                   ComputeHeading(UMath::Vector3* pOutHeading)                            override;
         virtual bool                   IsAnimating()                                                          override;
         virtual void                   SetAnimating(bool isAnimating)                                         override;
         virtual bool                   IsOffWorld()                                                           override;
         virtual FECustomizationRecord* GetCustomizations()                                                    override;
         virtual FECustomizationRecord* _dupGetCustomizations()                                                override;
         virtual void                   __unkFunc12()                                                          override;
         virtual void                   __unkFunc13()                                                          override;
         virtual void                   __unkFunc14()                                                          override;
         virtual void                   __unkFunc15()                                                          override;
         virtual void                   __unkFunc16()                                                          override;
         virtual void                   __unkFunc17()                                                          override;
         virtual void                   __unkFunc18()                                                          override;
         virtual void                   __unkFunc19()                                                          override;
         virtual void                   __unkFunc20()                                                          override;
         virtual void                   __unkFunc21()                                                          override;
         virtual void                   __unkFunc22()                                                          override;
         virtual void                   __unkFunc23()                                                          override;
         virtual void                   __unkFunc24()                                                          override;
         virtual void                   __unkFunc25()                                                          override;
         virtual void                   __unkFunc26()                                                          override;
         virtual bool                   GetDynamicData(EventSequencer::System*, void* EventDynamicData)        override;
         virtual void                   __unkFunc27()                                                          override;
         virtual void                   _dup__unkFunc27()                                                      override;
#pragma endregion
#pragma region EventSequencer::IContext overrides
         virtual void*   GetContextOwner()                                                   override;
         //virtual bool    GetDynamicData(EventSequencer::System*, void* pOutEventDynamicData) override;
         virtual int32_t GetContextDebugName(char*, uint32_t)                                override;
#pragma endregion
#pragma region IExplodeable overrides
         virtual void OnExplosion(UMath::Vector3*, UMath::Vector3*, float, void* IExplosion) override;
#pragma endregion
#pragma endregion

         static PVehicle* Create(DriverClass driverClass, DWORD carModelHash, FECustomizationRecord* pFECustomizationRecord,
                                 UMath::Vector3* pInitialRotation, UMath::Vector3* pInitialPosition) {
            if (!carModelHash)
               return nullptr;

            VehicleParams vehicleParams(driverClass, pInitialRotation, pInitialPosition, pFECustomizationRecord);
            if (vehicleParams.Init(carModelHash)) {
               // UTL::COM::Factory<Sim::Param,ISimable,UCrc32>::CreateInstance(UCrc32,Sim::Param)
               auto *pSimable = ((ISimable*(__cdecl*)(DWORD, DWORD, VehicleParams*))Memory::makeAbsolute(0x291F00))
                  (stringhash32("PVehicle"), vehicleParams.mTypeName, &vehicleParams);
               return pSimable ? (PVehicle*)((char*)pSimable - 0x24) : nullptr;
            }

			return nullptr;
         }
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;