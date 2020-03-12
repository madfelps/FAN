class Temperature1:public Torque{

private:
	float ModuleA;
	float ModuleAProcessed;
	float ModuleB;
	float ModuleBProcessed;
	float ModuleC;
	float ModuleCProcessed;
	float GateDriverBoard;
	float GateDriverBoardProcessed;

public:
	Temperature1();
	Temperature1(Temperature1 Object);

	void UpdateObject(unsigned char*);
	float GetModuleAProcessed();
	float GetModuleBProcessed();
	float GetModuleCProcessed();
	float GetGateDriverBoardProcessed();


//  A grandeza física temperatura se processa de modo igual ao Torque, ou seja, essa classe também usa a classe mãe Torque
	void  IfID_Temperature1();
	void ShowAllValuesProcessed();


};

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
	ModuleAProcessed 			= ProcessTorque(CAN_DATA, 1, 0);
	ModuleBProcessed 			= ProcessTorque(CAN_DATA, 3, 2);
	ModuleCProcessed 			= ProcessTorque(CAN_DATA, 5, 4);
	GateDriverBoardProcessed 	= ProcessTorque(CAN_DATA, 7, 6);
}

void Temperature1::UpdateObject(unsigned char* CAN_DATA){
		ModuleAProcessed 			= this->ProcessTorque(CAN_DATA, 1, 0);
		ModuleBProcessed 			= this->ProcessTorque(CAN_DATA, 3, 2);
		ModuleCProcessed 			= this->ProcessTorque(CAN_DATA, 5, 4);
		GateDriverBoardProcessed 	= this->ProcessTorque(CAN_DATA, 7, 6);	
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

void Temperature1::ShowAllValuesProcessed(){
		std::cout << "Temperatura do MóduloA: " << this->ModuleAProcessed() << std::endl;
		std::cout << "Temperatura do MóduloB: " << this->ModuleBProcessed() << std::endl;
		std::cout << "Temperatura do MóduloC: " << this->ModuleCProcessed() << std::endl;
		std::cout << "Temperatura do Gate Driver Board: " << this->GateDriverBoardProcessed() << std::endl;	
}

void  Temperature1::IfID_Temperature1(){
	if(frame.can_id == TEMPERATURES_1){

		// colocar direto?
		this->UpdateObject(frame.data);

		this->ShowAllValuesProcessed();


	}
}




//////////////////////////////////////////////////////////////////////////////
/*
class Temperature2{

private:
	float ControlBoard;
	float RTD1;
	float RTD2;
	float RTD3;

public:
	Temperature2();

	float GetControlBoard();
	float GetRTD1();
	float GetRTD2();
	float GetRTD3();

};

Temperature2::Temperature2(){
	ControlBoard 		= 0;
	RTD1 				= 0;
	RTD2 				= 0;
	RTD3 				= 0;
}

float Temperature2::GetControlBoard(){
	return ControlBoard;
}

float Temperature2::GetRTD1(){
	return RTD1;
}

float Temperature2::GetRTD2(){
	return RTD2;
}

float Temperature2::GetRTD3(){
	return RTD3;
}


/////////////////////////////////

class Temperature3{

private:
	float RTD4;
	float RTD5;
	float Motor;
	float TorqueShudder;

public:
	Temperature3();

	float GetRTD4();
	float GetRTD5();
	float GetMotor();
	float GetTorqueShudder();

};

Temperature3::Temperature3(){
	RTD4 				= 0;
	RTD5 				= 0;
	Motor 				= 0;
	TorqueShudder 		= 0;
}

float Temperature3::GetRTD4(){
	return RTD4;
}

float Temperature3::GetRTD5(){
	return RTD5;
}

float Temperature3::GetMotor(){
	return Motor;
}

float Temperature3::GetTorqueShudder(){
	return TorqueShudder;
}

//////////////////////////////////////////

*/