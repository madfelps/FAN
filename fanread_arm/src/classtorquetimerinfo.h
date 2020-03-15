class TorqueTimerInfo: public Torque{
private:
	int CommandedTorque;
	int TorqueFeedback;
	int PowerOnTime;

	float CommandedTorqueProcessed;
	float TorqueFeedbackProcessed;
	float PowerOnTimeProcessed;


public:
	TorqueTimerInfo(unsigned char*);
	TorqueTimerInfo();

	float GetCommandedTorqueProcessed();
	float GetTorqueFeedbackProcessed();
	float GetPowerOnTimeProcessed();

};