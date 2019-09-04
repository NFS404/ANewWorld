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
#include "stdafx.h"
// Hooks
#include "Main/Modding/Hooks/_InstallHooks.hpp"
// Helpers
#include "Types/FeaturesType.hpp"
#include "Helpers/ServerTalk/ExternalServerTalk.hpp"

namespace Modding {
   namespace ServerJobs {
      void callback_GetFileModsSuccess(DWORD code, LPCSTR content) {
         printf("Server returned %ul for 'Modding/ANewWorld/GetFileMods'.\n", code);
      }
      void callback_GetFileModsFail(DWORD code) {
         MessageBoxA(NULL, "Server returned error code for 'Modding/ANewWorld/GetFileMods'! Report this to the server developers.", "Error", MB_ICONERROR);
         exit(EXIT_FAILURE);
      }

      void callback_GetServerFeaturesSuccess(DWORD code, LPCSTR content) {
         printf("Server returned %ul for 'Modding/ANewWorld/GetServerFeatures'.\n", code);

         Server::FeaturesType features;
         try {
            std::stringstream ss(content);
            cereal::XMLInputArchive iarchive(ss);
            iarchive(features);
         } catch (std::runtime_error& e) {
            MessageBox(NULL, e.what(), "Error during parsing server features! Report this to the server developers.", MB_ICONERROR);
            exit(EXIT_FAILURE);
         }

         for (auto& feature : features.listOfFeatures) {
            if (feature == Server::FeatureType::FileMods || feature == Server::FeatureType::EncryptedFileMods) {
               Hooks::installHooks();
               if (!ExternalServerTalk::GetInstance()->SendGetRequest("Modding/ANewWorld/GetFileMods", &callback_GetFileModsSuccess, &callback_GetFileModsFail)) {
                  MessageBoxA(NULL, "Server request 'Modding/ANewWorld/GetFileMods' failed! Report this to the server developers.", "Error", MB_ICONERROR);
                  exit(EXIT_FAILURE);
               }

            }
         }
      }
      void callback_GetServerFeaturesFail(DWORD code) {
         MessageBoxA(NULL, "Server returned error code for 'Modding/ANewWorld/GetServerFeatures', skipping ANewWorld...", "Warning", MB_ICONWARNING);
      }

      void doServerJobs() {
         if (!ExternalServerTalk::GetInstance()->SendGetRequest("Modding/ANewWorld/GetServerFeatures", &callback_GetServerFeaturesSuccess, &callback_GetServerFeaturesFail)) {
            MessageBoxA(NULL, "Server request 'Modding/ANewWorld/GetServerFeatures' failed, skipping ANewWorld...", "Warning", MB_ICONWARNING);
         }
      }
   }
}
