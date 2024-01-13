#include "Arduino.h"
#include <stdlib.h>
#include <SoftwareSerial.h>


// Digital Pins
#define MOTOR_IN1 15
#define MOTOR_IN2 14
#define MOTOR_IN3 16
#define MOTOR_IN4 17
#define LED_GREEN 2
#define LED_RED 3
#define LED_WHITE 4
#define ENCODER_A_CHANNEL_A 5
#define ENCODER_A_CHANNEL_B 6
#define ENCODER_B_CHANNEL_A 7
#define ENCODER_B_CHANNEL_B 8

// Analog Pins
#define MOTOR_ENA 9
#define MOTOR_ENB 10

SoftwareSerial mySerial(12,13);
char str[40];

void left_forward(uint8_t speed){
	analogWrite(MOTOR_ENA, speed);
	digitalWrite(MOTOR_IN1, HIGH);
	digitalWrite(MOTOR_IN2, LOW);
	sprintf(str, "\nCMD received lf: %d\0", speed);
	mySerial.println(str);
}

void right_forward(uint8_t speed){
	analogWrite(MOTOR_ENB, speed);
	digitalWrite(MOTOR_IN3, LOW);
	digitalWrite(MOTOR_IN4, HIGH);
	sprintf(str, "\nCMD received rf: %d\0", speed);
	mySerial.println(str);
}

void left_backward(uint8_t speed){
	analogWrite(MOTOR_ENA, speed);
	digitalWrite(MOTOR_IN1, LOW);
	digitalWrite(MOTOR_IN2, HIGH);
	sprintf(str, "\nCMD received lb: %d\0", speed);
	mySerial.println(str);
}

void right_backward(uint8_t speed){
	analogWrite(MOTOR_ENB, speed);
	digitalWrite(MOTOR_IN3, HIGH);
	digitalWrite(MOTOR_IN4, LOW);
	sprintf(str, "\nCMD received rb: %d\0", speed);
	mySerial.println(str);
}


// Serial.read wrapper that prints all characters read
char PrintAndRead()
{
	char input = Serial.read();
	//Serial.flush(); // Added just to experiment with waiting for string to be released all at once
	mySerial.print(input);
	return input;
}

// Get a 2 digit number from serial
int GetValueFromSerial()
{
	char num[3];
	char *ptr;

	num[0] = PrintAndRead();
	num[1] = PrintAndRead();
	num[2] = '\0';

	return strtol(num, &ptr, 10);
}


void setup(){
	Serial.begin(19200);
	mySerial.begin(19200);
	mySerial.println("Initialising");
	pinMode(MOTOR_IN1, OUTPUT);
	pinMode(MOTOR_IN2, OUTPUT);
	pinMode(MOTOR_IN3, OUTPUT);
	pinMode(MOTOR_IN4, OUTPUT);
	
	pinMode(MOTOR_ENA, OUTPUT);
	pinMode(MOTOR_ENB, OUTPUT);

	digitalWrite(MOTOR_IN1, HIGH); //MOTOR_IN1 = 1 & MOTOR_IN2 = 0 Forward vice versa for reverse
	digitalWrite(MOTOR_IN2, HIGH);
	digitalWrite(MOTOR_IN3, HIGH); //MOTOR_IN3 = 1 & MOTOR_IN4 = 0 Forward vice versa for reverse
	digitalWrite(MOTOR_IN4, HIGH);
	
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_WHITE, LOW);
}

void loop(){
	if(Serial.available() > 0){
		mySerial.println("\nReset\n");
		
		char input = PrintAndRead();
		if(input == '='){
			input = PrintAndRead();
			if(input == 'L'){
				input = PrintAndRead();
				if(input == 'F'){
					left_forward(GetValueFromSerial());
				}
				else if( input == 'B'){
					left_backward(GetValueFromSerial());          
				}
			}
			else if(input == 'R'){
				input = PrintAndRead();
				if(input == 'F'){
					right_forward(GetValueFromSerial());
				}
				else if(input == 'B'){
					right_backward(GetValueFromSerial());
				}
			}
		}
	}
}
