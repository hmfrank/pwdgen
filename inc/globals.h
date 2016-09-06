#ifndef GLOBALS_H
#define GLOBALS_H

#define STRCMP(a, R, b) (strcmp(a, b) R 0)

/* "account.len(account)@domain.len(domain):version\0" */
#define MAX_INPUT_LEN (256 + 5 + 256 + 5 + 16 + 1)
/* "Hash account.len(account)@domain.len(domain):version:pwdLen\n\0" */
#define MAX_LINE_LEN (64 + 1 + MAX_INPUT_LEN - 1 + 1 + 3 + 2)

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
