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
// Helpers
#include "Helpers\Game Internals\Data\GameTypes.h"
#include "Helpers\Game Internals\Data\GameFunctions.h"

class GameServerTalk : public IServerTalk {
   static inline GameServerTalk* pInstance = nullptr;

public:
   GameServerTalk() = default;
   GameServerTalk(const GameServerTalk&)     = delete;
   void operator=(const GameServerTalk&) = delete;
   static GameServerTalk* GetInstance() {
      if (!GameServerTalk::pInstance) {
         static GameServerTalk instance;
         instance.SetBaseUrl(*GameFunctions::Server::GetBaseUrl());
         instance.SetBaseRawUrl(*GameFunctions::Server::GetBaseRawUrl());

         GameServerTalk::pInstance = &instance;
      }
      return GameServerTalk::pInstance;
   }

   std::string UrlEncode(const std::string& str) override {
      return std::string(*GameFunctions::Server::UrlEncode(str.c_str()));
   }

   HRESULT SendDeleteRequest(const std::string& uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) override {
      std::string uri = GetBaseUrl() + "/" + uriPath;
      auto* pCallback = success ? GameTypes::Server::Callback::Create(&success) : nullptr;

      return GameFunctions::Server::SendDeleteRequest(uri.c_str(), pCallback);
   }

   HRESULT SendGetRequest(const std::string& uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) override {
      std::string uri = GetBaseUrl() + "/" + uriPath;
      auto* pCallback = success ? GameTypes::Server::Callback::Create(&success) : nullptr;

      return GameFunctions::Server::SendGetRequest(uri.c_str(), pCallback);
   }

   HRESULT SendPostRequest(const std::string& uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) override {
      std::string uri = GetBaseUrl() + "/" + uriPath;
      auto* pCallback = success ? GameTypes::Server::Callback::Create(&success) : nullptr;

      return GameFunctions::Server::SendPostRequest(uri.c_str(), nullptr, pCallback);
   }
   template <typename T, typename std::enable_if<std::is_base_of< ServerTalkObjects::IServerTalkObject, T>::value>::type>
   HRESULT SendPostRequest(const std::string& uriPath, const T& object, SuccessCallback success = nullptr, FailCallback fail = nullptr) {
      std::string uri = GetBaseUrl() + "/" + uriPath;
      auto* pCallback = success ? GameTypes::Server::Callback::Create(&success) : nullptr;
      std::string body = SerializeObject(object);
      const char* pBody = body.c_str();

      return GameFunctions::Server::SendPostRequest(uri.c_str(), &pBody, pCallback);
   }

   HRESULT SendPutRequest(const std::string& uriPath, SuccessCallback success = nullptr, FailCallback fail = nullptr) override {
      std::string uri = GetBaseUrl() + "/" + uriPath;
      auto* pCallback = success ? GameTypes::Server::Callback::Create(&success) : nullptr;

      return GameFunctions::Server::SendPutRequest(uri.c_str(), nullptr, pCallback);
   }
   template <typename T, typename std::enable_if<std::is_base_of<ServerTalkObjects::IServerTalkObject, T>::value>::type>
   HRESULT SendPutRequest(const std::string& uriPath, const T& object, SuccessCallback success = nullptr, FailCallback fail = nullptr) {
      std::string uri = GetBaseUrl() + "/" + uriPath;
      auto* pCallback = success ? GameTypes::Server::Callback::Create(&success) : nullptr;
      std::string body = SerializeObject(object);
      const char* pBody = body.c_str();

      return GameFunctions::Server::SendPutRequest(uri.c_str(), &pBody, pCallback);
   }
};