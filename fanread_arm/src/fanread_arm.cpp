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
#include <ncurses.h>
#include <time.h>

void rectangle(int y1, int x1, int y2, int x2);

void DescreveSensor(){
	strcpy(StringDescreveSensor[0], "Angle: ");
	strcpy(StringDescreveSensor[1], "Speed: ");
	strcpy(StringDescreveSensor[2], "Torque Command: ");
	strcpy(StringDescreveSensor[3], "TorqueFeedback: ");
	strcpy(StringDescreveSensor[4], "PowerOnTime: ");
	strcpy(StringDescreveSensor[5], "Temperatura do MóduloA: ");
	strcpy(StringDescreveSensor[6], "Temeperatura do MóduloB: ");
	strcpy(StringDescreveSensor[7], "Temperatura do MóduloC: ");
	strcpy(StringDescreveSensor[8], "Temperatura do GateDriverBoard: ");
}

void ColocaDadosSensoresEmString(){

}

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
	TorqueValue = TorqueValue/10; //todo O problema do multiplicador
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
	void UpdateObject(unsigned char* CAN_DATA);
	void ShowAllValuesProcessed();
	void IfID_MotorPosInfo(struct can_frame* frame);

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

void MotorPosInfo::UpdateObject(unsigned char* CAN_DATA){
	MotorAngle             			= 0;
	MotorAngleProcessed             = this->ProcessAngle(CAN_DATA, 1, 0);
	MotorSpeed            			= 0;
	MotorSpeedProcessed             = this->ProcessAngleVelocity(CAN_DATA, 3, 2);
	ElectricalOutFreq      			= 0;
	DeltaResolverFiltered  			= 0;
}

void MotorPosInfo::ShowAllValuesProcessed(){
	move(14, 60);
	printw("Angle: " ); this->GetMotorAngleProcessed() << " ";
	move(15, 60);
	printw("Speed: " ); this->GetMotorSpeedProcessed() << std::endl;
	refresh();
}

void MotorPosInfo::IfID_MotorPosInfo(struct can_frame* frame){
	if(frame->can_id == TORQUE_TIMER_INFO){


		this->UpdateObject(frame->data);

		this->ShowAllValuesProcessed();


	}
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
	TorqueTimerInfo();
	void UpdateObject(unsigned char*);
	void ShowAllValuesProcessed();
	void IfID_TorqueTimerInfo(struct can_frame* frame);

	float GetCommandedTorqueProcessed();
	float GetTorqueFeedbackProcessed();
	float GetPowerOnTimeProcessed();

};

TorqueTimerInfo::TorqueTimerInfo(){
	CommandedTorque     = 0;
	TorqueFeedback      = 0;
	PowerOnTime         = 0;

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

void TorqueTimerInfo::UpdateObject(unsigned char* CAN_DATA){
	CommandedTorqueProcessed     = this->ProcessTorque(CAN_DATA, 1, 0);
	TorqueFeedbackProcessed      = this->ProcessTorque(CAN_DATA, 3, 2);
	PowerOnTimeProcessed         = 0;
}


void TorqueTimerInfo::ShowAllValuesProcessed(){
		move(11, 60);
		printw("TorqueCommand: " ); this->GetCommandedTorqueProcessed() << std::endl;
		move(12, 60);
		printw("Torque Feedback: "); this->GetTorqueFeedbackProcessed() << std::endl;
		move(13, 60);
		printw("Power On Time: " ); this->GetPowerOnTimeProcessed() << std::endl;
		refresh();
}

void TorqueTimerInfo::IfID_TorqueTimerInfo(struct can_frame* frame){
	if(frame->can_id == MOTOR_POSITION){


		this->UpdateObject(frame->data);

		this->ShowAllValuesProcessed();


	}
}

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
	void  IfID_Temperature1(struct can_frame* frame);
	void ShowAllValuesProcessed();


};

Temperature1::Temperature1(){
	ModuleA 					= 0;
	ModuleAProcessed 			= 0;
	ModuleB 					= 0;
	ModuleBProcessed 			= 0;
	ModuleC 					= 0;
	ModuleCProcessed 			= 0;
	GateDriverBoard 			= 0;
	GateDriverBoardProcessed 	= 0;
}

Temperature1::Temperature1(unsigned char* CAN_DATA){
	ModuleAProcessed 			= ProcessTorque(CAN_DATA, 1, 0);
	ModuleBProcessed 			= ProcessTorque(CAN_DATA, 3, 2);
	ModuleCProcessed 			= ProcessTorque(CAN_DATA, 5, 4);
	GateDriverBoardProcessed 	= ProcessTorque(CAN_DATA, 7, 6);
}

void Temperature1::UpdateObject(unsigned char* CAN_DATA){
		ModuleAProcessed 			= this->ProcessTorque(CAN_DATA, 1, 0);
		ModuleBProcessed 			= this->ProcessTorque(CAN_DATA, 3, 2);
		ModuleCProcessed 			= this->ProcessTorque(CAN_DATA, 5, 4);
		GateDriverBoardProcessed 	= this->ProcessTorque(CAN_DATA, 7, 6);
}

float Temperature1::GetModuleAProcessed(){
	return ModuleAProcessed;
}

float Temperature1::GetModuleBProcessed(){
	return ModuleBProcessed;
}

float Temperature1::GetModuleCProcessed(){
	return ModuleCProcessed;
}

float Temperature1::GetGateDriverBoardProcessed(){
	return GateDriverBoardProcessed;
}

void Temperature1::ShowAllValuesProcessed(){
		move(10, 5);
		printw("Temperatura do MóduloA: "); this->GetModuleAProcessed() << std::endl;
		move(11, 5);
		printw("Temperatura do MóduloB: "); this->GetModuleBProcessed() << std::endl;
		move(12, 5);
		printw("Temperatura do MóduloC: "); this->GetModuleCProcessed() << std::endl;
		move(13, 5);
		printw("Temperatura do Gate Driver Board: " ); this->GetGateDriverBoardProcessed() << std::endl;
		refresh();
}

void  Temperature1::IfID_Temperature1(struct can_frame* frame){
	if(frame->can_id == TEMPERATURES_1){


		this->UpdateObject(frame->data);

		this->ShowAllValuesProcessed();


	}
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


	char StringDescreveSensor[10][50];
	char StringGuardaDadosSensor[10][10];
	initscr();
	rectangle(9, 4, 14, 50);
	rectangle(9, 55, 15, 85);

	char DataLog[NUM_MSG][8];
	unsigned int DataID[NUM_MSG];
	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;
	Temperature1 ObjTemperature1;

	int a;
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
		//printf("qtdd msgs: %d\n", MsgCounter);
		if(nbytes != 0){ // Verifica se a mensagem foi lida
			for(j = 0; j < frame.can_dlc; j++){
				DataLog[MsgCounter][j] = frame.data[j];
			}

			DataID[MsgCounter] = frame.can_id;

			//if(MsgCounter%100 == 0){ // imprime a cada 100 mensagens
				/*for (i = 0; i < 8; i++) {
					printf("%5d", DataLog[MsgCounter][i]);
				}*/

				//std::cout << "  |  ";

				ObjMotorPosInfo.IfID_MotorPosInfo(&frame);
				ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frame);
				ObjTemperature1.IfID_Temperature1(&frame);
				//Guarda dados dos sensores na string
				sprintf(numero[0], "%i", );
			//}

			MsgCounter++;
		}

	}

	endwin();

	close(SocketCan);

	return 0;
}

void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

