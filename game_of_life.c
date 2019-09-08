#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

const int MAX_LINES = 64;
const int MAX_COLS = 256;

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int LINES, COLS;
int actual_map[MAX_LINES][MAX_COLS];
int following_map[MAX_LINES][MAX_COLS];
int generation, ini_num;
int population = 0;

void menu()
{
    printf("\n\n\n\n\t##################################################################");
    printf("\n\t##\tGame of life:\t\t\t\t\t\t##\n");
    printf("\t##\tPablo Ospino.\t\t\t\t\t\t##\n");
    printf("\t##\t\tChoose one of the following options:\t\t##\n");
    printf("\t##\t\t\t1. Random\t\t\t\t##\n");
    printf("\t##\t\t\t2. Spiral\t\t\t\t##\n");
    printf("\t##\t\t\t3. Big Spiral\t\t\t\t##\n");
    printf("\t##\t\t\t4. Spaceship\t\t\t\t##\n");
    printf("\t##\t\t\t5. Jellyfish\t\t\t\t##\n");
    printf("\t##\t\t\t6. Square\t\t\t\t##\n");
    printf("\t##\t\t\t7. Canoe\t\t\t\t##\n");
    printf("\t##\t\t\t8. Diehard \t\t\t\t##\n");
    printf("\t##\t\t\t9. Line initialization\t\t\t##\n");
    printf("\t##\t\t\t10. Indefine\t\t\t\t##\n");
    printf("\t##################################################################\n");
    printf(": ");
    scanf("%i", &ini_num);
    while (ini_num > 10 || ini_num < 1)
    {
        printf("Choose one of the available options: ");
        scanf("%i", &ini_num);
    }

    system("clear");
    for (int i = 0; i < 3; i++)
    {
        printf("Wait, %i \n", 3 - i);
        sleep(1);
        system("clear");
    }
}

int get_alive_neighbors(int row, int col, int map[MAX_LINES][MAX_COLS])
{
    int alive_neighbors = 0;

    if (col == COLS & row != LINES)
    {
        //DERECHA
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row - 1 + i][0];
        }
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row - 1 + i][col - 1];
        }
        alive_neighbors += map[row + 1][col]; //izquierda
        alive_neighbors += map[row - 1][col]; //derecha
    }
    else if (col == 0 & row != 0)
    {
        //IZQUIERDA
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row - 1 + i][col + 1];
        }
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row - 1 + i][COLS];
        }
        alive_neighbors += map[row + 1][col]; //izquierda
        alive_neighbors += map[row - 1][col]; //derecha
    }
    else if (row == LINES & col != COLS)
    {
        //ABAJO
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row - 1][col - 1 + i];
        }
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[0][col - 1 + i];
        }
        alive_neighbors += map[row][col + 1]; //izquierda
        alive_neighbors += map[row][col - 1]; //derecha
    }
    else if (row == 0 & col != 0)
    {
        //ARRIBA
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[LINES][col - 1 + i];
        }
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row + 1][col - 1 + i];
        }
        alive_neighbors += map[row][col + 1]; //izquierda
        alive_neighbors += map[row][col - 1]; //derecha
    }
    else
    {

        //parte de arriba
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row - 1][col - 1 + i];
        }
        //parte de abajo
        for (int i = 0; i < 3; i++)
        {
            alive_neighbors += map[row + 1][col - 1 + i];
        }
        alive_neighbors += map[row][col - 1]; //izquierda
        alive_neighbors += map[row][col + 1]; //derecha
    }

    return alive_neighbors;
}

void reset_map()
{
    for (int row = 0; row < MAX_LINES; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            actual_map[row][col] = 0;
        }
    }
}

