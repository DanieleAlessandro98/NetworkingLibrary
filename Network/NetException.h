#pragma once

#include <string>
#include <cstdarg>
#include <cstdio>

namespace Net
{
    class NetException
    {
        public:
            NetException(const char* format, ...)
            {
                va_list args;
                va_start(args, format);
                char buffer[1024];
                vsnprintf(buffer, sizeof(buffer), format, args);
                va_end(args);
                exception = buffer;
            }
            ~NetException() = default;

            const char* what() { return exception.c_str(); }
            std::string ToString() { return exception; }

        private:
            std::string exception;
    };
}
