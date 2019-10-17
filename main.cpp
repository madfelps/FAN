//============================================================================
// Name        : main.cpp
// Author      : Felipe Madureira
// Version     : 0.0
// Copyright   : Your copyright notice
// Description : main FAN project
//============================================================================

/*
 * Algumas classes possuem metodos proprios para cortar os 8bytes: DigInStatus, TorqueTimeInfo, CommandMessage, AnaInVoltage e Internal States REVISAR ISSO POSTERIORMENTE
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

namespace ValuesUtils{

	void NegativeValuesTwoBytes(int* Value){ // para valores compostos de dois bytes, leva em conta os numeros negativos

		if(*Value >= 32768){
			*Value = *Value - 65536;
		}
		else{
		}
	}

	void ProcessTorque(int* TorqueEightBytes, int* TorqueValue){ // o segundo argumento não deveria ser um objeto?

		*TorqueValue = TorqueEightBytes[0] + TorqueEightBytes[1] * 256;
		NegativeValuesTwoBytes(TorqueValue);
		*TorqueValue = *TorqueValue/10;
	}

	void ProcessAngle(int* AngleEightBytes, int* AngleValue){

		*AngleValue = AngleEightBytes[0] + AngleEightBytes[1] * 256; //
		NegativeValuesTwoBytes(AngleValue);
		*AngleValue = *AngleValue/10;

		if(!((*AngleValue >= -359.9) && (*AngleValue <= 359.9))){ // tratamento de erros (a desenvolver)
			throw 1;
		}

	}

	/* void ProcessAngleVelocity(int* AngleVelocityTwoBytes, int* AngleVelocityValue){

		*AngleVelocityValue = AngleVelocityTwoBytes[0] + AngleVelocityTwoBytes[1] * 256;
		NegativeValuesTwoBytes(AngleVelocityValue);
		*AngleVelocityValue = *AngleVelocityValue/10;
	} */
};


enum MessageType
{
    UNDEFINED		        = 0,
    TEMPERATURES_1 	        = 0x0A0,
    TEMPERATURES_2	        = 0x0A1,
    TEMPERATURES_3 	        = 0x0A2,
    ANALOGIC_IN 	        = 0x0A3,
    DIGITAL_IN		        = 0x0A4,
    MOTOR_POSITION	        = 0x0A5,
    CURRENT_INFO	        = 0x0A6,
    VOLTAGE_INFO	        = 0x0A7,
    FLUX_INFO   	        = 0x0A8,
    INTERN_VOLTS 	        = 0x0A9,
    INTERN_STATES	        = 0x0AA,
    FAULT_CODES 	        = 0x0AB,
    TORQUE_TIMER_INFO	    = 0x0AC,
    MOD_FLUX_WEAK_OUT_INFO	= 0x0AD,
    FIRM_INFO	            = 0x0AE,
    DIAGNOSTIC_DATA	        = 0x0AF
};

