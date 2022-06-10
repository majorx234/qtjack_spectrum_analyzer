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

#include "spectrum_display_widget.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <utility>
#include <cassert>

SpectrumDisplayWidget::SpectrumDisplayWidget(
  int numBars, 
  float lowFreq, 
  float highFreq,
  QWidget *parent
)
  : QWidget(parent)
  , m_bars(numBars)
  , m_barSelected(-1)
  , m_lowFreq(lowFreq)
  , m_highFreq(highFreq)
{
  assert(numBars > 0);
  assert(highFreq > lowFreq);  
  setMinimumHeight(100);

}

SpectrumDisplayWidget::~SpectrumDisplayWidget() {

}
  
void SpectrumDisplayWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black); 

  const int numBars = m_bars.size();
  // Highlight region of selected bar
  if ((m_barSelected != -1) && numBars) {
    QRect regionRect = rect();
    regionRect.setLeft(m_barSelected * rect().width() / numBars);
    regionRect.setWidth(rect().width() / numBars);
    QColor regionColor(202, 202, 64);
    painter.setBrush(Qt::DiagCrossPattern);
    painter.fillRect(regionRect, regionColor);
    painter.setBrush(Qt::NoBrush);
  }
  
  QColor barColor(51, 204, 102);
  QColor clipColor(255, 255, 0);

  // Draw the outline
  const QColor gridColor = barColor.darker();
  QPen gridPen(gridColor);
  painter.setPen(gridPen);
  painter.drawLine(rect().topLeft(), rect().topRight());
  painter.drawLine(rect().topRight(), rect().bottomRight());
  painter.drawLine(rect().bottomRight(), rect().bottomLeft());
  painter.drawLine(rect().bottomLeft(), rect().topLeft());

  QVector<qreal> dashes;
  dashes << 2 << 2;;
  gridPen.setDashPattern(dashes);
  painter.setPen(gridPen);

  // Draw vertical lines between bars
  if (numBars) {
    const int numHorizontalSections = numBars;
    QLine line(rect().topLeft(), rect().bottomLeft());
    for (int i=1; i<numHorizontalSections; ++i) {
      line.translate(rect().width()/numHorizontalSections, 0);
      painter.drawLine(line);
    }
  }
  // Draw horizontal lines
  const int numVerticalSections = 10;
  QLine line(rect().topLeft(), rect().topRight());
  for (int i=1; i<numVerticalSections; ++i) {
    line.translate(0, rect().height()/numVerticalSections);
    painter.drawLine(line);
  }

  barColor = barColor.lighter();
  barColor.setAlphaF(0.75);
  clipColor.setAlphaF(0.75);

  // Draw the bars
  if (numBars) {
    // Calculate width of bars and gaps
    const int widgetWidth = rect().width();
    const int barPlusGapWidth = widgetWidth / numBars;
    const int barWidth = 0.8 * barPlusGapWidth;
    const int gapWidth = barPlusGapWidth - barWidth;
    const int paddingWidth = widgetWidth - numBars * (barWidth + gapWidth);
    const int leftPaddingWidth = (paddingWidth + gapWidth) / 2;
    const int barHeight = rect().height() - 2 * gapWidth;

    for (int i=0; i<numBars; ++i) {
      const qreal value = m_bars[i].value;
      if(!(value >= 0.0 && value <= 1.0))
        // ToDo: clipping here
        printf("value[%d]: %g\n", i,value);
      QRect bar = rect();
      bar.setLeft(rect().left() + leftPaddingWidth + (i * (gapWidth + barWidth)));
      bar.setWidth(barWidth);
      bar.setTop(rect().top() + gapWidth + (1.0 - value) * barHeight);
      bar.setBottom(rect().bottom() - gapWidth);

      QColor color = barColor;
      if (m_bars[i].clipped)
        color = clipColor;

      painter.fillRect(bar, color);
    }
  }
}

void SpectrumDisplayWidget::mousePressEvent(QMouseEvent *event) {
  const QPoint pos = event->pos();
  const int index = m_bars.size() * (pos.x() - rect().left()) / rect().width();
  selectBar(index);
}

void SpectrumDisplayWidget::reset() {
  //m_spectrum.reset();
  //spectrumChanged(m_spectrum)
  //updateBars();
}

void SpectrumDisplayWidget::printSpectrum(const float* spectrum, size_t length) {
  m_bars.assign(m_bars.size(),Bar());
  for (int i = 0 ; i <= length/2; ++i) {
    // ToDo: map Spectrum[m] to bars[n] (size m and n normally doesn't match)
    Bar &bar = m_bars[i];
    bar.value = spectrum[i];
      //bar.clipped |= spectrum[i]; // ToDo: check clipping ?
  }
  update();
}

std::pair<float, float> SpectrumDisplayWidget::barRange(int index) const {
  assert(((index >= 0) && (index < m_bars.size())));
  const qreal bandWidth = (m_highFreq - m_lowFreq) / m_bars.size();
  return std::pair<float, float>(index * bandWidth, (index+1) * bandWidth);

}

void SpectrumDisplayWidget::selectBar(int index) {
  const QString message = QString("%1 - %2 Hz")
                            .arg(barRange(index).first)
                            .arg(barRange(index).second);
  m_barSelected = index;
  update();
}