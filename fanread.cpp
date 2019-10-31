//============================================================================
// Name        : main.cpp
// Author      : Felipe Madureira e Henrique Borges Garcia
// Version     : 0.0
// Copyright   : Your copyright notice
// Description : main FAN project
//============================================================================

/*
 *
 *
 *
 *
 * */


#define NUM_MSG 1e6

#define    UNDEFINED		         0
#define    TEMPERATURES_1 	         0x0A0
#define    TEMPERATURES_2	         0x0A1
#define    TEMPERATURES_3 	         0x0A2
#define    ANALOGIC_IN 	             0x0A3
#define    DIGITAL_IN		         0x0A4
#define    MOTOR_POSITION	         0x0A5
#define    CURRENT_INFO	             0x0A6
#define    VOLTAGE_INFO	             0x0A7
#define    FLUX_INFO   	             0x0A8
#define    INTERN_VOLTS 	         0x0A9
#define    INTERN_STATES	         0x0AA
#define    FAULT_CODES 	             0x0AB
#define    TORQUE_TIMER_INFO	     0x0AC
#define    MOD_FLUX_WEAK_OUT_INFO	 0x0AD
#define    FIRM_INFO	             0x0AE
#define    DIAGNOSTIC_DATA	         0x0AF
#define    COMMAND_MESSAGE           0x0C0

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

class NegativeValues{
private:

public:
	NegativeValues();
	int NegativeValuesTwoBytes(int Value);

};

NegativeValues::NegativeValues();

int NegativeValues::NegativeValuesTwoBytes(int Value){
		if(Value >= 32768){
			Value = Value - 65536;
		}
		else{
		}
		return Value;
}

class Torque:public NegativeValues{
private:

public:
	Torque();
	int ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte);

};

Torque::Torque();

int Torque::ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte){
	int TorqueValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	TorqueValue = NegativeValuesTwoBytes(TorqueValue);
	TorqueValue = TorqueValue/10;
	return TorqueValue;
}

class Angle:public NegativeValues{
private:

public:
	Angle();
	int ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte);
};


Angle::Angle();

int Angle::ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte){

	int AngleValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleValue = NegativeValuesTwoBytes(AngleValue);
	AngleValue = AngleValue/10;
	return AngleValue;

		/*if(!((*AngleValue >= -359.9) && (*AngleValue <= 359.9))){ // tratamento de erros (a desenvolver)
			throw 1;
		}*/
}

class AngleVelocity:public NegativeValues{
private:

public:
	AngleVelocity();
	int ProcessAngleVelocity(unsigned char*, int MSByte, int LSByte);
};

AngleVelocity::AngleVelocity();

int AngleVelocity::ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte){

	int AngleVelocityValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleVelocityValue = NegativeValuesTwoBytes(AngleVelocityValue);
	AngleVelocityValue = AngleVelocityValue/10;
	return AngleVelocityValue;
}

class MotorPosInfo:public Angle, public AngleVelocity{
private:

	int MotorAngle;
	int MotorSpeed;
	int ElectricalOutFreq;
	int DeltaResolverFiltered;

public:

	MotorPosInfo(unsigned char*);
	int GetMotorAngle();
	int GetMotorSpeed();
	int GetElectricalOutFreq();
	int GetDeltaResolverFiltered();

};

MotorPosInfo::MotorPosInfo(unsigned char* CAN_DATA){
	MotorAngle             = 0;
	MotorAngle             = ProcessAngle(CAN_DATA, 1, 0);
	MotorSpeed             = 0;
	MotorSpeed             = ProcessAngleVelocity(CAN_DATA, 3, 2);
	ElectricalOutFreq      = 0;
	DeltaResolverFiltered  = 0;
};

int MotorPosInfo::GetMotorAngle(){
	return MotorAngle;
}

int MotorPosInfo::GetMotorSpeed(){
	return MotorSpeed;
}

int MotorPosInfo::GetElectricalOutFreq(){
	return ElectricalOutFreq;
}

int MotorPosInfo::GetDeltaResolverFiltered(){
	return DeltaResolverFiltered;
}


class TorqueTimerInfo: public Torque{
private:
	int CommandedTorque;
	int TorqueFeedback;
	int PowerOnTime;


public:
	TorqueTimerInfo(unsigned char*);

	int GetCommandedTorque();
	int GetTorqueFeedback();
	int GetPowerOnTime();

};

TorqueTimerInfoInfo::TorqueTimerInfo(unsigned char* CAN_DATA){
	CommandedTorque     = 0;
	CommandedTorque     = ProcessTorque(CAN_DATA, 1, 0);
	TorqueFeedback      = 0;
	TorqueFeedback      = ProcessTorque(CAN_DATA, 3, 2);
	PowerOnTime         = 0;

}

int TorqueTimerInfo::GetCommandedTorque(){
	return CommandedTorque;
}

int TorqueTimerInfo::GetTorqueFeedback(){
	return TorqueFeedback;
}

int TorqueTimerInfo::GetPowerOnTime(){
	return PowerOnTime;
}

