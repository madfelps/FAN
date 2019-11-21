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


#define NUM_MSG 100

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
	static float NegativeValuesTwoBytes(int Value);

};


float NegativeValues::NegativeValuesTwoBytes(float Value){ // segundo o manual, se o valor for maior que 32768, isso indica um numero negativo; logo, deve-se fazer a conversao apropriada
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
	static float ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte);

};

float Torque::ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float TorqueValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	TorqueValue = NegativeValuesTwoBytes(TorqueValue);
	TorqueValue = TorqueValue/10;
	return TorqueValue;
}

class Angle:public NegativeValues{
private:

public:
	static float ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte);
};



float Angle::ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleValue = NegativeValuesTwoBytes(AngleValue);
	AngleValue = AngleValue/10;
	return AngleValue;

		/*if(!((*AngleValue >= -359.9) && (*AngleValue <= 359.9))){ // TODO tratamento de erros (a desenvolver)
			throw 1;
		}*/
}

class AngleVelocity:public NegativeValues{
private:

public:
	static float ProcessAngleVelocity(unsigned char*, int MSByte, int LSByte);
};

float AngleVelocity::ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleVelocityValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
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

	float MotorAngleProcessed;
	float MotorSpeedProcessed;
	float ElectricalOutFreqProcessed;
	float DeltaResolverFilteredProcessed;	

public:

	MotorPosInfo(unsigned char*);
	MotorPosInfo();

	float GetMotorAngleProcessed();
	float GetMotorSpeedProcessed();
	float GetElectricalOutFreqProcessed();
	float GetDeltaResolverFilteredProcessed();

};

MotorPosInfo::MotorPosInfo(){
	MotorAngle             = 0;
	MotorSpeed             = 0;
	ElectricalOutFreq      = 0;
	DeltaResolverFiltered  = 0;
};

MotorPosInfo::MotorPosInfo(unsigned char* CAN_DATA){
	MotorAngle             			= 0;
	MotorAngleProcessed             = ProcessAngle(CAN_DATA, 1, 0);
	MotorSpeed            			= 0;
	MotorSpeedProcessed             = ProcessAngleVelocity(CAN_DATA, 3, 2);
	ElectricalOutFreq      			= 0;
	DeltaResolverFiltered  			= 0;
};

float MotorPosInfo::GetMotorAngleProcessed(){
	return MotorAngle;
}

float MotorPosInfo::GetMotorSpeedProcessed(){
	return MotorSpeed;
}

float MotorPosInfo::GetElectricalOutFreqProcessed(){
	return ElectricalOutFreq;
}

float MotorPosInfo::GetDeltaResolverFilteredProcessed(){
	return DeltaResolverFiltered;
}


class TorqueTimerInfo: public Torque{
private:
	int CommandedTorque;
	int TorqueFeedback;
	int PowerOnTime;

	float CommandedTorqueProcessed;
	float TorqueFeedbackProcessed;
	float PowerOnTimeProcessed;


public:
	TorqueTimerInfo(unsigned char*);
	TorqueTimerInfo();

	int GetCommandedTorqueProcessed();
	int GetTorqueFeedbackProcessed();
	int GetPowerOnTimeProcessed();

};

TorqueTimerInfo::TorqueTimerInfo(){
	CommandedTorque     = 0;
	TorqueFeedback     = 0;
	PowerOnTime         = 0;

}

TorqueTimerInfo::TorqueTimerInfo(unsigned char* CAN_DATA){
	CommandedTorqueProcessed     = ProcessTorque(CAN_DATA, 1, 0);
	TorqueFeedbackProcessed      = ProcessTorque(CAN_DATA, 3, 2);
	PowerOnTimeProcessed         = 0;

}

float TorqueTimerInfo::GetCommandedTorqueProcessed(){
	return CommandedTorqueProcessed;
}

float TorqueTimerInfo::GetTorqueFeedbackProcessed(){
	return TorqueFeedbackProcessed;
}

float TorqueTimerInfo::GetPowerOnTimeProcessed(){
	return PowerOnTimePRocessed;
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
};

