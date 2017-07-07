#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <stdint.h>
#include <stdlib.h>

class StopWatch {
public:


    uint64_t start;

    StopWatch();

    void restart();
    
    uint64_t elapsedNano() const;
    double elapsedSec() const;

};

class Profiler {
public:

    StopWatch watch;
    const char* action;
    const size_t count;
    size_t cur_trial;
    
    uint64_t total_nano;

    Profiler(const char* a, size_t n=10);
    bool running() const;
    void next();
    
};

#define PROFILE(action) for (Profiler p(action); p.running(); p.next())

#endif
