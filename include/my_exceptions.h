#pragma once
#include <exception>

class ConfigFileMissing : public std::exception
{
    const char* what() const noexcept override
    {
        return "\"config.json\" file is missing";
    }
};

class ConfigFileEmpty : public std::exception
{
    const char* what() const noexcept override
    {
        return "config file is empty";
    }
};

class DocFileMissing : public std::exception
{
    const char* what() const noexcept override
    {
        return "Document file is missing";
    }
};

class RequestsFileMissing : public std::exception
{
    const char* what() const noexcept override
    {
        return "\"requests.json\" file is missing";
    }
};