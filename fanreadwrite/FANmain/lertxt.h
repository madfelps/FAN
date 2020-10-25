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
#include <ncurses.h>
#include <time.h>
#include <fstream>
#include <omp.h>
#include "lertxt.h"
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

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

};

class TorqueTimerInfo: public Torque{
private:
	int CommandedTorque;
	int TorqueFeedback;
	int PowerOnTime;

	float CommandedTorqueProcessed;
	float TorqueFeedbackProcessed;
	float PowerOnTimeProcessed;


public:
	TorqueTimerInfo();
	void UpdateObject(unsigned char*);
	void ShowAllValuesProcessed();
	void IfID_TorqueTimerInfo(struct can_frame* frame, nlohmann::json& UDP_Package);

	float GetCommandedTorqueProcessed();
	float GetTorqueFeedbackProcessed();
	float GetPowerOnTimeProcessed();

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

public:
	Temperature1();
	Temperature1(unsigned char* CAN_DATA);

	void UpdateObject(unsigned char*);
	float GetModuleAProcessed();
	float GetModuleBProcessed();
	float GetModuleCProcessed();
	float GetGateDriverBoardProcessed();


//  A grandeza física temperatura se processa de modo igual ao Torque, ou seja, essa classe também usa a classe mãe Torque
	void  IfID_Temperature1(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();


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
public:
	CommandMessage();
	void UpdateFrame();
	void ProcessTorqueSend(float* TorqueCommand, int flag);
	void ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte);
	void UpdateFrame(struct can_frame* frame);


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

};



#endif //PROJECTFAN_LE_TXT_H