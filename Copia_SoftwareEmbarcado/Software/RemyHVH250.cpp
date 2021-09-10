//============================================================================
// Name        : main.cpp
// Author      : Felipe Moura Madureira, Henrique Borges Garcia e Gaspar Henrique
// Version     : 0.0
// Copyright   : Your copyright notice
// Description : cpp file
//============================================================================

/*
 *
 *
 *
 *
 * */


#include "RemyHVH250.h"

#define NUM_MSG 4

#define    UNDEFINED		         0
#define    TEMPERATURES_1 	         0x0A0	//CHECK
#define    TEMPERATURES_2	         0x0A1	//CHECK
#define    TEMPERATURES_3 	         0x0A2	//CHECK
#define    ANALOGIC_IN 	             0x0A3	//ALMOST CHECK
#define    DIGITAL_IN		         0x0A4	//CHECK
#define    MOTOR_POSITION	         0x0A5	//CHECK
#define    CURRENT_INFO	             0x0A6	//CHECK
#define    VOLTAGE_INFO	             0x0A7  //CHECK
#define    FLUX_INFO   	             0x0A8	//CHECK
#define    INTERN_VOLTS 	         0x0A9	//CHECK
#define    INTERN_STATES	         0x0AA	//CHECK
#define    FAULT_CODES 	             0x0AB
#define    TORQUE_TIMER_INFO	     0x0AC	//CHECK
#define    MOD_FLUX_WEAK_OUT_INFO	 0x0AD	//CHECK
#define    FIRM_INFO	             0x0AE
#define    DIAGNOSTIC_DATA	         0x0AF
#define    COMMAND_MESSAGE           0x0C0	//CHECK


/**
*@Função: Processa os números negativos em lógica binária para decimal, respeitando o sinal matemático \n
*@Parâmetros: Valor em lógica binária \n
*@Retorno: Valor decimal, positivo ou negativo, conforme a lógica
*/

float NegativeValues::NegativeValuesTwoBytes(float Value){
		if(Value >= 32768){
			Value = Value - 65536;
		}
		else{
		}
		return Value;
}

/**
*@Função: Recorta um pedaço de um byte \n
*@Parâmetros: Vetor de 8 bytes, a posição do byte a ser analisado, e as posições (LS e MS) para efetuar o recorte \n
*@Retorno: Variável contendo o recorte solicitado do byte
*/


int CutBytes::CutByteInterval(unsigned char* CAN_DATA, int CanDataPosition, int MS_Position, int LS_Position, int CuttedByte){
	CuttedByte = CAN_DATA[CanDataPosition]; //verificar a questão da tipagem

	if(MS_Position == 7){
		CuttedByte = CuttedByte & 0b11111111;
	}
	if(MS_Position == 6){
		CuttedByte = CuttedByte & 0b01111111;
	}
	if(MS_Position == 5){
		CuttedByte = CuttedByte & 0b00111111;
	}
	if(MS_Position == 4){
		CuttedByte = CuttedByte & 0b00011111;
	}
	if(MS_Position == 3){
		CuttedByte = CuttedByte & 0b00001111;
	}
	if(MS_Position == 2){
		CuttedByte = CuttedByte & 0b00000111;
	}
	if(MS_Position == 1){
		CuttedByte = CuttedByte & 0b00000011;
	}
	if(MS_Position == 0){
		CuttedByte = CuttedByte & 0b00000001;
	}
	CuttedByte = (CuttedByte >> LS_Position);
	return CuttedByte;
}



/**
 * @brief Processamento do torque recebido pelo motor para conversão na forma decimal
 * @param CAN_DATA Conjunto de 8 bytes recebidos por meio da CAN
 * @param MSByte índice do byte mais signiticativo
 * @param LSByte índice do byte menos significativo
 * @return Torque processado na forma decimal
 */

float Torque::ProcessTorqueReceive(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float TorqueValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	TorqueValue = NegativeValuesTwoBytes(TorqueValue);
	TorqueValue = TorqueValue/10;
	return TorqueValue;
}

/**
 * @brief Processamento do parâmetro de fluxo recebido pelo motor
 * @param Conjunto de 8 bytes recebidos pela CAN
 * @param MSByte índice do byte mais significativo
 * @param LSByte índice do byte menos significativo
 * @return Fluxo processado na forma decimal
 */

float Flux::ProcessFluxReceive(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float FluxValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	FluxValue = NegativeValuesTwoBytes(FluxValue);
	FluxValue = FluxValue/1000;
	return FluxValue;
}

/**
 * @brief Processamento do parâmetro de ângulo recebido pelo motor
 * @param Conjunto de 8 bytes recebidos pela CAN
 * @param MSByte índice do byte mais significativo
 * @param LSByte índice do byte menos significativo
 * @return Ângulo processado na forma decimal
 */

float Angle::ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float AngleValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleValue = NegativeValuesTwoBytes(AngleValue);
	AngleValue = AngleValue/10;
	return AngleValue;

		/*if(!((*AngleValue >= -359.9) && (*AngleValue <= 359.9))){ // TODO tratamento de erros (a desenvolver)
			throw 1;
		}*/
}

/**
 * @brief Processamento do parâmetro de velocidade angular recebido pelo motor
 * @param Conjunto de 8 bytes recebidos pela CAN
 * @param MSByte índice do byte mais significativo
 * @param LSByte índice do byte menos significativo
 * @return Velocidade angular processada na forma decimal
 */

float AngleVelocity::ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float AngleVelocityValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleVelocityValue = NegativeValuesTwoBytes(AngleVelocityValue);
	//AngleVelocityValue = AngleVelocityValue/10;
	return AngleVelocityValue;
}

