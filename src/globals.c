#include <assert.h>
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

/* Allocates the parameters and zeroises them */
void initParams()
{
    masterPwd = calloc(257, sizeof (char));
    account = calloc(257, sizeof (char));
    domain = calloc(257, sizeof (char));
    version = calloc(17, sizeof (char));
    password = calloc(257, sizeof (char));

    assert(masterPwd != NULL && account != NULL && domain != NULL &&
        version != NULL && password != NULL);

    pwdLen = 0;
    debug = 0;
}

/* Zeroises and deallocates the parameters */
void eraseParams()
{
    memset(masterPwd, 0, 257 * sizeof (char));
    memset(account, 0, 257 * sizeof (char));
    memset(domain, 0, 257 * sizeof (char));
    memset(version, 0, 17 * sizeof (char));
    memset(password, 0, 257 * sizeof (char));

    free(masterPwd);
    free(account);
    free(domain);
    free(version);
    free(password);

    pwdLen = 0;
}
