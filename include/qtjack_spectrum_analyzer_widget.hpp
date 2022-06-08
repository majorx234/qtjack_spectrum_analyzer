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

#ifndef QTJACK_SPECTRUM_ANALYZER_WIDGET_HPP 
#define QTJACK_SPECTRUM_ANALYZER_WIDGET_HPP

#include <atomic>

#include <QtWidgets/QMainWindow>
//#include "ui_qtjack_spectrum_analyzerwidget.h"

// QtJack includes
#include <Client>
#include <Processor>
#include <RingBuffer>

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
  void closed();

 protected:
void resizeEvent(QResizeEvent* event) override;

 private:
  void setupJackClient();
//  Ui::QtJackSpectrumAnalyzerWidget *qtjack_spectrum_analyzer_ui_ = nullptr;
  QtJack::Client _client;
  QtJack::AudioPort _audio_in;
  QtJack::AudioBuffer* _audio_in_buffer;
  std::atomic_bool started;
  unsigned int _sample_rate;
};        

#endif // QTJACK_SPECTRUM_ANALYZER_WIDGET_HPP