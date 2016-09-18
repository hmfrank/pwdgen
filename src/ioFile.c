#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <hashTools.h>
#include <ioFile.h>

static void tryCreateFile(char const *path);
static int checkNoDuplicate();
static void insertAccount();

/* Saves the new account to F_FILE_PATH in case it is no duplicate */
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
        fprintf(stderr, "Could not create file! Exiting...\n"
            "Try checking the file permissions.");
        eraseParams();
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

/* Returns `1' if the new account is not already listed in F_FILE_PATH */
/* A `0' is returned in the opposite case */
static int checkNoDuplicate()
{
    tryCreateFile(F_FILE_PATH);
    FILE *file = fopen(F_FILE_PATH, "r");
    char newHash[65] = {0};
    char readHash[65] = {0};
    char line[MAX_LINE_LEN] = {0};

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file! Exiting...\n");
        eraseParams();
        exit(EXIT_FAILURE);
    }

    snprintf(line, MAX_LINE_LEN, "%s %s %s %u", account, domain, version, pwdLen);
    hashSHA256Hex(newHash, 65, line, strlen(line));

    while ((fscanf(file, "%64s", readHash)) != EOF)
    {
        if (STRNCMP(readHash, ==, newHash, 64))
        {
            printf("\nAccount already in your list! Use the loading function"
                " to save time.\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

/* Inserts the new account into F_FILE_PATH, keeping an alphabetical order */
static void insertAccount()
{
    tryCreateFile(F_FILE_PATH_TMP);
    FILE *file = fopen(F_FILE_PATH, "r");
    FILE *fileTmp = fopen(F_FILE_PATH_TMP, "w");
    char newLine[MAX_LINE_LEN] = {0};
    char readLine[MAX_LINE_LEN] = {0};
    char newHash[65] = {0};
    int notInserted = 1;

    if (file == NULL || fileTmp == NULL)
    {
        fprintf(stderr, "Could not open file! Exiting...\n");
        eraseParams();
        exit(EXIT_FAILURE);
    }

    snprintf(newLine + 65, MAX_LINE_LEN - 65, "%s %s %s %u", account, domain,
        version, pwdLen);
    hashSHA256Hex(newHash, 65, newLine + 65, strlen(newLine + 65));
    strncpy(newLine, newHash, 65);
    newLine[64] = ' ';
    strncat(newLine, "\n", 1);

    /* Write new account list in F_FILE_PATH_TMP */
    while (fgets(readLine, MAX_LINE_LEN, file) != NULL)
    {
        if (notInserted && STRNCMP(newLine + 65, <, readLine + 65, MAX_LINE_LEN - 65))
        {
            fprintf(fileTmp, "%s", newLine);
            notInserted = 0;
        }

        fprintf(fileTmp, "%s", readLine);
        memset(readLine, 0, MAX_LINE_LEN);
    }

    if (notInserted)
    {
        fprintf(fileTmp, "%s", newLine);
    }

    fclose(file);
    fclose(fileTmp);
    file = fopen(F_FILE_PATH, "w");
    fileTmp = fopen(F_FILE_PATH_TMP, "r");

    /* Copy new list over to F_FILE_PATH */
    while (fgets(readLine, MAX_LINE_LEN, fileTmp) != NULL)
    {
        fprintf(file, "%s", readLine);
    }

    fclose(file);
    fclose(fileTmp);
    memset(newLine, 0, MAX_LINE_LEN);
    memset(readLine, 0, MAX_LINE_LEN);
    printf("\nAdded new account to your list.\n");
}

/* Reads a specific line (starting at 0) of F_FILE_PATH, stores the read parameters */
/* globally, and returns `1' on success and `0' when the line couldn't be reached */
int loadAccountLine(long lineNum)
{
    FILE *file = fopen(F_FILE_PATH, "r");
    char hash[65];
    int ch;

    if (file == NULL)
    {
        fprintf(stderr, "Could not open F_FILE_PATH! Exiting...\n"
            "Try adding a new account to create the file or check the file"
            " permissions.\n");
        eraseParams();
        exit(EXIT_FAILURE);
    }

    /* Move to the beginning of the desired line */
    for (long i = 0; i < lineNum; i++)
    {
        do
        {
            ch = fgetc(file);
        }
        while (ch != '\n');

        ch = fgetc(file);
    }

    if (fscanf(file, "%64s %256s %256s %16s %3u", hash, account,
        domain, version, &pwdLen) == EOF)
    {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}
