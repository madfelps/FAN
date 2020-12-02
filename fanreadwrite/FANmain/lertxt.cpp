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


#include "lertxt.h"

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



void DescreveSensor(char StringDescreveSensor[][50]){
	/*
	StringDescreveSensor[0] = "Angle: ";
	StringDescreveSensor[1] = "Speed: ";
	StringDescreveSensor[2] = "Torque Command: ";
	StringDescreveSensor[3] = "TorqueFeedback: ";
	StringDescreveSensor[4] = "PowerOnTime: ";
	StringDescreveSensor[5] = "Temperatura do MóduloA: ";
	StringDescreveSensor[6] = "Temeperatura do MóduloB: ";
	StringDescreveSensor[7] = "Temperatura do MóduloC: ";
	StringDescreveSensor[8] = "Temperatura do GateDriverBoard: ";
	StringDescreveSensor[9] = "Instante da geração dos dados: ";

	*/

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



float NegativeValues::NegativeValuesTwoBytes(float Value){
		if(Value >= 32768){
			Value = Value - 65536;
		}
		else{
		}
		return Value;
}

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


float Torque::ProcessTorqueReceive(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float TorqueValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	TorqueValue = NegativeValuesTwoBytes(TorqueValue);
	TorqueValue = TorqueValue/10; 
	return TorqueValue;
}

float Flux::ProcessFluxReceive(unsigned char* CAN_DATA, int MSByte, int LSByte){
	float FluxValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	FluxValue = NegativeValuesTwoBytes(FluxValue);
	FluxValue = FluxValue/1000; 
	return FluxValue;
}


float Angle::ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleValue = NegativeValuesTwoBytes(AngleValue);
	AngleValue = AngleValue/10;
	return AngleValue;

		/*if(!((*AngleValue >= -359.9) && (*AngleValue <= 359.9))){ // TODO tratamento de erros (a desenvolver)
			throw 1;
		}*/
}

float AngleVelocity::ProcessAngleVelocity(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float AngleVelocityValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	AngleVelocityValue = NegativeValuesTwoBytes(AngleVelocityValue);
	//AngleVelocityValue = AngleVelocityValue/10;
	return AngleVelocityValue;
}

float LowVoltage::ProcessLowVoltage(unsigned char* CAN_DATA, int MSByte, int LSByte){

	float LowVoltageValue = CAN_DATA[LSByte] + CAN_DATA[MSByte] * 256;
	LowVoltageValue = NegativeValuesTwoBytes(LowVoltageValue);
	LowVoltageValue = LowVoltageValue/100;
	return LowVoltageValue;
}

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

void MotorPosInfo::UpdateObject(unsigned char* CAN_DATA){
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

void MotorPosInfo::IfID_MotorPosInfo(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 165){

		this->UpdateObject(frame->data);

		//this->ShowAllValuesProcessed();

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

void TorqueTimerInfo::UpdateObject(unsigned char* CAN_DATA){
	CommandedTorqueProcessed     = this->ProcessTorqueReceive(CAN_DATA, 1, 0);
	TorqueFeedbackProcessed      = this->ProcessTorqueReceive(CAN_DATA, 3, 2);
	PowerOnTimeProcessed         = 0;
}


void TorqueTimerInfo::ShowAllValuesProcessed(){
		printf("Commanded Torque: %f\n", this->GetCommandedTorqueProcessed());
		printf("Torque Feedback: %f\n", this->GetTorqueFeedbackProcessed());
		printf("Power On Time: %f\n", this->GetPowerOnTimeProcessed());
}

void TorqueTimerInfo::IfID_TorqueTimerInfo(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 172){

		this->UpdateObject(frame->data);

		UDP_Package["ID"]	 			= "TORQUE_TIMER_INFO";
		UDP_Package["CommandedTorque"] 	= CommandedTorqueProcessed;
		UDP_Package["TorqueFeedback"]  	= TorqueFeedbackProcessed;

		this->ShowAllValuesProcessed();


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

void Temperature1::UpdateObject(unsigned char* CAN_DATA){
		ModuleAProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 1, 0);
		ModuleBProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 3, 2);
		ModuleCProcessed 			= this->ProcessTorqueReceive(CAN_DATA, 5, 4);
		GateDriverBoardProcessed 	= this->ProcessTorqueReceive(CAN_DATA, 7, 6);

}

float Temperature1::GetControlBoardTemperatureProcessed(){
	return ModuleAProcessed;
}

float Temperature1::GetRTD1Processed(){
	return ModuleBProcessed;
}

float Temperature1::GetRTD1Processed(){
	return ModuleCProcessed;
}

float Temperature1::GetRTD1Processed(){
	return GateDriverBoardProcessed;
}

void Temperature1::ShowAllValuesProcessed(){
		printf("Temperatura do MóduloA: %f\n", this->GetModuleAProcessed());
		printf("Temperatura do MóduloB: %f\n", this->GetModuleBProcessed()); 
		printf("Temperatura do MóduloC: %f\n", this->GetModuleCProcessed());
		printf("Temperatura do Gate Driver Board: %f\n", this->GetGateDriverBoardProcessed());
}

void  Temperature1::IfID_Temperature1(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 160){

		this->UpdateObject(frame->data);
		
		//this->ShowAllValuesProcessed();

		UDP_Package["ID"]							= "TEMPERATURES_1";
		UDP_Package["TemperatureModuleA"] 			= ModuleAProcessed;
		UDP_Package["TemperatureModuleB"]  			= ModuleBProcessed;
		UDP_Package["TemperatureModuleC"]			= ModuleCProcessed;
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

void Temperature2::ShowAllValuesProcessed(){
		printf("Temperatura do ControlBoardTemperature: %f\n", this->GetControlBoardTemperatureProcessed());
		printf("Temperatura do RTD1: %f\n", this->GetRTD1Processed()); 
		printf("Temperatura do RTD2: %f\n", this->GetRTD2Processed());
		printf("Temperatura do RTD3: %f\n", this->GetRTD3Processed());
}

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

void Temperature3::UpdateObject(unsigned char* CAN_DATA){
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

void Temperature3::ShowAllValuesProcessed(){
		printf("Temperatura do RTD4: %f\n", this->GetRTD4_TemperatureProcessed());
		printf("Temperatura do RTD5: %f\n", this->GetRTD5_TemperatureProcessed()); 
		printf("Temperatura do Motor: %f\n", this->GetMotorTemperatureProcessed());
		printf("Torque shudder: %f\n", this->GetTorqueShudderProcessed());
}

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

CurrentInformation::CurrentInformation(unsigned char* CAN_DATA){
	PhaseACurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 1, 0);
	PhaseBCurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 3, 2);
	PhaseCCurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 5, 4);
	DC_BusCurrentProcessed	=	ProcessTorqueReceive(CAN_DATA, 7, 6);
}

