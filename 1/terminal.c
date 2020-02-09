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
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

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

/**
 * @brief This function returns the mode in mode_t form
 * 
 * @param str 
 * @return mode_t 
 */
mode_t str2modet(char *str)
{
    char proper[3];
    int l = strlen(str);
    if(l <= 3)
    {
        int i = 0;
        int more = 3 - l;
        while(more--)
        {
            proper[i++] = '0';
        }
        for(int j=0; j<l; j++)
        {
            proper[i++] = str[j];
        }
    }
    else
    {
        str++;
        strcpy(proper, str);
    }

    /* Array of bits */
    char bits[8][3] = {"000","001","010","011","100","101","110","111"};
    char bit_str[PATH_MAX] = "";
    for(int i=0; i<3; i++)
    {
        if(bits[proper[i]-'0'][0] == '1')
            strncat(bit_str, "r", 1);
        else
            strncat(bit_str, "-", 1);
        if(bits[proper[i]-'0'][1] == '1')
            strncat(bit_str, "w", 1);
        else
            strncat(bit_str, "-", 1);
        if(bits[proper[i]-'0'][2] == '1')
            strncat(bit_str, "x", 1);
        else
            strncat(bit_str, "-", 1);
    }
    

    mode_t mode = 0;
    if (bit_str[0] == 'r')
        mode |= 0400;
    if (bit_str[1] == 'w')
        mode |= 0200;
    if (bit_str[2] == 'x')
        mode |= 0100;
    if (bit_str[3] == 'r')
        mode |= 0040;
    if (bit_str[4] == 'w')
        mode |= 0020;
    if (bit_str[5] == 'x')
        mode |= 0010;
    if (bit_str[6] == 'r')
        mode |= 0004;
    if (bit_str[7] == 'w')
        mode |= 0002;
    if (bit_str[8] == 'x')
        mode |= 0001;

    // printf("\n%o", mode);
    return mode;
}

/**
 * @brief To be used for qsort
 * 
 * @param A 
 * @param B 
 * @return int 
 */ 
int compareName(const void *const A, const void *const B)
{
    return strcmp((*(struct dirent **) A)->d_name, (*(struct dirent **) B)->d_name);
}

/**
 * @brief Main Function
 * 
 * @return int 
 */
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

        if(input[strlen(input)-1] == ' ')
        {
            input[strlen(input)-1] = '\0';
        }

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

        /* Add history to readline */
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

