#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/** Header with MainWindowClass, the base class.*/
#include <QMainWindow>
/** The QUdpSocket class provides a UDP socket.*/
#include <QUdpSocket>
/** The QDebug class provides an output stream for debugging information*/
#include <QDebug>
/** C++ base library. */
#include <iostream>
/** The QJsonObject class encapsulates a JSON object. */
#include <QJsonObject>
/** The QJsonDocument class provides a way to read and write JSON documents.*/
#include <QJsonDocument>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//!  The MainWindow class.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:

    /**  SEND button function
     * @brief This function is called when SEND button is clicked and send speed to socket.
    */
    void on_sendButton_clicked();

    /**  About tag function
     * @brief This function is called when Help/About tag is clicked and a new window
     * is showed with informations about the autors.
    */
    void on_actionAbout_triggered();

    /**  ENABLE button function
     * @brief This function is called when the ENABLE button is clicked and enables the motor and speed
     * sending by the user.
    */
    void on_enable_motor_button_clicked();

    /**  DISABLE button function
     * @brief This function is called when the DISABLE button is clicked and disables the speed by
     * sending by the user.
    */
    void on_disable_motor_button_clicked();

private:
    /** ui Describes the User Interface design */
    Ui::MainWindow *ui;
    QUdpSocket *socket;

    /**
     * @brief This function sets the engine speedometer design parameters.
     * When called, the objects and methods being called on your body
     * assign certain characteristics to the speedometer. To change such
     * characteristics, you must change the method arguments of each object
     * individually in the scope.
    */
    void speedometer_config();

    /**
     * @brief This function sets the engine SpeedSlider style and functional
     * characteristics. Style includes size, color, width,etc. Functionality
     * includes the maximum value that can be set by the slider, the step
     * between each value (discretization) and how the slider connects to the
     * text label that shows the set value.
    */
    void speedslider_config();

    /**
     * @brief THE DESCRIPTION OF THESE FUNCTIONS WILL BE FINISHED WHEN THE
     * THERMOMETERS ARE DECIDED AND FINISHED
    */
    void thermometers_config();

    /**
     * @brief Sets the style of the thermometer.
     * @param value The value of current temperature os system sent by the embedded board via UDP.
     * @param med_threshold Transition temperature between low temperature to medium temperature of system.
     * @param max_threshold Transition temperature between medium temperature to high temperature of system.
     * @return A string that sets de style of progressbar which represents the thermometer. The string has the
     * format "QProgressBar::chunk{""background-color: red;""border-radius: 7px""}QProgressBar{""background-color: ...
     * rgb(211, 215, 207);""border: 3px solid gray;""border-radius: 10px""}"};
     */
    std::string style(double value, double med_threshold, double max_threshold);

    /**
     * @brief This function is responsible by discretizing the speed slider.
     * @param value is the current value of the slider
     * @param singleStep is the value of each step of the slider
     * @return The value that will be set on the slider
    */
    int getProperValue(int value, int singleStep);

    /**
     * @brief flag_motor is the  motor activation flag. If false, the motor doesn't
     * will receive speed values. If true, the socket sends required speed to embedded
     * board.
     */
    bool flag_motor = false;

    /**
     * @brief This function sends a datagram formatted in JSON to socket.
     * If you need to send a JSON message, for example {"ID":0x01,"Speed": 100}, do this:
     * 1. create a object: QJsonObject Data_packet;
     * 2. fill in the message: Data_packet["ID"] = "0x01";
     *                         Data_packet["Speed"] = "100";
     * 3. send JSON message using this function: sendJsonToUDP(Data_packet);
     * @param QJsonObject& is a object of QJsonObject class.
     */
    void sendJsonToUDP(const QJsonObject&);

    /**
     *  @brief This function creates a socket, receive the messages sended by embedded board via UDP and
     * updates the values in the graphic interface.
     */
    void receiveMessages();

};
#endif // MAINWINDOW_H
