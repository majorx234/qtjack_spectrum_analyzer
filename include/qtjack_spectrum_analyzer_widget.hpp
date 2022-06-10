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

#ifndef QTJACK_SPECTRUM_ANALYZER_WIDGET_HPP 
#define QTJACK_SPECTRUM_ANALYZER_WIDGET_HPP

#include <atomic>

#include <QtWidgets/QMainWindow>
//#include "ui_qtjack_spectrum_analyzerwidget.h"

// QtJack includes
#include <Client>
#include <Processor>
#include <RingBuffer>
#include "spectrum_analyzer.hpp"
#include "spectrum_display_widget.hpp"
#include "super_circular_buffer.hpp"
/*
namespace Ui {
class QtJackSpectrumAnalyzerWidget;
}*/

class QtJackSpectrumAnalyzerWidget : public QWidget, public QtJack::Processor  {
  Q_OBJECT

 public:
  explicit QtJackSpectrumAnalyzerWidget(QWidget *parent = nullptr);
  ~QtJackSpectrumAnalyzerWidget();
  void process(int samples);
 
 signals:
  void on_buffer_ready_to_calc_spectrum();
  void closed();

 protected:
  void timerEvent(QTimerEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;

 private slots:
  void calc_spectrum();

 private:
  void setupJackClient();
  void audio_process_fct();
//  Ui::QtJackSpectrumAnalyzerWidget *qtjack_spectrum_analyzer_ui_ = nullptr;
  QtJack::Client _client;
  QtJack::AudioPort _audio_in_port[2];
  QtJack::AudioSample* _audio_buffer[2];
  QtJack::AudioRingBuffer _audio_ring_buffer[2];
  size_t _audio_buffer_size;
  size_t _new_samples_in_audio_buffer;
  std::atomic_bool started;
  unsigned int _sample_rate;
  unsigned int _chunk_size;
  SuperCircularBuffer<float> super_circular_buffer[2];
  //std::thread audio_processing;

  SpectrumAnalyzer* spectrum_analyzer;
  // GUI Elements:
  SpectrumDisplayWidget* spectrum_display_widget;
};        

#endif // QTJACK_SPECTRUM_ANALYZER_WIDGET_HPP
