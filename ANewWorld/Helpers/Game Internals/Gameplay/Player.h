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

#pragma once
#include <WinDef.h>
#include "..\Data\Data.h"
using namespace GameInternals::Data::GameTypes;

namespace GameInternals {
   enum class SpeedUnit : BYTE {
      MPH,
      KMH,
      MPS
   };

   namespace Gameplay {
      namespace Player {
         static int getCashBalance() {
            return -1;
         }

         namespace Car {
            static float getSpeed(const SpeedUnit& targetUnit) {
               auto v = *(float*)Memory::readPointer(0x8A1018, false, 1, 0x108);
               switch (targetUnit) {
               case SpeedUnit::MPH:
                  return v * 2.23694f;
               case SpeedUnit::KMH:
                  return v * 3.6f;
               default:
                  return v;
               }
            }

            static float* getNOSValueAsPointer() {
               return nullptr;
            }
            static float getNOSValue() {
               return -1.0f;
            }

         }
      }
   }
}
namespace PlayerCarInternals = GameInternals::Gameplay::Player::Car;
namespace PlayerInternals = GameInternals::Gameplay::Player;