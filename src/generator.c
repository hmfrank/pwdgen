#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>

/* Returns a string that consists of the bundled input */
/* Shape: account.len@domain.len:version */
char *bundleInput()
{
    char *combination = calloc(832, sizeof (char));
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
bool validPassword()
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

/* Changes interpretation of bits in password */
void reinterpretPwd()
{
    /* TODO: Add functionality */
    strcat(password, "nop");
}

/* Returns a password, generated from the input */
void generatePwd()
{
    char input[256] = {0};
    /* TODO: Hash output of `bundleInput()' */
    strcpy(input, bundleInput());
    /* TODO: Hash master password */

#if 0
    do
    {
        /* TODO: Hash input to password */
        /* TODO: Use generated digest as new input */
    }
    while (!validatePwd());
#endif

    strcpy(password, input);
    reinterpretPwd();
}
