#include <string.h>

#include <generator.h>
#include <globals.h>
#include <ioCLI.h>

int main(int argc, char *argv[])
{
    eraseParams();

#ifndef USE_ARGV_ONLY
    if (argc >= 2 && STRCMP(argv[1], ==, "-d"))
    {
        debug = 1;
    }

    getInput();
#else
    getInput(argc, argv);
#endif

    generatePwd();
    showOutput();
    eraseParams();

    return 0;
}
