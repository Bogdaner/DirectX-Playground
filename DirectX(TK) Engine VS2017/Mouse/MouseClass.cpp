#include "MouseClass.h"

MouseClass::MouseClass() :
    x{ 0 }, y{ 0 }, left_is_down{ false }, right_is_down{ false }, mid_is_down{ false }
{
}

void MouseClass::OnLeftPressed(const int x, const int y)
{
    left_is_down = true;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::LPress, x, y });
}

void MouseClass::OnLeftReleased(const int x, const int y)
{
    left_is_down = false;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::LRelease, x, y });
}

void MouseClass::OnRightPressed(const int x, const int y)
{
    right_is_down = true;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::RPress, x, y });
}

void MouseClass::OnRightReleased(const int x, const int y)
{
    right_is_down = false;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::RRelease, x, y });
}

void MouseClass::OnMidPressed(const int x, const int y)
{
    mid_is_down = true;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::MPress, x, y });
}

void MouseClass::OnMidReleased(const int x, const int y)
{
    mid_is_down = false;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::MRelease, x, y });
}

void MouseClass::OnWheelUp(const int x, const int y)
{
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::WheelUp, x, y });
}

void MouseClass::OnWheelDown(const int x, const int y)
{
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::WheelDown, x, y });
}

void MouseClass::OnMouseMove(const int x, const int y)
{
    this->x = x;
    this->y = y;
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::Move, x, y });
}

void MouseClass::OnMouseMoveRaw(const int x, const int y)
{
    eventBuffer.push(MouseEvent{ MouseEvent::EventType::RAW_MOVE, x, y });
}

bool MouseClass::IsLeftDown() const
{
    return left_is_down;
}

bool MouseClass::IsRightDown() const
{
    return right_is_down;
}

bool MouseClass::IsMidDown() const
{
    return mid_is_down;
}

int MouseClass::GetPosX() const
{
    return x;
}

int MouseClass::GetPosY() const
{
    return y;
}

MousePoint MouseClass::GetPoint() const
{
    return { x, y };
}

bool MouseClass::EventBufferIsEmpty() const
{
    return eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
    if (EventBufferIsEmpty())
        return MouseEvent();
    else
    {
        MouseEvent me = eventBuffer.front();
        eventBuffer.pop();
        return me;
    }
}
