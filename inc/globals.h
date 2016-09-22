#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#include <stdlib.h>

/* My scary macro stash */
#define STRCMP(a, R, b) (strcmp(a, b) R 0)
#define STRNCMP(a, R, b, l) (strncmp(a, b, l) R 0)

/* "account.len(account)@domain.len(domain):version\0" */
#define MAX_INPUT_LEN (256 + 5 + 256 + 5 + 16 + 1)
/* "Hash account.len(account)@domain.len(domain):version:pwdLen\n\0" */
#define MAX_LINE_LEN (64 + 1 + 256 + 1 + 256 + 1 + 16 + 1 + 3 + 1 + 1)
/* "Constants" for I/O modules */
#define F_FILE_PATH "accounts.pwdgen-list"
#define F_FILE_PATH_TMP "accountsTmp.pwdgen-list"

/* Global input parameters */
/* Points to string representation of master password */
extern char *masterPwd;
/* Points to string representation of account */
extern char *account;
/* Points to string representation of domain */
extern char *domain;
/* Points to string representation of version */
extern char *version;
/* Length of generated password */
extern unsigned pwdLen;

/* Points to string representation of generated password */
extern char *password;
/* Toggles debug features of the program */
extern int debug;

/* Prevent interpositioning */
#define initParams() pwdgenInitParams()
#define eraseParams() pwdgenEraseParams()
#define secureFree(a) pwdgenSecureFree(a)

void initParams();
void eraseParams();

/**
 * Prevents dangling pointers by setting them to NULL after freeing.
 *
 * @param ptr to-be-freed pointer
 */
inline void secureFree(void *ptr)
{
    free(ptr);
    ptr = NULL;
}

#endif
