#pragma once
#include "StringConverter.h"
#include<Windows.h>

class ErrorLogger
{
public:
    static void Log(const std::string& message);
    static void Log(const HRESULT hr, const std::string& message);
};