#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "culcan_memory.hpp"
#include "culcan_tmp_mem.hpp"
#include "culcan_control.hpp"

#define DO_LOG 0
#define LOG(x) if (DO_LOG) std::cout << "LOG:: " << x << "\n";

const int ERROR_VALUE = -1024;


struct cc_CulcanVM {
    cc_IntMemory stack;
    cc_CompareBuffer comparisonBuffer;
    std::string code;
    std::vector<cc_ControlData> controlStack;

    cc_CulcanVM(std::string code): stack(128), code(code), comparisonBuffer(){}


    void execute(){
        int i = 0;
        std::optional<cc_ControlData> control = std::nullopt;

        for (char c : code){
            if (!controlStack.empty()) control = getTopControl();

            if (control.has_value() && !control.value().active && c != control.value().ender){
                i++;
                LOG("Skipped " << i);
                continue;
            }

            if (control.has_value() && c == control.value().ender){
                LOG("Exited control at " << i)
                popTopControl();
            }

            control = std::nullopt;

            switch (c){
                case '>':
                    forwardCursor(i, c);
                    break;
                case '<':
                    backwardCursor(i, c);
                    break;
                case '+':
                    incrementCursor(i, c);
                    break;
                case '-':
                    decrementCursor(i, c);
                    break;
                case '@':
                    handleComparisonPush(i, c);
                    break;
                case '=':
                    handleComparison(i, c);
                    break;
                case '?':
                    handleInput(i, c);
                    break;
                case '#':
                    handleOut(i, c);
                    break;
                case '(':
                    handleIfZero(i, c);
                    break;
                case '[':
                    handleIfOne(i, c);
                    break;
                // case 'X':
                //     setValue(9'223'372'036'854'775'807);
                //     break;
            }
        }
        i++;
    }

    void forwardCursor(int index, char current){
        moveCursorTo(getValue() + 1);
    }
    
    void backwardCursor(int index, char current){
        moveCursorTo(getValue() - 1);
    }
    
    void incrementCursor(int index, char current){
        setValue(getValue() + 1);
    }
    
    void decrementCursor(int index, char current){
        setValue(getValue() - 1);
    }
    
    void handleOut(int index, char current){
        std::cout << getValue() << " ";
    }
    
    void handleComparisonPush(int index, char current){
        int v = getValue();
        pushComparisonValue(v);
    }

    void handleComparison(int index, char current){
        if (!comparisonBuffer.hasValue()){
            setValue(ERROR_VALUE);
            return;
        }

        bool equals = compareEquality();
        setValue(equals ? 1 : 0);
    }

    void handleInput(int index, char current){
        int o = getInput();
        setValue(o);
    }

    void handleIfCondition(int index, char current, bool active){
        LOG("Pushed new control " << current << " at " << index)
        LOG("Control skip " << active)
        pushControl(index, current, active);
    }

    void handleIfZero(int index, char current){
        handleIfCondition(index, current, getValue() == 0);
    }

    void handleIfOne(int index, char current){
        handleIfCondition(index, current, getValue() == 1);
    }

    // API FUNCS

    bool compareEquality(){
        return (comparisonBuffer.a.has_value() && comparisonBuffer.b.has_value()) && comparisonBuffer.a.value() == comparisonBuffer.b.value();
    }

    bool compareGreatness(){
        return (comparisonBuffer.a.has_value() && comparisonBuffer.b.has_value()) && comparisonBuffer.a.value() > comparisonBuffer.b.value();
    }

    int getInput(){
        int o;
        std::cout << ":IN:> ";
        std::cin >> o;

        return o;
    }

    bool pushComparisonValue(int v){
        if (!comparisonBuffer.a.has_value()){
            comparisonBuffer.a = v;
            return true;
        } else if (!comparisonBuffer.b.has_value()){
            comparisonBuffer.b = v;
            return true;
        }

        return false;
    }

    void printCell(int c){
        size_t size = stack.stackSize;
        if (c < 0 || c >= size) return;

        int value = stack.memory[c];

        std::cout << value << " ";
    }

    int getCursor() { return stack.cursor; }
    void moveCursorTo(int c) { stack.setCursor(c); }

    int getValue() { return stack.getCursorValue(); }
    void setValue(int v) { stack.setCursorValue(v); }

    cc_ControlData& getTopControl() { return controlStack.back(); }
    void popTopControl() { return controlStack.pop_back(); }

    bool checkControlSuffix(char suffix) { return !controlStack.empty() && getTopControl().ender == suffix; }
    bool doSkipControl() { return !controlStack.empty() && getTopControl().active; }

    void pushControl(int start, char starter, bool skip) { controlStack.push_back(cc_ControlData(start, starter, skip)); }
};


