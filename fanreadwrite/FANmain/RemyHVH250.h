//============================================================================
// Name        : lertxt.h
// Author      : Felipe Moura Madureira, Henrique Borges Garcia e Gaspar Henrique
// Version     : 0.0
// Copyright   : Your copyright notice
// Description : main header
//============================================================================

#ifndef PROJECTFAN_LE_TXT_H

#define PROJECTFAN_LE_TXT_H

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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <fstream>
#include <omp.h>
#include "RemyHVH250.h"
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include "nlohmann/json.hpp"
#include <string.h>
#include <array>
//#include "spdlog/spdlog.h"
//#include "spdlog/sinks/basic_file_sink.h"
#include <condition_variable>

/**
 * @brief This class' proposal is to process the data received from CAN using two's complement.
 */

class NegativeValues{
private:

public:
    /**
     * @brief This function processes binary data into decimal data through the two's complement
     * @param Value to be processed through two's complement
     * @return Signed decimal data
     */
	static float NegativeValuesTwoBytes(float Value);

};

//float NegativeValues::NegativeValuesTwoBytes(float Value);

/**
 * @brief This class provides a function that slices a bit range from a byte.
 */
class CutBytes{
private:

public:
    /**
     * @brief This function slices a bit range from a byte.
     * @param CAN_DATA Pointer to can data frame received from the motor
     * @param CanDataPosition Byte position that will be sliced
     * @param MS_Position Most significant bit position for the big range
     * @param LS_Position Less significant bit position for the bit range
     * @param CuttedByte Auxiliary variable to store the sliced data
     * @return Sliced data as the parameters requeriments
     */
	int CutByteInterval(unsigned char* CAN_DATA, int CanDataPosition, int MS_Position, int LS_Position, int CuttedByte);
};

/**
 * @brief This class refers to torque parameter
 */
class Torque:public NegativeValues{
private:

public:
    /**
     * @brief This function process torque parameter through two's complement
     * @param CAN_DATA Data received from motor
     * @param MSByte Most significant byte
     * @param LSByte Less significant byte
     * @return Processed decimal torque data according two's complement
     */
	static float ProcessTorqueReceive(unsigned char* CAN_DATA, int MSByte, int LSByte);

};

/**
 * @brief This function process flux parameter through two's complement
 */
class Flux:public NegativeValues{
private:

public:
    /**
     * @brief This function process flux parameter through two's complement
     * @param CAN_DATA Data received from motor
     * @param MSByte Most significant byte
     * @param LSByte Less significant byte
     * @return Processed decimal flux data according two's complement
     */
	static float ProcessFluxReceive(unsigned char* CAN_DATA, int MSByte, int LSByte);
};

/**
 * @brief This function process Angle parameter through two's complement
 */
class Angle:public NegativeValues{
private:

public:
    /**
     * @brief This function process angle parameter through two's complement
     * @param CAN_DATA Data received from motor
     * @param MSByte Most significant byte
     * @param LSByte Less significant byte
     * @return Processed decimal angle data according two's complement
     */
	static float ProcessAngle(unsigned char* CAN_DATA, int MSByte, int LSByte);
};

/**
 * @brief This function process angle velocity parameter through two's complement
 */
class AngleVelocity:public NegativeValues{
private:

public:
    /**
     * @brief This function process angle velocity parameter through two's complement
     * @param CAN_DATA Data received from motor
     * @param MSByte Most significant byte
     * @param LSByte Less significant byte
     * @return Processed decimal angle velocity data according two's complement
     */
	static float ProcessAngleVelocity(unsigned char*, int MSByte, int LSByte);
};

/**
 * @brief This function process low voltage parameter through two's complement
 */
class LowVoltage:public NegativeValues{
private:

public:
    /**
     * @brief This function process low voltage parameter through two's complement
     * @param CAN_DATA Data received from motor
     * @param MSByte Most significant byte
     * @param LSByte Less significant byte
     * @return Processed decimal low voltage data according two's complement
     */
	static float ProcessLowVoltage(unsigned char*, int MSByte, int LSByte);
};

/**
 * @brief This function process high voltage parameter through two's complement
 */
