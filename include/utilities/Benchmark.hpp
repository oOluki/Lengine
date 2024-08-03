#pragma once
#include <chrono>
#include <iostream>

namespace Le{

// An usefull clock implementation using std::chrono for benchmark purpouses
class BenchmarkClock{
    typedef std::chrono::_V2::system_clock::time_point timePoint;
    typedef std::common_type<std::chrono::_V2::system_clock::duration, std::chrono::_V2::system_clock::duration>::type timeDuration;

    enum{
    NONE = 0,
    PAUSED = 1 << 0,
    LOCKED = 1 << 1,
    } typedef ClockState;

    public:
    std::string name;

    BenchmarkClock() : Clock_state(NONE), StartTimePoint(std::chrono::high_resolution_clock::now()), Ticks(0)
    {
        EndTimePoint = StartTimePoint;
        Duration = EndTimePoint - StartTimePoint;
    };

    BenchmarkClock(const char* _name)
    : Clock_state(NONE), StartTimePoint(std::chrono::high_resolution_clock::now()), Ticks(0), name(_name)
    {
        EndTimePoint = StartTimePoint;
        Duration = EndTimePoint - StartTimePoint;
    }

    ~BenchmarkClock(){
        if(!(Clock_state & PAUSED))
            Update();
        double duration = get_clock_active_time().count();
        if(name.data()){
            std::cout << "Le::Clock, " << name << ", Total Active Time Is " <<
            duration << " ns, with " << Ticks << " ticks, averaging to "
            << (double)(duration / Ticks) << " ns / tick" << std::endl;
            return;
        }
        std::cout << "Le::Clock, & " << this << ", Total Active Time Is " <<
        duration << " ns, with " << Ticks << " ticks, averaging to "
        << (double)(duration / Ticks) << " ns / tick" << std::endl;
    }


    // returns a time point in nanoseconds
    static timePoint get_time_ns(){
        return std::chrono::high_resolution_clock::now();
    }

    void pause_clock(){
        if(Clock_state & PAUSED || Clock_state & LOCKED)
            return;
        Clock_state |= PAUSED;
        StartTimePoint = EndTimePoint;
        EndTimePoint = std::chrono::high_resolution_clock::now();
        Duration += EndTimePoint - StartTimePoint;
        
    }

    void resume_clock(){
        if(Clock_state & PAUSED && !(Clock_state & LOCKED)){
            Clock_state &= ~PAUSED;
            EndTimePoint = std::chrono::high_resolution_clock::now();
        }
    }

    void reset_clock(){
        StartTimePoint = std::chrono::high_resolution_clock::now();
        EndTimePoint = StartTimePoint;
        Duration = EndTimePoint - StartTimePoint;
        Clock_state = NONE;
        Ticks = 0;
    }

    void lock_clock_state(){
        Clock_state |= LOCKED;
    }

    void unlock_clock_state(){
        Clock_state &= ~LOCKED;
    }

    // updates the clock And increases tick By one
    void tick(){
        Ticks += 1;
        if(!(Clock_state & PAUSED)){
            StartTimePoint = EndTimePoint;
            EndTimePoint = std::chrono::high_resolution_clock::now();
            Duration += EndTimePoint - StartTimePoint;
        }
    }

    // returns the total duration, in nanoseconds, that the clock was unpaused
    // this doesn't update the clock
    timeDuration get_clock_active_time(){
        if(!(Clock_state & PAUSED)){
            return Duration + std::chrono::high_resolution_clock::now() - EndTimePoint;
        }
        return Duration;
    }

    // return the number of ticks the clock has performed
    size_t get_ticks(){
        return Ticks;
    }

    short get_clock_state(){
        return Clock_state;
    }


    private:
    size_t Ticks;
    timePoint EndTimePoint;
    timePoint StartTimePoint;
    timeDuration Duration;
    short Clock_state;

    void Update(){
        StartTimePoint = EndTimePoint;
        EndTimePoint = std::chrono::high_resolution_clock::now();
        Duration += EndTimePoint - StartTimePoint;
    }
    

};

}
