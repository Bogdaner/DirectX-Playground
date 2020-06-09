#pragma once

struct MousePoint
{
    int x;
    int y;
};

class MouseEvent
{
public:
    enum class EventType
    {
        LPress,
        LRelease,
        RPress,
        RRelease,
        MPress,
        MRelease,
        WheelUp,
        WheelDown,
        Move, 
        RAW_MOVE,
        Invalid
    };
    MouseEvent();
    MouseEvent(const EventType type, const int x, const int y);
    bool IsValid() const;
    EventType GetType() const;
    MousePoint GetPoint() const;
    int GetPosX() const;
    int GetPosY() const;
protected:
private:
    EventType type;
    int x;
    int y;
};