/*
class Temperature1{
private:
//Lembrando que essa declaraçao coloca o inteiro como signed, para unsigned eh uint64_t
	int64_t BytesTemperature1;
	int16_t ModuleATemperature;
	int16_t ModuleBTemperature;
	int16_t ModuleCTemperature;
	int16_t GateDriverBoardTemperature;

public:

	Temperature1();
	int GetModuleATemperature();
	int GetModuleBTemperature();
	int GetModuleCTemperature();
	int GetGateDriverBoardTemperature();



};

Temperature1::Temperature1(){
	BytesTemperature1          = 0;
	ModuleATemperature         = 0; // 0, 1
	ModuleBTemperature         = 0; // 2, 3
	ModuleCTemperature         = 0; // 4, 5
	GateDriverBoardTemperature = 0; // 6, 7
}

int Temperature1::GetModuleATemperature(){
	return ModuleATemperature;
}

int Temperature1::GetModuleBTemperature(){
	return ModuleBTemperature;
}

int Temperature1::GetModuleCTemperature(){
	return ModuleCTemperature;
}

int Temperature1::GetGateDriverBoardTemperature(){
	return GateDriverBoardTemperature;
}


class Temperature2{
private:

	int64_t BytesTemperature2;
	int16_t ControlBoardTemperature;
	int16_t RTD1Temp;
	int16_t RTD2Temp;
	int16_t RTD3Temp;

public:
	Temperature2();
	int GetControlBoardTemperature();
	int GetRTD1Temp();
	int GetRTD2Temp();
	int GetRTD3Temp();

};

Temperature2::Temperature2(){
	BytesTemperature2       = 0;
	ControlBoardTemperature = 0;
	RTD1Temp                = 0;
	RTD2Temp                = 0;
	RTD3Temp                = 0;
}

int Temperature2::GetControlBoardTemperature(){
	return ControlBoardTemperature;
}

int Temperature2::GetRTD1Temp(){
	return RTD1Temp;
}

int Temperature2::GetRTD2Temp(){
	return RTD2Temp;
}

int Temperature2::GetRTD3Temp(){
	return RTD3Temp;
}

class Temperature3{
private:

	int64_t BytesTemperature3;
	int16_t RTD4Temp;
	int16_t RTD5Temp;
	int16_t MotorTemp;
	int16_t TorqueShudder;

public:

	Temperature3();
	int GetRTD4Temp();
	int GetRTD5Temp();
	int GetMotorTemp();
	int GetTorqueShudder();

};

Temperature3::Temperature3(){
	BytesTemperature3 = 0;
	RTD4Temp          = 0;
	RTD5Temp          = 0;
	MotorTemp         = 0;
	TorqueShudder     = 0;
}

int Temperature3::GetRTD4Temp(){
	return RTD4Temp;
}

int Temperature3::GetRTD5Temp(){
	return RTD5Temp;
}

int Temperature3::GetMotorTemp(){
	return MotorTemp;
}

int Temperature3::GetTorqueShudder(){
	return TorqueShudder;
}

class AnaInVoltage(){
private:
	//<1995
	int64_t BytesAnInVoltage0;
	int16_t AnIn01;
	int16_t AnIn02;
	int16_t AnIn03;
	int16_t AnIn04;

	//>1995
	int64_t BytesAnInVoltage1;
	int16_t AnIn11;
	int16_t AnIn12;
	int16_t AnIn13;
	int16_t AnIn14;
	int16_t AnIn15;
	int16_t AnIn16;
public:
	AnaInVoltage();
	void CutBytesAnaInVoltage(int* BytesEight, int* Byte1, int* Byte2, int* Byte3, int* Byte3, int* Byte4, int* Byte5, int* Byte6);
	int GetAnIn01();
	int GetAnIn02();
	int GetAnIn03();
	int GetAnIn04();
	int GetAnIn11();
	int GetAnIn12();
	int GetAnIn13();
	int GetAnIn14();
	int GetAnIn15();
	int GetAnIn16()
};

AnaInVoltage::AnaInVoltage(){

	BytesAnInVoltage0 = 0;
	AnIn01            = 0;
	AnIn02            = 0;
	AnIn03            = 0;
	AnIn04            = 0;

	BytesAnInVoltage1 = 0;
	AnIn11            = 0;
	AnIn12            = 0;
	AnIn13            = 0;
	AnIn14            = 0;
	AnIn15            = 0;
	AnIn16            = 0;

}

void CutBytesAnaInVoltage(int* BytesEight, int* Byte1, int* Byte2, int* Byte3, int* Byte3, int* Byte4, int* Byte5, int* Byte6){
	*Byte1 = (*BytesEight & 1023);
	*Byte2 = ((*ByteEight >> 10) & 1023);
	*Byte3 = ((*ByteEight >> 20) & 1023);
	*Byte4 = ((*ByteEight >> 32) & 1023);
	*Byte5 = ((*ByteEight >> 42) & 1023);
	*Byte6 = ((*ByteEight >> 52) & 1023);

}

int AnaInVoltage::GetAnIn01(){
	return AnIn01;
}

int AnaInVoltage::GetAnIn02(){
	return AnIn02;
}

int AnaInVoltage::GetAnIn03(){
	return AnIn03;
}

int AnaInVoltage::GetAnIn04(){
	return AnIn04;
}

int AnaInVoltage::GetAnIn11(){
	return AnIn11;
}

int AnaInVoltage::GetAnIn12(){
	return AnIn12;
}

int AnaInVoltage::GetAnIn13(){
	return AnIn13;
}

int AnaInVoltage::GetAnIn14(){
	return AnIn14;
}

int AnaInVoltage::GetAnIn15(){
	return AnIn15;
}

int AnaInVoltage::GetAnIn16(){
	return AnIn16;
}


class DigInStatus(){
private:

	int64_t BytesDigInStatus;
	int8_t DigIn1;
	int8_t DigIn2;
	int8_t DigIn3;
	int8_t DigIn4;
	int8_t DigIn5;
	int8_t DigIn6;
	int8_t DigIn7;
	int8_t DigIn8;

public:

	DigInStatus();
	void CutBytesDigInStatus();

};

DigInStatus::DigInStatus(){
	BytesDigInStatus = 0;
	DigIn1           = 0;
	DigIn2           = 0;
	DigIn3           = 0;
	DigIn4           = 0;
	DigIn5           = 0;
	DigIn6           = 0;
	DigIn7           = 0;
	DigIn8           = 0;
}

void CutBytesDigInStatus(int* BytesDigInStatus, int* DigIn1, int* DigIn2, int* DigIn3, int* DigIn4, int* DigIn5, int* DigIn6, int* DigIn7, int* DigIn8){
	*DigIn1 = *BytesDigInStatus & 255;
	*DigIn2 = ((*BytesDigInStatus>>8) & 255);
	*DigIn3 = ((*BytesDigInStatus>>16) & 255);
	*DigIn4 = ((*BytesDigInStatus>>24) & 255);
	*DigIn5 = ((*BytesDigInStatus>>32) & 255);
	*DigIn6 = ((*BytesDigInStatus>>40) & 255);
	*DigIn7 = ((*BytesDigInStatus>>48) & 255);
	*DigIn8 = ((*BytesDigInStatus>>56) & 255);
}

int DigInStatus::GetDigIn1(){
	return DigIn1;
}

int DigInStatus::GetDigIn2(){
	return DigIn2;
}

int DigInStatus::GetDigIn3(){
	return DigIn3;
}

int DigInStatus::GetDigIn4(){
	return DigIn4;
}

int DigInStatus::GetDigIn5(){
	return DigIn5;
}

int DigInStatus::GetDigIn6(){
	return DigIn6;
}

int DigInStatus::GetDigIn7(){
	return DigIn7;
}

int DigInStatus::GetDigIn8(){
	return DigIn8;
}
*/
class MotorPosInfo{
private:

