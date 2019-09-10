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
#include <stdint.h>
#include <stdlib.h>

namespace GameInternals {
   namespace Data::GameTypes {
      struct bNode {
         bNode* Next;
         bNode* Prev;

         bNode* GetNext() {
            return Next;
         }
         bNode* GetPrev() {
            return Prev;
         }
         bNode* Remove() {
            if (this->Prev)
               if (this->Next)
                  this->Prev->Next = this->Next;
               else
                  this->Prev->Next = nullptr;
            if (this->Next)
               if (this->Prev)
                  this->Next->Prev = this->Prev;
               else
                  this->Next->Prev = nullptr;

            return this;
         }
         bNode* AddAfter(bNode* node) {
            if (this->Next) {
               node->Next       = this->Next;
               this->Next->Prev = node;
            }
            this->Next = node;
            node->Prev = this;

            return node;
         }
         bNode* AddBefore(bNode* node) {
            node->Prev       = this->Prev;
            node->Next       = this;
            this->Prev->Next = node;
            this->Prev       = node;

            return node;
         }

         bNode() {
            bNode* startNode;
            startNode = new bNode;
            startNode->Next = nullptr;
            startNode->Prev = nullptr;
         }
         ~bNode() {
            if (this->Next)
               this->Prev->Next = this->Next;
            else
               this->Prev->Next = nullptr;
            free(this);
         }
      };
      template<class T>
      struct bTNode : bNode {
         T* GetNext() {
            return Next;
         }
         T* GetPrev() {
            return Prev;
         }
         T* Remove() {
            if (this->Prev)
               if (this->Next)
                  this->Prev->Next = this->Next;
               else
                  this->Prev->Next = nullptr;
            if (this->Next)
               if (this->Prev)
                  this->Next->Prev = this->Prev;
               else
                  this->Next->Prev = nullptr;

            return this;
         }
         T* AddAfter(T* node) {
            if (this->Next) {
               node->Next       = this->Next;
               this->Next->Prev = node;
            }
            this->Next = node;
            node->Prev = this;

            return node;
         }
         T* AddBefore(T* node) {
            node->Prev       = this->Prev;
            node->Next       = this;
            this->Prev->Next = node;
            this->Prev       = node;

            return node;
         }

         bTNode() {
            bTNode<T>* startNode;
            startNode = new bTNode<T>;
            startNode->Next = nullptr;
            startNode->Prev = nullptr;
         }
         ~bTNode() {
            free(this);
         }
      };
   }
} namespace GameTypes = GameInternals::Data::GameTypes;