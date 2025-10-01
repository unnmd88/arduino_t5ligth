#include <Arduino.h>
#include "constants.h"

#pragma once

class Direction {
    public:
        int pinNum, _pinMode, relayNum;
        States curState {States::forbidden};
        States requestedState;
        SignalState signalState {SignalState::stable};


        uint32_t timeStampFlashing {0};
        uint32_t timeStampChangeState {0};
        uint32_t timeStampEvent {0};
        uint32_t debounceTimeStamp {0};
        byte currPinState {0};
        byte prevPinState {0};
        byte waitFor {0};
        bool requestChangeState {false};
        
        byte relayInpState {0};
        bool rest {true};
        bool debounce {true};
        bool allowedSwitchState {false};

    public:
        Direction (int pinNum, int pinMode, int relayNum) : pinNum {pinNum}, _pinMode {pinMode}, relayNum {relayNum}
            { }
                
        bool pinIsActive()
        {
            if (_pinMode == INPUT_PULLUP) return !digitalRead(pinNum);
            return digitalRead(pinNum);
        }

        int digPinRead()
        {
            return !digitalRead(this->pinNum);
        }

        void readPinAndSetData()
        {
            if ( curState == States::flashing )
                {
                    if ((millis() - timeStampFlashing) >= 3000 )
                    {
                        curState = States::forbidden;
                        timeStampFlashing = 0;
                        timeStampEvent = millis();
                    }    
                    return;
                }
            
            currPinState = digPinRead();
            // Ловим момент изменения уровня сигнала
            if ( currPinState != prevPinState && signalState != SignalState::debounce ) 
            {
                signalState = SignalState::debounce;
                debounceTimeStamp = millis();
                waitFor = currPinState;
            }
            if (signalState == SignalState::debounce && (millis() - debounceTimeStamp > 50) )
            {
                signalState = SignalState::stable;
                allowedSwitchState = true;
                debounceTimeStamp = 0;
            }
            if ( allowedSwitchState && (waitFor == digPinRead()) )
            {
                allowedSwitchState = false;
                timeStampChangeState = millis();
                // curState = (waitFor == LOW) ?  States::flashing : States::allowed;
                if (waitFor == LOW)
                {
                    curState = States::flashing;
                    timeStampFlashing = millis();
                }
                else curState = States::allowed;

            }
            prevPinState = digPinRead();
        }

};

