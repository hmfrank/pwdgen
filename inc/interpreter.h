#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>

#define interpretLastBits(a) pwdgenInterpretLastBits(a)

void interpretLastBits(uint8_t *digest);

#endif
