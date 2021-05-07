#pragma once
#include <string>

class RuntimeError
{
public:
    RuntimeError(const std::string message)
    {
        this->message = message;
        this->displayHelpMessage = false;
    }

    RuntimeError(const std::string message, const bool displayHelpMessage)
    {
        this->message = message;
        this->displayHelpMessage = displayHelpMessage;
    }
    
    std::string message;
    bool displayHelpMessage;
};
