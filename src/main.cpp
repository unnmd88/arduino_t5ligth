
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
Direction LEFT {2, INPUT_PULLUP, 1};
Direction STRAIGHT {3, INPUT_PULLUP, 2};
Direction RIGHT {4, INPUT_PULLUP, 3};

void flashingRelay(Direction lamp)
{
  if ((millis() - lamp.timeStampFlashing) <= 500) relay.digitalWrite(lamp.relayNum, 0);
  else if ((millis() - lamp.timeStampFlashing) >= 500 && (millis() - lamp.timeStampFlashing) <= 1000) relay.digitalWrite(lamp.relayNum, 1);
  else if ((millis() - lamp.timeStampFlashing) >= 1000 && (millis() - lamp.timeStampFlashing) <= 1500) relay.digitalWrite(lamp.relayNum, 0);
  else if ((millis() - lamp.timeStampFlashing) >= 1500 && (millis() - lamp.timeStampFlashing) <= 2000) relay.digitalWrite(lamp.relayNum, 1);
  else if ((millis() - lamp.timeStampFlashing) >= 2000 && (millis() - lamp.timeStampFlashing) <= 2500) relay.digitalWrite(lamp.relayNum, 0);
  else if ((millis() - lamp.timeStampFlashing) >= 2500 && (millis() - lamp.timeStampFlashing) <= 3000) relay.digitalWrite(lamp.relayNum, 1);
}


void setup(){
  delay(500);       
  Serial.begin(9600);
  relay.digitalWrite(ALL_CHANNEL,LOW);                               
  relay.begin(&Wire); // &Wire1, &Wire2 ...//  
  pinMode(LEFT.pinNum, INPUT_PULLUP);
  pinMode(STRAIGHT.pinNum, INPUT_PULLUP);
  pinMode(RIGHT.pinNum, INPUT_PULLUP);
}

void loop(){
  LEFT.readPinAndSetData();
  STRAIGHT.readPinAndSetData();
  RIGHT.readPinAndSetData();

  // if ( (STRAIGHT.curState == States::forbidden) && (LEFT.curState == States::forbidden) && (RIGHT.curState == States::forbidden) )    
  Serial.print("STRAIGHT.pinState=");
  Serial.print(STRAIGHT.digPinRead());
  Serial.println();
  Serial.print("STRAIGHT.curState=");
  Serial.print(static_cast<int>(STRAIGHT.curState));
  Serial.println();
  Serial.print("-------------");
  Serial.println();

  // Serial.println();
  // Serial.print(STRAIGHT.digPinRead());
  // Serial.println();

  if ( (STRAIGHT.curState == States::forbidden) && (LEFT.curState == States::forbidden) && (RIGHT.curState == States::forbidden) )    
    {
      relay.digitalWrite(LEFT.relayNum, HIGH);
      relay.digitalWrite(STRAIGHT.relayNum, HIGH);
      relay.digitalWrite(RIGHT.relayNum, HIGH);
      relay.digitalWrite(4, LOW);
      return;
    }
  relay.digitalWrite(4, HIGH);
    
  if (LEFT.curState == States::forbidden) relay.digitalWrite(LEFT.relayNum, LOW);
  if (STRAIGHT.curState == States::forbidden) relay.digitalWrite(STRAIGHT.relayNum, LOW);
  if (RIGHT.curState == States::forbidden) relay.digitalWrite(RIGHT.relayNum, LOW);

  if (STRAIGHT.curState == States::flashing) flashingRelay(STRAIGHT);
  if (LEFT.curState == States::flashing) flashingRelay(LEFT);
  if (RIGHT.curState == States::flashing) flashingRelay(RIGHT);

  // Serial.print(millis() - STRAIGHT.timeStampFlashing);
  // Serial.println();

  // if ((millis() - STRAIGHT.timeStampFlashing) <= 500) relay.digitalWrite(STRAIGHT.relayNum, 0);
  // else if ((millis() - STRAIGHT.timeStampFlashing) >= 500 && (millis() - STRAIGHT.timeStampFlashing) <= 1000) relay.digitalWrite(STRAIGHT.relayNum, 1);
  // else if ((millis() - STRAIGHT.timeStampFlashing) >= 1000 && (millis() - STRAIGHT.timeStampFlashing) <= 1500) relay.digitalWrite(STRAIGHT.relayNum, 0);
  // else if ((millis() - STRAIGHT.timeStampFlashing) >= 1500 && (millis() - STRAIGHT.timeStampFlashing) <= 2000) relay.digitalWrite(STRAIGHT.relayNum, 1);
  // else if ((millis() - STRAIGHT.timeStampFlashing) >= 500 && ((millis() - STRAIGHT.timeStampFlashing) <= 1000)) relay.digitalWrite(STRAIGHT.relayNum, 1);
  // else if ((millis() - STRAIGHT.timeStampFlashing) == 1000) relay.digitalWrite(STRAIGHT.relayNum, 0);
  // else if ((millis() - STRAIGHT.timeStampFlashing) == 1500) relay.digitalWrite(STRAIGHT.relayNum, 1);
  // else if ((millis() - STRAIGHT.timeStampFlashing) == 2000) relay.digitalWrite(STRAIGHT.relayNum, 0);
  // else if ((millis() - STRAIGHT.timeStampFlashing) == 2500) relay.digitalWrite(STRAIGHT.relayNum, 1);



    

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
  
}



