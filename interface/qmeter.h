/*
 *   Copyright (C) 2016 by P. Sereno
 *   http://www.qt-italia.net
 */

#ifndef QMETER_H
#define QMETER_H

 
#include <Qt>
#include <QWidget>
#include <QPropertyAnimation>

class QColor;
/**  The speedometer widget class. */
class QMeter : public QWidget
{
 Q_OBJECT
	Q_ENUMS(errorCode);
	Q_PROPERTY(double value READ value WRITE setValue);
	Q_PROPERTY(double minValue READ minValue WRITE setMinValue);
	Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue);
	Q_PROPERTY(double threshold READ threshold WRITE setThreshold);
    Q_PROPERTY(double thereshold)
	Q_PROPERTY(int precision READ precision WRITE setPrecision);
    Q_PROPERTY(int precisionNumeric READ precisionNumeric WRITE setPrecisionNumeric);
	Q_PROPERTY(QString units READ units WRITE setUnits);
    Q_PROPERTY(QString label READ label WRITE setLabel);
    Q_PROPERTY(int steps READ steps WRITE setSteps);
	Q_PROPERTY(double startAngle READ startAngle WRITE setStartAngle);
	Q_PROPERTY(double endAngle READ endAngle WRITE setEndAngle);	
	Q_PROPERTY(QColor foreground READ foreground WRITE setForeground);
	Q_PROPERTY(QColor background READ background WRITE setBackground);
	Q_PROPERTY(bool enableThreshold READ enableThreshold WRITE setThresholdEnabled);
    Q_PROPERTY(double beginValidValue READ beginValidValue WRITE setBeginValidValue);
    Q_PROPERTY(double endValidValue READ endValidValue WRITE setEndValidValue);
    Q_PROPERTY(bool enableValidWindow READ enableValidWindow WRITE setEnableValidWindow);
    Q_PROPERTY(double beginWarningValue READ beginWarningValue WRITE setBeginWarningValue);
    Q_PROPERTY(double endWarningValue READ endWarningValue WRITE setEndWarningValue);
    Q_PROPERTY(bool enableWarningWindow READ enableWarningWindow WRITE setEnableWarningWindow);
	Q_PROPERTY(bool enableNumericIndicator READ enableNumericIndicator WRITE setNumericIndicatorEnabled);

	
public: 
    /**
     * @brief The ErrorCode enum
     */
    enum ErrorCode {MaxValueError=1,MinValueError,ThresholdError,TargetError,PrecisionError,ColorError,UnitsEmpty,OutOfRange};

    /**
     * @brief QMeter
     * @param parent
     */
    QMeter(QWidget *parent = 0);

    /**
     * @brief value
     * @return
     */
    double value() const { return m_value; }

    /**
     * @brief minValue
     * @return
     */
    double minValue() const { return m_minValue; }

    /**
     * @brief maxValue
     * @return
     */
    double maxValue() const { return m_maxValue; }

    /**
     * @brief threshold
     * @return
     */
    double threshold() const { return m_threshold; }

    /**
     * @brief threshold_medium
     * @return
     */
    double threshold_medium() const { return m_threshold_medium;}

    /**
     * @brief beginValidValue
     * @return
     */
    double beginValidValue() const { return m_beginValidValue; }

    /**
     * @brief endValidValue
     * @return
     */
    double endValidValue() const { return m_endValidValue; }

    /**
     * @brief beginWarningValue
     * @return
     */
    double beginWarningValue() const { return m_beginWarningValue; }

    /**
     * @brief endWarningValue
     * @return
     */
    double endWarningValue() const { return m_endWarningValue; }

    /**
     * @brief precision
     * @return
     */
    int precision() const { return m_precision; }

    /**
     * @brief precisionNumeric
     * @return
     */
    int precisionNumeric() const { return m_precisionNumeric; }

    /**
     * @brief units
     * @return
     */
    QString units()const { return m_units; }

    /**
     * @brief label
     * @return
     */
    QString label()const { return m_label; }

    /**
     * @brief steps
     * @return
     */
    int steps() const { return m_steps; }

    /**
     * @brief startAngle
     * @return
     */
    double startAngle() const { return m_startAngle; }

    /**
     * @brief endAngle
     * @return
     */
    double endAngle() const { return m_endAngle; }

    /**
     * @brief foreground
     * @return
     */
    QColor foreground() const { return m_foreground; }

    /**
     * @brief background
     * @return
     */
    QColor background() const { return m_background; }

    /**
     * @brief enableThreshold
     * @return
     */
    bool enableThreshold() const { return m_thresholdEnabled; }

    /**
     * @brief enableMediumThreshold
     * @return
     */
    bool enableMediumThreshold() const { return m_thresholdMediumEnabled; }

    /**
     * @brief enableValidWindow
     * @return
     */
    bool enableValidWindow() const { return m_enableValidWindow; }

    /**
     * @brief enableWarningWindow
     * @return
     */
    bool enableWarningWindow() const { return m_enableWarningWindow; }

    /**
     * @brief enableNumericIndicator
     * @return
     */
    bool enableNumericIndicator() const { return m_numericIndicatorEnabled; }

signals:

    /**
     * @brief errorSignal
     */
    void errorSignal(int);

    /**
     * @brief thresholdAlarm
     */
    void thresholdAlarm(bool);

