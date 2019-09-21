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
#include "Settings.h"
// IO
#include <fstream>
#include <filesystem>
// (De)Serialization
#include <cereal\archives\xml.hpp>

namespace Settings {
   std::string mainFolder;
   std::string settingsFile;

   SettingsType instance ={};

   bool loadSettings() {
      try {
         Log(LogLevel::Info, "Loading settings file.");
         if (!std::filesystem::exists(settingsFile)) {
            Log(LogLevel::Warning, "Settings file was not found, creating a new one!");
            saveSettings();
            return true;
         }
         std::ifstream ifs(settingsFile);
         cereal::XMLInputArchive iarchive(ifs);
         iarchive(cereal::make_nvp("Settings", instance));
      } catch (std::runtime_error& e) {
         Log(LogLevel::Error, Logger::FormatString("Error during loading settings!\n%s", e.what()));
         MessageBox(NULL, e.what(), "Error during loading settings.", MB_ICONERROR | MB_OK);
         return false;
      }
      return true;
   }

   bool saveSettings() {
      try {
         Log(LogLevel::Info, "Saving settings file.");
         std::filesystem::create_directories(mainFolder);

         std::ofstream ofs(settingsFile);
         cereal::XMLOutputArchive oarchive(ofs);
         oarchive(cereal::make_nvp("Settings", instance));
      } catch (std::runtime_error& e) {
         Log(LogLevel::Error, Logger::FormatString("Error during saving settings!\n%s", e.what()));
         MessageBox(NULL, e.what(), "Error during saving settings.", MB_ICONERROR | MB_OK);
         return false;
      }
      return true;
   }

   void Init() {
      std::vector<wchar_t> _path(2048, 0);
      GetModuleFileNameW(NULL, &_path[0], _path.size());
      std::filesystem::path path(&_path[0]);

      mainFolder   = path.parent_path().u8string() + "\\ANewWorld\\";
      settingsFile = mainFolder + "ANewWorld.xml";
      Log(LogLevel::Debug, Logger::FormatString("Settings target file: \"%s.\"", settingsFile.c_str()));
      loadSettings();
   }
}