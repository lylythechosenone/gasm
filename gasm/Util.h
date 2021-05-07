#pragma once
#include <cstring>
#include <type_traits>

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
};
