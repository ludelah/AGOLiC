#include "raylib.h"
#include <stdio.h>
#define SIZE 40
#define SCREEN_HEIGHT 1000
#define SCREEN_WIDTH 1000


void printGrid(int grid[SIZE][SIZE], int isDarkMode) {
    Color alive = isDarkMode ? WHITE : BLACK;
    Color dead = isDarkMode ? BLACK : WHITE;
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0)
                DrawRectangle(j * (SCREEN_WIDTH / SIZE), i * (SCREEN_HEIGHT / SIZE), SIZE, SIZE, dead);
            else if(grid[i][j] == 1)
            {   
                Color color = { GetRandomValue(20, 255), GetRandomValue(0, 90), GetRandomValue(150, 255), 255 };
                DrawRectangleGradientV(j * (SCREEN_WIDTH / SIZE), i * (SCREEN_HEIGHT / SIZE), SIZE, SIZE, alive, color);
            }
        }
    }
    

}

void updateGrid(int grid[SIZE][SIZE]) {
    int tempGrid[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int aliveNeighbors = 0;

            for (int y = i-1; y <= i+1; y++)
            {
                if (y < 0 || y > SIZE){continue;}
                
                for (int x = j-1; x <= j+1; x++)
                {
                    if (x < 0 || x > SIZE){continue;}
                    aliveNeighbors += grid[y][x];
                }
            }
            aliveNeighbors -= grid[i][j];

            if (aliveNeighbors < 2 || aliveNeighbors > 3)
                tempGrid[i][j] = 0;
            else if (aliveNeighbors == 3)
                tempGrid[i][j] = 1;
            else 
                tempGrid[i][j] = grid[i][j];
        }
    }

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = tempGrid[i][j];
}

void generateGrid(int grid[SIZE][SIZE], unsigned int seed, float rate)
{
    SetRandomSeed(seed);
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            grid[i][j] = GetRandomValue(0, 1);
        }
    }
}

int main(void)
{
    unsigned int seed = 69;
    int grid[SIZE][SIZE] = {0};
    float rate = 0.5f;
    int time = 0;
    
    int isDarkMode = 0;
    
    int scalar = SCREEN_WIDTH / SIZE;
    // Glider
    /*grid[1][0] = 1;
    grid[2][1] = 1;
    grid[0][2] = 1;
    grid[1][2] = 1;
    grid[2][2] = 1;
       */
       
    generateGrid(grid, seed, rate);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
    
    SetTargetFPS(15);
    
    
    
    while(!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
            generateGrid(grid, GetRandomValue(0, 255), rate);
        if (IsKeyPressed(KEY_TAB))
            isDarkMode = isDarkMode ? 0 : 1; 

        BeginDrawing();
            printGrid(grid, isDarkMode);
            //draw grid lines
            for (int i = scalar; i < SCREEN_WIDTH; i += scalar)
            {
                DrawLine(i, 0, i, SCREEN_HEIGHT, RAYWHITE);
                DrawLine(0, i, SCREEN_WIDTH, i, RAYWHITE);
            }
        EndDrawing();
        updateGrid(grid);
    }
    return 0;
}
