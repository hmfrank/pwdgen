#include <string.h>

#include <ioCLI.h>
#include <generator.h>
#include <globals.h>

int main()
{
    initParams();
    askForInput();
    generatePwd();
    showOutput();
    eraseParams();

    return 0;
}
