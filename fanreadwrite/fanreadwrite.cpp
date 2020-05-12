//============================================================================
// Name        : main.cpp
// Author      : Felipe Moura Madureira e Henrique Borges Garcia
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

void DescreveSensor(char StringDescreveSensor[][50]){
	strcpy(StringDescreveSensor[0], "Angle: ");
	strcpy(StringDescreveSensor[1], "Speed: ");
	strcpy(StringDescreveSensor[2], "Torque Command: ");
	strcpy(StringDescreveSensor[3], "TorqueFeedback: ");
	strcpy(StringDescreveSensor[4], "PowerOnTime: ");
	strcpy(StringDescreveSensor[5], "Temperatura do MóduloA: ");
	strcpy(StringDescreveSensor[6], "Temeperatura do MóduloB: ");
	strcpy(StringDescreveSensor[7], "Temperatura do MóduloC: ");
	strcpy(StringDescreveSensor[8], "Temperatura do GateDriverBoard: ");
	strcpy(StringDescreveSensor[9], "Instante da geração dos dados: ");
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
	static float ProcessTorqueReceive(unsigned char* CAN_DATA, int MSByte, int LSByte);

};

// Função: Processar determinado dado para seguir o padrão do Torque, conforme manual(lembrando que o valor recebido é multiplicado por 10).
// Parâmetros: Payload do CAN e o número do byte mais significativo e menos significativo para extrair o valor.
// Retorno: Valor processado.
float Torque::ProcessTorqueReceive(unsigned char* CAN_DATA, int MSByte, int LSByte){
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
	printw("Angle: %f\n", this->GetMotorAngleProcessed());
	move(15, 60);
	printw("Speed: %f\n", this->GetMotorSpeedProcessed());
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
	CommandedTorqueProcessed     = this->ProcessTorqueReceive(CAN_DATA, 1, 0);
	TorqueFeedbackProcessed      = this->ProcessTorqueReceive(CAN_DATA, 3, 2);
	PowerOnTimeProcessed         = 0;
}


void TorqueTimerInfo::ShowAllValuesProcessed(){
		move(11, 60);
		printw("Commanded Torque: %f\n", this->GetCommandedTorqueProcessed());
		move(12, 60);
		printw("Torque Feedback: %f\n", this->GetTorqueFeedbackProcessed());
		move(13, 60);
		printw("Power On Time: %f\n", this->GetPowerOnTimeProcessed());
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
	ModuleAProcessed 			= ProcessTorqueReceive(CAN_DATA, 1, 0);
	ModuleBProcessed 			= ProcessTorqueReceive(CAN_DATA, 3, 2);
	ModuleCProcessed 			= ProcessTorqueReceive(CAN_DATA, 5, 4);
	GateDriverBoardProcessed 	= ProcessTorqueReceive(CAN_DATA, 7, 6);
}

void Temperature1::UpdateObject(unsigned char* CAN_DATA){
		ModuleAProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 1, 0);
		ModuleBProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 3, 2);
		ModuleCProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 5, 4);
		GateDriverBoardProcessed 	= this->ProcessTorqueReceive(CAN_DATA, 7, 6);
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
		printw("Temperatura do MóduloA: %f\n", this->GetModuleAProcessed());
		move(11, 5);
		printw("Temperatura do MóduloB: %f\n", this->GetModuleBProcessed()); 
		move(12, 5);
		printw("Temperatura do MóduloC: %f\n", this->GetModuleCProcessed());
		move(13, 5);
		printw("Temperatura do Gate Driver Board: %f\n", this->GetGateDriverBoardProcessed());
		refresh();
}

void  Temperature1::IfID_Temperature1(struct can_frame* frame){
	if(frame->can_id == TEMPERATURES_1){


		this->UpdateObject(frame->data);

		this->ShowAllValuesProcessed();


	}
}

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
	float CommandedTorqueLimit;
public:
	CommandMessage();
	void UpdateFrame();
	void ProcessTorqueSend(float* TorqueCommand);
	void UpdateFrame(struct can_frame* frame);

};

CommandMessage::CommandMessage(){

}

void CommandMessage::ProcessTorqueSend(float* Torque){
	TorqueCommand =  (int) (*Torque)*10;
	if(TorqueCommand < 32768){
		TorqueCommandMSByte = 0;
		TorqueCommandLSByte = TorqueCommand;
	}
	if(TorqueCommand >= 32768){
		TorqueCommandLSByte = (TorqueCommand & 0xFF); //faz sentido isso?
		TorqueCommandMSByte = (TorqueCommand >> 8); //e isso?
	}
}

