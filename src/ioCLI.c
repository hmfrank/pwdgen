#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <generator.h>
#include <globals.h>
#include <ioCLI.h>
#include <ioFile.h>

#ifndef USE_ARGV_ONLY

static void askForNewAccount();
static void askForSavedAccount();
static void askForMasterPwd();
static inline void guidedCacheGen();
static void readStr(char * const dest, size_t len, char const *text);
static void readLong(long * const dest, size_t len, long min, long max, char const *text);
static inline int invalidNatNum(char const *str);
static void flush();
static char readChNoEcho();

/**
 * Lets the user select the program mode (create new or load existing account)
 * and executes the corresponding I/O routines.
 */
void getInput()
{
    char buffer[3] = {0};
    printf("== Deterministic Password Generator (V3) ==\n"
        "Please select one of the options below:\n");
    do
    {
        printf("\nSelection   Name\n"
            "------------------------------------\n"
            "    0       Create new account\n"
            "*   1       Load existing account\n"
            "    2       Generate new cache\n\n"
            "Enter a number, or press ENTER to choose the default [*]: ");

        memset(buffer, 0, 3);
        fgets(buffer, 3, stdin);
        size_t index = strcspn(buffer, "\n");

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
            printf("\n");
            askForNewAccount();
            saveAccount();
            break;
        }
        else if (buffer[0] == '1' || strlen(buffer) == 0)
        {
            printf("\n");
            askForSavedAccount();
            break;
        }
        else if (buffer[0] == '2')
        {
            printf("\n");
            guidedCacheGen();
            continue;
        }
    }
    while (1);

    if (!tryLoadCache())
    {
        printf("\nYou need to generate a cache before the first use of a"
            " master password.\n");
        guidedCacheGen();
    }
    else
    {
        printf("\n");
        askForMasterPwd();
    }
}

/**
 * Shows the user the generated password.
 */
void showOutput()
{
    printf("\nPassword: %s\n", password);
}

/**
 * Ask the user for the parameters of a new account.
 */
static void askForNewAccount()
{
    long lBuff;

    readStr(account, 257, "Account:          ");
    readStr(domain, 257, "Domain:           ");

    do
    {
        readStr(version, 17, "Version:          ");
    }
    while (invalidNatNum(version));

    readLong(&lBuff, 4, 10, 256, "Length (10..256): ");
    pwdLen = (unsigned) lBuff;
}

/**
 * Displays a list of all locally saved accounts, lets the user select one,
 * and makes ioFile write the account parameters to the global variables.
 */
static void askForSavedAccount()
{
    /* NOTE: The output of bundleInput() could also be printed */
    /* Maybe as an option */

    long i = 0;

    printf("== Your account list ("F_LIST_PATH") ==\n"
        "Selection   Account Domain Version Length\n"
        "-------------------------------------------\n");

    while (loadAccountLine(i))
    {
        printf("%5li       %s %s %s %u\n", i++, account, domain, version, pwdLen);
    }

    printf("\n");
    readLong(&i, 6, 0, i - 1, "Enter the number of an entry to load it: ");
    loadAccountLine(i);
}

/**
 * Asks the user for their master password, displaying nothing,
 * and saves it globally in `masterPwd'.
 */
static void askForMasterPwd()
{
    char const kEnter = 10;
    char const kBackspace = 127;
    char ch;

    memset(masterPwd, 0, 257);
    printf("Master password: ");

    while ((ch = readChNoEcho()) != kEnter)
    {
        size_t len = strlen(masterPwd);

        if (ch == kBackspace)
        {
            if (len != 0)
            {
                masterPwd[len - 1] = '\0';
            }
        }
        else
        {
            if (len == 256)
            {
                fprintf(stderr, "The maximum password length has been exceeded!"
                    " Exiting...\n");
                eraseParams();
                exit(EXIT_FAILURE);
            }

            masterPwd[len] = ch;
            masterPwd[len + 1] = '\0';
        }
    }
}

/**
 * Guides the user through the cache generation.
 */
