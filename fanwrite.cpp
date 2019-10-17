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
	int TorqueCommand;
	int SpeedCommand;
	bool DirectionCommand;
	bool InverterEnable;
	bool InverterDischarge;
	bool SpeedMode;
	int CommandedTorqueLimit;
	struct can_frame frame;

public:
	CommandMessage(int TorqueCommand, int SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, int CommandedTorqueLimit);
	void SetCommandedTorque;
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
	//séed command nao importa por enquanto (soh fazendo o modo de torque aqui!)
	frame.data[2] = 0;
	frame.data[3] = 0;
	//Direction Command
	frame.data[4] = (unsigned char)this->DirectionCommand;
	//Inverter Enable

	frame.data[5] = ((unsigned char)this->InverterEnable) & 0x1 | ((unsigned char)this->InverterDischarge) & 0x1) << 1;

	frame.data[6] = ;
	frame.data[7] = ;

	SetCommandedTorque(CommandedTorque); //TODO precisa disso aqui?

};

void SetCommandedTorque(int Torque){ //TODO a desenvolver
	frame.can_id = COMMAND_MESSAGE;
	frame.data[]; //
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

	bool QuestionSetTorque;
	int Aux;
	int Aux1;
	// Setar torque
	std::cout < <"\nVoce deseja configurar o torque? Digite (1) para SIM ou (0) para NAO"<< std::endl;
	std::cin >> QuestionSetTorque;
	if(QuestionSetTorque){
		std::cout << "Qual o valor do torque desejado? " << std::endl;
		std::cin >> Aux;
		std::cout << "Qual o valor limite para o torque? " << std::endl;
		std::cin >> Aux1;
		CommandMessage ObjCommandMessage(Aux, 0, 1, ?, ?, 0, Aux1);
		//TODO mandar
	}
	else{

	}
	//

	int nbytes = write(s, &frame, sizeof(struct can_frame)); //TODO por que fazer isso?

	nbytes = 0;


	close(s);

	return 0;
}