/**
 * @brief Processamento do parâmetro de baixa tensão recebido pelo motor
 * @param Conjunto de 8 bytes recebidos pela CAN
 * @param MSByte índice do byte mais significativo
 * @param LSByte índice do byte menos significativo
 * @return Baixa tensão processada na forma decimal
 */

float LowVoltage::ProcessLowVoltage(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float LowVoltageValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	LowVoltageValue = NegativeValuesTwoBytes(LowVoltageValue);
	LowVoltageValue = LowVoltageValue/100;
	return LowVoltageValue;
}

/**
 * @brief Processamento do parâmetro de alta tensão recebido pelo motor
 * @param Conjunto de 8 bytes recebidos pela CAN
 * @param MSByte índice do byte mais significativo
 * @param LSByte índice do byte menos significativo
 * @return Alta tensão processada na forma decimal
 */

float HighVoltage::ProcessHighVoltage(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float HighVoltageValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	HighVoltageValue = NegativeValuesTwoBytes(HighVoltageValue);
	HighVoltageValue = HighVoltageValue/10;
	return HighVoltageValue;
}

MotorPosInfo::MotorPosInfo(){
	MotorAngle             			= 0;
	MotorAngleProcessed             = 0;
	MotorSpeed            			= 0;
	MotorSpeedProcessed             = 0;
	ElectricalOutFreq      			= 0;
	DeltaResolverFiltered  			= 0;
}

/**
 * @brief Construtor da classe MotorPosInfo
 */

MotorPosInfo::MotorPosInfo(unsigned char* CAN_DATA){
	MotorAngle             			= 0;
	MotorAngleProcessed             = ProcessAngle(CAN_DATA, 1, 0);
	MotorSpeed            			= 0;
	MotorSpeedProcessed             = ProcessAngleVelocity(CAN_DATA, 3, 2);
	ElectricalOutFreq      			= 0;
	DeltaResolverFiltered  			= 0;
}


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

int MotorPosInfo::GetByte(int Position){
	return Byte[Position];
}


/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void MotorPosInfo::UpdateObject(unsigned char* CAN_DATA){
    for(int i = 0; i < 8; i++){
        Byte[i] = CAN_DATA[i];
    }

	MotorAngle             			= 0;
	MotorAngleProcessed             = this->ProcessAngle(CAN_DATA, 1, 0);
	MotorSpeed            			= 0;
	MotorSpeedProcessed             = this->ProcessAngleVelocity(CAN_DATA, 3, 2);
	ElectricalOutFreq      			= 0;
	DeltaResolverFiltered  			= 0;

}

//
void MotorPosInfo::ShowAllValuesProcessed(){
	//printf("Angle: %f\n", this->GetMotorAngleProcessed());
	//printf("Speed: %f\n", this->GetMotorSpeedProcessed());
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe MotorPosInfo; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void MotorPosInfo::IfID_MotorPosInfo(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 165){

		this->UpdateObject(frame->data);

		UDP_Package["ID"]	 = "MOTOR_POSITION";
		UDP_Package["Angle"] = MotorAngleProcessed;
		UDP_Package["Speed"] = MotorSpeedProcessed;


	}
}


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

