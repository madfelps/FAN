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

int bytes; // variavel global auxiliar para realizar o envio do canframe (write)

struct can_frame frame;


class NegativeValues{
private:

public:
	static int NegativeValuesTwoBytes(int Value);

};


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
<<<<<<< Updated upstream
	Torque();
	int ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte);
	int ProcessTorqueInvMSB(int Value); 
	int ProcessTorqueInvLSB(int Value);
=======
	static int ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte);
>>>>>>> Stashed changes

};

int Torque::ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte){
	int TorqueValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	TorqueValue = NegativeValuesTwoBytes(TorqueValue);
	TorqueValue = TorqueValue/10;
	return TorqueValue;
}

int Torque::ProcessTorqueInvMSB(int Value){
	return (Value&0xFF00)>>8;
}

int Torque::ProcessTorqueInvLSB(int Value){
	return Value&0x00FF;
}

class Angle:public NegativeValues{
private:

public:
	int ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte);
};


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
	int ProcessAngleVelocity(unsigned char*, int MSByte, int LSByte);
};


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
	MotorPosInfo();
	MotorPosInfo(unsigned char*);
	int GetMotorAngle();
	int GetMotorSpeed();
	int GetElectricalOutFreq();
	int GetDeltaResolverFiltered();

};

MotorPosInfo::MotorPosInfo(){
	MotorAngle             = 0;
	MotorAngle             = 0;
	MotorSpeed             = 0;
	MotorSpeed             = 0;
	ElectricalOutFreq      = 0;
	DeltaResolverFiltered  = 0;
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
	TorqueTimerInfo();
	TorqueTimerInfo(unsigned char*);
	int GetCommandedTorque();
	int GetTorqueFeedback();
	int GetPowerOnTime();

};

TorqueTimerInfo::TorqueTimerInfo(){
	CommandedTorque     = 0;
	CommandedTorque     = 0;
	TorqueFeedback      = 0;
	TorqueFeedback      = 0;
	PowerOnTime         = 0;

}

TorqueTimerInfo::TorqueTimerInfo(unsigned char* CAN_DATA){
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
	int TorqueCommand;  //FLAG 0
	int SpeedCommand; //FLAG 1
	bool DirectionCommand; //FLAG 2
	bool InverterEnable; //FLAG 3
	bool InverterDischarge; //FLAG 4
	bool SpeedMode; //FLAG 5
	int CommandedTorqueLimit; //FLAG 6
	struct can_frame frame;

public:
	CommandMessage(int TorqueCommand, int SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, int CommandedTorqueLimit);
	void SetParameter(int, signed short int);
	void SendFrame(); //TODO Desenvolver essa função para envio do frame pelo CAN ao motor 

};

CommandMessage::CommandMessage(int TorqueCommand, int SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, int CommandedTorqueLimit){
	frame.can_id = COMMAND_MESSAGE;

	this->TorqueCommand = TorqueCommand;
	this->SpeedCommand = SpeedCommand;
	this->DirectionCommand = Direction;
	this->InverterEnable = InverterEnable;
	this->InverterDischarge = InverterDischarge;
	this->SpeedMode = SpeedMode;
	this->CommandedTorqueLimit = CommandedTorqueLimit;

	//TorqueCommand
	frame.data[0] = TorqueCommand & 0xFF;
	frame.data[1] = TorqueCommand >> 8;


	 //SpeedCommand
	frame.data[2] = SpeedCommand;
	frame.data[3] = SpeedCommand;


	{//DirectionCommand
	frame.data[4] = (unsigned char)Direction;


	//InverterEnable 
	if(InverterEnable){
		frame.data[5] = frame.data[5] | 0b00000001;
	}
	if(!InverterEnable){
		frame.data[5] = frame.data[5] & 0b11111110;
	}
	
	
	//InverterDischarge
	if(InverterDischarge){
		frame.data[5] = frame.data[5] | 0b00000010;
	}
	if(!InverterDischarge){
		frame.data[5] = frame.data[5] & 0b11111101;
	}


	 //SpeedMode
	if(SpeedMode){ //TODO usar volatile nesse caso?
		frame.data[5] = frame.data[5] | 0b00000010;
	}
	if(!SpeedMode){
		frame.data[5] = frame.data[5] & 0b11111101;
	}


	//CommandedTorqueLimit
	frame.data[6] = CommandedTorqueLimit & 0xFF;
	frame.data[7] = CommandedTorqueLimit >> 8;


}


};

