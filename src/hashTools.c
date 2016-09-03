#include <assert.h>
#include <openssl/sha.h>

#include <globals.h>
#include <hashTools.h>

/* Converts a char string to the string of the corresponding uint8_t values */
void charToUint8_t(uint8_t *output, char const *input, size_t len)
{
    size_t i;

    /* Sanity check */
    assert(input != NULL && output != NULL);

    for (i = 0; i < len; i++)
    {
        assert(input[i] >= 0);
        output[i] = (uint8_t) input[i];
    }
}

/* Converts a uint8_t string to the string of the corresponding base 64 chars */
void encodeBase64(char *output, size_t outputLen, uint8_t const *input, size_t inputLen)
{
    char b64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t i, j;

    /* Sanity check */
    assert(input != NULL && output != NULL);
    assert(outputLen >= inputLen / 3 * 4 + 1);
    /* Function does not support padding */
    assert(inputLen % 3 == 0);

    for (i = 0, j = 0; i < inputLen; i += 3, j += 4)
    {
        output[j] = b64Chars[(size_t) input[i] >> 2];
        output[j + 1] = b64Chars[(size_t) ((input[i] & 0x3) << 4) + (input[i + 1] >> 4)];
        output[j + 2] = b64Chars[(size_t) ((input[i + 1] & 0xF) << 2) + (input[i + 2] >> 6)];
        output[j + 3] = b64Chars[(size_t) input[i + 2] & 0x3F];
    }

    output[outputLen - 1] = 0;
}

/* Converts a uint8_t string to the string of the corresponding hex chars */
void uint8_tToHex(char *output, size_t outputLen, uint8_t const *input, size_t inputLen)
{
    char hexChars[] = "0123456789abcdef";
    size_t i, j;

    /* Sanity check */
    assert(input != NULL && output != NULL);
    assert(outputLen >= inputLen * 2 + 1);

    for (i = 0, j = 0; i < inputLen; i += 1, j += 2)
    {
        output[j] = hexChars[input[i] >> 4];
        output[j + 1] = hexChars[input[i] & 0xF];
    }

    output[outputLen - 1] = 0;
}

/* Hashes a uint8_t string with SHA256 and returns the digest as a uint8_t string */
void hashSHA256(uint8_t *output, size_t outputLen, uint8_t const *input, size_t inputLen)
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
