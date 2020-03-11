class Temperature1{

private:
	float ModuleA;
	float ModuleB;
	float ModuleC;
	float GateDriverBoard;
public:
	Temperature1();

	float GetModuleA();
	float GetModuleB();
	float GetModuleC();
	float GetGateDriverBoard();

};

Temperature1::Temperature1(){
	ModuleA 		= 0;
	ModuleB 		= 0;
	ModuleC 		= 0;
	GateDriverBoard = 0;
}

float Temperature1::GetModuleA(){
	return ModuleA;
}

float Temperature1::GetModuleB(){
	return ModuleB;
}

float Temperature1::GetModuleC(){
	return ModuleC;
}