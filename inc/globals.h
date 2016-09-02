#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

#define initParams() pwdgenInitParams()
#define eraseParams() pwdgenEraseParams()

extern char *masterPwd;
extern char *account;
extern char *domain;
extern char *version;
extern char *password;
extern unsigned pwdLen;
extern bool debug;

void initParams();
void eraseParams();

#endif
