# New feature added 02/2021 - accelerated movement

In case a range to select is large, for example - select a value between 0 and 1000 and we want 785, without accelerateion you need long time to get to that number.
However using new implemented acceleration, faster you turn, faster will the value raise.For fine tuning just slow down.
In new example imagine any random naumber between 1 and 1000 and try to select it as fast as you can using example code, and after that disable acceleration and try again.

rotaryEncoder.setAcceleration(250); 
this will set acceleration; larger number = more accelearation; 0 or 1 means disabled acceleration


rotaryEncoder.disableAcceleration(); 
This will disable acceleration (now enabled by default) in case you dont need one.

Recomendation - turn off for small ranges; but turn on when you select from more than 50 values in range.


# Arduino Esp32 Encoder Library

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

README_old.md contains more information but some parts are obsolete.

