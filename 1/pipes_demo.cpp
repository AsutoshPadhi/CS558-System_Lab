#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

int Randoms(int l, int u, int c)
{
    int i;
    for (i = 0; i < c; i++)
    {
        int num = (rand() % (u - l + 1)) + l;
        return num;
    }
}

int main()
{
    srand(time(NULL));
    int n, min = 1, score, readscore, flag = 0, cp, N, index, p;
    printf("Enter the number of players : ");
    scanf("%d", &n);
    int max = n;
    int position[n + 1] = {0};
    int fd[2 * n];
    char c;
    for (int i = 0; i < n; i++)
    {
        pipe(&fd[2 * i]);
    }

    cout << "enter the grid number ";
    cin >> N;
    int board[N + 1];
    for (int i = 1; i <= N; i++)
        board[i] = i;
    string word;
    ifstream myfile("file.txt");
    if (myfile.is_open())
    {
        while (myfile >> word)
        {
            myfile >> index;
            myfile >> p;
            board[index] = p;
        }
        myfile.close();
    };

    int range = max - min + 1;
    // int num =  (min + (rand() % (int)(max - min + 1)));
    int num = Randoms(min, max, n);
    int i = num;
    while (flag != 1)
    {
        do
        {
            printf("play your chance number %d player ", i);
            cin >> score;
            write(fd[2 * i + 1], &score, sizeof(score));
            read(fd[2 * i], &readscore, sizeof(readscore));
            if (readscore > 6)
            {
                cout << "score should be between 1 to 6 plz try again"<<"\n";
            }
            else
            {
                cp = position[i];
                if ((cp + readscore) >= N)
                {
                    cout << "player " << i << " is the winner"
                         << "\n";
                    return 0;
                }
                else
                {
                    position[i] = board[cp + readscore];
                    for (int i = 1; i <= n; i++)
                        cout << position[i] << " ";
                    cout << "\n";
                }
            }
        } while (readscore == 6||readscore>6);
        if (i == max)
            i = min;
        else
            i++;
    }
}