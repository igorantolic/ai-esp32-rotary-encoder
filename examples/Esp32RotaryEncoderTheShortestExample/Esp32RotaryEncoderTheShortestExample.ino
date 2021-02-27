#include "AiEsp32RotaryEncoder.h"
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_STEPS 4
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

void rotary_onButtonClick()
{
    Serial.println("button pressed");
}

void setup()
{
    Serial.begin(115200);
    rotaryEncoder.begin();
    rotaryEncoder.setup(
        [] { rotaryEncoder.readEncoder_ISR(); },
        [] { rotary_onButtonClick(); });
    rotaryEncoder.setBoundaries(0, 1000, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.setAcceleration(250);
}

void loop()
{
    if (rotaryEncoder.encoderChanged())
    {
        Serial.println(rotaryEncoder.readEncoder());
    }
}