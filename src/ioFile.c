#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <hashTools.h>
#include <ioFile.h>

char newHash[65] = {0};

static void createFile();
static int checkNoDuplicate();
static void appendAccount();

/* Saves the new account to accounts.txt in case it is no duplicate */
void saveAccount()
{
    if (checkNoDuplicate())
    {
        appendAccount();
    }
}

/* Creates the file accounts.txt if it doesn't already exist */
static void createFile()
{
    FILE *file = fopen("accounts.txt", "a");

    if (file == NULL)
    {
        fprintf(stderr, "Could not create file! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

/* Returns 1 if the new account is not already listed in accounts.txt */
static int checkNoDuplicate()
{
    char readHash[65] = {0};
    char readAcc[257] = {0};
    char readDmn[257] = {0};
    char readVer[17] = {0};
    unsigned readPwdLen = 0;

    union
    {
        char inChar[MAX_INPUT_LEN];
        uint8_t inUint8[MAX_INPUT_LEN - 1];
    } buff;

    memset(buff.inChar, 0, MAX_INPUT_LEN * sizeof (char));
    createFile();
    FILE *file = fopen("accounts.txt", "r");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    bundleInput(buff.inChar, MAX_INPUT_LEN);
    charToUint8_t(buff.inUint8, buff.inChar, MAX_INPUT_LEN - 1);
    hashSHA256(buff.inUint8, 32, buff.inUint8, strlen(buff.inChar));
    uint8_tToHex(newHash, 65, buff.inUint8, 32);

    while ((fscanf(file, "%s %s %s %s %u", readHash, readAcc, readDmn,
        readVer, &readPwdLen)) != EOF)
    {
        if (STRCMP(readHash, ==, newHash))
        {
            printf("\nAccount already in your list! Consider using the loading function.\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    memset(buff.inChar, 0, MAX_INPUT_LEN * sizeof (char));
    return 1;
}

/* Appends the new account to the list of accounts.txt */
static void appendAccount()
{
    FILE *file = fopen("accounts.txt", "a");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s %s %s %u\n", newHash, account, domain, version, pwdLen);
    printf("\nAdded new account to your list.\n");
    fclose(file);
    memset(newHash, 0, 65);
}
