#pragma once

class Variable
{
public:
    Variable()
    {
        size = 0;
        uses = 0;
        definedOn = 0;
        allowDelete = false;
    }
    
    Variable(const int size, const int definedOn)
    {
        this->size = size;
        uses = 0;
        this->definedOn = definedOn;
        allowDelete = true;
    }
    
    /**
     * ***DON'T USE THIS*** unless you want the variable to be un-deletable
     */
    explicit Variable(const int size)
    {
        this->size = size;
        uses = 0;
        definedOn = 0;
        allowDelete = false;
    }
    
    int size;
    int uses;
    int definedOn;
    bool allowDelete;
};
