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


#define NUM_MSG 1000

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
	static int WriteNegativeValuesTwoBytes(int Value);

};


int NegativeValues::WriteNegativeValuesTwoBytes(int Value){
		if((-32768 <= Value) && (Value <= -1)){
			Value = Value + 65536;
		}
		if((0 <= Value) && (Value <= 32767)){
			Value = Value;
		}
		return Value;
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
public:
	struct can_frame frame;

public:
	CommandMessage(float TorqueCommand, float SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, float CommandedTorqueLimit);
	void SetParameter(int, signed short int);
	int SendFrame(int*);
};

CommandMessage::CommandMessage(float TorqueCommand, float SpeedCommand, bool Direction, bool InverterEnable, bool InverterDischarge, bool SpeedMode, float CommandedTorqueLimit){
	frame.can_id = COMMAND_MESSAGE;
	frame.can_dlc = 8;
	//TODO avaliar se os argumentos estão dentro de seus respectivos limites numéricos

	this->TorqueCommand = static_cast<int>(10*TorqueCommand);
	this->SpeedCommand = static_cast<int>(10*SpeedCommand);
	this->DirectionCommand = Direction;
	this->InverterEnable = InverterEnable;
	this->InverterDischarge = InverterDischarge;
	this->SpeedMode = SpeedMode;
	this->CommandedTorqueLimit = static_cast<int>(10*CommandedTorqueLimit);

	//TorqueCommand
	frame.data[0] = (__u8) (this->TorqueCommand & 0xFF);
	frame.data[1] = (__u8) (this->TorqueCommand >> 8);


	 //SpeedCommand
	frame.data[2] = (__u8) (this->SpeedCommand & 0xFF);
	frame.data[3] = (__u8) (this->SpeedCommand >> 8);


	//DirectionCommand
	frame.data[4] = (__u8)Direction;


	//InverterEnable 
	if(InverterEnable){
		frame.data[5] = (__u8) (frame.data[5] | 0b00000001);
	}
	if(!InverterEnable){
		frame.data[5] = (__u8) (frame.data[5] & 0b11111110);
	}
	
	
	//InverterDischarge
	if(InverterDischarge){
		frame.data[5] = (__u8) (frame.data[5] | 0b00000010);
	}
	if(!InverterDischarge){
		frame.data[5] = (__u8) (frame.data[5] & 0b11111101);
	}


	 //SpeedMode
	if(SpeedMode){ //TODO usar volatile nesse caso?
		frame.data[5] = (__u8) (frame.data[5] | 0b00000010);
	}
	if(!SpeedMode){
		frame.data[5] = (__u8) (frame.data[5] & 0b11111101);
	}

	//CommandedTorqueLimit
	frame.data[6] = (__u8) (this->CommandedTorqueLimit & 0xFF);
	frame.data[7] = (__u8) (this->CommandedTorqueLimit >> 8);

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

int CommandMessage::SendFrame(int* socketCan){
	int nbytes = write(*socketCan, &frame, sizeof(struct can_frame));
	return nbytes;
}



void SetupCanInterface(int* socketCan)
{
	printf("debugging!\n");
	*socketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	struct sockaddr_can addr;

	struct ifreq ifr;

	strcpy(ifr.ifr_name, "can1");

	ioctl(*socketCan, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;

	addr.can_ifindex = ifr.ifr_ifindex;

	bind(*socketCan, (struct sockaddr *) &addr, sizeof(addr));
}


int main(){


	//Configuração do CAN
	//FUNCIONANDO!
	/*int SocketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	struct sockaddr_can addr;

	struct ifreq ifr;

	strcpy(ifr.ifr_name, "can1");

	ioctl(SocketCan, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;

	addr.can_ifindex = ifr.ifr_ifindex;

	bind(SocketCan, (struct sockaddr *) &addr, sizeof(addr));

	struct can_frame frame;*/


	//TESTE DE SETUP DE INTERFACE CAN
	int SocketCan;

	SetupCanInterface(&SocketCan);

	frame.can_dlc = 8;
	//

	bool Question;
	float TorqueValue;
	float TorqueLimitValue;
	float SpeedValue;

	int sentBytes;
	// Setar torque
	while (1){
		std::cout <<"\nVoce deseja configurar o torque limite? Digite (1) para prosseguir com a configuração ou (0) para fechar o software. "<< std::endl;
		std::cin >> Question;
		if(Question){
			std::cout << "Qual o valor limite para o torque? " << std::endl;
			std::cin >> TorqueLimitValue;
			std::cout << "Qual o valor para o torque? " << std::endl;
			std::cin >> TorqueValue;
			std::cout << "Qual o valor da velocidade? " << std::endl;
			std::cin >> SpeedValue;
			CommandMessage ObjCommandMessage(TorqueValue, SpeedValue, 1, 0, 0, 0, TorqueLimitValue); //
			sentBytes = ObjCommandMessage.SendFrame(&SocketCan);
		}
		else{ // Programa será fechado
			close(SocketCan);
			return 0;
		}
	}


}

