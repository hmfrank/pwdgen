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

static void bundleInput(char *output, size_t outputLen);
static int validatePwd(char *password);

/**
 * Combines the global input parameters and stores the result in output.
 * Shape: account.len(account)@domain.len(domain):version
 *
 * @param output    combination will be stored here
 * @param outputLen length of output
 */
static void bundleInput(char *output, size_t outputLen)
{
    /* Sanity check */
    assert(output != NULL && outputLen >= MAX_INPUT_LEN);

    strncpy(output, account, 256);
    snprintf(output + strlen(output), 6, ".%u@", (unsigned) strlen(account));
    strncat(output, domain, 256);
    snprintf(output + strlen(output), 6, ".%u:", (unsigned) strlen(domain));
    strncat(output, version, 16);
}

/**
 * Checks if a password is valid or not.
 *
 * @param password points to the to-be-checked password
 * @return         1 when password is valid, 0 otherwise
 */
static int validatePwd(char *password)
{
    /* Sanity check */
    assert(password != NULL);

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

/**
 * Generates a password from the input parameters and the master password,
 * and stores it in the global variable `password'.
 */
/* TODO: Add more detailed description */
void generatePwd()
{
    uint8_t hashedMPwd[32] = {0};
    uint8_t hashedInput[32] = {0};
    uint8_t digest[194] = {0};
    char buffer[MAX_INPUT_LEN] = {0};

    /* Program will end up in infinite loop otherwise */
    /* If this occurs, some terrible mistake has happend */
    assert(pwdLen >= 4);

    hashSHA256((uint8_t *) masterPwd, strlen(masterPwd), hashedMPwd, 32);
    bundleInput(buffer, MAX_INPUT_LEN);
    hashSHA256((uint8_t *) buffer, strlen(buffer), hashedInput, 32);

    if (debug)
    {
        printf("\nInput: `%s\'\n", buffer);
    }

    do
    {
        /* password, len, salt, len, cost, block size, parallelisation, digest, len */
        /* TODO: Test for errors */
        libscrypt_scrypt(hashedMPwd, 32, hashedInput, 32, pow(2, 17), 8, 1, digest, 194);
        encodeBase64(digest, 192, buffer, 192 / 3 * 4 + 1);
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
    memset(hashedMPwd, 0, 32);
    memset(hashedInput, 0, 32);
    memset(digest, 0, 194);
    memset(buffer, 0, MAX_INPUT_LEN);
}
