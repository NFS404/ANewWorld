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

namespace GameInternals {
   namespace Data::GameTypes {
      enum class DriverAidType : uint32_t
      {
         TRACTION_CONTROL      = 0,
         ANTI_BRAKE_LOCK       = 1,
         STABILITY_MANAGEMENT  = 2,
         DRIFT_STEERING_ASSIST = 3,
         DRIFT_GLUE_TO_ROAD    = 4,
         DRIFT_DYNAMIC_BRAKE   = 5,
         DRIFT_SPEED_CONTROL   = 6,
         RACELINE_ASSIST       = 7,
         BRAKING_ASSIST        = 8,
         DRIVER_AID_FIRST      = 0,
         DRIVER_AID_LAST       = 8,
         DRIVER_AID_NUMBER     = 9
      };
      enum class DriverClass : uint32_t
      {
         DRIVER_HUMAN        = 0,
         DRIVER_TRAFFIC      = 1,
         DRIVER_COP          = 2,
         DRIVER_RACER        = 3,
         DRIVER_NONE         = 4,
         DRIVER_NIS          = 5,
         DRIVER_REMOTE       = 6,
         DRIVER_REMOTE_RACER = 7,
         DRIVER_GHOST        = 8,
         DRIVER_MAX          = 9
      };
      enum class DriverStyle : uint32_t
      {
         STYLE_RACING     = 0,
         STYLE_DRAG       = 1,
         STYLE_DRIFT      = 2,
         STYLE_HIGH_SPEED = 3,
         STYLE_TRAFFIC    = 4
      };
      enum class ForceStopType : uint8_t {
         kEventForceStop   = 1,
         kEventInstantStop = 2,
         kEventForceCoast  = 4,
         kEventStopOnline  = 16
      };
      enum class EAILaneChangeType : uint32_t
      {
         LANECHG_NONE = 0,
         LANECHG_LEFT = 1,
         LANECHG_RIGHT = 2
      };
      enum class eTurnSignalState : uint32_t
      {
         TURNSIGNAL_NONE = 0,
         TURNSIGNAL_LEFT = 1,
         TURNSIGNAL_RIGHT = 2
      };
      enum class PhysicsMode : uint32_t
      {
         PHYSICS_MODE_INACTIVE  = 0,
         PHYSICS_MODE_SIMULATED = 1,
         PHYSICS_MODE_EMULATED  = 2
      };
      enum class SimableType : uint32_t
      {
         SIMABLE_INVALID   = 0,
         SIMABLE_VEHICLE   = 1,
         SIMABLE_SMACKABLE = 2,
         SIMABLE_EXPLOSION = 3,
         SIMABLE_HUMAN     = 4,
         SIMABLE_WEAPON    = 5,
         SIMABLE_NEWTON    = 6,
         SIMABLE_SENTRY    = 7,
         SIMABLE_FRAGMENT  = 8
      };
      enum class StatusType : uint32_t {
         STATUS_NO_TRIGGER          = 1,
         STATUS_ATTACHED            = 2,
         STATUS_COLLISION_WORLD     = 4,
         STATUS_COLLISION_OBJECT    = 8,
         STATUS_ENABLE_DRAG         = 32,
         STATUS_CHECKWORLD          = 64,
         STATUS_FIXEDCG             = 128,
         STATUS_ANIMATING           = 256,
         STATUS_INITIALIZED         = 512,
         STATUS_INTEGRATING         = 1024,
         STATUS_ENABLE_DRAG_ANGULAR = 2048,
         STATUS_DISABLE_INTEGRATOR  = 4096,
         STATUS_MODIFY_PRIMS        = 8192,
         STATUS_INACTIVE            = 16384,
         STATUS_COLLISION_GROUND    = 32768,
      };

      namespace CollisionGeometry {
         enum class BoundFlags : uint32_t {
            kBounds_Disabled             = 1,
            kBounds_PrimVsWorld          = 2,
            kBounds_PrimVsObjects        = 4,
            kBounds_PrimVsGround         = 8,
            kBounds_MeshVsGround         = 16,
            kBounds_Internal             = 32,
            kBounds_Box                  = 64,
            kBounds_Sphere               = 128,
            kBounds_Constraint_Conical   = 256,
            kBounds_Constraint_Prismatic = 512,
            kBounds_Joint_Female         = 1024,
            kBounds_Joint_Male           = 2048,
            kBounds_Male_Post            = 4096,
            kBounds_Joint_Invert         = 8192,
            kBounds_PrimVsOwnParts       = 16384,
         };
      }
      namespace VehicleFX {
         enum class ID : uint32_t
         {
            LIGHT_NONE        = 0,
            LIGHT_LHEAD       = 1,
            LIGHT_RHEAD       = 2,
            LIGHT_CHEAD       = 4,
            LIGHT_HEADLIGHTS  = COMPILETIME_OR_3FLAGS(LIGHT_LHEAD, LIGHT_RHEAD, LIGHT_CHEAD),
            LIGHT_LBRAKE      = 8,
            LIGHT_RBRAKE      = 16,
            LIGHT_CBRAKE      = 32,
            LIGHT_BRAKELIGHTS = COMPILETIME_OR_3FLAGS(LIGHT_LBRAKE, LIGHT_RBRAKE, LIGHT_CBRAKE),
            LIGHT_LREVERSE    = 64,
            LIGHT_RREVERSE    = 128,
            LIGHT_REVERSE     = COMPILETIME_OR_2FLAGS(LIGHT_LREVERSE, LIGHT_RREVERSE),
            LIGHT_LRSIGNAL    = 256,
            LIGHT_RRSIGNAL    = 512,
            LIGHT_LFSIGNAL    = 1024,
            LIGHT_RFSIGNAL    = 2048,
            LIGHT_LSIGNAL     = COMPILETIME_OR_2FLAGS(LIGHT_LFSIGNAL, LIGHT_LRSIGNAL),
            LIGHT_RSIGNAL     = COMPILETIME_OR_2FLAGS(LIGHT_RFSIGNAL, LIGHT_RRSIGNAL),
            LIGHT_COPRED      = 4096,
            LIGHT_COPBLUE     = 8192,
            LIGHT_COPWHITE    = 16384,
            LIGHT_COPS        = COMPILETIME_OR_3FLAGS(LIGHT_COPRED, LIGHT_COPBLUE, LIGHT_COPWHITE)
         };
      }

#pragma region C++ stuff
      DEFINE_ENUM_FLAG_OPERATORS(DriverAidType);
      DEFINE_ENUM_FLAG_OPERATORS(DriverClass);
      DEFINE_ENUM_FLAG_OPERATORS(DriverStyle);
      DEFINE_ENUM_FLAG_OPERATORS(ForceStopType);
      DEFINE_ENUM_FLAG_OPERATORS(EAILaneChangeType);
      DEFINE_ENUM_FLAG_OPERATORS(eTurnSignalState);
      DEFINE_ENUM_FLAG_OPERATORS(PhysicsMode);
      DEFINE_ENUM_FLAG_OPERATORS(SimableType);
      DEFINE_ENUM_FLAG_OPERATORS(StatusType);
      DEFINE_ENUM_FLAG_OPERATORS(CollisionGeometry::BoundFlags);
      DEFINE_ENUM_FLAG_OPERATORS(VehicleFX::ID);
#pragma endregion
   }
} namespace GameTypes = GameInternals::Data::GameTypes;