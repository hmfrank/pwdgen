/* libscrypt requires size_t */
#include <stddef.h>

#include <assert.h>
#include <libscrypt.h>
#include <math.h>
#include <openssl/sha.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <hashTools.h>
#include <interpreter.h>

static int validatePwd(char *password);

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
static int validatePwd(char *password)
{
    size_t length = strlen(password);
    int containsUpperCase = 0;
    int containsLowerCase = 0;

    for (size_t i = 0; i < length; i++)
    {
        if (65 <= password[i] && password[i] <= 90)
        {
            containsUpperCase = 1;
        }

        if (97 <= password[i] && password[i] <= 122)
        {
            containsLowerCase = 1;
        }
    }

    return containsUpperCase && containsLowerCase;
}

/* Generates a password and stores it in the global variable `password' */
/* TODO: Add more detailed description */
void generatePwd()
{
    uint8_t hashedMPwd[32] = {0};
    uint8_t hashedInput[32] = {0};
    uint8_t digest[194] = {0};
    size_t lenInput = 0;

    union
    {
        char inChar[MAX_INPUT_LEN];
        uint8_t uint8Input[MAX_INPUT_LEN - 1];
        uint8_t uint8MPwd[256];
    } buff;

    /* Program will end up in infinite loop otherwise */
    /* If this occurs, ioCLI has made a mistake */
    assert(pwdLen >= 4);

    memset(buff.uint8MPwd, 0, 256);
    charToUint8_t(buff.uint8MPwd, masterPwd, strlen(masterPwd));
    hashSHA256(hashedMPwd, 32, buff.uint8MPwd, strlen(masterPwd));
    memset(buff.inChar, 0, MAX_INPUT_LEN * sizeof (char));
    bundleInput(buff.inChar, MAX_INPUT_LEN);

    if (debug)
    {
        printf("\nInput: `%s\'\n", buff.inChar);
    }

    lenInput = strlen(buff.inChar);
    charToUint8_t(buff.uint8Input, buff.inChar, lenInput);
    hashSHA256(hashedInput, 32, buff.uint8Input, lenInput);


    do
    {
        /* password, len, salt, len, cost, block size, parallelisation, digest, len */
        /* TODO: Test for errors */
        libscrypt_scrypt(hashedMPwd, 32, hashedInput, 32, pow(2, 17), 8, 1, digest, 194);
        encodeBase64(buff.inChar, 192 / 3 * 4 + 1, digest, 192);
        strncpy(password, buff.inChar, pwdLen - 2);

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
    memset(buff.inChar, 0, MAX_INPUT_LEN * sizeof (char));
    memset(hashedMPwd, 0, 32);
    memset(hashedInput, 0, 32);
    memset(digest, 0, 194);
}
