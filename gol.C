#include "raylib.h"
#include <stdio.h>
#include <cstdint>
#define GRID_HEIGHT 120
#define GRID_WIDTH 120
#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 720

void clearGrid(long grid[GRID_HEIGHT][GRID_WIDTH])
{
    for (int i = 0; i < GRID_HEIGHT; i++)
        for (int j = 0; j < GRID_WIDTH; j++)
            grid[i][j] = 0;
}

void printGrid(long grid[GRID_HEIGHT][GRID_WIDTH], int isDarkMode) {
    Color color = isDarkMode ? RAYWHITE : BLACK;
    Color backgroundColor = isDarkMode ? BLACK : RAYWHITE;
    
    int scalarX = SCREEN_WIDTH / GRID_WIDTH;
    int scalarY = SCREEN_HEIGHT / GRID_HEIGHT;
    int commonScalar = scalarX + scalarY;
    
    ClearBackground(backgroundColor);
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) 
        {
            
            if (grid[i][j] == 0)      // if the cell is not alive, don't draw it
            {
                continue;
            }
            
            DrawRectangle(//j * commonScalar, // X 
                          j * scalarX,  
                          
                          //i * commonScalar, // Y
                          i * scalarY,
                          
                          //commonScalar,     // width
                          scalarX,        // uncomment this line and comment the previous one for an atari look
                          
                          //commonScalar,     // height
                          scalarY,      
                          
                          color         // color, default: black (if darkmode: raywhite)
                          );       
        }
    }
    

}

void updateGrid(long grid[GRID_HEIGHT][GRID_WIDTH]) {
    int tempGrid[GRID_HEIGHT][GRID_WIDTH];

    // foreach row in the grid
    for (int i = 0; i < GRID_HEIGHT; i++) {
        
        //foreach cell in the row
        for (int j = 0; j < GRID_WIDTH; j++) {
            
            int aliveNeighbors = 0;

            for (int y = i-1; y <= i+1; y++)
            {   
                // if the neighbor is outside the border, keep looping
                if (y < 0 || y > GRID_HEIGHT){continue;}
                
                for (int x = j-1; x <= j+1; x++)
                {
                    // if the neighbor is outside the border, keep looping
                    if (x < 0 || x > GRID_WIDTH){continue;}
                    aliveNeighbors += grid[y][x];
                }
            }
            
            // taking its own state from the neighbors
            aliveNeighbors -= grid[i][j];
            
            // game of life rules
            if (aliveNeighbors < 2 || aliveNeighbors > 3)
                tempGrid[i][j] = 0;
            else if (aliveNeighbors == 3)
                tempGrid[i][j] = 1;
            else 
                tempGrid[i][j] = grid[i][j];
        }
    }

    // update grid
    for (int i = 0; i < GRID_HEIGHT; i++)
        for (int j = 0; j < GRID_WIDTH; j++)
            grid[i][j] = tempGrid[i][j];
}

void generateGrid(long grid[GRID_HEIGHT][GRID_WIDTH], unsigned int seed, float rate)
{
    SetRandomSeed(seed);
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            grid[i][j] = GetRandomValue(0, 1);
        }
    }
}

void generatePulsar(long grid[GRID_HEIGHT][GRID_WIDTH], int x, int y)
{
    
    char pulsar[14][14] = {"..OOO...OOO..", ".............", "O....O.O....O", "O....O.O....O", "O....O.O....O", "..OOO...OOO..", ".............", "..OOO...OOO..", "O....O.O....O", "O....O.O....O", "O....O.O....O", ".............", "..OOO...OOO.."};
    
    
    for (int i = 0; i < 13; i++ )
        for (int j = 0; j < 13; j++)
        {
            if (pulsar[i][j] == 'O')
                grid[i+y][j+x] = 1;
        }
}

void generateGlider(long grid[GRID_HEIGHT][GRID_WIDTH])
{   
    grid[1][0] = 1;
    grid[2][1] = 1;
    grid[0][2] = 1;
    grid[1][2] = 1;
    grid[2][2] = 1;
}

int main(void)
{
    unsigned int seed = 69;
    long grid[GRID_HEIGHT][GRID_WIDTH] = {0};
    float rate = 0.5f;
    int FPS = 10; 
    int isDarkMode = 0;
    int shouldShowHelp = 0;
       
    
    //generateGlider(grid)
    //generatePulsar(grid, 20, 20);   
    generateGrid(grid, seed, rate);
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
    
    SetTargetFPS(FPS);
    
    
    while(!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP) && (FPS <= 30))
        {
            FPS++;
            SetTargetFPS(FPS);
        } 
        if (IsKeyPressed(KEY_DOWN) && (FPS >= 5))
        {
            FPS--;
            SetTargetFPS(FPS);
        }
        
        if (IsKeyPressed(KEY_SPACE))
            generateGrid(grid, GetRandomValue(0, 100000), rate);
        if (IsKeyPressed(KEY_TAB))
            isDarkMode = isDarkMode ? 0 : 1; 

        if (IsKeyPressed(KEY_ONE))
            generateGlider(grid);
        if (IsKeyPressed(KEY_TWO))
            generatePulsar(grid, 2, 2);
        if (IsKeyPressed(KEY_BACKSPACE))
            clearGrid(grid);
        if (IsKeyPressed(KEY_F1))
            shouldShowHelp = shouldShowHelp ? 0 : 1;
        
        BeginDrawing();
            printGrid(grid, isDarkMode);
            DrawText("F1: Show help", 10, SCREEN_HEIGHT - 20, 20, isDarkMode ? WHITE : BLACK);
            
            if (shouldShowHelp)
            {
                DrawText("2 to spawn a pulsar", 10, SCREEN_HEIGHT - 40, 20, isDarkMode ? WHITE : BLACK);
                DrawText("1 to spawn a glider", 10, SCREEN_HEIGHT - 60, 20, isDarkMode ? WHITE : BLACK);
                DrawText("TAB: dark mode", 10, SCREEN_HEIGHT - 80, 20, isDarkMode ? WHITE : BLACK);
                DrawText("SPACE: restart", 10, SCREEN_HEIGHT - 100, 20, isDarkMode ? WHITE : BLACK);
                DrawText("UP or DOWN: change speed", 10, SCREEN_HEIGHT - 120, 20, isDarkMode ? WHITE : BLACK);
                DrawText("BACKSPACE: clear grid ", 10, SCREEN_HEIGHT - 140, 20, isDarkMode ? WHITE : BLACK);
                DrawText("ESC: close game", 10, SCREEN_HEIGHT - 160, 20, isDarkMode ? WHITE : BLACK);
            }
            
        EndDrawing();
        
        updateGrid(grid);
    }
    return 0;
}