/* ******************************************************* LS ******************************************************* */        
        if (strcmp(command, "ls") == 0)
        {            
            if (command_args != NULL && strcmp(command_args, "-a") == 0)
            {
                /* Check if ls of directory is required */
                char *dir_name = strtok(NULL, " ");

                /* If ls of current directory is required */
                if(dir_name == NULL)
                {
                    /* Structure containing all the information about the files */
                    struct dirent *dir_struct;
                    struct dirent **dir_list;

                    /* Open the current directory */
                    DIR *d = opendir(".");

                    /* Check if directory can be opened */
                    if(!d)
                    {
                        printf("Error is opening the current directory\n");
                        closedir(d);
                        continue;
                    }

                    /* Count the number of files */
                    int file_cnt = 0;
                    while((dir_struct = readdir(d)) != NULL)
                    {
                        // printf("%s\n",dir_struct->d_name);
                        file_cnt++;
                    }

                    /* Allocate space for list of directories */
                    dir_list = malloc(file_cnt * sizeof(dir_list));
                    if(!dir_list)
                    {
                        printf("Error: Memory exhausted\n");
                        closedir(d);
                        continue;
                    }

                    /* Reset the pointer of directories */
                    rewinddir(d);

                    /* Store the file names in dir_list */
                    int i = 0;
                    while((dir_struct = readdir(d)) != NULL)
                    {
                        dir_list[i++] = dir_struct;
                    }

                    /* Sort the file names */
                    qsort(dir_list, file_cnt, sizeof(*dir_list), compareName);
                    
                    /* Display */
                    for(int i=0; i<file_cnt; i++)
                    {
                        printf("%d)\t%s\n", i+1, dir_list[i]->d_name);
                    }

                    closedir(d);
                }
                /* If ls of specific directory is required */
                else
                {
                    while(dir_name != NULL)
                    {
                        /* Structure containing all the information about the files */
                        struct dirent *dir_struct;
                        struct dirent **dir_list;

                        /* Open the current directory */
                        DIR *d = opendir(dir_name);

                        /* Check if directory can be opened */
                        if(!d)
                        {
                            printf("Error: Can not open the directory\n");
                            closedir(d);
                            break;
                        }

                        /* Count the number of files */
                        int file_cnt = 0;
                        while((dir_struct = readdir(d)) != NULL)
                        {
                            file_cnt++;
                        }

                        /* Allocate space for list of directories */
                        dir_list = malloc(file_cnt * sizeof(dir_list));
                        if(!dir_list)
                        {
                            printf("Error: Memory exhausted\n");
                            closedir(d);
                            break;
                        }

                        /* Reset the pointer of directories */
                        rewinddir(d);

                        /* Store the file names in dir_list */
                        int i = 0;
                        while((dir_struct = readdir(d)) != NULL)
                        {
                            dir_list[i++] = dir_struct;
                        }

                        /* Sort the file names */
                        qsort(dir_list, file_cnt, sizeof(*dir_list), compareName);
                        
                        /* Display */
                        printf("\n%s : \n",dir_name);
                        for(int i=0; i<file_cnt; i++)
                        {
                            printf("%d)\t%s\n", i+1, dir_list[i]->d_name);
                        }

                        closedir(d);
                        dir_list = NULL;
                        dir_struct = NULL;

                        dir_name = strtok(NULL, " ");
                    }
                }
            }
            else if (command_args != NULL && strcmp(command_args, "-a") != 0)
            {
                if(!opendir(command_args))
                {
                    printf("Arguement is not recognized");
                }
                else
                {
                    char *dir_name = command_args;

                    while(dir_name != NULL)
                    {
                        /* Structure containing all the information about the files */
                        struct dirent *dir_struct;
                        struct dirent **dir_list;

                        /* Open the current directory */
                        DIR *d = opendir(dir_name);

                        /* Check if directory can be opened */
                        if(!d)
                        {
                            printf("Error: Can not open the directory\n");
                            closedir(d);
                            break;
                        }

                        /* Count the number of files */
                        int file_cnt = 0;
                        while((dir_struct = readdir(d)) != NULL)
                        {
                            if(dir_struct->d_name[0] != '.')
                            {
                                file_cnt++;
                            }
                        }

                        /* Allocate space for list of directories */
                        dir_list = malloc(file_cnt * sizeof(dir_list));
                        if(!dir_list)
                        {
                            printf("Error: Memory exhausted\n");
                            closedir(d);
                            break;
                        }

                        /* Reset the pointer of directories */
                        rewinddir(d);

                        /* Store the file names in dir_list */
                        int i = 0;
                        while((dir_struct = readdir(d)) != NULL)
                        {
                            if(dir_struct->d_name[0] != '.')
                            {
                                dir_list[i++] = dir_struct;
                            }
                        }

                        /* Sort the file names */
                        qsort(dir_list, file_cnt, sizeof(*dir_list), compareName);
                        
                        /* Display */
                        printf("\n%s : \n",dir_name);
                        for(int i=0; i<file_cnt; i++)
                        {
                            printf("%d)\t%s\n", i+1, dir_list[i]->d_name);
                        }

                        closedir(d);
                        dir_list = NULL;
                        dir_struct = NULL;

                        dir_name = strtok(NULL, " ");
                    }
                }
            }
            else
            {
                /* Structure containing all the information about the files */
                struct dirent *dir_struct;
                struct dirent **dir_list;

                /* Open the current directory */
                DIR *d = opendir(".");

                /* Check if directory can be opened */
                if(!d)
                {
                    printf("Error: Can not open the directory\n");
                    closedir(d);
                    continue;
                }

                /* Count the number of files */
                int file_cnt = 0;
                while((dir_struct = readdir(d)) != NULL)
                {
                    if(dir_struct->d_name[0] != '.')
                    {
                        file_cnt++;
                    }
                }

                /* Allocate space for list of directories */
                dir_list = malloc(file_cnt * sizeof(dir_list));
                if(!dir_list)
                {
                    printf("Error: Memory exhausted\n");
                    closedir(d);
                    continue;
                }

                /* Reset the pointer of directories */
                rewinddir(d);

                /* Store the file names in dir_list */
                int i = 0;
                while((dir_struct = readdir(d)) != NULL)
                {
                    if(dir_struct->d_name[0] != '.')
                    {
                        dir_list[i++] = dir_struct;
                    }
                }

                /* Sort the file names */
                qsort(dir_list, file_cnt, sizeof(*dir_list), compareName);
                
                /* Display */
                for(int i=0; i<file_cnt; i++)
                {
                    printf("%d)\t%s\n", i+1, dir_list[i]->d_name);
                }

                closedir(d);
            }
        }
