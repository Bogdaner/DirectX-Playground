#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log( const std::string& message )
{
    MessageBoxA( NULL, ( "Error: " + message ).c_str(), "Error", MB_ICONERROR );
}

void ErrorLogger::Log( HRESULT hr, const std::string& message)
{
    const _com_error error{ hr }; // to recive information about the error
    std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
    MessageBoxW( NULL, error_message.c_str(), L"Error", MB_ICONERROR );
}

void ErrorLogger::Log( HRESULT hr, const std::wstring & message )
{
    const _com_error error{ hr }; // to recive information about the error
    std::wstring error_message = L"Error: " + message  + L"\n" + error.ErrorMessage();
    MessageBoxW( NULL, error_message.c_str(), L"Error", MB_ICONERROR );
}
