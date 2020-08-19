#include "KeyboardClass.h"

KeyboardClass::KeyboardClass() 
    : autoRepeatKeys{ false },
      autoRepeatChars{ false }
{
    for (bool& s : keyStates)
        s = false;
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode) const
{
    return keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty() const
{
    return keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty() const
{
    return charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
    if (KeyBufferIsEmpty())
    {
        return KeyboardEvent();
    }
    else
    {
        KeyboardEvent e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
}

unsigned char KeyboardClass::ReadChar()
{
    if (CharBufferIsEmpty())
    {
        return 0u;
    }
    else
    {
        unsigned char ch = charBuffer.front();
        charBuffer.pop();
        return ch;
    }
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
    keyStates[key] = true;
    keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
    keyStates[key] = false;
    keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
    keyStates[key] = true;
    charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
    autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
    autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
    autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
    autoRepeatChars = false;
}

bool KeyboardClass::IsKeyAutoRepeat() const
{
    return autoRepeatKeys;
}

bool KeyboardClass::IsCharAutoRepeat() const
{
    return autoRepeatChars;
}