	int16_t MotorAngle;
	int16_t MotorSpeed;
	int16_t ElectricalOutFreq;
	int16_t DeltaResolverFiltered;

public:

	MotorPosInfo();
	int GetMotorAngle();
	int GetMotorSpeed();
	int GetElectricalOutFreq();
	int GetDeltaResolverFiltered();

};

MotorPosInfo::MotorPosInfo(){
	MotorAngle            = 0;
	MotorSpeed            = 0;
	ElectricalOutFreq     = 0;
	DeltaResolverFiltered = 0;
};

int MotorPosInfo::GetMotorAngle(){
	return MotorAngle;
}

int MotorPosInfo::GetMotorSpeed(){
	return MotorSpeed;
}

int MotorPosInfo::GetElectricalOutFreq(){
	return ElectricalOutFreq;
}

int MotorPosInfo::GetDeltaResolverFiltered(){
	return DeltaResolverFiltered;
}

/*
class CurrentInfo{
private:
	int64_t BytesCurrentInfo;
	int16_t PhaseCurrentA;
	int16_t PhaseCurrentB;
	int16_t PhaseCurrentC;
	int16_t DCBusCurrent;
public:
	CurrentInfo();
	int GetPhaseCurrentA();
	int GetPhaseCurrentB();
	int GetPhaseCurrentC();
	int GetDCBusCurrent();

};

CurrentInfo::CurrentInfo(){
	BytesCurrentInfo = 0;
	PhaseCurrentA    = 0;
	PhaseCurrentB    = 0;
	PhaseCurrentC    = 0;
	DCBusCurrent     = 0;
}

int CurrentInfo::GetPhaseCurrentA(){
	return PhaseCurrentA;
}

int CurrentInfo::GetPhaseCurrentB(){
	return PhaseCurrentB;
}

int CurrentInfo::GetPhaseCurrentC(){
	return PhaseCurrentC;
}

int CurrentInfo::GetDCBusCurrent(){
	return DCBusCurrent;
}

class VoltageInfo{
private:
	int64_t BytesVoltageInfo;
	int16_t DCBusVoltage;
	int16_t OutVoltage;
	int16_t VAB_Vd_Voltage;
	int16_t VBC_Vq_Voltage;
public:
	VoltageInfo();
	int GetDCBusVoltage();
	int OutVoltage();
	int GetVAB_Vd_Voltage();
	int GetVBC_Vq_Voltage();

};

VoltageInfo::VoltageInfo(){
	BytesVoltageInfo = 0;
	DCBusVoltage     = 0;
	OutVoltage       = 0;
	VAB_Vd_Voltage   = 0;
	VBC_Vq_Voltage   = 0;
}

int VoltageInfo::GetDCBusVoltage(){
	return DCBusVoltage;
}

int VoltageInfo::GetOutVoltage(){
	return OutVoltage;
}

int VoltageInfo::GetVAB_Vd_Voltage(){
	return VAB_Vd_Voltage;
}

int VoltageInfo::GetVBC_Vq_Voltage(){
	return VBC_Vq_Voltage;
}

class FluxInfo{
private:
	int64_t BytesFluxInfo;
	int16_t FluxCommand;
	int16_t FluxFeedback;
	int16_t IdFeedback;
	int16_t IqFeedback;
public:
	FluxInfo();
	int GetFluxCommand();
	int GetFluxFeedback();
	int GetIdFeedback();
	int GetIqFeedback();
};

FluxInfo::FluxInfo(){
	BytesFluxInfo = 0;
	FluxCommand   = 0;
	FluxFeedback  = 0;
	IdFeedback    = 0;
	IqFeedback    = 0;
}

int FluxInfo::GetFluxCommand(){
	return FluxCommand;
}

int FluxInfo::GetFluxFeedback(){
	return FluxFeedback;
}

int FluxInfo::GetIdFeedback(){
	return IdFeedback;
}

int FluxInfo::GetIqFeedback(){
	return IqFeedback;
}

class InternalVoltage{
private:
	int64_t BytesInternalVoltage;
	int16_t Ref1V5Voltage;
	int16_t Ref2V5Voltage;
	int16_t Ref3V5Voltage;
	int16_t Ref4V5Voltage;
public:
	InternalVoltage();
	int GetRef1V5Voltage();
	int GetRef2V5Voltage();
	int GetRef3V5Voltage();
	int GetRef4V5Voltage();
};

InternalVoltage::InternalVoltage(){
	BytesInternalVoltage = 0;
	Ref1V5Voltage        = 0;
	Ref2V5Voltage        = 0;
	Ref3V5Voltage        = 0;
	Ref4V5Voltage        = 0;
}

int InternalVoltage::GetRef1V5Voltage(){
	return GetRef1V5Voltage;
}

int InternalVoltage::GetRef2V5Voltage(){
	return GetRef2V5Voltage;
}

int InternalVoltage::GetRef3V5Voltage(){
	return GetRef3V5Voltage;
}

int InternalVoltage::GetRef4V5Voltage(){
	return GetRef4V5Voltage;
}

// Internal States
class InternalStates{
private:
	int64_t BytesInternalStates;
	int16_t VSMState;
	int8_t InverterState;
	int8_t RelayState;
	int8_t InverterRunMode; // 4b0
	int8_t InverterActDischargeState // 4b5, 4b7
	int8_t InverterCommandMode; // 5
	int8_t InverterEnableState; // 6b7
	int8_t DirectionCommand; // 7b0
	int8_t BMSActive; // 7b1
	int8_t BMSLimitingTorque; // 7b2
public:

};

InternalStates::InternalStates(){
	BytesInternalStates       = 0;
	VSMState                  = 0;
	InverterState             = 0;
	RelayState                = 0;
	InverterRunMode           = 0;
	InverterActDischargeState = 0;
	InverterCommandMode       = 0;
	InverterEnableState       = 0;
	DirectionCommand          = 0;
	BMSActive                 = 0;
	BMSLimitingTorque         = 0;
}

int InternalStates::GetVSMState(){
	return VSMState;
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

int InternalStates::GetInverterActDischargeState(){
	return InverterActDischargeState;
}

int InternalStates::GetInverterCommandMode(){
	return InverterCommandMode;
}

int InternalStates::GetInverterEnableState(){
	return InverterEnableState;
}

int InternalStates::GetDirectionMode(){
	return DirectionMode;
}

int InternalStates::GetBMSActive(){
	return BMSActive;
}

int InternalStates::GetBMSLimitingTorque(){
	return LimitingTorque;
}

void CutBytesInternalStates(int* BytesInternalStates, int* VSMState, int* InverterState, int* RelayState, int* InverterRunMode, int* InverterActDischargeState, int* InverterCommandMode, int* InverterEnableState, int* DirectionCommand, int* BMSActive, int* BMSLimitingTorque){
	*VSMState = *BytesInternalStates & 65535;
	*InverterState = (*BytesInternalStates >> 16) & 255;
	*RelayState = (*BytesInternalStates >> 24) & 255;
	*InverterRunMode = (*BytesInternalStates >> 32) * 1;
	// DEPOIS TEM QUE TERMINAR ISSO AQUI

}

// Fault Codes
*/
class TorqueTimeInfo{
private:
	int16_t CommandedTorque;
	int16_t TorqueFeedback;
	int32_t PowerOnTime;


public:
	TorqueTimeInfo(const unsigned char* data[8]);