public slots:

    /**
     * @brief Sets the value to be displayed by the speedometer.
     * Usage example: Suppose you want to pass the value of a variable
     * "speed" to the speedometer. Just use meterWidget.setValue(speed).
     */
	void setValue(double);
    void setValue(int);
    /**
     * @brief Sets the speedometer minimum value.
     */
	void setMinValue(double);
	void setMinValue(int);	
    /**
     * @brief Sets the speedometer maximum value.
     */
	void setMaxValue(double);	
    void setMaxValue(int);
    /**
     * @brief Sets the start of high speed range
     */
	void setThreshold(double);
    void setThreshold(int);
    /**
     * @brief Sets the start of medium speed range
     */
    void setThresholdMedium(double);
    void setThresholdMedium(int);
    /**
     * @brief Sets the numerical precision of the speedometer
     */
    void setPrecision(int);
    void setPrecisionNumeric(int);
    /**
     * @brief Sets the unit of measure of the speedometer. If the unit is RPM, do
     * meterWidget.setUnits("RPM").
     */
	void setUnits(QString);
    /**
     * @brief Sets the type of data the gauge is showing. If it is speed, use the string
     * meterWidget.setLabel("Speed") and the string "Speed" will appear on the speedometer.
     * If you are interested in torque, just change the string to "Torque" or whatever.
     */
    void setLabel(QString);
    /**
     * @brief arrow the difference between two consecutive values ​​in the speedometer layout.
     * The speedometer will show (maximum - minimum)/steps values in the arc.
     */
	void setSteps(int);	
    /**
     * @brief Sets start angle of the speedometer's inner arc. Used in building widget layout,
     * no real-time functionality.
     */
	void setStartAngle(double);	
    /**
     * @brief setMediumAngle
     */
    void setMediumAngle(double);
    /**
     * @brief Sets end angle of the speedometer's inner arc. Used in building widget layout,
     * no real-time functionality.
     */
	void setEndAngle(double);	
    /**
     * @brief setForeground
     */
	void setForeground(QColor);
    /**
     * @brief setBackground
     */
	void setBackground(QColor);
    /**
     * @brief setThresholdEnabled
     */
    void setThresholdEnabled(bool);
    /**
     * @brief setThresholdMediumEnabled
     */
    void setThresholdMediumEnabled(bool);
    /**
     * @brief setNumericIndicatorEnabled
     */
    void setNumericIndicatorEnabled(bool);
    /**
     * @brief setValueWithSpringEffect
     * @param value
     */
    void setValueWithSpringEffect(int value);
    /**
     * @brief setBeginValidValue
     */
    void setBeginValidValue(double);
    /**
     * @brief setEndValidValue
     */
    void setEndValidValue(double);
    /**
     * @brief setEnableValidWindow
     */
    void setEnableValidWindow(bool);
    /**
     * @brief setBeginWarningValue
     */
    void setBeginWarningValue(double);
    /**
     * @brief setEndWarningValue
     */
    void setEndWarningValue(double);
    /**
     * @brief setEnableWarningWindow
     */
    void setEnableWarningWindow(bool);


protected:
    double m_value;
    double m_maxValue, m_minValue;
    int m_precision;
    int m_precisionNumeric;
    /**
     * @brief thresholdManager
     */
    void thresholdManager();
    /**
     * @brief paintEvent
     * @param event
     */
    void paintEvent(QPaintEvent *event);
    /**
     * @brief drawCrown
     * @param painter
     */
    void drawCrown(QPainter *painter);
    /**
     * @brief drawBackground
     * @param painter
     */
    void drawBackground(QPainter *painter);
    /**
     * @brief drawTicks
     * @param painter
     */
    void drawTicks(QPainter *painter);
    /**
     * @brief drawScale
     * @param painter
     */
    void drawScale(QPainter *painter);
    /**
     * @brief drawUnits
     * @param painter
     */
    void drawUnits(QPainter *painter);
    /**
     * @brief drawNeedle
     * @param painter
     */
    void drawNeedle(QPainter *painter);
    /**
     * @brief drawThresholdLine
     * @param painter
     */
    void drawThresholdLine(QPainter *painter);
    /**
     * @brief drawNumericValue
     * @param painter
     */
    void drawNumericValue(QPainter *painter);
    /**
     * @brief drawCoverGlass
     * @param painter
     */
    void drawCoverGlass(QPainter *painter);
    /**
     * @brief drawLabel
     * @param painter
     */
    void drawLabel(QPainter *painter);
    /**
     * @brief drawValidWindow
     * @param painter
     */
    void drawValidWindow(QPainter *painter);
    /**
     * @brief drawWarningWindow
     * @param painter
     */
    void drawWarningWindow(QPainter *painter);
    
	QString m_units;
    QString m_label;
	int m_steps;
    double m_startAngle,m_endAngle,m_mediumAngle;
	double m_threshold;
    double m_threshold_medium;
    bool m_thresholdFlag;
    bool m_thresholdMediumFlag;
    bool m_enableValidWindow;
    double m_beginValidValue, m_endValidValue;    
    bool m_enableWarningWindow;
    double m_beginWarningValue, m_endWarningValue;
	bool m_thresholdEnabled;
    bool m_thresholdMediumEnabled;
	bool m_numericIndicatorEnabled;
	QColor m_foreground;
	QColor m_background;
    QPropertyAnimation m_valAnimation;
};

#endif
