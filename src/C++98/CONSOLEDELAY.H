#ifndef CONSOLEDELAY_H
#define CONSOLEDELAY_H

/* consoledelay.h - small C++-98 (Turbo) delay wrapper */

#include "sleepdelay.h"

class SLEEP {
public:
    SLEEP() { }
    void delay(int ms) {
        /* Uses delay(ms) from dos.h via sleepdelay.h */
        sleep_ms(ms);
    }
};

/* declare extern in header; define instance in build398.cpp */
extern SLEEP sleep;

#endif /* CONSOLEDELAY_H */
