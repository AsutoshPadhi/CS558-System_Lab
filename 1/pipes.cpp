#include <iostream>
#include <map>
#include <string.h>
#include <fstream>
#include <unistd.h>

using namespace std;

void childProcess(int i, int n, int fdc[][2], int fdp[][2])
{
    /* Close the writing end of fdc */
    close(fdc[i][1]);

    /* Read the message from parent */
    char *message_to_child;
    read(fdc[i][0], message_to_child, 100);

    /* Close reading end of pipes */
    close(fdc[i][0]);
    close(fdp[i][0]);

    // if(message_to_child[0] == '1')
    // {
    //     /* Roll the dice */
    //     int lower = 1, upper = 6;
    //     srand(time(0));
    //     int dice = (rand() % (upper - lower + 1)) + lower;

    //     /* Create message for parent */
    //     char *message_to_parent = NULL;
    //     sprintf(message_to_parent, "%d", dice);
    //     write(fdp[i][1], message_to_parent, strlen(message_to_parent)+1);
    //     close(fdp[i][1]);
    // }
    // else
    // {
    //     exit(0);
    // }
}

int main(int argc, char** argv)
{
    /* Read input from command line arguement */
    string file_name = argv[1];
    int n_squares = atoi(argv[2]);
    int n_players = atoi(argv[3]);

    /**
     * @brief Create a data structure (map) for snakes and ladders
     * The index will be the initial position and the value will be the final position 
     */
    map<int, int> snakes_n_ladders;
    
    /* Read file */
    ifstream infile(file_name);
    if(!infile)
    {
        cout<<"Error: Can not open file"<<endl;
    }

    string letter;
    int start_pos, end_pos;
    while(infile >> letter >> start_pos >> end_pos)
    {
        if(letter == "L")
        {
            if(start_pos < end_pos)
            {
                snakes_n_ladders.insert(pair<int, int>(start_pos, end_pos));
            }
            else
            {
                cout<<"Error: File has one start position more than end position in case of ladder";
            }
        }
        else if(letter == "S")
        {
            if(start_pos > end_pos)
            {
                snakes_n_ladders.insert(pair<int, int>(start_pos, end_pos));
            }
            else
            {
                cout<<"Error: File has one start position less than end position in case of snake";
            }
        }
        else
        {
            cout<<"Error : Input File has an undefined character"<<endl;
        }
    }

    /**
     * @brief Create pipes
     */
    int fd_to_child[n_players][2];
    int fd_to_parent[n_players][2];
    for(int i=0; i<n_players; i++)
    {
        if(pipe(fd_to_child[i]) < 0)
        {
            cout<<"Error: Unable to create pipes"<<endl;
        }
        if(pipe(fd_to_parent[i]) < 0)
        {
            cout<<"Error: Unable to create pipes"<<endl;
        }
    }

    /**
     * @brief Create n_player processes and store their pids
     */
    int pid[n_players];
    for(int i=0; i<n_players; i++)
    {
        pid[i] = fork();

        /* Call the child process */
        childProcess(i, n_players, fd_to_child, fd_to_parent);
    }

    /* Keep a record of the current position of players */
    int positions[n_players] = {0};

    /* Flag variable to suggest if game has ended */
    bool end_game = false;

    /* Indicates the current player */
    int current_player = 0;

    /* Randomly select first player */
    int lower = 1, upper = n_players;
    srand(time(0));
    current_player = (rand() % (upper - lower + 1)) + lower;

    /* Start the game */
    while(!end_game)
    {
        /* Notify the child of current process to play dice */
        write(fd_to_child[current_player][1], "1", 2);
        close(fd_to_child[current_player][1]);

        /* Read the response of child */
        char *message_from_child = NULL;
        read(fd_to_parent[current_player][0], message_from_child, 100);
        close(fd_to_parent[current_player][0]);

        /* Move in board */
        int dice = atoi(message_from_child);
        
        /* Search if the position is present in the map */

        /* Update and print the position */

        /* Check if the game is over */

        /* Update current player */
        if(dice != 6)
        {
            current_player = (current_player + 1)%n_players;
        }
    }

    return 0;
}