// Установить режим для пинов МК   
void setPinmode(byte startPin, byte lastPin, byte mode) {
    for (byte pin = startPin; pin <= lastPin; pin++){
    pinMode(pin, mode);
  }
}



void workingVer ()
{
  STRAIGHT.currPinState = STRAIGHT.digPinRead();
// Ловим момент изменения уровня сигнала
if ( STRAIGHT.currPinState != STRAIGHT.prevPinState && STRAIGHT.signalState != SignalState::debounce ) 
{
  STRAIGHT.signalState = SignalState::debounce;
  STRAIGHT.debounceTimeStamp = millis();
  STRAIGHT.waitFor = STRAIGHT.currPinState;
}
if (STRAIGHT.signalState == SignalState::debounce && (millis() - STRAIGHT.debounceTimeStamp > 50) )
{
  STRAIGHT.signalState = SignalState::stable;
  STRAIGHT.allowedSwitchState = true;
  STRAIGHT.debounceTimeStamp = 0;
}
if (STRAIGHT.allowedSwitchState && STRAIGHT.waitFor == STRAIGHT.digPinRead())
{
  STRAIGHT.allowedSwitchState = false;
  STRAIGHT.timeStampChangeState = millis();
  relay.digitalWrite(STRAIGHT.relayNum, STRAIGHT.waitFor);
}
STRAIGHT.prevPinState = STRAIGHT.digPinRead();
}

void setRelay(Direction direction)
{
    switch (direction.curState)
  {
  case States::allowed:
    {
      if (!relay.digitalRead(direction.relayNum)) relay.digitalWrite(direction.relayNum, HIGH);
    }
    break;
  case States::flashing:
    {
    if (millis() - direction.timeStampFlashing <= 10) relay.digitalWrite(direction.relayNum, LOW);
    else if ((millis() - direction.timeStampFlashing) == 500) relay.digitalWrite(direction.relayNum, HIGH);
    else if ((millis() - direction.timeStampFlashing) == 1000) relay.digitalWrite(direction.relayNum, LOW);
    else if ((millis() - direction.timeStampFlashing) == 1500) relay.digitalWrite(direction.relayNum, HIGH);
    else if ((millis() - direction.timeStampFlashing) == 2000) relay.digitalWrite(direction.relayNum, LOW);
    else if ((millis() - direction.timeStampFlashing) == 2500) relay.digitalWrite(direction.relayNum, HIGH);
    }
    break;
  case States::forbidden:
    {
      if (relay.digitalRead(STRAIGHT.relayNum)) relay.digitalWrite(STRAIGHT.relayNum, LOW);
    }
    break;   
  }
}