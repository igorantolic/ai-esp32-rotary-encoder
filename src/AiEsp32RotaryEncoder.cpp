// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code
// 
// 

#include "esp_log.h"
#define LOG_TAG "AiEsp32RotaryEncoder"

#include "AiEsp32RotaryEncoder.h"

void IRAM_ATTR AiEsp32RotaryEncoder::readEncoder_ISR()
{		
	portENTER_CRITICAL_ISR(&(this->mux));
	if (this->isEnabled) {
		// code from https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
		/**/
		this->old_AB <<= 2;                   //remember previous state

		int8_t ENC_PORT = ((digitalRead(this->encoderBPin)) ? (1 << 1) : 0) | ((digitalRead(this->encoderAPin)) ? (1 << 0) : 0);
				
		this->old_AB |= ( ENC_PORT & 0x03 );  //add current state

		this->encoder0Pos += ( this->enc_states[( this->old_AB & 0x0f )]);	

		if (this->encoder0Pos > (this->_maxEncoderValue))
			this->encoder0Pos = this->_circleValues ? this->_minEncoderValue : this->_maxEncoderValue;
		if (this->encoder0Pos < (this->_minEncoderValue))
			this->encoder0Pos = this->_circleValues ? this->_maxEncoderValue : this->_minEncoderValue;		

	}
	portEXIT_CRITICAL_ISR(&(this->mux));
}

void IRAM_ATTR AiEsp32RotaryEncoder::readButton_ISR()
{
	portENTER_CRITICAL_ISR(&(this->buttonMux));

	uint8_t butt_state = !digitalRead(this->encoderButtonPin);

	if (!this->isEnabled)
	{
		buttonState = BUT_DISABLED;
	}
	else if (butt_state && !this->previous_butt_state)
	{
		this->previous_butt_state = true;
		//Serial.println("Button Pushed");
		buttonState = BUT_PUSHED;		
	}
	else if (!butt_state && this->previous_butt_state)
	{
		this->previous_butt_state = false;
		//Serial.println("Button Released");
		buttonState = BUT_RELEASED;		
	}
	else
	{
		buttonState = (butt_state ? BUT_DOWN : BUT_UP);
	}

	portEXIT_CRITICAL_ISR(&(this->buttonMux));
}

AiEsp32RotaryEncoder::AiEsp32RotaryEncoder(uint8_t encoder_APin, uint8_t encoder_BPin, uint8_t encoder_ButtonPin, uint8_t encoder_VccPin, uint8_t encoderSteps)
{
	this->old_AB = 0;
	
	this->encoderAPin = encoder_APin;
	this->encoderBPin = encoder_BPin;
	this->encoderButtonPin = encoder_ButtonPin;
	this->encoderVccPin = encoder_VccPin;
	this->encoderSteps = encoderSteps;
	
	pinMode(this->encoderAPin, INPUT);
	pinMode(this->encoderBPin, INPUT);
}

void AiEsp32RotaryEncoder::setBoundaries(int16_t minEncoderValue, int16_t maxEncoderValue, bool circleValues)
{
	this->_minEncoderValue = minEncoderValue * this->encoderSteps;
	this->_maxEncoderValue = maxEncoderValue * this->encoderSteps;
	
	this->_circleValues = circleValues;
}

int16_t AiEsp32RotaryEncoder::readEncoder()
{
	return (this->encoder0Pos / this->encoderSteps);
}

int16_t AiEsp32RotaryEncoder::encoderChanged() {
	int16_t _encoder0Pos = readEncoder();	
	int16_t encoder0Diff = _encoder0Pos - this->lastReadEncoder0Pos;

	this->lastReadEncoder0Pos = _encoder0Pos;
	
	return encoder0Diff;
}

void AiEsp32RotaryEncoder::setup(void (*ISR_callback)(void), void (*ISR_button)(void))
{
	attachInterrupt(digitalPinToInterrupt(this->encoderAPin), ISR_callback, CHANGE);
	attachInterrupt(digitalPinToInterrupt(this->encoderBPin), ISR_callback, CHANGE);
	attachInterrupt(digitalPinToInterrupt(this->encoderButtonPin), ISR_button, CHANGE);
}

void AiEsp32RotaryEncoder::begin()
{
	this->lastReadEncoder0Pos = 0;	
	if (this->encoderVccPin >= 0) {
		pinMode(this->encoderVccPin, OUTPUT);	
		digitalWrite(this->encoderVccPin, 1);//Vcc for encoder 
	}
	
	// Initialize rotary encoder reading and decoding
	this->previous_butt_state = 0;
	if (this->encoderButtonPin >= 0) {
		pinMode(this->encoderButtonPin, INPUT_PULLUP);
	}
}

ButtonState AiEsp32RotaryEncoder::currentButtonState()
{
	return buttonState;
}

void AiEsp32RotaryEncoder::reset(int16_t newValue_) {
	newValue_ = newValue_ * this->encoderSteps;
	this->encoder0Pos = newValue_;
	if (this->encoder0Pos > this->_maxEncoderValue) this->encoder0Pos = this->_circleValues ? this->_minEncoderValue : this->_maxEncoderValue;
	if (this->encoder0Pos < this->_minEncoderValue) this->encoder0Pos = this->_circleValues ? this->_maxEncoderValue : this->_minEncoderValue;	
}

void AiEsp32RotaryEncoder::enable() {
	this->isEnabled = true;
}
void AiEsp32RotaryEncoder::disable() {
	this->isEnabled = false;
}
