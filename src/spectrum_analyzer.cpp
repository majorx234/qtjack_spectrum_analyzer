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

#include "spectrum_analyzer.hpp"
#include <cassert>
#include <math.h>

SpectrumAnalyzer::SpectrumAnalyzer(size_t samples,
                   unsigned int sample_rate,
                   WindowFct window_fct)
  : samples_(samples)
  , sample_rate_(sample_rate)
  , window_fct_(window_fct)
  , window_(nullptr)
  , signal_(nullptr)
  , result_(nullptr)
{
  window_ = fftw_alloc_real(samples);
  signal_ = fftw_alloc_complex(samples);
  result_ = fftw_alloc_complex(samples);
  p_ = fftw_plan_dft_1d(samples, signal_, result_, FFTW_FORWARD, FFTW_ESTIMATE);
  calculate_window_fct();
}

SpectrumAnalyzer::~SpectrumAnalyzer() {
  fftw_destroy_plan(p_);
  fftw_free(window_);
  fftw_free(signal_);
  fftw_free(result_);
}

void SpectrumAnalyzer::calculate(const float* in_buffer,float* out_buffer){
  for(int i = 0;i<samples_;i++) {
    signal_[i][0] = window_[i] * static_cast<double>(in_buffer[i]);
    signal_[i][1] = 0;
  }
  fftw_execute(p_);
  for(int i = 0;i<samples_;i++) {
    out_buffer[i] = static_cast<float>(sqrt(result_[i][0] * result_[i][0] +
                                            result_[i][1] * result_[i][1]));
  }
}

void SpectrumAnalyzer::calculate_window_fct()
{
  for (int i=0; i<samples_; ++i) {
    double x = 0.0;

    switch (window_fct_) {
      case WindowFct::NONE:
        x = 1.0;
        break;
      case WindowFct::HANNING:
        x = 0.5 * (1 - cos((2 * M_PI * i) / (samples_ - 1)));
        break;
      default:
          assert(false);
    }

    window_[i] = x;
  }
}