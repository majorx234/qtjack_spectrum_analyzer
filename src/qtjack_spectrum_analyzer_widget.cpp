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
{
  setupJackClient();
}

QtJackSpectrumAnalyzerWidget::~QtJackSpectrumAnalyzerWidget() {
  
}

void QtJackSpectrumAnalyzerWidget::setupJackClient() {
  // Connect to JACK server
  bool connected = _client.connectToServer("qtjack_spectrum_analyzer");
  if(connected) {
    _audio_in = _client.registerAudioInPort("in");
    _sample_rate = _client.sampleRate();
    _client.setMainProcessor(this);
    _client.activate();
  }
  else {
    printf("Connection to jack server failed\n");
    emit closed();
  }
}

void QtJackSpectrumAnalyzerWidget::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
}

void QtJackSpectrumAnalyzerWidget::process(int samples){

}