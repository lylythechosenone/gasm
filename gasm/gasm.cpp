#include <iostream>
#include <string>

#include "RuntimeError.h"
#include "Util.h"

// Arguments
const char *fileName = nullptr;
const char *format = nullptr;

// Constants
const char *formats[] = {"bin", "ith", "srec", "obj", "win32", "win64", "coff", "macho32", "macho64", "elf32", "elf64", "elfx32", "aout",
"aoutb", "as86", "rdf", "dbg", "nasm"};

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
