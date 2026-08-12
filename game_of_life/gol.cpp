#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_stdinc.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#define WIDTH 620
#define HEIGHT 620
#define RES 15
#define COLS WIDTH / RES
#define ROWS HEIGHT / RES

void initCells ();

bool withinBounds (int val, int counter);

struct Cell
{
  int gX = 0, gY = 0;
  bool alive = false;
  SDL_Rect pos;

  Cell (int x, int y, bool isAlive)
  {
    gX = x;
    gY = y;
    alive = isAlive;
    pos = SDL_Rect{ 0, 0, RES, RES };
  }

  void
  show (SDL_Surface *mSurface)
  {
    // Uint32 teal = SDL_MapRGB (SDL_GetPixelFormatDetails (mSurface->format),
    // nullptr, 109, 249, 156);
    Uint32 white = SDL_MapRGB (SDL_GetPixelFormatDetails (mSurface->format),
                               nullptr, 255, 255, 255);
    // pos = SDL_Rect{ gX * RES, gY * RES, RES, RES
    // };
    pos.x = gX * RES;
    pos.y = gY * RES;
    SDL_FillSurfaceRect (mSurface, &pos, white);
  }
};

int checkAlive (Cell cell, std::vector<std::vector<Cell>> grid, int mX,
                int mY);

std::vector<std::vector<Cell>>
make2DArray ()
{
  std::vector<std::vector<Cell>> vec (
      COLS, std::vector<Cell> (ROWS, Cell (-1, -1, false)));
  return vec;
}

int
main ()
{
  srand (time (nullptr));
  if (!SDL_Init (SDL_INIT_VIDEO))
    {
      SDL_Log ("Init error: %s", SDL_GetError ());
    }

  SDL_Window *gWindow;
  SDL_Renderer *gRenderer;
  SDL_CreateWindowAndRenderer ("Life of Game", WIDTH, HEIGHT, 0, &gWindow,
                               &gRenderer);
  SDL_Surface *gSurface = SDL_GetWindowSurface (gWindow);

  std::vector<std::vector<Cell>> grid = make2DArray ();

  if (!gWindow || !gRenderer)
    {
      SDL_Log ("Creation error: %s", SDL_GetError ());
    }

  bool isRunning{ true };
  SDL_Event event;

  // for (int i = 0; i < 10; i++)
  //   {
  //     int mX = rand () % COLS - 1;
  //     int mY = rand () % ROWS - 1;
  //     if (withinBounds (mX, WIDTH) && withinBounds (mY, HEIGHT)
  //         && withinBounds (mX / RES, COLS) && withinBounds (mY / ROWS,
  //         ROWS))
  //       {
  //         Cell cell = Cell (mX, mY, true);
  //         grid[mX][mY] = cell;
  //       }
  //   }

  while (isRunning)
    {
      if (SDL_PollEvent (&event))
        {
          if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_X)
            {
              isRunning = false;
            }

          if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN
              && event.button.button == 1)
            {
              float mX = 0, mY = 0;
              SDL_GetMouseState (&mX, &mY);
              if (withinBounds (mX, WIDTH) && withinBounds (mY, HEIGHT)
                  && withinBounds (mX / RES, COLS)
                  && withinBounds (mY / ROWS, ROWS))
                {
                  mX = mX / RES;
                  mY = mY / RES;

                  Cell cell = Cell (mX, mY, true);
                  grid[mX][mY] = cell;
                }
            }
        }

      for (int x = 0; x < COLS; x++)
        {
          for (int y = 0; y < ROWS; y++)
            {
              Cell cell = grid[x][y];
              if (cell.alive)
                {
                  cell.show (gSurface);
                }
            }
        }

      for (int x = 0; x < COLS; x++)
        {
          for (int y = 0; y < ROWS; y++)
            {
              if (grid[x][y].alive)
                {
                  int aliveCount = checkAlive (grid[x][y], grid, x, y);
                  if (aliveCount == 3)
                    {
                      std::cout << "check\n";
                    }
                  if (aliveCount < 2)
                    {
                      grid[x][y].alive = false;
                      ;
                    }
                  if (aliveCount > 3)
                    {
                      grid[x][y].alive = false;
                    }
                  if (aliveCount == 3)
                    {
                      grid[x][y].alive = true;
                      std::cout << grid[x][y].alive << "\n";
                    }
                }
            }
        }

      SDL_UpdateWindowSurface (gWindow);
    }
  std::cout << "Hello World!\n";
  return 0;
}

bool
withinBounds (int val, int counter)
{
  return val >= 0 && val <= counter - 1;
}

int
checkAlive (Cell cell, std::vector<std::vector<Cell>> grid, int mX, int mY)
{
  int aliveCount = 0;
  int matrix{ 1 }, extent{ 2 };
  std::cout << mX << " " << mY << "\n";
  for (int i = -extent; i < matrix; i++)
    {
      for (int j = -extent; j < matrix; j++)
        {
          if (withinBounds (mX + i, COLS) && withinBounds (mY + j, ROWS))

            {
              Cell c = grid[mX + i][mY + j];
              std::cout << mX + i << " " << mY + j << "\n";
              if (c.gX == mX && c.gY == mY)
                {
                  continue;
                }
              else if (c.alive)
                {
                  aliveCount++;
                }
            }
        }
    }
  return aliveCount - 1;
}
