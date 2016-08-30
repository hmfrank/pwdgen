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
    char buffer[257] = {0};
    size_t index;

    printf("== Deterministic Password Generator (V3) ==\n"
        "Please enter the following:\n");

    do
    {
        printf("Account: ");
        fgets(buffer, 257, stdin);
        index = strcspn(buffer, "\n");

        /* Remove trailing '\n' */
        if (index < strlen(buffer))
        {
            buffer[strcspn(buffer, "\n")] = 0;
        }

        /* Stdin is not empty */
        if (buffer[255] != 0)
        {
            flush();
            buffer[255] = 0;
        }
    }
    while (strlen(buffer) == 0);

    strncpy(account, buffer, 256);
    memset(buffer, 0, 257);

    do
    {
        printf("Domain: ");
        fgets(buffer, 257, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[strcspn(buffer, "\n")] = 0;
        }

        if (buffer[255] != 0)
        {
            flush();
            buffer[255] = 0;
        }
    }
    while (strlen(buffer) == 0);

    strncpy(domain, buffer, 256);
    memset(buffer, 0, 257);

    do
    {
        printf("Version: ");
        fgets(buffer, 33, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[strcspn(buffer, "\n")] = 0;
        }

        if (buffer[31] != 0)
        {
            flush();
            buffer[31] = 0;
        }
    }
    while (strlen(buffer) == 0 ||
        strspn(buffer, "0123456789") < strlen(buffer) - 1);

    strncpy(version, buffer, 32);
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
    printf("\'%s\'\n", password);
}
