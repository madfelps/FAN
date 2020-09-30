//============================================================================
// Name        : main.cpp
// Author      : Felipe Moura Madureira, Henrique Borges Garcia e Gaspar Henrique
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
#include <vector>
#include <string>


#define NUM_MSG 4



int main()
{
	float SpeedPretendida;
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
	
	/*int SocketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	struct sockaddr_can addr;
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "can1");
	ioctl(SocketCan, SIOCGIFINDEX, &ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(SocketCan, (struct sockaddr *) &addr, sizeof(addr));
	struct can_frame frameRead, frameWrite; //Criação dos frames
	frameRead.can_dlc = 8;
	frameWrite.can_dlc = 8;*/
	

	//Configuração do protocolo UDP



	//Criação do vector string para envio das mensagens por UDP
	vector<string> DataUDPSendQt;

	struct can_frame frameRead, frameWrite; //Criação dos frames
	frameRead.can_dlc = 8;

	//Configuração do arquivo para teste
	std::ifstream listaCAN("listaCAN.txt");
	std::string auxStr;

	//std::ofstream Log("log.txt");
	//vector<std::string> StringDescreveSensor, StringGuardaDadosSensor;
	int wordCounter = 0;


	int FlagRead = 0;
	int FlagWrite = 0;

	int MsgCounter = 0;
	int i, j;
	int CounterMotorPosition = 0;
	int CounterTorqueTimerInfo = 0;

	omp_set_num_threads(2);
	

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
				FlagRead = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				}

					//if(FlagRead != 0){ // Verifica se a mensagem foi lida

					if(FlagRead != 0){ // Verifica se a mensagem foi lida


						while(wordCounter < 9)
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
						}

						wordCounter = 0;



						GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, DataUDPSendQt);
						ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, DataUDPSendQt);
						ObjTemperature1.IfID_Temperature1(&frameRead, DataUDPSendQt);
						ObjInternalStates.IfID_InternalStates(&frameRead, DataUDPSendQt);

						//Rotina para envio via UDP pro PC


						
						//Guarda dados dos sensores na string PARA VECTOR STRING, USAR PUSH BACK
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
						strcpy(StringDescreveSensor[9], "Instante da geracao dos dados (em segundos): ");
						sprintf(TempoEmString, "%lf", (double) Temporaria*10000/CLOCKS_PER_SEC);
						strcat(StringDescreveSensor[9], TempoEmString);

						//Concatena os valores na string
						for(i = 0; i < 8; i++){
							strcat(StringDescreveSensor[i], StringGuardaDadosSensor[i]);
							//StringDescreveSensor[i] = StringDescreveSensor[i] + StringGuardaDadosSensor[i];
						}
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
						/*for(i = 0; i < 9; i++){
							Log << StringDescreveSensor[i];
							Log << "\n";
						}*/
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

				//}
					}
				}
			}
		}
		
	}

			
	/*#pragma omp section //TASK WRITE
	{ 
		print("Digite o valor desejado de Speed (RPM)\n");
		scanf("%f", &SpeedPretendida);
		print("Digite o valor desejado de TorqueLimit\n");
		scanf("%f", &TorqueLimit)

		ObjCommandMessage.ProcessAngleVelocity(&SpeedPretendida)
		ObjCommandMessage.ProcessTorqueSend(&TorqueLimit, 1);
		ObjCommandMessage.UpdateFrame(&frameWrite);
		#pragma omp critical (mutex)
		{
			FlagWrite = write(SocketCan, &frameWrite, sizeof(struct can_frame));
		}
	}*/

	//close(SocketCan);
	fclose(Arquivo);

	return 0;
}
