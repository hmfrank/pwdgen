#include <stdbool.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <ioCLI.h>

#define STRCMP(a, R, b) (strcmp(a, b) R 0)

int main(int argc, char *argv[])
{
    initParams();

    if (argc >= 2 && STRCMP(argv[1], ==, "-d"))
    {
        debug = true;
    }

    askForInput();
    generatePwd();
    showOutput();
    eraseParams();

    return 0;
}
