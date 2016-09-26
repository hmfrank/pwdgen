#ifndef INC_IOCLI_H_
#define INC_IOCLI_H_

/* Prevent interpositioning */
#ifndef USE_ARGV_ONLY
    #define getInput() pwdgenGetInput()
    void getInput();
#else
    #define getInput(a, b) pwdgenGetInput(a, b)
    void getInput(int argc, char *argv[]);
#endif

#define showOutput() pwdgenShowOutput()
void showOutput();

#endif
