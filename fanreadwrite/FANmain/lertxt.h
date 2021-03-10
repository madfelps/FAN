//============================================================================
// Name        : lertxt.h
// Author      : Felipe Moura Madureira, Henrique Borges Garcia e Gaspar Henrique
// Version     : 0.0
// Copyright   : Your copyright notice
// Description : main header
//============================================================================

#ifndef PROJECTFAN_LE_TXT_H

#define PROJECTFAN_LE_TXT_H

#include <iostream>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#include <fstream>
#include <omp.h>
#include "lertxt.h"
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include <string.h>
#include <array>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

void DescreveSensor(char StringDescreveSensor[][50]);

class NegativeValues{
private:


/* Conforme o manual RMS Can Protocol na página 12, algumas grandezas cujos valores ocupam 2 (dois) bytes devem passar por processamento para distinguirmos os números positivos e os negativos.  */

public:
	static float NegativeValuesTwoBytes(float Value);

};

//float NegativeValues::NegativeValuesTwoBytes(float Value);

class CutBytes{
private:

public:
	int CutByteInterval(unsigned char* CAN_DATA, int CanDataPosition, int MS_Position, int LS_Position, int CuttedByte);
};

class Torque:public NegativeValues{
private:

public:
	static float ProcessTorqueReceive(unsigned char* CAN_DATA, int MSByte, int LSByte);

};

class Flux:public NegativeValues{
private:

public:
	static float ProcessFluxReceive(unsigned char* CAN_DATA, int MSByte, int LSByte);
};

class Angle:public NegativeValues{
private:

public:
	static float ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte);
};

class AngleVelocity:public NegativeValues{
private:

public:
	static float ProcessAngleVelocity(unsigned char*, int MSByte, int LSByte);
};

class LowVoltage:public NegativeValues{
private:

public:
	static float ProcessLowVoltage(unsigned char*, int MSByte, int LSByte);
};

class HighVoltage:public NegativeValues{
private:

public:
	static float ProcessHighVoltage(unsigned char*, int MSByte, int LSByte);
};

class MotorPosInfo:public Angle, public AngleVelocity{
private:

	float MotorAngle;
	float MotorSpeed;
	float ElectricalOutFreq;
	float DeltaResolverFiltered;

	float MotorAngleProcessed;
	float MotorSpeedProcessed;
	float ElectricalOutFreqProcessed;
	float DeltaResolverFilteredProcessed;	
	int Byte[8];

public:

	MotorPosInfo(unsigned char*);
	MotorPosInfo();
	void UpdateObject(unsigned char* CAN_DATA);
	void ShowAllValuesProcessed();
	void IfID_MotorPosInfo(struct can_frame* frame, nlohmann::json& UDP_Package);

	float GetMotorAngleProcessed();
	float GetMotorSpeedProcessed();
	float GetElectricalOutFreqProcessed();
	float GetDeltaResolverFilteredProcessed();
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();

};

class TorqueTimerInfo: public Torque{
private:
	int CommandedTorque;
	int TorqueFeedback;
	int PowerOnTime;

	float CommandedTorqueProcessed;
	float TorqueFeedbackProcessed;
	float PowerOnTimeProcessed;
	float Byte[8];


public:
	TorqueTimerInfo();
	void UpdateObject(unsigned char*);
	void ShowAllValuesProcessed();
	void IfID_TorqueTimerInfo(struct can_frame* frame, nlohmann::json& UDP_Package);

	float GetCommandedTorqueProcessed();
	float GetTorqueFeedbackProcessed();
	float GetPowerOnTimeProcessed();
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();

};

class Temperature1:public Torque{

private:
	float ModuleA;
	float ModuleAProcessed;
	float ModuleB;
	float ModuleBProcessed;
	float ModuleC;
	float ModuleCProcessed;
	float GateDriverBoard;
	float GateDriverBoardProcessed;
	float Byte[8];

public:
	Temperature1();
	Temperature1(unsigned char* CAN_DATA);