class HighVoltage:public NegativeValues{
private:

public:
    /**
     * @brief This function process high voltage parameter through two's complement
     * @param CAN_DATA Data received from motor
     * @param MSByte Most significant byte
     * @param LSByte Less significant byte
     * @return Processed decimal high voltage data according two's complement
     */
	static float ProcessHighVoltage(unsigned char*, int MSByte, int LSByte);
};

/**
 * @brief Motor position information class
 */
class MotorPosInfo:public Angle, public AngleVelocity{
private:

	float MotorAngle; /*! Motor angle parameter without two's complement processing */
	float MotorSpeed; /*! Motor speed parameter without two's complement processing */
	float ElectricalOutFreq; /*! Electrical Out Frequency parameter without two's complement processing */
	float DeltaResolverFiltered; /*! Delta Resolver Filtered parameter without two's complement processing */

	float MotorAngleProcessed; /*! Motor angle parameter processed though two's complement concept */
	float MotorSpeedProcessed; /*! Motor speed parameter processed though two's complement concept */
	float ElectricalOutFreqProcessed; /*! Electrical out frequency parameter processed though two's complement concept */
	float DeltaResolverFilteredProcessed; /*! Delta resolver filtered parameter processed though two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol */

public:

	MotorPosInfo(unsigned char*);
	MotorPosInfo();
	/**
	 * Start the data processing according to the manual
	 * @param CAN_DATA 8 bytes data received from the motor
	 */
	void UpdateObject(unsigned char* CAN_DATA);
	/**
	 * @brief Print all the processd data from this class
	 */
	void ShowAllValuesProcessed();
	/**
	 * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
	 * @param CAN frame received from the motor
	 * @param Json packet to store the data to be sent for the graphical interface
	 */
	void IfID_MotorPosInfo(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @return Return motor angle parameter processed through two's complement concept
     */
	float GetMotorAngleProcessed();
	/**
	 * @return Return motor speed parameter processed through two's complement concept
	 */
	float GetMotorSpeedProcessed();
	/**
	 * @return Return electrical out frequency parameter processed through two's complement concept
	 */
	float GetElectricalOutFreqProcessed();
	/**
	 * @return Return delta resolver filtered parameter processed through two's complement concept
	 */
	float GetDeltaResolverFilteredProcessed();
	/**
	 * @param Position Byte position to be returned
	 * @return Byte returned from CAN_DATA
	 */
	int GetByte(int Position);

};

/**
 * @brief Torque timer information class
 */
class TorqueTimerInfo: public Torque{
private:
	int CommandedTorque; /*! Commanded torque parameter without two's complement processing */
	int TorqueFeedback; /*! Torque feedback parameter without two's complement processing */
	int PowerOnTime; /*! Power on Time parameter without two's complement processing */

	float CommandedTorqueProcessed; /*! Commanded torque parameter processed though two's complement concept */
	float TorqueFeedbackProcessed; /*! Torque feedback parameter processed though two's complement concept */
	float PowerOnTimeProcessed; /*! Motor angle parameter processed though two's complement concept */
	float Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */


public:
	TorqueTimerInfo();
    /**
	 * Start the data processing according to the manual
	 * @param CAN_DATA 8 bytes data received from the motor
	 */
	void UpdateObject(unsigned char*);
    /**
     * @brief Print all the processed data from this class
     */
	void ShowAllValuesProcessed();
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_TorqueTimerInfo(struct can_frame* frame, nlohmann::json& UDP_Package);

