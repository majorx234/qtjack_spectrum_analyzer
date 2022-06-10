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

#include <fftw3.h>
#include <stdio.h>
#include <math.h>

#define SAMPLES 1024

void generate_signal(fftw_complex* signal){
  // saw wave example
  int freq = 100;
  const unsigned int sample_rate = SAMPLES;
  unsigned int num_samples_ = SAMPLES;

  unsigned int max = sample_rate/freq;
  float fmax = static_cast<float>(max);
 
  for(int i =0; i<num_samples_;i++) {
    signal[i][0] = 2.0*(((i % (max)) / fmax) - 0.5) ; //Real
    signal[i][1] = 0.0;  // Imag
  }
}

void print(fftw_complex* result) {
  for (int i = 0; i < SAMPLES; ++i) {
    double abs = sqrt(result[i][0] * result[i][0] +
                      result[i][1] * result[i][1]);

    printf("%g\n", abs);
  }
}

int main() {
  fftw_complex signal[SAMPLES];
  fftw_complex result[SAMPLES];
  fftw_plan p;

  generate_signal(signal);
  p = fftw_plan_dft_1d(SAMPLES, signal, result, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);
  fftw_destroy_plan(p);
  print(result);
  return 0;
}
