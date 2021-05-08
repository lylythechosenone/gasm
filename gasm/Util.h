#pragma once
#include <cstring>
#include <fstream>
#include <type_traits>
#include <vector>

class Util
{
public:
    template<size_t Size>
    static bool includes(const char *array[Size], const char *item)
    {
        for (int i = 0; i < Size; i++)
        {
            if (strcmp(item, array[i]) == 0)
            {
                return true;
            }
        }
        return false;
    }
    
    template<typename T, typename std::enable_if<!std::is_same<T, char *>::value>::type* = nullptr, size_t Size>
    static bool includes(T array[Size], T item)
    {
        for (int i = 0; i < Size; i++)
        {
            if (array[i] == item)
            {
                return true;
            }
        }
        return false;
    }

    static std::vector<std::string> split(std::string str, std::string delimiter)
    {
        std::vector<std::string> toReturn;
        
        auto start = 0U;
        auto end = str.find(delimiter);
        while (end != std::string::npos)
        {
            toReturn.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        toReturn.push_back(str.substr(start, end));

        return toReturn;
    }
};
