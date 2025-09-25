#include <Arduino.h>
#include "constants.h"

#pragma once

class Direction {
    public:
        int pinNum, pinMode, relayNum;
        CurrentState curState {CurrentState::forbidden};
        uint32_t timeStampChangeState {0};

    public:
        Direction (int pinNum, int pinMode, int relayNum) : pinNum {pinNum}, pinMode {pinMode}, relayNum {relayNum} 
            { }
            
        void setState(CurrentState state)
        {
            curState = state;
        }

        bool pinIsActive()
        {
            if (pinMode = INPUT_PULLUP) return !digitalRead(pinNum);
            return digitalRead(pinNum);
        }


};