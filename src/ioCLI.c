#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <globals.h>
#include <ioCLI.h>
#include <ioFile.h>

static void askForNewAccount();
static void askForSavedAccount();
static void askForMasterPwd();
static void readStr(char * const dest, size_t len, char const *text);
static void readLong(long * const dest, size_t len, long min, long max, char const *text);
static inline int invalidNatNum(char const *str);
static void flush();
static char readChNoEcho();

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
    }
    while (1);

    printf("\n");
    askForMasterPwd();
}

/* Ask the user for the parameters of a new account */
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

/* Lets the user select a to-be-loaded account and lets ioFile load that account */
static void askForSavedAccount()
{
    /* NOTE: The output of bundleInput() could also be printed */
    /* Maybe as an option */

    long i = 0;

    printf("\n== Your account list (accounts.txt) ==\n"
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

/* Asks the user for their master password and saves it globally */
static void askForMasterPwd()
{
    char const ENTER = 10;
    char const BACKSPACE = 127;

    char ch;
    size_t len;

    printf("Master password: ");

    while ((ch = readChNoEcho()) != ENTER)
    {
        len = strlen(masterPwd);

        if (ch == BACKSPACE)
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

/* Shows the user the generated password */
void showOutput()
{
    printf("\nPassword: %s\n", password);
}

/* Displays a prompt and reads a char string from stdin of the length len (including '\0') */
/* If the string was unsatisfactory, the prompt is displayed again */
static void readStr(char * const dest, size_t len, char const *text)
{
    char buffer[len + 1];
    size_t index;
    int reloop;

    do
    {
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

    strncpy(dest, buffer, len);
    memset(buffer, 0, len + 1);
}

/* Works like readStr(), but converts the read string of char's to a long */
/* If the long is unsatisfactory, the prompt is displayed again */
static void readLong(long * const dest, size_t len, long min, long max, char const *text)
{
    char buffer[len + 1];
    size_t index;
    int reloop;

    do
    {
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
            *dest = strtol(buffer, NULL, 10);
        }
    }
    while (invalidNatNum(buffer) || reloop ||
        *dest < min || *dest > max);

    memset(buffer, 0, len + 1);
}

/* Returns whether str points to a char representation of a natural number or not */
/* E.g. "01" or ".1" will be rejected */
static inline int invalidNatNum(char const *str)
{
    return strspn(str, "0123456789") < strlen(str) || (str[0] == '0' && str[1] != '\0');
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

/* Reads one character from stdin without displaying it and returns it */
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

