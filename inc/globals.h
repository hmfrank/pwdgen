#ifndef GLOBALS_H
#define GLOBALS_H

#define initParams() pwdgenInitParams()
#define eraseParams() pwdgenEraseParams()

extern char *masterPwd;
extern char *account;
extern char *domain;
extern char *version;
extern char *password;

void initParams();
void eraseParams();

#endif
