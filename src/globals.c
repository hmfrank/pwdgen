#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>

char masterPwd[257];
char account[257];
char domain[257];
char version[17];
char password[257];
uint8_t cache[32];
unsigned pwdLen;
int debug;

/**
 * Zeroises the global input parameters.
 */
void eraseParams()
{
    memset(masterPwd, 0, 257);
    memset(account, 0, 257);
    memset(domain, 0, 257);
    memset(version, 0, 17);
    memset(password, 0, 257);
    memset(cache, 0, 32);

    pwdLen = 0;
    debug = 0;
}
