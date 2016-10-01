#ifndef INC_GENERATOR_H_
#define INC_GENERATOR_H_

#include <stddef.h>

/* Prevent interpositioning */
#define generateCache() pwdgenGenerateCache()
#define generatePwd() pwdgenGeneratePwd()

int generateCache();
void generatePwd();

#endif