CommandMessage::CommandMessage(int TorqueCommand, int SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, int CommandedTorqueLimit){
	frame.can_id = COMMAND_MESSAGE;

	this->TorqueCommand = TorqueCommand;
	this->SpeedCommand = SpeedCommand;
	this->DirectionCommand = DirectionCommand;
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


	//InverterEnable and InverterDischarge
	frame.data[5] = ((unsigned char)InverterEnable) & 0x1;


	//InverterDischarge
	frame.data[5] = ((unsigned char)InverterDischarge) & 0x1 << 1;


	 //SpeedMode
	frame.data[6] = SpeedMode;


	//CommandedTorqueLimit
	frame.data[7] = CommandedTorqueLimit;


}


};

void CommandMessage::SetParameter(int Value, signed short int flag){ // TODO A DESENVOLVER
	frame.can_id = COMMAND_MESSAGE;
	if(flag == 0){ //TorqueCommand
		TorqueCommand = Value;
		frame.data[0] = TorqueCommand & 0xFF;
		frame.data[1] = TorqueCommand >> 8;

	}
	if(flag == 1){ //SpeedCommand
		SpeedCommand = Value;
		frame.data[2] = SpeedCommand;
		frame.data[3] = SpeedCommand;
		
	}
	if(flag == 2){//DirectionCommand
		DirectionCommand = (unsigned char)Value;
		frame.data[4] = DirectionCommand;
		
	}
	if(flag == 3){ //InverterEnable and InverterDischarge
		InverterEnable = ((unsigned char)Value) & 0x1;
		frame.data[5] = ((unsigned char)Value) & 0x1;
		
	}
	if(flag == 4){ //InverterDischarge
		InverterDischarge = ((unsigned char)Value) & 0x1 << 1;
		frame.data[5] = ((unsigned char)Value) & 0x1 << 1;
		
	}
	if(flag == 5){ //SpeedMode
		SpeedMode = Value;
		frame.data[6] = SpeedMode;
		
	}
	if(flag == 6){ //CommandedTorqueLimit
		CommandedTorqueLimit = Value;
		frame.data[7] = CommandedTorqueLimit;
		
	}

}


int main(){

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


	int nbytes = 0;

	int MsgCounter = 0;
	int i, j;
	int CounterMotorPosition = 0;
	int CounterTorqueTimerInfo = 0;

	while (MsgCounter < NUM_MSG) {
		nbytes = read(SocketCan, &frame, sizeof(struct can_frame));
		printf("qtdd msgs: %d\n", MsgCounter);
		if(nbytes != 0){ // se realmente leu msg...
			for(j = 0; j < frame.can_dlc; j++){
				DataLog[MsgCounter][j] = frame.data[j];
			}

			DataID[MsgCounter] = frame.can_id;

			//if(MsgCounter%100 == 0){ // imprime a cada 100 mensagens
				for (i = 0; i < 8; i++) {
					printf("%5d", DataLog[MsgCounter][i]);
				}

				std::cout << "  |  ";

				if(frame.can_id == MOTOR_POSITION){
					CounterMotorPosition++;
					//if(CounterMotorPosition == 100){
						ObjMotorPosInfo = MotorPosInfo(frame.data); //TODO reseta os parametros para zero e depois processa os de interesse
						std::cout << "Angle: " << ObjMotorPosInfo.GetMotorAngle() << " ";
						std::cout << "Speed: " << ObjMotorPosInfo.GetMotorSpeed() << std::endl;
						CounterMotorPosition = 0;
					//}
				}
				else if(frame.can_id == TORQUE_TIMER_INFO){
					CounterTorqueTimerInfo++;
					//if(CounterTorqueTimerInfo == 100){
						ObjTorqueTimerInfo = TorqueTimerInfo(frame.data);
						std::cout << "Torque Feedback: " << ObjTorqueTimerInfo.GetTorqueFeedbackProcessed() << " ";
						std::cout << "Commanded Torque: " << ObjTorqueTimerInfo.GetCommandedTorqueProcessed() << std::endl;
						CounterTorqueTimerInfo = 0;
					//}
				}
			//}

			MsgCounter++;
		}

	}

	close(SocketCan);

	return 0;
}

