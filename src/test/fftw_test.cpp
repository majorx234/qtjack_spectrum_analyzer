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