	void UpdateObject(unsigned char*);
	float GetModuleAProcessed();
	float GetModuleBProcessed();
	float GetModuleCProcessed();
	float GetGateDriverBoardProcessed();
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();


//  A grandeza física temperatura se processa de modo igual ao Torque, ou seja, essa classe também usa a classe mãe Torque
	void  IfID_Temperature1(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();


};

class Temperature2:public Torque{

private:
	float ControlBoardTemperature;
	float ControlBoardTemperatureProcessed;
	float RTD1_Temperature;
	float RTD1_TemperatureProcessed;
	float RTD2_Temperature;
	float RTD2_TemperatureProcessed;
	float RTD3_Temperature;
	float RTD3_TemperatureProcessed;
	int Byte[8];

public:
	Temperature2();
	Temperature2(unsigned char* CAN_DATA);

	void UpdateObject(unsigned char*);
	float GetControlBoardTemperatureProcessed();
	float GetRTD1Processed();
	float GetRTD2Processed();
	float GetRTD3Processed();
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();


//  A grandeza física temperatura se processa de modo igual ao Torque, ou seja, essa classe também usa a classe mãe Torque
	void  IfID_Temperature2(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();
};

class Temperature3:public Torque{

private:
	float RTD4_Temperature;
	float RTD4_TemperatureProcessed;
	float RTD5_Temperature;
	float RTD5_TemperatureProcessed;
	float MotorTemperature;
	float MotorTemperatureProcessed;
	float TorqueShudder;
	float TorqueShudderProcessed;
	int Byte[8];

public:
	Temperature3();
	Temperature3(unsigned char* CAN_DATA);

	void UpdateObject(unsigned char*);
	float GetRTD4_TemperatureProcessed();
	float GetRTD5_TemperatureProcessed();
	float GetMotorTemperatureProcessed();
	float GetTorqueShudderProcessed();
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();


//  A grandeza física temperatura se processa de modo igual ao Torque, ou seja, essa classe também usa a classe mãe Torque
	void  IfID_Temperature3(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();
};

class CurrentInformation:public Torque{
private:
	float PhaseACurrent;
	float PhaseACurrentProcessed;
	float PhaseBCurrent;
	float PhaseBCurrentProcessed;
	float PhaseCCurrent;
	float PhaseCCurrentProcessed;
	float DC_BusCurrent;
	float DC_BusCurrentProcessed;
	int Byte[8];
public:
	CurrentInformation();
	CurrentInformation(unsigned char* CAN_DATA);
	void UpdateObject(unsigned char* CAN_DATA);
	void IfID_CurrentInformation(struct can_frame* frame, nlohmann::json& UDP_Package);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();
};

class VoltageInformation:public HighVoltage{
private:
	float DC_BusVoltage;
	float DC_BusVoltageProcessed;
	float OutputVoltage;
	float OutputVoltageProcessed;
	float VAB_Vd_Voltage;
	float VAB_Vd_VoltageProcessed;
	float VBC_Vd_Voltage;
	float VBC_Vd_VoltageProcessed;
	int Byte[8];
public:
	VoltageInformation();
	VoltageInformation(unsigned char* CAN_DATA);
	void UpdateObject(unsigned char* CAN_DATA);
	void IfID_VoltageInformation(struct can_frame* frame, nlohmann::json& UDP_Package);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();

};

class FluxInformation:public Torque, Flux{
private:
	float FluxCommand;
	float FluxCommandProcessed;
	float FluxFeedback;
	float FluxFeedbackProcessed;
	float IdFeedback;
	float IdFeedbackProcessed;
	float IqFeedback;
	float IqFeedbackProcessed;
	int Byte[8];
public:
	FluxInformation();
	FluxInformation(unsigned char* CAN_DATA);
	void UpdateObject(unsigned char* CAN_DATA);
	void IfID_FluxInformation(struct can_frame* frame, nlohmann::json& UDP_Package);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();
};

class InternalVoltages:public LowVoltage{
private:
	float VoltageReference1Dot5;
	float VoltageReference2Dot5;
	float VoltageReference5Dot0;
	float VoltageReference12;
	int Byte[8];
public:
	InternalVoltages();
	InternalVoltages(unsigned char* CAN_DATA);
	void UpdateObject(unsigned char* CAN_DATA);
	void IfID_InternalVoltages(struct can_frame* frame, nlohmann::json& UDP_Package);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();
};

class AnalogInputVoltages:public LowVoltage{

private:
	float AnalogInput1;
	float AnalogInput1Processed;
	float AnalogInput2;
	float AnalogInput2Processed;
	float AnalogInput3;
	float AnalogInput3Processed;
	float AnalogInput4;
	float AnalogInput4Processed;
	float AnalogInput5;
	float AnalogInput5Processed;
	float AnalogInput6;
	float AnalogInput6Processed;
	int Byte[8];

public:
	AnalogInputVoltages();
	AnalogInputVoltages(unsigned char* CAN_DATA);

