#include "AiEsp32RotaryEncoder.h"

#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#else
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#endif

#define ROTARY_ENCODER_STEPS 4
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

/*
FM radio tunner is suposed to set frequency between 88.0 MHz and 104.0 MHz by 0.1MHz steps
Rotary encoder works with integers so we will map 88.0 to 166 and then divide by 10 to get 0.1 steps
frequency = rotaryValue / 2;
*/

float getFrequency()
{
    return (float)rotaryEncoder.readEncoder() / 10.0;
}

void rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < 200)
        return;
    lastTimePressed = millis();

    Serial.print("Radio station set to ");
    Serial.print(getFrequency());
    Serial.println(" MHz ");
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
    rotaryEncoder.setBoundaries(88 * 10, 104 * 10, true); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.setAcceleration(50);
    rotaryEncoder.setEncoderValue(92.1 * 10); //set default to 92.1 MHz
    Serial.println("FM Radio");
    Serial.print("Radio station initially set to ");
    Serial.print(getFrequency());
    Serial.println(" MHz. Tune to some other station like 103.2... and press button ");
}

void loop()
{
    if (rotaryEncoder.encoderChanged())
    {
        Serial.print(getFrequency(), 1);
        Serial.println(" MHz ");
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        rotary_onButtonClick();
    }
}