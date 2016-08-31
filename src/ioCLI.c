#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ioCLI.h>
#include <globals.h>

#define STRCMP(a, R, b) (strcmp(a, b) R 0)

static void flush();

/* Asks the user for the generation parameters */
void askForInput()
{
    char buffer[258] = {0};
    size_t index;

    printf("== Deterministic Password Generator (V3) ==\n"
        "Please enter the following:\n");

    do
    {
        printf("Account /.{256}/:           ");
        fgets(buffer, 258, stdin);
        index = strcspn(buffer, "\n");

        /* Remove trailing '\n' */
        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        /* Stdin is not empty */
        if (buffer[256] != 0) {
            flush();
            buffer[256] = 0;
        }
    }
    while (strlen(buffer) == 0);

    strncpy(account, buffer, 257);
    memset(buffer, 0, 258);

    do
    {
        printf("Domain  /.{256}/:           ");
        fgets(buffer, 258, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[256] != 0)
        {
            flush();
            buffer[256] = 0;
        }
    }
    while (strlen(buffer) == 0);

    strncpy(domain, buffer, 257);
    memset(buffer, 0, 258);

    do
    {
        printf("Version /[1-9][0-9]{0,15}/: ");
        fgets(buffer, 18, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[16] != 0)
        {
            flush();
            buffer[16] = 0;
        }
    }
    while (strlen(buffer) == 0 ||
        strspn(buffer, "0123456789") < strlen(buffer));

    strncpy(version, buffer, 17);
    memset(buffer, 0, 257);

    do
    {
        printf("Length  (4..256):           ");
        fgets(buffer, 5, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[3] != 0)
        {
            flush();
            buffer[3] = 0;
        }

        pwdLen = (unsigned) strtoul(buffer, NULL, 10);
    }
    while (pwdLen < 4 || pwdLen > 256);

    memset(buffer, 0, 257);
}

/* Empties stdin up to the next newline */
static void flush()
{
    int ch;

    do
    {
        ch = getchar();
    }
    while (ch != EOF && ch != '\n');
}

/* Shows the user the generated password */
void showOutput()
{
    printf("Password: `%s\'\n", password);
}
