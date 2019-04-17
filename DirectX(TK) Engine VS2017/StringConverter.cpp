#include "StringConverter.h"

std::wstring StringConverter::StringToWide(const std::string& str)
{
    const std::wstring wide{ str.begin(), str.end() };
    return wide;
}