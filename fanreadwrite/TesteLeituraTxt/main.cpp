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
#include <fstream>
#include <omp.h>
#include "lertxt.h"

#define    NUM_MSG 4

#define    UNDEFINED		         0
#define    TEMPERATURES_1 	         0x0A0	//CHECK
#define    TEMPERATURES_2	         0x0A1
#define    TEMPERATURES_3 	         0x0A2
#define    ANALOGIC_IN 	             0x0A3
#define    DIGITAL_IN		         0x0A4
#define    MOTOR_POSITION	         0x0A5	//CHECK
#define    CURRENT_INFO	             0x0A6
#define    VOLTAGE_INFO	             0x0A7
#define    FLUX_INFO   	             0x0A8
#define    INTERN_VOLTS 	         0x0A9
#define    INTERN_STATES	         0x0AA	//CHECK
#define    FAULT_CODES 	             0x0AB
#define    TORQUE_TIMER_INFO	     0x0AC	//CHECK
#define    MOD_FLUX_WEAK_OUT_INFO	 0x0AD
#define    FIRM_INFO	             0x0AE
#define    DIAGNOSTIC_DATA	         0x0AF
#define    COMMAND_MESSAGE           0x0C0	//CHECK


int main()
{

	float TorquePretendido;
	float TorqueLimit;

	char TempoEmString[10];

	clock_t GuardaIntervaloTempo;
	clock_t Temporaria;

	FILE* Arquivo;
	Arquivo = fopen("log.txt", "a");

	char StringDescreveSensor[11][50];
	char StringGuardaDadosSensor[10][10];


	//Declaração dos Objetos
	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;
	Temperature1 ObjTemperature1;
	CommandMessage ObjCommandMessage;
	InternalStates ObjInternalStates;

	//Configuração do CAN
	/*
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
	*/
	//

	struct can_frame frameRead, frameWrite; //Criação dos frames
	frameRead.can_dlc = 8;

	//Configuração do arquivo para teste
	std::ifstream listaCAN("listaCAN.txt");
	std::string auxStr;
	int wordCounter = 0;


	int nbytes = 0;

	int MsgCounter = 0;
	int i, j;
	int CounterMotorPosition = 0;
	int CounterTorqueTimerInfo = 0;

	omp_set_num_threads(1);
	
	#pragma omp parallel
	{
				while (MsgCounter < NUM_MSG) 
				{
					MsgCounter++; 

				
				//nbytes = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				
					//if(nbytes != 0){ // Verifica se a mensagem foi lida

						while(listaCAN >> auxStr)
						{
							if(wordCounter == 0)
							{
								frameRead.can_id = stoi(auxStr, 0, 16);
							}
							else
							{
								frameRead.data[wordCounter-1] = stoi(auxStr, 0, 16);
							}

							wordCounter++;

							if(wordCounter == 9){
								break;
							}
						}

						wordCounter = 0;



						GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead);
						ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead);
						ObjTemperature1.IfID_Temperature1(&frameRead);
						ObjInternalStates.IfID_InternalStates(&frameRead);
						
						
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
				}

	}
	//close(SocketCan);
	fclose(Arquivo);

	return 0;
}