int TorqueTimerInfo::GetByte(int Position){
	return Byte[Position];
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void TorqueTimerInfo::UpdateObject(unsigned char* CAN_DATA){
    for(int i = 0; i < 8; i++){
        Byte[i] = CAN_DATA[i];
    }

	CommandedTorqueProcessed     = this->ProcessTorqueReceive(CAN_DATA, 1, 0);
	TorqueFeedbackProcessed      = this->ProcessTorqueReceive(CAN_DATA, 3, 2);
	PowerOnTimeProcessed         = 0;
}

/**
 * @brief Printa os parâmetros CommandedTorqueProcessed, TorqueFeedbackProcessed e PowerOnTimeProcessed
 */

void TorqueTimerInfo::ShowAllValuesProcessed(){
		printf("Commanded Torque: %f\n", this->GetCommandedTorqueProcessed());
		printf("Torque Feedback: %f\n", this->GetTorqueFeedbackProcessed());
		printf("Power On Time: %f\n", this->GetPowerOnTimeProcessed());
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe TorqueTimerInfo; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void TorqueTimerInfo::IfID_TorqueTimerInfo(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 172){

		this->UpdateObject(frame->data);

		UDP_Package["ID"]	 			= "TORQUE_TIMER_INFO";
		UDP_Package["CommandedTorque"] 	= CommandedTorqueProcessed;
		UDP_Package["TorqueFeedback"]  	= TorqueFeedbackProcessed;

		//this->ShowAllValuesProcessed();


	}
}

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

int Temperature1::GetByte(int Position){
	return Byte[Position];
}


/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void Temperature1::UpdateObject(unsigned char* CAN_DATA){
    for(int i = 0; i < 8; i++){
        Byte[i] = CAN_DATA[i];
    }

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

/*
void Temperature1::ShowAllValuesProcessed(){
		printf("Temperatura do MóduloA: %f\n", this->GetModuleAProcessed());
		printf("Temperatura do MóduloB: %f\n", this->GetModuleBProcessed());
		printf("Temperatura do MóduloC: %f\n", this->GetModuleCProcessed());
		printf("Temperatura do Gate Driver Board: %f\n", this->GetGateDriverBoardProcessed());
}
 */

/**
 * @brief Verifica se o ID da CAN refere-se a classe Temperature1; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void  Temperature1::IfID_Temperature1(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 160){

		this->UpdateObject(frame->data);

		//this->ShowAllValuesProcessed();

		UDP_Package["ID"]							= "TEMPERATURES_1";
		UDP_Package["ModuleATemperature"] 			= ModuleAProcessed;
		UDP_Package["ModuleBTemperature"]  			= ModuleBProcessed;
		UDP_Package["ModuleCTemperature"]			= ModuleCProcessed;
		UDP_Package["TemperatureGateDriverBoard"] 	= GateDriverBoardProcessed;

	}
}

Temperature2::Temperature2(){
	ControlBoardTemperature 			= 0.0;
	ControlBoardTemperatureProcessed 	= 0.0;
	RTD1_Temperature 					= 0.0;
	RTD1_TemperatureProcessed 			= 0.0;
	RTD2_Temperature 					= 0.0;
	RTD2_TemperatureProcessed 			= 0.0;
	RTD3_Temperature 					= 0.0;
	RTD3_TemperatureProcessed 			= 0.0;
}

Temperature2::Temperature2(unsigned char* CAN_DATA){
	ControlBoardTemperatureProcessed 	= ProcessTorqueReceive(CAN_DATA, 1, 0);
	RTD1_TemperatureProcessed 			= ProcessTorqueReceive(CAN_DATA, 3, 2);
	RTD2_TemperatureProcessed 			= ProcessTorqueReceive(CAN_DATA, 5, 4);
	RTD3_TemperatureProcessed 			= ProcessTorqueReceive(CAN_DATA, 7, 6);
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void Temperature2::UpdateObject(unsigned char* CAN_DATA){
		ControlBoardTemperatureProcessed 	= this->ProcessTorqueReceive(CAN_DATA, 1, 0);
		RTD1_TemperatureProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 3, 2);
		RTD2_TemperatureProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 5, 4);
		RTD3_TemperatureProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 7, 6);

}

float Temperature2::GetControlBoardTemperatureProcessed(){
	return ControlBoardTemperatureProcessed;
}

float Temperature2::GetRTD1Processed(){
	return RTD1_TemperatureProcessed ;
}

float Temperature2::GetRTD2Processed(){
	return RTD2_TemperatureProcessed ;
}

float Temperature2::GetRTD3Processed(){
	return RTD3_TemperatureProcessed;
}

int Temperature2::GetByte(int Position){
	return Byte[Position];
}

/*
void Temperature2::ShowAllValuesProcessed(){
		printf("Temperatura do ControlBoardTemperature: %f\n", this->GetControlBoardTemperatureProcessed());
		printf("Temperatura do RTD1: %f\n", this->GetRTD1Processed());
		printf("Temperatura do RTD2: %f\n", this->GetRTD2Processed());
		printf("Temperatura do RTD3: %f\n", this->GetRTD3Processed());
}
*/

/**
 * @brief Verifica se o ID da CAN refere-se a classe Temperature2; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void  Temperature2::IfID_Temperature2(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 161){

		this->UpdateObject(frame->data);

		//this->ShowAllValuesProcessed();

		UDP_Package["ID"]							= "TEMPERATURES_2";
		UDP_Package["ControlBoardTemperature"] 		= ControlBoardTemperatureProcessed;
		UDP_Package["RTD1_Temperature"]  			= RTD1_TemperatureProcessed;
		UDP_Package["RTD2_Temperature"]				= RTD2_TemperatureProcessed;
		UDP_Package["RTD3_Temperature"] 			= RTD3_TemperatureProcessed;

	}
}

Temperature3::Temperature3(){
	RTD4_Temperature 			= 0.0;
	RTD4_TemperatureProcessed   = 0.0;
	RTD5_Temperature 			= 0.0;
	RTD5_TemperatureProcessed   = 0.0;
	MotorTemperature 			= 0.0;
	MotorTemperatureProcessed   = 0.0;
	TorqueShudder 				= 0.0;
	TorqueShudderProcessed 		= 0.0;
}

Temperature3::Temperature3(unsigned char* CAN_DATA){
	RTD4_TemperatureProcessed 			= ProcessTorqueReceive(CAN_DATA, 1, 0);
	RTD5_TemperatureProcessed 			= ProcessTorqueReceive(CAN_DATA, 3, 2);
	MotorTemperatureProcessed 			= ProcessTorqueReceive(CAN_DATA, 5, 4);
	TorqueShudderProcessed 				= ProcessTorqueReceive(CAN_DATA, 7, 6);
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void Temperature3::UpdateObject(unsigned char* CAN_DATA){
    for(int i = 0; i < 8; i++){
        Byte[i] = CAN_DATA[i];
    }
    RTD4_TemperatureProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 1, 0);
    RTD5_TemperatureProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 3, 2);
    MotorTemperatureProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 5, 4);
    TorqueShudderProcessed 				= this->ProcessTorqueReceive(CAN_DATA, 7, 6);

}

float Temperature3::GetRTD4_TemperatureProcessed(){
	return RTD4_TemperatureProcessed;
}

float Temperature3::GetRTD5_TemperatureProcessed(){
	return RTD5_TemperatureProcessed;
}

float Temperature3::GetMotorTemperatureProcessed(){
	return MotorTemperatureProcessed;
}

float Temperature3::GetTorqueShudderProcessed(){
	return TorqueShudderProcessed;
}

int Temperature3::GetByte(int Position){
	return Byte[Position];
}

/*
void Temperature3::ShowAllValuesProcessed(){
		printf("Temperatura do RTD4: %f\n", this->GetRTD4_TemperatureProcessed());
		printf("Temperatura do RTD5: %f\n", this->GetRTD5_TemperatureProcessed());
		printf("Temperatura do Motor: %f\n", this->GetMotorTemperatureProcessed());
		printf("Torque shudder: %f\n", this->GetTorqueShudderProcessed());
}

*/

/**
 * @brief Verifica se o ID da CAN refere-se a classe Temperature3; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void  Temperature3::IfID_Temperature3(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 162){

		this->UpdateObject(frame->data);

		//this->ShowAllValuesProcessed();

		UDP_Package["ID"]							= "TEMPERATURES_3";
		UDP_Package["RTD4_Temperature"] 			= RTD4_TemperatureProcessed;
		UDP_Package["RTD5_Temperature"]  			= RTD5_TemperatureProcessed;
		UDP_Package["MotorTemperature"]				= MotorTemperatureProcessed;
		UDP_Package["TorqueShudder"] 				= TorqueShudderProcessed;

	}
}

CurrentInformation::CurrentInformation(){
	PhaseACurrent					=	0.0;
	PhaseACurrentProcessed			=	0.0;
	PhaseBCurrent					=	0.0;
	PhaseBCurrentProcessed			=	0.0;
	PhaseCCurrent					=	0.0;
	PhaseCCurrentProcessed			=	0.0;
	DC_BusCurrent					=	0.0;
	DC_BusCurrentProcessed			=	0.0;
}


int CurrentInformation::GetByte(int Position){
	return Byte[Position];
}


CurrentInformation::CurrentInformation(unsigned char* CAN_DATA){
	PhaseACurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 1, 0);
	PhaseBCurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 3, 2);
	PhaseCCurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 5, 4);
	DC_BusCurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 7, 6);
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void CurrentInformation::UpdateObject(unsigned char* CAN_DATA){
    for(int i = 0; i < 8; i++){
        Byte[i] = GetByte(i);
    }

	PhaseACurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 1, 0);
	PhaseBCurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 3, 2);
	PhaseCCurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 5, 4);
	DC_BusCurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 7, 6);
}

float CurrentInformation::GetPhaseACurrentProcessed(){
	return PhaseACurrentProcessed;
}

float CurrentInformation::GetPhaseBCurrentProcessed(){
	return PhaseBCurrentProcessed;
}

float CurrentInformation::GetPhaseCCurrentProcessed(){
	return PhaseCCurrentProcessed;
}

float CurrentInformation::GetDCBusCurrentProcessed(){
	return DC_BusCurrentProcessed;
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe CurrentInformation; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void CurrentInformation::IfID_CurrentInformation(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 166){

		this->UpdateObject(frame->data);

		UDP_Package["ID"]					=		"CURRENT_INFORMATION";
		UDP_Package["PhaseACurrent"]		=		PhaseACurrentProcessed;
		UDP_Package["PhaseBCurrent"]		=		PhaseBCurrentProcessed;
		UDP_Package["PhaseCCurrent"]		=		PhaseCCurrentProcessed;
		UDP_Package["DC_BusCurrent"]		=		DC_BusCurrentProcessed;
	}
}

VoltageInformation::VoltageInformation(){
	DC_BusVoltage 			= 0.0;
	DC_BusVoltageProcessed  = 0.0;
	OutputVoltage 			= 0.0;
	OutputVoltageProcessed 	= 0.0;
	VAB_Vd_Voltage 			= 0.0;
	VAB_Vd_VoltageProcessed = 0.0;
	VBC_Vd_Voltage 			= 0.0;
	VBC_Vd_VoltageProcessed = 0.0;
}

int VoltageInformation::GetByte(int Position){
	return Byte[Position];
}

VoltageInformation::VoltageInformation(unsigned char* CAN_DATA){
	DC_BusVoltageProcessed 	= ProcessHighVoltage(CAN_DATA, 1, 0);
	OutputVoltageProcessed 	= ProcessHighVoltage(CAN_DATA, 3, 2);
	VAB_Vd_VoltageProcessed = ProcessHighVoltage(CAN_DATA, 5, 4);
	VBC_Vd_VoltageProcessed = ProcessHighVoltage(CAN_DATA, 7, 6);
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void VoltageInformation::UpdateObject(unsigned char* CAN_DATA){
	DC_BusVoltageProcessed 	= this->ProcessHighVoltage(CAN_DATA, 1, 0);
	OutputVoltageProcessed 	= this->ProcessHighVoltage(CAN_DATA, 3, 2);
	VAB_Vd_VoltageProcessed = this->ProcessHighVoltage(CAN_DATA, 5, 4);
	VBC_Vd_VoltageProcessed = this->ProcessHighVoltage(CAN_DATA, 7, 6);
}

float VoltageInformation::GetDCBusVoltageProcessed(){
	return DC_BusVoltageProcessed;
}

float VoltageInformation::GetOutputVoltageProcessed(){
	return OutputVoltageProcessed;
}

float VoltageInformation::GetVAB_Vd_VoltageProcessed(){
	return VAB_Vd_VoltageProcessed;
}

float VoltageInformation::GetVBC_Vd_VoltageProcessed(){
	return VBC_Vd_VoltageProcessed;
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe VoltageInformation; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void VoltageInformation::IfID_VoltageInformation(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 167){

		this->UpdateObject(frame->data);

		UDP_Package["ID"]					=		"VOLTAGE_INFORMATION";
		UDP_Package["DC_BusVoltage"]		=		DC_BusVoltageProcessed;
		UDP_Package["OutputVoltage"]		=		OutputVoltageProcessed;
		UDP_Package["VAB_Vd_Voltage"]		=		VAB_Vd_VoltageProcessed;
		UDP_Package["VBC_Vd_Voltage"]		=		VBC_Vd_VoltageProcessed;

	}
}

FluxInformation::FluxInformation(){
	FluxCommand 			= 0.0;
	FluxCommandProcessed 	= 0.0;
	FluxFeedback 			= 0.0;
	FluxFeedbackProcessed 	= 0.0;
	IdFeedback				= 0.0;
	IdFeedbackProcessed		= 0.0;
	IqFeedback				= 0.0;
	IqFeedbackProcessed		= 0.0;
}

FluxInformation::FluxInformation(unsigned char* CAN_DATA){
	FluxCommand 			= 0.0;
	FluxCommandProcessed 	= 0.0;
	FluxFeedback 			= 0.0;
	FluxFeedbackProcessed 	= 0.0;
	IdFeedback				= 0.0;
	IdFeedbackProcessed		= 0.0;
	IqFeedback				= 0.0;
	IqFeedbackProcessed		= 0.0;
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void FluxInformation::UpdateObject(unsigned char* CAN_DATA){
    for(int i = 0; i < 8; i++){
        Byte[i] = CAN_DATA[i];
    }

	FluxCommandProcessed	=	ProcessFluxReceive(CAN_DATA, 1, 0);
	FluxFeedbackProcessed	=	ProcessFluxReceive(CAN_DATA, 3, 2);
	IdFeedbackProcessed		=	ProcessTorqueReceive(CAN_DATA, 5, 4);
	IdFeedbackProcessed		=	ProcessTorqueReceive(CAN_DATA, 7, 6);
}

float FluxInformation::GetFluxCommandProcessed(){
	return FluxCommandProcessed;
}

float FluxInformation::GetFluxFeedbackProcessed(){
	return FluxFeedbackProcessed;
}

float FluxInformation::GetIdFeedbackProcessed(){
	return IdFeedbackProcessed;
}

float FluxInformation::GetIqFeedbackProcessed(){
	return IqFeedbackProcessed;
}

int FluxInformation::GetByte(int Position){
	return Byte[Position];
}


/**
 * @brief Verifica se o ID da CAN refere-se a classe FluxInformation; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void FluxInformation::IfID_FluxInformation(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 168){

		this->UpdateObject(frame->data);

		UDP_Package["ID"]				=	"FluxInformation";
		UDP_Package["FluxCommand"]		=	FluxCommandProcessed;
		UDP_Package["FluxFeedback"]		=	FluxFeedbackProcessed;
		UDP_Package["IdFeedback"]		=	IdFeedbackProcessed;
		UDP_Package["IqFeedback"]		=	IqFeedbackProcessed;

	}
}

InternalVoltages::InternalVoltages(){
	VoltageReference1Dot5 = 0.0;
	VoltageReference2Dot5 = 0.0;
	VoltageReference5Dot0 = 0.0;
	VoltageReference12	  = 0.0;
}

InternalVoltages::InternalVoltages(unsigned char* CAN_DATA){
	VoltageReference1Dot5 = 0.0;
	VoltageReference2Dot5 = 0.0;
	VoltageReference5Dot0 = 0.0;
	VoltageReference12	  = 0.0;
}

int InternalVoltages::GetByte(int Position){
	return Byte[Position];
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void InternalVoltages::UpdateObject(unsigned char* CAN_DATA){
	VoltageReference1Dot5	=	ProcessLowVoltage(CAN_DATA, 1, 0);
	VoltageReference2Dot5	=	ProcessLowVoltage(CAN_DATA, 3, 2);
	VoltageReference5Dot0	=	ProcessLowVoltage(CAN_DATA, 5, 4);
	VoltageReference12		=	ProcessLowVoltage(CAN_DATA, 7, 6);
}

float InternalVoltages::GetVoltageReference1Dot5(){
	return GetVoltageReference1Dot5();
}

float InternalVoltages::GetVoltageReference2Dot5(){
	return GetVoltageReference2Dot5();
}

float InternalVoltages::GetVoltageReference5Dot0(){
	return GetVoltageReference5Dot0();
}

float InternalVoltages::GetVoltageReference12(){
	return GetVoltageReference12();
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe InternalVoltages; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void InternalVoltages::IfID_InternalVoltages(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 169){
		this->UpdateObject(frame->data);

		UDP_Package["ID"]						=	"InternalVoltages";
		UDP_Package["VoltageReference1Dot5"]	=	VoltageReference1Dot5;
		UDP_Package["VoltageReference2Dot5"]	=	VoltageReference2Dot5;
		UDP_Package["VoltageReference5Dot0"]	=	VoltageReference5Dot0;
		UDP_Package["VoltageReference12"]	=	VoltageReference12;
	}
}



AnalogInputVoltages::AnalogInputVoltages(){
	AnalogInput1 			= 0.0;
	AnalogInput1Processed 	= 0.0;
	AnalogInput2 			= 0.0;
	AnalogInput2Processed	= 0.0;
	AnalogInput3 			= 0.0;
	AnalogInput3Processed   = 0.0;
	AnalogInput4 			= 0.0;
	AnalogInput4Processed 	= 0.0;
	AnalogInput5 			= 0.0;
	AnalogInput5Processed 	= 0.0;
	AnalogInput6 			= 0.0;
	AnalogInput6Processed 	= 0.0;
}

AnalogInputVoltages::AnalogInputVoltages(unsigned char* CAN_DATA){
	AnalogInput1Processed 	= 0.0;
	AnalogInput2Processed 	= 0.0;
	AnalogInput3Processed 	= 0.0;
	AnalogInput4Processed 	= 0.0;
	AnalogInput5Processed 	= 0.0;
	AnalogInput6Processed 	= 0.0;

}

int AnalogInputVoltages::GetByte(int Position){
	return Byte[0];
}

ModulationIndex_FluxWeakening::ModulationIndex_FluxWeakening(){
	ModulationIndex 				=	0.0;
	ModulationIndexProcessed		=	0.0;
	FluxWeakeningOutput 			=	0.0;
	FluxWeakeningOutputProcessed 	=	0.0;
	IdCommand 						=	0.0;
	IdCommandProcessed 				=	0.0;
	IqCommand 						=	0.0;
	IqCommandProcessed 				=	0.0;
}

ModulationIndex_FluxWeakening::ModulationIndex_FluxWeakening(unsigned char* CAN_DATA){
	ModulationIndex 				=	0.0;
	ModulationIndexProcessed		=	0.0;
	FluxWeakeningOutput 			=	0.0;
	FluxWeakeningOutputProcessed 	=	0.0;
	IdCommand 						=	0.0;
	IdCommandProcessed 				=	0.0;
	IqCommand 						=	0.0;
	IqCommandProcessed 				=	0.0;
}

int ModulationIndex_FluxWeakening::GetByte(int Position){
	return Byte[Position];
}


/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void ModulationIndex_FluxWeakening::UpdateObject(unsigned char* CAN_DATA){
	ModulationIndexProcessed		=	ProcessTorqueReceive(CAN_DATA, 1, 0)/10;
	FluxWeakeningOutputProcessed	=	ProcessTorqueReceive(CAN_DATA, 3, 2);
	IdCommandProcessed				=	ProcessTorqueReceive(CAN_DATA, 5, 4);
	IqCommandProcessed				=	ProcessTorqueReceive(CAN_DATA, 7, 6);
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe ModulationIndex_FluxWeakening; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void ModulationIndex_FluxWeakening::IfID_ModulationIndex_FluxWeakening(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 173){

		UpdateObject(frame->data);

		UDP_Package["ID"]						=		"ModulationIndex_FluxWeakening";
		UDP_Package["FluxWeakeningOutput"]		=		FluxWeakeningOutputProcessed;
		UDP_Package["IdCommandProcessed"]		=		IdCommandProcessed;
		UDP_Package["IqCommandProcessed"]		=		IqCommandProcessed;

	}
}


CommandMessage::CommandMessage(){


}

void CommandMessage::ProcessTorqueSend(float* Torque, int flag){
	if(flag == 0){ //Commanded Torque
		TorqueCommand =  (int) (*Torque)*10;
		if(TorqueCommand < 32768){
			TorqueCommandMSByte = 0;
			TorqueCommandLSByte = TorqueCommand;
		}
		if(TorqueCommand >= 32768){
			TorqueCommandLSByte = (TorqueCommand & 0xFF);
			TorqueCommandMSByte = (TorqueCommand >> 8);
		}
	}
	if(flag == 1){ //Torque Limit
		CommandedTorqueLimit =  (int) (*Torque)*10;
		if(CommandedTorqueLimit < 32768){
			CommandedTorqueLimitMSB = 0;
			CommandedTorqueLimitLSB = CommandedTorqueLimit;
		}
		if(CommandedTorqueLimit >= 32768){
			CommandedTorqueLimitMSB = (CommandedTorqueLimit & 0xFF);
			CommandedTorqueLimitLSB = (CommandedTorqueLimit >> 8);
		}

	}
}

void CommandMessage::SetTorqueLimit(float TorqueLimit) {
    CommandedTorqueLimit =  (int) (TorqueLimit)*10;
    if(CommandedTorqueLimit < 32768){
        CommandedTorqueLimitMSB = 0;
        CommandedTorqueLimitLSB = CommandedTorqueLimit;
    }
    if(CommandedTorqueLimit >= 32768){
        CommandedTorqueLimitMSB = (CommandedTorqueLimit & 0xFF);
        CommandedTorqueLimitLSB = (CommandedTorqueLimit >> 8);
    }
}

void CommandMessage::Process_SpeedSend(float* Speed){
	SpeedCommand =  (int) (*Speed)*10;
	SpeedCommandLSByte = (SpeedCommand & 0xFF);
	SpeedCommandMSByte = (SpeedCommand >> 8);
	
	// if(SpeedCommand < 32768){
	// 	SpeedCommandMSByte = 0;
	// 	SpeedCommandLSByte = SpeedCommand;
	// }
	// if(SpeedCommand >= 32768){
	// 	SpeedCommandLSByte = (SpeedCommand & 0xFF);
	// 	SpeedCommandMSByte = (SpeedCommand >> 8);
	// }
}

void CommandMessage::UpdateFrame(struct can_frame* frame, float Speed){
	frame->can_id = COMMAND_MESSAGE;
	Process_SpeedSend(&Speed);
	TorqueCommandLSByte = 0;
	TorqueCommandMSByte = 0;
	CommandedTorqueLimitLSB = 0;
	CommandedTorqueLimitMSB = 0;
	frame->data[0] = TorqueCommandLSByte;
	frame->data[1] = TorqueCommandMSByte;
	frame->data[2] = SpeedCommandLSByte;
	frame->data[3] = SpeedCommandMSByte;
	frame->data[6] = CommandedTorqueLimitLSB;
	frame->data[7] = CommandedTorqueLimitMSB;

}

void CommandMessage::SetInverterEnable(int Inverter_Enable, struct can_frame* frame){
	frame->can_id = COMMAND_MESSAGE;
	if(Inverter_Enable){
		frame->data[5] = frame->data[5]|0b00000001;
	}
	else{
		frame->data[5] = frame->data[5]&11111110;
	}
}

int CommandMessage::GetByte(int Position){
    return Byte[Position];
}

void CommandMessage::SetSpeedMode(struct can_frame* frame){
	
    frame->data[5] = frame->data[5] | 00010000;
}

DigitalInputStates::DigitalInputStates(){
	DigitalInput_1 = 0;
	DigitalInput_2 = 0;
	DigitalInput_3 = 0;
	DigitalInput_4 = 0;
	DigitalInput_5 = 0;
	DigitalInput_6 = 0;
	DigitalInput_7 = 0;
	DigitalInput_8 = 0;
}

void DigitalInputStates::UpdateFrame(struct can_frame* frame){
	frame->can_id = DIGITAL_IN;
	frame->data[0] = DigitalInput_1;
	frame->data[1] = DigitalInput_2;
	frame->data[2] = DigitalInput_3;
	frame->data[3] = DigitalInput_4;
	frame->data[4] = DigitalInput_5;
	frame->data[5] = DigitalInput_6;
	frame->data[6] = DigitalInput_7;
	frame->data[7] = DigitalInput_8;
}

int DigitalInputStates::GetByte(int Position){
	return Byte[Position];
}

int InternalStates::GetVSM_State(){
	return VSM_State;
}

int InternalStates::GetInverterState(){
	return InverterState;
}

int InternalStates::GetRelayState(){
	return RelayState;
}

int InternalStates::GetInverterRunMode(){
	return InverterRunMode;
}

int InternalStates::GetInverterActiveDischargeState(){
	return InverterActiveDischargeState;
}

int InternalStates::GetInverterCommandMode(){
	return InverterCommandMode;
}

int InternalStates::GetInverterEnableState(){
	return InverterEnableState;
}

int InternalStates::GetInverterEnableLockout(){
	return InverterEnableLockout;
}

int InternalStates::GetDirectionCommand(){
	return DirectionCommand;
}

int InternalStates::GetBMS_Active(){
	return BMS_Active;
}

int InternalStates::GetBMS_LimitingTorque(){
	return BMS_LimitingTorque;
}

int InternalStates::GetByte(int Position){
	return Byte[Position];
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe InternalStates; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void InternalStates::IfID_InternalStates(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 170){


		this->UpdateObject(frame->data);
		//this->ShowAllValuesProcessed();

	}
}

/**
 * @brief Atualiza os parâmetros do objeto conforme a mensagem CAN recebida
 * @param CAN_DATA Conjunto de 8 bytes recebidos pela CAN
 * @return void
 */

void InternalStates::UpdateObject(unsigned char* CAN_DATA){
	VSM_State 							= CutByteInterval(CAN_DATA, 0, 0, 0, AuxByteCut);
	InverterState 						= CutByteInterval(CAN_DATA, 2, 0, 0, AuxByteCut);
	RelayState 							= CutByteInterval(CAN_DATA, 3, 0, 0, AuxByteCut);
	InverterRunMode 					= CutByteInterval(CAN_DATA, 4, 0, 0, AuxByteCut);
	InverterActiveDischargeState 		= CutByteInterval(CAN_DATA, 4, 7, 5, AuxByteCut);
	InverterCommandMode 				= CutByteInterval(CAN_DATA, 5, 7, 0, AuxByteCut);
	InverterEnableState 				= CutByteInterval(CAN_DATA, 6, 0, 0, AuxByteCut);
	InverterEnableLockout 				= CutByteInterval(CAN_DATA, 6, 7, 7, AuxByteCut);
	DirectionCommand 					= CutByteInterval(CAN_DATA, 7, 0, 0, AuxByteCut);
	BMS_Active 							= CutByteInterval(CAN_DATA, 7, 1, 1, AuxByteCut);
	BMS_LimitingTorque 					= CutByteInterval(CAN_DATA, 7, 2, 2, AuxByteCut);

}

void InternalStates::ShowAllValuesProcessed(){
	printf("DirectionCommand: %d\n", this->GetDirectionCommand());
	printf("BMS_Active: %d\n", this->GetBMS_Active());
}

FaultErrors::FaultErrors(){
	std::array<std::pair<bool, std::string>, 64> errors{
	std::make_pair(false, "Hardware Gate/Desaturation Fault"),
	std::make_pair(false, "HW Over-current Fault"),
	std::make_pair(false, "Accelerator Shorted"),
	std::make_pair(false, "Accelerator Open"),
	std::make_pair(false, "Current Sensor Low"),
	std::make_pair(false, "Current Sensor High"),
	std::make_pair(false, "Module Temperature Low"),
	std::make_pair(false, "Module Temperature High"),

	std::make_pair(false, "Control PCB Temperature Low"),
	std::make_pair(false, "Control PCB Temperature High"),
	std::make_pair(false, "Gate Drive PCB Temperature Low"),
	std::make_pair(false, "Gate Drive PCB Temperature High"),
	std::make_pair(false, "5V Sense Voltage Low"),
	std::make_pair(false, "5V Sense Voltage High"),
	std::make_pair(false, "12V Sense Voltage Low"),
	std::make_pair(false, "12V Sense Voltage High"),

	std::make_pair(false, "2.5V Sense Voltage Low"),
	std::make_pair(false, "2.5V Sense Voltage High"),
	std::make_pair(false, "1.5V Sense Voltage Low"),
	std::make_pair(false, "1.5V Sense Voltage High"),
	std::make_pair(false, "DC Bus Voltage High"),
	std::make_pair(false, "DC Bus Voltage Low"),
	std::make_pair(false, "Pre-charge Timeout"),
	std::make_pair(false, "Pre-charge Voltage Failure"),

	std::make_pair(false, "EEPROM Checksum Invalid"),
	std::make_pair(false, "EEPROM Data Out of Range"),
	std::make_pair(false, "EEPROM Update Required"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Brake Shorted"),
	std::make_pair(false, "Brake Open"),

	std::make_pair(false, "Motor Over-speed Fault"),
	std::make_pair(false, "Over-current Fault"),
	std::make_pair(false, "Over-voltage Fault"),
	std::make_pair(false, "Inverter Over-temperature Fault"),
	std::make_pair(false, "Accelerator Input Shorted Fault"),
	std::make_pair(false, "Accelerator Input Open Fault"),
	std::make_pair(false, "Direction Command Fault"),
	std::make_pair(false, "Inverter Response Time-out Fault"),

	std::make_pair(false, "Hardware Gate/Desaturation Fault"),
	std::make_pair(false, "Hardware Over-current Fault"),
	std::make_pair(false, "Under-voltage Fault"),
	std::make_pair(false, "CAN Command Message Lost Fault"),
	std::make_pair(false, "Motor Over-temperature Fault"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),

	std::make_pair(false, "Brake Input Shorted Fault"),
	std::make_pair(false, "Brake Input Open Fault"),
	std::make_pair(false, "Module A Over-temperature Fault"),
	std::make_pair(false, "Module B Over-temperature Fault"),
	std::make_pair(false, "Module C Over-temperature Fault"),
	std::make_pair(false, "PCB Over-temperature Fault"),
	std::make_pair(false, "Gate Drive Board 1 Over-temperature Fault"),
	std::make_pair(false, "Gate Drive Board 2 Over-temperature Fault"),

	std::make_pair(false, "Gate Drive Board 3 Over-temperature Fault"),
	std::make_pair(false, "Current Sensor Fault"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Reserved"),
	std::make_pair(false, "Resolver Not Connected"),
	std::make_pair(false, "Inverter Discharge Active")

	};
}

/**
 * @brief Verifica se o ID da CAN refere-se a classe Fault Errors; caso afirmativo, chama a função de update e atualiza os campos da mensagem json
 * @param Struct referente ao can
 * @param Pacote json para posterior envio a interface
 * @return void
 */

void FaultErrors::IfId_FaultErrors(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 171){
		this->UpdateObject(frame->data);

		UDP_Package["ID"]							= "FaultErrors";
		UDP_Package["FaultCode_0"]					= Errors_Bytes[0];
		UDP_Package["FaultCode_1"]					= Errors_Bytes[1];
		UDP_Package["FaultCode_2"]					= Errors_Bytes[2];
		UDP_Package["FaultCode_3"]					= Errors_Bytes[3];
		UDP_Package["FaultCode_4"]					= Errors_Bytes[4];
		UDP_Package["FaultCode_5"]					= Errors_Bytes[5];
		UDP_Package["FaultCode_6"]					= Errors_Bytes[6];
		UDP_Package["FaultCode_7"]					= Errors_Bytes[7];
	}
}

void FaultErrors::UpdateObject(unsigned char* CAN_DATA){
	for(int i = 0; i < 7; i++){
		Errors_Bytes[i] = CAN_DATA[i];
	}

}

/*excluir depois dos testes*/
/*
void fill_can(int count)
{
	switch(count)
	{
		case(0):

			frameRead.can_id = TEMPERATURES_1;
			frameRead.data[0] = 0;
			frameRead.data[1] = 0;
			//frameRead.data[2] = 0;
			frameRead.data[2] = 5;
			frameRead.data[3] = 0;
			//frameRead.data[3] = ((SpeedTeste & 0b1111111100000000) >> 8);
			frameRead.data[4] = 30;
			frameRead.data[5] = 0;
			frameRead.data[6] = 0;
			frameRead.data[7] = 0;
			break;
		case(1):
			frameRead.can_id = TEMPERATURES_2;
			frameRead.data[0] = 0;
			frameRead.data[1] = 0;
			frameRead.data[2] = 5;
			frameRead.data[3] = 0;
			frameRead.data[4] = 30;
			frameRead.data[5] = 0;
			frameRead.data[6] = 0;
			frameRead.data[7] = 0;
			break;
		case(2):
			frameRead.can_id = TEMPERATURES_3;
			frameRead.data[0] = 0;
			frameRead.data[1] = 0;
			frameRead.data[2] = 5;
			frameRead.data[3] = 0;
			frameRead.data[4] = 30;
			frameRead.data[5] = 0;
			frameRead.data[6] = 0;
			frameRead.data[7] = 0;
			break;
		case(3):
			frameRead.can_id = MOTOR_POSITION;
			frameRead.data[0] = 0;
			frameRead.data[1] = 0;
			frameRead.data[2] = 5;
			frameRead.data[3] = 0;
			frameRead.data[4] = 30;
			frameRead.data[5] = 0;
			frameRead.data[6] = 0;
			frameRead.data[7] = 0;
			break;
		case(4):
			frameRead.can_id = CURRENT_INFO;
			frameRead.data[0] = 0;
			frameRead.data[1] = 0;
			frameRead.data[2] = 5;
			frameRead.data[3] = 0;
			frameRead.data[4] = 30;
			frameRead.data[5] = 0;
			frameRead.data[6] = 0;
			frameRead.data[7] = 0;
			break;
		case(5):
			frameRead.can_id = TORQUE_TIMER_INFO;
			frameRead.data[0] = 0;
			frameRead.data[1] = 0;
			frameRead.data[2] = 5;
			frameRead.data[3] = 0;
			frameRead.data[4] = 30;
			frameRead.data[5] = 0;
			frameRead.data[6] = 0;
			frameRead.data[7] = 0;
			break;


	}
}
*/