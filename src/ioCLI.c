#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <ioCLI.h>
#include <ioFile.h>

static void askForNewAccount();
static void askForSavedAccount();
static void flush();

/* Asks the user for the mode of the program and executes corresponding I/O routines */
void askForInput()
{
    char buffer[3] = {0};
    size_t index;

    printf("== Deterministic Password Generator (V3) ==\n"
        "Do you wish to add a new account to your list or load an existing one?\n\n"
        "Selection   Name\n"
        "------------------------------------\n"
        "    0       Create new account\n"
        "*   1       Load existing account\n\n");

    do
    {
        memset(buffer, 0, 3);
        printf("Enter a number, or press ENTER to choose the default [*]: ");
        fgets(buffer, 3, stdin);
        index = strcspn(buffer, "\n");

        /* Remove trailing '\n' */
        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        /* Stdin is not empty */
        if (buffer[1] != '\0')
        {
            flush();
            continue;
        }

        if (buffer[0] == '0')
        {
            askForNewAccount();
            saveAccount();
            return;
        }
        else if (buffer[0] == '1' || strlen(buffer) == 0)
        {
            askForSavedAccount();
            return;
        }
    }
    while (1);
}

/* Ask the user for the parameters of a new account */
static void askForNewAccount()
{
    char buffer[258] = {0};
    size_t index;
    long lBuff;

    do
    {
        memset(buffer, 0, 258);
        printf("Account:           ");
        fgets(buffer, 258, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[256] != '\0')
        {
            flush();
            continue;
        }
    }
    while (strlen(buffer) == 0);

    strncpy(account, buffer, 257);

    do
    {
        memset(buffer, 0, 258);
        printf("Domain:            ");
        fgets(buffer, 258, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[256] != '\0')
        {
            flush();
            continue;
        }
    }
    while (strlen(buffer) == 0);

    strncpy(domain, buffer, 257);

    do
    {
        memset(buffer, 0, 258);
        printf("Version:           ");
        fgets(buffer, 18, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[16] != '\0')
        {
            flush();
            continue;
        }
    }
    while (strlen(buffer) == 0 || buffer[0] == '0' ||
        strspn(buffer, "0123456789") < strlen(buffer));

    strncpy(version, buffer, 17);

    do
    {
        memset(buffer, 0, 258);
        printf("Length  (10..256): ");
        fgets(buffer, 5, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[3] != '\0')
        {
            flush();
            continue;
        }
        else
        {
            lBuff = strtol(buffer, NULL, 10);
        }

    }
    while (lBuff < 10 || lBuff > 256);

    pwdLen = (unsigned) lBuff;
    memset(buffer, 0, 258);
    index = 0;
}

/* Lets the user select a to-be-loaded account and lets ioFile load that account */
static void askForSavedAccount()
{
    /* NOTE: The output of bundleInput() could also be printed */
    /* Maybe as an option */

    char buffer[7] = {0};
    unsigned i = 0;
    size_t index;
    long lBuff;

    printf("\n== Your account list (accounts.txt) ==\n"
        "Selection   Account Domain Version Length\n"
        "-------------------------------------------\n");

    while (loadAccountLine(i))
    {
        printf("%5u       %s %s %s %u\n", i++, account, domain, version, pwdLen);
    }

    printf("\n");

    do
    {
        memset(buffer, 0, 7);
        printf("Enter the number of an entry to load it: ");
        fgets(buffer, 7, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = 0;
        }

        if (buffer[5] != '\0')
        {
            flush();
            continue;
        }
        else
        {
            lBuff = strtol(buffer, NULL, 10);
        }
    }
    while (lBuff < 0 || lBuff > (long) i - 1 || lBuff > UINT_MAX);

    loadAccountLine((unsigned) lBuff);
}

/* Empties stdin up to the next newline */
/* Will require a keystroke if stdin was already empty */
static void flush()
{
    char ch;

    do
    {
        ch = getchar();
    }
    while (ch != EOF && ch != '\n');
}

/* Shows the user the generated password */
void showOutput()
{
    printf("\nPassword: `%s\'\n", password);
}
