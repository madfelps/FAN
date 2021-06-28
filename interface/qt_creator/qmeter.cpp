/*
 *   Copyright (C) 2016 by P. Sereno
 *   http://www.qt-italia.net
 */

 
#include <QtGlobal>
#include <QtGui>
#include "qmeter.h"
#include <QPainter>
#include <QFont>
#include <QDebug>

QMeter::QMeter(QWidget *parent)
    : QWidget(parent)
{
   setPrecision(0);
   setSteps(10);
   m_thresholdFlag=false;
   m_thresholdMediumFlag = false;

   setForeground(Qt::white);
   setBackground(Qt::black);

   m_thresholdEnabled=true;
   m_thresholdMediumEnabled = true;
   m_numericIndicatorEnabled=true;
   setMinValue(0);
   setMaxValue(5000);
   setValue(0);
   setStartAngle(225);
   setMediumAngle(45);
   setEndAngle(-45);
   setMinimumSize(QSize(50,50));
   setLabel("Speed");
   setUnits("RPM");
   setThreshold(4000);
   setThresholdMedium(2500);

   setEnableValidWindow(false);
   setBeginValidValue(40.0);
   setEndValidValue(50.0);
   setEnableWarningWindow(false);
   setBeginWarningValue(30.0);
   setEndWarningValue(60.0);

   // Needle animation parameters
   m_valAnimation.setTargetObject(this);
   m_valAnimation.setPropertyName("value");
   m_valAnimation.setDuration(1000);


}

void QMeter::setValue(double value)
{
    if(value>m_maxValue)
    {
        m_value=m_maxValue;
    	emit errorSignal(OutOfRange);
    }
    else	
      if(value<m_minValue)
      {
        m_value=m_minValue;
        emit errorSignal(OutOfRange);	
      }
      else
        m_value=value;


    if(m_thresholdEnabled || m_thresholdMediumEnabled)
    {
     thresholdManager();    

    }
    update();
}

void QMeter::setValue(int value)
{
   setValue((double)value);
   update();
}

void QMeter::setMinValue(double value)
{
   m_minValue=value;
   update();
}

void QMeter::setMinValue(int value)
{
  setMinValue((double)value);
}


void QMeter::setMaxValue(double value)
{
	if(value > m_minValue)
	{
    	m_maxValue=value;
   		update();
   	}
   	else
   		emit errorSignal(MaxValueError);
}

void QMeter::setMaxValue(int value)
{
  setMaxValue((double)value);
}


void QMeter::setThreshold(double value)
{
	if(value > m_minValue && value < m_maxValue)
	{
    	m_threshold=value;
   		update();
   	}
   	else
   		emit errorSignal(ThresholdError);

}

void QMeter::setThreshold(int value)
{
  setThreshold((double)value);
}

void QMeter::setThresholdMedium(double value)
{
    if(value > m_minValue && value < m_maxValue)
    {
        m_threshold_medium=value;
        update();
    }
}

void QMeter::setThresholdMedium(int value)
{
  setThresholdMedium((double)value);
}
void QMeter::setPrecision(int precision)
{
   m_precision=precision;
   update();
}

void QMeter::setPrecisionNumeric(int precision)
{
   m_precisionNumeric=precision;
   update();
}

void QMeter::setUnits(QString units)
{
  m_units=units;
  update(); 
}

void QMeter::setLabel(QString label)
{
    m_label=label;
}

void QMeter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
	QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int side = qMin(width(), height());

    painter.setViewport((width() - side) / 2, (height() - side) / 2,side, side);
    painter.setWindow(-50, -50, 100, 100);

    drawBackground(&painter);
    drawCoverGlass(&painter);
    drawTicks(&painter);
    drawScale(&painter);
    drawUnits(&painter);
    drawThresholdLine(&painter);
    drawWarningWindow((&painter));
    drawValidWindow(&painter);
    drawNeedle(&painter);
    drawNumericValue(&painter);
    drawLabel(&painter);
    drawCrown(&painter);
}

void QMeter::setSteps(int nSteps)
{
	m_steps=nSteps;
	update();
}

void QMeter::setStartAngle(double value)
{
	m_startAngle=value;
	update();
}

void QMeter::setMediumAngle(double value)
{
    m_mediumAngle = value;
    update();
}

void QMeter::setEndAngle(double value)
{
	m_endAngle=value;
	update();
}

void QMeter::setForeground(QColor newForeColor)
{
	m_foreground=newForeColor;
	update();
}

void QMeter::setBackground(QColor newBackColor)
{
	m_background=newBackColor;
	update();
}


void QMeter::thresholdManager()
{
    //qDebug() <<"speed:" << value();
    //qDebug() << "m_thresholdMediumFlag :" << m_thresholdMediumFlag;
   // qDebug()  << "m_thresholdFlag :" << m_thresholdFlag<< "\n";

    if(m_value > m_threshold_medium && m_value < m_threshold && !m_thresholdMediumFlag)
    {
        m_thresholdMediumFlag=true;

    }
    else if(m_value > m_threshold && !m_thresholdFlag)
	{
  	 m_thresholdFlag=true;
     m_thresholdMediumFlag = false;
  	 emit thresholdAlarm(m_thresholdFlag);    
  	}
    else if(m_value < m_threshold && m_thresholdFlag)
	 {
  	  m_thresholdFlag=false;
      m_thresholdMediumFlag = false;
  	  emit thresholdAlarm(m_thresholdFlag);    
	 }    
    else if(m_value < m_threshold_medium && m_thresholdMediumFlag)
     {
      m_thresholdMediumFlag = false;
     }
}

void QMeter::setThresholdEnabled(bool enable)
{
  m_thresholdEnabled=enable;
  update();
}
void QMeter::setThresholdMediumEnabled(bool enable)
{
    m_thresholdMediumEnabled = enable;
    update();
}

void QMeter::setNumericIndicatorEnabled(bool enable)
{
  m_numericIndicatorEnabled=enable;
  update();
}

void QMeter::setValueWithSpringEffect(int value)
{
    m_valAnimation.setStartValue(m_value);
    m_valAnimation.setEndValue((double)value);
    m_valAnimation.setEasingCurve(QEasingCurve::OutBack);
    m_valAnimation.start();
}

void QMeter::setBeginValidValue(double beginValue)
{
    m_beginValidValue=beginValue;
    update();
}

void QMeter::setEndValidValue(double endValue)
{
    m_endValidValue=endValue;
    update();
}

void QMeter::setEnableValidWindow(bool enable)
{
    m_enableValidWindow=enable;
    update();
}

void QMeter::setBeginWarningValue(double beginValue)
{
    m_beginWarningValue=beginValue;
    update();

}

void QMeter::setEndWarningValue(double endValue)
{
    m_endWarningValue=endValue;
    update();

}

void QMeter::setEnableWarningWindow(bool enable)
{
    m_enableWarningWindow=enable;
    update();
}

