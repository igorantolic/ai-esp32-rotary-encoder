// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code
// 
// 

#include "AiEsp32RotaryEncoder.h"

volatile int16_t encoder0Pos = 0;
bool _circleValues = false;
bool isEnabled = true;

uint8_t encoderAPin      = AIESP32ROTARYENCODER_DEFAULT_A_PIN;
uint8_t encoderBPin      = AIESP32ROTARYENCODER_DEFAULT_B_PIN;
uint8_t encoderButtonPin = AIESP32ROTARYENCODER_DEFAULT_BUT_PIN;
uint8_t encoderVccPin    = AIESP32ROTARYENCODER_DEFAULT_VCC_PIN;

int16_t _minEncoderValue = -1 << 15;
int16_t _maxEncoderValue = 1 << 15;

void readEncoder_ISR()
{
	if (!isEnabled) {
		return;
	}
	static uint8_t old_AB = 0;
	// grey code
	// http://hades.mech.northwestern.edu/index.php/Rotary_Encoder
	// also read up on 'Understanding Quadrature Encoded Signals'
	// https://www.pjrc.com/teensy/td_libs_Encoder.html
	// another interesting lib: https://github.com/0xPIT/encoder/blob/arduino/ClickEncoder.cpp
	static int8_t enc_states[] = { 0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0 };

	old_AB <<= 2;
	old_AB |= ((digitalRead(encoderBPin)) ? (1 << 1) : 0) | ((digitalRead(encoderAPin)) ? (1 << 0) : 0);

	encoder0Pos += (enc_states[(old_AB & 0x0f)]);

	if (encoder0Pos > _maxEncoderValue) encoder0Pos = _circleValues?_minEncoderValue : _maxEncoderValue;
	if (encoder0Pos < _minEncoderValue) encoder0Pos = _circleValues ? _maxEncoderValue : _minEncoderValue;


}


AiEsp32RotaryEncoder::AiEsp32RotaryEncoder(uint8_t encoder_APin, uint8_t encoder_BPin, uint8_t encoder_ButtonPin, uint8_t encoder_VccPin)
{
	encoderAPin = encoder_APin;
	encoderBPin = encoder_BPin;
	encoderButtonPin = encoder_ButtonPin;
	encoderVccPin = encoder_VccPin;
	pinMode(encoderAPin, INPUT);
	pinMode(encoderBPin, INPUT);
}

void AiEsp32RotaryEncoder::setBoundaries(int16_t minEncoderValue, int16_t maxEncoderValue, bool circleValues)
{
	_minEncoderValue = minEncoderValue * 2;
	_maxEncoderValue = maxEncoderValue * 2;
	_circleValues = circleValues;
}



int16_t AiEsp32RotaryEncoder::readEncoder()
{
	return encoder0Pos/2;
}

int16_t lastReadEncoder0Pos = 0;
int16_t AiEsp32RotaryEncoder::encoderChanged() {
	int16_t _encoder0Pos = readEncoder();
	
	int16_t encoder0Diff = _encoder0Pos - lastReadEncoder0Pos;

	lastReadEncoder0Pos = _encoder0Pos;
	return encoder0Diff;
}

void AiEsp32RotaryEncoder::begin()
{
	//Serial.begin(115200);
	if (encoderVccPin >= 0) {
		pinMode(encoderVccPin, OUTPUT);	digitalWrite(encoderVccPin, 1);//Vcc for encoder 
	}
	//Serial.println("Enable rotary encoder ISR:");
	// Initialize rotary encoder reading and decoding
	attachInterrupt(encoderAPin, readEncoder_ISR, CHANGE);
	attachInterrupt(encoderBPin, readEncoder_ISR, CHANGE);
	if (encoderButtonPin >= 0) {
		pinMode(encoderButtonPin, INPUT_PULLUP);
	}
}

ButtonState AiEsp32RotaryEncoder::currentButtonState()
{
	if (!isEnabled) {
		return BUT_DISABLED;
	}

	static bool previous_butt_state;
	uint8_t butt_state = !digitalRead(encoderButtonPin);


	if (butt_state && !previous_butt_state)
	{
		previous_butt_state = true;
		//Serial.println("Button Pushed");
		return BUT_PUSHED;
	}
	if (!butt_state && previous_butt_state)
	{
		previous_butt_state = false;
		//Serial.println("Button Released");
		return BUT_RELEASED; 
	}
	return (butt_state ? BUT_DOWN : BUT_UP);
}

void AiEsp32RotaryEncoder::reset(int16_t newValue) {
	newValue = newValue * 2;
	encoder0Pos = newValue;
	if (encoder0Pos > _maxEncoderValue) encoder0Pos = _circleValues?_minEncoderValue : _maxEncoderValue;
	if (encoder0Pos < _minEncoderValue) encoder0Pos = _circleValues ? _maxEncoderValue : _minEncoderValue;	
}

void AiEsp32RotaryEncoder::enable() {
	isEnabled = true;
}
void AiEsp32RotaryEncoder::disable() {
	isEnabled = false;
}
