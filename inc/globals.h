#ifndef GLOBALS_H
#define GLOBALS_H

#define STRCMP(a, R, b) (strcmp(a, b) R 0)
#define MAX_INPUT_LEN (256 * 2 + 10 + 16 + 1)

#define initParams() pwdgenInitParams()
#define eraseParams() pwdgenEraseParams()

extern char *masterPwd;
extern char *account;
extern char *domain;
extern char *version;
extern char *password;
extern unsigned pwdLen;
extern int debug;

void initParams();
void eraseParams();

#endif
