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
	inet_aton("192.168.15.12" , &servaddr.sin_addr); 
	//servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(8080); 
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
	

	struct can_frame frameRead, frameWrite; //Criação dos frames
	frameRead.can_dlc = 8;


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
    std::string auxStr;

    //Verifica se o arquivo abriu
    if(fileCAN.is_open()){
        std::cout << "Arquivo aberto!" << std::endl;
    }
    else{
        std::cout << "Arquivo nao aberto!" << std::endl;
    }

	
	#pragma omp parallel default (none) shared(sockfd) firstprivate(wordCounter, buffer3, frameRead, n, buffer, ObjMotorPosInfo, ObjTorqueTimerInfo, ObjTemperature1, ObjInternalStates, UDP_Package, MsgToClient, contador, len, cliaddr)
	{
		#pragma omp sections
		{

		 	#pragma omp section //TASK READ 

		 	{
                

		 	{ 


					while (1) 
					{

				//#pragma omp critical (mutex)
				//{
				//FlagRead = read(SocketCan, &frameRead, sizeof(struct can_frame)); // A função read retorna o número de bytes lidos
				//}

					//if(FlagRead != 0){ // Verifica se a mensagem foi lida



						//while(fileCAN >> auxStr)
						//{
							//printf("wordCounter: %d \n", wordCounter);
							//std::cout << "auxStr: " << auxStr;
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


						while(listaCAN >> auxStr)
						{
							//printf("entrou no while \n");
							if(wordCounter == 0)
							{
								frameRead.can_id = stoi(auxStr, 0, 16);
							}
							else
							{
								frameRead.data[wordCounter-1] = stoi(auxStr, 0, 16);
							}


						printf("2frameRead.can_id: %d \n", frameRead.can_id);

						wordCounter = 0;


						frameRead.can_id = 245;
						printf("%d \n", frameRead.can_id);
						frameRead.data[0] = 2;
                        frameRead.data[1] = 51;
                        frameRead.data[2] = 250;
                        frameRead.data[3] = 8;
                        frameRead.data[4] = 153;
                        frameRead.data[5] = 51;
                        frameRead.data[6] = 250;
                        frameRead.data[7] = 8;


						//GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);

						//ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
						//ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
						//ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);
						//printf("Aloooooooooo");


						//GuardaIntervaloTempo = clock();
						ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);
						//ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
						//ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
						//ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);
						printf("Aloooooooooo");


						//Envio do pacote UDP para o computador

						//UDP_Package_StdString = UDP_Package.dump();
						std::string UDP_Package_StdString = UDP_Package.dump();

						strcpy(MsgToClient, UDP_Package_StdString.c_str());
						//printf("%d\n", contador);
						printf("%s\n", MsgToClient);
						//contador++;
						sendto(sockfd, (const char *)MsgToClient, strlen(MsgToClient),
						MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
						len);


						
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

                        scanf(" %s", buffer3);
                        sendto(sockfd, ( char *)buffer3, strlen(buffer3),
                               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                               len);

				//}
					//}
					}
				

			}
		
	
			#pragma omp section //TASK WRITE
			{ 
				while(1){
					n = recvfrom(sockfd, (char *)buffer, 100, 
						MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
						&len); 
					buffer[n] = '\0'; 
					printf("Client : %s\n", buffer); 
					//ObjCommandMessage.ProcessAngleVelocity(&SpeedPretendida)
					//ObjCommandMessage.ProcessTorqueSend(&TorqueLimit, 1);
					//ObjCommandMessage.UpdateFrame(&frameWrite);
					//#pragma omp critical (mutex)
					//{
					//FlagWrite = write(SocketCan, &frameWrite, sizeof(struct can_frame));
					//}
				}
			}
		}
	}

	//close(SocketCan);
	fclose(Arquivo);
	close(sockfd);

	return 0;
}
