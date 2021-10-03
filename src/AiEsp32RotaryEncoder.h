// AiEsp32RotaryEncoder.h
// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code

#ifndef _AIESP32ROTARYENCODER_h
#define _AIESP32ROTARYENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Rotary Encocer
#define AIESP32ROTARYENCODER_DEFAULT_A_PIN 25
#define AIESP32ROTARYENCODER_DEFAULT_B_PIN 26
#define AIESP32ROTARYENCODER_DEFAULT_BUT_PIN 15
#define AIESP32ROTARYENCODER_DEFAULT_VCC_PIN -1
#define AIESP32ROTARYENCODER_DEFAULT_STEPS 2

typedef enum
{
	BUT_DOWN = 0,
	BUT_PUSHED = 1,
	BUT_UP = 2,
	BUT_RELEASED = 3,
	BUT_DISABLED = 99,
} ButtonState;

class AiEsp32RotaryEncoder
{

private:
#if defined(ESP8266)
#else
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	portMUX_TYPE buttonMux = portMUX_INITIALIZER_UNLOCKED;
#endif
	volatile long encoder0Pos = 0;

	volatile int8_t lastMovementDirection = 0; //1 right; -1 left
	volatile unsigned long lastMovementAt = 0;
	unsigned long rotaryAccelerationCoef = 150;

	bool _circleValues = false;
	bool isEnabled = true;

	uint8_t encoderAPin = AIESP32ROTARYENCODER_DEFAULT_A_PIN;
	uint8_t encoderBPin = AIESP32ROTARYENCODER_DEFAULT_B_PIN;
	uint8_t encoderButtonPin = AIESP32ROTARYENCODER_DEFAULT_BUT_PIN;
	uint8_t encoderVccPin = AIESP32ROTARYENCODER_DEFAULT_VCC_PIN;
	long encoderSteps = AIESP32ROTARYENCODER_DEFAULT_STEPS;

	long _minEncoderValue = -1 << 15;
	long _maxEncoderValue = 1 << 15;

	uint8_t old_AB;
	long lastReadEncoder0Pos;
	bool previous_butt_state;

	ButtonState buttonState;

	int8_t enc_states[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
	void (*ISR_callback)();
	void (*ISR_button)();

public:
	AiEsp32RotaryEncoder(
		uint8_t encoderAPin = AIESP32ROTARYENCODER_DEFAULT_A_PIN,
		uint8_t encoderBPin = AIESP32ROTARYENCODER_DEFAULT_B_PIN,
		uint8_t encoderButtonPin = AIESP32ROTARYENCODER_DEFAULT_BUT_PIN,
		uint8_t encoderVccPin = AIESP32ROTARYENCODER_DEFAULT_VCC_PIN,
		uint8_t encoderSteps = AIESP32ROTARYENCODER_DEFAULT_STEPS);
	void setBoundaries(long minValue = -100, long maxValue = 100, bool circleValues = false);
#if defined(ESP8266)
	ICACHE_RAM_ATTR void readEncoder_ISR();
	ICACHE_RAM_ATTR void readButton_ISR();
#else
	void IRAM_ATTR readEncoder_ISR();
	void IRAM_ATTR readButton_ISR();

#endif

	void setup(void (*ISR_callback)(void));
	void setup(void (*ISR_callback)(void), void (*ISR_button)(void));
	void begin();
	void reset(long newValue = 0);
	void enable();
	void disable();
	long readEncoder();
	void setEncoderValue(long newValue);
	long encoderChanged();
	ButtonState currentButtonState();
	ButtonState readButtonState();
	unsigned long getAcceleration() { return this->rotaryAccelerationCoef; }
	void setAcceleration(unsigned long acceleration) { this->rotaryAccelerationCoef = acceleration; }
	void disableAcceleration() { setAcceleration(0); }

	bool isEncoderButtonClicked(unsigned long maximumWaitMilliseconds = 300);
	bool isEncoderButtonDown();
};
#endif
