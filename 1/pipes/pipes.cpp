#include <iostream>
#include <map>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 4)
    {
        cout<<"\nError : There should be 3 arguements - ./a.out <filename> <#sqaures> <#players>\n"<<endl;
        exit(1);
    }
    
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
                cout<<"\nError: File has one start position more than end position in case of ladder\n"<<endl;
                exit(1);
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
                cout<<"\nError: File has one start position less than end position in case of snake\n"<<endl;
                exit(1);
            }
        }
        else
        {
            cout<<"\nError : Input File has an undefined character\n"<<endl;
        }
    }
    infile.close();

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
        if(pid[i] == 0)
        {
            srand(time(0)*i);

            for(int j=0; j<n_players; j++)
            {
                if(i != j)
                {
                    close(fd_to_child[j][0]);
                    close(fd_to_child[j][1]);
                    close(fd_to_parent[j][0]);
                    close(fd_to_parent[j][1]);
                }
            }
            close(fd_to_child[i][1]);
            
            while(1)
            {
                /* Read the message from parent */
                char message_to_child[2];
                int status = read(fd_to_child[i][0], message_to_child, 2);

                if(status ==  0)
                {
                    cout<<"Closing Process"<<endl;
                    exit(0);
                }

                if(message_to_child[0] == '1')
                {
                    /* Roll the dice */
                    int lower = 1, upper = 6;
                    int dice = (rand() % (upper - lower + 1)) + lower;

                    /* Create message for parent */
                    char message_to_parent[2];
                    sprintf(message_to_parent, "%d", dice);
                    write(fd_to_parent[i][1], message_to_parent, strlen(message_to_parent)+1);
                }
            }
        }
    }

    /* Keep a record of the current position of players */
    int position[n_players] = {0};

    /* Flag variable to suggest if game has ended */
    bool end_game = false;

    /* Indicates the current player */
    int current_player = 0;

    /* Randomly select first player */
    int lower = 0, upper = n_players-1;
    srand(time(0));
    current_player = (rand() % (upper - lower + 1)) + lower;

    /* Start the game */
    // close(fd_to_parent[current_player][0]);
    while(!end_game)
    {
        cout<<"\n\nPlayer "<<current_player+1<<" is playing ..."<<endl;

        /* Notify the child of current process to play dice */
        write(fd_to_child[current_player][1], "1", 2);

        /* Read the response of child */
        char message_from_child[2];
        read(fd_to_parent[current_player][0], message_from_child, 100);

        /* Move in board */
        int dice = message_from_child[0] - '0';
        cout<<"Dice = "<<dice<<endl;
        
        /* Update position accoring to dice */
        position[current_player] += dice;

        /* Search if there are any snakes or ladders */
        auto snl = snakes_n_ladders.find(position[current_player]);
        
        /* Update the position */
        if(snl != snakes_n_ladders.end())
        {
            position[current_player] = snl->second;
            if(snl->first > snl->second)
            {
                cout<<"Oh no! The snake threw you back to "<<snl->second<<endl;
            }
            else
            {
                cout<<"Yes!!! a direct ladder to "<<snl->second<<endl;
            }
        }

        /* Print the position */
        for(int i=0; i<n_players; i++)
        {
            cout<<i+1<<":"<<position[i]<<"\t";
        }
        cout<<endl;

        /* Check if the game is over */
        if(position[current_player] >= n_squares)
        {
            end_game = true;
            cout<<"\nPlayer "<<current_player+1<<" has won the game !!!\n\n";
            break;
        }

        /* Update current player */
        if(dice != 6)
        {
            current_player = (current_player + 1)%n_players;
        }
    }

    if(end_game)
    {
        /* Close all pipes */
        cout<<"Closing all pipes"<<endl;
        cout<<"PIDs: "<<endl;
        for(int i=0; i<n_players; i++)
        {
            cout<<pid[i]<<"\t";
        }
        cout<<endl;
        for(int i=0; i<n_players; i++)
        {
            close(fd_to_child[i][0]);
            close(fd_to_child[i][1]);
            close(fd_to_parent[i][0]);
            close(fd_to_parent[i][1]);
        }
        
        /* Kill all child processes */
        // cout<<"Gracefully exiting all child processes"<<endl;
        for(int i=0; i<n_players; i++)
        {
            char kill_cmd[50];
            sprintf(kill_cmd, "kill -s TERM %d", pid[i]);
            // system(kill_cmd);
        }
        cout<<endl;
    }

    return 0;
}