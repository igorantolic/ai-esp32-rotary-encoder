# Intro

Use this library to connect and use a rotary encoder with ESP32 or ESP8266.

But it is a bit more than just that. **Strongly recomended is to try a few included examples to see how it works.**

**The main fatures are:**
- select a number in a predifined range. For example select a termostat value between 18 and 29 Celsius degrees.
- you can also set **step size**. Meaning in previous example you can select temperature in steps of 0.5 degrees (18.0, 18.5 .....). Of course step can be also greater than 1 like select a motr speed 500-3000 in steps of 100 (500,600, ....,2900,3000). See examples like **FM-radio-tuner**
- when using large range like select a number of repeating operation (1....5000) setting a desired number like 4525 cen be frustrating without using **acceleration**. Acceleration is a nice option you can use in such cases, meaning that if you rotate fast it will go like (1,2,3,5,10,15,30,50,100...) so use fast movements for getting close to desired number and then start rotating slower for a precise selection.
- additionally there is a helper so see example Number-select which includes a NumberSellector
- if you want to implement a menu, see example Multi-select
- in some cases like menus, you would like to go from last item to first. Boolean cycleValues parameter set to true will do exactly that.

**Using a button**

You can also use a button. Version 1.3 and above has significant changes after I found previous implementation didnt fit my needs. All examples are now updated, so if you used it before and getting crashes or you dont like a button how it is working, check a new examples.

For compatibility older examples still works but will become obsolete soon.


# Support fot ESP8266 added 10/2021

Support added also for ESP8266

# Changes in button processing 10/2021

Old button using interrupt is now obsolete.
Please look at upadated examples how to handle click, properly initialize encoder and avoid crashing.

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

Since 2021 you can download using Arduino, which is preffered way:
 
     Sketch / include Library / Manage Libraries

     in search box type "ai rotary" (without quotes)


Alternative way (to get not yet published version)
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
#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
```
Then upload code to microcontroller.

README_old.md contains more information but some parts are obsolete.

