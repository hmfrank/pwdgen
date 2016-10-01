#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generator.h>
#include <globals.h>
#include <hashTools.h>
#include <ioFile.h>

static inline FILE *secureFopen(char const *kPath, char const *kMode);
static int checkNoDuplicate();
static void insertAccount();

/**
 * Saves the new account to F_LIST_PATH in case it is no duplicate.
 */
void saveAccount()
{
    if (checkNoDuplicate())
    {
        insertAccount();
    }
}

/**
 * A wrapper for fopen() that also checks if the returned pointer is NULL.
 * XXX: Caller must free allocated memory.
 *
 * @param kPath path to file
 * @param kMode mode for fopen (see its man page)
 * @return      pointer to FILE structure
 */
static inline FILE *secureFopen(char const *kPath, char const *kMode)
{
    FILE *file = fopen(kPath, kMode);

    if (file == NULL)
    {
        fprintf(stderr, "Could not access `%s\'! Exiting...\nTry checking your"
            " permissions. Maybe it needs to be created first.\n", kPath);
        eraseParams();
        exit(EXIT_FAILURE);
    }

    return file;
}

/**
 * Checks whether the account specified by the global input parameters is
 * already listed in F_LIST_PATH or not.
 *
 * @return 1 when no duplicate was found, 0 otherwise
 */
static int checkNoDuplicate()
{
    FILE *file = secureFopen(F_LIST_PATH, "a");
    fclose(file);
    file = secureFopen(F_LIST_PATH, "r");

    char newHash[65] = {0};
    char readHash[65] = {0};
    char line[MAX_LINE_LEN] = {0};

    snprintf(line, MAX_LINE_LEN, "%s %s %s %u", account, domain, version, pwdLen);
    hashSHA256Hex(line, strlen(line), newHash, 65);

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

/**
 * Inserts the new account into F_LIST_PATH, keeping an alphabetical order.
 */
static void insertAccount()
{
    FILE *file = secureFopen(F_LIST_PATH, "r");
    FILE *fileTmp = secureFopen(F_LIST_TMP_PATH, "w");

    char newLine[MAX_LINE_LEN] = {0};
    char readLine[MAX_LINE_LEN] = {0};
    char newHash[65] = {0};
    int notInserted = 1;

    snprintf(newLine + 65, MAX_LINE_LEN - 65, "%s %s %s %u", account, domain,
        version, pwdLen);
    hashSHA256Hex(newLine + 65, strlen(newLine + 65), newHash, 65);
    strncpy(newLine, newHash, 65);
    newLine[64] = ' ';
    strncat(newLine, "\n", 1);

    /* Write new account list in F_LIST_TMP_PATH */
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
    file = secureFopen(F_LIST_PATH, "w");
    fileTmp = secureFopen(F_LIST_TMP_PATH, "r");

    /* Copy new list over to F_LIST_PATH */
    while (fgets(readLine, MAX_LINE_LEN, fileTmp) != NULL)
    {
        fprintf(file, "%s", readLine);
    }

    fclose(file);
    fclose(fileTmp);
    memset(newLine, 0, MAX_LINE_LEN);
    memset(readLine, 0, MAX_LINE_LEN);
    printf("\nAdded new account to your list.\n");

    if (remove(F_LIST_TMP_PATH) != 0)
    {
        fprintf(stderr, "Warning: Failed to delete "F_LIST_TMP_PATH"!\n");
    }
}

/**
 * Reads a specific line of F_LIST_PATH, stores the read parameters globally,
 * and returns its success.
 *
 * @param lineNum number of to-be-read line (first line = 0)
 * @return        1 on success, 0 otherwise
 */
int loadAccountLine(long lineNum)
{
    FILE *file = secureFopen(F_LIST_PATH, "r");
    char hash[65];
    int ch;

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

/**
 * Creates or truncates F_CACHE_PATH to zero length and then writes the
 * contents of `cache' to the file.
 */
void saveCache()
{
    FILE *file = secureFopen(F_CACHE_PATH, "w");

    for (int i = 0; i < 32; i++)
    {
        fprintf(file, "%"SCNu8"\n", cache[i]);
    }

    fclose(file);
}

/**
 * Stores the contents of F_CACHE_PATH globally in `cache' if the file exists.
 *
 * @return 1 on success, 0 otherwise
 */
int tryLoadCache()
{
    FILE *file = fopen(F_CACHE_PATH, "r");

    if (file == NULL)
    {
        return 0;
    }

    for (int i = 0; i < 32; i++)
    {
        fscanf(file, "%"SCNu8, cache + i);
    }

    fclose(file);
    return 1;
}
