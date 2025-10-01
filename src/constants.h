
#pragma once

enum class States {
    allowed=0, 
    forbidden=1, 
    flashing=2,    
};


enum class SignalState {
    notDefined=0,
    debounce=1,
    stable=2,
};
