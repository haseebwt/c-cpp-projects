#include <cstdlib>
#include <ctime>
#include <iostream>
#include <raylib.h>
#include <vector>

#define WIDTH 1280
#define HEIGHT 720
#define RES 20
#define COLS WIDTH / RES
#define ROWS WIDTH / RES

std::vector<std::vector<int>> make2DArray ();
bool withinBounds (int val, int counter);
std::vector<std::vector<int>> initCells (std::vector<std::vector<int>> grid,
                                         int amount);
int countNeighbors (std::vector<std::vector<int>> *grid, int mX,
                    int mY); // m = local idk

int
main ()
{
  srand (time (nullptr));

  std::vector<std::vector<int>> grid = make2DArray ();
  std::vector<std::vector<int>> newGrid;

  InitWindow (WIDTH, HEIGHT, "Game of Life");
  SetTargetFPS (30);

  grid = initCells (grid, 300);

  bool isRunning = true;
  while (isRunning)
    {
      if (IsKeyPressed (KEY_X))
        {
          isRunning = false;
        }
      if (IsKeyPressed (KEY_SPACE))
        {
          grid = initCells (grid, 300);
        }

      if (IsMouseButtonDown (MOUSE_LEFT_BUTTON))
        {
          Vector2 mouseState = GetMousePosition ();

          int mX, mY;
          mX = mouseState.x / RES;
          mY = mouseState.y / RES;

          if (withinBounds (mouseState.x, WIDTH)
              && withinBounds (mouseState.y, HEIGHT) && withinBounds (mX, COLS)
              && withinBounds (mY, ROWS))
            {
              grid[mX][mY] = 1;
            }
        }

      BeginDrawing ();
      ClearBackground (DARKBLUE);
      for (int x = 0; x < COLS; x++)
        {
          for (int y = 0; y < ROWS; y++)
            {
              if (grid[x][y] > 0)
                {
                  DrawRectangle (x * RES, y * RES, RES, RES, PINK);
                }
              DrawRectangleLines (x * RES, y * RES, RES, RES,
                                  DARKGREEN); // stroke
            }
        }
      EndDrawing ();

      // run logic
      newGrid = make2DArray ();
      for (int x = 0; x < COLS; x++)
        {
          for (int y = 0; y < ROWS; y++)
            {
              int cell = grid[x][y];
              int neighbors = countNeighbors (&grid, x, y);
              if (neighbors < 2 || neighbors > 3)
                {
                  newGrid[x][y] = 0;
                }
              else if (cell == 0 && neighbors == 3)
                {
                  newGrid[x][y] = 1;
                }
              else
                {
                  newGrid[x][y] = cell;
                }
            }
        }
      grid = newGrid;
    }

  std::cout << "Game of Life\n";
  return 0;
}

std::vector<std::vector<int>>
make2DArray ()
{
  std::vector<std::vector<int>> vec (COLS, std::vector<int> (ROWS, 0));
  return vec;
}

bool
withinBounds (int val, int counter)
{
  return val >= 0 && val <= counter - 1;
}

std::vector<std::vector<int>>
initCells (std::vector<std::vector<int>> grid, int amount)
{

  // spawining some cells
  for (int i = 0; i < amount; i++)
    {
      int x, y;
      x = rand () % COLS - 1;
      y = rand () % ROWS - 1;

      if (withinBounds (x, WIDTH) && withinBounds (y, HEIGHT))
        {
          grid[x][y] = 1;
        }
    }
  return grid;
}

int
countNeighbors (std::vector<std::vector<int>> *grid, int mX, int mY)
{
  int aliveCount = 0;
  for (int i = -1; i < 2; i++)
    {
      for (int j = -1; j < 2; j++)
        {
          if (withinBounds (mX + i, COLS) && withinBounds (mY + j, ROWS))
            {
              int cell = grid->at (mX + i).at (mY + j);
              if (mX == i && mY == j)
                {
                  continue;
                }
              else if (cell > 0)
                {
                  aliveCount++;
                }
            }
        }
    }
  return aliveCount;
}
