#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <interpreter.h>

/* Interprets the last 12 bits of the digest and adjusts the password accordingly */
void interpretLastBits(uint8_t *digest)
{
    char specialChars[] = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
    char symbol = specialChars[digest[192] >> 3];
    char number = ((unsigned) (digest[192] & 0x7)) + '0';
    int pos = (unsigned) (digest[193] >> 5);
    int symbolFst = (int) ((digest[193] >> 4) & 0x1);

    for (int i = 253; i >= pos; i--)
    {
        if (password[i] == 0)
        {
            continue;
        }

        password[i + 2] = password[i];
    }

    if (symbolFst)
    {
        password[pos] = symbol;
        password[pos + 1] = number;
    }
    else
    {
        password[pos] = number;
        password[pos + 1] = symbol;
    }

    if (debug)
    {
        printf("Interpreter: symbol = %c, number = %c, pos = %d, symbolFst = %s\n",
            symbol, number, pos, symbolFst ? "true" : "false");
    }

    /* Erase confidential data */
    symbol = number = 0;
    pos = symbolFst = 0;
}
