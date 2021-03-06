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

#include "stdafx.h"
#include <shellapi.h> // CommandLineToArgvW
// Main Features
#include "Main/Anticheat/EditWindowProperties.hpp"
#include "Main/Modding/Init.hpp"
// Extensions
#include "Extensions/Extensions.h"
#include "Extensions/InGame Menu/InGameMenu.h"
#include "Extensions/InGame Menu/Items/_LoadInGameMenuItems.hpp"
#include "Extensions/DInput8/DI8Extension.h"
#include "Extensions/WndProc/WndProcExtensions.hpp"

namespace TemporaryHelper {
   DWORD hkEInitEngineRet = 0x24EE20;
   void __declspec(naked) hkEInitEngine() {
      __asm {
         push ebp
         mov ebp, esp
         call AntiCheat::EditWindowProperties::editWindowProperties
         mov esp, ebp
         pop ebp
         jmp[hkEInitEngineRet]
      }
   }

   void WINAPI hkOutputDebugStringA(LPCSTR lpOutputString) {
      printf("[PASS_THROUGH] OutputDebugStringA: %s\n", lpOutputString);
   }
   void WINAPI hkOutputDebugStringW(LPCWSTR lpOutputString) {
      wprintf(L"[PASS_THROUGH] OutputDebugStringW: %ws\n", lpOutputString);
   }
   void __cdecl hkPassThrough_fputs(const char* Str, FILE* File) {
      printf("[PASS_THROUGH] fputs: %s\n", Str);
      fputs(Str, File);
   }
}

DWORD WINAPI Init(LPVOID) {
   Settings::Init();
   Logger::currentLogLevel = Settings::instance.logLevel;
   if (Settings::instance.logLevel == LogLevel::DebugConsole) {
      AllocConsole();
      freopen("CONOUT$", "w", stdout);
   } else {
      std::stringstream ss;
      auto _time = std::time(nullptr);
      ss << std::put_time(localtime(&_time), "Logs\\ANewWorld_Log [%d-%m-%Y %H.%M.%S].log");

      std::filesystem::create_directories(Settings::mainFolder + "Logs");
      freopen((Settings::mainFolder + ss.str()).c_str(), "w+", stdout);
   }
   Log(LogLevel::Info, std::string("Welcome to ANewWorld.\n\t") + Logger::FormatString("Command line: %ws", GetCommandLineW()));

   int32_t nArgs;
   LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   if (nArgs < 5) {
      Log(LogLevel::Error, "Invalid command line!");
      MessageBoxA(nullptr, "Invalid command line. Report this to a developer.", "Error", MB_ICONERROR);
      exit(-1);
   }

   // Helpers
   {
      Log(LogLevel::Info, "Initializing helpers.");
      // Set up ExternalServerTalk
      {
         Log(LogLevel::Debug, "Setting up ExternalServerTalk.");
         ExternalServerTalk::GetInstance()->setServerInfo(szArgList[2]);
         std::wstring wstrUserToken(szArgList[3]);
         ExternalServerTalk::GetInstance()->setUserInfo(std::string(wstrUserToken.begin(), wstrUserToken.end()), std::stoi(szArgList[4]));
         ExternalServerTalk::GetInstance()->setHttpCli();
      }
      Log(LogLevel::Debug, "Setting up LocalMirrorHook.");
      LocalMirrorHook::Prepare();
      Log(LogLevel::Debug, "Setting up WndProcHook.");
      Helpers::WndProcHook::Init();
   }

   // Extensions
   {
      Log(LogLevel::Info, "Initializing extensions.");
      Log(LogLevel::Debug, "Setting up InGameMenu.");
      Extensions::InGameMenu::Init();
      while (!Extensions::pImGuiIO)
         Sleep(100);

      Log(LogLevel::Debug, "Setting up DI8 extensions.");
      Extensions::DI8::Init();
      Log(LogLevel::Debug, "Setting up WndProc extensions.");
      Extensions::WndProc::InitAll();

      // InGameMenu Items
      Log(LogLevel::Debug, "Loading InGameMenu items.");
      Extensions::InGameMenu::loadItemsToInGameMenu();
      Log(LogLevel::Info, "Initialization complete.");
   }
   static std::string copyright = "Copyright (c) Berkay Yigit - berkay(2578). Greets to GamerZ.";
   return TRUE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
   if (ul_reason_for_call == DLL_PROCESS_ATTACH) { // hook: start -> _tmainCRTStartup
      DisableThreadLibraryCalls(hModule);
      Memory::Init();

      // Hook eInitEngine - to edit window properties (title & class names)
      TemporaryHelper::hkEInitEngineRet += Memory::baseAddress;
      Memory::writeCall(0x24F156, (DWORD)&TemporaryHelper::hkEInitEngine, false);

      // redirect OutputDebugStringA to printf
      DWORD addrOutputDebugStringA = Memory::makeAbsolute(0x71D140);
      Memory::openMemoryAccess(addrOutputDebugStringA, 4);
      *(DWORD*)addrOutputDebugStringA = (DWORD)&TemporaryHelper::hkOutputDebugStringA;
      Memory::restoreMemoryAccess();

      // redirect OutputDebugStringW to wprintf
      DWORD addrOutputDebugStringW = Memory::makeAbsolute(0x71D2FC);
      Memory::openMemoryAccess(addrOutputDebugStringW, 4);
      *(DWORD*)addrOutputDebugStringW = (DWORD)&TemporaryHelper::hkOutputDebugStringW;
      Memory::restoreMemoryAccess();

      // redirect fputs to printf & fputs
      DWORD addrFputs = Memory::makeAbsolute(0x71D4C4);
      Memory::openMemoryAccess(addrFputs, 4);
      *(DWORD*)addrFputs = (DWORD)&TemporaryHelper::hkPassThrough_fputs;
      Memory::restoreMemoryAccess();

      // log stuff
      Memory::writeRaw(0x910C2C, false, 1, 0x1);

      // Let's just disable the crash logs real quick
      Memory::writeRet(0x210920, false);

      // FastLoading (per nfsw_mod.txt, in order)
      //Memory::writeRaw(0x896D2, false, 2, 0x90, 0x90);
      //Memory::writeRaw(0x360AEE, false, 1, 0xEB);
      //Memory::writeRaw(0x360B27, false, 1, 0x84);
      //Memory::writeRaw(0x360BCD, false, 1, 0xEB);
      //Memory::writeRaw(0x288600, false, 2, 0xEB, 0x35);
      //Memory::writeRaw(0x306452, false, 1, 0xC3);

      // Let's get rid of ASI loaders while we are at it
      // EDIT: these happen too late, game already imports them --> close the game with a message and restart.
      //   or, look through the d3d9 init etc. functions and change the addresses.
      std::rename("LightFX.dll", "LightFX.dll.DisabledByANewWorld");
      std::rename("d3d9.dll", "d3d9.dll.DisabledByANewWorld");
      std::rename("dinput8.dll", "dinput8.dll.DisabledByANewWorld");

      // Run ANewWorld
      CreateThread(NULL, 0, &Init, NULL, 0, NULL);
   }
   return TRUE;
}