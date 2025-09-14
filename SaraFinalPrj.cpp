#include <iostream>
#include <cstring>
#include <ncurses.h>
using namespace std;

int totalscore = 0;
int incnum = 0;
int truenum = 0;
int selectedRow = 0, selectedCol = 0;
int remainingTime;
bool gamePaused = false;
bool gameOver = false;

bool SudokuAllg(int a[9][9]);
void PrintSudoku(int a[9][9]);
void ClearScreen(void);
void initializeEditable(int a[9][9]);
void updateTimer();
void saveGame(int a[9][9]);
void endGame(bool won);
void pauseMenu();
void title(void);
char *GetUsername(void);
bool CheckUsername(char *);
int MainMenu(char *);
int Gamelevel(void);
void FirstSudoku(int a[9][9]);
void loadGame(int a[9][9]);

bool editable[9][9] = {};
bool isFixed[9][9] = {};

struct Player
{
    char name[100];
    int finalscore;
    int WinORLose;
    int truenums;
    int falsenums;
    int timeuse;
};
struct SavedGame
{
    int isfixed[9][9];
    int matrix[9][9];
    int score, incorrect, correct, remainingTime;
    bool paused, gameOver;
    bool won;
    char name[50];
};
SavedGame savedGames[10];
int savedGameCount = 0;

void doCurrentGame(int a[9][9])
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(1000);

    while (!gameOver)
    {
        if (!gamePaused)
        {
            ClearScreen();
            updateTimer();
            PrintSudoku(a);
        }
        ClearScreen();
        updateTimer();
        PrintSudoku(a);

        int ch = getch();

        if (remainingTime <= 0 || incnum == 3)
            endGame(false);

        switch (ch)
        {
        case KEY_UP:
            if (selectedRow > 0)
                selectedRow--;
            break;
        case KEY_DOWN:
            if (selectedRow < 8)
                selectedRow++;
            break;
        case KEY_LEFT:
            if (selectedCol > 0)
                selectedCol--;
            break;
        case KEY_RIGHT:
            if (selectedCol < 8)
                selectedCol++;
            break;

        case 'q':
            endGame(false);
            break;

        case 'l':
            loadGame(a);
            ClearScreen();
            updateTimer();
            PrintSudoku(a);
            break;

        case 's':
            saveGame(a);
            ClearScreen();
            return;
            break;

        case 27: // ESC
            gamePaused = !gamePaused;
            if (gamePaused)
                pauseMenu();

            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (!isFixed[selectedRow][selectedCol])
            {
                int oldNum = a[selectedRow][selectedCol];
                a[selectedRow][selectedCol] = ch - '0';

                if (!SudokuAllg(a))
                {
                    mvprintw(30, 0, "Incorrect answer! Cell not changed.\n");
                    a[selectedRow][selectedCol] = oldNum;
                    incnum++;
                    totalscore--;
                }
                if (SudokuAllg(a))
                {
                    truenum++;
                    totalscore++;
                }
            }
            else
                mvprintw(10, 0, "You can't change this cell!\n");

            break;
        }
    }
}

int main()
{
    Player P[100];
    int i = 0;
    int a[9][9] = {};
    title();
    char *username = GetUsername();
    if (!CheckUsername(username))
    {
        cout << "Username is invalid. Exiting program!" << endl;
        return 0;
    }

    strcpy(P[i].name, username);
    ClearScreen();
    bool toExit = false;
    while(!toExit)
        switch (MainMenu(P[i].name))
        {
            case 0:
                doCurrentGame(a);
                break;
            case 1:
                ClearScreen();
                initscr();
                cbreak();
                noecho();
                keypad(stdscr, TRUE);
                timeout(1000);
                switch (Gamelevel())
                {
                    ClearScreen();
                    refresh();
                case 1:
                    remainingTime = 5 * 60;
                    break;
                case 2:
                    remainingTime = 3 * 60;
                    break;
                case 3:
                    remainingTime = 2 * 60;
                    break;
                }

                a[0][1] = 2;
                a[0][3] = 5;
                a[0][5] = 1;
                a[0][7] = 9;
                a[1][0] = 8;
                a[1][3] = 2;
                a[1][5] = 3;
                a[1][8] = 6;
                a[2][1] = 3;
                a[2][4] = 6;
                a[2][7] = 7;
                a[3][2] = 1;
                a[3][6] = 6;
                a[4][0] = 5;
                a[4][1] = 4;
                a[4][7] = 1;
                a[4][8] = 9;
                a[5][2] = 2;
                a[5][6] = 7;
                a[6][1] = 9;
                a[6][4] = 3;
                a[6][7] = 8;
                a[7][0] = 2;
                a[7][3] = 8;
                a[7][5] = 4;
                a[7][8] = 7;
                a[8][1] = 1;
                a[8][4] = 9;
                a[8][6] = 7;
                a[8][7] = 6;

                initializeEditable(a);

                doCurrentGame(a);
                printf("\nAfter Game!");
                break;

            case 2:
                loadGame(a);
                ClearScreen();
                updateTimer();
                PrintSudoku(a);
                break;

            case 3:

                break;

            case 4:
                toExit = true;
                break;
        }
    
    endwin();

    return 0;
}

void title(void)
{
    cout << "IN HIS SUBLIME NAME" << endl;
    cout << "Let's Play Sudoku!" << endl;
}

char *GetUsername(void)
{
    cout << "Please enter your username: ";
    static char a[100];
    cin >> a;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    return a;
}

bool CheckUsername(char *a)
{
    if (strlen(a) > 0)
        return true;
    else
        return false;
}

