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
#include "Object\IServerTalkObject.hpp"

#include <cereal/archives/xml.hpp>

class IServerTalk {
   // + /Engine.svc
   std::string baseUrl;
   // barebones
   std::string baseRawUrl;

public:
   typedef void(__cdecl* SuccessCallback)(DWORD, LPCSTR strResponseContent);
   typedef void(__cdecl* FailCallback)(DWORD);

   template <typename T>
   std::string SerializeObject(const T& object) {
      try {

         std::stringstream ss;
         {
            auto* pBase = (ServerTalkObjects::IServerTalkObject*) & object;
            cereal::XMLOutputArchive oarchive(ss, cereal::XMLOutputArchive::Options::NoIndent());
            oarchive(cereal::make_nvp(pBase->getName(), object));
         }
         return ss.str();
      } catch (std::runtime_error e) {
         MessageBox(NULL, e.what(), "Error during serializing.", MB_ICONERROR | MB_OK);
         return NULL;
      }
   }

   virtual std::string& GetBaseUrl() {
      return baseUrl;
   }
   virtual void SetBaseUrl(const char* baseUrl) {
      this->baseUrl = baseUrl;
   }
   virtual void SetBaseUrl(std::string& baseUrl) {
      this->baseUrl = baseUrl;
   }

   virtual std::string& GetBaseRawUrl() {
      return baseRawUrl;
   }
   virtual void SetBaseRawUrl(const char* baseRawUrl) {
      this->baseRawUrl = baseRawUrl;
   }
   virtual void SetBaseRawUrl(std::string& baseRawUrl) {
      this->baseRawUrl = baseRawUrl;
   }

   virtual std::string UrlEncode(const std::string & str) = 0;
   virtual HRESULT SendDeleteRequest(const std::string & uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) = 0;
   virtual HRESULT SendGetRequest(const std::string & uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) = 0;
   virtual HRESULT SendPostRequest(const std::string & uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) = 0;
   virtual HRESULT SendPutRequest(const std::string& uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) = 0;
};