	float GetCommandedTorqueProcessed(); /*! Commanded Torque parameter processed though two's complement concept */
	float GetTorqueFeedbackProcessed(); /*! Torque Feedback parameter processed though two's complement concept */
	float GetPowerOnTimeProcessed(); /*! Power On Time parameter processed though two's complement concept */
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA
     */
	int GetByte(int Position);

};

class Temperature1:public Torque{

private:
	float ModuleA; /*! Module A temperature parameter without two's complement processing */
	float ModuleAProcessed; /*! Module A temperature parameter processed though two's complement concept */
	float ModuleB; /*! Module B temperature parameter without two's complement processing */
	float ModuleBProcessed; /*! Module B temperature parameter processed though two's complement concept */
	float ModuleC; /*! Module C temperature parameter without two's complement processing */
	float ModuleCProcessed; /*! Module C temperature parameter processed though two's complement concept */
	float GateDriverBoard; /*! Gate Driver temperature parameter without two's complement processing */
	float GateDriverBoardProcessed; /*! Gate Driver temperature parameter processed though two's complement concept */
	float Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	Temperature1();
	Temperature1(unsigned char* CAN_DATA);
    /**
	 * Start the data processing according to the manual
	 * @param CAN_DATA 8 bytes data received from the motor
	 */
	void UpdateObject(unsigned char*);
	/**
	 * @return Module A temperature processed through two's complement
	 */
	float GetModuleAProcessed();
    /**
     * @return Module B temperature processed through two's complement
     */
	float GetModuleBProcessed();
    /**
     * @return Module C temperature processed through two's complement
     */
	float GetModuleCProcessed();
    /**
     * @return Gate Driver Board temperature processed through two's complement
     */
	float GetGateDriverBoardProcessed();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);


	 /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void  IfID_Temperature1(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();


};

class Temperature2:public Torque{

private:
	float ControlBoardTemperature; /*! Control Board temperature parameter without two's complement processing */
	float ControlBoardTemperatureProcessed; /*! Control Board temperature parameter processed though two's complement concept */
	float RTD1_Temperature; /*! RTD1 temperature parameter without two's complement processing */
	float RTD1_TemperatureProcessed; /*! RTD1 temperature parameter processed though two's complement concept */
	float RTD2_Temperature; /*! RTD2 temperature parameter without two's complement processing */
	float RTD2_TemperatureProcessed; /*! RTD2 temperature parameter processed though two's complement concept */
	float RTD3_Temperature; /*! RTD3 temperature parameter without two's complement processing */
	float RTD3_TemperatureProcessed; /*! RTD3 temperature parameter processed though two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	Temperature2();
	Temperature2(unsigned char* CAN_DATA);
    /**
	 * Start the data processing according to the manual
	 * @param CAN_DATA 8 bytes data received from the motor
	 */
	void UpdateObject(unsigned char*);
    /**
     * @return Control Board temperature processed through two's complement
     */
	float GetControlBoardTemperatureProcessed();
    /**
     * @return RTD 1 temperature processed through two's complement
     */
	float GetRTD1Processed();
    /**
     * @return RTD 2 temperature processed through two's complement
     */
	float GetRTD2Processed();
    /**
     * @return RTD 3 temperature processed through two's complement
     */
	float GetRTD3Processed();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);

    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void  IfID_Temperature2(struct can_frame* frame, nlohmann::json& UDP_Package);
	//void ShowAllValuesProcessed();
};

class Temperature3:public Torque{

private:
	float RTD4_Temperature; /*! RTD4 temperature parameter without two's complement processing */
	float RTD4_TemperatureProcessed; /*! RTD4 temperature parameter processed through two's complement concept */
	float RTD5_Temperature; /*! RTD5 temperature parameter without two's complement processing */
	float RTD5_TemperatureProcessed; /*! RTD5 temperature parameter processed through two's complement concept */
	float MotorTemperature; /*! Motor temperature parameter without two's complement processing */
	float MotorTemperatureProcessed; /*! Motor temperature parameter processed through two's complement concept */
	float TorqueShudder; /*! Torque shudder parameter without two's complement processing */
	float TorqueShudderProcessed; /*! Torque shudder parameter processed through two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	Temperature3();
	Temperature3(unsigned char* CAN_DATA);
    /**
	 * Start the data processing according to the manual
	 * @param CAN_DATA 8 bytes data received from the motor
	 */
	void UpdateObject(unsigned char*);
    /**
     * @return RTD 4 temperature processed through two's complement
     */
	float GetRTD4_TemperatureProcessed();
    /**
     * @return RTD 5 temperature processed through two's complement
     */
	float GetRTD5_TemperatureProcessed();
    /**
     * @return Motor temperature processed through two's complement
     */
	float GetMotorTemperatureProcessed();
    /**
     * @return Torque shudder processed through two's complement
     */
	float GetTorqueShudderProcessed();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);


    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void  IfID_Temperature3(struct can_frame* frame, nlohmann::json& UDP_Package);
	//void ShowAllValuesProcessed();
};

