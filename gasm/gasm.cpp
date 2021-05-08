#include <iostream>
#include <map>
#include <string>

#include "RuntimeError.h"
#include "Util.h"
#include "Variable.h"

// Arguments
const char *fileName = nullptr;
const char *format = nullptr;

// Constants
const char *formats[] = {"bin", "ith", "srec", "obj", "win32", "win64", "coff", "macho32", "macho64", "elf32", "elf64", "elfx32", "aout",
"aoutb", "as86", "rdf", "dbg", "nasm"};

std::map<std::string, int> sizes = {
    {"byte", 1},
    {"word", 2},
    {"dword", 4},
    {"qword", 8},
    
    {"bytes", 1},
    {"words", 2},
    {"dwords", 4},
    {"qwords", 8}
};

std::map<std::string, Variable> variables = {
    {"al", Variable(1)},
    {"ah", Variable(1)},
    {"ax", Variable(2)},
    {"eax", Variable(4)},
    {"rax", Variable(8)},

    {"cl", Variable(1)},
    {"ch", Variable(1)},
    {"cx", Variable(2)},
    {"ecx", Variable(4)},
    {"rcx", Variable(8)},

    {"dl", Variable(1)},
    {"dh", Variable(1)},
    {"dx", Variable(2)},
    {"edx", Variable(4)},
    {"rdx", Variable(8)},

    {"bl", Variable(1)},
    {"bh", Variable(1)},
    {"bx", Variable(2)},
    {"ebx", Variable(4)},
    {"rbx", Variable(8)},

    {"spl", Variable(1)},
    {"sp", Variable(2)},
    {"esp", Variable(4)},
    {"rsp", Variable(8)},

    {"bpl", Variable(1)},
    {"bp", Variable(2)},
    {"ebp", Variable(4)},
    {"rbp", Variable(8)},

    {"sil", Variable(1)},
    {"si", Variable(2)},
    {"esi", Variable(4)},
    {"rsi", Variable(8)},

    {"dil", Variable(1)},
    {"di", Variable(2)},
    {"edi", Variable(4)},
    {"rdi", Variable(8)}
};

void parseArgs(const int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--format") == 0 || strcmp(argv[i], "-f") == 0)
        {
            if (format == nullptr)
            {
                if (Util::includes<sizeof formats / sizeof(char *)>(formats, argv[i + 1]))
                {
                    format = argv[i + 1];
                    i++;
                } else
                {
                    throw RuntimeError(std::string("Unknown format '") + argv[i + 1] + "'", true);
                }
            } else
            {
                throw RuntimeError(std::string("Format is already set, attempted to reset to '") + argv[i + 1] + "'");
            }
        } else
        {
            if (fileName == nullptr)
            {
                fileName = argv[i];
            } else
            {
                throw RuntimeError(std::string("Unknown argument '") + argv[i] + "'", true);
            }
        }
    }
}

void printHelp()
{
    std::string formatsString;
    for (int i = 0; i < sizeof formats / sizeof(char *); i++)
    {
        formatsString += formats[i];
        if (i != sizeof formats / sizeof(char *) - 1)
        {
            formatsString += ", ";
        }
        if ((i & 7) == 7)
        {
            formatsString += "\n\t";
        }
    }
    std::cout << "Usage:" << '\n' <<
        "[filename]: the file to compile" << '\n' <<
        "-f/--format [format]: the format to output. Possible options:" << '\n' << '\t' <<
            formatsString << '\n' <<
        "-h/--help/?: display this message" << '\n';
}

int main(const int argc, char* argv[])
{
    try
    {
        parseArgs(argc, argv);
        // Make sure all required arguments were specified
        if (fileName == nullptr)
        {
            throw RuntimeError("Filename was not specified", true);
        }

        std::ifstream stream(fileName);
        if (!stream.good())
        {
            throw RuntimeError(std::string("No file '") + fileName + "' exists, or it is inaccessible.");
        }

        int lineNum = 0;
        while (true)
        {
            lineNum++;
            std::string line;
            std::getline(stream, line);

            auto instruction = Util::split(line, " ");
            std::string translated;
            
            if (instruction[0] == "move")
            {
                // move <thing> to <place>
                if (variables.count(instruction[1]) == 0)
                {
                    throw RuntimeError(std::string("Position ") + std::to_string(lineNum) + ":" +
                        std::to_string(instruction[0].length() + 2) + ": "
                        "Invalid operation. No variable '" + instruction[1] + "' exists.");
                }

                if (variables.count(instruction[3]) == 0)
                {
                    throw RuntimeError(std::string("Position ") + std::to_string(lineNum) + ":" +
                        std::to_string(instruction[0].length() + 1 + instruction[1].length() + 5) + ": "
                        "Invalid operation. No variable '" + instruction[1] + "' exists.");
                }

                if (variables[instruction[1]].size > variables[instruction[3]].size)
                {
                    throw RuntimeError(std::string("Position ") + std::to_string(lineNum) + ":" +
                        std::to_string(instruction[0].length() + 1 + instruction[1].length() + 5) + ": "
                        "Cannot fit '" + instruction[1] + "' inside of '" + instruction[3] + "' because it is too large.");
                }
                
                translated = "mov " + instruction[3] + ", " + instruction[1];
            }
            else if (instruction[0] == "reserve")
            {
                // reserve <number> <size> as <name>
                if (sizes.count(instruction[2]) == 0)
                {
                    throw RuntimeError(std::string("Position ") + std::to_string(lineNum) + ":" +
                        std::to_string(instruction[0].length() + 1 + instruction[1].length() + 2) + ": "
                        "Unknown size '" + instruction[2] + "'");
                }

                if (variables.count(instruction[4]) == 1)
                {
                    throw RuntimeError(std::string("Position ") + std::to_string(lineNum) + ":" +
                        std::to_string(instruction[0].length() + 1 + instruction[1].length() + 1 + instruction[2].length() + 5) + ": "
                        "Variable '" + instruction[4] + "' is already defined.");
                }

                const int size = std::stoi(instruction[1]) * sizes[instruction[2]];
                variables[instruction[4]] = Variable(size, lineNum);
                translated = instruction[4] + " resb " + std::to_string(size);
            }

            std::cout << translated << '\n';
            
            if (stream.eof()) break;
        }
        
    } catch (RuntimeError err)
    {
        std::cerr << err.message << '\n';
        if (err.displayHelpMessage)
        {
            printHelp();
        }
        return -1;
    }
    return 0;
}
