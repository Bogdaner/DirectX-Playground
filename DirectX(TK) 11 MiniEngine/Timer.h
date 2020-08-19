#pragma once

#include <chrono>

class Timer
{
public:
    Timer();
    double GetMilisecondsElapsed();
    bool IsTimeRunning();
    void Start();
    void Restart();
    void Stop();
protected:
private:
    bool isRunning = false;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point stop;
};