#ifndef INC_GENERATOR_H_
#define INC_GENERATOR_H_

#include <stddef.h>

/* Prevent interpositioning */
#define generatePwd() pwdgenGeneratePwd()

void generatePwd();

#endif
