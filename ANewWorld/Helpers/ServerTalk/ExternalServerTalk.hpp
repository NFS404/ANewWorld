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
#include "IServerTalk.hpp"
#include <algorithm> // std::find
#include <utility>   // unique_ptr
// Helpers
//#define CPPHTTPLIB_ZLIB_SUPPORT -- someone fix this
#include <httplib.h>

class ExternalServerTalk : public IServerTalk {
   int32_t     port;
   uint32_t    userID;
   std::string securityToken;

   ExternalServerTalk() : port(80), userID(0), securityToken(std::string()) {}
public:
   std::unique_ptr<httplib::Client> httpCli;

   ExternalServerTalk(const ExternalServerTalk&) = delete;
   void operator=(const ExternalServerTalk&) = delete;
   static ExternalServerTalk* GetInstance() {
      static ExternalServerTalk instance;
      return &instance;
   }

   void setServerInfo(LPCWSTR url) {
      // parse url, from https://stackoverflow.com/a/11044337, modified
      {
         std::string uri = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(url);
         auto uriBegin   = uri.begin();
         auto uriEnd     = uri.end();

         auto protocolStart = uri.begin();
         auto protocolEnd   = std::find(protocolStart, uriEnd, ':');

         auto hostStart = protocolEnd + 3;
         auto pathStart = std::find(hostStart, uriEnd, '/');
         auto hostEnd   = std::find(hostStart, pathStart != uriEnd ? pathStart : uriEnd, ':');

         // port
         if (hostEnd != uriEnd && hostEnd[0] == ':') {
            auto dummy = hostEnd + 1;
            auto portEnd = pathStart != uriEnd ? pathStart : uriEnd;
            port = std::stoi(std::string(dummy, portEnd));
         }

         SetBaseUrl(std::string(uriBegin, hostEnd) + std::string(pathStart, uriEnd));
         SetBaseRawUrl(std::string(uriBegin, hostEnd));
      }
   }
   void setUserInfo(const std::string& token, uint32_t userID) {
      this->securityToken = token;
      this->userID = userID;
   }
   void setHttpCli() {
      Log(LogLevel::Debug, Logger::FormatString("Setting httpCli to %s (port %d)", GetBaseUrl(), port));
      httpCli.reset();
      httpCli = std::make_unique<httplib::Client>(GetBaseUrl().c_str(), port);
   }

   std::string UrlEncode(const std::string& str) override {
      // Copyright (C) 2003  Davis E. King (davis@dlib.net)
      // License: Boost Software License
      auto to_hex = [](unsigned char x) {
         return x + (x > 9 ? ('A'-10) : '0');
      };
      std::ostringstream os;

      for (std::string::const_iterator ci = str.begin(); ci != str.end(); ++ci) {
         if ((*ci >= 'a' && *ci <= 'z') ||
            (*ci >= 'A' && *ci <= 'Z') ||
             (*ci >= '0' && *ci <= '9')) { // allowed
            os << *ci;
         } else if (*ci == ' ') {
            os << '+';
         } else {
            os << '%' << to_hex(*ci >> 4) << to_hex(*ci % 16);
         }
      }

      return os.str();
   }

   HRESULT SendDeleteRequest(const std::string& uriPath, SuccessCallback success, FailCallback fail) override {
      auto res = this->httpCli->Delete(
         (GetBaseUrl() + "/" + uriPath).c_str(),
         {
            { "securityToken", this->securityToken },
            { "userID", std::to_string(this->userID) },
            { "Accept-Encoding", "gzip, deflate" }
         },
         "",
         "application/xml"
      );

      if (!res) {
         if (fail)
            fail(res->status);
      } else {
         if (res->status >= 200 && res->status <= 399) {
            if (success)
               success(res->status, res->body.c_str());
            return TRUE;
         }
      }
      return FALSE;
   }

   HRESULT SendGetRequest(const std::string& uriPath, SuccessCallback success, FailCallback fail) override {
      auto res = this->httpCli->Get(
         (GetBaseUrl() + "/" + uriPath).c_str(),
         {
            { "securityToken", this->securityToken },
            { "userID", std::to_string(this->userID) },
            { "Accept-Encoding", "gzip, deflate" }
         }
      );

      if (!res) {
         if (fail)
            fail(res->status);
      } else {
         if (res->status >= 200 && res->status <= 399) {
            if (success)
               success(res->status, res->body.c_str());
            return TRUE;
         }
      }
      return FALSE;
   }

   HRESULT SendPostRequest(const std::string& uriPath, SuccessCallback success, FailCallback fail) override {
      auto res = this->httpCli->Post(
         (GetBaseUrl() + "/" + uriPath).c_str(),
         {
            { "securityToken", this->securityToken },
            { "userID", std::to_string(this->userID) },
            { "Accept-Encoding", "gzip, deflate" }
         },
         "",
         "application/xml"
      );

      if (!res) {
         if (fail)
            fail(res->status);
      } else {
         if (res->status >= 200 && res->status <= 399) {
            if (success)
               success(res->status, res->body.c_str());
            return TRUE;
         }
      }
      return FALSE;
   }
   template <typename T, typename std::enable_if<std::is_base_of<ServerTalkObjects::IServerTalkObject, T>::value>::type>
   HRESULT SendPostRequest(const std::string& uriPath, const T& object, SuccessCallback success, FailCallback fail) {
      auto res = this->httpCli->Post(
         (GetBaseUrl() + "/" + uriPath).c_str(),
         {
            { "securityToken", this->securityToken },
            { "userID", std::to_string(this->userID) },
            { "Accept-Encoding", "gzip, deflate" }
         },
         SerializeObject(object),
         "application/xml"
      );

      if (!res) {
         if (fail)
            fail(res->status);
      } else {
         if (res->status >= 200 && res->status <= 399) {
            if (success)
               success(res->status, res->body.c_str());
            return TRUE;
         }
      }
      return FALSE;
   }

   HRESULT SendPutRequest(const std::string& uriPath, SuccessCallback success, FailCallback fail) override {
      auto res = this->httpCli->Put(
         (GetBaseUrl() + "/" + uriPath).c_str(),
         {
            { "securityToken", this->securityToken },
            { "userID", std::to_string(this->userID) },
            { "Accept-Encoding", "gzip, deflate" }
         },
         "",
         "application/xml"
      );

      if (!res) {
         if (fail)
            fail(res->status);
      } else {
         if (res->status >= 200 && res->status <= 399) {
            if (success)
               success(res->status, res->body.c_str());
            return TRUE;
         }
      }
      return FALSE;
   }
   template <typename T, typename std::enable_if<std::is_base_of<ServerTalkObjects::IServerTalkObject, T>::value>::type>
   HRESULT SendPutRequest(const std::string& uriPath, const T& object, SuccessCallback success, FailCallback fail) {
      auto res = this->httpCli->Put(
         (GetBaseUrl() + "/" + uriPath).c_str(),
         {
            { "securityToken", this->securityToken },
            { "userID", std::to_string(this->userID) },
            { "Accept-Encoding", "gzip, deflate" }
         },
         SerializeObject(object),
         "application/xml"
      );

      if (!res) {
         if (fail)
            fail(res->status);
      } else {
         if (res->status >= 200 && res->status <= 399) {
            if (success)
               success(res->status, res->body.c_str());
            return TRUE;
         }
      }
      return FALSE;
   }
};
