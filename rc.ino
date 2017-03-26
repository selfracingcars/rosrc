#include <ros.h>
#include <std_msgs/Int16.h>
#define PINS 4
volatile unsigned long d_start[PINS];
volatile int d_state[PINS];
volatile int d_transition[PINS];
volatile unsigned long d_cur[PINS];

ros::NodeHandle nh;

std_msgs::Int16 chan0_msg;
std_msgs::Int16 chan1_msg;
std_msgs::Int16 chan2_msg;
std_msgs::Int16 chan3_msg;
ros::Publisher chan0("rc/str", &chan0_msg);
ros::Publisher chan1("rc/thr", &chan1_msg);
ros::Publisher chan2("rc/aux1", &chan2_msg);
ros::Publisher chan3("rc/aux2", &chan3_msg);


// http://playground.arduino.cc/Main/PinChangeInterrupt
void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

// http://playground.arduino.cc/Code/TimingRollover

ISR(PCINT0_vect) {
	int pin;
	for (int i=0; i <PINS; i++ ) {
		pin = digitalRead(i+8);
		if ( pin != d_state[i] ) {
			if (pin == HIGH) {
				d_start[i] = micros();
			} else {
				d_cur[i] = micros() - d_start[i];
				d_transition[i] = 1;
			}
			d_state[i] = pin;
		}
	}
		
} 


void setup() {

	for (int i=0; i <PINS; i++ ) {
		d_start[i] = 0;
		d_state[i] = LOW;
		d_transition[i] = 0;
		d_cur[i] = 0;
		pinMode(i+8, INPUT);
		digitalWrite(i+8, HIGH);
		pciSetup(i+8);
	}
	nh.initNode();
	nh.advertise(chan0);
	nh.advertise(chan1);
	nh.advertise(chan2);
	nh.advertise(chan3);
} 


void loop() {
	for (int i=0; i <PINS; i++ ) {
		if (d_cur[i] > 0 && d_transition[i] == 1) {
			d_transition[i] = 0;
			switch (i) {
				case 0:
					chan0_msg.data = d_cur[i];
					chan0.publish( &chan0_msg );
					break;
				case 1:
					chan1_msg.data = d_cur[i];
					chan1.publish( &chan1_msg );
					break;
				case 2:
					chan2_msg.data = d_cur[i];
					chan2.publish( &chan2_msg );
					break;
				case 3:
					chan3_msg.data = d_cur[i];
					chan3.publish( &chan2_msg );
					break;
			}
		}
	}

	nh.spinOnce();
}
