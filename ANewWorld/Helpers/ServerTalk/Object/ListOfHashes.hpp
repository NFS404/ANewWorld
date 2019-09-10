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
#include "IServerTalkObject.hpp"
#include <fstream>

namespace ServerTalkObjects {
   struct ListOfHashes : IServerTalkObject {
      std::map<std::string, DWORD64> HashMap;

      const char* getName() override {
         return "ListOfHashes";
      }
      const DWORD64 getPacketBodyHash() override {
         DWORD64 result = 0;
         for (auto& item : HashMap) {
            result += item.first.length() + item.second;
         }
         return result;
      };

      void addFileToHashMap(const std::filesystem::path& pathToFile) {
         std::ifstream inf(pathToFile);
         std::string fileContentStr(static_cast<const std::stringstream&>(std::stringstream() << inf.rdbuf()).str());
         HashMap[pathToFile.filename().u8string()] = static_cast<DWORD64>(std::hash<std::string>{}(fileContentStr));
      }

      // cereal
      template <class Archive>
      void serialize(Archive& archive) {
         archive(
            cereal::make_nvp("BodyHash", getPacketBodyHash()),
            cereal::make_nvp("HashMap", HashMap)
         );
      }
   };
}