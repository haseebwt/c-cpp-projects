// g++ image-viewer.cpp -o image-viewer `pkg-config --libs --cflags sdl3`

#include <SDL3/SDL.h>
#include <cstdio>
#include <iostream>
#include <string>

int WINDOW_WIDTH{0};
int WINDOW_HEIGHT{0};
std::string line;

int main()
{

        for (int i = 0; i < 3; i++)
        {
                std::getline(std::cin, line);
        }

        sscanf(line.c_str(), "%d %d", &WINDOW_WIDTH, &WINDOW_HEIGHT);

        // Get the max rgb value
        std::getline(std::cin, line);

        // Initialize the window and the surface
        SDL_Window *pWindow = SDL_CreateWindow("Image Viewer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
        SDL_Surface *pSurface = SDL_GetWindowSurface(pWindow);

        // Making variables to store value in
        Uint8 r, g, b = 0;

        // Getting pixel format details for making an RGB color
        SDL_PixelFormatDetails pFormatDetails = *SDL_GetPixelFormatDetails(pSurface->format);

        // Making the color
        Uint32 iColor = 0;

        // Initializing a rect that behaves as a pixel:
        SDL_Rect pixel = (SDL_Rect){0, 0, 1, 1};

        // Looping over image width and height to draw at each pixel:
        for (int j = 0; j < WINDOW_HEIGHT; j++)
        {
                for (int i = 0; i < WINDOW_WIDTH; i++)
                {

                        // Getting the character from stdin and assigning the value to r, g, and b
                        r = (char)getchar();
                        g = (char)getchar();
                        b = (char)getchar();

                        // Assigning the new color
                        iColor = SDL_MapRGB(&pFormatDetails, nullptr, r, g, b);

                        // Setting the pixel location
                        pixel.x = i;
                        pixel.y = j;

                        // Coloring the surface
                        SDL_FillSurfaceRect(pSurface, &pixel, iColor);
                }
        }

        // // Finally displaying the surface aka image:
        SDL_UpdateWindowSurface(pWindow);

        SDL_Event event;
        int running = 1;

        while (running)
        {
                while (SDL_PollEvent(&event))
                {
                        // Display until the x button pressed:
                        if (event.type == SDL_EVENT_QUIT)
                        {
                                running = 0;
                        }
                }
        }

        // Quit SDL
        SDL_Quit();
        return 0;
}