static inline void guidedCacheGen()
{
    printf("Generation will take ~1-2 min, depending on your"
        " machine's performance.\nIt is not recommended for computers with less"
        " than 3GB of RAM!\n");

    askForMasterPwd();
    printf("\nGenerating cache...\n");
    int takenSecs = generateCache();
    saveCache();
    printf("Cache was generated. (Taken time: %d min %d s)\n", takenSecs / 60,
        takenSecs % 60);
}

/**
 * Displays a prompt and reads a char string from stdin (including '\0').
 * If the string was unsatisfactory, the prompt is displayed again.
 *
 * @param kDest location where read string will be saved
 * @param len   max length of to-be-read string (including '\0')
 * @param text  will be displayed in front of input prompt
 */
static void readStr(char * const kDest, size_t len, char const *text)
{
    char buffer[len + 1];
    int reloop;

    do
    {
        size_t index;

        memset(buffer, 0, len + 1);
        reloop = 0;

        printf("%s", text);
        fgets(buffer, len + 1, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = '\0';
        }

        if (buffer[len - 1] != '\0')
        {
            flush();
            reloop = 1;
        }
    }
    while (strlen(buffer) == 0 || reloop ||
        strcspn(buffer, " ") < strlen(buffer));

    strncpy(kDest, buffer, len);
    memset(buffer, 0, len + 1);
}

/**
 * Works like readStr(), but converts the read string of char's to a long.
 * If the long is unsatisfactory, the prompt is displayed again.
 *
 * @param kDest location where read long will be saved
 * @param len   max length of to-be-read string (including '\0')
 * @param min   smallest acceptable value
 * @param max   largest acceptable value
 * @param text  will be displayed in front of input prompt
 */
static void readLong(long * const kDest, size_t len, long min, long max, char const *text)
{
    char buffer[len + 1];
    int reloop;

    do
    {
        size_t index;

        memset(buffer, 0, len + 1);
        reloop = 0;

        printf("%s", text);
        fgets(buffer, len + 1, stdin);
        index = strcspn(buffer, "\n");

        if (index < strlen(buffer))
        {
            buffer[index] = '\0';
        }

        if (buffer[len - 1] != '\0')
        {
            flush();
            reloop = 1;
        }
        else
        {
            *kDest = strtol(buffer, NULL, 10);
        }
    }
    while (strlen(buffer) == 0 || reloop ||
        invalidNatNum(buffer) || *kDest < min || *kDest > max);

    memset(buffer, 0, len + 1);
}

/**
 * Checks whether a string of char's is a valid representation of a natural
 * number or not. E.g. "01" or ".1" will be rejected.
 *
 * @param kStr location of to-be-checked string
 * @return    1 when string is accepted, 0 otherwise
 */
static inline int invalidNatNum(char const *kStr)
{
    return strspn(kStr, "0123456789") < strlen(kStr) || (kStr[0] == '0' && kStr[1] != '\0');
}

/**
 * Empties stdin up to the next newline.
 * Will require a keystroke if stdin was already empty before calling flush().
 */
static void flush()
{
    char ch;

    do
    {
        ch = getchar();
    }
    while (ch != EOF && ch != '\n');
}

/**
 * Reads one character from stdin without displaying it and returns it.
 *
 * @return read character
 */
static char readChNoEcho()
{
    char ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

#else

/**
 * Reads the parameters passed to the program via argv and stores them globally.
 */
void getInput(int argc, char *argv[])
{
    if (argc < 6)
    {
        eraseParams();
        exit(EXIT_FAILURE);
    }

    strncpy(account, argv[1], 257);
    strncpy(domain, argv[2], 257);
    strncpy(version, argv[3], 17);
    pwdLen = (unsigned) atoi(argv[4]);
    strncpy(masterPwd, argv[5], 257);

    memset(argv[1], 0, 257);
    memset(argv[2], 0, 257);
    memset(argv[3], 0, 17);
    memset(argv[4], 0, 257);
    memset(argv[5], 0, 257);

    if (!tryLoadCache())
    {
        generateCache();
        saveCache();
    }
}

/**
 * Prints the generated password on the command line.
 */
void showOutput()
{
    printf("%s\n", password);
}

#endif