void CommandMessage::UpdateFrame(struct can_frame* frame){
	frame->data[0] = TorqueCommandLSByte;
	frame->data[1] = TorqueCommandMSByte;

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


int main()
{

	float TorquePretendido;

	char TempoEmString[10];

	clock_t GuardaIntervaloTempo;
	clock_t Temporaria;

	FILE* Arquivo;
	Arquivo = fopen("log.txt", "a");

	char StringDescreveSensor[11][50];
	char StringGuardaDadosSensor[10][10];
	initscr();
	rectangle(9, 4, 14, 50);
	rectangle(9, 55, 15, 85);

	char DataLog[NUM_MSG][8];
	unsigned int DataID[NUM_MSG];

	//Declaração dos Objetos
	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;
	Temperature1 ObjTemperature1;
	CommandMessage ObjCommandMessage;

	//Configuração do CAN
	int SocketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	struct sockaddr_can addr;
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "can1");
	ioctl(SocketCan, SIOCGIFINDEX, &ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(SocketCan, (struct sockaddr *) &addr, sizeof(addr));
	struct can_frame frameRead, frameWrite; //Criação dos frames
	frameRead.can_dlc = 8;
	frameWrite.can_dlc = 8;
	//


	int nbytes = 0;

	int MsgCounter = 0;
	int i, j;
	int CounterMotorPosition = 0;
	int CounterTorqueTimerInfo = 0;
	

	#pragma omp parallel
	{

		#pragma omp sections
		{

		 	#pragma omp section //TASK READ 
		 	{ 
		 		MsgCounter++; //faz sentido colocar o MsgCounter aqui?

				while (MsgCounter < NUM_MSG) 
				{
				#pragma omp critical (mutex)
				{	
				nbytes = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				}
					if(nbytes != 0){ // Verifica se a mensagem foi lida


						GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead);
						ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead);
						ObjTemperature1.IfID_Temperature1(&frameRead);
						//Guarda dados dos sensores na string
						sprintf(StringGuardaDadosSensor[0], "%f", ObjMotorPosInfo.GetMotorAngleProcessed());
						sprintf(StringGuardaDadosSensor[1], "%f", ObjMotorPosInfo.GetMotorSpeedProcessed());

						sprintf(StringGuardaDadosSensor[2], "%f", ObjTorqueTimerInfo.GetCommandedTorqueProcessed());
						sprintf(StringGuardaDadosSensor[3], "%f", ObjTorqueTimerInfo.GetTorqueFeedbackProcessed());
						sprintf(StringGuardaDadosSensor[4], "%f", ObjTorqueTimerInfo.GetPowerOnTimeProcessed());

						sprintf(StringGuardaDadosSensor[5], "%f", ObjTemperature1.GetModuleAProcessed());
						sprintf(StringGuardaDadosSensor[6], "%f", ObjTemperature1.GetModuleBProcessed());
						sprintf(StringGuardaDadosSensor[7], "%f", ObjTemperature1.GetModuleCProcessed());
						sprintf(StringGuardaDadosSensor[8], "%f", ObjTemperature1.GetGateDriverBoardProcessed());
						//}


						//Pega o instante de tempo da geração dos dados, transforma em string e guarda na variável StringDescreveSensor.
						Temporaria = clock() - GuardaIntervaloTempo;
						strcpy(StringDescreveSensor[9], "Instante da geração dos dados (em segundos): ");
						sprintf(TempoEmString, "%lf", (double) Temporaria*10000/CLOCKS_PER_SEC);
						strcat(StringDescreveSensor[9], TempoEmString);

						//Concatena os valores na string
						strcat(StringDescreveSensor[0], StringGuardaDadosSensor[0]);
						strcat(StringDescreveSensor[1], StringGuardaDadosSensor[1]);
						strcat(StringDescreveSensor[2], StringGuardaDadosSensor[2]);
						strcat(StringDescreveSensor[3], StringGuardaDadosSensor[3]);
						strcat(StringDescreveSensor[4], StringGuardaDadosSensor[4]);
						strcat(StringDescreveSensor[5], StringGuardaDadosSensor[5]);
						strcat(StringDescreveSensor[6], StringGuardaDadosSensor[6]);
						strcat(StringDescreveSensor[7], StringGuardaDadosSensor[7]);
						strcat(StringDescreveSensor[8], StringGuardaDadosSensor[8]);
				

						//Gera e escreve o log
						fprintf(Arquivo, "%s\n", StringDescreveSensor[0]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[1]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[2]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[3]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[4]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[5]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[6]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[7]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[8]);
						fprintf(Arquivo, "%s\n", StringDescreveSensor[9]);
						fprintf(Arquivo, "%s\n", "-----------------------------------------------------------------------");

						MsgCounter = 0;
					}
				}
			}

			#pragma omp section //TASK WRITE
			{ 
				move(25, 5); //Aqui altera o cursor para setar o valor do torque
				printw("Digite o valor desejado de Torque\n");
				scanf("%f", &TorquePretendido);
				ObjCommandMessage.ProcessTorqueSend(&TorquePretendido);
				ObjCommandMessage.UpdateFrame(&frameWrite);
				#pragma omp critical (mutex)
				{
				nbytes = write(SocketCan, &frameWrite, sizeof(struct can_frame));
				}



			}

		}
	}

	endwin();

	close(SocketCan);
	fclose(Arquivo);

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

