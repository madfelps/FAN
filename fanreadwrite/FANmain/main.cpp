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
	float SpeedPretendida;
	float TorquePretendido;
	float TorqueLimit;

	char TempoEmString[10];

	clock_t GuardaIntervaloTempo;
	clock_t Temporaria;

	FILE* Arquivo;
	Arquivo = fopen("log.txt", "a");

	char StringDescreveSensor[11][10];
	char StringGuardaDadosSensor[10][10];


	//Declaração dos Objetos
	TorqueTimerInfo ObjTorqueTimerInfo;
	MotorPosInfo ObjMotorPosInfo;
	Temperature1 ObjTemperature1;
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

	inet_aton("192.168.7.2" , &servaddr.sin_addr); 

	//servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(8080); 

	/* Set address automatically if desired */
	if (argc == 2)
	{

    /* Get host name of this computer */
    gethostname(host_name, sizeof(host_name));
    hp = gethostbyname(host_name);

    /* Check for NULL pointer */
    if (hp == NULL)
    {
        fprintf(stderr, "Could not get host name.\n");
        closesocket(sd);
        WSACleanup();
        exit(0);
    }

    /* Assign the address */
    server.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
    server.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
    server.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
    server.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];
	}

	/* Otherwise assign it manually */
	else
	{
	    server.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)a1;
	    server.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)a2;
	    server.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)a3;
	    server.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)a4;
	}


	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	socklen_t len;
	len = sizeof(cliaddr); //len is value/resuslt 

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

	

	//struct can_frame frameRead, frameWrite; //Criação dos frames
	//frameRead.can_dlc = 8;


	//Configuração do arquivo para teste


	//std::ofstream Log("log.txt");
	//vector<std::string> StringDescreveSensor, StringGuardaDadosSensor;


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
   
/*
	while(fileCAN>> auxstr)
{
    
	 matrx[lin][col]  = stoi(auxstr, nullptr, 16);
    
	wordCounter++;
    col++;
    if(wordCounter == 9)
    {
        lin++;
        col = 0;
        wordCounter = 0;
    }
    
}
for(lin = 0; lin<4;lin++)
{
    for(col=0; col<9; col++)
    {
        printf("%d ",matrx[lin][col]);
    }
}

fileCAN.close();
*/

/*
    //Verifica se o arquivo abriu
    if(fileCAN.is_open()){
        std::cout << "Arquivo aberto!" << std::endl;
    }
    else{
        std::cout << "Arquivo nao aberto!" << std::endl;
    }

<<<<<<< Updated upstream
    while(fileCAN >> auxStr)
	{
		GuardaDados.push_back(stoi(auxStr, nullptr, 16));

	}
*/
 
