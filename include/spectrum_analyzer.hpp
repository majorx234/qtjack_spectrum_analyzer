/* 
 * This file is part of the rtaudio_example distribution (https://github.com/majorx234/qtjack_spectrum_analyzer ).
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

#ifndef SPECTRUM_ANALYZER_HPP 
#define SPECTRUM_ANALYZER_HPP

#include <fftw3.h>

class SpectrumAnalyzer {

 public:
  enum class WindowFct{
    NONE = 0,
    HANNING = 1
  }

  SpectrumAnalyzer(size_t samples,
                   unsigned int sample_rate,
                   WindowFct window_fct = None);
  ~SpectrumAnalyzer();
  void calculate(const float* in_buffer,float* out_buffer);
 private:
  void calculate_window_fct();
  size_t samples_;
  unsigned int sample_rate_;
  WindowFct window_fct_;
  double* window_;
  fftw_complex* signal_;
  fftw_complex* result_;
  fftw_plan p_;
}

#endif // SPECTRUM_ANALYZER_HPP