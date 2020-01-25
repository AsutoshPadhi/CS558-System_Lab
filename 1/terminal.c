#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define RESET "\033[0m"
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define BGRN "\033[1m\033[32m"
#define BBLU "\033[1m\033[34m"
#define BWHT "\033[1m\033[37m"


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
    while (1)
    {
        /* Print the handle */
        printf("\n%s%s", BGRN, handle);
        printf("%s%s", BBLU, cwd);
        printf("%s$ ", RESET);

        /* Take the command input from user */
        char input[PATH_MAX];
        fgets(input, PATH_MAX, stdin);

        /* Get the command from the string and separate the command and the extra function */
        char *command = strtok(input, "\n");
        command = strtok(input, " ");

        /* Check if it has any further instruction like -a */
        char *command_args = strtok(NULL, " ");

        /* Check which command is input */
        if (strcmp(command, "ls") == 0)
        {
            if (command_args != NULL && strcmp(command_args, "-a") == 0)
            {
                system("ls -a");
            }
            else if (command_args != NULL && strcmp(command_args, "-a") != 0)
            {
                printf("Arguement is not recognized");
            }
            else
            {
                system("ls");
            }
        }
        else if (strcmp(command, "cd") == 0)
        {
            /* Check if the directory in the input is present */
            DIR* dir = opendir(command_args);
            if(dir)
            {
                closedir(dir);

                /* Add a / before appending the new path to the old path */
                if(command_args[0] != '/')
                {
                    strcat(cwd, "/");
                }
                strcat(cwd, command_args);

                /* System call to change directory */
                chdir(cwd);
            }
            else
            {
                printf("No such directory exists");
            }
            
            /* Update the handle if any changes are made */
            getcwd(cwd, sizeof(cwd));
        }
        else if(strcmp(command, "mkdir") == 0)
        {
            if(command_args != NULL && strcmp(command_args, "-m") != 0)
            {
                /* Create a directory; Implement mkdir <filename> */
                if (mkdir(command_args, 0777) == -1)
                {
                    printf("Error in creating directory");
                }
            }
            else if(command_args != NULL && strcmp(command_args, "-m") == 0)
            {
                /* Create a directory with -m option */

                /* To store the mode; Eg : 0777, 0733 */
                char *command_args_1 = strtok(NULL, " ");
                
                /* To store the filename */
                char *command_args_2 = strtok(NULL, " ");
                
                /* Append and create the final command */
                char md_command[PATH_MAX] = "";
                snprintf(md_command, sizeof(md_command), "%s %s %s %s", command, command_args, command_args_1, command_args_2);
                
                if(command_args_1 && command_args_2)
                {
                    /* System call */
                    system(md_command);
                }
                else
                {
                    printf("Arguements not found\n");
                }
            }
            else
            {
                printf("Arguement not found\n");
            }
        }
        /*else if (strcmp(command, "cp") == 0)
        {
            if (command_args != NULL && strcmp(command_args, "-u") == 0)
            {
                FILE *fptr1, *fptr2;
                char filename[100], c;

                fptr1 = fopen(command_args1, "r");
                if (fptr1 == NULL)
                {
                    printf("Cannot open file %s \n", filename);
                    exit(0);
                }

                // Open another file for writing
                fptr2 = fopen(command_args2, "w");
                if (fptr2 == NULL)
                {
                    printf("Cannot open file %s \n", filename);
                    exit(0);
                }
                // Read contents from file
                c = fgetc(fptr1);
                while (c != EOF)
                {
                    fputc(c, fptr2);
                    c = fgetc(fptr1);
                }
                fclose(fptr1);
                fclose(fptr2);
            }
            else
            {

                FILE *fptr1, *fptr2;
                char filename[100], c;

                fptr1 = fopen(command_args, "r");
                if (fptr1 == NULL)
                {
                    printf("Cannot open file %s \n", filename);
                    exit(0);
                }

                // Open another file for writing
                fptr2 = fopen(command_args1, "w");
                if (fptr2 == NULL)
                {
                    printf("Cannot open file %s \n", filename);
                    exit(0);
                }
                // Read contents from file
                c = fgetc(fptr1);
                while (c != EOF)
                {
                    fputc(c, fptr2);
                    c = fgetc(fptr1);
                }
                fclose(fptr1);
                fclose(fptr2);
            }
        }
        else if (strcmp(command, "sort") == 0)
        {
             
            
            char strTempData[1000];
            char **strData = NULL; // String List
            int i, j;
            int noOfLines = 0;

            FILE *ptrFileLog = NULL;
            FILE *ptrSummary = NULL;
                if ( strcmp(command_args, "-r") != 0){
            ptrFileLog = fopen(command_args, "r");
                }
                else if( strcmp(command_args, "-r") == 0)
                {
                     ptrFileLog = fopen(command_args1, "r");
                }
            ptrSummary = fopen("out.txt", "a");
            if (ptrSummary  == NULL)
            {
               printf("not created");
                return 1;
            }

            // Read and store in a string list.
            while (fgets(strTempData, 1000, ptrFileLog) != NULL)
            {
                // Remove the trailing newline character
                if (strchr(strTempData, '\n'))
                    strTempData[strlen(strTempData) - 1] = '\0';
                strData = (char **)realloc(strData, sizeof(char **) * (noOfLines + 1));
                strData[noOfLines] = (char *)calloc(1000, sizeof(char));
                strcpy(strData[noOfLines], strTempData);
                noOfLines++;
            }
            // Sort the array.
            for (i = 0; i < (noOfLines - 1); ++i)
            {
                for (j = 0; j < (noOfLines - i - 1); ++j)
                {
                    if (strcmp(strData[j], strData[j + 1]) > 0)
                    {
                        strcpy(strTempData, strData[j]);
                        strcpy(strData[j], strData[j + 1]);
                        strcpy(strData[j + 1], strTempData);
                    }
                }
            }
         if ( strcmp(command_args, "-r") != 0)   // Write it to outfile. file.
          {for (i = 0; i < noOfLines; i++)
            {
               // fprintf(ptrSummary, "%s\n", strData[i]);
                printf("%s\n",strData[i]);
                }
                }
                else if(strcmp(command_args, "-r") == 0)
             {    for (i =noOfLines-1; i>0; i--)
            {
               // fprintf(ptrSummary, "%s\n", strData[i]);
                printf("%s\n",strData[i]);
                }
                }
            // free each string
            for (i = 0; i < noOfLines; i++)
                free(strData[i]);
            // free string list.
            remove("out.txt");
            free(strData);
            fclose(ptrFileLog);
            fclose(ptrSummary);
        }
        else if (strcmp(command, "cat") == 0)
        {
           //static int i=1;
             if (strcmp(command_args, "-n") != 0)
            { int fdold,count;
             char buffer[2000];
            fdold=open(command_args, O_RDONLY);
           while((count=read(fdold,buffer,sizeof(buffer)))>0)
            //displaying the content
              {
                 printf("%s",buffer);
             }
             if(command_args1!=NULL){
             fdold=open(command_args1, O_RDONLY);
           while((count=read(fdold,buffer,sizeof(buffer)))>0)
            //displaying the content
              {
                 printf("%s",buffer);
             }
             }
            }
            else if (strcmp(command_args, "-n") == 0)
             {
                 int fdold,count;
             char buffer[2000];
            fdold=open(command_args1, O_RDONLY);
           while((count=read(fdold,buffer,sizeof(buffer)))>0)
            //displaying the content
              {
                 printf("%s",buffer);

             }

             }
        }*/
    }
    return 0;
}