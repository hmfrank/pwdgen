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

#include <interpreter.h>
#include <generator.h>
#include <globals.h>

static char *bundleInput();
static bool validatePwd(char *password);
static void charToUint8_t(uint8_t *output, char const *input, size_t len);
static void hashSHA256(uint8_t *output, size_t outputLen, uint8_t const *input, size_t inputLen);
static void encodeBase64(char *output, size_t outputLen, uint8_t const *input, size_t inputLen);

/* Returns a string that consists of the bundled input */
/* Shape: account.len@domain.len:version */
static char *bundleInput()
{
    char *combination = calloc(256 * 2 + 10 + 16 + 1, sizeof (char));
    char buffer[5] = {0};
    assert(combination != NULL);

    strncpy(combination, account, 256);
    sprintf(buffer, ".%u@", (unsigned) strlen(account));
    strncat(combination, buffer, 5);
    memset(buffer, 0, 5);

    strncat(combination, domain, 256);
    sprintf(buffer, ".%u:", (unsigned) strlen(domain));
    strncat(combination, buffer, 5);

    strncat(combination, version, 16);
    memset(buffer, 0, 5);
    return combination;
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

/* Converts a char array to a uint8_t array */
static void charToUint8_t(uint8_t *output, char const *input, size_t len)
{
    /* Sanity check */
    assert(input != NULL && output != NULL);

    for (size_t i = 0; i < len; i++)
    {
        assert(input[i] >= 0);
        output[i] = (uint8_t) input[i];
    }
}

/* Hashes a uint8_t string with SHA256 and returns the digest as a uint8_t string */
static void hashSHA256(uint8_t *output, size_t outputLen, uint8_t const *input, size_t inputLen)
{
    unsigned char inBuffer[inputLen];
    unsigned char outBuffer[32];
    size_t i;
    SHA256_CTX sha256;

    /* Sanity check */
    assert(input != NULL && output != NULL);
    assert(outputLen >= 32);
    /* TODO: Add support for char type that is not 8 bits long */
    assert(SHA256_DIGEST_LENGTH == 32);

    for (i = 0; i < inputLen; i++)
    {
        inBuffer[i] = (unsigned char) input[i];
    }

    assert(SHA256_Init(&sha256) == 1);
    assert(SHA256_Update(&sha256, inBuffer, inputLen) == 1);
    assert(SHA256_Final(outBuffer, &sha256) == 1);

    for (i = 0; i < 32; i++)
    {
        output[i] = (uint8_t) outBuffer[i];
    }
}

/* Converts a uint8_t string to the corresponding base 64 char string */
static void encodeBase64(char *output, size_t outputLen, uint8_t const *input, size_t inputLen)
{
    char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t i, j;

    /* Sanity check */
    assert(input != NULL && output != NULL);
    assert(outputLen >= inputLen / 3 * 4 + 1);
    /* Function does not support padding */
    assert(inputLen % 3 == 0);

    for (i = 0, j = 0; i < inputLen; i += 3, j += 4)
    {
        output[j] = b64[(size_t) input[i] >> 2];
        output[j + 1] = b64[(size_t) ((input[i] & 0x3) << 4) + (input[i + 1] >> 4)];
        output[j + 2] = b64[(size_t) ((input[i + 1] & 0xF) << 2) + (input[i + 2] >> 6)];
        output[j + 3] = b64[(size_t) input[i + 2] & 0x3F];
    }

    output[outputLen - 1] = 0;
}

/* Generates a password and stores it in the global variable `password' */
/* TODO: Add more detailed description */
void generatePwd()
{
    char *buffer = bundleInput();
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
    memset(buffer, 0, strlen(buffer));
    memset(uint8MPwd, 0, 256);
    memset(uint8Input, 0, 256 * 2 + 10 + 16);
    memset(hashedMPwd, 0, 32);
    memset(hashedInput, 0, 32);
    memset(digest, 0, 194);
    free(buffer);
}
