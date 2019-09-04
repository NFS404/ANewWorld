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

#include "stdafx.h"
#include "Memory.h"

namespace Memory {
   namespace DLLMapping {
      FARPROC LoadedDLLInfo::GetProcAddress(std::string& funcName) {
         if (!rvaExportTable)
            return nullptr;

         DWORD ordinal                = (DWORD)funcName.c_str();
         IMAGE_EXPORT_DIRECTORY* pIED = (IMAGE_EXPORT_DIRECTORY*)(base + rvaExportTable);

         if (ordinal < 0x10000) {
            if (ordinal < pIED->Base)
               return nullptr;
            ordinal -= pIED->Base;
         } else {
            for (size_t i = 0; i < pIED->NumberOfNames; ++i) {
               if (!strcmp((char*)(((DWORD*)(pIED->AddressOfNames + base))[i] + base), funcName.c_str())) {
                  ordinal = ((WORD*)(pIED->AddressOfNameOrdinals + base))[i];
                  break;
               }
            }
         }

         if (ordinal >= pIED->NumberOfFunctions)
            return nullptr;
         return (FARPROC)(((DWORD*)(pIED->AddressOfFunctions + base))[ordinal] + base);
      }

      bool mapDLL(uint32_t dllSize, BYTE* dllBytes, LoadedDLLInfo* pOut_LoadedDLLInfo) {
         using DLLMain = BOOL(WINAPI*)(DWORD image_base, DWORD reason, LPVOID reserved);
         auto fnReadDLL = [dllSize, dllBytes](void* pTargetBuffer, uint32_t position, uint32_t readSize) {
            if (position + readSize > dllSize)
               return false;
            if (readSize == 0)
               return true;
            memcpy_s(pTargetBuffer, readSize, dllBytes + position, readSize);
            return true;
         };

         IMAGE_DOS_HEADER dosHeader;
         IMAGE_NT_HEADERS ntHeaders;

         void* image;
         DWORD_PTR image_base;

         DLLMain pDLLMain;

         // Load and check headers
         {
            // DOS Header
            fnReadDLL(&dosHeader, 0, sizeof(dosHeader));
            if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE || !dosHeader.e_lfanew)
               return false;

            // NT Header
            fnReadDLL(&ntHeaders, dosHeader.e_lfanew, sizeof(ntHeaders));
            if (ntHeaders.Signature != IMAGE_NT_SIGNATURE || ntHeaders.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC ||
                !(ntHeaders.FileHeader.Characteristics & IMAGE_FILE_DLL) || ntHeaders.FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED ||
                !ntHeaders.FileHeader.NumberOfSections)
               return false;
         }

