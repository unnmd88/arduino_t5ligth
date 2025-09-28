
// uint8_t newAddress = 0x09;                                          //   Назначаемый модулю адрес (0x07 < адрес < 0x7F).
                                                                    //
#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <avr/interrupt.h>
#include <iarduino_I2C_Relay.h>
#include <Arduino.h>

#include "constants.h"
#include "state.cpp"

#define IN_LEFT 1
#define IN_STRAIGHT 2
#define IN_RIGHT 3
#define IN_ALLOWED 4

#define OUT_LEFT 1
#define OUT_STRAIGHT 2
#define OUT_RIGHT 3
#define OUT_ALLOWED 4

iarduino_I2C_Relay relay;  
Direction LEFT {0, INPUT_PULLUP, 1};
Direction STRAIGHT {1, INPUT_PULLUP, 2};
Direction RIGHT {2, INPUT_PULLUP, 3};

byte cur {0};

void setup(){
  delay(500);       
  Serial.begin(9600);
  relay.digitalWrite(ALL_CHANNEL,LOW);                               
  relay.begin(&Wire); // &Wire1, &Wire2 ...//  
  STRAIGHT.timeStampChangeState = millis(); 
}

void loop(){
  cur = !digitalRead(STRAIGHT.pinNum);
  if (cur != STRAIGHT.prevPinState && STRAIGHT.curState != CurrentState::flashing) 
  {
    STRAIGHT.timeStampChangeState = millis();
    STRAIGHT.requestChangeState = true;
    STRAIGHT.rest = false;
    STRAIGHT.prevPinState = cur;
  }

  if (STRAIGHT.requestChangeState && millis() - STRAIGHT.timeStampChangeState > 50 && STRAIGHT.curState != CurrentState::flashing)
    if (!cur)
    {
      STRAIGHT.curState = CurrentState::flashing;
      STRAIGHT.timeStampChangeStateFlashing = millis();
    }
    else 
    {
      relay.digitalWrite(STRAIGHT.relayNum, cur);
      STRAIGHT.requestChangeState = false;
      STRAIGHT.timeStampChangeState = 0;
      STRAIGHT.rest = true;
      STRAIGHT.curState = CurrentState::allowed;
    }
  
  if (STRAIGHT.curState == CurrentState::flashing)
  {

    if ((millis() - STRAIGHT.timeStampChangeStateFlashing) <= 1) relay.digitalWrite(STRAIGHT.relayNum, 0);
    else if ((millis() - STRAIGHT.timeStampChangeStateFlashing) == 500) relay.digitalWrite(STRAIGHT.relayNum, 1);
    else if ((millis() - STRAIGHT.timeStampChangeStateFlashing) == 1000) relay.digitalWrite(STRAIGHT.relayNum, 0);
    else if ((millis() - STRAIGHT.timeStampChangeStateFlashing) == 1500) relay.digitalWrite(STRAIGHT.relayNum, 1);
    else if ((millis() - STRAIGHT.timeStampChangeStateFlashing) == 2000) relay.digitalWrite(STRAIGHT.relayNum, 0);
    else if ((millis() - STRAIGHT.timeStampChangeStateFlashing) == 2500) relay.digitalWrite(STRAIGHT.relayNum, 1);
    else if ((millis() - STRAIGHT.timeStampChangeStateFlashing) == 3000) 
    {
      relay.digitalWrite(STRAIGHT.relayNum, 0);
      STRAIGHT.requestChangeState = false;
      STRAIGHT.rest = true;
      STRAIGHT.prevPinState = 0;
      STRAIGHT.curState = CurrentState::forbidden;
    }
  }

  return;




  





  // if (STRAIGHT.pinIsActive()) STRAIGHT.setState(CurrentState::allowed);
  // if (STRAIGHT.curState == CurrentState::allowed && !STRAIGHT.pinIsActive() && STRAIGHT.timeStampChangeState == 0)
  // {
  //   STRAIGHT.setState(CurrentState::flashing);
  //   STRAIGHT.timeStampChangeState = millis();
  // }
  // if ((STRAIGHT.curState == CurrentState::flashing) && (millis() - STRAIGHT.timeStampChangeState) >= 3010) 
  // {
  //   STRAIGHT.setState(CurrentState::forbidden);
  //   STRAIGHT.timeStampChangeState = 0;
  // }
    

  // // Секция установки выходов на реле
  // if (STRAIGHT.curState == CurrentState::flashing) 
  // {
  //   if ((millis() - STRAIGHT.timeStampChangeState) <= 1) relay.digitalWrite(STRAIGHT.relayNum, 0);
  //   else if ((millis() - STRAIGHT.timeStampChangeState) == 500) relay.digitalWrite(STRAIGHT.relayNum, 1);
  //   else if ((millis() - STRAIGHT.timeStampChangeState) == 1000) relay.digitalWrite(STRAIGHT.relayNum, 0);
  //   else if ((millis() - STRAIGHT.timeStampChangeState) == 1500) relay.digitalWrite(STRAIGHT.relayNum, 1);
  //   else if ((millis() - STRAIGHT.timeStampChangeState) == 2000) relay.digitalWrite(STRAIGHT.relayNum, 0);
  //   else if ((millis() - STRAIGHT.timeStampChangeState) == 2500) relay.digitalWrite(STRAIGHT.relayNum, 1);
  // }
  
  // else if (STRAIGHT.curState == CurrentState::allowed) relay.digitalWrite(STRAIGHT.relayNum, 1);


  // if (LEFT.curState == CurrentState::forbidden && STRAIGHT.curState == CurrentState::forbidden && RIGHT.curState == CurrentState::forbidden) 
  // {
  //   relay.digitalWrite(LEFT.relayNum, 1); 
  //   relay.digitalWrite(STRAIGHT.relayNum, 1); 
  //   relay.digitalWrite(RIGHT.relayNum, 1); 
  // }

    

  // relay.digitalWrite(STRAIGHT.relayNum, 1);
  

  // for (unsigned long i=0, state=0; i<=3000; i+=500, state = !state){
  //   if ((millis() - STRAIGHT.timeStampChangeState) == i) relay.digitalWrite(STRAIGHT.relayNum, state);
  // }

//  if ((millis() - STRAIGHT.timeStampChangeState) == 500) relay.digitalWrite(STRAIGHT.relayNum, 1);
//  if ((millis() - STRAIGHT.timeStampChangeState) == 1000) relay.digitalWrite(STRAIGHT.relayNum, 0);
//  if ((millis() - STRAIGHT.timeStampChangeState) == 1500) relay.digitalWrite(STRAIGHT.relayNum, 1);
//  if ((millis() - STRAIGHT.timeStampChangeState) == 2000) relay.digitalWrite(STRAIGHT.relayNum, 0);
//  if ((millis() - STRAIGHT.timeStampChangeState) == 2500) relay.digitalWrite(STRAIGHT.relayNum, 1);
//  if ((millis() - STRAIGHT.timeStampChangeState) == 3000) relay.digitalWrite(STRAIGHT.relayNum, 0);
//  if ((millis() - STRAIGHT.timeStampChangeState) == 3500) relay.digitalWrite(STRAIGHT.relayNum, 1);






}



// Установить режим для пинов МК   
void setPinmode(byte startPin, byte lastPin, byte mode) {
    for (byte pin = startPin; pin <= lastPin; pin++){
    pinMode(pin, mode);
  }
}