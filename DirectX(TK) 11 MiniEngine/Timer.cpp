#include "Timer.h"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
    stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed()
{
    if ( isRunning )
        return std::chrono::duration<double, std::milli>( std::chrono::high_resolution_clock::now() - start).count();
    else
        return std::chrono::duration<double, std::milli>( stop - start ).count();
}

bool Timer::IsTimeRunning()
{
    return isRunning;
}

void Timer::Start()
{
    if ( !isRunning )
    {
        isRunning = true;
        start = std::chrono::high_resolution_clock::now();
    }
}

void Timer::Restart()
{
    isRunning = true;
    start = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
    if ( isRunning )
    {
        isRunning = false;
        stop = std::chrono::high_resolution_clock::now();
    }
}
