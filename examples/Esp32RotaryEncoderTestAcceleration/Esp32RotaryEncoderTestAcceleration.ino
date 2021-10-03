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
    if (millis() - lastTimePressed < 500)
        return; //ignore multiple press in that time milliseconds
    lastTimePressed = millis();

    unsigned long acceletation = rotaryEncoder.getAcceleration() + 50;
    if (acceletation > 400)
        acceletation = 0;
    rotaryEncoder.setAcceleration(acceletation);

    Serial.print("new acceleration is ");
    Serial.println(acceletation);
    Serial.print("Try to set value: ");
    Serial.println(random(-999, 999));
    Serial.println("Set as fast as you can. If it is too hard or you suceeded, press the button again.");
}

void rotary_loop()
{
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
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    bool circleValues = false;
    rotaryEncoder.setBoundaries(-1000, 1000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.disableAcceleration();                    //acceleration is now enabled by default - disable if you dont need it
    Serial.println("Ready");
    Serial.print("Try to set value: ");
    Serial.println(752);
    Serial.println("If it is too hard press the button.");
}

void loop()
{
    rotary_loop();
    delay(50);
}