#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <hashTools.h>
#include <ioFile.h>

static void tryCreateFile(char const *path);
static void bundleInputWithLen(char *output, size_t outputLen);
static void generateLineHash(char *hash, size_t hashLen);
static int checkNoDuplicate();
static void insertAccount();

/* Saves the new account to accounts.txt in case it is no duplicate */
void saveAccount()
{
    if (checkNoDuplicate())
    {
        insertAccount();
    }
}

/* Creates a file if it doesn't already exist */
static void tryCreateFile(char const *path)
{
    /* Sanity check */
    assert(path != NULL);

    FILE *file = fopen(path, "a");

    if (file == NULL)
    {
        fprintf(stderr, "Could not create file! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

/* Generates the same output as bundleInput(), but adds ':' and pwdLen at the end */
/* Shape: account.len(account)@domain.len(domain):version:pwdLen */
static void bundleInputWithLen(char *output, size_t outputLen)
{
    /* Sanity check */
    assert (output != NULL && outputLen >= MAX_INPUT_LEN + 4);

    bundleInput(output, MAX_INPUT_LEN);
    snprintf(output + strlen(output), 5, ":%u", pwdLen);
}

static void generateLineHash(char *hash, size_t hashLen)
{
    /* Sanity check */
    assert (hash != NULL && hashLen >= 65);

    union
    {
        char inChar[MAX_INPUT_LEN + 4];
        uint8_t inUint8[MAX_INPUT_LEN + 3];
    } buff;

    memset(buff.inChar, 0, (MAX_INPUT_LEN + 4) * sizeof (char));
    bundleInputWithLen(buff.inChar, MAX_INPUT_LEN + 4);
    charToUint8_t(buff.inUint8, buff.inChar, MAX_INPUT_LEN + 3);
    hashSHA256(buff.inUint8, 32, buff.inUint8, strlen(buff.inChar));
    uint8_tToHex(hash, 65, buff.inUint8, 32);
    memset(buff.inChar, 0, (MAX_INPUT_LEN + 4) * sizeof (char));
}

/* Returns 1 if the new account is not already listed in accounts.txt */
static int checkNoDuplicate()
{
    tryCreateFile("accounts.txt");
    FILE *file = fopen("accounts.txt", "r");
    char newHash[65] = {0};
    char readHash[65] = {0};

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    generateLineHash(newHash, 65);

    while ((fscanf(file, "%64s", readHash)) != EOF)
    {
        if (STRCMP(readHash, ==, newHash))
        {
            printf("\nAccount already in your list! Consider using the loading function.\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

/* Inserts the new account into accounts.txt, keeping an alphabetical order */
static void insertAccount()
{
    tryCreateFile("accountsTmp.txt");
    FILE *file = fopen("accounts.txt", "r");
    FILE *fileTmp = fopen("accountsTmp.txt", "w");
    char newLine[MAX_LINE_LEN] = {0};
    char line[MAX_LINE_LEN] = {0};
    int notInserted = 1;

    if (file == NULL || fileTmp == NULL)
    {
        fprintf(stderr, "Could not open file! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    generateLineHash(newLine, 65);
    strncat(newLine, " ", 1);
    bundleInputWithLen(newLine + 65, MAX_INPUT_LEN + 4);
    strncat(newLine, "\n", 1);

    /* Write new account list in accountsTmp.txt */
    while (fgets(line, MAX_LINE_LEN, file) != NULL)
    {
        if (notInserted && STRCMP(newLine + 65, <, line + 65))
        {
            fprintf(fileTmp, "%s", newLine);
            notInserted = 0;
        }

        fprintf(fileTmp, "%s", line);
        memset(line, 0, MAX_LINE_LEN);
    }

    if (notInserted)
    {
        fprintf(fileTmp, "%s", newLine);
    }

    fclose(file);
    fclose(fileTmp);
    file = fopen("accounts.txt", "w");
    fileTmp = fopen("accountsTmp.txt", "r");

    /* Copy new list over to accounts.txt */
    while (fgets(line, MAX_LINE_LEN, fileTmp) != NULL)
    {
        fprintf(file, "%s", line);
    }

    fclose(file);
    fclose(fileTmp);
    memset(newLine, 0, MAX_LINE_LEN);
    memset(line, 0, MAX_LINE_LEN);
    printf("\nAdded new account to your list.\n");
}
