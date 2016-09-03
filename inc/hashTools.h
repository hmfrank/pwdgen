#ifndef HASH_TOOLS_H
#define HASH_TOOLS_H

#include <stddef.h>
#include <stdint.h>

#define charToUint8_t(a, b, c) pwdgenCharToUint8_t(a, b, c)
#define encodeBase64(a, b, c, d) pwdgenEncodeBase64(a, b, c, d)
#define uint8_tToHex(a, b, c, d) pwdgenUint8_tToHex(a, b, c, d)
#define hashSHA256(a, b, c, d) pwdgenHashSHA256(a, b, c, d)

void charToUint8_t(uint8_t *output, char const *input, size_t len);
void encodeBase64(char *output, size_t outputLen, uint8_t const *input, size_t inputLen);
void uint8_tToHex(char *output, size_t outputLen, uint8_t const *input, size_t inputLen);
void hashSHA256(uint8_t *output, size_t outputLen, uint8_t const *input, size_t inputLen);

#endif
