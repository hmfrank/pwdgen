#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ioCLI.h>
#include <generator.h>
#include <globals.h>

int main()
{
    strcpy(account, "AccountTest");
    strcpy(domain, "DomainTest");
    strcpy(masterPwd, "pw");
    version = 0;

    generatePwd();

    printf("´%s\'\n", password);

    return 0;
}