void CurrentInformation::UpdateObject(unsigned char* CAN_DATA){
	PhaseACurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 1, 0);
	PhaseBCurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 3, 2);
	PhaseCCurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 5, 4);
	DC_BusCurrentProcessed	=	this->ProcessTorqueReceive(CAN_DATA, 7, 6);
}

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



VoltageInformation::VoltageInformation(unsigned char* CAN_DATA){
	DC_BusVoltageProcessed 	= ProcessHighVoltage(CAN_DATA, 1, 0);
	OutputVoltageProcessed 	= ProcessHighVoltage(CAN_DATA, 3, 2);
	VAB_Vd_VoltageProcessed = ProcessHighVoltage(CAN_DATA, 5, 4);
	VBC_Vd_VoltageProcessed = ProcessHighVoltage(CAN_DATA, 7, 6);
}

void VoltageInformation::UpdateObject(unsigned char* CAN_DATA){
	DC_BusVoltageProcessed 	= this->ProcessHighVoltage(CAN_DATA, 1, 0);
	OutputVoltageProcessed 	= this->ProcessHighVoltage(CAN_DATA, 3, 2);
	VAB_Vd_VoltageProcessed = this->ProcessHighVoltage(CAN_DATA, 5, 4);
	VBC_Vd_VoltageProcessed = this->ProcessHighVoltage(CAN_DATA, 7, 6);
}

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

void FluxInformation::UpdateObject(unsigned char* CAN_DATA){
	FluxCommandProcessed	=	ProcessFluxReceive(CAN_DATA, 1, 0);
	FluxFeedbackProcessed	=	ProcessFluxReceive(CAN_DATA, 3, 2);
	IdFeedbackProcessed		=	ProcessTorqueReceive(CAN_DATA, 5, 4);
	IdFeedbackProcessed		=	ProcessTorqueReceive(CAN_DATA, 7, 6);
}

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

void InternalVoltages::UpdateObject(unsigned char* CAN_DATA){
	VoltageReference1Dot5	=	ProcessLowVoltage(CAN_DATA, 1, 0);
	VoltageReference2Dot5	=	ProcessLowVoltage(CAN_DATA, 3, 2);
	VoltageReference5Dot0	=	ProcessLowVoltage(CAN_DATA, 5, 4);
	VoltageReference12		=	ProcessLowVoltage(CAN_DATA, 7, 6);
}

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

void ModulationIndex_FluxWeakening::UpdateObject(unsigned char* CAN_DATA){
	ModulationIndexProcessed		=	ProcessTorqueReceive(CAN_DATA, 1, 0)/10;
	FluxWeakeningOutputProcessed	=	ProcessTorqueReceive(CAN_DATA, 3, 2);
	IdCommandProcessed				=	ProcessTorqueReceive(CAN_DATA, 5, 4);
	IqCommandProcessed				=	ProcessTorqueReceive(CAN_DATA, 7, 6);
}

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

void CommandMessage::UpdateFrame(struct can_frame* frame){ 
	frame->can_id = COMMAND_MESSAGE;
	frame->data[0] = TorqueCommandLSByte;
	frame->data[1] = TorqueCommandMSByte;
	frame->data[6] = CommandedTorqueLimitLSB;
	frame->data[7] = CommandedTorqueLimitMSB;

}

DigitalInputStatus::DigitalInputStatus(){
	DigitalInput_1 = 0;
	DigitalInput_2 = 0;
	DigitalInput_3 = 0;
	DigitalInput_4 = 0;
	DigitalInput_5 = 0;
	DigitalInput_6 = 0;
	DigitalInput_7 = 0;
	DigitalInput_8 = 0;
}

void DigitalInputStatus::UpdateFrame(struct can_frame* frame){
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

void InternalStates::IfID_InternalStates(struct can_frame* frame, nlohmann::json& UDP_Package){
	if(frame->can_id == 170){


		this->UpdateObject(frame->data);
		//this->ShowAllValuesProcessed();

	}
}

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


