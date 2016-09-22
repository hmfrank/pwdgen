#ifndef INC_IOCLI_H_
#define INC_IOCLI_H_

/* Prevent interpositioning */
#define askForInput() pwdgenAskForInput()
#define showOutput() pwdgenShowOutput()

void askForInput();
void showOutput();

#endif
