/* Todas as classes com respectiva seção no manual possui objetos que guardam os valores respectivos "crus", advindo do envio de dados pelo motor. Esses dados são armazenados no objeto de nome com a grandeza relacionada. */
/* Finalizado o processamento do dado (para numeros negativos e peso dos bytes), o valor "real" é armazenado no objetivo com terminação Processed.  */

class MotorPosInfo:public Angle, public AngleVelocity{
private:

	float MotorAngle;
	float MotorSpeed;
	float ElectricalOutFreq;
	float DeltaResolverFiltered;

	float MotorAngleProcessed;
	float MotorSpeedProcessed;
	float ElectricalOutFreqProcessed;
	float DeltaResolverFilteredProcessed;	

public:

	MotorPosInfo(unsigned char*);
	MotorPosInfo();

	float GetMotorAngleProcessed();
	float GetMotorSpeedProcessed();
	float GetElectricalOutFreqProcessed();
	float GetDeltaResolverFilteredProcessed();

};