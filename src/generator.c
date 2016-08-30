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

static char *bundleInput();
static bool validatePwd();
static void hashSHA256(char const *input, size_t inputLen, uint8_t *output, size_t outputLen);
static void reinterpretPwd();

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
    return combination;
}

/* Returns `true' if a password is valid */
static bool validatePwd()
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

/* Hashes the input with SHA256 and converts the result to uint8_t */
static void hashSHA256(char const *input, size_t inputLen, uint8_t *output, size_t outputLen)
{
    unsigned char inBuffer[inputLen];
    unsigned char outBuffer[32];
    size_t i;
    SHA256_CTX sha256;

    /* Sanity check */
    assert(input != NULL);
    assert(output != NULL);
    assert(outputLen >= 32);
    /* TODO: Add support for char type that is not 8 bits long */
    assert(SHA256_DIGEST_LENGTH == 32);

    for (i = 0; i < inputLen; i++)
    {
        assert(input[i] >= 0);
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

/* Generates a password and stores it in the global variable `password' */
/* TODO: Add more detailed description */
void generatePwd()
{
    char *input = bundleInput();
    uint8_t hashedMPwd[32];
    uint8_t hashedInput[32];
    uint8_t digest[32];
    uint64_t cost = pow(2, 17);

    printf("Input:\n`%s\'\n", input);

    hashSHA256(masterPwd, strlen(masterPwd), hashedMPwd, 32);
    hashSHA256(input, strlen(input), hashedInput, 32);

    /* Just for testing, not final in any way */
    strncpy(password, input, 256);

    if (validatePwd())
    {
        printf("Password is valid.\n");
    }
    else
    {
        printf("Password is invalid. Enjoy your infinite loop.\n");
    }

    do
    {
        /* password, len, salt, len, cost, block size, parallelisation, digest, len */
        /* TODO: Test for errors */
        libscrypt_scrypt(hashedMPwd, 32, hashedInput, 32, cost, 8, 1, digest, 32);
        /* TODO: Convert digest to base 64 */
        /* TODO: Use generated digest as new input */
    }
    while (!validatePwd());

    printf("Scrypt digest: ");
    for (size_t i = 0; i < 32; i++)
    {
        printf("%u : ", (unsigned) digest[i]);
    }
    printf("\b\b  \n");

    reinterpretPwd();
    free(input);
}

/* Changes the interpretation of bits in password */
static void reinterpretPwd()
{
    /* TODO: Add functionality */
    printf("nop\n");
}
