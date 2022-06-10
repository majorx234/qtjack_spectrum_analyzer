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

#include <string>
#include <sstream>

#include <QtWidgets>
#include <QString>

#include "qtjack_spectrum_analyzer_widget.hpp"

QtJackSpectrumAnalyzerWidget::QtJackSpectrumAnalyzerWidget(QWidget *parent)
  : QWidget(parent)
  , Processor(_client)
//  , qtjack_spectrum_analyzer_ui_(new Ui::QtJackSpectrumAnalyzerWidget)
  , started(false)
  , _sample_rate(48000)
  , _chunk_size(1024)
  , _audio_buffer_size(48000*30) // default, but will reset in setupJackClient
  , _new_samples_in_audio_buffer(0)
  , super_circular_buffer{SuperCircularBuffer<float>(300,_chunk_size),SuperCircularBuffer<float>(300,_chunk_size)}
{
  setupJackClient();

  int numBars = 512; // chunk size /2
  float lowFreq =  _sample_rate/_chunk_size;
  float highFreq = _sample_rate/2.0;

  spectrum_analyzer = new SpectrumAnalyzer(_chunk_size,_sample_rate,
    SpectrumAnalyzer::WindowFct::NONE);

  spectrum_display_widget = new SpectrumDisplayWidget(512,
    lowFreq, highFreq, this);
  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(spectrum_display_widget,0,0);
  this->setLayout(layout);
  connect(this, &QtJackSpectrumAnalyzerWidget::on_buffer_ready_to_calc_spectrum,
          this, &QtJackSpectrumAnalyzerWidget::calc_spectrum);
  startTimer(50);
}

QtJackSpectrumAnalyzerWidget::~QtJackSpectrumAnalyzerWidget() {
  delete spectrum_analyzer;
}

void QtJackSpectrumAnalyzerWidget::setupJackClient() {
  // Connect to JACK server
  bool connected = _client.connectToServer("qtjack_spectrum_analyzer");
  if(connected) {
    _audio_in_port[0] = _client.registerAudioInPort("in 1");
    _audio_in_port[1] = _client.registerAudioInPort("in 2");
    _sample_rate = _client.sampleRate();
    _audio_ring_buffer[0] = QtJack::AudioRingBuffer();
    _audio_ring_buffer[1] = QtJack::AudioRingBuffer();

    // 30 seconds audiobuffer
    _audio_buffer_size = _sample_rate *30;
    _audio_buffer[0] =
      (QtJack::AudioSample*)malloc(_audio_buffer_size
                                   * sizeof(QtJack::AudioSample));
    _audio_buffer[1] =
      (QtJack::AudioSample*)malloc(_audio_buffer_size
                                   * sizeof(QtJack::AudioSample));
    _client.setMainProcessor(this);
    _client.activate();
  }
  else {
    printf("Connection to jack server failed\n");
    emit closed();
  }
}

void QtJackSpectrumAnalyzerWidget::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event)
  audio_process_fct();
}

void QtJackSpectrumAnalyzerWidget::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
}

void QtJackSpectrumAnalyzerWidget::process(int samples){
  // audio part
  _audio_in_port[0].buffer(samples).push(_audio_ring_buffer[0]);
  _audio_in_port[1].buffer(samples).push(_audio_ring_buffer[1]);
}

void QtJackSpectrumAnalyzerWidget::audio_process_fct() {
  int max_elements[2];
  max_elements[0] = _audio_buffer_size;
  max_elements[1] = _audio_buffer_size;

  int timeInMillisec = 1000;
  size_t ring_buffer_right_size =
      _audio_ring_buffer[0].numberOfElementsAvailableForRead();
  size_t ring_buffer_left_size =
      _audio_ring_buffer[1].numberOfElementsAvailableForRead();
  //need to copy, not more than buffer size
  max_elements[0] = ring_buffer_right_size < _audio_buffer_size ?
                   ring_buffer_right_size : _audio_buffer_size;
  // norm to 1024
  max_elements[0] = 1024*(max_elements[0] / 1024);
  int s0 = _audio_ring_buffer[0].read(_audio_buffer[0], max_elements[0]);

  //need to copy, not more than buffer size
  max_elements[1] = ring_buffer_left_size < _audio_buffer_size ?
                   ring_buffer_left_size : _audio_buffer_size;
  // norm to 1024
  max_elements[1] = 1024*(max_elements[1] / 1024);
  // copy data from ringbuffer to audio buffer
  int s1 = _audio_ring_buffer[1].read(_audio_buffer[1], max_elements[1]);
  for (int i = 0; i < max_elements[0]/1024; ++i)
  {
    size_t offset = i * 1024;
    super_circular_buffer[0].add_chunk(_audio_buffer[0] + offset);
  }
  for (int i = 0; i < max_elements[1]/1024; ++i)
  {
    size_t offset = i * 1024;
    super_circular_buffer[1].add_chunk(_audio_buffer[1] + offset);
  }
  emit on_buffer_ready_to_calc_spectrum();
}

void QtJackSpectrumAnalyzerWidget::calc_spectrum(){
  float result[1024];
  spectrum_analyzer->calculate(super_circular_buffer[0].access_data(),result);
  spectrum_display_widget->printSpectrum((float*)&result,512);
}