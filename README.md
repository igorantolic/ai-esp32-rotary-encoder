# Arduino Esp32 Rotary Encoder Library

This project is completly based on the (https://github.com/marcmerlin/IoTuz)
Rotary encoder code is extracted from that implementataion and some additional features are included here.


## Description

This library enables your esp32 easy implementation of rotary encoder functionality in your application.

## Installing

The downloaded code can be included as a new library into the IDE selecting the menu:

     Sketch / include Library / Add .Zip library

Restart the Arduino IDE and follow the examples located at

     File -> Examples -> Ai Esp32 Rotary Encoder

## Connecting Rotary Encoder Module to your microcontroller ESP32 (or similar)

This are instructions for "Rotary Encoder Module" which is actually Rotary Encoder with 2 resistors on board.  

CLK (A pin) - connect to any microcontroler intput pin with interrupt -> in this example pin 32

DT (B pin) - connect to any microcontroler intput pin with interrupt -> in this example pin 21

SW (button pin) - connect to any microcontroler intput pin -> in this example pin 25

VCC - for this you can two options:

a) connect to microcontroler VCC (then set ROTARY_ENCODER_VCC_PIN -1) or 

b) connect to any microcontroler output pin - in this example pin 25

GND - connect to microcontroler GND

## How to use

I suggest you should start using built in example. After installing library (and reopening Arduino IDE if required) open 

File -> Examples -> Ai Esp32 Rotary Encoder

If you didnt use suggested pins, adjust defines

#define ROTARY_ENCODER_A_PIN 32

#define ROTARY_ENCODER_B_PIN 21

#define ROTARY_ENCODER_BUTTON_PIN 25

#define ROTARY_ENCODER_VCC_PIN 27

Then upload code to microcontroller.

This example is initally set that you can adjust values 0 to 10. If you try to go beyond upper limit 10, since cycle option is set to true, it will restart from 0. Also tryinf to adjust lower than 0 will go back to 10.

If you press button first time, new limits -2 ... 2 are set. But this time cycle is set to false, so you have real minimum and maximum values.

Further pressing button will double the same limit to -4 ... 4. Then -8...8 and so on. I wonder how far will you go. Real limt is int16 limit (signed int16) which should be sufficient for most applications.

## Details

step 1) include library using 

#include "AiEsp32RotaryEncoder.h"

step 2) set pins used. Important: A and B pins must support interrupts.

#define ROTARY_ENCODER_A_PIN 32

#define ROTARY_ENCODER_B_PIN 21

#define ROTARY_ENCODER_BUTTON_PIN 25

#define ROTARY_ENCODER_VCC_PIN 27 /*put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

step 3) declare your variable like rotaryEncoder

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN);

step 4) in setup() function you should call begin method to initialize encoder.

rotaryEncoder.begin();

step 5) in loop() call some function like rotary_loop();

rotary_loop();

step 6) define function rotary_loop()

Example:

void rotary_loop() {

	//first lets handle rotary encoder button click

	if (rotaryEncoder.butEnc() == BUT_RELEASED) {

		//we can process it here or call separate function like:

		rotary_onButtonClick();

	}

	//lets see if anything changed

	int8_t encoderDelta = rotaryEncoder.encoder_changed();
	
	//optionally we can ignore whenever there is no change

	if (encoderDelta == 0) return;
	
	//for some cases we only want to know if value is increased or decreased (typically for menu items)

	if (encoderDelta>0) Serial.print("+");

	if (encoderDelta<0) Serial.print("-");

	//for other cases we want to know what is current value. Additionally often we only want if something changed

	//example: when using rotary encoder to set termostat temperature, or sound volume etc
	
	//if value is changed compared to our last read

	if (encoderDelta!=0) {

		//now we need current value

		int16_t encoderValue = rotaryEncoder.read_encoder();

		//process new value. Here is simple output.

		Serial.print("Value: ");

		Serial.println(encoderValue);

	}
	
}

step 7) if you use separate function for processing rotary encoder button click, implmement it

In step 6 we call rotary_onButtonClick();

example: 

void rotary_onButtonClick() {

	rotaryEncoder.disable();

}

In this example we disable encoder on first click on button. Dont expect any further effects before you call 

rotaryEncoder.enable();

...for obvious reasons