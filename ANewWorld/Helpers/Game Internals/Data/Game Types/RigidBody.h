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
#include "_enums.h"
#include "Behavior.h"
#include "bNode.h"
#include "CollisionGeometry.h"
#include "ICollisionBody.h"
#include "IDynamicsEntity.h"
#include "IRigidBody.h"
#include "WCollisionMgr.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct PVehicle;
      struct RigidBody : Behavior, IRigidBody, ICollisionBody, IDynamicsEntity, WCollisionMgr::ICollisionHandler, CollisionGeometry::IBoundable {
         struct Mesh {
            bTNode<Mesh> offset;
            UMath::Vector4*     mVerts;
            uint16_t            mNumVertices;
            uint16_t            mFlags;
            Attrib::Collection* mMaterial;
            UCrc32              mName;

            uint32_t Count() {
               sizeof(mVerts) / sizeof(UMath::Vector4);
            }
            Attrib::Collection* GetMaterial() {
               return mMaterial;
            }
            uint32_t GetNumVertices() {
               return mNumVertices;
            }
            UMath::Vector4* GetOrtho() {
               // calculate orthocentre
               return nullptr;
            }
            UMath::Vector4* GetOrtho(uint32_t) {
               // calculate orthocentre of the specified vertice maybe?
               return nullptr;
            }
            UCrc32 GetName() {
               return mName;
            }
         };
         struct State {
            UMath::Vector4 orientation;
            UMath::Vector3 position;
            StatusType     status;
            UMath::Vector3 linearVelocity;
            float          mass;
            UMath::Vector3 angularVelocity;
            float          oom; // OOMass
            UMath::Vector3 InertiaTensor; // PrincipalInertia
            /* ------------------------------------------ */ unsigned char  __unk1[0x4];
            UMath::Vector3 force;
            int8_t         leversInContact;
            BYTE           mode;
            BYTE           index;
            unsigned char  __unused2;
            UMath::Vector3 torque;
            float          radius;
            UMath::Matrix4 bodyMatrix;

            bool GetStatus(StatusType status) {
               return (int)(this->status & status) > 0;
            }
            void SetStatus(StatusType status) {
               this->status |= status;
            }
            void RemoveStatus(StatusType status) {
               this->status ^= status;
            }
         };

         virtual ~RigidBody();
         virtual void OnDebugDraw() = 0;
         virtual void OnBeginFrame(float);
         virtual void OnEndFrame(float);
         virtual void sub_83CBF0();
         virtual void sub_844260();
         virtual void DoPenetration(RigidBody* pTargetRigidBody);
         virtual bool ShouldSleep();
         virtual void ResolveWorldCollision(UMath::Vector3*, UMath::Vector3*, void* pSimCollisionInfo, Attrib::Collection*, SimSurface*, UMath::Vector3*);
         virtual bool CanCollideWith(RigidBody* pTargetRigidBody);
         virtual bool CanCollideWithWorld();
         virtual bool CanCollideWithGround();
         virtual bool CanCollideWithObjects();

#pragma region Behavior overrides
         virtual void Reset() override;
#pragma endregion
#pragma region IRigidBody overrides
         virtual ISimable*       GetOwner()                                                                override;
         virtual bool            IsSimple()                                                                override;
         virtual uint32_t        GetIndex()                                                                override;
         virtual SimableType     GetSimableType()                                                          override;
         virtual float           GetRadius()                                                               override;
         virtual float           GetMass()                                                                 override;
         virtual float           GetOOMass()                                                               override;
         virtual UMath::Vector3* GetPosition()                                                             override;
         virtual UMath::Vector3* GetLinearVelocity()                                                       override;
         virtual UMath::Vector3* GetAngularVelocity()                                                      override;
         virtual float           GetSpeed()                                                                override;
         virtual float           GetSpeedXZ()                                                              override;
         virtual void            GetForwardVector(UMath::Vector3 *)                                        override;
         virtual void            GetRightVector(UMath::Vector3 *)                                          override;
         virtual void            GetUpVector(UMath::Vector3 *)                                             override;
         virtual void            GetMatrix4(UMath::Matrix4 *)                                              override;
         virtual UMath::Vector4* GetOrientation()                                                          override;
         virtual void            GetDimension(UMath::Vector3 *)                                            override;
         virtual void            _dupGetDimension(UMath::Vector3 *)                                        override;
         virtual uint32_t        GetTriggerFlags()                                                         override;
         virtual void*           GetWCollider()                                                            override;
         virtual void            GetPointVelocity(UMath::Vector3* pInVector3, UMath::Vector3* pOutVector3) override;
         virtual void            SetPosition(UMath::Vector3*)                                              override;
         virtual void            SetLinearVelocity(UMath::Vector3*)                                        override;
         virtual void            SetAngularVelocity(UMath::Vector3*)                                       override;
         virtual void            SetRadius(float)                                                          override;
         virtual void            SetMass(float)                                                            override;
         virtual void            SetOrientation(UMath::Matrix4*)                                           override;
         virtual void            SetOrientation(UMath::Vector4*)                                           override;
         virtual void            ModifyXPos(float)                                                         override;
         virtual void            ModifyYPos(float)                                                         override;
         virtual void            ModifyZPos(float)                                                         override;
         virtual void            Resolve(UMath::Vector3*, UMath::Vector3*)                                 override;
         virtual void            ResolveForce(UMath::Vector3*, UMath::Vector3*)                            override;
         virtual void            ResolveForce(UMath::Vector3*)                                             override;
         virtual void            ResolveTorque(UMath::Vector3*, UMath::Vector3*)                           override;
         virtual void            ResolveTorque(UMath::Vector3*)                                            override;
         virtual void            PlaceObject(UMath::Matrix4*, UMath::Vector3*)                             override;
         virtual void            Accelerate(UMath::Vector3*, float)                                        override;
         virtual bool            IsTumbling()                                                              override;
         virtual void            ConvertLocalToWorld(UMath::Vector3*, bool)                                override;
         virtual void            ConvertWorldToLocal(UMath::Vector3*, bool)                                override;
         virtual void            Debug()                                                                   override;
#pragma endregion
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;