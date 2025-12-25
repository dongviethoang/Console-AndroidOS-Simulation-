#ifndef SLEEPDELAY_H
#define SLEEPDELAY_H

/* sleepdelay.h - Turbo C++ friendly delay macros/wrappers */

#include <dos.h>    /* provides delay(ms) */
#include <stdlib.h>

#ifdef __TURBOC__
/* nothing extra */
#else
/* If not Turbo, ensure delay exists (some compilers map to Sleep/usleep) */
#endif

/* Provide a portable alias for milliseconds sleep */
#define sleep_ms(ms) delay(ms)
#define sleep_sec(s) delay((s) * 1000)

#endif /* SLEEPDELAY_H */