class CurrentInformation:public Torque{
private:
	float PhaseACurrent; /*! Phase A current parameter without two's complement processing */
	float PhaseACurrentProcessed; /*! Phase A current parameter processed through two's complement concept */
	float PhaseBCurrent; /*! Phase B current parameter without two's complement processing */
	float PhaseBCurrentProcessed; /*! Phase B current temperature parameter processed through two's complement concept */
	float PhaseCCurrent; /*! Phase C current parameter without two's complement processing */
	float PhaseCCurrentProcessed; /*! Phase C current parameter processed through two's complement concept */
	float DC_BusCurrent; /*! DC Bus current parameter without two's complement processing */
	float DC_BusCurrentProcessed; /*! DC Bus current parameter processed through two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */
public:
	CurrentInformation();
	CurrentInformation(unsigned char* CAN_DATA);
    /**
     * Start the data processing according to the manual
     * @param CAN_DATA 8 bytes data received from the motor
     */
	void UpdateObject(unsigned char* CAN_DATA);
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_CurrentInformation(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @return Phase A current processed through two's complement
     */
	float GetPhaseACurrentProcessed();
    /**
     * @return Phase B current processed through two's complement
     */
	float GetPhaseBCurrentProcessed();
    /**
     * @return Phase C current processed through two's complement
     */
	float GetPhaseCCurrentProcessed();
    /**
     * @return DC Bus current processed through two's complement
     */
	float GetDCBusCurrentProcessed();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);
};

class VoltageInformation:public HighVoltage{
private:
	float DC_BusVoltage; /*! DC Bus voltage parameter without two's complement processing */
	float DC_BusVoltageProcessed; /*! DC Bus voltage parameter processed through two's complement concept */
	float OutputVoltage; /*! Output voltage parameter without two's complement processing */
	float OutputVoltageProcessed; /*! Output voltage parameter processed through two's complement concept */
	float VAB_Vd_Voltage; /*! VAB Vd voltage parameter without two's complement processing */
	float VAB_Vd_VoltageProcessed; /*! VAB Vd voltage parameter processed through two's complement concept */
	float VBC_Vd_Voltage; /*! VAB Vd voltage parameter without two's complement processing */
	float VBC_Vd_VoltageProcessed; /*! VAB Vd voltage parameter processed through two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */
public:
	VoltageInformation();
	VoltageInformation(unsigned char* CAN_DATA);
    /**
     * Start the data processing according to the manual
     * @param CAN_DATA 8 bytes data received from the motor
     */
	void UpdateObject(unsigned char* CAN_DATA);
    /**
     * @return DC Bus voltage processed through two's complement
     */
	float GetDCBusVoltageProcessed();
    /**
     * @return Output voltage processed through two's complement
     */
	float GetOutputVoltageProcessed();
    /**
     * @return VAB Vd processed through two's complement
     */
	float GetVAB_Vd_VoltageProcessed();
    /**
     * @return VBC Vd processed through two's complement
     */
	float GetVBC_Vd_VoltageProcessed();
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_VoltageInformation(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);

};

class FluxInformation:public Torque, Flux{
private:
	float FluxCommand; /*! Flux command parameter without two's complement processing */
	float FluxCommandProcessed; /*! Flux Command parameter processed through two's complement concept */
	float FluxFeedback; /*! Flux Feedback parameter without two's complement processing */
	float FluxFeedbackProcessed; /*! Flux Feedback parameter processed through two's complement concept */
	float IdFeedback; /*! Id Feedback parameter without two's complement processing */
	float IdFeedbackProcessed; /*! Id Feedback parameter processed through two's complement concept */
	float IqFeedback; /*! Iq Feedback parameter without two's complement processing */
	float IqFeedbackProcessed; /*! Iq Feedback Command parameter processed through two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */
public:
	FluxInformation();
	FluxInformation(unsigned char* CAN_DATA);
    /**
     * Start the data processing according to the manual
     * @param CAN_DATA 8 bytes data received from the motor
     */
	void UpdateObject(unsigned char* CAN_DATA);
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_FluxInformation(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @return Flux Command processed through two's complement
     */
	float GetFluxCommandProcessed();
    /**
     * @return Flux Feedback processed through two's complement
     */
	float GetFluxFeedbackProcessed();
    /**
     * @return Id Feedback processed through two's complement
     */
	float GetIdFeedbackProcessed();
    /**
     * @return Iq Feedback processed through two's complement
     */
	float GetIqFeedbackProcessed();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);
};

