#ifndef SLEEPDELAY_H
#define SLEEPDELAY_H

// Cross-platform sleep/delay functions

#ifdef _WIN32
    #include <windows.h>
    // Windows Sleep uses milliseconds
    #define delay(ms) Sleep(ms)
    #define sleep_ms(ms) Sleep(ms)
    #define sleep_sec(s) Sleep(s * 1000)
    
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    // POSIX sleep functions
    #define delay(ms) usleep(ms * 1000)  // Convert ms to microseconds
    #define sleep_ms(ms) usleep(ms * 1000)
    #define sleep_sec(s) sleep(s)
    
#else
    // Fallback using standard C
    #include <time.h>
    #include <stdio.h>
    
    inline void delay(int ms) {
        clock_t start = clock();
        while (clock() < start + ms * CLOCKS_PER_SEC / 1000) {
            // Busy wait - not ideal but works everywhere
        }
    }
    #define sleep_ms(ms) delay(ms)
    #define sleep_sec(s) delay(s * 1000)
#endif

#endif // SLEEPDELAY_H