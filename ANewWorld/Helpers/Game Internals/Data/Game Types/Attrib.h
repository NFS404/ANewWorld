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
#include <WinDef.h>
#include "UCrc32.h"
#include "Helpers\Memory\Memory.h"

namespace GameInternals {
   namespace Data::GameTypes {
      namespace Attrib {
#pragma region Forward Declarations
         struct Array;
         struct Class;
         struct ClassPrivate;
         struct Collection;
         struct Definition;
         struct ExportManager;
         struct ExportNode;
         struct IExportPolicy;
         struct IGarbageCollector;
         struct Instance;
         struct HashMap;
         struct Node;
         struct PointerNode;
         struct Vault;
#pragma endregion

         static Collection* FindCollection(DWORD vaultHash, DWORD hash) {
            return ((Collection*(__cdecl*)(DWORD, DWORD))Memory::makeAbsolute(0x2C1D10))(vaultHash, hash);
         }
         static DWORD StringToKey(const char* string) {
            return ((DWORD(__cdecl*)(const char*))Memory::makeAbsolute(0x2C4720))(string);
         }

         struct HashMap {
            Node*    mTable;
            uint16_t mTableSize;
            uint16_t mNumEntries;
            uint16_t mRefCount;
            BYTE     mWorstCollision;
            BYTE     mKeyShift;
         };
         struct Array {
            uint16_t mAlloc;
            uint16_t mCount;
            uint16_t mSize;
            uint16_t mEncodedTypeAndPad;
         };
         struct ClassPrivate {
            struct CollectionHashMap /* : VecHashMap<unsigned int,Attrib::Collection,Attrib::Class::TablePolicy,1,96>::Node*, mTable*/ {
               //uint16_t mTableSize;
               //uint16_t mNumEntries;
               //uint16_t mFixedAlloc;
               //uint16_t mWorstCollision;
            };

            int32_t           Padding_2065[2];
            HashMap           mLayoutTable;
            CollectionHashMap mCollections;
            uint16_t          mLayoutSize;
            uint16_t          mNumDefinitions;
            Definition*       mDefinitions;
            Vault*            mSource;
            void*             mStaticData;
         };
         struct Class {
            uint32_t      mKey;
            ClassPrivate* mPrivates;
         };
         struct Collection {
            HashMap           mTable;
            const Collection* mParent;
            uint32_t          mKey;
            Class*            mClass;
            void*             mLayout;
            Vault*            mSource;
         };
         struct Definition {
            DWORD    mKey;
            DWORD    mType;
            uint16_t mOffset;
            uint16_t mSize;
            uint16_t mMaxCount;
            BYTE     mFlags;
            BYTE     mAlignment;
         };
         struct ExportManager {
            struct ExportPolicyPair {
               uint32_t       mType;
               IExportPolicy* mPolicy;
            };

            ExportPolicyPair* mExportPolicys;
            uint32_t          mReserve;
            uint32_t          mCount;
         };
         struct ExportNode {
            int32_t  Padding_2067[2];
            uint32_t mCount;
         };
         struct Instance {
            enum class Flags : uint32_t {
               kDynamic = 1
            };

            Collection* mCollection;
            void*       mLayoutPtr;
            uint32_t    mMsgPort;
            Flags       mFlags;
         };
         struct Node {
            DWORD mKey;
            union {
               void*    mPtr;
               Array*   mArray;
               uint32_t mValue;
               uint32_t mOffset;
            };
            uint16_t mTypeIndex;
            uint8_t  mMax;
            uint8_t  mFlags;
         };
         struct Vault {
            struct DataBlock {
               void* mData;
               union {
                  struct /* bitfield */
                  {
                     uint32_t _unk : 24;
                     uint32_t mKind : 8;
                  };
                  uint32_t mSize : 24;
               };
            };
            struct DependencyNode {
               int32_t Padding_2066[2];
               uint32_t mCount;
            };

            uint64_t           mVersion;
            uint32_t           mUserID;
            uint32_t           mRefCount;
            ExportManager&     mExportMgr;
            IGarbageCollector* mGC;
            DependencyNode*    mDependencies;
            DataBlock*         mDepData;
            uint32_t*          mDepIDs;
            uint32_t           mNumDependencies;
            uint32_t           mResolvedCount;
            PointerNode*       mPointers;
            BYTE*              mTransientData;
            ExportNode*        mExports;
            DataBlock*         mExportData;
            uint32_t*          mExportIDs;
            uint32_t           mNumExports;
            uint32_t           mNumAllocExports;
            uint32_t           mNumLoadedExports;
            bool               mInited;
            bool               mDeinited;
         };
      }
   }
} namespace GameTypes = GameInternals::Data::GameTypes;