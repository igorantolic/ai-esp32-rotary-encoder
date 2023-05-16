#include "AiEsp32RotaryEncoder.h"
#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#else
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#endif
#define ROTARY_ENCODER_STEPS 4
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

/*
Select fast food - Hot dog, Pizza, Hamburger, Cheeseburger 
Then select how many pieces 1 to 10
At the end select if you want fast delivery for extra 2$
*/
String options[] = {"Select fast food", "How many (1...10)", "Do you want fast delicery for 2$?"};

String foods[] = {"Hot dog", "Pizza", "Hamburger", "Cheeseburger"};
//String foods[] = {"   1", "   2", "   3", "   4"};
String selectedFood = "";

String delivery[] = {"Normal", "Fast for extra 2$"};
String selectedDelivery = "";

int selectedQuantity = 1;

int currentOption = 0;

void setForOption(int newOption)
{
    currentOption = newOption;

    switch (newOption)
    {
    case 0:
        rotaryEncoder.setBoundaries(0, 3, true); //food type 0..3
        rotaryEncoder.setEncoderValue(0);
        break;
    case 1:
        rotaryEncoder.setBoundaries(1, 10, false); //how many pieces 1 to 10; do not go from 10 to 1
        rotaryEncoder.setEncoderValue(1);
        break;
    case 2:
        rotaryEncoder.setBoundaries(0, 1, false); //select delivery
        rotaryEncoder.setEncoderValue(0);
        break;

    default:
        break;
    }

    Serial.println(options[currentOption]);
    showSelection();
}

void rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < 200)
        return;
    lastTimePressed = millis();

    int selecedValue = rotaryEncoder.readEncoder();

    switch (currentOption)
    {
    case 0: //"Select fast food"
        selectedFood = foods[selecedValue];
        setForOption(1);
        break;
    case 1: //"Select quantity"
        selectedQuantity = selecedValue;
        setForOption(2);
        break;
    case 2: //"Select delivery"
        selectedDelivery = delivery[selecedValue];
        showOrder();
        setForOption(0);
        break;

    default:
        break;
    }
}

void showOrder()
{
    Serial.println("++++++++++++++++++++++++ ORDER DETAILS ++++++++++++++++++++++++++++++++++++++++++");
    Serial.print("You ordered ");
    Serial.print(selectedFood);
    Serial.print(", quantity: ");
    Serial.print(selectedQuantity);
    Serial.print(". Delivery: ");
    Serial.println(selectedDelivery);
    Serial.println("---------------------------------------------------------------------------------");
    Serial.println("");

    Serial.print("NEW ORDER ");
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

void setup()
{
    Serial.begin(115200);
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setAcceleration(0);

    rotaryEncoder.correctionOffset=2; //try with zero or ROTARY_ENCODER_STEPS/2
    rotaryEncoder.isButtonPulldown = false;
	  rotaryEncoder.areEncoderPinsPulldownforEsp32 = true;

    setForOption(0);
}

void showSelection()
{
    int selecedValue = rotaryEncoder.readEncoder();

    switch (currentOption)
    {
    case 0: //"Select fast food"
        Serial.println(foods[selecedValue]);
        break;
    case 1: //"Select quantity"
        Serial.print("Quantity: ");
        Serial.println(selecedValue);
        break;
    case 2: //"Select delivery"
        Serial.println(delivery[selecedValue]);
        break;
    default:
        break;
    }
}

void loop()
{
    if (rotaryEncoder.encoderChanged())
    {
        showSelection();
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        rotary_onButtonClick();
    }
}