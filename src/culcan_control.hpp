#pragma once
#include <map>

static std::map<char, char> CONTROL_SYMBOLS = {
    {'(', ')'},
    {'[', ']'}
};


// CONTROLS

struct cc_ControlData {
    int startingPos;
    char starter;
    char ender;
    bool active;

    cc_ControlData(int starting, char starter, bool skip): startingPos(starting), starter(starter), active(skip), ender(CONTROL_SYMBOLS[starter]){
    }

    cc_ControlData(): startingPos(0), starter('.'), ender('.'), active(false){}
};

struct IfZeroControlData : cc_ControlData {
    IfZeroControlData(int starting, char starter, bool skip): cc_ControlData(starting, starter, skip){}
};


