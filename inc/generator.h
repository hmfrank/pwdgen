#ifndef GENERATOR_H
#define GENERATOR_H

#include <stddef.h>

/* `Defuses' global function */
#define bundleInput(a, b) pwdgenBundleInput(a, b)
#define generatePwd() pwdgenGeneratePwd()

void bundleInput(char *output, size_t outputLen);
void generatePwd();

#endif