void CommandMessage::SetParameter(int Value, signed short int flag){ //
	frame.can_id = COMMAND_MESSAGE;
	
	if(flag == 0){ //TorqueCommand
		TorqueCommand = (unsigned char)Value;
		frame.data[0] = TorqueCommand & 0xFF;
		frame.data[1] = TorqueCommand >> 8;

	}
	if(flag == 1){ //SpeedCommand
		SpeedCommand = (unsigned char)Value;
		frame.data[2] = SpeedCommand & 0xFF;
		frame.data[3] = SpeedCommand >> 8;

	}
	if(flag == 2){//DirectionCommand
		DirectionCommand = (unsigned char)Value;
		frame.data[4] = DirectionCommand;

	}
	if(flag == 3){ //InverterEnable
		InverterEnable = ((unsigned char)Value) & 0x1; 
		if(InverterEnable){
			frame.data[5] = frame.data[5] | 0b00000001;
		}
		if(!InverterEnable){
			frame.data[5] = frame.data[5] & 0b11111110;
		}
	}
	if(flag == 4){ //InverterDischarge
		InverterDischarge = ((unsigned char)Value);
		if(InverterDischarge){
			frame.data[5] = frame.data[5] | 0b00000010;
		}
		if(!InverterDischarge){
			frame.data[5] = frame.data[5] & 0b11111101;
		}

	}
	if(flag == 5){ //SpeedMode
		SpeedMode = Value & 0x1;
		if(SpeedMode){ //TODO usar volatile nesse caso?
			frame.data[5] = frame.data[5] | 0b00000010;
		}
		if(!SpeedMode){
			frame.data[5] = frame.data[5] & 0b11111101;
		}
	}
	if(flag == 6){ //CommandedTorqueLimit
		CommandedTorqueLimit = Value;
		frame.data[6] = CommandedTorqueLimit & 0xFF;
		frame.data[7] = CommandedTorqueLimit >> 8;

	}

}

void CommandMessage::SendFrame(struct canFrame); //TODO eu coloco o frame como variavel global?
	int nbytes = write(s, &frame, sizeof(struct can_frame));



int main(){

	int MessageSend;
	char DataLog[NUM_MSG][8];
	unsigned int DataID[NUM_MSG];
	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;


	//Configuração do CAN
	int SocketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	struct sockaddr_can addr;

	struct ifreq ifr;

	strcpy(ifr.ifr_name, "can1");

	ioctl(SocketCan, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;

	addr.can_ifindex = ifr.ifr_ifindex;

	bind(SocketCan, (struct sockaddr *) &addr, sizeof(addr));

	struct can_frame frame;

	frame.can_dlc = 8;
	//

	bool Question;
	int TorqueValue;
	int TorqueLimitValue;
	// Setar torque
	std::cout < <"\nVoce deseja configurar o torque limite? Digite (1) para SIM ou (0) para NAO"<< std::endl;
	std::cin >> Question;
	if(Question){
		std::cout << "Qual o valor limite para o torque? " << std::endl;
		std::cin >> TorqueLimitValue;
		std::cout << "Qual o valor para o torque? " << std::endl;
		std::cin << TorqueValue;
		CommandMessage ObjCommandMessage(TorqueValue, 0, 1, 0, 0, 0, TorqueLimitValue); //
		//TODO mandar
	}
	else{ // Programa será fechado
		return 0;
	}

	int MessageSend = write(SocketCan, &frame, sizeof(struct can_frame));

	MessageSend = 0;

	while (1){
		std::cout<<".... Voce deseja fechar o software? ";
		std::cin>>Question;
		if(Question){
			return 0;
		}
	}

	close (SocketCan);


	close(s);
