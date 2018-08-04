# Arduino Esp32 / ESP8266 MULTIPLE Encoder Library

This project is based on the (https://github.com/igorantolic/ai-esp32-rotary-encoder) with only main difference that it supports
multiple instances on encoders. Original library supported only one instance, but this project allows you to create more instance of encoders like this:

```c
#include "AiEsp32RotaryEncoder.h"
#define ROTARY_ENCODER2_A_PIN GPIO_NUM_16
#define ROTARY_ENCODER2_B_PIN GPIO_NUM_4
#define ROTARY_ENCODER1_A_PIN GPIO_NUM_17
#define ROTARY_ENCODER1_B_PIN GPIO_NUM_5

AiEsp32RotaryEncoder rotaryEncoder1 = AiEsp32RotaryEncoder(ROTARY_ENCODER1_A_PIN, ROTARY_ENCODER1_B_PIN, -1, -1);
AiEsp32RotaryEncoder rotaryEncoder2 = AiEsp32RotaryEncoder(ROTARY_ENCODER2_A_PIN, ROTARY_ENCODER2_B_PIN, -1, -1);
```

in setup() of arduino:
```c
	rotaryEncoder1.begin();
	rotaryEncoder1.setup([]{rotaryEncoder1.readEncoder_ISR();});

	rotaryEncoder2.begin();
	rotaryEncoder2.setup([]{rotaryEncoder2.readEncoder_ISR();});
```

Rotary encoder main interrupt code is extracted from (https://github.com/marcmerlin/IoTuz) and some additional features are included here.

## Description

This library enables  easy implementation of rotary encoder functionality in your application for ESP32, ESP8266 (or similar microcontroller).

## Installing

The downloaded code can be included as a new library into the IDE selecting the menu:

     Sketch / include Library / Add .Zip library

Restart the Arduino IDE and follow the examples located at

     File -> Examples -> Ai Esp32 Rotary Encoder

## Connecting Rotary Encoder Module to your microcontroller ESP32, ESP8266 (or similar)

This are instructions for "Rotary Encoder Module" which is actually Rotary Encoder with 2 resistors on board.  

CLK (A pin) - connect to any microcontroler intput pin with interrupt -> in this example pin 32

DT (B pin) - connect to any microcontroler intput pin with interrupt -> in this example pin 21

SW (button pin) - connect to any microcontroler intput pin -> in this example pin 25

VCC - for this you can two options:

a) connect to microcontroler VCC (then set ROTARY_ENCODER_VCC_PIN -1) or 

b) connect to any microcontroler output pin - in this example pin 25

GND - connect to microcontroler GND

## How to use

I suggest you should start using built in example. After installing library (and reopening Arduino IDE if required) open 

File -> Examples -> Ai Esp32 Rotary Encoder

If you didnt use suggested pins, adjust defines

```c
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN 27
```

For ESP8266 you can use Dx as pin names like in example:
```c
#define ROTARY_ENCODER_A_PIN D3
#define ROTARY_ENCODER_B_PIN D2
#define ROTARY_ENCODER_BUTTON_PIN D4
```
Then upload code to microcontroller.

This example is initally set that you can adjust values 0 to 10. If you try to go beyond upper limit 10, since cycle option is set to true, it will restart from 0. Also tryinf to adjust lower than 0 will go back to 10.

If you press button first time, new limits -2 ... 2 are set. But this time cycle is set to false, so you have real minimum and maximum values.

Further pressing button will double the same limit to -4 ... 4. Then -8...8 and so on. I wonder how far will you go. Real limt is int16 limit (signed int16) which should be sufficient for most applications.

## Details

### step 1) include library using 

```c
#include "AiEsp32RotaryEncoder.h"
```

### step 2) set pins used. Important: A and B pins must support interrupts.

```c
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN 27 /*put -1 of Rotary encoder Vcc is 
                                    connected directly to 3,3V; 
                                    else you can use declared output pin 
                                    for powering rotary encoder */
```

### step 3) declare your variable like rotaryEncoder

```c
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(
    ROTARY_ENCODER_A_PIN, 
    ROTARY_ENCODER_B_PIN, 
    ROTARY_ENCODER_BUTTON_PIN, 
    ROTARY_ENCODER_VCC_PIN
    );
```

### step 4) in setup() function you should call begin method to initialize encoder.

