TorqueTimerInfo::TorqueTimerInfo(){
	CommandedTorque     = 0;
	TorqueFeedback      = 0;
	PowerOnTime         = 0;

}

TorqueTimerInfo::TorqueTimerInfo(unsigned char* CAN_DATA){
	CommandedTorqueProcessed     = ProcessTorque(CAN_DATA, 1, 0);
	TorqueFeedbackProcessed      = ProcessTorque(CAN_DATA, 3, 2);
	PowerOnTimeProcessed         = 0;

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