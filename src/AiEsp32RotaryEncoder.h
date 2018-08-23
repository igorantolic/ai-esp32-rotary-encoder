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

typedef enum {
	BUT_DOWN = 0,
	BUT_PUSHED = 1,
	BUT_UP = 2,
	BUT_RELEASED = 3,
	BUT_DISABLED = 99,
} ButtonState;

class AiEsp32RotaryEncoder {
	
private:
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	volatile int16_t encoder0Pos = 0;
	bool _circleValues = false;
	bool isEnabled = true;

	uint8_t encoderAPin      = AIESP32ROTARYENCODER_DEFAULT_A_PIN;
	uint8_t encoderBPin      = AIESP32ROTARYENCODER_DEFAULT_B_PIN;
	uint8_t encoderButtonPin = AIESP32ROTARYENCODER_DEFAULT_BUT_PIN;
	uint8_t encoderVccPin    = AIESP32ROTARYENCODER_DEFAULT_VCC_PIN;

	int16_t _minEncoderValue = -1 << 15;
	int16_t _maxEncoderValue = 1 << 15;

	uint8_t old_AB;
	int16_t lastReadEncoder0Pos;
	bool previous_butt_state;

	int8_t enc_states[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
	void(*ISR_callback)();

public: 
	AiEsp32RotaryEncoder(
		uint8_t encoderAPin = AIESP32ROTARYENCODER_DEFAULT_A_PIN,
		uint8_t encoderBPin = AIESP32ROTARYENCODER_DEFAULT_B_PIN,
		uint8_t encoderButtonPin = AIESP32ROTARYENCODER_DEFAULT_BUT_PIN,
		uint8_t encoderVccPin = AIESP32ROTARYENCODER_DEFAULT_VCC_PIN
	);
	void setBoundaries(int16_t minValue = -100, int16_t maxValue = 100, bool circleValues = false);
	void IRAM_ATTR readEncoder_ISR();
	
	void setup(void (*ISR_callback)(void));
	void begin();
	void reset(int16_t newValue = 0);
	void enable();
	void disable();
	int16_t readEncoder();
	int16_t encoderChanged();
	ButtonState currentButtonState();
};
#endif

