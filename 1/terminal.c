#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>

#define RESET "\033[0m"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BGRN  "\033[1m\033[32m"
#define BBLU  "\033[1m\033[34m"
#define BWHT  "\033[1m\033[37m"

int main()
{
    char cwd[PATH_MAX];
    char hostname[PATH_MAX];
    char username[PATH_MAX];
    char handle[PATH_MAX] = "";
    char dollar[1] = "$";

    /* Get current working directory */
    getcwd(cwd, sizeof(cwd));

    /* Get the hostname */
    gethostname(hostname, sizeof(hostname));

    /* Get the username */
    getlogin_r(username, sizeof(username));

    /* Create the handle */
    snprintf(handle, sizeof(handle), "%s@%s:", username, hostname);
    
    printf("\n************************** LENGENDARY TERMINAL **************************\n\n");
    while(1)
    {
        /* Print the handle */
        printf("\n%s%s", BGRN, handle);
        printf("%s%s", BBLU, cwd);
        printf("%s$ ", RESET);

        /* Take the command input from user */
        char input[PATH_MAX];
        fgets(input, PATH_MAX, stdin);

        /* Get the command from the string and separate the command and the extra function */
        char* command = strtok(input, "\n");
        command = strtok(input, " ");
        
        /* Check if it has any further instruction like -a */
        char* command_args = strtok(NULL, " ");

        /* Check which command is input */
        if(strcmp(command,"ls") == 0)
        {
            if(command_args != NULL && strcmp(command_args,"-a") == 0)
            {
                system("ls -a");
            }
            else if(command_args != NULL && strcmp(command_args,"-a") != 0)
            {
                printf("Arguement is not recognized");
            }
            else
            {
                system("ls");
            }
        }
        else
        {
            printf("Could not recognize the command %s", command);
        }
    }

    return 0;
}