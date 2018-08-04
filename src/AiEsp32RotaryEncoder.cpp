// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code
// 
// 

#include "AiEsp32RotaryEncoder.h"

void IRAM_ATTR AiEsp32RotaryEncoder::readEncoder_ISR()
{	
	//Serial.println("interrupt!");
	portENTER_CRITICAL_ISR(&(this->mux));
	if (this->isEnabled) {

	/*
		digitalWrite(2, HIGH);
		digitalWrite(2, HIGH);
		digitalWrite(2, HIGH);
		digitalWrite(2, LOW);
	*/
	
	/*
		static uint8_t old_AB = 0;
		// grey code
		// http://hades.mech.northwestern.edu/index.php/Rotary_Encoder
		// also read up on 'Understanding Quadrature Encoded Signals'
		// https://www.pjrc.com/teensy/td_libs_Encoder.html
		// another interesting lib: https://github.com/0xPIT/encoder/blob/arduino/ClickEncoder.cpp
		// explanation of this code is at: https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
		static int8_t enc_states[] = { 0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0 };

		old_AB <<= 2;
		old_AB |= ((digitalRead(encoderBPin)) ? (1 << 1) : 0) | ((digitalRead(encoderAPin)) ? (1 << 0) : 0);

		encoder0Pos += (enc_states[(old_AB & 0x0f)]);
		
		Serial.println(encoder0Pos);

		if (encoder0Pos > _maxEncoderValue) encoder0Pos = _circleValues ? _minEncoderValue : _maxEncoderValue;
		if (encoder0Pos < _minEncoderValue) encoder0Pos = _circleValues ? _maxEncoderValue : _minEncoderValue;
	*/
		// code from https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
		/**/
		this->old_AB <<= 2;                   //remember previous state

		//Serial.print("OldAB= ");
		//Serial.println(old_AB, BIN);

		int8_t ENC_PORT = ((digitalRead(this->encoderBPin)) ? (1 << 1) : 0) | ((digitalRead(this->encoderAPin)) ? (1 << 0) : 0);
		
		//Serial.print("ENC_PORT= ");
		//Serial.println(ENC_PORT, BIN);
		
		this->old_AB |= ( ENC_PORT & 0x03 );  //add current state

		//Serial.print("NewAB= ");
		//Serial.println(old_AB, BIN);		

		//Serial.print("old_AB & 0x0f= ");
		//Serial.println(( old_AB & 0x0f ), BIN);

		this->encoder0Pos += ( this->enc_states[( this->old_AB & 0x0f )]);	

		if (this->encoder0Pos > this->_maxEncoderValue)
			this->encoder0Pos = this->_circleValues ? this->_minEncoderValue : this->_maxEncoderValue;
		if (this->encoder0Pos < this->_minEncoderValue)
			this->encoder0Pos = this->_circleValues ? this->_maxEncoderValue : this->_minEncoderValue;		

		//Serial.print("encoder0Pos= ");
		//Serial.println(this->encoder0Pos);	
		//Serial.println("---------------");
	}
	portEXIT_CRITICAL_ISR(&mux);
}


AiEsp32RotaryEncoder::AiEsp32RotaryEncoder(uint8_t encoder_APin, uint8_t encoder_BPin, uint8_t encoder_ButtonPin, uint8_t encoder_VccPin)
{
	this->old_AB = 0;
	
	this->encoderAPin = encoder_APin;
	this->encoderBPin = encoder_BPin;
	this->encoderButtonPin = encoder_ButtonPin;
	this->encoderVccPin = encoder_VccPin;
	pinMode(this->encoderAPin, INPUT);
	pinMode(this->encoderBPin, INPUT);
}

void AiEsp32RotaryEncoder::setBoundaries(int16_t minEncoderValue, int16_t maxEncoderValue, bool circleValues)
{
	this->_minEncoderValue = minEncoderValue * 2;
	this->_maxEncoderValue = maxEncoderValue * 2;
	this->_circleValues = circleValues;
}



int16_t AiEsp32RotaryEncoder::readEncoder()
{
	return (this->encoder0Pos / 2);
}

int16_t AiEsp32RotaryEncoder::encoderChanged() {
	int16_t _encoder0Pos = readEncoder();
	
	int16_t encoder0Diff = _encoder0Pos - this->lastReadEncoder0Pos;

	this->lastReadEncoder0Pos = _encoder0Pos;
	return encoder0Diff;
}

void AiEsp32RotaryEncoder::setup(void (*ISR_callback)(void))
{
	attachInterrupt(digitalPinToInterrupt(this->encoderAPin), ISR_callback, CHANGE);
	attachInterrupt(digitalPinToInterrupt(this->encoderBPin), ISR_callback, CHANGE);
}


void AiEsp32RotaryEncoder::begin()
{
	this->lastReadEncoder0Pos = 0;
	//Serial.begin(115200);
	if (this->encoderVccPin >= 0) {
		pinMode(this->encoderVccPin, OUTPUT);	digitalWrite(this->encoderVccPin, 1);//Vcc for encoder 
	}
	//Serial.println("Enable rotary encoder ISR:");
	// Initialize rotary encoder reading and decoding
	this->previous_butt_state = 0;
	if (this->encoderButtonPin >= 0) {
		pinMode(this->encoderButtonPin, INPUT_PULLUP);
	}
}

ButtonState AiEsp32RotaryEncoder::currentButtonState()
{
	if (!this->isEnabled) {
		return BUT_DISABLED;
	}

	uint8_t butt_state = !digitalRead(this->encoderButtonPin);


	if (butt_state && !this->previous_butt_state)
	{
		this->previous_butt_state = true;
		//Serial.println("Button Pushed");
		return BUT_PUSHED;
	}
	if (!butt_state && this->previous_butt_state)
	{
		this->previous_butt_state = false;
		//Serial.println("Button Released");
		return BUT_RELEASED; 
	}
	return (butt_state ? BUT_DOWN : BUT_UP);
}

void AiEsp32RotaryEncoder::reset(int16_t newValue_) {
	newValue_ = newValue_ * 2;
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