/**/

    int OpcaoTorqueLimit;

	lin = 0;
	#pragma omp parallel default (none) shared(sockfd) firstprivate(FlagWrite, ObjCommandMessage, FlagRead, frameWrite, SocketCan, wordCounter, TorqueLimit, buffer3, frameRead, n, buffer, ObjMotorPosInfo, ObjTorqueTimerInfo, ObjTemperature1, ObjInternalStates, UDP_Package, MsgToClient, contador, len, cliaddr)

	{
		#pragma omp sections
		{

		 	#pragma omp section //TASK READ SOFTWARE-MOTOR

		 	{
                
					while (1) 
					{

				#pragma omp critical (mutex)
				{
				FlagRead = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				}

					//if(FlagRead != 0){ // Verifica se a mensagem foi lida



						//while(fileCAN >> auxStr)
						//{
							//if(wordCounter == 0)
							//{
								//frameRead.can_id = stoi(auxStr, nullptr, 16);
								//printf("1frameRead.can_id: %d\n", frameRead.can_id);

							//}
							//else
							//{
								//frameRead.data[wordCounter-1] = stoi(auxStr, nullptr, 16);
							//}

							//wordCounter++;
						//}
	
	/*	
    for(col=0; col<9; col++)
    {
        if(col == 0) frameRead.can_id = matrx[lin][col];
		else frameRead.data[col] = matrx[lin][col];
		
    }

	lin++;
	if(lin == 4) lin = 0;
	*/

                        /* while(WordFlag){
                        	if(WordCounter2 == 0){
                        		frameRead.can_id = GuardaDados[WordCounter];

                        	}
                        	else{
                        		frameRead.data[WordCounter2-1] = GuardaDados[WordCounter];
                        	}

                        	if(WordCounter2 == 9){
                        		WordFlag = 0;
                        	}

                        	WordCounter++;
                        	WordCounter2++;

                        	
                        }

                        WordCounter2 == 0;

                        for(int k = 0; k < 8; k++){
                        	printf("%d ", frameRead.data[k]);
                        }
                        */


						//GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);

						ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
						ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
						ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);
						//printf("Aloooooooooo");


						//GuardaIntervaloTempo = clock();
						//ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);
						//ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
						//ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
						//ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);

						
						//Guarda dados dos sensores na string PARA VECTOR STRING, USAR PUSH BACK

						//sprintf(StringGuardaDadosSensor[0], "%f", ObjMotorPosInfo.GetMotorAngleProcessed());
						//sprintf(StringGuardaDadosSensor[1], "%f", ObjMotorPosInfo.GetMotorSpeedProcessed());

						//sprintf(StringGuardaDadosSensor[2], "%f", ObjTorqueTimerInfo.GetCommandedTorqueProcessed());
						//sprintf(StringGuardaDadosSensor[3], "%f", ObjTorqueTimerInfo.GetTorqueFeedbackProcessed());
						//sprintf(StringGuardaDadosSensor[4], "%f", ObjTorqueTimerInfo.GetPowerOnTimeProcessed());

						//sprintf(StringGuardaDadosSensor[5], "%f", ObjTemperature1.GetModuleAProcessed());
						//sprintf(StringGuardaDadosSensor[6], "%f", ObjTemperature1.GetModuleBProcessed());
						//sprintf(StringGuardaDadosSensor[7], "%f", ObjTemperature1.GetModuleCProcessed());
						//sprintf(StringGuardaDadosSensor[8], "%f", ObjTemperature1.GetGateDriverBoardProcessed());
						//}


						//Pega o instante de tempo da geração dos dados, transforma em string e guarda na variável StringDescreveSensor.
						
						/*Temporaria = clock() - GuardaIntervaloTempo;
						strcpy(StringDescreveSensor[9], "Instante da geracao dos dados (em segundos): ");
						sprintf(TempoEmString, "%lf", (double) Temporaria*10000/CLOCKS_PER_SEC);
						strcat(StringDescreveSensor[9], TempoEmString);*/

						//Concatena os valores na string
						//for(i = 0; i < 8; i++){
							//strcat(StringDescreveSensor[i], StringGuardaDadosSensor[i]);
							//StringDescreveSensor[i] = StringDescreveSensor[i] + StringGuardaDadosSensor[i];
						//}
						//strcat(StringDescreveSensor[0], StringGuardaDadosSensor[0]);
						//strcat(StringDescreveSensor[1], StringGuardaDadosSensor[1]);
						//strcat(StringDescreveSensor[2], StringGuardaDadosSensor[2]);
						//strcat(StringDescreveSensor[3], StringGuardaDadosSensor[3]);
						//strcat(StringDescreveSensor[4], StringGuardaDadosSensor[4]);
						//strcat(StringDescreveSensor[5], StringGuardaDadosSensor[5]);
						//strcat(StringDescreveSensor[6], StringGuardaDadosSensor[6]);
						//strcat(StringDescreveSensor[7], StringGuardaDadosSensor[7]);
						//strcat(StringDescreveSensor[8], StringGuardaDadosSensor[8]);
				

						//Gera e escreve o log
						/*for(i = 0; i < 9; i++){
							Log << StringDescreveSensor[i];
							Log << "\n";
						}*/
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[0]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[1]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[2]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[3]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[4]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[5]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[6]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[7]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[8]);
						//fprintf(Arquivo, "%s\n", StringDescreveSensor[9]);
						//fprintf(Arquivo, "%s\n", "-----------------------------------------------------------------------");

                        //scanf(" %s", buffer3);
                        //sendto(sockfd, ( char *)buffer3, strlen(buffer3),
                               //MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                               //len);

				//}
					//}
					}
				

			}
		
	
			#pragma omp section //TASK WRITE MOTOR 
			{ 
				while(1){
					//ObjCommandMessage.ProcessAngleVelocity(&SpeedPretendida)
					ObjCommandMessage.ProcessTorqueSend(&TorqueLimit, 1);
					ObjCommandMessage.UpdateFrame(&frameWrite);
					#pragma omp critical (mutex)
					{
					FlagWrite = write(SocketCan, &frameWrite, sizeof(struct can_frame));
					}
					usleep(400);
				}
			}

			#pragma omp section // TASK READ INTERFACE
			{
				while(1){
					n = recvfrom(sockfd, (char *)buffer, 100, 
						MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
						&len); 
					buffer[n] = '\0'; 
					TorqueLimit = atof(buffer);
				}

			}

			#pragma omp section // TASK WRITE INTERFACE
			{
				while(1){
					//Envio do pacote UDP para o computador

					//UDP_Package_StdString = UDP_Package.dump();
					std::string UDP_Package_StdString = UDP_Package.dump();

					strcpy(MsgToClient, UDP_Package_StdString.c_str());
					//printf("%d\n", contador);
					//printf("%s\n", MsgToClient);
					//contador++;
					sendto(sockfd, (const char *)MsgToClient, strlen(MsgToClient),
					MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
					len);

				}

				

			}
		}
	}

	close(SocketCan);
	fclose(Arquivo);
	close(sockfd);

	return 0;
}
