#include <stdlib.h>
#include <string.h>

#include <globals.h>

char *masterPwd;
char *account;
char *domain;
char *version;
char *password;

/* Allocates the parameters and zeroises them */
void initParams()
{
    masterPwd = calloc(256, sizeof (char));
    account = calloc(256, sizeof (char));
    domain = calloc(256, sizeof (char));
    version = calloc(32, sizeof (char));
    password = calloc(256, sizeof (char));
}

/* Zeroises and deallocates the parameters */
void eraseParams()
{
    memset(masterPwd, 0, 256);
    memset(account, 0, 256);
    memset(domain, 0, 256);
    memset(version, 0, 32);
    memset(password, 0, 256);

    free(masterPwd);
    free(account);
    free(domain);
    free(version);
    free(password);
}
