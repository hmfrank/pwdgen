#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>

static char *bundleInput();
static bool validatePwd();
static void reinterpretPwd();

/* Returns a string that consists of the bundled input */
/* Shape: account.len@domain.len:version */
static char *bundleInput()
{
    char *combination = calloc(255 * 2 + 10 + 31 + 1, sizeof (char));
    char buffer[5] = {0};

    strncpy(combination, account, 255);
    sprintf(buffer, ".%u@", (unsigned) strlen(account));
    strncat(combination, buffer, 5);
    memset(buffer, 0, 5);

    strncat(combination, domain, 255);
    sprintf(buffer, ".%u:", (unsigned) strlen(domain));
    strncat(combination, buffer, 5);

    strncat(combination, version, 31);
    return combination;
}

/* Returns `true' if a password is valid */
static bool validatePwd()
{
    unsigned length = (unsigned) strlen(password);
    bool containsUpperCase = false;
    bool containsLowerCase = false;

    for (unsigned i = 0; i < length; i++)
    {
        if (65 <= password[i] && password[i] <= 90)
        {
            containsUpperCase = true;
        }

        if (97 <= password[i] && password[i] <= 122)
        {
            containsLowerCase = true;
        }
    }

    return containsUpperCase && containsLowerCase;
}

/* Changes the interpretation of bits in password */
static void reinterpretPwd()
{
    /* TODO: Add functionality */
    printf("nop\n");
}

/* Returns a password, generated from the input */
void generatePwd()
{
    char *input = bundleInput();
    printf("Input:\n%s\n", input);

    /* Just for testing, not final in any way */
    strncpy(password, input, 255);

    if (validatePwd())
    {
        printf("Password is valid.\n");
    }
    else
    {
        printf("Password is invalid.\n");
    }

#if 0
    do
    {
        /* TODO: Hash input to password */
        /* TODO: Use generated digest as new input */
    }
    while (!validatePwd());
#endif

    reinterpretPwd();
    free(input);
}
