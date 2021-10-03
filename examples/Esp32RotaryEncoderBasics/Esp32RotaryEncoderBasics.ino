#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"

/*
connecting Rotary encoder

Rotary encoder side    MICROCONTROLLER side  
-------------------    ---------------------------------------------------------------------
CLK (A pin)            any microcontroler intput pin with interrupt -> in this example pin 32
DT (B pin)             any microcontroler intput pin with interrupt -> in this example pin 21
SW (button pin)        any microcontroler intput pin with interrupt -> in this example pin 25
GND - to microcontroler GND
VCC                    microcontroler VCC (then set ROTARY_ENCODER_VCC_PIN -1) 

***OR in case VCC pin is not free you can cheat and connect:***
VCC                    any microcontroler output pin - but set also ROTARY_ENCODER_VCC_PIN 25 
                        in this example pin 25

*/
#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#else
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#endif
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void rotary_onButtonClick()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 500)
	{
		return;
	}
	lastTimePressed = millis();
	Serial.print("button pressed ");
	Serial.print(millis());
	Serial.println(" milliseconds after restart");
}

void rotary_loop()
{
	//dont print anything unless value changed
	if (rotaryEncoder.encoderChanged())
	{
		Serial.print("Value: ");
		Serial.println(rotaryEncoder.readEncoder());
	}
	if (rotaryEncoder.isEncoderButtonClicked())
	{
		rotary_onButtonClick();
	}
}

void IRAM_ATTR readEncoderISR()
{
	rotaryEncoder.readEncoder_ISR();
}

void setup()
{
	Serial.begin(115200);

	//we must initialize rotary encoder
	rotaryEncoder.begin();
	rotaryEncoder.setup(readEncoderISR);
	//set boundaries and if values should cycle or not
	//in this example we will set possible values between 0 and 1000;
	bool circleValues = false;
	rotaryEncoder.setBoundaries(0, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

	/*Rotary acceleration introduced 25.2.2021.
   * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
   * without accelerateion you need long time to get to that number
   * Using acceleration, faster you turn, faster will the value raise.
   * For fine tuning slow down.
   */
	//rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
	rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
}

void loop()
{
	//in loop call your custom function which will process rotary encoder values
	rotary_loop();
	delay(50); //or do whatever you need to do...
}