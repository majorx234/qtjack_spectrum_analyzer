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

#define SAMPLES 1024

void generate_signal(float* signal){
  // saw wave example
  int freq = 100;
  const unsigned int sample_rate = SAMPLES;
  unsigned int num_samples_ = SAMPLES;

  unsigned int max = sample_rate/freq;
  float fmax = static_cast<float>(max);
 
  for(int i =0; i<num_samples_;i++) {
    signal[i] = 2.0*(((i % (max)) / fmax) - 0.5) ; //Real
  }
}

void print(float* result) {
  for (int i = 0; i < SAMPLES; ++i) {
    double abs = result[i];

    printf("%g\n", abs);
  }
}

int main() {
  float signal[SAMPLES];
  float result[SAMPLES];
  generate_signal(signal);
  SpectrumAnalyzer spectrum_analyzer(SAMPLES,SAMPLES, 
    SpectrumAnalyzer::WindowFct::NONE);
  spectrum_analyzer.calculate(signal,result);

  print(result);

  return 0;
}