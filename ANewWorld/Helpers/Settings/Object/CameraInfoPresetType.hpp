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
#include "Helpers\Game Internals\Gameplay\Camera.h"
// (De)Serialization
#include <cereal\details\traits.hpp>
#include <cereal\types\map.hpp>
#include <cereal\types\string.hpp>
#include <cereal\archives\xml.hpp>

namespace Settings {
   struct CameraNodeDetails {
      float MinX;
      float MaxX;
      float MinY;
      float MaxY;
      float GraphScale;
      float XValues[3];
      float YValues[3];
      float Y2Values[3];

      template <class Archive>
      void serialize(Archive& archive) {
         archive(
            CEREAL_NVP(MinX),
            CEREAL_NVP(MaxX),
            CEREAL_NVP(MinY),
            CEREAL_NVP(MaxY),
            CEREAL_NVP(GraphScale),
            CEREAL_NVP(XValues),
            CEREAL_NVP(YValues),
            CEREAL_NVP(Y2Values)
         );
      }
   };
   struct CameraInfoPreset {
      CameraNodeDetails Height;
      CameraNodeDetails FOV;
      float Stiffness[2];
      bool  Selectable[2];
      float Angle[2];
      CameraNodeDetails Follow;

      void operator=(const GameInternals::CameraInfo& newCameraInfo) {
         memcpy_s(Stiffness, sizeof(float) * 2, newCameraInfo.Stiffness, sizeof(float) * 2);
         memcpy_s(Selectable, sizeof(bool) * 2, newCameraInfo.Selectable, sizeof(bool) * 2);
         memcpy_s(Angle, sizeof(float) * 2, newCameraInfo.Angle, sizeof(float) * 2);

         memcpy_s(&Height, sizeof(float) * 4, &newCameraInfo.Height, sizeof(float) * 4);
         //memcpy_s(&Height.XValues, sizeof(float) * 3, newCameraInfo.Height.pXValues, sizeof(float) * 3);
         //memcpy_s(&Height.YValues, sizeof(float) * 3, newCameraInfo.Height.pYValues, sizeof(float) * 3);
         //memcpy_s(&Height.Y2Values, sizeof(float) * 3, newCameraInfo.Height.pY2Values, sizeof(float) * 3);

         memcpy_s(&FOV, sizeof(float) * 4, &newCameraInfo.FOV, sizeof(float) * 4);
         //memcpy_s(&FOV.XValues, sizeof(float) * 3, newCameraInfo.FOV.pXValues, sizeof(float) * 3);
         //memcpy_s(&FOV.YValues, sizeof(float) * 3, newCameraInfo.FOV.pYValues, sizeof(float) * 3);
         //memcpy_s(&FOV.Y2Values, sizeof(float) * 3, newCameraInfo.FOV.pY2Values, sizeof(float) * 3);

         memcpy_s(&Follow, sizeof(float) * 4, &newCameraInfo.Follow, sizeof(float) * 4);
         //memcpy_s(&Follow.XValues, sizeof(float) * 3, newCameraInfo.Follow.pXValues, sizeof(float) * 3);
         //memcpy_s(&Follow.YValues, sizeof(float) * 3, newCameraInfo.Follow.pYValues, sizeof(float) * 3);
         //memcpy_s(&Follow.Y2Values, sizeof(float) * 3, newCameraInfo.Follow.pY2Values, sizeof(float) * 3);
      }
      void setTo(const GameInternals::CameraInfo* pNewCameraInfo) {
         *this = *pNewCameraInfo;
      }

      void copyDataToGameInternalsCompliantPointer(GameInternals::CameraInfo* pCameraInfo) {
         memcpy_s(pCameraInfo->Stiffness, sizeof(float) * 2, Stiffness, sizeof(float) * 2);
         memcpy_s(pCameraInfo->Selectable, sizeof(bool) * 2, Selectable, sizeof(bool) * 2);
         memcpy_s(pCameraInfo->Angle, sizeof(float) * 2, Angle, sizeof(float) * 2);

         memcpy_s(&pCameraInfo->Height, sizeof(float) * 4, &Height, sizeof(float) * 4);
         //memcpy_s(pCameraInfo->Height.pXValues, sizeof(float) * 3, &Height.XValues, sizeof(float) * 3);
         //memcpy_s(pCameraInfo->Height.pYValues, sizeof(float) * 3, &Height.YValues, sizeof(float) * 3);
         //memcpy_s(pCameraInfo->Height.pY2Values, sizeof(float) * 3, &Height.Y2Values, sizeof(float) * 3);

         memcpy_s(&pCameraInfo->FOV, sizeof(float) * 4, &FOV, sizeof(float) * 4);
         //memcpy_s(pCameraInfo->FOV.pXValues, sizeof(float) * 3, &FOV.XValues, sizeof(float) * 3);
         //memcpy_s(pCameraInfo->FOV.pYValues, sizeof(float) * 3, &FOV.YValues, sizeof(float) * 3);
         //memcpy_s(pCameraInfo->FOV.pY2Values, sizeof(float) * 3, &FOV.Y2Values, sizeof(float) * 3);

         memcpy_s(&pCameraInfo->Follow, sizeof(float) * 4, &Follow, sizeof(float) * 4);
         //memcpy_s(pCameraInfo->Follow.pXValues, sizeof(float) * 3, &Follow.XValues, sizeof(float) * 3);
         //memcpy_s(pCameraInfo->Follow.pYValues, sizeof(float) * 3, &Follow.YValues, sizeof(float) * 3);
         //memcpy_s(pCameraInfo->Follow.pY2Values, sizeof(float) * 3, &Follow.Y2Values, sizeof(float) * 3);
      }

      template <class Archive>
      void serialize(Archive& archive) {
         archive(
            CEREAL_NVP(Height),
            CEREAL_NVP(FOV),
            CEREAL_NVP(Stiffness),
            CEREAL_NVP(Selectable),
            CEREAL_NVP(Angle),
            CEREAL_NVP(Follow)
         );
      }
   };
   struct CameraPreset {
      bool             JoyViewEnabled;
      bool             SpeedFOVEnabled;
      float            SpeedFOVScale;
      int16_t          NitrousFOVWidening;
      CameraInfoPreset InfoPreset;

      template <class Archive>
      void serialize(Archive& archive) {
         archive(
            CEREAL_NVP(JoyViewEnabled),
            CEREAL_NVP(SpeedFOVEnabled),
            CEREAL_NVP(SpeedFOVScale),
            CEREAL_NVP(NitrousFOVWidening),
            CEREAL_NVP(InfoPreset)
         );
      }
   };
}

// True std::map (de)serialization
namespace cereal {
   template <class Archive, class C, class A,
      traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae> inline
      void save(Archive& ar, const std::map<std::string, Settings::CameraPreset, C, A>& map) {
      for (const auto& item : map)
         ar(cereal::make_nvp(item.first, item.second));
   }

   template <class Archive, class C, class A,
      traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae> inline
      void load(Archive& ar, std::map<std::string, Settings::CameraPreset, C, A>& map) {
      map.clear();

      auto hint = map.begin();
      while (true) {
         const auto namePtr = ar.getNodeName();
         if (!namePtr)
            break;

         Settings::CameraPreset value; ar(value);
         hint = map.emplace_hint(hint, std::move(std::string(namePtr)), std::move(value));
      }
   }
}