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
#include<readline/readline.h>
#include<readline/history.h>

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
    snprintf(handle, sizeof(handle), "\n%s%s@%s:%s%s%s$ ", BGRN, username, hostname, BBLU, cwd, RESET);
    
    system("clear");

    printf("\n************************** LENGENDARY TERMINAL **************************\n\n");
    while (1)
    {
        /* Use readline to take input from user */
        char *input = "";
        input = readline(handle);

        /* If nothing has been input */
        if(strlen(input) == 0)
        {
            printf("Enter a command\n");
            continue;
        }

        /* If the user chooses to exit */
        if(strcmp(input,"exit") == 0)
        {
            printf("Bye !!!\n\n\n\n");
            break;
        }

        /* Add history */
        add_history(input);

        /* main_cmd contains the first word of the command */
        char main_cmd[PATH_MAX] = "";
        strcpy(main_cmd, input);

        /* Get the command from the string and separate the command and the extra function */
        char *command = strtok(main_cmd, "\n");
        command = strtok(main_cmd, " ");

        /* Check if it has any further instruction like -a */
        char *command_args = strtok(NULL, " ");

        /* Check which command is input */
        if (strcmp(command, "ls") == 0)
        {
            /**************************************** ls ****************************************/
            if (command_args != NULL && strcmp(command_args, "-a") == 0)
            {
                /* Structure containing all the information about the files */
                struct dirent *dir_struct;

                /* Open the current directory */
                DIR *d = opendir(".");

                if(d)
                {
                    while((dir_struct = readdir(d)) != NULL)
                    {
                        printf("%s\n",dir_struct->d_name);
                    }   
                }
                else
                {
                    printf("Error is opening the current directory\n");
                }
            }
            else if (command_args != NULL && strcmp(command_args, "-a") != 0)
            {
                printf("Arguement is not recognized");
            }
            else
            {
                /* Structure containing all the information about the files */
                struct dirent *dir_struct;

                /* Open the current directory */
                DIR *d = opendir(".");
                
                if(d)
                {
                    while((dir_struct = readdir(d)) != NULL)
                    {
                        if(dir_struct->d_name[0] != '.')
                        {
                            printf("%s\n",dir_struct->d_name);
                        }
                    }   
                }
                else
                {
                    printf("Error is opening the current directory\n");
                }
            }
        }
        else if (strcmp(command, "cd") == 0)
        {
            /**************************************** cd ****************************************/
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
            handle[0] = '\0';
            snprintf(handle, sizeof(handle), "\n%s%s@%s:%s%s%s$ ", BGRN, username, hostname, BBLU, cwd, RESET);
        }
        else if (strcmp(command, "cat") == 0)
        {
            /**************************************** cat ****************************************/
            if(strstr(input, ">") != NULL)
            {
                /* Get the last word of the string, i.e. the destination file */
                char *dest_file = strrchr(input, ' ');
                dest_file++;
                
                /* Remove \n from the end of the string due to fgets */
                char *pos;
                if ((pos=strchr(dest_file, '\n')) != NULL)
                    *pos = '\0';
                
                /* Open the destination files */
                FILE *fd;
                fd = fopen(dest_file, "w");

                /* Write contents into the destination file */
                if(dest_file && *(dest_file + 1))
                {
                    char *file_name = command_args;
                    while(file_name != NULL)
                    {
                        /* If '>' is encountered, ditch the entire process because writing was already done */
                        if(strcmp(file_name, ">") == 0)
                        {
                            break;
                        }

                        /* Check if the file is present */
                        if(access(file_name,R_OK) != -1)
                        {
                            /* Read file */
                            FILE *fp = NULL;
                            char *line = NULL;
                            size_t len = 0;
                            ssize_t read;

                            fp = fopen(file_name, "r");
                            while((read = getline(&line, &len, fp)) != -1)
                            {
                                /* Write to file */
                                fprintf(fd, "%s", line);
                            }
                        }
                        else
                        {
                            printf("The file %s was not found",file_name);
                        }
                        file_name = strtok(NULL, " ");
                    }
                    fclose(fd);
                }
                else
                {
                    printf("Error: Enter the destination file\n");
                }
            }
            else if(command_args != NULL && strcmp(command_args, "-n") == 0)
            {
                int line_number = 0;
                char *file_name = strtok(NULL, " ");
                while(file_name != NULL)
                {
                    /* Check if the file is present */
                    if(access(file_name,R_OK) != -1)
                    {
                        /* Read file */
                        FILE *fp = NULL;
                        char *line = NULL;
                        size_t len = 0;
                        ssize_t read;

                        fp = fopen(file_name, "r");
                        while((read = getline(&line, &len, fp)) != -1)
                        {
                            /* Increment the line number */
                            line_number++;

                            /* Print the lines of file */
                            printf("\t%d:  %s",line_number, line);
                        }
                    }
                    else
                    {
                        printf("The file %s was not found",file_name);
                    }
                    file_name = strtok(NULL, " ");
                }
            }
            else if(command_args != NULL && strcmp(command_args, "-n") != 0)
            {
                char *file_name = command_args;
                while(file_name != NULL)
                {
                    /* Check if the file is present */
                    if(access(file_name,R_OK) != -1)
                    {
                        /* Read file */
                        FILE *fp = NULL;
                        char *line = NULL;
                        size_t len = 0;
                        ssize_t read;

                        fp = fopen(file_name, "r");
                        while((read = getline(&line, &len, fp)) != -1)
                        {
                            /* Print the lines of file */
                            printf("%s", line);
                        }
                    }
                    else
                    {
                        printf("The file %s was not found",file_name);
                    }
                    file_name = strtok(NULL, " ");
                }
            }
            else
            {
                printf("Arguement Error : Enter a file name\n");
            }
        }
        else if(strcmp(command, "mkdir") == 0)
        {
            /**************************************** mkdir ****************************************/
            if(command_args != NULL && strcmp(command_args, "-m") != 0)
            {
                struct stat st = {0};

                if(stat(command_args, &st) == -1)
                {
                    /* Create a directory; Implement mkdir <filename> */
                    if (mkdir(command_args, 0777) == -1)
                    {
                        printf("Error in creating directory\n");
                    }
                }
                else
                {
                    printf("Directory with the same name exists\n");
                }
            }
            else if(command_args != NULL && strcmp(command_args, "-m") == 0)
            {
                /* Create a directory with -m option */

                /* To store the mode; Eg : 0777, 0733 */
                char *mode_str = strtok(NULL, " ");
                
                /* To store the filename */
                char *file_name = strtok(NULL, " ");

                /* Append and create the final command */
                char md_command[PATH_MAX] = "";
                snprintf(md_command, sizeof(md_command), "%s %s %s %s", command, command_args, mode_str, file_name);
                
                if(mode_str && file_name)
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
        else if(strcmp(command, "grep") == 0)
        {
            /**************************************** grep ****************************************/
            if(command_args != NULL && strcmp(command_args, "-n") == 0)
            {
                char *pattern_raw = strtok(NULL, " ");
                char pattern[(int)strlen(pattern_raw)];
                int j = 0;
                for(int i=1; i<(int)strlen(pattern_raw)-1; i++)
                {
                    pattern[j++] = pattern_raw[i];
                }
                pattern[j] = '\0';

                /* Take and check if filenames are valid */
                int file_cnt = 0;
                char *file_name = strtok(NULL, " ");
                while(file_name != NULL)
                {
                    /* Check if the file is present */
                    if(access(file_name,R_OK) != -1)
                    {
                        /* Read file */
                        FILE *fp = NULL;
                        char *line = NULL;
                        size_t len = 0;
                        ssize_t read;

                        fp = fopen(file_name, "r");
                        int line_cnt = 0;
                        while((read = getline(&line, &len, fp)) != -1)
                        {
                            line_cnt++;
                            /* Print the line if it contains the pattern */
                            if(strstr(line, pattern) != NULL)
                            {
                                printf("%s : %d: %s", file_name, line_cnt, line);
                            }
                        }
                    }
                    else
                    {
                        printf("The file %s was not found",file_name);
                    }
                    file_name = strtok(NULL, " ");
                    file_cnt++;
                }
                if(file_name == NULL && file_cnt == 0)
                {
                    printf("Enter the file name\n");
                }
            }
            else if(command_args != NULL && strcmp(command_args, "-n") != 0)
            {
                /* Extract pattern without quotes */
                char pattern[(int)strlen(command_args)];
                int j = 0;
                for(int i=1; i<(int)strlen(command_args)-1; i++)
                {
                    pattern[j++] = command_args[i];
                }
                pattern[j] = '\0';

                /* Take and check if filenames are valid */
                int file_cnt = 0;
                char *file_name = strtok(NULL, " ");
                while(file_name != NULL)
                {
                    /* Check if the file is present */
                    if(access(file_name,R_OK) != -1)
                    {
                        /* Read file */
                        FILE *fp = NULL;
                        char *line = NULL;
                        size_t len = 0;
                        ssize_t read;

                        fp = fopen(file_name, "r");
                        while((read = getline(&line, &len, fp)) != -1)
                        {
                            /* Print the line if it contains the pattern */
                            if(strstr(line, pattern) != NULL)
                            {
                                printf("%s : %s", file_name, line);
                            }
                        }
                    }
                    else
                    {
                        printf("The file %s was not found",file_name);
                    }
                    file_name = strtok(NULL, " ");
                    file_cnt++;
                }
                if(file_name == NULL && file_cnt == 0)
                {
                    printf("Enter the file name\n");
                }
            }
            else
            {
                printf("Arguement not found\n");
            }
        }
        else if (strcmp(command, "cp") == 0)
        {
            char *command_args1 = strtok(NULL, " ");
            char *command_args2 = strtok(NULL, " ");
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
             
            char *command_args1 = strtok(NULL, " ");
            char *command_args2 = strtok(NULL, " ");
            
            char strTempData[1000];
            char **strData = NULL; // String List
            int i, j;
            int noOfLines = 0;

            FILE *ptrFileLog = NULL;
            FILE *ptrSummary = NULL;
            if ( strcmp(command_args, "-r") != 0)
            {
                ptrFileLog = fopen(command_args, "r");
            }
            else if( strcmp(command_args, "-r") == 0)
            {
                ptrFileLog = fopen(command_args1, "r");
            }
            ptrSummary = fopen("out.txt", "a");
            if(ptrSummary  == NULL)
            {
                printf("not created");
                return 1;
            }

            // Read and store in a string list.
            while(fgets(strTempData, 1000, ptrFileLog) != NULL)
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
            for(i = 0; i < (noOfLines - 1); ++i)
            {
                for(j = 0; j < (noOfLines - i - 1); ++j)
                {
                    if(strcmp(strData[j], strData[j + 1]) > 0)
                    {
                        strcpy(strTempData, strData[j]);
                        strcpy(strData[j], strData[j + 1]);
                        strcpy(strData[j + 1], strTempData);
                    }
                }
            }
            if(strcmp(command_args, "-r") != 0)   // Write it to outfile. file.
            {
                for(i = 0; i < noOfLines; i++)
                {
                    // fprintf(ptrSummary, "%s\n", strData[i]);
                    printf("%s\n",strData[i]);
                }
            }
            else if(strcmp(command_args, "-r") == 0)
            {
                for(i =noOfLines-1; i>0; i--)
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
        else
        {
            printf("This command is not recognized \n");
        }
        
    }
    return 0;
}