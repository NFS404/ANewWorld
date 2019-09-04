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

namespace GameInternals {
   namespace Data::GameTypes {
      struct Vector2 {
         float x, y;

         Vector2() : x(0.0f), y(0.0f) {}
         Vector2(const float _x, const float _y) {
            x = _x;
            y = _y;
         }

         operator float*() {
            return reinterpret_cast<float*>(this);
         }
      };
      struct Vector3 {
         float x, y, z;

         Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
         Vector3(const float _x, const float _y, const float _z) {
            x = _x;
            y = _y;
            z = _z;
         }

         operator float*() {
            return reinterpret_cast<float*>(this);
         }
      };
      struct Vector4 {
         float x, y, z, w;

         Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
         Vector4(const float _x, const float _y, const float _z, const float _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
         }

         operator float*() {
            return reinterpret_cast<float*>(this);
         }
      };
      struct Matrix4 {
         Vector4 v0, v1, v2, v3;
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;