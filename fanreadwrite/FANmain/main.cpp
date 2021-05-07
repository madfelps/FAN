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


#include "lertxt.h"


#define NUM_MSG 4
#define MAXLINE 50



using json = nlohmann::json;


int main()
{
	printf("Iniciando.. \n");
	float SpeedPretendida;
	float TorquePretendido;
	float TorqueLimit;

	char TempoEmString[10];

	//clock_t GuardaIntervaloTempo;
	//clock_t Temporaria;

	//FILE* Arquivo;
	//Arquivo = fopen("log.txt", "a");

	char StringDescreveSensor[11][10];
	char StringGuardaDadosSensor[10][10];


	//Declaração dos Objetos
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
	struct can_frame frameWrite_Reserva, frameWrite_Titular;
	frameRead.can_dlc = 8;
	frameWrite.can_dlc = 8;



	//Configuração do protocolo UDP
	int sockfd;
	char MsgToClient[400];
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


	//Inicialização e Configuração do Pacote por JSON
	json UDP_Package;
	//std::string UDP_Package_StdString = "";

	UDP_Package["ID"] 							= "X";
	UDP_Package["Angle"] 						= 0.0;
	UDP_Package["Speed"] 						= 0.0;
	UDP_Package["CommandedTorque"] 				= 0.0;
	UDP_Package["TorqueFeedback"] 				= 0.0;
	UDP_Package["TemperatureModuleA"] 			= 0.0;
	UDP_Package["TemperatureModuleB"] 			= 0.0;
	UDP_Package["TemperatureModuleC"] 			= 0.0;
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

	//Recepção de dados pela interface
	json UDP_Interface;

	UDP_Interface["Parameter_1"]	=	0;
	UDP_Interface["Parameter_2"]	=	0;
	UDP_Interface["Parameter_3"]	=	0;
	UDP_Interface["Parameter_4"]	=	0;




	//struct can_frame frameRead, frameWrite; //Criação dos frames
	//frameRead.can_dlc = 8;



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
	printf("Client : %s\n", buffer);



	strcpy(buffer2, "Teste para o Qt..");
	sendto(sockfd, (const char *)buffer2, strlen(buffer2),
	MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
	len);

    int wordCounter = 0;

    std::ifstream fileCAN("listaCAN.txt");
    std::string auxstr;
	int matrx[4][9];
    int lin = 0;
    int col = 0;

	//Configuração do log
	auto logger = spdlog::basic_logger_mt("logger", "log.txt");
	logger->info("Arquivo de log de dados do software embarcado");


    int OpcaoTorqueLimit;
    int AuxiliarInterface;
    std::mutex valorMtx;


	lin = 0;

	//em caso de teste do chrono, incluir std::cout no shared do openmp
	int hasMessage = 0;
	#pragma omp parallel default (none) shared(sockfd, hasMessage, MsgToClient) firstprivate(FlagWrite, ObjCommandMessage, FlagRead, frameWrite, SocketCan, wordCounter, TorqueLimit, buffer3, frameRead, n, buffer, ObjMotorPosInfo, ObjTorqueTimerInfo, ObjTemperature1, ObjTemperature2, ObjTemperature3, ObjInternalStates, ObjCurrentInformation, ObjVoltageInformation, ObjFluxInformation, ObjInternalVoltages, ObjAnalogInputVoltages, ObjModulationIndex_FluxWeakening, UDP_Package, contador, len, cliaddr, logger, AuxiliarInterface, frameWrite_Reserva)
	{
		#pragma omp sections
		{

		 	#pragma omp section //TASK READ SOFTWARE-MOTOR

		 	{

				while (1)
				{

				//#pragma omp critical (mutex)
				//{
				//FlagRead = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				//}

					   frameRead.can_id = 162;
					   frameRead.data[0] = 2;
					   frameRead.data[1] = 1;
					   frameRead.data[2] = 51;
					   frameRead.data[3] = 1;
					   frameRead.data[4] = 2;
					   frameRead.data[5] = 2;
					   frameRead.data[6] = 51;
					   frameRead.data[7] = 2;




						//GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);

						ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
						ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
						ObjTemperature2.IfID_Temperature2(&frameRead, UDP_Package);
						ObjTemperature3.IfID_Temperature3(&frameRead, UDP_Package);
						ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);


						//Desenvolvimento do log
						if(frameRead.can_id == 160){
							logger->info("ID: TEMPERATURES_1");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjTemperature1.GetByte7(), ObjTemperature1.GetByte6(), ObjTemperature1.GetByte5(), ObjTemperature1.GetByte4(), ObjTemperature1.GetByte3(), ObjTemperature1.GetByte2(), ObjTemperature1.GetByte1(), ObjTemperature1.GetByte0());
							logger->info("Module A: {:f}", ObjTemperature1.GetModuleAProcessed());
							logger->info("Module B: {:f}", ObjTemperature1.GetModuleBProcessed());
							logger->info("Module C: {:f}", ObjTemperature1.GetModuleCProcessed());
							logger->info("--------------------------------------------------");
						}

						//TO DO ARRUMAR O ELSE
						else if(frameRead.can_id == 161){
							logger->info("ID: TEMPERATURES_2");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjTemperature2.GetByte7(), ObjTemperature2.GetByte6(), ObjTemperature2.GetByte5(), ObjTemperature2.GetByte4(), ObjTemperature2.GetByte3(), ObjTemperature2.GetByte2(), ObjTemperature2.GetByte1(), ObjTemperature2.GetByte0());
							logger->info("Control Board Temperature: {:f}", ObjTemperature2.GetControlBoardTemperatureProcessed());
							logger->info("RTD1: {:f}", ObjTemperature2.GetRTD1Processed());
							logger->info("RTD2: {:f}", ObjTemperature2.GetRTD2Processed());
							logger->info("RTD3: {:f}", ObjTemperature2.GetRTD3Processed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 162){
							logger->info("ID: TEMPERATURES_3");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjTemperature3.GetByte7(), ObjTemperature3.GetByte6(), ObjTemperature3.GetByte5(), ObjTemperature3.GetByte4(), ObjTemperature3.GetByte3(), ObjTemperature3.GetByte2(), ObjTemperature3.GetByte1(), ObjTemperature3.GetByte0());
							logger->info("RTD4 Temperature: {:f}", ObjTemperature3.GetRTD4_TemperatureProcessed());
							logger->info("RTD5 Temperature: {:f}", ObjTemperature3.GetRTD5_TemperatureProcessed());
							logger->info("Motor Temperature: {:f}", ObjTemperature3.GetMotorTemperatureProcessed());
							logger->info("Torque Shudder: {:f}", ObjTemperature3.GetTorqueShudderProcessed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 165){
							logger->info("ID: MOTOR_POSITION");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjMotorPosInfo.GetByte7(), ObjMotorPosInfo.GetByte6(), ObjMotorPosInfo.GetByte5(), ObjMotorPosInfo.GetByte4(), ObjMotorPosInfo.GetByte3(), ObjMotorPosInfo.GetByte2(), ObjMotorPosInfo.GetByte1(), ObjMotorPosInfo.GetByte0());
							logger->info("Angle: {:f}", ObjMotorPosInfo.GetMotorAngleProcessed());
							logger->info("Speed: {:f}", ObjMotorPosInfo.GetMotorSpeedProcessed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 166){
							logger->info("ID: CURRENT_INFORMATION");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjCurrentInformation.GetByte7(), ObjCurrentInformation.GetByte6(), ObjCurrentInformation.GetByte5(), ObjCurrentInformation.GetByte4(), ObjCurrentInformation.GetByte3(), ObjCurrentInformation.GetByte2(), ObjCurrentInformation.GetByte1(), ObjCurrentInformation.GetByte0());
							logger->info("Phase A Current: {:f}", ObjCurrentInformation.GetPhaseACurrentProcessed());
							logger->info("Phase B Current: {:f}", ObjCurrentInformation.GetPhaseBCurrentProcessed());
							logger->info("Phase C Current: {:f}", ObjCurrentInformation.GetPhaseCCurrentProcessed());
							logger->info("DC Bus Current: {:f}", ObjCurrentInformation.GetDCBusCurrentProcessed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 167){
							logger->info("ID: VOLTAGE_INFORMATION");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjVoltageInformation.GetByte7(), ObjVoltageInformation.GetByte6(), ObjVoltageInformation.GetByte5(), ObjVoltageInformation.GetByte4(), ObjVoltageInformation.GetByte3(), ObjVoltageInformation.GetByte2(), ObjVoltageInformation.GetByte1(), ObjVoltageInformation.GetByte0());
							logger->info("DC Bus Voltage: {:f}", ObjVoltageInformation.GetDCBusVoltageProcessed());
							logger->info("Output Voltage: {:f}", ObjVoltageInformation.GetOutputVoltageProcessed());
							logger->info("VAB Vd Voltage: {:f}", ObjVoltageInformation.GetVAB_Vd_VoltageProcessed());
							logger->info("VBC Vd Voltage: {:f}", ObjVoltageInformation.GetVBC_Vd_VoltageProcessed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 168){
							logger->info("ID: FLUX_INFORMATION");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjFluxInformation.GetByte7(), ObjFluxInformation.GetByte6(), ObjFluxInformation.GetByte5(), ObjFluxInformation.GetByte4(), ObjVoltageInformation.GetByte3(), ObjFluxInformation.GetByte2(), ObjFluxInformation.GetByte1(), ObjFluxInformation.GetByte0());
							logger->info("Flux Command: {:f}", ObjFluxInformation.GetFluxCommandProcessed());
							logger->info("Flux Feedback: {:f}", ObjFluxInformation.GetFluxFeedbackProcessed());
							logger->info("Id Feedback: {:f}", ObjFluxInformation.GetIdFeedbackProcessed());
							logger->info("Iq Feedback: {:f}", ObjFluxInformation.GetIqFeedbackProcessed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 169){
							logger->info("ID: INTERNAL_VOLTAGES");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjInternalVoltages.GetByte7(), ObjInternalVoltages.GetByte6(), ObjInternalVoltages.GetByte5(), ObjInternalVoltages.GetByte4(), ObjVoltageInformation.GetByte3(), ObjInternalVoltages.GetByte2(), ObjInternalVoltages.GetByte1(), ObjInternalVoltages.GetByte0());
							logger->info("Voltage Reference 1.5: {:f}", ObjInternalVoltages.GetVoltageReference1Dot5());
							logger->info("Voltage Reference 2.5: {:f}", ObjInternalVoltages.GetVoltageReference2Dot5());
							logger->info("Voltage Reference 5.0: {:f}", ObjInternalVoltages.GetVoltageReference5Dot0());
							logger->info("Voltage Reference 12: {:f}", ObjInternalVoltages.GetVoltageReference12());
							logger->info("--------------------------------------------------");
						}
						//VER ISSO DAQUI DPS (INTERNAL STATES)
						else if(frameRead.can_id == 170){
							logger->info("ID: INTERNAL_VOLTAGES");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjInternalVoltages.GetByte7(), ObjInternalVoltages.GetByte6(), ObjInternalVoltages.GetByte5(), ObjInternalVoltages.GetByte4(), ObjVoltageInformation.GetByte3(), ObjInternalVoltages.GetByte2(), ObjInternalVoltages.GetByte1(), ObjInternalVoltages.GetByte0());
							logger->info("Voltage Reference 1.5: {:f}", ObjInternalVoltages.GetVoltageReference1Dot5());
							logger->info("Voltage Reference 2.5: {:f}", ObjInternalVoltages.GetVoltageReference2Dot5());
							logger->info("Voltage Reference 5.0: {:f}", ObjInternalVoltages.GetVoltageReference5Dot0());
							logger->info("Voltage Reference 12: {:f}", ObjInternalVoltages.GetVoltageReference12());
							logger->info("--------------------------------------------------");
						}
						//VER ISSO TBM (FAULT ERRORS)
						else if(frameRead.can_id == 171){
							logger->info("ID: INTERNAL_VOLTAGES");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjInternalVoltages.GetByte7(), ObjInternalVoltages.GetByte6(), ObjInternalVoltages.GetByte5(), ObjInternalVoltages.GetByte4(), ObjVoltageInformation.GetByte3(), ObjInternalVoltages.GetByte2(), ObjInternalVoltages.GetByte1(), ObjInternalVoltages.GetByte0());
							logger->info("Voltage Reference 1.5: {:f}", ObjInternalVoltages.GetVoltageReference1Dot5());
							logger->info("Voltage Reference 2.5: {:f}", ObjInternalVoltages.GetVoltageReference2Dot5());
							logger->info("Voltage Reference 5.0: {:f}", ObjInternalVoltages.GetVoltageReference5Dot0());
							logger->info("Voltage Reference 12: {:f}", ObjInternalVoltages.GetVoltageReference12());
							logger->info("--------------------------------------------------");
						}


						else if(frameRead.can_id == 172){
							logger->info("ID: TORQUE_TIMER_INFO");
							logger->info("Bytes recebidos: %f %f %f %f %f %f %f %f", ObjTorqueTimerInfo.GetByte7(), ObjTorqueTimerInfo.GetByte6(), ObjTorqueTimerInfo.GetByte5(), ObjTorqueTimerInfo.GetByte4(), ObjTorqueTimerInfo.GetByte3(), ObjTorqueTimerInfo.GetByte2(), ObjTorqueTimerInfo.GetByte1(), ObjTorqueTimerInfo.GetByte0());
							logger->info("Commanded Torque: %f", ObjTorqueTimerInfo.GetCommandedTorqueProcessed());
							logger->info("Torque Feedback: %f", ObjTorqueTimerInfo.GetTorqueFeedbackProcessed());
							logger->info("--------------------------------------------------");
						}

						else if(frameRead.can_id == 171){ //TODO VER ISSO TBM (FAULT ERRORS)
							logger->info("ID: MODULATIONINDEX_FLUXWEAKENING");
							logger->info("ID: {:d}", frameRead.can_id);
							logger->info("Bytes recebidos: {:d} {:d} {:d} {:d} {:d} {:d} {:d} {:d}", ObjInternalVoltages.GetByte7(), ObjInternalVoltages.GetByte6(), ObjInternalVoltages.GetByte5(), ObjInternalVoltages.GetByte4(), ObjVoltageInformation.GetByte3(), ObjInternalVoltages.GetByte2(), ObjInternalVoltages.GetByte1(), ObjInternalVoltages.GetByte0());
							logger->info("Voltage Reference 1.5: {:f}", ObjInternalVoltages.GetVoltageReference1Dot5());
							logger->info("Voltage Reference 2.5: {:f}", ObjInternalVoltages.GetVoltageReference2Dot5());
							logger->info("Voltage Reference 5.0: {:f}", ObjInternalVoltages.GetVoltageReference5Dot0());
							logger->info("Voltage Reference 12: {:f}", ObjInternalVoltages.GetVoltageReference12());
							logger->info("--------------------------------------------------");
						}


						std::string UDP_Package_StdString = UDP_Package.dump();
						#pragma omp critical (teste)
						{
						strcpy(MsgToClient, UDP_Package_StdString.c_str());
						//printf("Mensagem aqui: %s\n", MsgToClient);
						hasMessage = 1;
						}
					}


			}


			#pragma omp section //TASK WRITE MOTOR
			{
				while(1){

					#pragma omp critical (writeFrame)
					{
					frameWrite = frameWrite_Reserva;
					}

					#pragma omp critical (mutex)
					{
					FlagWrite = write(SocketCan, &frameWrite, sizeof(struct can_frame));

					}



					//delay_openmp(400);
					//std::cout << std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
					std::this_thread::sleep_for (std::chrono::milliseconds(400));
				}
			}

			#pragma omp section // TASK READ INTERFACE
			{
				std::string Mensagem;
				json UDP_Receive;
				std::string RecID;
				std::string const Speed_Literal 			= "speed_id";
				std::string const InverterEnable_Literal 	= "enable_id";
				int lenUDP = 0;


				while(1){
					//printf("Chegando da interface..");
					//TODO ver a necessidade de utilizar mutex no UDP
					ioctl(sockfd, FIONREAD, &lenUDP);
					if(lenUDP > 0 && lenUDP < sizeof(buffer)){
						n = recvfrom(sockfd, (char *)buffer, lenUDP,
							MSG_WAITALL, ( struct sockaddr *) &cliaddr,
							&len);
						buffer[n] = '\0';
						//TODO Rever a questão da adição do '\0' (bitrix)

					}
					else{
						continue;
					}

					printf("%s\n", buffer);
					Mensagem = buffer;

					UDP_Receive = json::parse(Mensagem);

					//TorqueLimit = atof(buffer);
					//printf("%s \n", buffer);

					if(UDP_Receive.find("ID") != UDP_Receive.end()){
						#pragma omp critical (writeFrame)
						{
						if(UDP_Receive["ID"] == "speed_id"){
							//ObjCommandMessage.Process_SpeedSend(static_cast<float*>(UDP_Receive.at("speed_id"))); //verificar isso depois
							ObjCommandMessage.UpdateFrame(&frameWrite_Reserva);
						}
						else if(UDP_Receive["ID"] == "enable_id"){
							if(UDP_Receive["Enable_Command"]){
								ObjCommandMessage.SetInverterEnable(1, &frameWrite_Reserva);
							}
							else{
								ObjCommandMessage.SetInverterEnable(0, &frameWrite_Reserva);
							}

							//TODO ponteiro pra frameWrite pro conceito de titular e reserva (2 framewrite); atomic boolean
							//Esse ponteiro e flag vão ser utilizados nas duas threads
							// flag atomic (flag e bool)
						}
						}

					}

				}
			}

			#pragma omp section // TASK WRITE INTERFACE
			{
				while(1){

					#pragma omp critical (teste)
					{
					if(hasMessage){
						//printf("Mensagem: %s\n", MsgToClient);
						sendto(sockfd, (const char *)MsgToClient, strlen(MsgToClient),
						MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
						len);
						hasMessage = 0;

					}
					}

					//Envio do pacote UDP para o computador

					//UDP_Package_StdString = UDP_Package.dump();
					//std::string UDP_Package_StdString = UDP_Package.dump();

					//DESCOMENTAR A LINHA DE BAIXO DEPOIS
					//strcpy(MsgToClient, UDP_Package_StdString.c_str());
					//printf("Mensagem: %s\n", MsgToClient);
					//DESCOMENTAR A LINHA DE BAIXO DEPOIS


//					printf("vamos testarrrrrr\n");
//					scanf(" %[^\n]s", buffer3);
//					sendto(sockfd, (const char *)buffer3, strlen(buffer3),
//					MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
//					len);
//					printf("vamos testarrrr\n");

				}
			}
		}
	}

	close(SocketCan);
	//fclose(Arquivo);
	close(sockfd);

	return 0;
}
