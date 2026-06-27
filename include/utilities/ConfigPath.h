#pragma once

#include <string>
#include <filesystem>
#include <iostream>

namespace Config
{
    class Path
    {
    private:
        static std::string dataDirectory;

    public:

        static void initialize(const std::string& dir = "../data")
        {
            dataDirectory = dir;
            
            try
            {
                if(!std::filesystem::exists(dataDirectory))
                {
                    std::filesystem::create_directories(dataDirectory);
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << "✗ Error creating data directory: " << e.what() << std::endl;
            }
        }
        static std::string getGroupsFile()
        {
            return dataDirectory + "/groups.txt";
        }
        static std::string getMembersFile()
        {
            return dataDirectory + "/members.txt";
        }

        static std::string getExpensesFile()
        {
            return dataDirectory + "/expenses.txt";
        }

        static std::string getExpensePaymentsFile()
        {
            return dataDirectory + "/expense_payments.txt";
        }

        static std::string getFile(const std::string& filename)
        {
            return dataDirectory + "/" + filename;
        }

        static std::string getDataDirectory()
        {
            return dataDirectory;
        }

        static bool fileExists(const std::string& filepath)
        {
            return std::filesystem::exists(filepath);
        }

        static size_t getFileSize(const std::string& filepath)
        {
            try
            {
                return std::filesystem::file_size(filepath);
            }
            catch(const std::exception&)
            {
                return 0;
            }
        }
    };

   // std::string Path::dataDirectory = "../data";
}