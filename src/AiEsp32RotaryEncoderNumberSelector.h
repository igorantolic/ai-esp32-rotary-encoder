#pragma once

#include <Arduino.h>
#include "AiEsp32RotaryEncoder.h"
/*
setRange (float min, max, step)
    100,500,25
    88 , 104 , 0.05

coef = 1/step
range min/step, max/step ->  4 - 20
realValue = value*step   -> 6*25 =150

*/
//#define DEBUG_NUM_SEL

class AiEsp32RotaryEncoderNumberSelector
{
private:
    float minValue = 0;
    float maxValue = 100;
    float step = 2;
    float coeficient = 1;
    AiEsp32RotaryEncoder *encoder;

public:
    AiEsp32RotaryEncoderNumberSelector(AiEsp32RotaryEncoder *encoderInstance = NULL)
    {
        encoder = encoderInstance;
    }

    void attachEncoder(AiEsp32RotaryEncoder *encoderInstance = NULL)
    {
        encoder = encoderInstance;
    }

    void setRange(float minValue, float maxValue, float step, bool cycleValues, unsigned int decimals = 0)
    {
        this->minValue = minValue;
        this->maxValue = maxValue;
        this->coeficient = pow(10.0, decimals);
        if (maxValue < minValue)
            coeficient *= -1.0;
        this->step = step * this->coeficient;

        long minEncoderValue = (long)((this->coeficient * this->minValue) / this->step);
        long maxEncoderValue = (long)((this->coeficient * this->maxValue) / this->step);
        long range = maxEncoderValue - minEncoderValue;

        encoder->setBoundaries(minEncoderValue, maxEncoderValue, cycleValues);

        if (range < 20)
            encoder->setAcceleration(0);
        else if (range < 60)
            encoder->setAcceleration(20);
        else if (range < 200)
            encoder->setAcceleration(100);
        else if (range < 1000)
            encoder->setAcceleration(300);
        else
            encoder->setAcceleration(500);

#ifdef DEBUG_NUM_SEL
        Serial.println(minEncoderValue);
        Serial.println(maxEncoderValue);
        Serial.println(range);
        Serial.println(step);
        Serial.println(coeficient);

#endif
    }

    void setValue(float value)
    {
        long encoderValue = (long)((coeficient * value) / step);
        encoder->setEncoderValue(encoderValue);
    }

    float getValue()
    {
        float encoderValue = 1.0 * encoder->readEncoder();
        float value = encoderValue * step / coeficient;

#ifdef DEBUG_NUM_SEL
        Serial.print(encoderValue);
        Serial.print(" -> ");
        Serial.println(value);
#endif
        return value;
    }
};