```c
rotaryEncoder.begin();
```

### step 4a) in setup() function you should call begin method to initialize encoder.

```c
rotaryEncoder.setup([]{rotaryEncoder.readEncoder_ISR();});
```

### step 5) in loop() call some function like rotary_loop();

```c
rotary_loop();
```

### step 6) define function rotary_loop()

Example:

```c

void rotary_loop() {
	//first lets handle rotary encoder button click
	if (rotaryEncoder.currentButtonState() == BUT_RELEASED) {
		//we can process it here or call separate function like:
    	rotary_onButtonClick();
	}

	//lets see if anything changed
	int8_t encoderDelta = rotaryEncoder.encoderChanged();
	
	//optionally we can ignore whenever there is no change
	if (encoderDelta == 0) return;
	
    //for some cases we only want to know if value is 
    //increased or decreased (typically for menu items)
	if (encoderDelta>0) Serial.print("+");
	if (encoderDelta<0) Serial.print("-");

    //for other cases we want to know what is current value. 
    //Additionally often we only want if something changed
	//example: when using rotary encoder to set termostat temperature, or sound volume etc
	
	//if value is changed compared to our last read
	if (encoderDelta!=0) {
		//now we need current value
		int16_t encoderValue = rotaryEncoder.readEncoder();

		//process new value. Here is simple output.
		Serial.print("Value: ");
		Serial.println(encoderValue);

	}
}
```

### step 7) if you use separate function for processing rotary encoder button click, implmement it

In step 6 we call rotary_onButtonClick();

example: 

```c
void rotary_onButtonClick() {

	rotaryEncoder.disable();

}
```

In this example we disable encoder on first click on button. Dont expect any further effects before you call 

```c
rotaryEncoder.enable();
```

...for obvious reasons

## List of methods 

### Constructor 


Call to define local variable. 4 parameters are pin numbers.
```c
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN 27

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(
	ROTARY_ENCODER_A_PIN, 
	ROTARY_ENCODER_B_PIN, 
	ROTARY_ENCODER_BUTTON_PIN, 
	ROTARY_ENCODER_VCC_PIN
	);
//or empty constructor
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder();
/* Note: in case of empty constructor these are fefault pins:
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN 27
*/
```

### setBoundaries (optional)

sets minimum and maximum value. 
Additionally set circleValues to false if you want to stop at min or max limits.
If you want to min to continue with max value or vice versa set to true,

For example when using menu with 4 items you can call
```c
	rotaryEncoder.setBoundaries(1,4,true);
```
rotating right will select valus 1, 2, 3, 4, 1 ,2, 3, ....

while rotating left will select valus 1, 4, 3,2,1,4,3, ....

### begin - initialization method

You must call this in setup()
```c		
void setup() {
	rotaryEncoder.begin();
	//...
}
```

### reset to zero or selected value

selected value will change to selected value. If value not provided 0 is assumed.
Please note that there is a limit check. If value is lower than low limit low limit will be set. The same is for maximum limit.

```c
	//reaset to 0		
	rotaryEncoder.reset();
	//reaset to 3	
	rotaryEncoder.reset(3);
```

### disable and enable

This will disable rotary movement or button events. You must call enable to contunue getting new values or button clicks.
```c		
	rotaryEncoder.disable();
	rotaryEncoder.enable();
```


### readEncoder

This methot will fetch current value of encoder.
```c		
	int16_t currentValue = rotaryEncoder.readEncoder();
```

### encoderChanged

This methot will return delta (absolute difference) comparing to previous read.
```c		
	//you can react only on changes
	int16_t encoderDelta = rotaryEncoder.encoderChanged();
	if (encoderDelta>0) Serial.print("+");
	if (encoderDelta<0) Serial.print("-");	
```

### currentButtonState

This methor returns value of enum - current button state
```c		
	ButtonState current  = rotaryEncoder.currentButtonState();
	// or
	if (rotaryEncoder.currentButtonState() == BUT_RELEASED) {
		Serial.println("Click!");	
	}	

	/*
	Button values are:
	typedef enum {
		BUT_DOWN = 0,
		BUT_PUSHED = 1,
		BUT_UP = 2,
		BUT_RELEASED = 3,
		BUT_DISABLED = 99, //this state is after you call rotaryEncoder.disable(); 
	} ButtonState;
	*/

```