void ini_random()
{
    for (int row = 0; row < MAX_LINES; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            actual_map[row][col] = floor(rand() % 2); // alive or not
        }
    }
}
void ini_spiral()
{
    int tabla[16] = {0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0};
    int i = 0;
    for (int row = 10; row < 14; row++)
        for (int col = 10; col < 14; col++)
        {
            actual_map[row][col] = tabla[i];
            i++;
        }
}
void ini_bigger_spiral()
{
    int tabla[81] = {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
                     0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1};

    int i = 0;
    for (int row = 15; row < 24; row++)
        for (int col = 15; col < 24; col++)
        {
            actual_map[row][col] = tabla[i];
            i++;
        }
}
void ini_spaceship()
{
    int tabla[35] = {0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                     0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1};
    int i = 0;
    for (int row = 10; row < 15; row++)
        for (int col = 10; col < 17; col++)
        {
            actual_map[row][col] = tabla[i];
            actual_map[col][row + 30] = tabla[i];
            i++;
        }
}
void ini_jellyfish()
{
    int tabla[35] = {1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1};
    int i = 0;
    for (int row = 10; row < 15; row++)
        for (int col = 10; col < 17; col++)
        {
            actual_map[row][col] = tabla[i];
            i++;
        }
}
void ini_wiki()
{
    int tabla[9] = {1, 1, 0, 0, 1, 1, 0, 1, 0};
    int i = 0;
    for (int row = 10; row < 13; row++)
        for (int col = 40; col < 43; col++)
        {
            actual_map[row][col] = tabla[i];
            i++;
        }
}
void ini_square()
{
    int tabla[9] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
    int i = 0;
    for (int row = 10; row < 13; row++)
        for (int col = 10; col < 13; col++)
        {
            actual_map[row][col] = tabla[i];
            actual_map[row][col + 3] = tabla[i];
            actual_map[row + 3][col] = tabla[i];
            actual_map[row + 3][col + 3] = tabla[i];
            i++;
        }
}
void ini_canoe()
{
    int tabla[25] = {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1};
    int i = 0;
    for (int row = 10; row < 15; row++)
        for (int col = 10; col < 15; col++)
        {
            actual_map[row][col] = tabla[i];
            actual_map[row][col + 10] = tabla[i];
            actual_map[row][col + 20] = tabla[i];
            i++;
        }
}
void ini_diehard()
{
    int tabla[24] = {0, 0, 0, 0, 0, 0, 1, 0,
                     1, 1, 0, 0, 0, 0, 0, 0,
                     0, 1, 0, 0, 0, 1, 1, 1};
    int i = 0;
    for (int row = 10; row < 13; row++)
        for (int col = 50; col < 58; col++)
        {
            actual_map[row][col] = tabla[i];
            i++;
        }
}
void ini_line()
{
    int tabla[39] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
                     1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1};
    int i = 0;
    for (int col = 40; col < 79; col++)
    {
        actual_map[20][col] = tabla[i];
        i++;
    }
}

void ini_indef()
{
    int tabla[25] = {
        1,
        1,
        1,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        1,
        1,
        0,
        1,
        1,
        0,
        1,
        0,
        1,
    };
    int i = 0;
    for (int row = 20; row < 25; row++)
        for (int col = 50; col < 55; col++)
        {
            actual_map[row][col] = tabla[i];
            i++;
        }
}

void update()
{
    for (int row = 0; row < MAX_LINES; row++)
        for (int col = 0; col < MAX_COLS; col++)
        {
            int alive_neighbors = get_alive_neighbors(row, col, actual_map);
            int alive = actual_map[row][col];

            if (alive)
                following_map[row][col] = (alive_neighbors == 2 || alive_neighbors == 3);
            else
                following_map[row][col] = (alive_neighbors == 3);
        }

    //update la matriz actual_mapual.
    for (int row = 0; row < MAX_LINES; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            actual_map[row][col] = following_map[row][col];
        }
    }
}

void draw()
{
    system("clear");
    printf("\n\n\n");
    population = 0;
    for (int row = 0; row < MIN(MAX_LINES, LINES); row++)
    {
        for (int col = 0; col < MIN(MAX_COLS, COLS); col++)
        {
            if (actual_map[row][col])
            {
                printf("◼");
                population += actual_map[row][col];
            }
            else
                printf(" ");
        }
        printf("\n");
    }
    printf("Game of life: PABLO OSPINO\t Generation nº: %i\t Population: %i\n", generation, population);
    printf("Ctrl + C: QUIT    Ctrl + S: STOP    Ctrl + P: PLAY \n ");
}

int get_window_size(int *LINES, int *COLS)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *LINES = w.ws_row - 3;
    *COLS = w.ws_col;
    return 0;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    reset_map();
    menu();
    switch (ini_num)
    {
    case 1:
        ini_random();
        break;
    case 2:
        ini_spiral();
        break;
    case 3:
        ini_bigger_spiral();
        break;
    case 4:
        ini_spaceship();
        break;
    case 5:
        ini_jellyfish();
        break;
    case 6:
        ini_square();
        break;
    case 7:
        ini_canoe();
        break;
    case 8:
        ini_diehard();
        break;
    case 9:
        ini_line();
        break;
    case 10:
        ini_indef();
        break;
    }

    while (1)
    {
        get_window_size(&LINES, &COLS);
        draw();
        update();
        usleep(200000); //Wait 2 milisegundos
        generation++;
    }
    return 0;
}
