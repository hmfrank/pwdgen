#ifndef INC_HASH_TOOLS_H_
#define INC_HASH_TOOLS_H_

#include <stddef.h>
#include <stdint.h>

/* Prevent interpositioning */
#define charToUint8_t(a, b, c) pwdgenCharToUint8_t(a, b, c)
#define encodeBase64(a, b, c, d) pwdgenEncodeBase64(a, b, c, d)
#define uint8_tToHex(a, b, c, d) pwdgenUint8_tToHex(a, b, c, d)
#define hashSHA256(a, b, c, d) pwdgenHashSHA256(a, b, c, d)

void encodeBase64(uint8_t const *kInput, size_t inputLen, char *output, size_t outputLen);
void hashSHA256(uint8_t const *kInput, size_t inputLen, uint8_t *output, size_t outputLen);
void hashSHA256Hex(char const *kInput, size_t inputLen, char *output, size_t outputLen);

#endif