         // Allocate memory for DLL
         {
            image = VirtualAlloc(NULL, ntHeaders.OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (!image)
               return false;

            image_base = (DWORD)image;
         }

         // Do relocation (if needed, most likely will)
         {
            if (image_base != ntHeaders.OptionalHeader.ImageBase) {
               if (!ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress ||
                   !ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
                  VirtualFree(image, 0, MEM_RELEASE);
                  return false;
               }

               IMAGE_BASE_RELOCATION* pIBR      = (IMAGE_BASE_RELOCATION*)(image_base + ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
               IMAGE_BASE_RELOCATION* pIBR_Last = (IMAGE_BASE_RELOCATION*)(image_base + ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size - sizeof(IMAGE_BASE_RELOCATION));
               DWORD                  baseDiff  = image_base - ntHeaders.OptionalHeader.ImageBase;

               while (pIBR < pIBR_Last) {
                  if (pIBR->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION)) {
                     WORD* relocItems  = (WORD*)pIBR + 1;
                     uint32_t numItems = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

                     for (uint32_t i = 0; i < numItems; ++i, ++relocItems) {
                        if (*relocItems >> 12 == IMAGE_REL_BASED_HIGHLOW)
                           *(DWORD*)(image_base + pIBR->VirtualAddress + (*relocItems & 0xFFF)) += baseDiff;
                     }
                  }

                  pIBR = (IMAGE_BASE_RELOCATION*)((BYTE*)pIBR + pIBR->SizeOfBlock);
               }
            }
         }

         // Resolve imports (if needed, most likely will)
         {
            if (ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress ||
                ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
               IMAGE_IMPORT_DESCRIPTOR* pIID;
               HMODULE hDLL;

               pIID = (IMAGE_IMPORT_DESCRIPTOR*)(image_base + ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
               while (pIID->Characteristics) {
                  hDLL = LoadLibraryA((char*)(image_base + pIID->Name));
                  if (!hDLL) {
                     VirtualFree(image, 0, MEM_RELEASE);
                     return false;
                  }

                  pIID++;
               }

               auto _pFirstThunk = (IMAGE_THUNK_DATA*)pIID->FirstThunk;
               auto _pOriginalFirstThunk = (IMAGE_THUNK_DATA*)pIID->OriginalFirstThunk;
               while (_pOriginalFirstThunk->u1.AddressOfData) {
                  if (_pOriginalFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) {
                     DWORD addrFunc = (DWORD)GetProcAddress(hDLL, (char*)(_pOriginalFirstThunk->u1.Ordinal & 0xFFFF));
                     if (!addrFunc) {
                        VirtualFree(image, 0, MEM_RELEASE);
                        return false;
                     }

                     _pFirstThunk->u1.Function = addrFunc;
                  } else {
                     auto pIBN      = (IMAGE_IMPORT_BY_NAME*)_pOriginalFirstThunk->u1.AddressOfData;
                     DWORD addrFunc = (DWORD)GetProcAddress(hDLL, (char*)pIBN->Name);
                     if (!addrFunc) {
                        VirtualFree(image, 0, MEM_RELEASE);
                        return false;
                     }

                     _pFirstThunk->u1.Function = addrFunc;
                  }

                  _pFirstThunk++;
                  _pOriginalFirstThunk++;
               }
            }
         }

         // Call entry point
         {

            if (ntHeaders.OptionalHeader.AddressOfEntryPoint) {
               pDLLMain = (DLLMain)(ntHeaders.OptionalHeader.AddressOfEntryPoint + image_base);
               __try {
                  if (!pDLLMain(image_base, DLL_PROCESS_ATTACH, NULL)) {
                     VirtualFree(image, 0, MEM_RELEASE);
                     return false;
                  }
               } __except (EXCEPTION_EXECUTE_HANDLER)
               {
                  return false;
               }
            }
         }

         // Fill in the _out
         pOut_LoadedDLLInfo->base           = image_base;
         pOut_LoadedDLLInfo->rvaExportTable = ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

         return true;
      }
   }

   CRITICAL_SECTION cs;
   DWORD oldMemoryAccess, memoryAccessAddress;
   int32_t memoryAccessSize;
   DWORD baseAddress = 0;

   void openMemoryAccess(const DWORD address, const int32_t& size) {
      memoryAccessAddress = address;
      memoryAccessSize = size;
      VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldMemoryAccess);
   }
   void restoreMemoryAccess() {
      VirtualProtect((LPVOID)memoryAccessAddress, memoryAccessSize, oldMemoryAccess, &oldMemoryAccess);
      memoryAccessAddress = 0;
      memoryAccessSize = 0;
   }

   DWORD calculateRelativeAddress(const DWORD from, const DWORD to, const bool isFromAbsolute) {
      if (!isFromAbsolute) {
         DWORD absoluteFrom = baseAddress + from;
         return to - absoluteFrom - 0x5;
      }
      return to - from - 0x5;
   }

   DWORD makeAbsolute(const DWORD relativeAddress) {
      return baseAddress + relativeAddress;
   }

   DWORD* readPointer(const DWORD baseOffset, const bool isBaseOffsetAbsolute, const int offsetCount, ...) {
      DWORD* pointer = (DWORD*)baseOffset;
      if (!isBaseOffsetAbsolute)
         pointer = (DWORD*)makeAbsolute(baseOffset);
      if (!*pointer)
         return nullptr;

      if (offsetCount > 0) {
         va_list offsets;
         va_start(offsets, offsetCount);
         for (int i = 0; i < offsetCount - 1; i++) {
            if (!pointer)
               break;

            pointer = (DWORD*)(*pointer + va_arg(offsets, int));
            if (!*pointer) {
               pointer = nullptr;
               break;
            }
         }

         if (pointer)
            pointer = (DWORD*)(*pointer + va_arg(offsets, int));
         va_end(offsets);
      } else {
         pointer = *(PDWORD*)pointer;
      }

      return pointer;
   }

