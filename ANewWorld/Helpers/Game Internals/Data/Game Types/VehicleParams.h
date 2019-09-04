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
#include <WinDef.h>
#include "_enums.h"
#include "Attrib.h"
#include "UMath.h"

namespace GameInternals {
   namespace Data::GameTypes {
      struct VehicleParams {
         /* 0x00 */ DWORD mTypeName;
         /* 0x04 */ VehicleParams* mSource;
         /* 0x08 */ unsigned char __padding[0x8];
         /* 0x10 */ DriverClass driverClass;
         /* 0x14 */ LPVOID pCar_CollectionData; // Attrib::gen::pvehicle?
         /* 0x18 */ /*  /\   */ DWORD __classSpace1;
         /* 0x1C */ /* /||\  */ DWORD __classSpace2;
         /* 0x20 */ /* ||||  */ DWORD __classSpace3;
         /* 0x24 */ UMath::Vector3*        pInitialRotation;
         /* 0x28 */ UMath::Vector3*        pInitialPosition;
         /* 0x2C */ FECustomizationRecord* pFECustomizationRecord;
         /* 0x30 */ /* ------------------------------------------ */ DWORD __unk_AIRelated;
         /* 0x34 */ /* ------------------------------------------ */ DWORD __unk_arg_4;
         /* 0x38 */ /* ------------------------------------------ */ LPVOID __unk2;
         /* 0x3C */ /* ------------------------------------------ */ DWORD  __unk_ImportanceRelated;
         /* 0x40 */ /* ------------------------------------------ */ unsigned char _unk6[0x4 * 4];
         /* 0x50 */ /* ------------------------------------------ */ LPVOID pSomeInstance;
         /* 0x54 */ /* ------------------------------------------ */ unsigned char _unk7[0x4 * 3];
         /* 0x60 */ VehicleParams* pSelf;

         VehicleParams() = default;
         VehicleParams(DriverClass driverClass, UMath::Vector3* pInitialRotation, UMath::Vector3* pInitialPosition,
            FECustomizationRecord* pFECustomizationRecord) {
            ZeroMemory(this, sizeof(VehicleParams));
            this->driverClass            = driverClass;
            this->pInitialRotation       = pInitialRotation;
            this->pInitialPosition       = pInitialPosition;
            this->pFECustomizationRecord = pFECustomizationRecord;

            this->pCar_CollectionData     = calloc(20, 20);
            this->pSomeInstance           = calloc(20, 20);
            this->__unk_ImportanceRelated = 0x8;
         }
         VehicleParams* Init(DWORD carHash) {
            auto* pCollection = Attrib::FindCollection(0x4A97EC8F, carHash);
            // __Q26Attrib8InstancePCQ26Attrib10CollectionUiPQ33UTL3COM8IUnknown
            ((LPVOID(__thiscall*)(LPVOID, LPVOID, int32_t))Memory::makeAbsolute(0x2C1740))(pCar_CollectionData, pCollection, 0);

            return ((VehicleParams*(__thiscall*)(VehicleParams*, DWORD, DWORD, DriverClass, LPVOID, UMath::Vector3*, UMath::Vector3*,
               DWORD, FECustomizationRecord*, LPVOID))Memory::makeAbsolute(0x284290))
               (this, NULL, NULL, driverClass, pCar_CollectionData, pInitialRotation, pInitialPosition, __unk_ImportanceRelated, pFECustomizationRecord, __unk2);
         }
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;