#pragma once

#include <string>
#include <iostream>
#include <limits>
#include <cctype>

class InputValidator
{
public:

    static bool isBlank(const std::string& str)
    {
        for(char ch : str)
        {
            if(!std::isspace(ch))
            {
                return false;
            }
        }
        return true;
    }

    static std::string getValidString(
        const std::string& prompt,
        const std::string& errorMsg = "Input cannot be empty."
    )
    {
        clearInputBuffer();
        
        std::string input;
        while(true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);

            if(!isBlank(input))
            {
                return input;
            }
            std::cout << "✗ " << errorMsg << std::endl;
        }
    }

    static double getPositiveDouble(
        const std::string& prompt,
        double mustBeGreater = 0.0
    )
    {
        double value;
        while(true)
        {
            std::cout << prompt;
            std::cin >> value;

            if(std::cin.fail())
            {
                std::cin.clear();
                clearInputBuffer();
                std::cout << "✗ Invalid input. Enter a valid number." << std::endl;
                continue;
            }

            if(value > mustBeGreater)
            {
                clearInputBuffer();
                return value;
            }

            clearInputBuffer();
            std::cout << "✗ Value must be greater than " << mustBeGreater << std::endl;
        }
    }

    static int getPositiveInt(
        const std::string& prompt,
        int mustBeGreater = 0
    )
    {
        int value;
        while(true)
        {
            std::cout << prompt;
            std::cin >> value;

            if(std::cin.fail())
            {
                std::cin.clear();
                clearInputBuffer();
                std::cout << "✗ Invalid input. Enter a valid integer." << std::endl;
                continue;
            }

            if(value > mustBeGreater)
            {
                clearInputBuffer();
                return value;
            }

            clearInputBuffer();
            std::cout << "✗ Value must be greater than " << mustBeGreater << std::endl;
        }
    }

    static int getIntInRange(
        const std::string& prompt,
        int minVal,
        int maxVal
    )
    {
        int value;
        while(true)
        {
            std::cout << prompt;
            std::cin >> value;

            if(std::cin.fail())
            {
                std::cin.clear();
                clearInputBuffer();
                std::cout << "✗ Invalid input. Enter a valid integer." << std::endl;
                continue;
            }

            if(value >= minVal && value <= maxVal)
            {
                clearInputBuffer();
                return value;
            }

            clearInputBuffer();
            std::cout << "✗ Value must be between " << minVal 
                      << " and " << maxVal << std::endl;
        }
    }

    static void clearInputBuffer()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    static bool isApproximatelyEqual(
        double a,
        double b,
        double epsilon = 0.001
    )
    {
        double diff = (a > b) ? (a - b) : (b - a);
        return diff < epsilon;
    }

    static bool isEffectivelyZero(
        double value,
        double epsilon = 0.001
    )
    {
        return isApproximatelyEqual(value, 0.0, epsilon);
    }

    static bool hasDebt(
        double value,
        double epsilon = 0.001
    )
    {
        return !isEffectivelyZero(value, epsilon);
    }
};