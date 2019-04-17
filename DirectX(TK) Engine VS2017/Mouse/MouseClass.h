#pragma once
#include "MouseEvent.h"

#include<queue>

class MouseClass
{
public:
    MouseClass();
    void OnLeftPressed(const int x, const int y);
    void OnLeftReleased(const int x, const int y);

    void OnRightPressed(const int x, const int y);
    void OnRightReleased(const int x, const int y);

    void OnMidPressed(const int x, const int y);
    void OnMidReleased(const int x, const int y);

    void OnWheelUp(const int x, const int y);
    void OnWheelDown(const int x, const int y);

    void OnMouseMove(const int x, const int y);

    void OnMouseMoveRaw(const int x, const int y);

    bool IsLeftDown() const;
    bool IsRightDown() const;
    bool IsMidDown() const;

    int GetPosX() const;
    int GetPosY() const;
    MousePoint GetPoint() const;

    bool EventBufferIsEmpty() const;
    MouseEvent ReadEvent();

protected:
private:
    std::queue<MouseEvent> eventBuffer;
    int x;
    int y;
    bool left_is_down;
    bool right_is_down;
    bool mid_is_down;
};