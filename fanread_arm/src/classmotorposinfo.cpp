MotorPosInfo::MotorPosInfo(){
	MotorAngle             = 0;
	MotorSpeed             = 0;
	ElectricalOutFreq      = 0;
	DeltaResolverFiltered  = 0;
};

MotorPosInfo::MotorPosInfo(unsigned char* CAN_DATA){
	MotorAngle             			= 0;
	MotorAngleProcessed             = ProcessAngle(CAN_DATA, 1, 0);
	MotorSpeed            			= 0;
	MotorSpeedProcessed             = ProcessAngleVelocity(CAN_DATA, 3, 2);
	ElectricalOutFreq      			= 0;
	DeltaResolverFiltered  			= 0;
};

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