   void writeCall(const DWORD from, const DWORD to, const bool isFromAbsolute) {
      EnterCriticalSection(&cs);
      DWORD relativeAddress = calculateRelativeAddress(from, to, isFromAbsolute);

      if (!isFromAbsolute) {
         DWORD absoluteFrom = baseAddress + from;

         openMemoryAccess(absoluteFrom, 5);
         *(BYTE*)(absoluteFrom) = 0xE8;
         *(DWORD*)(absoluteFrom + 0x1) = relativeAddress;
      } else {
         openMemoryAccess(from, 5);
         *(BYTE*)(from) = 0xE8;
         *(DWORD*)(from + 0x1) = relativeAddress;
      }

      restoreMemoryAccess();
      LeaveCriticalSection(&cs);
   }

   void writeJMP(const DWORD from, const DWORD to, const bool isFromAbsolute) {
      EnterCriticalSection(&cs);
      DWORD relativeAddress = calculateRelativeAddress(from, to, isFromAbsolute);

      if (!isFromAbsolute) {
         DWORD absoluteFrom = baseAddress + from;

         openMemoryAccess(absoluteFrom, 5);
         *(BYTE*)(absoluteFrom) = 0xE9;
         *(DWORD*)(absoluteFrom + 0x1) = relativeAddress;
      } else {
         openMemoryAccess(from, 5);
         *(BYTE*)(from) = 0xE9;
         *(DWORD*)(from + 0x1) = relativeAddress;
      }

      restoreMemoryAccess();
      LeaveCriticalSection(&cs);
   }

   void writeInterrupt3(const DWORD to, const int amount, const bool isFromAbsolute) {
      EnterCriticalSection(&cs);
      if (!isFromAbsolute) {
         DWORD absoluteTo = baseAddress + to;
         openMemoryAccess(absoluteTo, amount);

         for (int i = 0; i < amount; i++)
            * (BYTE*)(absoluteTo + i) = 0xCC;
      } else {
         openMemoryAccess(to, amount);
         for (int i = 0; i < amount; i++)
            * (BYTE*)(to + i) = 0xCC;
      }

      restoreMemoryAccess();
      LeaveCriticalSection(&cs);
   }
   void writeRet(const DWORD to, const bool isToAbsolute) {
      EnterCriticalSection(&cs);
      if (!isToAbsolute) {
         DWORD absoluteTo = baseAddress + to;
         openMemoryAccess(absoluteTo, 1);
         *(BYTE*)(absoluteTo) = 0xC3;
      } else {
         openMemoryAccess(to, 1);
         *(BYTE*)(to) = 0xC3;
      }

      restoreMemoryAccess();
      LeaveCriticalSection(&cs);
   }
   void writeNop(const DWORD to, const int amount, const bool isToAbsolute) {
      EnterCriticalSection(&cs);
      if (!isToAbsolute) {
         DWORD absoluteTo = baseAddress + to;
         openMemoryAccess(absoluteTo, amount);

         for (int i = 0; i < amount; i++)
            * (BYTE*)(absoluteTo + i) = 0x90;
      } else {
         openMemoryAccess(to, amount);
         for (int i = 0; i < amount; i++)
            * (BYTE*)(to + i) = 0x90;
      }

      restoreMemoryAccess();
      LeaveCriticalSection(&cs);
   }
   void writeRaw(const DWORD to, const bool isToAbsolute, const int byteCount, ...) {
      EnterCriticalSection(&cs);
      va_list bytes;
      va_start(bytes, byteCount);

      if (!isToAbsolute) {
         DWORD absoluteTo = baseAddress + to;
         openMemoryAccess(absoluteTo, byteCount);

         for (int i = 0; i < byteCount; i++)
            * (BYTE*)(absoluteTo + i) = va_arg(bytes, BYTE);
      } else {
         openMemoryAccess(to, byteCount);

         for (int i = 0; i < byteCount; i++)
            * (BYTE*)(to + i) = va_arg(bytes, BYTE);
      }

      va_end(bytes);
      restoreMemoryAccess();
      LeaveCriticalSection(&cs);
   }

   void Init() {
      InitializeCriticalSection(&cs);
      baseAddress = (DWORD)GetModuleHandle(0);
   }
}