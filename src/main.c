#include <string.h>

#include <generator.h>
#include <globals.h>
#include <ioCLI.h>

int main(int argc, char *argv[])
{
    initParams();

    if (argc >= 2 && STRCMP(argv[1], ==, "-d"))
    {
        debug = 1;
    }

    askForInput();
    generatePwd();
    showOutput();
    eraseParams();

    return 0;
}
