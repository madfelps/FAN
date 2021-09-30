

//============================================================================
// Name        : main.cpp
// Author      : Felipe Moura Madureira, Henrique Borges Garcia e Gaspar Henrique
// Version     : 0.0
// Copyright   : Your copyright notice
// Description : main FAN project
//============================================================================




#include "RemyHVH250.h"
#include <signal.h>


#define NUM_MSG 4
#define MAXLINE 1024
std::atomic_bool keeprunning{true};

void int_handler(int dummy)
{
	keeprunning = false;
}

using json = nlohmann::json;

#define TORQUE_LIMIT_PARAMETER 800

/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */


int main(){
	signal(SIGINT,int_handler);
	printf("Iniciando.. \n");
	float SpeedPretendida;
	float TorquePretendido;
	float TorqueLimit;

	char TempoEmString[10];

	//clock_t GuardaIntervaloTempo;
	//clock_t Temporaria;

	//FILE* Arquivo;
	//Arquivo = fopen("log.txt", "a");


	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;
	Temperature1 ObjTemperature1;
	Temperature2 ObjTemperature2;
	Temperature3 ObjTemperature3;
	CurrentInformation ObjCurrentInformation;
	VoltageInformation ObjVoltageInformation;
	FluxInformation ObjFluxInformation;
	InternalVoltages ObjInternalVoltages;
	AnalogInputVoltages ObjAnalogInputVoltages;
	ModulationIndex_FluxWeakening ObjModulationIndex_FluxWeakening;
	CommandMessage ObjCommandMessage;
	InternalStates ObjInternalStates;



	int SocketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	struct sockaddr_can addr;
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "can1");
	ioctl(SocketCan, SIOCGIFINDEX, &ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(SocketCan, (struct sockaddr *) &addr, sizeof(addr));
	struct can_frame frameRead, frameWrite; //Criação dos frames
	struct can_frame frameWrite_Reserva, frameWrite_Titular;
	frameRead.can_dlc = 8;
	frameWrite.can_dlc = 8;

	for(int i = 0; i < 8; i++){
		frameWrite_Reserva.data[i] = 0;
	}


    /**
	 * @brief CAN configuration
	 */

	int sockfd;
	char MsgToClient[1024];
	struct sockaddr_in servaddr, cliaddr;
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	inet_aton("127.0.0.1" , &servaddr.sin_addr);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(8080);



	/* Set address automatically if desired */
	//if (argc == 2)
	//{

    /* Get host name of this computer */
    //gethostname(host_name, sizeof(host_name));
    //hp = gethostbyname(host_name);

    /* Check for NULL pointer */
    //if (hp == NULL)
    //{
    //    fprintf(stderr, "Could not get host name.\n");
    //    closesocket(sd);
    //    WSACleanup();
    //    exit(0);
    //}

    /* Assign the address */
    //server.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
    //server.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
    //server.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
    //server.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];
	//}

	/* Otherwise assign it manually */
	//else
	//{
	//   server.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)a1;
	//    server.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)a2;
	//    server.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)a3;
	//    server.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)a4;
	//}


	//Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	socklen_t len;
	len = sizeof(cliaddr); //len is value/result

	


	/**
	 * @brief Inicialização e Configuração do Pacote por JSON
	 */
	json UDP_Package;
	UDP_Package["ID"] 							= "X";
	UDP_Package["Angle"] 						= 0.0;
	UDP_Package["Speed"] 						= 0.0;
	UDP_Package["CommandedTorque"] 				= 0.0;
	UDP_Package["TorqueFeedback"] 				= 0.0;
	UDP_Package["ModuleATemperature"] 			= 0.0;
	UDP_Package["ModuleBTemperature"] 			= 0.0;
	UDP_Package["ModuleCTemperature"] 			= 0.0;
	UDP_Package["TemperatureGateDriverBoard"] 	= 0.0;
	UDP_Package["ControlBoardTemperature"]		= 0.0;
	UDP_Package["RTD1_Temperature"]				= 0.0;
	UDP_Package["RTD2_Temperature"]				= 0.0;
	UDP_Package["RTD3_Temperature"] 			= 0.0;
	UDP_Package["RTD4_Temperature"]				= 0.0;
	UDP_Package["MotorTemperature"]				= 0.0;
	UDP_Package["Torque_Shudder"]	 			= 0.0;
	UDP_Package["RTD4_Temperature"]				= 0.0;
	UDP_Package["RTD5_Temperature"] 			= 0.0;
	UDP_Package["MotorTemperature"]				= 0.0;
	UDP_Package["Torque_Shudder"] 				= 0.0;
	UDP_Package["PhaseACurrent"]				= 0.0;
	UDP_Package["PhaseBCurrent"]				= 0.0;
	UDP_Package["PhaseCCurrent"]				= 0.0;
	UDP_Package["DC_BusCurrent"]				= 0.0;
	UDP_Package["DC_BusVoltage"]				= 0.0;
	UDP_Package["OutputVoltage"]				= 0.0;
	UDP_Package["VAB_Vd_Voltage"]				= 0.0;
	UDP_Package["VBC_Vd_Voltage"]				= 0.0;
	UDP_Package["FluxCommand"]					= 0.0;
	UDP_Package["FluxFeedback"]					= 0.0;
	UDP_Package["IdFeedback"]					= 0.0;
	UDP_Package["IqFeedback"]					= 0.0;
	UDP_Package["VoltageReference1Dot5"]		= 0.0;
	UDP_Package["VoltageReference2Dot5"]		= 0.0;
	UDP_Package["VoltageReference5Dot0"]		= 0.0;
	UDP_Package["VoltageReference12"]			= 0.0;
	UDP_Package["FaultCode_0"]					= 0;
	UDP_Package["FaultCode_1"]					= 0;
	UDP_Package["FaultCode_2"]					= 0;
	UDP_Package["FaultCode_3"]					= 0;
	UDP_Package["FaultCode_4"]					= 0;
	UDP_Package["FaultCode_5"]					= 0;
	UDP_Package["FaultCode_6"]					= 0;
	UDP_Package["FaultCode_7"]					= 0;



	//struct can_frame frameRead, frameWrite; //Criação dos frames
	//frameRead.can_dlc = 8;

	//CONFIGURAÇÃO DO TCP/IP

	struct sockaddr_in serverTCP, clientTCP;
	int socketTCP, clientTCP_fd;

	socketTCP = socket(AF_INET, SOCK_STREAM, 0);
	if(socketTCP == -1){
		printf("Impossível criar socket\n");
	}

	serverTCP.sin_family = AF_INET;
	serverTCP.sin_addr.s_addr = INADDR_ANY;
	serverTCP.sin_port = htons(8053);

	if( bind(socketTCP,(struct sockaddr *)&serverTCP , sizeof(serverTCP)) < 0)
    {
        perror("Erro ao fazer bind no TCP");
        return 1;
    }

	listen(socketTCP, 1);

  	int SizeSockAddr = sizeof(struct sockaddr_in);

  	clientTCP_fd = accept(socketTCP, (struct sockaddr *)&clientTCP, (socklen_t*)&SizeSockAddr);

  	if(clientTCP_fd < 0){
    	perror("Falha ao aceitar conexão TCP!");
    	return 1;
  	}
  	puts("Conexão TCP realizada com sucesso!\n");



	int FlagRead = 0;
	int FlagWrite = 0;
	int n = 0;
	char buffer[200];
	char buffer2[100];
    char buffer3[100];
	int contador = 0;
	int MsgCounter = 0;
	int i, j;
	int CounterMotorPosition = 0;
	int CounterTorqueTimerInfo = 0;


	n = recvfrom(sockfd, (char *)buffer, 100,
	MSG_WAITALL, ( struct sockaddr *) &cliaddr,
	&len);

	buffer[n] = '\0';

	json UDP;
	std::string aux_buf = buffer;
	UDP = json::parse(aux_buf);
	printf("Interface : %s\n", UDP.dump().c_str());

	/* 04/09/21
    //Testing message to Qt
	strcpy(buffer2, "Teste para o Qt..");
	sendto(sockfd, (const char *)buffer2, strlen(buffer2),
	MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
	len);
	*/

    int wordCounter = 0;

	/*Leitura de arquivo de texto para testes de processamento
	de dados da CAN*/
    //std::ifstream fileCAN("listaCAN.txt");
    //std::string auxstr;
	//int matrx[4][9];
    //int lin = 0;
    //int col = 0;

	//Configuração do log
	auto logger = spdlog::basic_logger_mt("logger", "log.txt");
	logger->info("Initializing Data Logging");

    //int OpcaoTorqueLimit;
    //int AuxiliarInterface;
    std::mutex valorMtx;

    ObjCommandMessage.SetTorqueLimit(TORQUE_LIMIT_PARAMETER);

	omp_set_num_threads(5);

	volatile int flagAtivaLog = 0;

	int hasMessage = 0;
	//adicionar o logger

	int contador_log = 0; //counter to logging only some data
	int can_counter = 0; //message counter to inverter simulation
	int dyn_can_id = 0;  //dynamic can id to change can id in simulation
	float dyn_motor_speed = 1;
	float dyn_motor_torque = 0;
	float dyn_motor_temp = 0;
	float dyn_inverter_temp = 0;
	int dyn_bytes[8] = {0,0,0,0,0,0,0,0};
	#pragma omp parallel default (none) shared(keeprunning,sockfd, hasMessage, MsgToClient, flagAtivaLog, clientTCP_fd,dyn_motor_speed,dyn_can_id,dyn_bytes) firstprivate(FlagWrite, ObjCommandMessage, FlagRead, frameWrite, SocketCan, wordCounter, TorqueLimit, buffer3, frameRead, n, buffer, ObjMotorPosInfo, ObjTorqueTimerInfo, ObjTemperature1, ObjTemperature2, ObjTemperature3, ObjInternalStates, ObjCurrentInformation, ObjVoltageInformation, ObjFluxInformation, ObjInternalVoltages, ObjAnalogInputVoltages, ObjModulationIndex_FluxWeakening, UDP_Package, contador, len, cliaddr, frameWrite_Reserva, logger,contador_log,can_counter)
	{
		#pragma omp sections
		{
			dyn_can_id = 160; //initialize ID
			/*This thread simulates the motor dynamic, updating the parameters,
			such as, speed, temperatures, torque, etc*/
			#pragma omp section //THREAD MOTOR SIMULATOR
		 	{
				 while(keeprunning)
				{
				/*Sleep for processing reduce*/
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				can_counter++;
				if(1 == can_counter) /*atualiza com ID 160 - temperatures*/
				{
					dyn_can_id = 160;
					dyn_bytes[2]++;
					dyn_bytes[2] >= 80? dyn_bytes[2] = 0 : dyn_bytes[2] = dyn_bytes[2];
					dyn_bytes[3] = 0;
				}
				else if(2 == can_counter)/*atualiza com ID 165 - position*/
				{
					dyn_can_id = 162;
				}
				else if(3 == can_counter)/*atualiza com ID 165 - position*/
				{
					dyn_can_id = 165;
					dyn_motor_speed = dyn_motor_speed + 0.1;
					dyn_motor_speed >= 5000? dyn_motor_speed = 0 : dyn_motor_speed = dyn_motor_speed;
					dyn_bytes[2] = (int)(dyn_motor_speed) & 0xFF;
					dyn_bytes[3] = ((int)(dyn_motor_speed) & 0xFF00) >> 8;
					}
				else if(4 == can_counter)/*atualiza com ID 172 - torque*/
				{
					dyn_can_id = 172;
					dyn_bytes[2] = 0;
					//dyn_bytes[2] >= 1000? dyn_bytes[2] = 0 : dyn_bytes[2] = dyn_bytes[2];
					dyn_bytes[3] = 0;
					can_counter = 0;
					}
				
				 }//end while
			}
		 	#pragma omp section //TASK READ SOFTWARE-MOTOR
		 	
			 {

				std::ifstream ReadLog;

				while (keeprunning)
				{
				//#pragma omp critical (mutex)
				//{
				//The function reads the CAN frame
				//FlagRead = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				//}

				/*Simula latẽncia da CAN*/
				
  						/*A cada 100 mensagens CAN, uma vai para o data logging*/	
						
						contador_log++;
						

						frameRead.can_id = dyn_can_id;
						frameRead.data[0] = 0;
						frameRead.data[1] = 0;
						frameRead.data[2] = dyn_bytes[2];
						frameRead.data[3] = dyn_bytes[3];
						frameRead.data[4] = 255;
						frameRead.data[5] = 0;
						frameRead.data[6] = 0;
						frameRead.data[7] = 0;

						
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
				

						//GuardaIntervaloTempo = clock();
						//The messages passes to all classes for verifying if the received CAN frame is related to this class
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);
						ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
						ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
						ObjTemperature2.IfID_Temperature2(&frameRead, UDP_Package);
						ObjTemperature3.IfID_Temperature3(&frameRead, UDP_Package);
						ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);

						
						if( 10 == contador_log)
						{
							contador_log = 0;
						//Logging generator through SPD log
						if(frameRead.can_id == 160)
						{
							logger->info("{:d} {:.2f} {:.2f} {:.2f}", frameRead.can_id,ObjTemperature1.GetModuleAProcessed(),
							ObjTemperature1.GetModuleBProcessed(),ObjTemperature1.GetModuleCProcessed());
							
						}

						else if(frameRead.can_id == 161){
							logger->info("{:d} {:.2f} {:.2f} {:.2f} {:.2f}", frameRead.can_id,ObjTemperature2.GetControlBoardTemperatureProcessed(),
							ObjTemperature2.GetRTD1Processed(),ObjTemperature2.GetRTD2Processed(),ObjTemperature2.GetRTD3Processed());
							
						}

						else if(frameRead.can_id == 162){
							logger->info("{:d} {:.2f} {:.2f} {:.2f} {:.2f} ", frameRead.can_id,ObjTemperature3.GetRTD4_TemperatureProcessed(),
							ObjTemperature3.GetRTD5_TemperatureProcessed(),ObjTemperature3.GetMotorTemperatureProcessed(),
							ObjTemperature3.GetTorqueShudderProcessed());
						}

						else if(frameRead.can_id == 165){
							logger->info("{:d} {:.2f} {:.2f}  ", frameRead.can_id,ObjMotorPosInfo.GetMotorAngleProcessed(),
							ObjMotorPosInfo.GetMotorSpeedProcessed());
						}

				// 		else if(frameRead.can_id == 166){
				// 			logger->info("ID: CURRENT_INFORMATION/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjCurrentInformation.GetByte(7), ObjCurrentInformation.GetByte(6), ObjCurrentInformation.GetByte(5), ObjCurrentInformation.GetByte(4), ObjCurrentInformation.GetByte(3), ObjCurrentInformation.GetByte(2), ObjCurrentInformation.GetByte(1), ObjCurrentInformation.GetByte(0));
				// 			logger->info("Phase A Current: {:f}/", ObjCurrentInformation.GetPhaseACurrentProcessed());
				// 			logger->info("Phase B Current: {:f}/", ObjCurrentInformation.GetPhaseBCurrentProcessed());
				// 			logger->info("Phase C Current: {:f}/", ObjCurrentInformation.GetPhaseCCurrentProcessed());
				// 			logger->info("DC Bus Current: {:f}/", ObjCurrentInformation.GetDCBusCurrentProcessed());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}

				// 		else if(frameRead.can_id == 167){
				// 			logger->info("ID: VOLTAGE_INFORMATION/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjVoltageInformation.GetByte(7), ObjVoltageInformation.GetByte(6), ObjVoltageInformation.GetByte(5), ObjVoltageInformation.GetByte(4), ObjVoltageInformation.GetByte(3), ObjVoltageInformation.GetByte(2), ObjVoltageInformation.GetByte(1), ObjVoltageInformation.GetByte(0));
				// 			logger->info("DC Bus Voltage: {:f}/", ObjVoltageInformation.GetDCBusVoltageProcessed());
				// 			logger->info("Output Voltage: {:f}/", ObjVoltageInformation.GetOutputVoltageProcessed());
				// 			logger->info("VAB Vd Voltage: {:f}/", ObjVoltageInformation.GetVAB_Vd_VoltageProcessed());
				// 			logger->info("VBC Vd Voltage: {:f}/", ObjVoltageInformation.GetVBC_Vd_VoltageProcessed());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}

				// 		else if(frameRead.can_id == 168){
				// 			logger->info("ID: FLUX_INFORMATION/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjFluxInformation.GetByte(7), ObjFluxInformation.GetByte(6), ObjFluxInformation.GetByte(5), ObjFluxInformation.GetByte(4), ObjVoltageInformation.GetByte(3), ObjFluxInformation.GetByte(2), ObjFluxInformation.GetByte(1), ObjFluxInformation.GetByte(0));
				// 			logger->info("Flux Command: {:f}/", ObjFluxInformation.GetFluxCommandProcessed());
				// 			logger->info("Flux Feedback: {:f}/", ObjFluxInformation.GetFluxFeedbackProcessed());
				// 			logger->info("Id Feedback: {:f}/", ObjFluxInformation.GetIdFeedbackProcessed());
				// 			logger->info("Iq Feedback: {:f}/", ObjFluxInformation.GetIqFeedbackProcessed());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}

				// 		else if(frameRead.can_id == 169){
				// 			logger->info("ID: INTERNAL_VOLTAGES/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjInternalVoltages.GetByte(7), ObjInternalVoltages.GetByte(6), ObjInternalVoltages.GetByte(5), ObjInternalVoltages.GetByte(4), ObjVoltageInformation.GetByte(3), ObjInternalVoltages.GetByte(2), ObjInternalVoltages.GetByte(1), ObjInternalVoltages.GetByte(0));
				// 			logger->info("Voltage Reference 1.5: {:f}/", ObjInternalVoltages.GetVoltageReference1Dot5());
				// 			logger->info("Voltage Reference 2.5: {:f}/", ObjInternalVoltages.GetVoltageReference2Dot5());
				// 			logger->info("Voltage Reference 5.0: {:f}/", ObjInternalVoltages.GetVoltageReference5Dot0());
				// 			logger->info("Voltage Reference 12: {:f}/", ObjInternalVoltages.GetVoltageReference12());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}
				// 		//VER ISSO DAQUI DPS (INTERNAL STATES)
				// 		else if(frameRead.can_id == 170){
				// 			logger->info("ID: INTERNAL_VOLTAGES/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjInternalVoltages.GetByte(7), ObjInternalVoltages.GetByte(6), ObjInternalVoltages.GetByte(5), ObjInternalVoltages.GetByte(4), ObjVoltageInformation.GetByte(3), ObjInternalVoltages.GetByte(2), ObjInternalVoltages.GetByte(1), ObjInternalVoltages.GetByte(0));
				// 			logger->info("Voltage Reference 1.5: {:f}/", ObjInternalVoltages.GetVoltageReference1Dot5());
				// 			logger->info("Voltage Reference 2.5: {:f}/", ObjInternalVoltages.GetVoltageReference2Dot5());
				// 			logger->info("Voltage Reference 5.0: {:f}/", ObjInternalVoltages.GetVoltageReference5Dot0());
				// 			logger->info("Voltage Reference 12: {:f}/", ObjInternalVoltages.GetVoltageReference12());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}
						
				// 		else if(frameRead.can_id == 171){
				// 			logger->info("ID: INTERNAL_VOLTAGES/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjInternalVoltages.GetByte(7), ObjInternalVoltages.GetByte(6), ObjInternalVoltages.GetByte(5), ObjInternalVoltages.GetByte(4), ObjVoltageInformation.GetByte(3), ObjInternalVoltages.GetByte(2), ObjInternalVoltages.GetByte(1), ObjInternalVoltages.GetByte(0));
				// 			logger->info("Voltage Reference 1.5: {:f}/", ObjInternalVoltages.GetVoltageReference1Dot5());
				// 			logger->info("Voltage Reference 2.5: {:f}/", ObjInternalVoltages.GetVoltageReference2Dot5());
				// 			logger->info("Voltage Reference 5.0: {:f}/", ObjInternalVoltages.GetVoltageReference5Dot0());
				// 			logger->info("Voltage Reference 12: {:f}/", ObjInternalVoltages.GetVoltageReference12());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}


				// 		else if(frameRead.can_id == 172){
				// 			logger->info("ID: TORQUE_TIMER_INFO/");
				// 			logger->info("Bytes recebidos: %f %f %f %f %f %f %f %f", ObjTorqueTimerInfo.GetByte(7), ObjTorqueTimerInfo.GetByte(6), ObjTorqueTimerInfo.GetByte(5), ObjTorqueTimerInfo.GetByte(4), ObjTorqueTimerInfo.GetByte(3), ObjTorqueTimerInfo.GetByte(2), ObjTorqueTimerInfo.GetByte(1), ObjTorqueTimerInfo.GetByte(0));
				// 			logger->info("Commanded Torque: %f", ObjTorqueTimerInfo.GetCommandedTorqueProcessed());
				// 			logger->info("Torque Feedback: %f", ObjTorqueTimerInfo.GetTorqueFeedbackProcessed());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}

				// 		else if(frameRead.can_id == 173){ //TODO VER ISSO TBM (FAULT ERRORS)
				// 			logger->info("ID: MODULATIONINDEX_FLUXWEAKENING/");
				// 			logger->info("ID: {:d}", frameRead.can_id);
				// 			logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}/", ObjInternalVoltages.GetByte(7), ObjInternalVoltages.GetByte(6), ObjInternalVoltages.GetByte(5), ObjInternalVoltages.GetByte(4), ObjVoltageInformation.GetByte(3), ObjInternalVoltages.GetByte(2), ObjInternalVoltages.GetByte(1), ObjInternalVoltages.GetByte(0));
				// 			logger->info("Voltage Reference 1.5: {:f}/", ObjInternalVoltages.GetVoltageReference1Dot5());
				// 			logger->info("Voltage Reference 2.5: {:f}/", ObjInternalVoltages.GetVoltageReference2Dot5());
				// 			logger->info("Voltage Reference 5.0: {:f}/", ObjInternalVoltages.GetVoltageReference5Dot0());
				// 			logger->info("Voltage Reference 12: {:f}/", ObjInternalVoltages.GetVoltageReference12());
				// 			logger->info("--------------------------------------------------/");
				// 			//printf("\n%d",frameRead.can_id);
				// 		}
						}
				

						
						

                        //Convert JSON format to std::string
						std::string UDP_Package_StdString = UDP_Package.dump();
						#pragma omp critical (teste)
						{
						    //Basically, convert std::string to c-string
						    strcpy(MsgToClient, UDP_Package_StdString.c_str());
						    //printf("Mensagem aqui: %s\n", MsgToClient);

						    //Indicate that there's a message to be sent
						    hasMessage = 1;
						}

				// 		if(flagAtivaLog){
							
				// 			ReadLog.open("log.txt");
							
				// 			std::string linha;
				// 			if(ReadLog.is_open()){
  				// 				while(getline(ReadLog, linha)){
								  
  	   			// 				//write(client_fd, linha.c_str(), linha.size());
  				// 				send(clientTCP_fd, linha.c_str(), linha.size(),0);
				// 				std::this_thread::sleep_for (std::chrono::milliseconds(100));	
  				// 				}
				// 				linha = "*";
				// 				send(clientTCP_fd, linha.c_str(), linha.size(), 0);
  				// 			}
				// 			flagAtivaLog = 0;  
				// 	
					
				}//end while


			}//end thread 1


			#pragma omp section //TASK WRITE MOTOR
			{
				while(keeprunning){

					#pragma omp critical (writeFrame)
					{
					    //The project uses two frames for sending data to the motor: one is used to store
					    //all the parameters to be sent while the another is used for
					    frameWrite = frameWrite_Reserva;
					}

					#pragma omp critical (mutex)
					{
					    //The main frame write is sent to the motor
					FlagWrite = write(SocketCan, &frameWrite, sizeof(struct can_frame));

					}


                    //As the CAN manual requires a minimum write data rate of 500ms, it's configured to send every 400ms
					//delay_openmp(400);
					//std::cout << std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
					std::this_thread::sleep_for (std::chrono::milliseconds(400));
				}
			}//end thread 2

			#pragma omp section // TASK READ INTERFACE
			{
				std::string Mensagem;
				json UDP_Receive;
				std::string RecID;
				std::string const Speed_Literal 			= "speed_id";
				std::string const InverterEnable_Literal 	= "enable_id";
				int lenUDP = 0;
				std::string SpeedTeste;
				float SpeedSend;
				
				char IdMensagem[100];

				while(keeprunning){
					ioctl(sockfd, FIONREAD, &lenUDP);
					if(lenUDP > 0 && lenUDP < sizeof(buffer)){
						n = recvfrom(sockfd, (char *)buffer, lenUDP,
							MSG_WAITALL, ( struct sockaddr *) &cliaddr,
							&len);
						buffer[n] = '\0';

					}
					else{
						continue;
					}

					//printf("%s\n", buffer);
					Mensagem = buffer;

					UDP_Receive = json::parse(Mensagem);
					

					
					Mensagem = UDP_Receive.dump();
					//strcpy(Mensagem, Mensagem.c_str());
					printf("Interface: %s\n", UDP_Receive.dump().c_str());


					
					//strcpy(MensagemTeste, UDP_Receive.c_str());


					//TorqueLimit = atof(buffer);
					//printf("%s \n", buffer);

					//Verify if the message contains an ID field
					if(UDP_Receive.find("ID") != UDP_Receive.end()){
						#pragma omp critical (writeFrame)
						{
						
						if(UDP_Receive["ID"] == "speed_id"){
							SpeedTeste = UDP_Receive["Speed_Command"];
							SpeedSend = std::stod(SpeedTeste);
							printf("Speed: %d\n", (int)SpeedSend);
							//Adicionar a descarga do inversor SetDischarge()
							
							ObjCommandMessage.UpdateFrame(&frameWrite_Reserva, SpeedSend); //Update the speed value in write frame
							printf("CAN ID: %d\n", frameWrite_Reserva.can_id);
							printf("CAN Data: ");
							for(int i = 0; i < 8; i++){
								printf("%d ", frameWrite_Reserva.data[i]);
							}
							printf("\n\n");
						}
						else if(UDP_Receive["ID"] == "enable_id"){
							 
							 if(UDP_Receive["Enable_Command"] == "true"){
								 
							 	ObjCommandMessage.SetInverterEnable(1, &frameWrite_Reserva);
							 	
							 }
							 else{
							 	ObjCommandMessage.SetInverterEnable(0, &frameWrite_Reserva);
								 flagAtivaLog = 1;
							 }
							printf("CAN ID: %d\n", frameWrite_Reserva.can_id);
							printf("CAN Data: "); 
							for(int i = 0; i < 8; i++){
								printf("%d ", frameWrite_Reserva.data[i]);
							}
							printf("\n\n");
							// //Esse ponteiro e flag vão ser utilizados nas duas threads
							// // flag atomic (flag e bool)
						}

						}//end critical

					}//end if

				}//end while
			}//end thread 3
			
			#pragma omp section // TASK WRITE INTERFACE
			{
				while(keeprunning){
					
					#pragma omp critical (teste)
					{
						
					if(hasMessage){
						//printf("Mensagem: %s\n", MsgToClient);
						sendto(sockfd, (const char *)MsgToClient, strlen(MsgToClient),
						MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
						len);
						hasMessage = 0;			
						std::this_thread::sleep_for(std::chrono::milliseconds(5));	
						
					}
					}//end while


					

				}
			}
		}//end sections
	
	
	} //end openmp
	close(SocketCan);
	//fclose(Arquivo);
	close(sockfd);
	close(socketTCP);
	logger->info("End");
	std::cout << std::endl <<"fechando bonitinho" << std::endl;
	return 0;
}//end main
