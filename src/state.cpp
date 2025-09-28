#include <Arduino.h>
#include "constants.h"

#pragma once

class Direction {
    public:
        int pinNum, pinMode, relayNum;
        CurrentState curState {CurrentState::forbidden};
        uint32_t timeStampChangeStateFlashing {0};
        uint32_t timeStampChangeState {0};
        byte prevPinState {0};
        bool requestChangeState {false};
        bool rest {true};
        bool debounce {true};

    public:
        Direction (int pinNum, int pinMode, int relayNum) : pinNum {pinNum}, pinMode {pinMode}, relayNum {relayNum}
            { }
            
        void setState(CurrentState state)
        {
            curState = state;
        }

        bool pinIsActive()
        {
            if (pinMode == INPUT_PULLUP) return !digitalRead(pinNum);
            return digitalRead(pinNum);
        }

};


class TrafficLightT5 {
    public:
        Direction* left;
        Direction* straight;
        Direction* right;
        CurrentState curState {CurrentState::forbidden};
        uint32_t timeStampChangeState {0};

        TrafficLightT5(Direction* left, Direction* straight, Direction* right): left{left}, straight {straight}, right {right}, curState {CurrentState::forbidden }
        { }

        void setState(CurrentState state)
        {
            curState = state;
        }
};