#pragma once
#include <cstdlib>

struct cc_IntMemory {
    int* memory;
    int cursor = 0;
    const size_t stackSize;
    cc_IntMemory(size_t stackSize): stackSize(stackSize){
        memory = (int*) calloc(stackSize, sizeof(int));
    }

    cc_IntMemory(): stackSize(0){}

    ~cc_IntMemory(){
        free(memory);
    }

    void setCursor(int x){
        if (x < 0) cursor = 0;
        else if (x >= stackSize) cursor = stackSize - 1;
        else cursor = x;
    }

    int getCursorValue(){
        return memory[cursor];
    }

    void backward() { setCursor(cursor - 1); }
    void forward() { setCursor(cursor + 1); }

    void setCursorValue(int v) { memory[cursor] = v; }
};
