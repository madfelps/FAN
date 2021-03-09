#include "mainwindow.h"

#include <QApplication>

class FaultError{
private:

public:
    FaultError();
};

FaultError::FaultError(){
    std::array<std::pair<bool, std::string>, 64> errors{
    std::make_pair(false, "Hardware Gate/Desaturation Fault"),
    std::make_pair(false, "HW Over-current Fault"),
    std::make_pair(false, "Accelerator Shorted"),
    std::make_pair(false, "Accelerator Open"),
    std::make_pair(false, "Current Sensor Low"),
    std::make_pair(false, "Current Sensor High"),
    std::make_pair(false, "Module Temperature Low"),
    std::make_pair(false, "Module Temperature High"),

    std::make_pair(false, "Control PCB Temperature Low"),
    std::make_pair(false, "Control PCB Temperature High"),
    std::make_pair(false, "Gate Drive PCB Temperature Low"),
    std::make_pair(false, "Gate Drive PCB Temperature High"),
    std::make_pair(false, "5V Sense Voltage Low"),
    std::make_pair(false, "5V Sense Voltage High"),
    std::make_pair(false, "12V Sense Voltage Low"),
    std::make_pair(false, "12V Sense Voltage High"),

    std::make_pair(false, "2.5V Sense Voltage Low"),
    std::make_pair(false, "2.5V Sense Voltage High"),
    std::make_pair(false, "1.5V Sense Voltage Low"),
    std::make_pair(false, "1.5V Sense Voltage High"),
    std::make_pair(false, "DC Bus Voltage High"),
    std::make_pair(false, "DC Bus Voltage Low"),
    std::make_pair(false, "Pre-charge Timeout"),
    std::make_pair(false, "Pre-charge Voltage Failure"),

    std::make_pair(false, "EEPROM Checksum Invalid"),
    std::make_pair(false, "EEPROM Data Out of Range"),
    std::make_pair(false, "EEPROM Update Required"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Brake Shorted"),
    std::make_pair(false, "Brake Open"),

    std::make_pair(false, "Motor Over-speed Fault"),
    std::make_pair(false, "Over-current Fault"),
    std::make_pair(false, "Over-voltage Fault"),
    std::make_pair(false, "Inverter Over-temperature Fault"),
    std::make_pair(false, "Accelerator Input Shorted Fault"),
    std::make_pair(false, "Accelerator Input Open Fault"),
    std::make_pair(false, "Direction Command Fault"),
    std::make_pair(false, "Inverter Response Time-out Fault"),

    std::make_pair(false, "Hardware Gate/Desaturation Fault"),
    std::make_pair(false, "Hardware Over-current Fault"),
    std::make_pair(false, "Under-voltage Fault"),
    std::make_pair(false, "CAN Command Message Lost Fault"),
    std::make_pair(false, "Motor Over-temperature Fault"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),

    std::make_pair(false, "Brake Input Shorted Fault"),
    std::make_pair(false, "Brake Input Open Fault"),
    std::make_pair(false, "Module A Over-temperature Fault"),
    std::make_pair(false, "Module B Over-temperature Fault"),
    std::make_pair(false, "Module C Over-temperature Fault"),
    std::make_pair(false, "PCB Over-temperature Fault"),
    std::make_pair(false, "Gate Drive Board 1 Over-temperature Fault"),
    std::make_pair(false, "Gate Drive Board 2 Over-temperature Fault"),

    std::make_pair(false, "Gate Drive Board 3 Over-temperature Fault"),
    std::make_pair(false, "Current Sensor Fault"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Reserved"),
    std::make_pair(false, "Resolver Not Connected"),
    std::make_pair(false, "Inverter Discharge Active")

    };
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}
