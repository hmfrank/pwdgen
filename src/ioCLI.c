#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ioCLI.h>
#include <globals.h>

#define STRCMP(a, R, b) (strcmp(a, b) R 0)

/* Asks the user for the generation parameters */
void askForInput()
{
    char buffer[256];

    printf("== Deterministic Password Generator (V3) ==\n"
        "Please enter the following:\n");

    do
    {
        printf("Account: ");
        fgets(account, 256, stdin);
        account[strcspn(account, "\n")] = 0;
    }
    while (strlen(account) == 0);

    do
    {
        printf("Domain: ");
        fgets(domain, 256, stdin);
        domain[strcspn(domain, "\n")] = 0;
    }
    while (strlen(domain) == 0);

    do
    {
        printf("Version: ");
        fgets(buffer, 256, stdin);
    }
    while (STRCMP(buffer, ==, "\n"));

    version = (unsigned) atoi(buffer);
}

void showOutput()
{
    printf("\'%s\'\n", password);
}