class InternalVoltages:public LowVoltage{
private:
	float VoltageReference1Dot5; /*! 1.5 Voltage Reference */
	float VoltageReference2Dot5; /*! 2.5 Voltage Reference */
	float VoltageReference5Dot0;  /*! 5.0 Voltage Reference */
	float VoltageReference12; /*! 12 Voltage Reference */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */
public:
	InternalVoltages();
	InternalVoltages(unsigned char* CAN_DATA);
    /**
     * Start the data processing according to the manual
     * @param CAN_DATA 8 bytes data received from the motor
     */
	void UpdateObject(unsigned char* CAN_DATA);
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_InternalVoltages(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @return 1.5V Reference
     */
	float GetVoltageReference1Dot5();
    /**
     * @return 1.5V Reference
     */
	float GetVoltageReference2Dot5();
    /**
     * @return 1.5V Reference
     */
	float GetVoltageReference5Dot0();
    /**
     * @return 12V Reference
     */
	float GetVoltageReference12();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);
};

class AnalogInputVoltages:public LowVoltage{

private:
	float AnalogInput1; /*! First analogical parameter */
	float AnalogInput1Processed; /*! First analogical parameter */
	float AnalogInput2; /*! Second analogical parameter */
	float AnalogInput2Processed; /*! Second analogical parameter */
	float AnalogInput3; /*! Third analogical parameter */
	float AnalogInput3Processed; /*! Third analogical parameter */
	float AnalogInput4; /*! Fourth analogical parameter */
	float AnalogInput4Processed; /*! Fourth analogical parameter */
	float AnalogInput5; /*! Fifth analogical parameter */
	float AnalogInput5Processed; /*! Fifth analogical parameter */
	float AnalogInput6; /*! Sixth analogical parameter */
	float AnalogInput6Processed; /*! Sixth analogical parameter */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	AnalogInputVoltages();
	AnalogInputVoltages(unsigned char* CAN_DATA);
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_AnalogInputVoltages(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);

};

class ModulationIndex_FluxWeakening:public Torque{
private:
	float ModulationIndex; /*! Modulation index parameter without two's complement processing */
	float ModulationIndexProcessed; /*! Modulation index parameter processed through two's complement concept */
	float FluxWeakeningOutput; /*! Flux Weakening Output parameter without two's complement processing */
	float FluxWeakeningOutputProcessed; /*! Flux Weakening Output parameter processed through two's complement concept */
	float IdCommand; /*! Id parameter without two's complement processing */
	float IdCommandProcessed; /*! Id parameter processed through two's complement concept */
	float IqCommand; /*! Iq parameter without two's complement processing */
	float IqCommandProcessed; /*! Iq parameter processed through two's complement concept */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	ModulationIndex_FluxWeakening();
	ModulationIndex_FluxWeakening(unsigned char* CAN_DATA);
    /**
     * Start the data processing according to the manual
     * @param CAN_DATA 8 bytes data received from the motor
     */
	void UpdateObject(unsigned char* CAN_DATA);
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_ModulationIndex_FluxWeakening(struct can_frame* frame, nlohmann::json& UDP_Package);
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);

};