/* ******************************************************* CD ******************************************************* */        
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
            handle[0] = '\0';
            snprintf(handle, sizeof(handle), "\n%s%s@%s:%s%s%s$ ", BGRN, username, hostname, BBLU, cwd, RESET);
        }

/* ******************************************************* CAT ******************************************************* */        
        else if (strcmp(command, "cat") == 0)
        {
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


/* ******************************************************* MKDIR ******************************************************* */        
        else if(strcmp(command, "mkdir") == 0)
        {
            if(command_args != NULL && strcmp(command_args, "-m") != 0)
            {
                struct stat st = {0};

                if(stat(command_args, &st) == -1)
                {
                    /* Create a directory; Implement mkdir <filename> */
                    umask(0);
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

                if(!mode_str)
                {
                    printf("Error: Specify the mode\n");
                    continue;
                }
                
                /* Validate mode */
                for(int i=0; i<strlen(mode_str); i++)
                {
                    if(mode_str[i]-'0' > 7)
                    {
                        printf("Error: Mode is invalid\n");
                        continue;
                    }
                }
                
                /* Create the mode */
                mode_t mode = str2modet(mode_str);

                /* To store the filename */
                char *file_name = strtok(NULL, " ");

                if(!mode_str)
                {
                    printf("Error: Specify the directory name\n");
                    continue;
                }

                if(mode_str && file_name)
                {
                    umask(0);
                    if (mkdir(file_name, mode) == -1)
                    {
                        printf("Error in creating directory\n");
                    }
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


/* ******************************************************* CP ******************************************************* */        
        else if(strcmp(command, "cp") == 0)
        {
            if(command_args != NULL && strcmp(command_args, "-u") == 0)
            {
                FILE *fp_src, *fp_dest;
                char *file_name_src = strtok(NULL, " ");
                char *file_name_dest = strtok(NULL, " ");

                /* Open file for reading */
                fp_src = fopen(file_name_src, "r");
                if (fp_src == NULL)
                {
                    printf("Cannot open file %s \n", file_name_src);
                    continue;
                }

                /* Open another file for writing */
                if(fopen(file_name_dest, "r"))
                {
                    fp_dest = fopen(file_name_dest, "r");
                    
                    /* Check the timestamps of source */
                    struct stat attr_src;
                    stat(file_name_src, &attr_src);

                    /* Check the timestamps of destination */
                    struct stat attr_dest;
                    stat(file_name_dest, &attr_dest);

                    /* Check if timestamp of src is greater than destination */
                    if(difftime(attr_src.st_mtime, attr_dest.st_mtime) > 0)
                    {
                        fp_dest = fopen(file_name_dest, "w");
                        char c;

                        /* Read contents from file */
                        c = fgetc(fp_src);
                        while (c != EOF)
                        {
                            fputc(c, fp_dest);
                            c = fgetc(fp_src);
                        }
                        fclose(fp_src);
                        fclose(fp_dest);
                    }
                    else
                    {
                        printf("Can not be coppied since the source file is older than the destination\n");
                    }
                }
                if (fp_dest == NULL)
                {
                    printf("Cannot open file %s \n", file_name_dest);
                    exit(0);
                }
            }
            else if(command_args != NULL && strcmp(command_args, "-u") != 0)
            {                
                /* Check if the command has a directory */
                char *destination = strrchr(input, ' ');
                destination++;
                DIR *d = opendir(destination);
                
                if(d)
                {
                    /* The last argument is a folder */

                    /* Copy files to destination directory iteratively */
                    char *file_name = command_args;
                    while(file_name != NULL && strcmp(file_name, destination) != 0)
                    {
                        FILE *fp_src, *fp_dest;
                        char file_name_dest[PATH_MAX] = "";
                        sprintf(file_name_dest, "%s/%s", destination, file_name);
                        fp_src = fopen(file_name, "r");
                        fp_dest = fopen(file_name_dest, "w");

                        if(fp_src && fp_dest)
                        {
                            char buffer[PATH_MAX] = {0};

                            while(fgets(buffer, PATH_MAX, fp_src))
                            {
                                fputs(buffer, fp_dest);
                            }
                            fclose(fp_dest);
                            fclose(fp_src);
                        }
                        else
                        {
                            if(!fp_src)
                            {
                                printf("Error: Can not open file 1\n");
                            }
                            else if(!fp_dest)
                            {
                                //printf("Error: Can not open file 2\n");
                            }
                            else
                            {
                                break;
                            }
                        }

                        file_name = strtok(NULL, " ");
                    }
                    closedir(d);
                }
                /* If it is a file */
                else if (ENOENT == errno)
                {
                    FILE *fp_src, *fp_dest;
                    char filename[100], c;

                    /* Open file for reading */
                    fp_src = fopen(command_args, "r");
                    if (fp_src == NULL)
                    {
                        printf("Cannot open file 1%s \n", filename);
                    }

                    /* Open another file for writing */
                    fp_dest = fopen(destination, "w");
                    if (fp_dest == NULL)
                    {
                        printf("Cannot open file 2%s \n", destination);
                    }

                    /* Read contents from file */
                    c = fgetc(fp_src);
                    while (c != EOF)
                    {
                        fputc(c, fp_dest);
                        c = fgetc(fp_src);
                    }
                    fclose(fp_src);
                    fclose(fp_dest);
                }
            }
            else
            {
                printf("Error: Arguement is not recognized\n");
            }
        }

/* ******************************************************* SORT ******************************************************* */        
        else if (strcmp(command, "sort") == 0)
        {
             
            char *file_name = command_args;
            FILE *fptr;
            FILE *destination = NULL;

            destination = fopen("out.txt", "a");// create a temperory txt 

            while (file_name != NULL)
            {
                
                if (strcmp(file_name, "-r") != 0)
                {
                    FILE *ptr1, *ptr2;
                    char filename[100000], c;

                    ptr1 = fopen(file_name, "r");
                    if (ptr1 == NULL)
                    {
                        printf("Cannot open file %s \n", file_name);
                        // exit(0);
                    }
                    
                    c = fgetc(ptr1);
                    while (c != EOF)
                    {

                        fputc(c, destination);
                        c = fgetc(ptr1);
                    }
                    fclose(ptr1);
                }
                file_name = strtok(NULL, " ");
            }

            fclose(destination);
            char Temp[1000000];
            char **Data = NULL; // String List
            
            /* Data_small is a copy of Data but the lowercase and uppercase is interchanged for comparison */
            char **Data_small = NULL;
            int i, j;
            int Lines = 0;

            FILE *ptr1 = NULL;
            FILE *ptr2 = NULL;
            if (strcmp(command_args, "-r") != 0)
            {
                ptr1 = fopen("out.txt", "r");
            }
            else if (strcmp(command_args, "-r") == 0)
            {
                ptr1 = fopen("out.txt", "r");
            }
             
            // Read and store in a string list.
            while (fgets(Temp, 1000, ptr1) != NULL)
            {
                // Remove the trailing newline character
                if (strchr(Temp, '\n'))
                    Temp[strlen(Temp) - 1] = '\0';
                Data = (char **)realloc(Data, sizeof(char **) * (Lines + 1));
                Data_small = (char **)realloc(Data_small, sizeof(char **) * (Lines + 1));
                
                Data[Lines] = (char *)calloc(1000, sizeof(char));
                Data_small[Lines] = (char *)calloc(1000, sizeof(char));
                
                strcpy(Data[Lines], Temp);
                strcpy(Data_small[Lines], Temp);
                for(int i=0; i<strlen(Data_small[Lines]); i++)
                {
                    char ch = Data_small[Lines][i];
                    // putchar(ch);
                    if(ch >= 'A' && ch <= 'Z')
                        Data_small[Lines][i] = (tolower(ch));
                    else if(ch >= 'a' && ch <= 'z')
                        Data_small[Lines][i] = (toupper(ch));
                }
                // printf("%s\n",Data_small[Lines]);
                Lines++;
            }
            // printf("---\n");

            // use bubble sort for sorting array;
            for (i = 0; i < (Lines - 1); ++i)
            {
                for (j = 0; j < (Lines - i - 1); ++j)
                {
                    if (strcmp(Data_small[j], Data_small[j + 1]) > 0)
                    {
                        strcpy(Temp, Data[j]);
                        strcpy(Data[j], Data[j + 1]);
                        strcpy(Data[j + 1], Temp);

                        char Temp_temp[10000];
                        strcpy(Temp_temp, Data_small[j]);
                        strcpy(Data_small[j], Data_small[j + 1]);
                        strcpy(Data_small[j + 1], Temp_temp);
                    }
                }
            }
            if (strcmp(command_args, "-r") != 0) // Write it to outfile. file.
            {
                for (i = 0; i < Lines; i++)
                {
                    // fprintf(ptrSummary, "%s\n", strData[i]);
                    printf("%s\n", Data[i]);
                }
            }
            else if (strcmp(command_args, "-r") == 0)
            {
                for (i = Lines - 1; i >= 0; i--)
                {
                    // fprintf(ptrSummary, "%s\n", strData[i]);
                    printf("%s\n", Data[i]);
                }
            }

            // free each string
            for (i = 0; i < Lines; i++)
                free(Data[i]);

            // free string list.
            remove("out.txt"); //remove the temp out.txt file
            free(Data);
            fclose(ptr1);
            //fclose(ptr2);
        }


/* ******************************************************* GREP ******************************************************* */        
        else if(strcmp(command, "grep") == 0)
        {
            if(command_args != NULL && strcmp(command_args, "-n") == 0)
            {
                /* Remove inital quotes from the pattern if they are present */
                char *pattern_raw = strtok(NULL, " ");
                char pattern[(int)strlen(pattern_raw)];
                if(pattern_raw[0] == '\'' || pattern_raw[0] == '"')
                {
                    pattern_raw++;
                }
                int j = 0;
                for(int i=0; i<(int)strlen(pattern_raw); i++)
                {
                    pattern[j++] = pattern_raw[i];
                }
                if(pattern_raw[j-1] == '\'' || pattern_raw[j-1] == '"')
                {
                    pattern[j-1] = '\0';
                }
                else
                {
                    pattern[j] = '\0';
                }

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
                /* Remove inital quotes from the pattern if they are present */
                char *pattern_raw = command_args;
                char pattern[(int)strlen(pattern_raw)];
                if(pattern_raw[0] == '\'' || pattern_raw[0] == '"')
                {
                    pattern_raw++;
                }
                int j = 0;
                for(int i=0; i<(int)strlen(pattern_raw); i++)
                {
                    pattern[j++] = pattern_raw[i];
                }
                if(pattern_raw[j-1] == '\'' || pattern_raw[j-1] == '"')
                {
                    pattern[j-1] = '\0';
                }
                else
                {
                    pattern[j] = '\0';
                }

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
        else
        {
            printf("This command is not recognized \n");
        }
        
    }
    return 0;
}