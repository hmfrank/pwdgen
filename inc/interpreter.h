#ifndef INC_INTERPRETER_H_
#define INC_INTERPRETER_H_

#include <stdint.h>

/* Prevent interpositioning */
#define interpretLastBits(a) pwdgenInterpretLastBits(a)

void interpretLastBits(uint8_t *digest);

#endif
