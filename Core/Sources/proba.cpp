#include "stm32f072xb.h"

extern "C" void fCPP();

class C {
    char *name;
    unsigned id;
public:
    char *nameGet() { return name; }
    void nameSet(char *s) { name = s; fCPP(); }
};

class Unit {
    char *name;
    uintptr_t base;

};

class UnitGPIO {

};




void fCPP() {
        C c;
        //c.nameSet("abs\n\0");
    }

