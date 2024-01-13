#include "Arduino.h"
#include <stdlib.h>
#include <SoftwareSerial.h>

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 9
#define ENB 10

SoftwareSerial mySerial(12,13);
char str[40];

void left_forward(uint8_t speed){
	analogWrite(ENA, speed);
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	sprintf(str, "\nCMD received lf: %d\0", speed);
	mySerial.println(str);
}

void right_forward(uint8_t speed){
	analogWrite(ENB, speed);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
	sprintf(str, "\nCMD received rf: %d\0", speed);
	mySerial.println(str);
}

void left_backward(uint8_t speed){
	analogWrite(ENA, speed);
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	sprintf(str, "\nCMD received lb: %d\0", speed);
	mySerial.println(str);
}

void right_backward(uint8_t speed){
	analogWrite(ENB, speed);
	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
	sprintf(str, "\nCMD received rb: %d\0", speed);
	mySerial.println(str);
}


void setup(){
	Serial.begin(19200);
	mySerial.begin(19200);
	mySerial.println("Initialising");
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	
	pinMode(ENA, OUTPUT);
	pinMode(ENB, OUTPUT);

	digitalWrite(IN1, HIGH); //IN1 = 1 & IN2 = 0 Forward vice versa for reverse
	digitalWrite(IN2, HIGH);
	digitalWrite(IN3, HIGH); //IN3 = 1 & IN4 = 0 Forward vice versa for reverse
	digitalWrite(IN4, HIGH);
		
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
