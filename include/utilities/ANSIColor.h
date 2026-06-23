#pragma once

#include <string>
#include <iostream>

namespace Colors
{
    constexpr const char* RESET = "\033[0m";
    constexpr const char* BLACK = "\033[30m";
    constexpr const char* RED = "\033[31m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* BLUE = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* WHITE = "\033[37m";

    constexpr const char* BRIGHT_BLACK = "\033[90m";
    constexpr const char* BRIGHT_RED = "\033[91m";
    constexpr const char* BRIGHT_GREEN = "\033[92m";
    constexpr const char* BRIGHT_YELLOW = "\033[93m";
    constexpr const char* BRIGHT_BLUE = "\033[94m";
    constexpr const char* BRIGHT_MAGENTA = "\033[95m";
    constexpr const char* BRIGHT_CYAN = "\033[96m";
    constexpr const char* BRIGHT_WHITE = "\033[97m";

    constexpr const char* BG_BLACK = "\033[40m";
    constexpr const char* BG_RED = "\033[41m";
    constexpr const char* BG_GREEN = "\033[42m";
    constexpr const char* BG_YELLOW = "\033[43m";
    constexpr const char* BG_BLUE = "\033[44m";
    constexpr const char* BG_MAGENTA = "\033[45m";
    constexpr const char* BG_CYAN = "\033[46m";
    constexpr const char* BG_WHITE = "\033[47m";

    constexpr const char* BOLD = "\033[1m";
    constexpr const char* DIM = "\033[2m";
    constexpr const char* ITALIC = "\033[3m";
    constexpr const char* UNDERLINE = "\033[4m";



    inline void success(const std::string& message)
    {
        std::cout << GREEN << "✓ " << message << RESET << std::endl;
    }

    inline void error(const std::string& message)
    {
        std::cout << RED << "✗ " << message << RESET << std::endl;
    }

    inline void warning(const std::string& message)
    {
        std::cout << YELLOW << "⚠ " << message << RESET << std::endl;
    }

    inline void info(const std::string& message)
    {
        std::cout << BLUE << "ℹ " << message << RESET << std::endl;
    }

    inline void header(const std::string& message)
    {
        std::cout << BRIGHT_CYAN << BOLD << message << RESET << std::endl;
    }

    inline void highlight(const std::string& message)
    {
        std::cout << BRIGHT_YELLOW << message << RESET << std::endl;
    }

    // Menu separator line
    inline void separator()
    {
        std::cout << BRIGHT_BLACK << "=================================" << RESET << std::endl;
    }

    // Create colored text (returns string)
    inline std::string format(const std::string& text, const char* color)
    {
        return std::string(color) + text + RESET;
    }
}