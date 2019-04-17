#pragma once
#include "KeyboardEvent.h"

#include <queue>

class KeyboardClass
{
public:
    KeyboardClass();
    bool KeyIsPressed(const unsigned char keycode) const;
    bool KeyBufferIsEmpty() const;
    bool CharBufferIsEmpty() const;

    KeyboardEvent ReadKey();
    unsigned char ReadChar();

    void OnKeyPressed(const unsigned char key);
    void OnKeyReleased(const unsigned char key);
    void OnChar(const unsigned char key);

    void EnableAutoRepeatKeys();
    void DisableAutoRepeatKeys();
    void EnableAutoRepeatChars();
    void DisableAutoRepeatChars();

    bool IsKeyAutoRepeat() const;
    bool IsCharAutoRepeat() const;

protected:
private:
    bool autoRepeatKeys;
    bool autoRepeatChars;
    bool keyStates[256];

    std::queue<KeyboardEvent> keyBuffer;
    std::queue<unsigned char> charBuffer;
};