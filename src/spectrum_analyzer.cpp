#include "spectrum_analyzer.hpp"
#include <cassert>

SpectrumAnalyzer::SpectrumAnalyzer(size_t samples,
                   unsigned int sample_rate,
                   WindowFct window_fct = None)
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
  p = fftw_plan_dft_1d(SAMPLES, signal, result, FFTW_FORWARD, FFTW_ESTIMATE);
}

SpectrumAnalyzer::~SpectrumAnalyzer() {
  fftw_destroy_plan(p);
  fftw_free(window_);
  fftw_free(signal_);
  fftw_free(result_);
}

void SpectrumAnalyzer::calculate(const float* in_buffer,float* out_buffer){
  for(int i = 0,i<samples;i++) {
    signal_[i] = window_[i] * static_cast<double>in_buffer[i];
  }
  fftw_execute(p);
  for(int i = 0,i<samples;i++) {
    out_buffer[i] = static_cast<float>result_[i];
  }
}

void SpectrumAnalyzer::calculate_window_fct()
{
  for (int i=0; i<samples_; ++i) {
    DataType x = 0.0;

    switch (window_fct_) {
      case NONE:
        x = 1.0;
        break;
      case HANNING:
        x = 0.5 * (1 - cos((2 * M_PI * i) / (samples_ - 1)));
        break;
      default:
          assert(false);
    }

    window_[i] = x;
  }
}