int MainMenu(char *a)
{
    ClearScreen();
    mvprintw(4, 0, "You Logged in as");
    mvprintw(4, 18, a);
    mvprintw(4, 30, "\n");
    
    mvprintw(5, 0, "0: Continue..."
                   "\n");
    mvprintw(6, 0, "1: Start a New Game"
                   "\n");
    mvprintw(7, 0, "2: Play a Saved Game "
                   "\n");
    mvprintw(8, 0, "3: LeaderBoard "
                   "\n");
    mvprintw(9, 0, "4: Exit the Game"
                   "\n");
    mvprintw(13, 0, "Press a number from 1 to 4: "
                    "\n");
    int num = 0;
    while (true)
    {
        int ch = getch();

        if (ch == '0')
        {
            num = 0;
            break;
        }
        else if (ch == '1')
        {
            num = 1;
            break;
        }
        else if (ch == '2')
        {
            num = 2;
            break;
        }
        else if (ch == '3')
        {
            num = 3;
            break;
        }
        else if (ch == '4')
        {
            num = 4;
            break;
        }
        else
        {
            mvprintw(15, 0, "Invalid choice. Please choose 1, 2, 3 or 4.");
            refresh();
        }
    }
    return num;
}

int Gamelevel(void)
{
    ClearScreen();
    mvprintw(5, 0, "Please choose your favorite level:");
    mvprintw(6, 0, "1: simple");
    mvprintw(7, 0, "2: intermediate");
    mvprintw(8, 0, "3: hard");
    int levelnum = 0;
    while (true)
    {
        int ch = getch();

        if (ch == '1')
        {
            levelnum = 1;
            break;
        }
        else if (ch == '2')
        {
            levelnum = 2;
            break;
        }
        else if (ch == '3')
        {
            levelnum = 3;
            break;
        }
        else
        {
            mvprintw(10, 0, "Invalid choice. Please choose 1, 2, or 3.");
            refresh();
        }
    }
    return levelnum;
}

void updateTimer()
{
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    mvprintw(0, 0, "Time Remaining: %02d:%02d", minutes, seconds);
    refresh();

    remainingTime--;
}

bool SudokuAllg(int a[9][9])
{
    int num = a[selectedRow][selectedCol];

    for (int j = 0; j < 9; j++)
    {
        if (a[selectedRow][j] == num && j != selectedCol)
        {
            return false;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (a[i][selectedCol] == num && i != selectedRow)
        {
            return false;
        }
    }

    int startRow = (selectedRow / 3) * 3;
    int startCol = (selectedCol / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            if (a[i][j] == num && (i != selectedRow || j != selectedCol))
            {
                return false;
            }
        }
    }

    return true;
}

void PrintSudoku(int a[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0 && i != 0)
        {
            printw("-------------\n");
        }
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0 && j != 0)
            {
                printw("| ");
            }
            if (a[i][j] == 0)
            {
                if (i == selectedRow && j == selectedCol)
                {
                    printw("[ ] ");
                }
                else
                {
                    printw(". ");
                }
            }
            else
            {
                if (i == selectedRow && j == selectedCol)
                {
                    printw("[%d] ", a[i][j]);
                }
                else
                {
                    printw("%d ", a[i][j]);
                }
            }
        }
        printw("\n");
    }
}

void ClearScreen()
{
    clear();
    refresh();
}

void initializeEditable(int a[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (a[i][j] != 0)
            {
                isFixed[i][j] = true;
            }
            else
            {
                isFixed[i][j] = false;
            }
        }
    }
}

void saveGame(int a[9][9])
{
    char name[50];
    mvprintw(12, 0, "Enter save name: ");
    refresh();
    timeout(-1); //time limit
    echo();
    getstr(name);
    noecho();
    timeout(1000);
    mvprintw(13, 0, "Game saved as: %s", name);
    refresh();
    getch();
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            savedGames[savedGameCount].matrix[i][j] = a[i][j];
            savedGames[savedGameCount].isfixed[i][j] = isFixed[i][j];
        }
    savedGames[savedGameCount].remainingTime = remainingTime;
    strcpy(savedGames[savedGameCount].name, name);
    savedGameCount++;
}

void loadGame(int a[9][9])
{
    endwin();
    printf("THIS IS LOADING...\n");

    for (int k = 0; k < savedGameCount; k++)
        printf("%d-%s\n", k, savedGames[k].name);

    printf("\nEnter saved game number to load:");
    int selection;
    cin >> selection;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            a[i][j] = savedGames[selection].matrix[i][j];
            isFixed[i][j] = savedGames[selection].isfixed[i][j];
        }
    remainingTime = savedGames[selection].remainingTime;
}

void endGame(bool won)
{
    ClearScreen();
    mvprintw(5, 0, "Game Over!");
    mvprintw(6, 0, "Status: %s", won ? "You Won!" : "You Lost!");
    mvprintw(7, 0, "Total Score: %d", totalscore);
    mvprintw(8, 0, "Corrects: %d", truenum);
    mvprintw(9, 0, "InCorrects: %d", incnum);
    if (remainingTime <= 0)
    {
        mvprintw(10, 0, "Time's up! You Lost!");
    }
    else if (incnum == 3)
    {
        mvprintw(10, 0, "3 Wrong Entries! You Lost!");
    }
    else if (won)
    {
        mvprintw(10, 0, "You completed the Sudoku correctly! You Won!");
    }
    refresh();

    while (true)
    {
        int ch = getch();
        if (ch == 10)
        { //10 means Enter
            break;
        }
    }
    gameOver = true;
}

void pauseMenu()
{
    ClearScreen();
    mvprintw(5, 0, "Game Paused");
    mvprintw(6, 0, "Press ESC to resume");
    refresh();
    while (gamePaused)
    {
        int ch = getch();
        if (ch == 27)
        {
            gamePaused = false;
        }
    }
}
