#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>

char *masterPwd;
char *account;
char *domain;
char *version;
char *password;
unsigned pwdLen;
int debug;

extern inline void secureFree(void *ptr);

/**
 * Allocates the global input parameters and zeroises them.
 * XXX: Caller must free allocated memory.
 */
void initParams()
{
    masterPwd = calloc(257, sizeof *masterPwd);
    account = calloc(257, sizeof *account);
    domain = calloc(257, sizeof *domain);
    version = calloc(17, sizeof *version);
    password = calloc(257, sizeof *password);

    if (masterPwd == NULL || account == NULL || domain == NULL ||
        version == NULL || password == NULL)
    {
        fprintf(stderr, "Allocation error! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    pwdLen = 0;
    debug = 0;
}

/**
 * Zeroises and deallocates the global input parameters,
 * as well as makes them point to NULL.
 */
void eraseParams()
{
    memset(masterPwd, 0, 257);
    memset(account, 0, 257);
    memset(domain, 0, 257);
    memset(version, 0, 17);
    memset(password, 0, 257);

    secureFree(masterPwd);
    secureFree(account);
    secureFree(domain);
    secureFree(version);
    secureFree(password);

    pwdLen = 0;
}
