#include "Profile.h"
#include <iostream>

#ifdef __APPLE__
#include <mach/mach_time.h>
#else
#include <time.h>
#endif


#ifndef __APPLE__

static uint64_t absolute_time() {
    struct timespec ts;
    const uint64_t GIGA = 1000000000ULL;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return uint64_t(ts.tv_sec)*GIGA + uint64_t(ts.tv_nsec);
}

#endif


static mach_timebase_info_data_t timebase;

StopWatch::StopWatch() { restart(); }

void StopWatch::restart() {
#ifdef __APPLE__

    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }
    start = mach_absolute_time();
#else
    start = absolute_time();
#endif
}

uint64_t StopWatch::elapsedNano() const {
#ifdef __APPLE__
    uint64_t now = mach_absolute_time();
    return (now - start) * timebase.numer / timebase.denom;
#else
    return absolute_time() - start;
#endif
}

double StopWatch::elapsedSec() const {
    return elapsedNano() * 1e-9;
}

Profiler::Profiler(const char* a, size_t n):
    action(a), count(n), cur_trial(0), total_nano(0) {}

bool Profiler::running() const { return cur_trial < count; }

void Profiler::next() {

    uint64_t trial_nano = watch.elapsedNano();
    ++cur_trial;
        
    //std::cout << "  trial " << cur_trial << " took " << trial_nano*1e-9 << ".s\n";
    
    total_nano += trial_nano;
    
    if (running()) {
        watch.restart();
    } else {
        std::cout << action << " in " << total_nano*(1e-9)/count << "s.\n";
    }
    
}
