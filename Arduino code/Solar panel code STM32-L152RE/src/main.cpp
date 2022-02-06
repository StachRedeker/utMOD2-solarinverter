#include <Arduino.h>

/*
Script to use for an STM32 NUCLEO - L152RE using the Arduino IDE to program it
*/

//Pin mapping according to how they are called in the STM32

#define pin1 PA6 //D12
#define pin2 PB6 //D10

float val = 0;
int variable = 0;

//STM32 Configuration data

TIM_TypeDef *Instance1 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pin1), PinMap_PWM);
uint32_t channel1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin1), PinMap_PWM));
HardwareTimer *MyTim1 = new HardwareTimer(Instance1);
TIM_TypeDef *Instance2 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pin2), PinMap_PWM);
uint32_t channel2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin2), PinMap_PWM));
HardwareTimer *MyTim2 = new HardwareTimer(Instance2);

void setup (){ 

    //PWM Configuration parameters

    MyTim1->setPWM(channel1, pin1, 50, 50);
    MyTim2->setPWM(channel2, pin2, 50, 50);
    Serial.begin(9600);
    if (val < 0.01){
        variable = variable +1;
    }
}

void loop(){
    val = (analogRead(PC0)*3.3/1024); // PA0 => A0
    if(variable == 1){
        MyTim2->setCaptureCompare(channel2, 50, PERCENT_COMPARE_FORMAT);
        delay(0.000055); // delay used to correctly math the phase after using the inverting IC, since it applies a delay at inverting the signal
        MyTim1->setCaptureCompare(channel1, 50, PERCENT_COMPARE_FORMAT);
    }else{
        MyTim2->setCaptureCompare(channel2, 0, PERCENT_COMPARE_FORMAT); 
        delay(0.0002); // delay used to correctly math the phase after using the inverting IC, since it applies a delay at inverting the signal
        MyTim1->setCaptureCompare(channel1, 0, PERCENT_COMPARE_FORMAT);
    }
}