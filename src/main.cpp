
// uint8_t newAddress = 0x09;                                          //   Назначаемый модулю адрес (0x07 < адрес < 0x7F).
                                                                    //
#include <Wire.h>                                                   //   Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_I2C_Relay.
#include <iarduino_I2C_Relay.h>
#include <Arduino.h>

#include "constants.h"


#define IN_LEFT 1
#define IN_STRAIGHT 2
#define IN_RIGHT 3
#define IN_ALLOWED 4

#define OUT_LEFT 1
#define OUT_STRAIGHT 2
#define OUT_RIGHT 3
#define OUT_ALLOWED 4
                                     //   Подключаем библиотеку для работы с модулем
iarduino_I2C_Relay relay;   

byte pState = 0;
byte cState = 0;
unsigned long timeStamp;
byte reqTransitionCount = 0;

                   
void setup(){                                                       
    delay(500);       
    Serial.begin(9600);                                              
//  Готовим модуль к работе:        
// Инициируем работу с модулем, указав ссылку на объект для работы с шиной I2C на которой находится модуль (по умолчанию &Wire).
    relay.digitalWrite(ALL_CHANNEL,LOW);                               
    relay.begin(&Wire); // &Wire1, &Wire2 ...//   
    pinMode(IN_RIGHT, INPUT_PULLUP);
}                                                           //
                                                                    //
void loop(){                                                        //
// Маппим вход цифровые входы на входы реле:
// 1й вх МК -> К1 реле, 2й вх МК -> К2 реле и т.д.   


  cState = !digitalRead(3);
  if (!cState) 
  {
    if (timeStamp == 0) 
    {
      timeStamp = millis();
    }
    
    if (millis() - timeStamp == 1000)
      relay.digitalWrite(2, 0);
    else if (millis() - timeStamp == 2000)
      relay.digitalWrite(2, 1);
    else if (millis() - timeStamp == 3000)
      relay.digitalWrite(2, 0);
    else if (millis() - timeStamp >= 4000)
    {
      relay.digitalWrite(2, 1);
      timeStamp = 0;
    }
    else 
    {
        
      relay.digitalWrite(1, 1);
      relay.digitalWrite(2, 1);
      relay.digitalWrite(3, 1);
      relay.digitalWrite(4, 0);
  
    }
      
      
  } 

  else if (cState && (timeStamp == 0)) 
  {
      relay.digitalWrite(1, 0);
      relay.digitalWrite(2, 1);
      relay.digitalWrite(3, 0);
      relay.digitalWrite(4, 1);
  }


}

// Установить режим для пинов МК   
void setPinmode(byte startPin, byte lastPin, byte mode) {
    for (byte pin = startPin; pin <= lastPin; pin++){
    pinMode(pin, mode);
  }
}