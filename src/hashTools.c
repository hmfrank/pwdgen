#include <assert.h>
#include <openssl/sha.h>

#include <globals.h>
#include <hashTools.h>

/**
 * Converts a uint8_t string to the string of the corresponding base 64 chars.
 *
 * @param kInput    location of to-be-converted string
 * @param inputLen  length of input
 * @param output    location of future base 64 string
 * @param outputLen length of output
 */
void encodeBase64(uint8_t const *kInput, size_t inputLen, char *output, size_t outputLen)
{
    char b64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t i, j;

    /* Sanity check */
    assert(kInput != NULL && output != NULL);
    assert(outputLen >= inputLen / 3 * 4 + 1);
    /* Function does not support padding */
    assert(inputLen % 3 == 0);

    for (i = 0, j = 0; i < inputLen; i += 3, j += 4)
    {
        output[j] = b64Chars[(size_t) kInput[i] >> 2];
        output[j + 1] = b64Chars[(size_t) ((kInput[i] & 0x3) << 4) + (kInput[i + 1] >> 4)];
        output[j + 2] = b64Chars[(size_t) ((kInput[i + 1] & 0xF) << 2) + (kInput[i + 2] >> 6)];
        output[j + 3] = b64Chars[(size_t) kInput[i + 2] & 0x3F];
    }

    output[outputLen - 1] = 0;
}

/**
 * Hashes a uint8_t string with SHA256 and saves the digest as a uint8_t string.
 *
 * @param kInput    location of to-be-hashed string
 * @param inputLen  length of input
 * @param output    location of future digest string
 * @param outputLen length of output
 */
void hashSHA256(uint8_t const *kInput, size_t inputLen, uint8_t *output, size_t outputLen)
{
    unsigned char *inPtr = (unsigned char *) kInput;
    unsigned char *outPtr = (unsigned char *) output;
    size_t i;
    SHA256_CTX sha256;

    /* Sanity check */
    assert(kInput != NULL && output != NULL && outputLen >= 32);
    /* TODO: Add support for char type that is not 8 bits long */
    assert(SHA256_DIGEST_LENGTH == 32);

    i = SHA256_Init(&sha256);
    assert(i == 1);
    i = SHA256_Update(&sha256, inPtr, inputLen);
    assert(i == 1);
    i = SHA256_Final(outPtr, &sha256);
    assert(i == 1);
}

/**
 * A wrapper to SHA256-hash a string of char's and save the digest as hex char string.
 *
 * @param kInput    location of to-be-hashed string
 * @param inputLen  length of input (without '\0')
 * @param output    location of future digest string of hex characters
 * @param outputLen length of output
 */
void hashSHA256Hex(char const *kInput, size_t inputLen, char *output, size_t outputLen)
{
    char hexChars[] = "0123456789abcdef";
    uint8_t buffer[32] = {0};
    size_t i, j;

    /* Sanity check */
    assert(outputLen >= 65);

    hashSHA256((uint8_t *) kInput, inputLen, buffer, 32);

    for (i = 0, j = 0; i < 32; i += 1, j += 2)
    {
        output[j] = hexChars[buffer[i] >> 4];
        output[j + 1] = hexChars[buffer[i] & 0xF];
    }
}
