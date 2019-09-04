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
#include "EventSequencer.h"
#include "FECustomizationRecord.h"
#include "ISimable.h"
#include "IVehicleAI.h"
#include "UCrc32.h"
#include "UTL.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct IVehicle : UTL::COM::IUnknown {
         virtual ~IVehicle();
         virtual ISimable*              GetSimable()                                                           = 0;
         virtual ISimable*              _dupGetSimable()                                                       = 0;
         virtual UMath::Vector3*        GetPosition()                                                          = 0;
         virtual void                   SetBehaviorOverride(UCrc32, UCrc32)                                    = 0;
         virtual void                   RemoveBehaviorOverride(UCrc32)                                         = 0;
         virtual void                   CommitBehaviorOverrides()                                              = 0;
         virtual void                   __unkFunc1()                                                           = 0;
         virtual void                   ReleaseBehaviorAudio()                                                 = 0;
         virtual void                   SetStaging(bool isStaging)                                             = 0;
         virtual bool                   IsStaging()                                                            = 0;
         virtual void                  __unkFunc2()                                                            = 0;
         virtual void                   SetDriverStyle(DriverStyle newDriverStyle)                             = 0;
         virtual DriverStyle            GetDriverStyle()                                                       = 0;
         virtual void                   SetPhysicsMode(PhysicsMode newPhysicsMode)                             = 0;
         virtual PhysicsMode            GetPhysicsMode()                                                       = 0;
         virtual void                   __unkFunc3()                                                           = 0;
         virtual void                   __unkFunc4()                                                           = 0;
         virtual void                   SetDraftZoneModifier(float newDraftZoneMultiplier)                     = 0;
         virtual float                  GetDraftZoneModifier()                                                 = 0;
         virtual uint32_t               GetModelType()                                                         = 0;
         virtual bool                   IsSpooled()                                                            = 0;
         virtual bool                   IsLowRez()                                                             = 0;
         virtual void                   __unkFunc5()                                                           = 0;
         virtual UCrc32*                GetVehicleClass()                                                      = 0;
         virtual void*                  GetVehicleAttributes()                                                 = 0;
         virtual const char*            GetVehicleName()                                                       = 0;
         virtual DWORD                  GetVehicleKey()                                                        = 0;
         virtual DWORD                  GetStockVehicleKey()                                                   = 0;
         virtual void                   SetDriverClass(DriverClass newDriverClass)                             = 0;
         virtual DriverClass            GetDriverClass()                                                       = 0;
         virtual bool                   IsLoading()                                                            = 0;
         virtual float                  GetOffscreenTime()                                                     = 0;
         virtual float                  GetOnScreenTime()                                                      = 0;
         virtual bool                   SetVehicleOnGround(UMath::Vector3*, UMath::Vector3*)                   = 0;
         virtual void                   ForceStopOn(ForceStopType type)                                        = 0;
         virtual void                   ForceStopOff(ForceStopType type)                                       = 0;
         virtual ForceStopType          GetForceStop()                                                         = 0;
         virtual void                   __unkFunc6()                                                           = 0;
         virtual void                   __unkFunc7()                                                           = 0;
         virtual bool                   InShock()                                                              = 0;
         virtual bool                   IsDestroyed()                                                          = 0;
         virtual void                   Activate()                                                             = 0;
         virtual void                   Deactivate()                                                           = 0;
         virtual bool                   IsActive()                                                             = 0;
         virtual float                  GetSpeedometer()                                                       = 0;
         virtual float                  GetSpeed()                                                             = 0;
         virtual void                   SetSpeed(float newSpeed)                                               = 0;
         virtual float                  GetAbsoluteSpeed()                                                     = 0;
         virtual bool                   IsGlareOn(VehicleFX::ID fxId)                                          = 0;
         virtual void                   GlareOn(VehicleFX::ID fxId)                                            = 0;
         virtual void                   GlareOff(VehicleFX::ID fxId)                                           = 0;
         virtual bool                   IsCollidingWithSoftBarrier()                                           = 0;
         virtual IVehicleAI*            GetAIVehiclePtr()                                                      = 0;
         virtual float                  GetSlipAngle()                                                         = 0;
         virtual UMath::Vector3*        GetLocalVelocity()                                                     = 0;
         virtual void                   __unkFunc8()                                                           = 0;
         virtual void                   __unkFunc9()                                                           = 0;
         virtual void                   __unkFunc10()                                                          = 0;
         virtual void                   __unkFunc11()                                                          = 0;
         virtual void                   ComputeHeading(UMath::Vector3* pOutHeading)                            = 0;
         virtual bool                   IsAnimating()                                                          = 0;
         virtual void                   SetAnimating(bool isAnimating)                                         = 0;
         virtual bool                   IsOffWorld()                                                           = 0;
         virtual FECustomizationRecord* GetCustomizations()                                                    = 0;
         virtual FECustomizationRecord* _dupGetCustomizations()                                                = 0;
         virtual void                   __unkFunc12()                                                          = 0;
         virtual void                   __unkFunc13()                                                          = 0;
         virtual void                   __unkFunc14()                                                          = 0;
         virtual void                   __unkFunc15()                                                          = 0;
         virtual void                   __unkFunc16()                                                          = 0;
         virtual void                   __unkFunc17()                                                          = 0;
         virtual void                   __unkFunc18()                                                          = 0;
         virtual void                   __unkFunc19()                                                          = 0;
         virtual void                   __unkFunc20()                                                          = 0;
         virtual void                   __unkFunc21()                                                          = 0;
         virtual void                   __unkFunc22()                                                          = 0;
         virtual void                   __unkFunc23()                                                          = 0;
         virtual void                   __unkFunc24()                                                          = 0;
         virtual void                   __unkFunc25()                                                          = 0;
         virtual void                   __unkFunc26()                                                          = 0;
         virtual bool                   GetDynamicData(EventSequencer::System*, void* EventDynamicData)        = 0;
         virtual void                   __unkFunc27()                                                          = 0;
         virtual void                   _dup__unkFunc27()                                                      = 0;
         DWORD _unk;
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;