	void IfID_AnalogInputVoltages(struct can_frame* frame, nlohmann::json& UDP_Package);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();

};

class ModulationIndex_FluxWeakening:public Torque{
private:
	float ModulationIndex;
	float ModulationIndexProcessed;
	float FluxWeakeningOutput;
	float FluxWeakeningOutputProcessed;
	float IdCommand;
	float IdCommandProcessed;
	float IqCommand;
	float IqCommandProcessed;
	int Byte[8];

public:
	ModulationIndex_FluxWeakening();
	ModulationIndex_FluxWeakening(unsigned char* CAN_DATA);
	void UpdateObject(unsigned char* CAN_DATA);
	void IfID_ModulationIndex_FluxWeakening(struct can_frame* frame, nlohmann::json& UDP_Package);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();

};

//Desenvolver o método pra dar update no frame
class CommandMessage{
private:
	int TorqueCommand;
	int TorqueCommandMSByte;
	int TorqueCommandLSByte;
	float SpeedCommand;
	float DirectionCommand;
	float InverterEnable;
	float InverterDischarge;
	float SpeedModeEnable;
	int CommandedTorqueLimit;
	int CommandedTorqueLimitMSB;
	int CommandedTorqueLimitLSB;
	int Byte[8];

public:
	CommandMessage();
	void UpdateFrame();
	void ProcessTorqueSend(float* TorqueCommand, int flag);
	void ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte);
	void UpdateFrame(struct can_frame* frame);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();


};

class DigitalInputStates{
private:
	int DigitalInput_1;
	int DigitalInput_2;
	int DigitalInput_3;
	int DigitalInput_4;
	int DigitalInput_5;
	int DigitalInput_6;
	int DigitalInput_7;
	int DigitalInput_8;
	int Byte[8];

public:
	DigitalInputStates();
	void UpdateFrame(struct can_frame* frame);
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();
};



class InternalStates: public CutBytes{
private:
	int VSM_State;
	int InverterState;
	int RelayState;
	int InverterRunMode;
	int InverterActiveDischargeState;
	int InverterCommandMode;
	int InverterEnableState;
	int InverterEnableLockout;
	int DirectionCommand;
	int BMS_Active;
	int BMS_LimitingTorque;

	int AuxByteCut;
	int Byte[8];

public:
	int GetVSM_State();
	int GetInverterState();
	int GetRelayState();
	int GetInverterRunMode();
	int GetInverterActiveDischargeState();
	int GetInverterCommandMode();
	int GetInverterEnableState();
	int GetInverterEnableLockout();
	int GetDirectionCommand();
	int GetBMS_Active();
	int GetBMS_LimitingTorque();

	void UpdateObject(unsigned char* CAN_DATA);
	void IfID_InternalStates(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();
	int GetByte0(), GetByte1(), GetByte2(), GetByte3(), GetByte4(), GetByte5(), GetByte6(), GetByte7();

};

class FaultErrors{
private:
	std::array<std::pair<bool, std::string>, 64>Errors;
	std::array<int, 8>Errors_Bytes;

public:
	FaultErrors();
	void IfId_FaultErrors(struct can_frame* frame, nlohmann::json& UDP_Package);
	void UpdateObject(unsigned char* CAN_DATA);
};

#endif //PROJECTFAN_LE_TXT_H