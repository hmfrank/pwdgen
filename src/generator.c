/* libscrypt requires size_t */
#include <stddef.h>

#include <assert.h>
#include <libscrypt.h>
#include <math.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <hashTools.h>
#include <interpreter.h>

static bool validatePwd(char *password);

/* Combines the input parameters and stores them in the output string */
/* Shape: account.len@domain.len:version */
void bundleInput(char *output, size_t outputLen)
{
    char buffer[5] = {0};

    /* Sanity check */
    assert(output != NULL);
    assert(outputLen >= 256 * 2 + 10 + 16 + 1);

    strncpy(output, account, 256);
    sprintf(buffer, ".%u@", (unsigned) strlen(account));
    strncat(output, buffer, 5);
    memset(buffer, 0, 5);

    strncat(output, domain, 256);
    sprintf(buffer, ".%u:", (unsigned) strlen(domain));
    strncat(output, buffer, 5);

    strncat(output, version, 16);
    memset(buffer, 0, 5);
}

/* Returns `true' if a password is valid */
static bool validatePwd(char *password)
{
    size_t length = strlen(password);
    bool containsUpperCase = false;
    bool containsLowerCase = false;

    for (size_t i = 0; i < length; i++)
    {
        if (65 <= password[i] && password[i] <= 90)
        {
            containsUpperCase = true;
        }

        if (97 <= password[i] && password[i] <= 122)
        {
            containsLowerCase = true;
        }
    }

    return containsUpperCase && containsLowerCase;
}

/* Generates a password and stores it in the global variable `password' */
/* TODO: Add more detailed description */
void generatePwd()
{
    char buffer[256 * 2 + 10 + 16 + 1] = {0};
    bundleInput(buffer, 256 * 2 + 10 + 16 + 1);
    uint8_t uint8MPwd[256] = {0};
    uint8_t uint8Input[256 * 2 + 10 + 16] = {0};
    uint8_t hashedMPwd[32] = {0};
    uint8_t hashedInput[32] = {0};
    uint8_t digest[194] = {0};

    /* Program will end up in infinite loop otherwise */
    assert(pwdLen >= 4);

    charToUint8_t(uint8MPwd, masterPwd, strlen(masterPwd));
    charToUint8_t(uint8Input, buffer, strlen(buffer));
    hashSHA256(hashedMPwd, 32, uint8MPwd, strlen(masterPwd));
    hashSHA256(hashedInput, 32, uint8Input, strlen(buffer));

    if (debug)
    {
        printf("\nInput: `%s\'\n", buffer);
    }

    do
    {
        /* password, len, salt, len, cost, block size, parallelisation, digest, len */
        /* TODO: Test for errors */
        libscrypt_scrypt(hashedMPwd, 32, hashedInput, 32, pow(2, 17), 8, 1, digest, 194);
        encodeBase64(buffer, 192 / 3 * 4 + 1, digest, 192);
        strncpy(password, buffer, pwdLen - 2);

        if (validatePwd(password))
        {
            if (debug)
            {
                printf("Scrypt: `%s\' <Okay>\n", password);
            }

            break;
        }
        else
        {
            if (debug)
            {
                printf("Scrypt: `%s\' <Bad password>\n", password);
            }

            hashSHA256(hashedInput, 32, digest, 194);
            continue;
        }
    }
    while (1);

    interpretLastBits(digest);

    /* Erase confidential data */
    memset(buffer, 0, 256 * 2 + 10 + 16 + 1);
    memset(uint8MPwd, 0, 256);
    memset(uint8Input, 0, 256 * 2 + 10 + 16);
    memset(hashedMPwd, 0, 32);
    memset(hashedInput, 0, 32);
    memset(digest, 0, 194);
}
