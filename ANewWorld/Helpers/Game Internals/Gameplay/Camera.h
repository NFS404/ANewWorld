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
#include "..\Data\Data.h"
#include "Helpers\Memory\Memory.h"
// Macro for the CameraInfo struct
#define _CAMERA_INFO_ARRAY_DESCRIPTOR(arrayId) WORD arrayCapacity_##arrayId; WORD arrayCount_##arrayId; WORD arrayItemSizeInBytes_##arrayId; WORD unknown_##arrayId;

namespace GameInternals {
   struct _internalCameraNodeDetails {
      float MinX;
      float MaxX;
      float MinY;
      float MaxY;
      float GraphScale;
      GameTypes::Vector3* pXValues;
      uint32_t pXValues_memberCount;
      GameTypes::Vector3* pYValues;
      uint32_t pYValues_memberCount;
      GameTypes::Vector3* pY2Values;
      uint32_t pY2Values_memberCount;
      DWORD AllocatedMemory;
   };
   struct CameraInfo {
      _internalCameraNodeDetails Height;
      _internalCameraNodeDetails FOV;
      char* CollectionName;
      _CAMERA_INFO_ARRAY_DESCRIPTOR(STIFFNESS);
      float Stiffness[2];
      _CAMERA_INFO_ARRAY_DESCRIPTOR(SELECTABLE);
      bool Selectable[2];
      _CAMERA_INFO_ARRAY_DESCRIPTOR(ANGLE);
      float Angle[2];
      _internalCameraNodeDetails Follow;

      void operator=(const CameraInfo& newCameraInfo) {
         memcpy_s(Stiffness, sizeof(float) * 2, newCameraInfo.Stiffness, sizeof(float) * 2);
         memcpy_s(Selectable, sizeof(bool) * 2, newCameraInfo.Selectable, sizeof(bool) * 2);
         memcpy_s(Angle, sizeof(float) * 2, newCameraInfo.Angle, sizeof(float) * 2);

         memcpy_s(&Height, sizeof(float) * 4, &newCameraInfo.Height, sizeof(float) * 4);
         //memcpy_s(Height.pXValues, sizeof(float) * 3, newCameraInfo.Height.pXValues, sizeof(float) * 3);
         //memcpy_s(Height.pYValues, sizeof(float) * 3, newCameraInfo.Height.pYValues, sizeof(float) * 3);
         //memcpy_s(Height.pY2Values, sizeof(float) * 3, newCameraInfo.Height.pY2Values, sizeof(float) * 3);

         memcpy_s(&FOV, sizeof(float) * 4, &newCameraInfo.FOV, sizeof(float) * 4);
         //memcpy_s(FOV.pXValues, sizeof(float) * 3, newCameraInfo.FOV.pXValues, sizeof(float) * 3);
         //memcpy_s(FOV.pYValues, sizeof(float) * 3, newCameraInfo.FOV.pYValues, sizeof(float) * 3);
         //memcpy_s(FOV.pY2Values, sizeof(float) * 3, newCameraInfo.FOV.pY2Values, sizeof(float) * 3);

         memcpy_s(&Follow, sizeof(float) * 4, &newCameraInfo.Follow, sizeof(float) * 4);
         //memcpy_s(Follow.pXValues, sizeof(float) * 3, newCameraInfo.Follow.pXValues, sizeof(float) * 3);
         //memcpy_s(Follow.pYValues, sizeof(float) * 3, newCameraInfo.Follow.pYValues, sizeof(float) * 3);
         //memcpy_s(Follow.pY2Values, sizeof(float) * 3, newCameraInfo.Follow.pY2Values, sizeof(float) * 3);
      }
      void setTo(CameraInfo* pNewCameraInfo) {
         *this = *pNewCameraInfo;
      }
   };

   namespace Gameplay {
      namespace Camera {
         static int* getActiveCameraIndexAsPointer() {
            auto* p = Memory::readPointer(0x920EAC, false, 1, 0x1D8);
            if (p && *p != 0xDEDEDEDE)
               return (int*)p;

            return nullptr;
         }
         static int getActiveCameraIndex() {
            int* p = getActiveCameraIndexAsPointer();
            return p ? *p : -1;
         }

         static CameraInfo* getActiveCameraInfoAsPointer() {
            return (CameraInfo*)Memory::readPointer(0x920EAC, false, 2, 0x254, 0x0);
         }
         static bool getActiveCameraInfo(CameraInfo*& out_CameraInfo) {
            auto p = getActiveCameraInfoAsPointer();
            if (!p) {
               out_CameraInfo = nullptr;
               return false;
            }

            out_CameraInfo = p;
            return true;
         }
      }
   }
} namespace CameraInternals = GameInternals::Gameplay::Camera;