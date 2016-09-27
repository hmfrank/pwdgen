#ifndef INC_IO_FILE_H_
#define INC_IO_FILE_H_

/* Prevent interpositioning */
#define saveAccount() pwdgenSaveAccount()
#define loadAccountLine(a) pwdgenLoadAccountLine(a)
#define saveCache() pwdgenSaveCache()
#define tryLoadCache() pwdgenTryLoadCache()

void saveAccount();
int loadAccountLine(long lineNum);
void saveCache();
int tryLoadCache();

#endif
