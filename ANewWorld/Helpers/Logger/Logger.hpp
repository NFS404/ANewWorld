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
#include <ctime>   // time
#include <sstream> // stringstream
#include <iomanip> // put_time

enum class LogLevel : uint32_t {
   DebugConsole = 0,
   Debug        = 1,
   Info         = 2,
   Warning      = 3,
   Error        = 4
};

namespace Logger {
   // https://stackoverflow.com/a/26221725
   template<typename ... Args>
   static std::string FormatString(const std::string& format, Args&& ... args) {
      size_t size = snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args) ...) + 1;
      std::unique_ptr<char[]> buf(new char[size]);
      snprintf(buf.get(), size, format.c_str(), args ...);
      return std::string(buf.get(), buf.get() + size - 1);
   }

   static inline std::stringstream ss;
   static inline LogLevel          currentLogLevel = LogLevel::Debug;
   static void Log_(LogLevel level, std::string message,
                    const char* functionName, const char* fileName, const int32_t lineNumber) {
      if (level < currentLogLevel)
         return;

      ss.str(std::string());
      ss.clear();

      auto _time = std::time(nullptr);
      ss << std::put_time(localtime(&_time), "%d/%m/%Y %H:%M:%S");
      switch (level) {
         case LogLevel::Debug:
            printf("[DEBUG] %s - %s\n\tFrom: %s (%s:%d)\n", ss.str().c_str(), message.c_str(), functionName, fileName, lineNumber);
            break;
         case LogLevel::Info:
            printf("[INFO] %s - %s\n\tFrom: %s (%s:%d)\n", ss.str().c_str(), message.c_str(), functionName, fileName, lineNumber);
            break;
         case LogLevel::Warning:
            printf("[WARN] %s - %s\n\tFrom: %s (%s:%d)\n", ss.str().c_str(), message.c_str(), functionName, fileName, lineNumber);
            break;
         case LogLevel::Error:
            printf("[ERROR] %s - %s\n\tFrom: %s (%s:%d)\n", ss.str().c_str(), message.c_str(), functionName, fileName, lineNumber);
            break;
      }
   }
#define Log(LogLevel, Message) Logger::Log_(LogLevel, Message, __FUNCSIG__, __FILE__, __LINE__)
};