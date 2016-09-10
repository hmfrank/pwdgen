#ifndef IO_FILE_H
#define IO_FILE_H

#define saveAccount() pwdgenSaveAccount()
#define loadAccountLine(a) pwdgenLoadAccountLine(a)

void saveAccount();
int loadAccountLine(long lineNum);

#endif
