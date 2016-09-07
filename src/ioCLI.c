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
        "  Selection   Name\n"
        "------------------------------------\n"
        "* 0           Create new account\n"
        "  1           Load exisint account\n\n");

    do
    {
        memset(buffer, 0, 3 * sizeof (char));
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

        if (buffer[0] == '0' || strlen(buffer) == 0)
        {
            askForNewAccount();
            saveAccount();
            return;
        }
        else if (buffer[0] == '1')
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
        memset(buffer, 0, 258 * sizeof (char));
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
        memset(buffer, 0, 258 * sizeof (char));
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
        memset(buffer, 0, 258 * sizeof (char));
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
        memset(buffer, 0, 258 * sizeof (char));
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
            lBuff = 0;
        }
        else
        {
            lBuff = strtol(buffer, NULL, 10);
        }

    }
    while (lBuff < 10 || lBuff > 256);

    pwdLen = (unsigned) lBuff;
    memset(buffer, 0, 258 * sizeof (char));
    index = 0;
}

/* Asks the user for the number of the to-be-loaded account from the list */
static void askForSavedAccount()
{
    printf("Your account list:\n\n"
        "  Selection   Account Domain Version Length\n"
        "---------------------------------------------\n");

    loadAccountLine(1);
    printf("%s %s %s %u\n", account, domain, version, pwdLen);
}

/* Empties stdin up to the next newline */
/* Will require a keystroke if was already stdin empty */
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