	int GetCommandedTorque();
	int GetTorqueFeedback();
	int GetPowerOnTime();

};

TorqueTimeInfo::TorqueTimeInfo(){

	CommandedTorque     = 0;
	TorqueFeedback      = 0;
	PowerOnTime         = 0;

}

int TorqueTimeInfo::GetCommandedTorque(){
	return CommandedTorque;
}

int TorqueTimeInfo::GetTorqueFeedback(){
	return TorqueFeedback;
}

int TorqueTimeInfo::GetPowerOnTime(){
	return PowerOnTime;
}


int main(){

	TorqueTimeInfo* TorqueTimeInfo1 = new TorqueTimeInfo();
	MotorPosInfo* MotorPosInfo1     = new MotorPosInfo();

	int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	struct sockaddr_can addr;

	struct ifreq ifr;

	strcpy(ifr.ifr_name, "can1");

	ioctl(s, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;

	addr.can_ifindex = ifr.ifr_ifindex;

	bind(s, (struct sockaddr *) &addr, sizeof(addr));

	struct can_frame frame;

	frame.can_dlc = 2; // perguntar isso aqui

	frame.data[0] = 0x41;
	frame.data[1] = 0x42;

	int nbytes = write(s, &frame, sizeof(struct can_frame)); //

	nbytes = 0;

	int msg_counter = 0;

	while (msg_counter < 10) {
		while (nbytes == 0) {
			nbytes = read(s, &frame, sizeof(struct can_frame));
		}

		printf("\n\nID: %d\n", frame.can_id);

		printf("\nDATA:");

		for (int i = 0; i < frame.can_dlc; i++) {
			printf("%d ", frame.data[i]);
		}

		printf("\n");

		msg_counter++;
	}

	close(s);

	while(1){
		if(frame.can_id == MOTOR_POSITION){
			ValuesUtils::ProcessAngle(frame.data, &MotorPosInfo1->MotorAngle);
		}

		if(frame.can_id == TORQUE_TIMER_INFO){
			ValuesUtils::ProcessTorque(frame.data, &TorqueTimeInfo1->CommandedTorque);
		}

	}

	return 0;
}
