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
    char *combination = calloc(256 * 3 + 64, sizeof (char));
    char *buffer = calloc(64, sizeof (char));

    strcpy(combination, account);
    sprintf(buffer, ".%u@", (unsigned) strlen(account));
    strcat(combination, buffer);

    strcat(combination, domain);
    sprintf(buffer, ".%u:", (unsigned) strlen(domain));
    strcat(combination, buffer);

    sprintf(buffer, "%u", version);
    strcat(combination, buffer);

    return combination;
}

/* Returns `true' if a password is valid */
static bool validatePwd()
{
    int length = (int) strlen(password);
    bool containsUpperCase = false;
    bool containsLowerCase = false;

    for (int i = 0; i < length; i++)
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
    strcat(password, "nop");
}

/* Returns a password, generated from the input */
void generatePwd()
{
    char input[256] = {0};
    strcpy(input, bundleInput());

    /* So loop condition fails, for testing purposes */
    strcat(input, "Aa");

    do
    {
        strcpy(password, input);
        /* TODO: Hash input to password */
        /* TODO: Use generated digest as new input */
    }
    while (!validatePwd());

    reinterpretPwd();
}
