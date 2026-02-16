// g++ ulam-spiral.cpp -o main `pkg-config --libs --cflags sdl3`

/*
 To run the code, SDL3 library must be present in PATH
 I know there are a billion different ways this could be written more efficiently
 inside c++, this is just my way.
*/

// Include the header file and library for fps control
#include "generate_spiral.h"
#include <SDL3/SDL_render.h>
#include <unistd.h>

// Window consts
const int WINDOW_WIDTH{1920};
const int WINDOW_HEIGHT{1080};

// Declaring some variables to tell the spiral where to start from:
int posX = std::floor(WINDOW_WIDTH / 2);
int posY = std::floor(WINDOW_HEIGHT / 2);

// Number of steps to increment it by:
int stepSize = 5;

// Declaring spiral an object
GenerateSpiral gen(posX, posY, stepSize);

/* Declaring a struct to help us easily create a new window and everything else that is
required for SDL to function
*/

struct SpiralWindow
{
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;

        int running = 1;

        SpiralWindow()
        {
                if (!SDL_Init(SDL_INIT_VIDEO))
                        SDL_Log("Erorr Initializing: %s", SDL_GetError());
                gWindow = SDL_CreateWindow("Ulam Spiral", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
                gRenderer = SDL_CreateRenderer(gWindow, "opengl");
        }

        ~SpiralWindow()
        {
                SDL_DestroyWindow(gWindow);
                SDL_DestroyRenderer(gRenderer);
                SDL_Quit();
        }

        void update()
        {
                SDL_Event event;

                while (SDL_PollEvent(&event))
                {
                        if (event.type == SDL_EVENT_QUIT)
                                running = 0;
                        if (event.type == SDL_EVENT_KEY_DOWN)
                        {
                                if (event.key.key == SDLK_X)
                                        running = 0;
                        }
                }
        }

        void begin()
        {

                // Make window white:
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
                SDL_RenderFillRect(gRenderer, nullptr);
                SDL_RenderClear(gRenderer);

                while (running)
                {

                        // Running the event loop:
                        update();

                        gen.drawSpiral(gRenderer);
                        SDL_RenderPresent(gRenderer);

                        gen.spiralMath();

                        // Capping the FPS to some value so the window won't flicker
                        usleep(30000);
                }
        }
};

int main()
{
        SpiralWindow ulamSpiral;
        ulamSpiral.begin();

        std::cout << "Program commenced!\n";

        return 0;
}
