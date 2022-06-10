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

#ifndef SPECTRUM_DISPLAY_WIDGET
#define SPECTRUM_DISPLAY_WIDGET

#include <utility>
#include <QWidget>

class SpectrumDisplayWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SpectrumDisplayWidget(int numBars, 
                                 float lowFreq, 
                                 float highFreq,
                                 QWidget *parent = nullptr);
  ~SpectrumDisplayWidget();

  // QWidget
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

public slots:
  void reset();
  void printSpectrum(const float* spectrum, size_t length);

private:
  std::pair<float, float> barRange(int barIndex) const;

  void selectBar(int index);    
private:
  struct Bar {
    Bar() : value(0.0), clipped(false) { }
    float   value;
    bool    clipped;
  };

  std::vector<Bar> m_bars;
  int m_barSelected;
  int m_timerId;
  float m_lowFreq;
  qreal m_highFreq;
//  FrequencySpectrum m_spectrum;
};

#endif // SPECTRUM_DISPLAY_WIDGET