class CommandMessage{
private:
	int TorqueCommand;  //FLAG 1
	int SpeedCommand; //FLAG 2
	bool DirectionCommand; //FLAG 3
	bool InverterEnable; //FLAG 4
	bool InverterDischarge; //FLAG 5
	bool SpeedMode; //FLAG 6
	int CommandedTorqueLimit; //FLAG 7
	struct can_frame frame;

public:
	CommandMessage(int TorqueCommand, int SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, int CommandedTorqueLimit);
	void SetParameter(int, int);
};

CommandMessage::CommandMessage(int TorqueCommand, int SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, int CommandedTorqueLimit){
	this->TorqueCommand = TorqueCommand;
	this->SpeedCommand = SpeedCommand;
	this->DirectionCommand = DirectionCommand;
	this->InverterEnable = InverterEnable;
	this->InverterDischarge = InverterDischarge;
	this->SpeedMode = SpeedMode;
	this->CommandedTorqueLimit = CommandedTorqueLimit;

	frame.can_dlc = 8;
	frame.can_id = 0xC0;

	//Torque Command
	frame.data[0] = this->TorqueCommand & 0xFF;
	frame.data[1] = this->TorqueCommand >> 8;
	//speed command nao importa por enquanto (soh fazendo o modo de torque aqui!) TODO a desenvolver
	frame.data[2] = 0;
	frame.data[3] = 0;
	//Direction Command
	frame.data[4] = (unsigned char)this->DirectionCommand;
	//Inverter Enable

	frame.data[5] = ((unsigned char)this->InverterEnable) & 0x1 | ((unsigned char)this->InverterDischarge) & 0x1 << 1; // TODO estudar essa lógica aqui

	//Speed Mode Enable
	frame.data[6] = 0; // O controlador não vai mudar para o modo speed
	//CommandedTorqueLimit
	frame.data[7] = 0; //como está zero, o valor usado será aquele da E2PROM


};

void SetCommandedTorque(int Torque){ //TODO a desenvolver
	frame.can_id = COMMAND_MESSAGE;
	frame.data[]; //
}

void CommandMessage::SetParameter(int Value, int flag){ // TODO A DESENVOLVER 
	if(flag == 1){ //TorqueCommand
		frame.data[0] = Value & 0xFF;
		frame.data[1] = Value >> 8;

	}
	if(flag == 2){
		frame.data[2] = Value;
		frame.data[3] = Value;
		
	}
	if(flag == 3){
		frame.data[4] = (unsigned char)Value;
		
	}
	if(flag == 4){
		frame.data[5] = ((unsigned char)Value) & 0x1 | ((unsigned char)Value) & 0x1 << 1;
		
	}
	if(flag == 5){
		InverterDischarge = Value;
		
	}
	if(flag == 6){
		SpeedMode = Value;
		
	}
	if(flag == 7){
		CommandedTorqueLimit = Value;
		
	}

}


int main(){

	char DataLog[NUM_MSG][8];
	unsigned int DataID[NUM_MSG];
	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;


	//Configuração do CAN
	int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	struct sockaddr_can addr;

	struct ifreq ifr;

	strcpy(ifr.ifr_name, "can1");

	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;

	addr.can_ifindex = ifr.ifr_ifindex;

	bind(s, (struct sockaddr *) &addr, sizeof(addr));

	struct can_frame frame;

	frame.can_dlc = 8;

	

	//


	nbytes = 0;

	int MsgCounter = 0;
	int i, j;
	int CounterMotorPosition = 0;
	int CounterTorqueTimerInfo = 0;

	while (MsgCounter < NUM_MSG) {
		nbytes = read(s, &frame, sizeof(struct can_frame));
		if(nbytes != 0){ // se realmente leu msg...
			for(j = 0; j < frame.can_dlc; j++){
				DataLog[MsgCounter][j] = frame.data[j];
			}

			DataID[MsgCounter] = frame.can_id;
			MsgCounter++;

			if(MsgCounter%100 == 0){ // imprime a cada 100 mensagens
				for (i = 0; i < 8; i++) {
					printf("%5d", DataLog[0][i]);
				}

				std::cout << "  |  ";

				if(frame.can_id == MOTOR_POSITION){
					CounterMotorPosition++;
					if(CounterMotorPosition == 100){
						ObjMotorPosInfo = MotorPosInfo(frame.data);
						std::cout << "Angle: " << ObjMotorPosInfo.GetMotorAngle() << " ";
						std::cout << "Speed: " << ObjMotorPosInfo.GetMotorSpeed() << std::endl;
						CounterMotorPosition = 0;
					}
				}
				else if(frame.can_id == TORQUE_TIMER_INFO){
					CounterTorqueTimerInfo++;
					if(CounterTorqueTimerInfo == 100){
						ObjTorqueTimerInfo = TorqueTimerInfo(frame.data);
						std::cout << "Torque Feedback: " << ObjTorqueTimerInfo.GetTorqueFeedback() << " ";
						std::cout << "Commanded Torque: " << ObjTorqueTimerInfo.GetCommandedTorque() << std::endl;
						CounterTorqueTimerInfo = 0;
					}
				}
			}
		}

	}

	close(s);

	return 0;
}
