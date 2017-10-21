/*
 Name:		Leib_Ramp.ino
 Created:	2017/2/3 14:56:27
 Author:	Qiayuan Liao
*/
//#include "DueTimer.h"
#include "DueTimer.h"
#define SPeed_Slew 100  //target velocity  (steps per second),
#define a 100            //acceleration     (steps per second per second),
#define Frequency_Time 1000000//count of timer   (ticks per second)
#define STEPS_PER_UNIT 26.666
#define PIN_STEP 22
#define PIN_DIR 23
#define TIMER Timer3
int Distance = 200;//number of steps
int A;

float R;//constant multiplier
float Delay_Period = 0;//

int Distance_Now;//number of steps         
int	Distance_Old;
int Distance_acceleration;//number of steps

DueTimer lallal = Timer3;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(250000);
	pinMode(PIN_STEP, OUTPUT);
	pinMode(PIN_DIR, OUTPUT);

	A = a*STEPS_PER_UNIT;
	Distance = Distance*STEPS_PER_UNIT;
	Distance_acceleration = (SPeed_Slew*SPeed_Slew*STEPS_PER_UNIT*STEPS_PER_UNIT) / (2 * A);
	if (Distance_acceleration >= Distance / 2)
		Distance_acceleration = Distance / 2;
	R = A / ((float)Frequency_Time*Frequency_Time);
	Delay_Period = Frequency_Time / sqrtf(2*A);
	Timer5.attachInterrupt(draw).setFrequency(20).start();
	lallal.attachInterrupt(move).setPeriod(Delay_Period).start();

}

// the loop function runs over and over again until power down or reset
void loop() {

}


void move() {
	if (Distance_Now < Distance) {
		Distance_Now++;
		digitalWrite(PIN_STEP, HIGH);
		digitalWrite(PIN_STEP, LOW);
		if (Distance_Now < Distance_acceleration)
			Delay_Period = Delay_Period*(1 - R*Delay_Period*Delay_Period);
		else if (Distance_Now >= Distance - Distance_acceleration)
			Delay_Period = Delay_Period*(1 + R*Delay_Period*Delay_Period);
		lallal.setPeriod(Delay_Period).start();
	}
	else {
		lallal.stop();
	}
}

void draw() {//draw the distance and v
	for (int i = 0; i < 5; i++)
	{
		Serial.print(Distance_Now);
		Serial.print(",");
		Serial.println((Distance_Now - Distance_Old) * 10);
	//	Serial.println(Delay_Period,6);
	}
	Distance_Old = Distance_Now;
}