//Desenvolver o método pra dar update no frame
class CommandMessage{
private:
	int TorqueCommand; /*! Torque value required by the user */
	int TorqueCommandMSByte; /*! Most significant byte from Torque processed (for later sent to the motor) */
	int TorqueCommandLSByte; /*! Less significant byte from Torque processed (for later sent to the motor) */
	int SpeedCommand; /*! Speed value required by the user */
	int SpeedCommandMSByte; /*! Most significant byte from Speed processed (for later sent to the motor) */
	int SpeedCommandLSByte; /*! Less significant byte from Speed processed (for later sent to the motor) */
	float DirectionCommand; /*! Command to set up the motor's direction */
	float InverterEnable; /*! Command to set up the inverter configuration */
	float InverterDischarge; /*! Command to set up the inverter discharge configuration */
	float SpeedModeEnable; /*! The motor can either operate as torque or speed; for default, the torque mode is enabled */
	int CommandedTorqueLimit; /*! Torque limit value required by the user */
	int CommandedTorqueLimitMSB; /*! Most significant byte from Torque limit processed (for later sent to the motor) */
	int CommandedTorqueLimitLSB; /*! Less significant byte from Torque limit processed (for later sent to the motor) */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	CommandMessage();
	/**
	 * @brief Torque processing to be send to the motor
	 * @param TorqueCommand Torque value to configure the motor
	 * @param flag The flag indicates if this torque correspond to the limit torque or the "common" torque (trocar isso)
	 */
	void ProcessTorqueSend(float* TorqueCommand, int flag);
    /**
     * @brief Speed processing to be send to the motor
     * @param SpeedCommand Speed value to configure the motor
     */
	void Process_SpeedSend(float* SpeedCommand);
    /**
     * @brief Store torque and velocity parameters through MSB and LSB values
     * @param frame frame that will be send to the motor
     */
	void UpdateFrame(struct can_frame* frame);
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);
	/**
	 * @param Inverter_Enable logical value for setting the inverter (0 - off, 1 - on)
	 * @param frame frame that will be send to the motor
	 */
	void SetInverterEnable(int Inverter_Enable, struct can_frame* frame);

};

class DigitalInputStates{
private:
	int DigitalInput_1; /*! 1° digital input according to the manual */
	int DigitalInput_2; /*! 2° digital input according to the manual */
	int DigitalInput_3; /*! 3° digital input according to the manual */
	int DigitalInput_4; /*! 4° digital input according to the manual */
	int DigitalInput_5; /*! 5° digital input according to the manual */
	int DigitalInput_6; /*! 6° digital input according to the manual */
	int DigitalInput_7; /*! 7° digital input according to the manual */
	int DigitalInput_8; /*! 8° digital input according to the manual */
	int Byte[8]; /*! Data bytes to be received from CAN protocol (little endian) */

public:
	DigitalInputStates();
	/**
	 * @brief Update the can frame's bytes according to the values from DigitalInput_X  (1 <= X <= 8)
	 * @param frame can frame to be send to the motor
	 */
	void UpdateFrame(struct can_frame* frame);
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
	int GetByte(int Position);
};



class InternalStates: public CutBytes{
private:
	int VSM_State;
	int InverterState;
	int RelayState;
	int InverterRunMode;
	int InverterActiveDischargeState;
	int InverterCommandMode;
	int InverterEnableState;
	int InverterEnableLockout;
	int DirectionCommand;
	int BMS_Active;
	int BMS_LimitingTorque;

	int AuxByteCut;
	int Byte[8];

public:
	int GetVSM_State();
	int GetInverterState();
	int GetRelayState();
	int GetInverterRunMode();
	int GetInverterActiveDischargeState();
	int GetInverterCommandMode();
	int GetInverterEnableState();
	int GetInverterEnableLockout();
	int GetDirectionCommand();
	int GetBMS_Active();
	int GetBMS_LimitingTorque();

	void UpdateObject(unsigned char* CAN_DATA);
    /**
     * @brief Verify if the ID's frame received refers to this class; if so, process according the manual
     * @param CAN frame received from the motor
     * @param Json packet to store the data to be sent for the graphical interface
     */
	void IfID_InternalStates(struct can_frame* frame, nlohmann::json& UDP_Package);
	void ShowAllValuesProcessed();
    /**
     * @param Position Byte position to be returned
     * @return Byte returned from CAN_DATA (little endian)
     */
    int GetByte(int Position);

};

/**Função: Fault Errors
*Parâmetros: Valor em lógica binária
*Retorno: Valor decimal, positivo ou negativo, conforme a lógica
*/
class FaultErrors{
private:
	std::array<std::pair<bool, std::string>, 64>Errors;
	std::array<int, 8>Errors_Bytes;

public:
	FaultErrors();
	void IfId_FaultErrors(struct can_frame* frame, nlohmann::json& UDP_Package);
	void UpdateObject(unsigned char* CAN_DATA);
};

#endif //PROJECTFAN_LE_TXT_H
