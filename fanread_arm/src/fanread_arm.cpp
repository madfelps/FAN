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


/* Conforme o manual RMS Can Protocol na página 12, algumas grandezas cujos valores ocupam 2 (dois) bytes devem passar por processamento para distinguirmos os números positivos e os negativos.  */

public:
	static float NegativeValuesTwoBytes(float Value);

};

/* Como 2^16 = 65536, dividimos pela metade, e, valores abaixo de 65536/2 = 32768 são convencionados como positivos, enquanto valores acima de 32768 (até 65536) são negativos.  */
/* Para entender a conversão, ver anexo 1 da documentação desse projeto.  */

// Função: Processar determinado dado para verificar se é positivo ou negativo
// Parâmetros: Valor a ser verificado e processado.
// Retorno: Valor processado (com correta indicação se é positivo ou negativo).
float NegativeValues::NegativeValuesTwoBytes(float Value){
		if(Value >= 32768){
			Value = Value - 65536;
		}
		else{
		}
		return Value;
}

/* Em grandezas compostas de dois bytes temos que o valor referido é a soma do valor do byte menos significativo (LSB) com o byte mais significativo (MSB) multiplicado por 256. (Ver Manual)  */

class Torque:public NegativeValues{
private:

public:
	static float ProcessTorque(unsigned char* CAN_DATA, int MSByte, int LSByte);

};

// Função: Processar determinado dado para seguir o padrão do Torque, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
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


// Função: Processar determinado dado para seguir o padrão do Angle, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
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

// Função: Processar determinado dado para seguir o padrão do AngleVelocity, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
float AngleVelocity::ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleVelocityValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleVelocityValue = NegativeValuesTwoBytes(AngleVelocityValue);
	AngleVelocityValue = AngleVelocityValue/10;
	return AngleVelocityValue;
}

/* Todas as classes com respectiva seção no manual possui objetos que guardam os valores respectivos "crus", advindo do envio de dados pelo motor. Esses dados são armazenados no objeto de nome com a grandeza relacionada. */
/* Finalizado o processamento do dado (para numeros negativos e peso dos bytes), o valor "real" é armazenado no objetivo com terminação Processed.  */

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
	return MotorAngleProcessed;
}

float MotorPosInfo::GetMotorSpeedProcessed(){
	return MotorSpeedProcessed;
}

float MotorPosInfo::GetElectricalOutFreqProcessed(){
	return ElectricalOutFreqProcessed;
}

float MotorPosInfo::GetDeltaResolverFilteredProcessed(){
	return DeltaResolverFilteredProcessed;
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

	float GetCommandedTorqueProcessed();
	float GetTorqueFeedbackProcessed();
	float GetPowerOnTimeProcessed();

};

TorqueTimerInfo::TorqueTimerInfo(){
	CommandedTorque     = 0;
	TorqueFeedback      = 0;
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
	return PowerOnTimeProcessed;
}

void SetupCanInterface(int* socketCan)
{
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
		nbytes = read(SocketCan, &frame, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
		printf("qtdd msgs: %d\n", MsgCounter);
		if(nbytes != 0){ // Verifica se a mensagem foi lida
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
						ObjMotorPosInfo = MotorPosInfo(frame.data);
						std::cout << "Angle: " << ObjMotorPosInfo.GetMotorAngleProcessed() << " ";
						std::cout << "Speed: " << ObjMotorPosInfo.GetMotorSpeedProcessed() << std::endl;
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

