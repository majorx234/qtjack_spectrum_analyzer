/* 
 * This file is part of the qtjack_spectrum_analyzer distribution (https://github.com/majorx234/qtjack_spectrum_analyzer ).
 * Copyright (c) 2021-2022 Majorx234 <majorx234@googlemail.com>
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SUPER_CICULAR_BUFFER_HPP_
#define SUPER_CICULAR_BUFFER_HPP_

#include <cstring>
#include <stdlib.h>

#include <stdio.h> // printf

template <class Type>
class SuperCircularBuffer
{
public:
  SuperCircularBuffer(unsigned int max_chunks, unsigned int chunk_size) 
  :  _chunks(0)
  ,  _max_chunks(2*max_chunks)
  , _chunk_size(chunk_size)
  , _data(nullptr)
  {
    _data = (Type*)std::malloc(2 * _max_chunks * chunk_size * sizeof(Type));
    std::memset(_data, 0, 2 * _max_chunks * _chunk_size * sizeof(Type));

  };
  ~SuperCircularBuffer() {
    if(_data)
      delete _data;
  };
  void add_chunk(Type* data) {
    if(_chunks < _max_chunks/2){
      size_t offset = (_chunks%_max_chunks) * _chunk_size;
      std::memcpy(_data + offset, data, _chunk_size);
    }
    else{
      size_t offset1 = (_chunks%(_max_chunks/2)) * _chunk_size;

      size_t offset2 = (_chunks%(_max_chunks/2) + (_max_chunks/2)) 
                       * _chunk_size;
      std::memcpy(_data + offset1,data, _chunk_size);
      std::memcpy(_data + offset2,data, _chunk_size);                 
    }
    _chunks++;
  };
  Type* access_data(){
    size_t offset1 = 0;
    if(_chunks >= _max_chunks/2)
      offset1 = (_chunks%(_max_chunks/2)) * _chunk_size;
    return _data + offset1;
  };
private:
  unsigned int _chunks;
  unsigned int _max_chunks;
  unsigned int _chunk_size;
  Type* _data;
};

#endif // SUPER_CICULAR_BUFFER_HPP_
