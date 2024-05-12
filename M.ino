#include <CheapStepper.h>
CheapStepper stepper (8,9,10,11);


	//creating a char that checks if the motor should move;
	char IsActivated;

	//setting up the motor;
	void setup(){
		Serial.begin(9600);
		stepper.setRpm(12);
	}

	void LoopThrough(){
		IsActivated = Serial.read();

		if (IsActivated == '1'){
			stepper.moveDegreesCW(180);
			delay(1000);
			stepper.moveDegreesCCW(180);
		}
	}



