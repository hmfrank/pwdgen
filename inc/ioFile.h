#ifndef INC_IO_FILE_H_
#define INC_IO_FILE_H_

/* Prevent interpositioning */
#define saveAccount() pwdgenSaveAccount()
#define loadAccountLine(a) pwdgenLoadAccountLine(a)

void saveAccount();
int loadAccountLine(long lineNum);

#endif
