#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#include <stdint.h>

/* My scary macro stash */
#define STRCMP(a, R, b) (strcmp(a, b) R 0)
#define STRNCMP(a, R, b, l) (strncmp(a, b, l) R 0)

/* "account.len(account)@domain.len(domain):version\0" */
#define MAX_INPUT_LEN (256 + 5 + 256 + 5 + 16 + 1)
/* "Hash account.len(account)@domain.len(domain):version:pwdLen\n\0" */
#define MAX_LINE_LEN (64 + 1 + 256 + 1 + 256 + 1 + 16 + 1 + 3 + 1 + 1)

/* "Constants" for I/O modules */
#define F_LIST_PATH "accounts.pwdgen-list"
#define F_LIST_TMP_PATH "accountsTmp.pwdgen-list"
#define F_CACHE_PATH "cache.pwdgen-cache"

/* Global input parameters */
/* Points to master password */
extern char *masterPwd;
/* Points to account name */
extern char *account;
/* Points to domain name */
extern char *domain;
/* Points to version */
extern char *version;
/* Points to cache (generated from master password in costly manner) */
extern uint8_t *cache;
/* Length of generated password */
extern unsigned pwdLen;

/* Points to string representation of generated password */
extern char *password;
/* Toggles debug features of the program */
extern int debug;

/* Prevent interpositioning */
#define initParams() pwdgenInitParams()
#define eraseParams() pwdgenEraseParams()

void initParams();
void eraseParams();

#endif
