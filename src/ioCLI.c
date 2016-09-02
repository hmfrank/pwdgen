#include <limits.h>
#include <stdbool.h>
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
    union { size_t index; bool reloop; long lBuff; } u;

    printf("== Deterministic Password Generator (V3) ==\n"
        "Please enter the following:\n");

    do
    {
        memset(buffer, 0, 258);
        printf("Account /.{256}/:           ");
        fgets(buffer, 258, stdin);
        u.index = strcspn(buffer, "\n");

        /* Remove trailing '\n' */
        if (u.index < strlen(buffer))
        {
            buffer[u.index] = 0;
        }

        /* Stdin is not empty */
        if (buffer[256] != 0) {
            flush();
            u.reloop = true;
        }
        else
        {
            u.reloop = false;
        }
    }
    while (strlen(buffer) == 0 || u.reloop);

    strncpy(account, buffer, 257);

    do
    {
        memset(buffer, 0, 258);
        printf("Domain  /.{256}/:           ");
        fgets(buffer, 258, stdin);
        u.index = strcspn(buffer, "\n");

        if (u.index < strlen(buffer))
        {
            buffer[u.index] = 0;
        }

        if (buffer[256] != 0)
        {
            flush();
            u.reloop = true;
        }
        else
        {
            u.reloop = false;
        }
    }
    while (strlen(buffer) == 0 || u.reloop);

    strncpy(domain, buffer, 257);

    do
    {
        memset(buffer, 0, 258);
        printf("Version /[1-9][0-9]{0,15}/: ");
        fgets(buffer, 18, stdin);
        u.index = strcspn(buffer, "\n");

        if (u.index < strlen(buffer))
        {
            buffer[u.index] = 0;
        }

        if (buffer[16] != 0)
        {
            flush();
            u.reloop = true;
        }
        else
        {
            u.reloop = false;
        }
    }
    while (strlen(buffer) == 0 || buffer[0] == '0' || u.reloop ||
        strspn(buffer, "0123456789") < strlen(buffer));

    strncpy(version, buffer, 17);

    do
    {
        memset(buffer, 0, 258);
        printf("Length  (10..256):          ");
        fgets(buffer, 5, stdin);
        u.index = strcspn(buffer, "\n");

        if (u.index < strlen(buffer))
        {
            buffer[u.index] = 0;
        }

        if (buffer[3] != 0)
        {
            flush();
            u.lBuff = 0;
        }
        else
        {
            u.lBuff = strtol(buffer, NULL, 10);
        }

    }
    while (u.lBuff < 10 || u.lBuff > 256);

    pwdLen = (unsigned) u.lBuff;
    memset(buffer, 0, 258);
    u.index = 0;
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
