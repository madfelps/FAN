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



#include "lertxt.h"


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
	TorqueValue = TorqueValue/10; //todo O problema do multiplicador
	return TorqueValue;
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
	AngleVelocityValue = AngleVelocityValue/10;
	return AngleVelocityValue;
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

void MotorPosInfo::ShowAllValuesProcessed(){
	//printf("Angle: %f\n", this->GetMotorAngleProcessed());
	//printf("Speed: %f\n", this->GetMotorSpeedProcessed());
}

void MotorPosInfo::IfID_MotorPosInfo(struct can_frame* frame){
	if(frame->can_id == TORQUE_TIMER_INFO){


		this->UpdateObject(frame->data);

		this->ShowAllValuesProcessed();


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
		//printf("Commanded Torque: %f\n", this->GetCommandedTorqueProcessed());
		//printf("Torque Feedback: %f\n", this->GetTorqueFeedbackProcessed());
		//printf("Power On Time: %f\n", this->GetPowerOnTimeProcessed());
}

void TorqueTimerInfo::IfID_TorqueTimerInfo(struct can_frame* frame){
	if(frame->can_id == MOTOR_POSITION){


		this->UpdateObject(frame->data);

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
		//printf("Temperatura do MóduloA: %f\n", this->GetModuleAProcessed());
		//printf("Temperatura do MóduloB: %f\n", this->GetModuleBProcessed()); 
		//printf("Temperatura do MóduloC: %f\n", this->GetModuleCProcessed());
		//printf("Temperatura do Gate Driver Board: %f\n", this->GetGateDriverBoardProcessed());
}

void  Temperature1::IfID_Temperature1(struct can_frame* frame){
	if(frame->can_id == TEMPERATURES_1){


		this->UpdateObject(frame->data);

		this->ShowAllValuesProcessed();


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
	frame->data[2] = SpeedCommandLSByte;
	frame->data[3] = SpeedCommandMSByte;
	frame->data[6] = CommandedTorqueLimitLSB;
	frame->data[7] = CommandedTorqueLimitMSB;

}

void CommandMessage::ProcessAngleVelocity(float* Speed){
	SpeedCommand = (int) *Speed;
	if(SpeedCommand < 32768){
		SpeedCommandMSByte = 0;
		SpeedCommandLSByte = SpeedCommand;
	}
	if(SpeedCommand >= 32768){
		SpeedCommandLSByte = (SpeedCommand & 0xFF); 
		SpeedCommandMSByte = (SpeedCommand >> 8); 
	}
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

void InternalStates::IfID_InternalStates(struct can_frame* frame){
	if(frame->can_id == INTERN_STATES){


		this->UpdateObject(frame->data);
		this->ShowAllValuesProcessed();

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
	//printf("DirectionCommand: %d\n", this->GetDirectionCommand());
	//printf("BMS_Active: %d\n", this->GetBMS_Active());
}


